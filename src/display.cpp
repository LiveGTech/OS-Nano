/*
    LiveG OS Nano

    Copyright (C) LiveG. All Rights Reserved.

    https://liveg.tech/os
    Licensed by the LiveG Open-Source Licence, which can be found at LICENCE.md.
*/

#ifdef GOSN_SIMULATOR
    #include <emscripten.h>
    #include <emscripten/bind.h>
#endif

#include "config.h"
#include "display.h"
#include "datatypes.h"
#include "fs.h"

static lv_disp_draw_buf_t drawBuffer;
static lv_color_t buffer[GOSN_SCREEN_WIDTH * 10];

lv_font_t* display::FONT_MAIN_20;
lv_font_t* display::FONT_NUMERALS_32;
lv_font_t* display::FONT_NUMERALS_64;

#ifndef GOSN_SIMULATOR
    TFT_eSPI tft = TFT_eSPI(GOSN_SCREEN_WIDTH, GOSN_SCREEN_HEIGHT);

    CST816S touch(8, 9, 10, 7);
#endif

bool display::touchIsDown = false;
unsigned int display::touchX = 0;
unsigned int display::touchY = 0;

#ifdef GOSN_SIMULATOR
    EM_JS(void, sendDisplayDataToSimulator, (uint8_t* dataPtr, uint32_t x1, uint32_t y1, uint32_t width, uint32_t height), {
        var length = width * height * 4;
        var buffer = new Uint8ClampedArray(HEAPU8.buffer.slice(dataPtr), 0, length);

        var context = document.querySelector("#osNanoDisplay").getContext("2d");
        var imageData = new ImageData(buffer, width, height);

        context.putImageData(imageData, x1, y1);
    });
#endif

void flush(lv_disp_drv_t* displayDriver, const lv_area_t* renderArea, lv_color_t* colours) {
    uint32_t width = renderArea->x2 - renderArea->x1 + 1;
    uint32_t height = renderArea->y2 - renderArea->y1 + 1;

    #ifndef GOSN_SIMULATOR
        tft.startWrite();
        tft.setAddrWindow(renderArea->x1, renderArea->y1, width, height);
        tft.pushColors((uint16_t*)&colours->full, width * height, true);
        tft.endWrite();
    #else
        uint8_t rgba8888ChannelData[width * height * 4];

        for (uint32_t y = 0; y < height; y++) {
            for (uint32_t x = 0; x < width; x++) {
                uint32_t i = (y * width) + x;
                uint32_t j = i * 4;
                uint16_t rgb565 = ((uint16_t*)&colours->full)[i];

                uint8_t r565 = (rgb565 & 0b1111100000000000) >> 11;
                uint8_t g565 = (rgb565 & 0b0000011111100000) >> 5;
                uint8_t b565 = (rgb565 & 0b0000000000011111) >> 0;

                rgba8888ChannelData[j + 0] = ((r565 * 527) + 23) >> 6;
                rgba8888ChannelData[j + 1] = ((g565 * 259) + 33) >> 6;
                rgba8888ChannelData[j + 2] = ((b565 * 527) + 23) >> 6;
                rgba8888ChannelData[j + 3] = 255;
            }
        }

        sendDisplayDataToSimulator(rgba8888ChannelData, renderArea->x1, renderArea->y1, width, height);
    #endif

    lv_disp_flush_ready(displayDriver);
}

void readTouch(lv_indev_drv_t* indev_driver, lv_indev_data_t* data) {
    if (!display::touchIsDown) {
        data->state = LV_INDEV_STATE_REL;
    } else {
        data->state = LV_INDEV_STATE_PR;

        data->point.x = (uint16_t)display::touchX;
        data->point.y = (uint16_t)display::touchY;
    }
}

bool fsReadyCallback(lv_fs_drv_t* fsDriver) {
    return true;
}

void* fsOpenCallback(lv_fs_drv_t* fsDriver, const char* path, lv_fs_mode_t mode) {
    fs::FileMode nativeMode;

    switch (mode) {
        case LV_FS_MODE_RD:
            nativeMode = fs::FileMode::READ;
            break;

        case LV_FS_MODE_WR:
            nativeMode = fs::FileMode::WRITE;
            break;

        default:
            return nullptr;
    }

    return (void*)fs::open(path, nativeMode);
}

lv_fs_res_t fsCloseCallback(lv_fs_drv_t* fsDriver, void* file) {
    ((fs::FileHandle*)file)->close();

    return LV_FS_RES_OK;
}

lv_fs_res_t fsReadCallback(lv_fs_drv_t* fsDriver, void* file, void* buffer, uint32_t bytesToRead, uint32_t* actualBytesRead) {
    Count i = 0;

    auto nativeFile = (fs::FileHandle*)file;

    while (bytesToRead > 0) {
        if (!nativeFile->isAvailable()) {
            return LV_FS_RES_OK;
        }

        char c = nativeFile->read();

        ((char*)buffer)[i++] = c;

        bytesToRead--;
        (*actualBytesRead)++;
    }

    return LV_FS_RES_OK;
}

lv_fs_res_t fsSeekCallback(lv_fs_drv_t* fsDriver, void* file, uint32_t pos, lv_fs_whence_t whence) {
    auto nativeFile = (fs::FileHandle*)file;

    if (!nativeFile->isAvailable()) {
        return LV_FS_RES_OK;
    }

    switch (whence) {
        case LV_FS_SEEK_SET:
            nativeFile->seek(pos, fs::SeekOrigin::START);
            break;

        case LV_FS_SEEK_CUR:
            nativeFile->seek(pos, fs::SeekOrigin::CURRENT);
            break;

        case LV_FS_SEEK_END:
            nativeFile->seek(pos, fs::SeekOrigin::END);
            break;
    }

    return LV_FS_RES_OK;
}

lv_fs_res_t fsTellCallback(lv_fs_drv_t* fsDriver, void* file, uint32_t* posPtr) {
    auto nativeFile = (fs::FileHandle*)file;

    if (!nativeFile->isAvailable()) {
        return LV_FS_RES_OK;
    }

    *posPtr = nativeFile->tell();

    return LV_FS_RES_OK;
}

bool display::init() {
    lv_init();

    #ifndef GOSN_SIMULATOR
        tft.begin();
        tft.setRotation(0);

        touch.begin();
    #endif

    lv_disp_draw_buf_init(&drawBuffer, buffer, NULL, GOSN_SCREEN_WIDTH * 10);

    static lv_disp_drv_t displayDriver;

    lv_disp_drv_init(&displayDriver);

    displayDriver.hor_res = GOSN_SCREEN_WIDTH;
    displayDriver.ver_res = GOSN_SCREEN_HEIGHT;
    displayDriver.flush_cb = flush;
    displayDriver.draw_buf = &drawBuffer;

    lv_disp_drv_register(&displayDriver);

    static lv_indev_drv_t touchDriver;

    lv_indev_drv_init(&touchDriver);

    touchDriver.type = LV_INDEV_TYPE_POINTER;
    touchDriver.read_cb = readTouch;

    lv_indev_drv_register(&touchDriver);

    static lv_fs_drv_t fsDriver;

    lv_fs_drv_init(&fsDriver);

    // We can implement all of these but some are not needed at the moment
    fsDriver.letter = 'S';
    fsDriver.ready_cb = fsReadyCallback;
    fsDriver.open_cb = fsOpenCallback;
    fsDriver.close_cb = fsCloseCallback;
    fsDriver.read_cb = fsReadCallback;
    fsDriver.write_cb = NULL;
    fsDriver.seek_cb = fsSeekCallback;
    fsDriver.tell_cb = fsTellCallback;
    fsDriver.dir_open_cb = NULL;
    fsDriver.dir_read_cb = NULL;
    fsDriver.dir_close_cb = NULL;

    lv_fs_drv_register(&fsDriver);

    FONT_MAIN_20 = lv_font_load("S:/system/fonts/main-20.gosnf");
    FONT_NUMERALS_32 = lv_font_load("S:/system/fonts/numerals-32.gosnf");
    FONT_NUMERALS_64 = lv_font_load("S:/system/fonts/numerals-64.gosnf");

    Serial.println("Font loaded");

    return true;
}

void display::update(unsigned int millisecondsPassed) {
    #ifndef GOSN_SIMULATOR
        if (touch.available()) {
            touchIsDown = touch.data.event != 1;
            touchX = touch.data.x;
            touchY = touch.data.y;
        }
    #endif

    lv_timer_handler();
    lv_tick_inc(millisecondsPassed);
}

void display::setTouchData(bool isDown, unsigned int x, unsigned int y) {
    touchIsDown = isDown;
    touchX = x;
    touchY = y;
}

#ifdef GOSN_SIMULATOR
    EMSCRIPTEN_BINDINGS(my_module) {
        emscripten::function("display_setTouchData", &display::setTouchData);
    }
#endif
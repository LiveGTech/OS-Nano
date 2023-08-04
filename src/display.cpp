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

static lv_disp_draw_buf_t drawBuffer;
static lv_color_t buffer[GOSN_SCREEN_WIDTH * 10];

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

void display::init() {
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
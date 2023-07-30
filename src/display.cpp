/*
    LiveG OS Nano

    Copyright (C) LiveG. All Rights Reserved.

    https://liveg.tech/os
    Licensed by the LiveG Open-Source Licence, which can be found at LICENCE.md.
*/

#include "config.h"
#include "display.h"

using namespace display;

static lv_disp_draw_buf_t drawBuffer;
static lv_color_t buffer[GOSN_SCREEN_WIDTH * 10];

#ifndef GOSN_SIMULATOR
    TFT_eSPI tft = TFT_eSPI(GOSN_SCREEN_WIDTH, GOSN_SCREEN_HEIGHT);

    CST816S touch(8, 9, 10, 7);
#endif

bool display::touchIsDown = false;
unsigned int display::touchX = 0;
unsigned int display::touchY = 0;

void flush(lv_disp_drv_t* displayDriver, const lv_area_t* renderArea, lv_color_t* colours) {
    uint32_t width = renderArea->x2 - renderArea->x1 + 1;
    uint32_t height = renderArea->y2 - renderArea->y1 + 1;

    #ifndef GOSN_SIMULATOR
        tft.startWrite();
        tft.setAddrWindow(renderArea->x1, renderArea->y1, width, height);
        tft.pushColors((uint16_t*)&colours->full, width * height, true);
        tft.endWrite();
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
            display::touchIsDown = touch.data.event != 1;
            display::touchX = touch.data.x;
            display::touchY = touch.data.y;
        }
    #endif

    lv_timer_handler();
    lv_tick_inc(millisecondsPassed);
}
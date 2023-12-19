/*
    LiveG OS Nano

    Copyright (C) LiveG. All Rights Reserved.

    https://liveg.tech/os
    Licensed by the LiveG Open-Source Licence, which can be found at LICENCE.md.
*/

#ifndef DISPLAY_H_
#define DISPLAY_H_

#include <lvgl.h>

#ifndef GOSN_SIMULATOR
    #include <TFT_eSPI.h>
    #include <CST816S.h>
#endif

#include "config.h"
#include "timing.h"

namespace display {
    extern lv_font_t* FONT_MAIN_20;
    extern lv_font_t* FONT_NUMERALS_32;
    extern lv_font_t* FONT_NUMERALS_64;

    extern bool touchIsDown;
    extern unsigned int touchX;
    extern unsigned int touchY;

    bool init();
    void update(timing::Duration millisecondsPassed);

    void setTouchData(bool isDown, unsigned int x, unsigned int y);
}

#endif
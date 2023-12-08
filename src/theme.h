/*
    LiveG OS Nano

    Copyright (C) LiveG. All Rights Reserved.

    https://liveg.tech/os
    Licensed by the LiveG Open-Source Licence, which can be found at LICENCE.md.
*/

#ifndef THEME_H_
#define THEME_H_

#include <lvgl.h>

namespace theme {
    extern lv_font_t* FONT_MAIN_20;
    extern lv_font_t* FONT_NUMERALS_32;
    extern lv_font_t* FONT_NUMERALS_64;

    bool init();
}

#endif
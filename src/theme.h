/*
    LiveG OS Nano

    Copyright (C) LiveG. All Rights Reserved.

    https://liveg.tech/os
    Licensed by the LiveG Open-Source Licence, which can be found at LICENCE.md.
*/

#ifndef THEME_H_
#define THEME_H_

#include <lvgl.h>

#define REM(em) theme::remToPixels(em)

namespace theme {
    typedef int Pixels;
    typedef float EmUnits;

    extern lv_font_t* FONT_MAIN_20;
    extern lv_font_t* FONT_NUMERALS_32;
    extern lv_font_t* FONT_NUMERALS_64;

    bool init();
    Pixels remToPixels(EmUnits em);
}

#endif
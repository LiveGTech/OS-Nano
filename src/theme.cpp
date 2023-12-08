/*
    LiveG OS Nano

    Copyright (C) LiveG. All Rights Reserved.

    https://liveg.tech/os
    Licensed by the LiveG Open-Source Licence, which can be found at LICENCE.md.
*/

#include "theme.h"

lv_font_t* theme::FONT_MAIN_20;
lv_font_t* theme::FONT_NUMERALS_32;
lv_font_t* theme::FONT_NUMERALS_64;

bool theme::init() {
    FONT_MAIN_20 = lv_font_load("S:/system/fonts/main-20.gosnf");
    FONT_NUMERALS_32 = lv_font_load("S:/system/fonts/numerals-32.gosnf");
    FONT_NUMERALS_64 = lv_font_load("S:/system/fonts/numerals-64.gosnf");

    return true;
}
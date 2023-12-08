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

lv_style_t globalStyle;

void applyStyles(lv_theme_t* theme, lv_obj_t* object) {
    lv_obj_add_style(object, &globalStyle, 0);
}

bool theme::init() {
    FONT_MAIN_20 = lv_font_load("S:/system/fonts/main-20.gosnf");
    FONT_NUMERALS_32 = lv_font_load("S:/system/fonts/numerals-32.gosnf");
    FONT_NUMERALS_64 = lv_font_load("S:/system/fonts/numerals-64.gosnf");

    lv_style_init(&globalStyle);
    lv_style_set_text_font(&globalStyle, theme::FONT_MAIN_20);

    lv_theme_t* defaultTheme = lv_disp_get_theme(NULL);
    static lv_theme_t newTheme = *defaultTheme;

    lv_theme_set_parent(&newTheme, defaultTheme);
    lv_theme_set_apply_cb(&newTheme, applyStyles);
    lv_disp_set_theme(nullptr, &newTheme);

    return true;
}
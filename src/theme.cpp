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
lv_style_t containerStyle;
lv_style_t buttonStyle;
lv_style_t buttonPressedStyle;

void applyStyles(lv_theme_t* theme, lv_obj_t* object) {
    lv_obj_add_style(object, &globalStyle, 0);

    if (lv_obj_check_type(object, &lv_obj_class)) {
        lv_obj_add_style(object, &containerStyle, 0);
    }

    if (lv_obj_check_type(object, &lv_btn_class)) {
        lv_obj_remove_style_all(object);
        lv_obj_add_style(object, &buttonStyle, 0);
        lv_obj_add_style(object, &buttonPressedStyle, LV_STATE_PRESSED);
    }
}

bool theme::init() {
    FONT_MAIN_20 = lv_font_load("S:/system/fonts/main-20.gosnf");
    FONT_NUMERALS_32 = lv_font_load("S:/system/fonts/numerals-32.gosnf");
    FONT_NUMERALS_64 = lv_font_load("S:/system/fonts/numerals-64.gosnf");

    lv_style_init(&globalStyle);
    lv_style_set_text_font(&globalStyle, theme::FONT_MAIN_20);

    lv_style_init(&containerStyle);
    lv_style_set_border_width(&containerStyle, 0);
    lv_style_set_pad_all(&containerStyle, 0);
    lv_style_set_radius(&containerStyle, 0);
    lv_style_set_bg_opa(&containerStyle, LV_OPA_TRANSP);

    lv_style_init(&buttonStyle);
    lv_style_set_pad_all(&buttonStyle, REM(0.5));
    lv_style_set_pad_hor(&buttonStyle, REM(1));
    lv_style_set_bg_color(&buttonStyle, lv_palette_main(LV_PALETTE_BLUE));
    lv_style_set_bg_opa(&buttonStyle, LV_OPA_100);
    lv_style_set_text_color(&buttonStyle, lv_color_white());
    lv_style_set_radius(&buttonStyle, REM(0.75));

    static lv_style_transition_dsc_t buttonTransition;
    static lv_style_prop_t buttonTransitionProps[] = {LV_STYLE_BG_COLOR, static_cast<lv_style_prop_t>(0)};

    lv_style_transition_dsc_init(&buttonTransition, buttonTransitionProps, lv_anim_path_linear, 500, 0, nullptr);
    lv_style_set_transition(&buttonStyle, &buttonTransition);

    lv_style_init(&buttonPressedStyle);
    lv_style_set_bg_color(&buttonPressedStyle, lv_palette_lighten(LV_PALETTE_BLUE, 2));

    lv_theme_t* defaultTheme = lv_disp_get_theme(NULL);
    static lv_theme_t newTheme = *defaultTheme;

    lv_theme_set_parent(&newTheme, defaultTheme);
    lv_theme_set_apply_cb(&newTheme, applyStyles);
    lv_disp_set_theme(nullptr, &newTheme);

    return true;
}

theme::Pixels theme::remToPixels(EmUnits em) {
    return em * 12;
}
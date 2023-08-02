/*
    LiveG OS Nano

    Copyright (C) LiveG. All Rights Reserved.

    https://liveg.tech/os
    Licensed by the LiveG Open-Source Licence, which can be found at LICENCE.md.
*/

#include <Arduino.h>
#include <lvgl.h>

#ifdef GOSN_SIMULATOR
    #include <emscripten.h>
#endif

#include "config.h"
#include "datatypes.h"
#include "display.h"

static int i = 0;

static lv_obj_t* label;

static lv_obj_t* lineX;
static lv_obj_t* lineY;

static lv_point_t lineXPoints[] = {
    {0, 0},
    {GOSN_SCREEN_WIDTH - 1, GOSN_SCREEN_HEIGHT - 1}
};

static lv_point_t lineYPoints[] = {
    {GOSN_SCREEN_WIDTH - 1},
    {0, GOSN_SCREEN_HEIGHT - 1}
};

void updateCounterTimer(lv_timer_t* timer) {
    lv_label_set_text_fmt(label, "Count: %d", i);

    lineXPoints[0].x = display::touchX;
    lineXPoints[0].y = 0;
    lineXPoints[1].x = display::touchX;
    lineXPoints[1].y = GOSN_SCREEN_HEIGHT - 1;

    lineYPoints[0].x = 0;
    lineYPoints[0].y = display::touchY;
    lineYPoints[1].x = GOSN_SCREEN_WIDTH - 1;
    lineYPoints[1].y = display::touchY;
    
    lv_line_set_points(lineX, lineXPoints, 2);
    lv_line_set_points(lineY, lineYPoints, 2);

    i++;
}

void setup() {
    Serial.begin(115200);

    Serial.println("Hello, world!");

    display::init();

    label = lv_label_create(lv_scr_act());

    lv_label_set_long_mode(label, LV_LABEL_LONG_WRAP);
    lv_obj_set_width(label, lv_pct(80));
    lv_obj_set_style_text_align(label, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);
    lv_label_set_text(label, "LiveG OS Nano");

    static lv_style_t lineStyle;

    lv_style_init(&lineStyle);
    lv_style_set_line_width(&lineStyle, 1);
    lv_style_set_line_color(&lineStyle, lv_palette_main(LV_PALETTE_RED));

    lineX = lv_line_create(lv_scr_act());

    lv_obj_add_style(lineX, &lineStyle, 0);

    lineY = lv_line_create(lv_scr_act());

    lv_obj_add_style(lineY, &lineStyle, 0);

    lv_line_set_points(lineX, lineXPoints, 2);
    lv_line_set_points(lineY, lineYPoints, 2);

    lv_timer_create(updateCounterTimer, 5, NULL);

    Serial.println("Setup done");

    dataTypes::List<int> numbers;

    int thing = 2;

    numbers.push(store(1));
    numbers.push(&thing);
    numbers.push(store(3));

    numbers.forEach([] (int* valuePtr, Count index) {
        Serial.print(*valuePtr);
        Serial.print(" ");
    });

    Serial.println("");

    Serial.print("Last item popped: ");
    Serial.println(discard(numbers.pop()));

    // TODO: Fix memory out-of-bounds error

    discard(numbers.pop());

    numbers.push(store(3));
    numbers.push(store(5));

    numbers.forEach([] (int* valuePtr, Count index) {
        Serial.print(*valuePtr);
        Serial.print(" ");
    });

    Serial.println("");
}

void loop() {
    display::update(1);

    #ifndef GOSN_SIMULATOR
        delay(1);
    #endif
}

#ifdef GOSN_SIMULATOR
    #include <stdio.h>

    int main(int argc, char** argv) {
        setup();

        emscripten_set_main_loop(loop, 0, true);

        return 0;
    }
#endif
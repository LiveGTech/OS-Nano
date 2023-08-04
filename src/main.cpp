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
#include "proc.h"
#include "app.h"

static int i = 0;

double lastTimestamp = 0;

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

struct HelloTaskState {
    String value;
    Count count;
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

    discard(numbers.pop());

    numbers.push(store(3));
    numbers.push(store(5));

    numbers.forEach([] (int* valuePtr, Count index) {
        Serial.print(*valuePtr);
        Serial.print(" ");
    });

    Serial.println("");

    Serial.print("First item shifted: ");
    Serial.println(discard(numbers.shift()));

    numbers.unshift(store(1));

    numbers.forEach([] (int* valuePtr, Count index) {
        Serial.print(*valuePtr);
        Serial.print(" ");
    });

    Serial.println("");
    Serial.print("Length: ");
    Serial.println(numbers.length());

    numbers.insert(1, store(3));
    numbers.insert(3, store(7));

    numbers.forEach([] (int* valuePtr, Count index) {
        Serial.print(*valuePtr);
        Serial.print(" ");
    });

    Serial.println("");

    Serial.print("Removed at index 2: ");
    Serial.println(discard(numbers.remove(2)));

    numbers.forEach([] (int* valuePtr, Count index) {
        Serial.print(*valuePtr);
        Serial.print(" ");
    });

    Serial.println("");

    dataTypes::List<int> multipliedNumbers = numbers.map([] (int* valuePtr, Count index) {
        return store(*valuePtr * 2);
    });

    multipliedNumbers.forEach([] (int* valuePtr, Count index) {
        Serial.print(*valuePtr);
        Serial.print(" ");
    });

    Serial.println("");

    dataTypes::List<int> combinedNumbers = numbers.concat(multipliedNumbers);

    dataTypes::List<int> filteredNumbers = combinedNumbers.filter([] (int* valuePtr, Count index) {
        return *valuePtr < 7;
    });

    filteredNumbers.forEach([] (int* valuePtr, Count index) {
        Serial.print(*valuePtr);
        Serial.print(" ");
    });

    Serial.println("");

    auto processStatePtr1 = new HelloTaskState();

    processStatePtr1->value = "Testing!";
    processStatePtr1->count = 10;

    auto process1 = new proc::Process([] (proc::Process* processPtr) {
        auto state = ((HelloTaskState*)processPtr->taskState);

        Serial.println("Hello, world!");
        Serial.print("My state value: ");
        Serial.println(state->value);

        Serial.print("I'll run for ");
        Serial.print(state->count);
        Serial.println(" more scheduler cycles");

        if (state->count == 0) {
            processPtr->stopAndDiscard();

            return;
        }

        state->count--;
    }, processStatePtr1);

    Serial.print("Process 1 ID: ");
    Serial.println(process1->id());

    Serial.print("Process 1 status: ");
    Serial.println(process1->status());

    auto processStatePtr2 = new HelloTaskState();

    processStatePtr2->value = "Another process!";
    processStatePtr2->count = 0;

    auto process2 = new proc::Process([] (proc::Process* processPtr) {
        auto state = ((HelloTaskState*)processPtr->taskState);

        Serial.println("Hey, world!");
        Serial.print("My state value: ");
        Serial.println(state->value);

        Serial.print("I've been running for ");
        Serial.print(state->count);
        Serial.println(" scheduler cycles");

        if (state->count == 15) {
            processPtr->stopAndDiscard();

            return;
        }

        state->count++;
    }, processStatePtr2);

    Serial.print("Running process count: ");
    Serial.println(proc::getRunningProcessesCount());

    app::launch("hello");
}

void loop() {
    proc::cycleScheduler();

    #ifndef GOSN_SIMULATOR
        double currentTimestamp = millis();
    #else
        double currentTimestamp = emscripten_performance_now();
    #endif

    double timeDelta = currentTimestamp - lastTimestamp;

    display::update(timeDelta);

    #ifndef GOSN_SIMULATOR
        delay(1);
    #endif

    lastTimestamp = currentTimestamp;
}

#ifdef GOSN_SIMULATOR
    #include <stdio.h>

    int main(int argc, char** argv) {
        setup();

        emscripten_set_main_loop(loop, 0, true);

        return 0;
    }
#endif
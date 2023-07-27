/*
    LiveG OS Nano

    Copyright (C) LiveG. All Rights Reserved.

    https://liveg.tech/os
    Licensed by the LiveG Open-Source Licence, which can be found at LICENCE.md.
*/

#ifndef DISPLAY_H_
#define DISPLAY_H_

#include <lvgl.h>
#include <TFT_eSPI.h>
#include <CST816S.h>

#include "config.h"

namespace display {
    extern bool touchIsDown;
    extern unsigned int touchX;
    extern unsigned int touchY;

    void init();
    void update(unsigned int millisecondsPassed);
}

#endif
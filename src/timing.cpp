/*
    LiveG OS Nano

    Copyright (C) LiveG. All Rights Reserved.

    https://liveg.tech/os
    Licensed by the LiveG Open-Source Licence, which can be found at LICENCE.md.
*/

#include <Arduino.h>

#ifdef GOSN_SIMULATOR
    #include <emscripten.h>
#endif

#include "timing.h"

timing::Duration timing::getUptime() {
    #ifndef GOSN_SIMULATOR
        return millis();
    #else
        return emscripten_get_now();
    #endif
}

timing::Timestamp timing::getCurrentTime() {
    return getUptime();
}
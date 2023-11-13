/*
    LiveG OS Nano

    Copyright (C) LiveG. All Rights Reserved.

    https://liveg.tech/os
    Licensed by the LiveG Open-Source Licence, which can be found at LICENCE.md.
*/

#ifndef APP_H_
#define APP_H_

#include <Arduino.h>
#include <duktape.h>
#include <lvgl.h>

#include "datatypes.h"
#include "proc.h"

namespace app {
    enum ElementType {
        NONE = 0,
        SCREEN = 1,
        CONTAINER = 2,
        PARAGRAPH = 3
    };

    struct Element {
        ElementType type;
        lv_obj_t* object;
    };

    struct ProcessTaskState {
        String id;
        char* scriptCodeCharArray;
        duk_context* duktapeContextPtr;
        bool setupCompleted;
        dataTypes::List<Element> ownedElements;
    };

    proc::Process* getProcessFromDuktapeContext(duk_context* ctx);
    ProcessTaskState* getStateFromDuktapeContext(duk_context* ctx);
    bool init();
    proc::Process* launch(String id);
    void crash();
}

#endif
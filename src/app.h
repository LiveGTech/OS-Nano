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
#include "timing.h"

namespace app {
    enum ElementType {
        TYPE_NONE = 0,
        TYPE_SCREEN = 1,
        TYPE_CONTAINER = 2,
        TYPE_TEXT = 3,
        TYPE_PARAGRAPH = 4,
        TYPE_BUTTON = 5
    };

    enum ElementProperty {
        PROP_NONE = 0,
        PROP_SHOWING = 1,
        PROP_TEXT = 2
    };

    enum EventType {
        EVENT_TYPE_NONE = 0,
        EVENT_TYPE_CLICK = 1
    };

    struct Element {
        ElementType type;
        lv_obj_t* object;
        bool listeningForEvents;
    };

    struct EventData {
        EventType type;
        int targetId;
    };

    struct ProcessTaskState {
        String id;
        char* scriptCodeCharArray;
        duk_context* duktapeContextPtr;
        bool setupCompleted;
        timing::Timestamp startTimestamp;
        dataTypes::List<Element> ownedElements;
    };

    proc::Process* getProcessFromDuktapeContext(duk_context* ctx);
    ProcessTaskState* getStateFromDuktapeContext(duk_context* ctx);

    bool init();
    proc::Process* launch(String id);
    void crash();

    void dispatchEvent(EventData eventData);
    void dispatchEventHandler(lv_event_t* event);
}

#endif
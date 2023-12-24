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

    enum ElementState {
        STATE_NONE = 0,
        STATE_PRESS = 1
    };

    enum EventType {
        EVENT_TYPE_NONE = 0,
        EVENT_TYPE_CLICK = 1
    };

    enum StyleProperty {
        STYLE_NONE = 0,
        STYLE_BACKGROUND = 1,
        STYLE_FOREGROUND = 2
    };

    struct Element {
        ElementType type;
        lv_obj_t* object;
        bool listeningForEvents;
    };

    struct GlobalElementReference {
        proc::Process* processPtr;
        Count elementId;
    };

    struct EventData {
        EventType type;
        GlobalElementReference target;
    };

    struct ElementStyleRule {
        ElementType targetType;
        ElementState targetState;
        lv_style_t style;
    };

    struct ProcessTaskState {
        String id;
        char* scriptCodeCharArray;
        duk_context* duktapeContextPtr;
        bool setupCompleted;
        bool wantsToStop;
        timing::Timestamp startTimestamp;
        dataTypes::List<Element> ownedElements;
        Element* activeScreen;
        dataTypes::List<EventData> eventsToProcess;
        dataTypes::List<ElementStyleRule> elementStyleRules;
    };

    proc::Process* getProcessFromDuktapeContext(duk_context* ctx);
    ProcessTaskState* getStateFromDuktapeContext(duk_context* ctx);

    bool init();
    proc::Process* launch(String id, bool activate);
    proc::Process* getRunningProcess(String id);
    Element* switchToProcess(proc::Process* processPtr, bool switchToActiveScreen);
    proc::Process* getActiveProcess();
    Element* goBackToPreviousActiveProcess(bool switchToActiveScreen);
    proc::Process* launchOrSwitchToProcess(String id, bool activate);

    void dispatchEvent(EventData eventData);
    void dispatchEventHandler(lv_event_t* event);

    void applyStyleRuleToElement(ElementStyleRule* styleRule, Element* element);
}

#endif
/*
    LiveG OS Nano

    Copyright (C) LiveG. All Rights Reserved.

    https://liveg.tech/os
    Licensed by the LiveG Open-Source Licence, which can be found at LICENCE.md.
*/

#include <Arduino.h>
#include <duktape.h>

#include "common.h"
#include "datatypes.h"
#include "app.h"
#include "proc.h"
#include "fs.h"
#include "api.h"

char* apiCodeCharArray;
auto eventsToProcess = dataTypes::List<app::EventData>();

proc::Process* app::getProcessFromDuktapeContext(duk_context* ctx) {
    duk_memory_functions functions;

    duk_get_memory_functions(ctx, &functions);

    return (proc::Process*)functions.udata;
}

app::ProcessTaskState* app::getStateFromDuktapeContext(duk_context* ctx) {
    auto processPtr = app::getProcessFromDuktapeContext(ctx);

    return (app::ProcessTaskState*)processPtr->taskState;
}

void processTask(proc::Process* processPtr) {
    auto state = (app::ProcessTaskState*)processPtr->taskState;
    auto ctx = state->duktapeContextPtr;

    if (!state->setupCompleted) {
        if (duk_peval_string(ctx, apiCodeCharArray) != 0) {
            Serial.print("[");
            Serial.print(state->id);
            Serial.print("] ");
            Serial.print("API error: ");
            Serial.println(duk_safe_to_string(ctx, -1));
        }

        duk_pop(ctx);

        if (duk_peval_string(ctx, state->scriptCodeCharArray) != 0) {
            Serial.print("[");
            Serial.print(state->id);
            Serial.print("] ");
            Serial.print("Script error: ");
            Serial.println(duk_safe_to_string(ctx, -1));
        }

        duk_pop(ctx);

        state->setupCompleted = true;
    }

    duk_get_global_string(ctx, "_nano_nextTick");
    duk_push_number(ctx, timing::getCurrentTime() - state->startTimestamp);
    duk_call(ctx, 1);
    duk_pop(ctx);

    eventsToProcess.start();

    while (auto event = eventsToProcess.next()) {
        duk_get_global_string(ctx, "_nano_processEvent");
        duk_push_int(ctx, event->type);
        duk_push_int(ctx, event->targetId);
        duk_call(ctx, 2);
        duk_pop(ctx);

        discard<app::EventData>(event);
    }

    eventsToProcess.empty();
}

void processCleanupHandler(proc::Process* processPtr) {
    auto state = (app::ProcessTaskState*)processPtr->taskState;

    free(state->scriptCodeCharArray);
    duk_destroy_heap(state->duktapeContextPtr);
}

bool app::init() {
    auto file = fs::open("/system/api.min.js", fs::FileMode::MODE_READ);

    if (!file) {
        return false;
    }

    apiCodeCharArray = file->readCharArray();

    return true;
}

void* customMalloc(void* udata, duk_size_t size) {
    return MALLOC_PSRAM(size);
}

void* customRealloc(void* udata, void* ptr, duk_size_t size) {
    return REALLOC_PSRAM(ptr, size);
}

void customFree(void* udata, void* ptr) {
    free(ptr);
}

void fatalHandler(void* udata, const char* msg) {
    Serial.println(msg);
    Serial.flush();

    while (true) {} // TODO: Terminate process
}

proc::Process* app::launch(String id) {
    auto processTaskState = new app::ProcessTaskState();
    auto process = new proc::Process(processTask, processTaskState);

    auto ctx = duk_create_heap(customMalloc, customRealloc, customFree, process, fatalHandler);

    auto file = fs::open("/apps/" + id + "/app.js", fs::FileMode::MODE_READ);

    if (!file) {
        return nullptr;
    }

    processTaskState->id = id;
    processTaskState->scriptCodeCharArray = file->readCharArray();
    processTaskState->duktapeContextPtr = ctx;
    processTaskState->setupCompleted = false;
    processTaskState->startTimestamp = timing::getCurrentTime();
    processTaskState->ownedElements = dataTypes::List<app::Element>();

    duk_push_c_function(ctx, api::print, DUK_VARARGS);
    duk_put_global_string(ctx, "print");

    duk_push_c_function(ctx, api::timing_getCurrentTime, 0);
    duk_put_global_string(ctx, "_nano_timing_getCurrentTime");

    duk_push_c_function(ctx, api::addElement, 2);
    duk_put_global_string(ctx, "_nano_addElement");

    duk_push_c_function(ctx, api::removeElement, 2);
    duk_put_global_string(ctx, "_nano_removeElement");

    duk_push_c_function(ctx, api::setElementProp, 3);
    duk_put_global_string(ctx, "_nano_setElementProp");

    duk_push_c_function(ctx, api::listenForEvents, 1);
    duk_put_global_string(ctx, "_nano_listenForEvents");

    process->setCleanupHandler(processCleanupHandler);

    return process;
}

void app::dispatchEvent(app::EventData eventData) {
    eventsToProcess.push(store<app::EventData>(eventData));
}

void app::dispatchEventHandler(lv_event_t* event) {
    int elementId = *(static_cast<int*>(lv_event_get_user_data(event)));

    switch (lv_event_get_code(event)) {
        case LV_EVENT_CLICKED:
            dispatchEvent((EventData) {
                .type = EventType::EVENT_TYPE_CLICK,
                .targetId = elementId
            });
            break;

        default:
            break;
    }
}
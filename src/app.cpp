/*
    LiveG OS Nano

    Copyright (C) LiveG. All Rights Reserved.

    https://liveg.tech/os
    Licensed by the LiveG Open-Source Licence, which can be found at LICENCE.md.
*/

#include <Arduino.h>
#include <duktape.h>

#include "datatypes.h"
#include "app.h"
#include "proc.h"
#include "fs.h"

struct ProcessTaskState {
    String id;
    char* scriptCodeCharArray;
    duk_context* duktapeContextPtr;
};

proc::Process* getProcessFromDuktapeContext(duk_context* ctx) {
    duk_memory_functions functions;

    duk_get_memory_functions(ctx, &functions);

    return (proc::Process*)functions.udata;
}

void processTask(proc::Process* processPtr) {
    auto state = (ProcessTaskState*)processPtr->taskState;
    auto ctx = state->duktapeContextPtr;

    duk_eval_string_noresult(ctx, state->scriptCodeCharArray);

    processPtr->stopAndDiscard();
}

void processCleanupHandler(proc::Process* processPtr) {
    auto state = (ProcessTaskState*)processPtr->taskState;

    duk_destroy_heap(state->duktapeContextPtr);
}

duk_ret_t native_print(duk_context* ctx) {
    auto processPtr = getProcessFromDuktapeContext(ctx);
    auto state = (ProcessTaskState*)processPtr->taskState;

    duk_push_string(ctx, " ");
    duk_insert(ctx, 0);
    duk_join(ctx, duk_get_top(ctx) - 1);

    Serial.print("[");
    Serial.print(state->id);
    Serial.print("] ");
    Serial.println(duk_safe_to_string(ctx, -1));

    return 0;
}

proc::Process* app::launch(String id) {
    auto processTaskState = new ProcessTaskState();
    auto process = new proc::Process(processTask, processTaskState);

    auto ctx = duk_create_heap(NULL, NULL, NULL, process, NULL);

    auto file = fs::open("/apps/" + id + "/app.js", fs::FileMode::READ);

    if (!file) {
        return nullptr;
    }

    auto scriptCode = file->readString();

    delete file;

    const Count CODE_CHAR_ARRAY_LENGTH = scriptCode.length() + 1;

    processTaskState->scriptCodeCharArray = (char*)malloc(CODE_CHAR_ARRAY_LENGTH);

    scriptCode.toCharArray(processTaskState->scriptCodeCharArray, CODE_CHAR_ARRAY_LENGTH);

    processTaskState->id = id;
    processTaskState->duktapeContextPtr = ctx;

    duk_push_c_function(ctx, native_print, DUK_VARARGS);
    duk_put_global_string(ctx, "print");

    process->setCleanupHandler(processCleanupHandler);

    return process;
}
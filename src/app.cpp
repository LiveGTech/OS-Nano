/*
    LiveG OS Nano

    Copyright (C) LiveG. All Rights Reserved.

    https://liveg.tech/os
    Licensed by the LiveG Open-Source Licence, which can be found at LICENCE.md.
*/

#include <Arduino.h>

#include "app.h"
#include "proc.h"

void processTask(proc::Process* processPtr) {
    Serial.println("Hello, world, from an app!");

    processPtr->stopAndDiscard();
}

void app::launch(String id) {
    auto processTaskState = new _AppProcessTaskState();

    processTaskState->id = id;

    auto process = new proc::Process(processTask, processTaskState);
}
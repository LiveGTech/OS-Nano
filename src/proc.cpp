/*
    LiveG OS Nano

    Copyright (C) LiveG. All Rights Reserved.

    https://liveg.tech/os
    Licensed by the LiveG Open-Source Licence, which can be found at LICENCE.md.
*/

#include "proc.h"
#include "datatypes.h"

unsigned int nextId = 0;
auto processes = dataTypes::List<proc::Process>();

proc::Process::Process(void* taskPtr) {
    id = nextId++;
    status = proc::Status::RUNNING;
    _taskPtr = taskPtr;

    processes.push(this);
}

void proc::Process::stop() {
    status = proc::Status::STOPPED;
}

proc::Process::~Process() {
    status = proc::Status::STOPPED;
}
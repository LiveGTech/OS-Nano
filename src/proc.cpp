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

proc::Process::Process(ProcessTask processTask) {
    _id = nextId++;
    _status = proc::Status::STOPPED;
    _processTask = processTask;

    start();
}

unsigned int proc::Process::id() {
    return _id;
}

proc::Status proc::Process::status() {
    return _status;
}

void proc::Process::start() {
    if (_status == Status::RUNNING) {
        return;
    }

    _status = Status::RUNNING;

    processes.push(this);
}

void proc::Process::stop() {
    if (_status == Status::STOPPED) {
        return;
    }

    _status = proc::Status::STOPPED;

    processes.remove(processes.indexOf(this));
}

proc::Process::~Process() {
    stop();
}
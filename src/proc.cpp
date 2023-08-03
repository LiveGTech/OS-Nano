/*
    LiveG OS Nano

    Copyright (C) LiveG. All Rights Reserved.

    https://liveg.tech/os
    Licensed by the LiveG Open-Source Licence, which can be found at LICENCE.md.
*/

#include "proc.h"
#include "datatypes.h"

unsigned int nextId = 0;
auto runningProcesses = dataTypes::List<proc::Process>();
bool schedulerIsRunning = false;
auto processesToStop = dataTypes::List<proc::Process>();

proc::Process::Process(ProcessTask task, void* initialTaskState) {
    _id = nextId++;
    _status = proc::Status::STOPPED;
    _task = task;
    taskState = initialTaskState;

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

    runningProcesses.push(this);

    int processToStopIndex = processesToStop.indexOf(this);

    if (processToStopIndex >= 0) {
        processesToStop.remove(processToStopIndex);
    }
}

void proc::Process::stop() {
    if (_status == Status::STOPPED) {
        return;
    }

    if (schedulerIsRunning) {
        // Don't remove from running processes as list is still being processed by scheduler

        if (_status == Status::STOPPING) {
            return;
        }

        processesToStop.push(this);

        _status = proc::Status::STOPPING;
    } else {
        runningProcesses.remove(runningProcesses.indexOf(this));

        _status = proc::Status::STOPPED;

        if (taskState) {
            delete taskState;
        }
    }
}

void proc::Process::stopAndDiscard() {
    stop();

    delete this;
}

void proc::Process::run() {
    _task(this);
}

proc::Process::~Process() {
    stop();
}

void proc::cycleScheduler() {
    if (runningProcesses.length() == 0) {
        return;
    }

    schedulerIsRunning = true;

    runningProcesses.start();

    while (auto process = runningProcesses.next()) {
        process->run();
    }

    schedulerIsRunning = false;

    while (auto process = processesToStop.shift()) {
        process->stop();
    }
}

Count proc::getRunningProcessesCount() {
    return runningProcesses.length();
}

proc::Process* proc::getRunningProcessById(unsigned int id) {
    runningProcesses.start();

    while (auto process = runningProcesses.next()) {
        if (process->id() == id) {
            return process;
        }
    }

    return nullptr;
}
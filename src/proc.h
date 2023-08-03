/*
    LiveG OS Nano

    Copyright (C) LiveG. All Rights Reserved.

    https://liveg.tech/os
    Licensed by the LiveG Open-Source Licence, which can be found at LICENCE.md.
*/

#ifndef PROC_H_
#define PROC_H_

#include "datatypes.h"

namespace proc {
    enum Status {
        STOPPED,
        RUNNING,
        STOPPING
    };

    class Process {
        public:
            typedef void (*ProcessTask)(Process* processPtr);

            void* taskState;

            Process(ProcessTask task, void* initialTaskState);
            ~Process();

            unsigned int id();
            Status status();
            void start();
            void stop();
            void stopAndDiscard();
            void run();

        private:
            unsigned int _id;
            Status _status;
            ProcessTask _task;
    };

    void cycleScheduler();
    Count getRunningProcessesCount();
    Process* getRunningProcessById(unsigned int id);
}

#endif
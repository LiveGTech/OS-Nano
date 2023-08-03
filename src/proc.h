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
        RUNNING
    };

    class Process {
        public:
            typedef void (*ProcessTask)(Process process);

            Process(ProcessTask processTask);
            ~Process();

            unsigned int id();
            Status status();
            void start();
            void stop();

            unsigned int _id;
            Status _status;
            ProcessTask _processTask;
    };

    Count getRunningProcessesCount();
    Process* getRunningProcessById(unsigned int id);
}

#endif
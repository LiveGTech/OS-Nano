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
        RUNNING,
        STOPPED
    };

    class Process {
        public:
            Process(void *taskPtr);
            ~Process();

            void stop();

        protected:
            unsigned int id;
            Status status;

        private:
            void* _taskPtr;
    };
}

#endif
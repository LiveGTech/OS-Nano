/*
    LiveG OS Nano

    Copyright (C) LiveG. All Rights Reserved.

    https://liveg.tech/os
    Licensed by the LiveG Open-Source Licence, which can be found at LICENCE.md.
*/

#ifndef APP_H_
#define APP_H_

#include <Arduino.h>

#include "proc.h"

namespace app {
    proc::Process* launch(String id);
}

#endif
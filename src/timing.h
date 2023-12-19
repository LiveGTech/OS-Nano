/*
    LiveG OS Nano

    Copyright (C) LiveG. All Rights Reserved.

    https://liveg.tech/os
    Licensed by the LiveG Open-Source Licence, which can be found at LICENCE.md.
*/

#ifndef TIMING_H_
#define TIMING_H_

namespace timing {
    typedef double Duration;
    typedef Duration Timestamp;

    Duration getUptime();
    Timestamp getCurrentTime();
}

#endif
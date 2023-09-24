/*
    LiveG OS Nano

    Copyright (C) LiveG. All Rights Reserved.

    https://liveg.tech/os
    Licensed by the LiveG Open-Source Licence, which can be found at LICENCE.md.
*/

#ifndef COMMON_H_
#define COMMON_H_

#ifndef GOSN_SIMULATOR
    #define MALLOC_PSRAM ps_malloc
    #define REALLOC_PSRAM ps_realloc
#else
    #define MALLOC_PSRAM malloc
    #define REALLOC_PSRAM realloc
#endif

#endif
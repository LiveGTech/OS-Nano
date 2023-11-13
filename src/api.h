/*
    LiveG OS Nano

    Copyright (C) LiveG. All Rights Reserved.

    https://liveg.tech/os
    Licensed by the LiveG Open-Source Licence, which can be found at LICENCE.md.
*/

#ifndef API_H_
#define API_H_

#include <duktape.h>

#include "app.h"

namespace api {
    duk_ret_t print(duk_context* ctx);
    duk_ret_t addElement(duk_context* ctx);
    duk_ret_t setElementProp(duk_context* ctx);
}

#endif
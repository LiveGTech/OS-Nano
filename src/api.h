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

    duk_ret_t timing_getCurrentTime(duk_context* ctx);

    duk_ret_t launch(duk_context* ctx);
    duk_ret_t back(duk_context* ctx);

    duk_ret_t addElement(duk_context* ctx);
    duk_ret_t removeElement(duk_context* ctx);

    duk_ret_t setElementProp(duk_context* ctx);
    duk_ret_t setElementStyleRule(duk_context* ctx);

    duk_ret_t listenForEvents(duk_context* ctx);
}

#endif
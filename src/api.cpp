/*
    LiveG OS Nano

    Copyright (C) LiveG. All Rights Reserved.

    https://liveg.tech/os
    Licensed by the LiveG Open-Source Licence, which can be found at LICENCE.md.
*/

#include <Arduino.h>
#include <duktape.h>
#include <lvgl.h>

#include "api.h"
#include "app.h"

app::Element* getElement(duk_context* ctx, Count argIndex) {
    auto state = app::getStateFromDuktapeContext(ctx);
    int elementId = duk_get_int(ctx, argIndex);

    if (elementId < 0) {
        return nullptr;
    }

    return state->ownedElements[elementId];
}

duk_ret_t api::print(duk_context* ctx) {
    auto state = app::getStateFromDuktapeContext(ctx);

    duk_push_string(ctx, " ");
    duk_insert(ctx, 0);
    duk_join(ctx, duk_get_top(ctx) - 1);

    Serial.print("[");
    Serial.print(state->id);
    Serial.print("] ");
    Serial.println(duk_safe_to_string(ctx, -1));

    return 0;
}

duk_ret_t api::addElement(duk_context* ctx) {
    auto state = app::getStateFromDuktapeContext(ctx);
    int type = duk_get_int(ctx, 1);
    app::Element* parentElement = nullptr;

    if (type != app::ElementType::TYPE_SCREEN) {
        parentElement = getElement(ctx, 0);

        if (!parentElement) {
            return DUK_RET_REFERENCE_ERROR;
        }
    }

    auto element = new app::Element();

    element->type = (app::ElementType)type;

    switch (type) {
        case app::ElementType::TYPE_SCREEN:
            element->object = lv_obj_create(NULL);
            break;

        case app::ElementType::TYPE_CONTAINER:
            element->object = lv_obj_create(parentElement->object);
            break;

        case app::ElementType::TYPE_PARAGRAPH:
            element->object = lv_label_create(parentElement->object);
            break;

        default:
            return DUK_RET_TYPE_ERROR;
    }

    duk_push_int(ctx, state->ownedElements.push(element) - 1);

    return 1;
}

duk_ret_t api::setElementProp(duk_context* ctx) {
    auto state = app::getStateFromDuktapeContext(ctx);
    auto element = getElement(ctx, 0);
    auto property = duk_get_int(ctx, 1);

    if (!element) {
        return DUK_RET_REFERENCE_ERROR;
    }

    switch (property) {
        case app::ElementProperty::PROP_SHOWING:
            if (element->type == app::ElementType::TYPE_SCREEN) {
                if (duk_get_boolean(ctx, 2)) {
                    lv_scr_load(element->object);
                }

                return 0;
            }

            return DUK_RET_TYPE_ERROR;

        case app::ElementProperty::PROP_TEXT:
            if (element->type == app::ElementType::TYPE_PARAGRAPH) {
                lv_label_set_text(element->object, duk_safe_to_string(ctx, 2));

                return 0;
            }

            return DUK_RET_TYPE_ERROR;
    }

    return DUK_RET_TYPE_ERROR;
}
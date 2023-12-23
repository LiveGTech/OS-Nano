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
#include "datatypes.h"
#include "app.h"
#include "timing.h"

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

duk_ret_t api::timing_getCurrentTime(duk_context* ctx) {
    duk_push_number(ctx, timing::getCurrentTime());

    return 1;
}

duk_ret_t api::launch(duk_context* ctx) {
    auto id = String(duk_safe_to_string(ctx, 0));
    auto processPtr = duk_get_boolean(ctx, 1) ? app::launchOrSwitchToProcess(id, true) : app::launch(id, true);

    if (!processPtr) {
        return DUK_RET_REFERENCE_ERROR;
    }

    return 0;
}

duk_ret_t api::back(duk_context* ctx) {
    auto processPtr = app::getProcessFromDuktapeContext(ctx);
    auto state = (app::ProcessTaskState*)processPtr->taskState;

    if (app::getActiveProcess() != processPtr) {
        return 0;
    }

    if (!app::goBackToPreviousActiveProcess(true)) {
        return 0;
    }

    if (duk_get_boolean(ctx, 0)) {
        state->wantsToStop = true;
    }

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
    element->listeningForEvents = false;

    switch (type) {
        case app::ElementType::TYPE_SCREEN:
            element->object = lv_obj_create(NULL);

            lv_obj_set_layout(element->object, LV_LAYOUT_FLEX);
            lv_obj_set_flex_flow(element->object, LV_FLEX_FLOW_ROW_WRAP);

            break;

        case app::ElementType::TYPE_CONTAINER:
            element->object = lv_obj_create(parentElement->object);

            lv_obj_set_width(element->object, lv_pct(100));
            lv_obj_set_height(element->object, LV_SIZE_CONTENT);
            lv_obj_set_layout(element->object, LV_LAYOUT_FLEX);
            lv_obj_set_flex_flow(element->object, LV_FLEX_FLOW_ROW_WRAP);

            break;

        case app::ElementType::TYPE_TEXT:
            element->object = lv_label_create(parentElement->object);

            break;

        case app::ElementType::TYPE_PARAGRAPH:
            element->object = lv_label_create(parentElement->object);

            lv_obj_set_width(element->object, lv_pct(100));

            break;

        case app::ElementType::TYPE_BUTTON:
            element->object = lv_btn_create(parentElement->object);

            break;

        default:
            return DUK_RET_TYPE_ERROR;
    }

    state->elementStyleRules.start();

    while (auto styleRule = state->elementStyleRules.next()) {
        app::applyStyleRuleToElement(styleRule, element);
    }

    duk_push_int(ctx, state->ownedElements.push(element) - 1);

    return 1;
}

duk_ret_t api::removeElement(duk_context* ctx) {
    auto state = app::getStateFromDuktapeContext(ctx);
    auto element = getElement(ctx, 0);
    int elementId = duk_get_int(ctx, 0);

    if (!element) {
        return DUK_RET_REFERENCE_ERROR;
    }

    lv_obj_del(element->object);

    state->ownedElements.set(elementId, nullptr);

    return 0;
}

duk_ret_t api::setElementProp(duk_context* ctx) {
    auto processPtr = app::getProcessFromDuktapeContext(ctx);
    auto state = (app::ProcessTaskState*)processPtr->taskState;
    auto element = getElement(ctx, 0);
    auto property = duk_get_int(ctx, 1);

    if (!element) {
        return DUK_RET_REFERENCE_ERROR;
    }

    switch (property) {
        case app::ElementProperty::PROP_SHOWING:
            if (element->type == app::ElementType::TYPE_SCREEN) {
                if (!duk_get_boolean(ctx, 2)) {
                    return 0;
                }

                state->activeScreen = element;

                if (app::getActiveProcess() != processPtr) {
                    return 0;
                }

                lv_scr_load(element->object);

                return 0;
            }

            return DUK_RET_TYPE_ERROR;

        case app::ElementProperty::PROP_TEXT:
            if (
                element->type == app::ElementType::TYPE_TEXT ||
                element->type == app::ElementType::TYPE_PARAGRAPH
            ) {
                lv_label_set_text(element->object, duk_safe_to_string(ctx, 2));

                return 0;
            }

            return DUK_RET_TYPE_ERROR;
    }

    return DUK_RET_TYPE_ERROR;
}

duk_ret_t api::setElementStyleRule(duk_context* ctx) {
    auto state = app::getStateFromDuktapeContext(ctx);
    auto targetType = static_cast<app::ElementType>(duk_get_int(ctx, 0));
    auto targetState = static_cast<app::ElementState>(duk_get_int(ctx, 1));
    auto styleProperty = duk_get_int(ctx, 2);

    app::ElementStyleRule* styleRule = nullptr;

    state->elementStyleRules.start();

    while (auto existingStyleRule = state->elementStyleRules.next()) {
        if (existingStyleRule->targetType != targetType) {
            continue;
        }

        if (existingStyleRule->targetState != targetState) {
            continue;
        }

        styleRule = existingStyleRule;

        break;
    }

    if (!styleRule) {
        app::ElementStyleRule createdStyleRule;

        createdStyleRule.targetType = targetType;
        createdStyleRule.targetState = targetState;

        lv_style_init(&(createdStyleRule.style));

        styleRule = store<app::ElementStyleRule>(createdStyleRule);

        state->elementStyleRules.push(styleRule);

        state->ownedElements.start();

        while (auto element = state->ownedElements.next()) {
            app::applyStyleRuleToElement(styleRule, element);
        }
    }

    auto stylePtr = &(styleRule->style);

    switch (styleProperty) {
        case app::StyleProperty::STYLE_BACKGROUND:
            lv_style_set_bg_color(stylePtr, lv_color_hex(duk_get_int(ctx, 3)));
            lv_style_set_bg_opa(stylePtr, LV_OPA_100);
            break;

        case app::StyleProperty::STYLE_FOREGROUND:
            lv_style_set_text_color(stylePtr, lv_color_hex(duk_get_int(ctx, 3)));
            break;

        default:
            return DUK_RET_TYPE_ERROR;
    }

    state->ownedElements.start();

    while (auto element = state->ownedElements.next()) {
        if (element->type != app::ElementType::TYPE_SCREEN) {
            continue;
        }

        lv_obj_invalidate(element->object);
    }

    return 0;
}

duk_ret_t api::listenForEvents(duk_context* ctx) {
    auto processPtr = app::getProcessFromDuktapeContext(ctx);
    auto element = getElement(ctx, 0);

    auto target = (app::GlobalElementReference) {
        .processPtr = processPtr,
        .elementId = static_cast<Count>(duk_get_int(ctx, 0))
    };

    if (!element) {
        return DUK_RET_REFERENCE_ERROR;
    }

    if (element->listeningForEvents) {
        return 0;
    }

    lv_obj_add_event_cb(element->object, app::dispatchEventHandler, LV_EVENT_ALL, store<app::GlobalElementReference>(target));

    return 0;
}
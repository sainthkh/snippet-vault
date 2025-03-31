#pragma once

#include <quickjs.h>

extern JSContext *J;

void js_init();
void js_run();
void js_release();

void qjs_dofile_module(JSContext *J, const char *filename);

void qjs_internal_free_values(JSContext *J, JSValue *values, int count);

#define qjs_free_values(J, ...)                                               \
    JSValue values_##__COUNTER__[] = {__VA_ARGS__};                           \
    int count_##__COUNTER__ = sizeof(values_##__COUNTER__) / sizeof(JSValue); \
    qjs_internal_free_values(J, values_##__COUNTER__, count_##__COUNTER__)

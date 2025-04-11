#pragma once

#include <quickjs.h>
#include "stringstore.h"

extern JSContext *J;

void js_init();
void js_run();
void js_release();

void qjs_dofile_module(JSContext *J, const char *filename);

int qjs_getlength(JSContext *J, JSValue array);
string_id qjs_addstring_idx(JSContext *J, JSValue array, int idx);
int qjs_getint_idx(JSContext *J, JSValue array, int idx);
float qjs_getfloat_idx(JSContext *J, JSValue array, int idx);
double qjs_getnumber_idx(JSContext *J, JSValue array, int idx);
string_id qjs_addstring(JSContext *J, JSValue obj, const char *propName);
int qjs_getint(JSContext *J, JSValue obj, const char *propName);
float qjs_getfloat(JSContext *J, JSValue obj, const char *propName);
double qjs_getnumber(JSContext *J, JSValue obj, const char *propName);

void qjs_setint(JSContext *J, JSValue obj, const char *propName, int value);
void qjs_setfloat(JSContext *J, JSValue obj, const char *propName, double value);
void qjs_setstring(JSContext *J, JSValue obj, const char *propName, const char *value);
void qjs_setfunc(JSContext *J, JSValue obj, const char *propName, JSCFunction *func, int length);
void qjs_setfunc_global(JSContext *J, const char *propName, JSCFunction *func, int length);
void qjs_setint_array(JSContext *J, JSValue array, int idx, int value);
void qjs_setfloat_array(JSContext *J, JSValue array, int idx, double value);
void qjs_setstring_array(JSContext *J, JSValue array, int idx, const char *value);

void qjs_internal_free_values(JSContext *J, JSValue *values, int count);

#define qjs_free_values(J, ...)                                               \
    JSValue values_##__COUNTER__[] = {__VA_ARGS__};                           \
    int count_##__COUNTER__ = sizeof(values_##__COUNTER__) / sizeof(JSValue); \
    qjs_internal_free_values(J, values_##__COUNTER__, count_##__COUNTER__)

#include <utest.h>
#include "js.h"

UTEST(js, getlength)
{
    js_init();

    JSValue array = JS_NewArray(J);
    qjs_setint_array(J, array, 0, 1);
    qjs_setint_array(J, array, 1, 2);
    qjs_setint_array(J, array, 2, 3);

    int length = qjs_getlength(J, array);

    ASSERT_EQ(3, length);

    qjs_free_values(J, array);

    js_release();
}

UTEST(js, addstring_idx)
{
    js_init();
    gss_init();

    JSValue array = JS_NewArray(J);
    qjs_setstring_array(J, array, 0, "Hello world!");

    string_id id = qjs_addstring_idx(J, array, 0);
    const char *str2 = gss_get(id);

    ASSERT_STREQ("Hello world!", str2);

    qjs_free_values(J, array);

    gss_release();
    js_release();
}

UTEST(js, addstring)
{
    js_init();
    gss_init();

    JSValue obj = JS_NewObject(J);
    qjs_setstring(J, obj, "greeting", "Hello world!");

    string_id id = qjs_addstring(J, obj, "greeting");
    const char *str2 = gss_get(id);

    ASSERT_STREQ("Hello world!", str2);

    qjs_free_values(J, obj);

    gss_release();
    js_release();
}

UTEST(js, getint_idx)
{
    js_init();

    JSValue array = JS_NewArray(J);
    qjs_setint_array(J, array, 0, 1);
    qjs_setint_array(J, array, 1, 2);
    qjs_setint_array(J, array, 2, 3);

    int value = qjs_getint_idx(J, array, 1);

    ASSERT_EQ(2, value);

    qjs_free_values(J, array);

    js_release();
}

UTEST(js, getint)
{
    js_init();

    JSValue obj = JS_NewObject(J);
    qjs_setint(J, obj, "value", 42);

    int value = qjs_getint(J, obj, "value");

    ASSERT_EQ(42, value);

    qjs_free_values(J, obj);

    js_release();
}

UTEST(js, getfloat_idx)
{
    js_init();

    JSValue array = JS_NewArray(J);
    qjs_setfloat_array(J, array, 0, 1.1);
    qjs_setfloat_array(J, array, 1, 2.2);
    qjs_setfloat_array(J, array, 2, 3.3);

    float value = qjs_getfloat_idx(J, array, 1);

    ASSERT_EQ(2.2f, value);

    qjs_free_values(J, array);

    js_release();
}

UTEST(js, getfloat)
{
    js_init();

    JSValue obj = JS_NewObject(J);
    qjs_setfloat(J, obj, "value", 42.0);

    float value = qjs_getfloat(J, obj, "value");

    ASSERT_EQ(42.0f, value);

    qjs_free_values(J, obj);

    js_release();
}

UTEST(js, getnumber)
{
    js_init();

    JSValue obj = JS_NewObject(J);
    qjs_setfloat(J, obj, "value", 42.0);

    double value = qjs_getnumber(J, obj, "value");

    ASSERT_EQ(42.0, value);

    qjs_free_values(J, obj);

    js_release();
}

UTEST(js, getnumber_castcorrectly)
{
    js_init();

    JSValue obj = JS_NewObject(J);
    qjs_setint(J, obj, "value", 1);

    double value = qjs_getnumber(J, obj, "value");

    ASSERT_EQ(1.0, value);

    qjs_free_values(J, obj);

    js_release();
}

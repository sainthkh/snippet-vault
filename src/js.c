#include "js.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

static JSRuntime *rt;
JSContext *J;
static JSValue mainFn;

static JSValue js_print(JSContext *J, JSValue this_val, int argc, JSValue *argv);
static JSValue set_main_func(JSContext *J, JSValue this_val, int argc, JSValue *argv);
static JSModuleDef *js_module_loader(JSContext *ctx, const char *module_name, void *opaque);

void js_init()
{
    rt = JS_NewRuntime();
    J = JS_NewContext(rt);
    mainFn = JS_UNDEFINED;

    JS_SetModuleLoaderFunc(rt, NULL, js_module_loader, NULL);

    JSValue global = JS_GetGlobalObject(J);

    JSValue print = JS_NewCFunction(J, js_print, "print", 1);
    JS_SetPropertyStr(J, global, "print", print);

    JSValue temp_mainFn = JS_NewCFunction(J, set_main_func, "setMain", 1);
    JS_SetPropertyStr(J, global, "setMain", temp_mainFn);

    JS_FreeValue(J, global);
}

static JSModuleDef *js_module_loader(JSContext *ctx, const char *module_name, void *opaque)
{
    FILE *file = fopen(module_name, "rb");

    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *source = (char *)malloc(fileSize + 1);
    if (source == NULL)
    {
        fprintf(stderr, "Memory allocation failed\n");
        fclose(file);
        exit(1);
    }

    fread(source, 1, fileSize, file);
    source[fileSize] = '\0';

    fclose(file);

    JSValue result = JS_Eval(J, source, fileSize, module_name, JS_EVAL_TYPE_MODULE);
    if (JS_IsException(result))
    {
        JSValue exception = JS_GetException(J);
        const char *error = JS_ToCString(J, exception);
        fprintf(stderr, "[module loader exception] %s\n", error);
        JS_FreeCString(J, error);
        JS_FreeValue(J, exception);
    }

    JSModuleDef *m = JS_VALUE_GET_PTR(result);
    JS_FreeValue(J, result);

    free(source);

    return m;
}

void js_run()
{
    if (JS_IsUndefined(mainFn))
    {
        fprintf(stderr, "Main function not set\n");
        return;
    }

    JSValue empty = JS_NewObject(J);
    JSValue result = JS_Call(J, mainFn, empty, 0, NULL);
    if (JS_IsException(result))
    {
        JSValue exception = JS_GetException(J);
        const char *error = JS_ToCString(J, exception);
        fprintf(stderr, "[main fn exception] %s\n", error);
        JS_FreeCString(J, error);
        JS_FreeValue(J, exception);
    }

    qjs_free_values(J, result, empty);
}

void js_release()
{
    JS_FreeContext(J);
    JS_FreeRuntime(rt);
}

static JSValue js_print(JSContext *J, JSValue this_val, int argc, JSValue *argv)
{
    for (int i = 0; i < argc; i++)
    {
        if (i > 0)
        {
            printf(" ");
        }
        JSValue str = JS_ToString(J, argv[i]);
        if (!JS_IsException(str))
        {
            const char *cstr = JS_ToCString(J, str);
            if (cstr)
            {
                printf("%s", cstr);
                JS_FreeCString(J, cstr);
            }
            JS_FreeValue(J, str);
        }
    }
    printf("\n");

    return JS_UNDEFINED;
}

static JSValue set_main_func(JSContext *J, JSValue this_val, int argc, JSValue *argv)
{
    if (argc < 1)
    {
        fprintf(stderr, "setMain requires a function argument\n");
        return JS_UNDEFINED;
    }

    mainFn = argv[0];

    return JS_UNDEFINED;
}

void qjs_dofile_type(JSContext *J, const char *filename, int eval_type)
{
    FILE *file = fopen(filename, "rb");
    if (file == NULL)
    {
        fprintf(stderr, "Could not open file: %s\n", filename);
        exit(1);
    }

    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *source = (char *)malloc(fileSize + 1);
    if (source == NULL)
    {
        fprintf(stderr, "Memory allocation failed\n");
        fclose(file);
        exit(1);
    }

    fread(source, 1, fileSize, file);
    source[fileSize] = '\0';

    fclose(file);

    JSValue result = JS_Eval(J, source, fileSize, filename, eval_type);

    if (JS_IsException(result))
    {
        JSValue exception = JS_GetException(J);
        const char *error = JS_ToCString(J, exception);
        fprintf(stderr, "dofile failed: %s\n", error);
        JS_FreeCString(J, error);
        JS_FreeValue(J, exception);
    }

    JSModuleDef *m = JS_VALUE_GET_PTR(result);

    JS_FreeValue(J, result);
    free(source);
}

void qjs_dofile_global(JSContext *J, const char *filename)
{
    qjs_dofile_type(J, filename, JS_EVAL_TYPE_GLOBAL);
}

void qjs_dofile_module(JSContext *J, const char *filename)
{
    qjs_dofile_type(J, filename, JS_EVAL_TYPE_MODULE);
}

int qjs_getlength(JSContext *J, JSValue array)
{
    JSValue length = JS_GetPropertyStr(J, array, "length");
    int len = JS_VALUE_GET_INT(length);
    JS_FreeValue(J, length);

    return len;
}

string_id qjs_addstring_idx(JSContext *J, JSValue array, int idx)
{
    JSValue prop = JS_GetPropertyUint32(J, array, idx);
    if (JS_IsException(prop))
    {
        fprintf(stderr, "Error getting property by index: %d\n", idx);
        return -1;
    }

    const char *str = JS_ToCString(J, prop);
    JS_FreeValue(J, prop);

    string_id id = gss_add(str);
    JS_FreeCString(J, str);

    return id;
}

int qjs_getint_idx(JSContext *J, JSValue array, int idx)
{
    JSValue prop = JS_GetPropertyUint32(J, array, idx);
    if (JS_IsException(prop))
    {
        fprintf(stderr, "Error getting property by index: %d\n", idx);
        return -1;
    }

    int value = JS_VALUE_GET_INT(prop);
    JS_FreeValue(J, prop);

    return value;
}

float qjs_getfloat_idx(JSContext *J, JSValue array, int idx)
{
    JSValue prop = JS_GetPropertyUint32(J, array, idx);
    if (JS_IsException(prop))
    {
        fprintf(stderr, "Error getting property by index: %d\n", idx);
        return -1;
    }

    double value = JS_VALUE_GET_FLOAT64(prop);
    JS_FreeValue(J, prop);

    return value;
}

double qjs_getnumber_idx(JSContext *J, JSValue array, int idx)
{
    JSValue prop = JS_GetPropertyUint32(J, array, idx);
    if (JS_IsException(prop))
    {
        fprintf(stderr, "Error getting property by index: %d\n", idx);
        return -1;
    }

    double value = prop.tag == JS_TAG_INT ? JS_VALUE_GET_INT(prop) : JS_VALUE_GET_FLOAT64(prop);
    JS_FreeValue(J, prop);

    return value;
}

string_id qjs_addstring(JSContext *J, JSValue obj, const char *propName)
{
    JSValue prop = JS_GetPropertyStr(J, obj, propName);
    if (JS_IsException(prop))
    {
        fprintf(stderr, "Error getting property: %s\n", propName);
        return -1;
    }

    const char *str = JS_ToCString(J, prop);
    JS_FreeValue(J, prop);

    string_id id = gss_add(str);
    JS_FreeCString(J, str);

    return id;
}

int qjs_getint(JSContext *J, JSValue obj, const char *propName)
{
    JSValue prop = JS_GetPropertyStr(J, obj, propName);
    if (JS_IsException(prop))
    {
        fprintf(stderr, "Error getting property: %s\n", propName);
        return -1;
    }

    int value = JS_VALUE_GET_INT(prop);
    JS_FreeValue(J, prop);

    return value;
}

float qjs_getfloat(JSContext *J, JSValue obj, const char *propName)
{
    JSValue prop = JS_GetPropertyStr(J, obj, propName);
    if (JS_IsException(prop))
    {
        fprintf(stderr, "Error getting property: %s\n", propName);
        return -1;
    }

    double value = JS_VALUE_GET_FLOAT64(prop);
    JS_FreeValue(J, prop);

    return value;
}

double qjs_getnumber(JSContext *J, JSValue obj, const char *propName)
{
    JSValue prop = JS_GetPropertyStr(J, obj, propName);
    if (JS_IsException(prop))
    {
        fprintf(stderr, "Error getting property: %s\n", propName);
        return -1;
    }

    double value = prop.tag == JS_TAG_INT ? JS_VALUE_GET_INT(prop) : JS_VALUE_GET_FLOAT64(prop);
    JS_FreeValue(J, prop);

    return value;
}

void qjs_setint(JSContext *J, JSValue obj, const char *propName, int value)
{
    JSValue prop = JS_NewInt32(J, value);
    JS_SetPropertyStr(J, obj, propName, prop);
}

void qjs_setfloat(JSContext *J, JSValue obj, const char *propName, double value)
{
    JSValue prop = JS_NewFloat64(J, value);
    JS_SetPropertyStr(J, obj, propName, prop);
}

void qjs_setstring(JSContext *J, JSValue obj, const char *propName, const char *value)
{
    JSValue prop = JS_NewString(J, value);
    JS_SetPropertyStr(J, obj, propName, prop);
}

void qjs_setint_array(JSContext *J, JSValue array, int idx, int value)
{
    JSValue val = JS_NewInt32(J, value);
    JS_SetPropertyUint32(J, array, idx, val);
}

void qjs_setfloat_array(JSContext *J, JSValue array, int idx, double value)
{
    JSValue val = JS_NewFloat64(J, value);
    JS_SetPropertyUint32(J, array, idx, val);
}

void qjs_setstring_array(JSContext *J, JSValue array, int idx, const char *value)
{
    JSValue val = JS_NewString(J, value);
    JS_SetPropertyUint32(J, array, idx, val);
}

void qjs_internal_free_values(JSContext *J, JSValue *values, int count)
{
    for (int i = 0; i < count; i++)
    {
        JS_FreeValue(J, values[i]);
    }
}

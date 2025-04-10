#include "api.h"

#include "js.h"

AppMetadata app_metadata;
AppMetadataProperties app_metadata_props;

static JSValue js_appinit(JSContext *J, JSValue this_val, int argc, JSValue *argv);

void init_api()
{
    JSValue global = JS_GetGlobalObject(J);

    JS_FreeValue(J, global);
}

void release_api()
{
}

static JSValue js_appinit(JSContext *J, JSValue this_val, int argc, JSValue *argv)
{
    if (argc < 1)
    {
        fprintf(stderr, "appInit requires a function argument\n");
        return JS_UNDEFINED;
    }

    JSValue appname = JS_GetPropertyStr(J, argv[0], "name");
    JSValue appversion = JS_GetPropertyStr(J, argv[0], "version");
    JSValue appid = JS_GetPropertyStr(J, argv[0], "id");

    return JS_UNDEFINED;
}

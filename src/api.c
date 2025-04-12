#include "api.h"

#include "js.h"
#include "scene.h"

AppWindow app_window;
AppMetadata app_metadata;
AppMetadataProperties app_metadata_props;

static JSValue js_appinit(JSContext *J, JSValue this_val, int argc, JSValue *argv);

void api_init()
{
    qjs_setfunc_global(J, "api_appInit", js_appinit, 2);

    scene_api_init();
}

void api_release()
{
    scene_api_release();
}

static JSValue js_appinit(JSContext *J, JSValue this_val, int argc, JSValue *argv)
{
    if (argc < 2)
    {
        fprintf(stderr, "appInit requires a function argument\n");
        return JS_UNDEFINED;
    }

    app_window.name = qjs_addstring(J, argv[0], "name");
    app_window.width = qjs_getint(J, argv[0], "width");
    app_window.height = qjs_getint(J, argv[0], "height");

    app_metadata.appname = qjs_addstring(J, argv[1], "appname");
    app_metadata.appversion = qjs_addstring(J, argv[1], "appversion");
    app_metadata.appid = qjs_addstring(J, argv[1], "appid");

    app_metadata_props.url = qjs_addstring(J, argv[2], "url");
    app_metadata_props.creator = qjs_addstring(J, argv[2], "creator");
    app_metadata_props.copyright = qjs_addstring(J, argv[2], "copyright");
    app_metadata_props.type = qjs_addstring(J, argv[2], "type");

    return JS_UNDEFINED;
}

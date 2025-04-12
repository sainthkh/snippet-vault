#include "scene.h"

#include <stdlib.h>

typedef struct
{
    Scene *scenes;
    int num_scenes;
    int capacity;
    int current_scene;
} SceneManager;

static SceneManager scene_manager;

static const int SCENE_MANAGER_INIT_CAPACITY = 4;

void scene_init()
{
    scene_manager.scenes = malloc(sizeof(Scene) * SCENE_MANAGER_INIT_CAPACITY);
    scene_manager.num_scenes = 0;
    scene_manager.capacity = SCENE_MANAGER_INIT_CAPACITY;
    scene_manager.current_scene = -1;
}

void scene_release()
{
    for (int i = 0; i < scene_manager.num_scenes; i++)
    {
        Scene *scene = &scene_manager.scenes[i];
        JS_FreeValue(J, scene->init);
        JS_FreeValue(J, scene->update);
        JS_FreeValue(J, scene->render);
        JS_FreeValue(J, scene->release);
    }
    free(scene_manager.scenes);
}

void scene_update()
{
    Scene *current_scene = &scene_manager.scenes[scene_manager.current_scene];
    JSValue result = JS_Call(J, current_scene->update, JS_UNDEFINED, 0, NULL);
    JS_FreeValue(J, result);
}

void scene_render()
{
    Scene *current_scene = &scene_manager.scenes[scene_manager.current_scene];
    JSValue result = JS_Call(J, current_scene->render, JS_UNDEFINED, 0, NULL);
    JS_FreeValue(J, result);
}

void scene_change(int scene)
{
    if (scene_manager.current_scene == scene)
    {
        return;
    }

    if (scene_manager.current_scene != -1)
    {
        Scene *current_scene = &scene_manager.scenes[scene_manager.current_scene];
        JSValue result = JS_Call(J, current_scene->release, JS_UNDEFINED, 0, NULL);
        JS_FreeValue(J, result);
    }

    scene_manager.current_scene = scene;

    Scene *current_scene = &scene_manager.scenes[scene_manager.current_scene];
    JSValue result = JS_Call(J, current_scene->init, JS_UNDEFINED, 0, NULL);
    JS_FreeValue(J, result);
}

int scene_add(Scene scene)
{
    if (scene_manager.num_scenes >= scene_manager.capacity)
    {
        scene_manager.capacity *= 2;
        scene_manager.scenes = realloc(scene_manager.scenes, sizeof(Scene) * scene_manager.capacity);
    }

    scene_manager.scenes[scene_manager.num_scenes] = scene;

    return scene_manager.num_scenes++;
}

static JSValue js_addscene(JSContext *J, JSValue this_val, int argc, JSValue *argv);

void scene_api_init()
{
    qjs_setfunc_global(J, "api_addScene", js_addscene, 1);
}

void scene_api_release()
{
}

static JSValue js_addscene(JSContext *J, JSValue this_val, int argc, JSValue *argv)
{
    if (argc < 1)
    {
        fprintf(stderr, "addScene requires a function argument\n");
        return JS_UNDEFINED;
    }

    Scene scene;
    scene.init = JS_GetPropertyStr(J, argv[0], "init");
    scene.update = JS_GetPropertyStr(J, argv[0], "update");
    scene.render = JS_GetPropertyStr(J, argv[0], "render");
    scene.release = JS_GetPropertyStr(J, argv[0], "release");

    int scene_id = scene_add(scene);
    JSValue scene_id_value = JS_NewInt32(J, scene_id);

    return scene_id_value;
}

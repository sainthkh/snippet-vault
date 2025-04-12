#pragma once

#include "js.h"

typedef struct
{
    JSValue init;
    JSValue update;
    JSValue render;
    JSValue release;
} Scene;

void scene_init();
void scene_release();
void scene_update();
void scene_render();
void scene_change(int scene);

void scene_api_init();
void scene_api_release();

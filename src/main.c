/*
 * Logic implementation of the Snake game. It is designed to efficiently
 * represent the state of the game in memory.
 *
 * This code is public domain. Feel free to use it for any purpose!
 */

#define SDL_MAIN_USE_CALLBACKS 1 /* use the callbacks instead of main() */
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include "stringstore.h"
#include "js.h"
#include "api.h"
#include "scene.h"

#define SNAKE_BLOCK_SIZE_IN_PIXELS 24
#define SDL_WINDOW_WIDTH (SNAKE_BLOCK_SIZE_IN_PIXELS * SNAKE_GAME_WIDTH)
#define SDL_WINDOW_HEIGHT (SNAKE_BLOCK_SIZE_IN_PIXELS * SNAKE_GAME_HEIGHT)

#define SNAKE_GAME_WIDTH 24U
#define SNAKE_GAME_HEIGHT 18U

static SDL_Joystick *joystick = NULL;

typedef struct
{
    int dummy;
} Context;

typedef struct
{
    SDL_Window *window;
    SDL_Renderer *renderer;
    Context ctx;
} AppState;

static SDL_AppResult handle_key_event_(Context *ctx, SDL_Scancode key_code)
{
    switch (key_code)
    {
    /* Quit. */
    case SDL_SCANCODE_ESCAPE:
    case SDL_SCANCODE_Q:
        return SDL_APP_SUCCESS;
    /* Restart the game as if the program was launched. */
    case SDL_SCANCODE_R:
        break;
    /* Decide new direction of the snake. */
    case SDL_SCANCODE_RIGHT:
        break;
    case SDL_SCANCODE_UP:
        break;
    case SDL_SCANCODE_LEFT:
        break;
    case SDL_SCANCODE_DOWN:
        break;
    default:
        break;
    }
    return SDL_APP_CONTINUE;
}

static SDL_AppResult handle_hat_event_(Context *ctx, Uint8 hat)
{
    switch (hat)
    {
    case SDL_HAT_RIGHT:
        break;
    case SDL_HAT_UP:
        break;
    case SDL_HAT_LEFT:
        break;
    case SDL_HAT_DOWN:
        break;
    default:
        break;
    }
    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void *appstate)
{
    AppState *as = (AppState *)appstate;

    scene_update();

    SDL_SetRenderDrawColor(as->renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(as->renderer);

    scene_render();

    SDL_RenderPresent(as->renderer);
    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[])
{
    gss_init();
    js_init();
    api_init();
    scene_init();

    qjs_dofile_module(J, "js/load.js");

    scene_change(0);

    size_t i;

    if (!SDL_SetAppMetadata(gss_get(app_metadata.appname), gss_get(app_metadata.appversion), gss_get(app_metadata.appid)))
    {
        SDL_Log("Couldn't set app metadata: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    const char *metakeys[] = {
        SDL_PROP_APP_METADATA_URL_STRING,
        SDL_PROP_APP_METADATA_CREATOR_STRING,
        SDL_PROP_APP_METADATA_COPYRIGHT_STRING,
        SDL_PROP_APP_METADATA_TYPE_STRING,
    };

    const char *metavalues[] = {
        gss_get(app_metadata_props.url),
        gss_get(app_metadata_props.creator),
        gss_get(app_metadata_props.copyright),
        gss_get(app_metadata_props.type),
    };

    for (i = 0; i < SDL_arraysize(metakeys); i++)
    {
        if (!SDL_SetAppMetadataProperty(metakeys[i], metavalues[i]))
        {
            return SDL_APP_FAILURE;
        }
    }

    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK))
    {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    AppState *as = (AppState *)SDL_calloc(1, sizeof(AppState));
    if (!as)
    {
        return SDL_APP_FAILURE;
    }

    *appstate = as;

    if (!SDL_CreateWindowAndRenderer(gss_get(app_window.name), app_window.width, app_window.height, 0, &as->window, &as->renderer))
    {
        return SDL_APP_FAILURE;
    }

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
{
    Context *ctx = &((AppState *)appstate)->ctx;
    switch (event->type)
    {
    case SDL_EVENT_QUIT:
        return SDL_APP_SUCCESS;
    case SDL_EVENT_JOYSTICK_ADDED:
        if (joystick == NULL)
        {
            joystick = SDL_OpenJoystick(event->jdevice.which);
            if (!joystick)
            {
                SDL_Log("Failed to open joystick ID %u: %s", (unsigned int)event->jdevice.which, SDL_GetError());
            }
        }
        break;
    case SDL_EVENT_JOYSTICK_REMOVED:
        if (joystick && (SDL_GetJoystickID(joystick) == event->jdevice.which))
        {
            SDL_CloseJoystick(joystick);
            joystick = NULL;
        }
        break;
    case SDL_EVENT_JOYSTICK_HAT_MOTION:
        return handle_hat_event_(ctx, event->jhat.value);
    case SDL_EVENT_KEY_DOWN:
        return handle_key_event_(ctx, event->key.scancode);
    default:
        break;
    }
    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
    if (joystick)
    {
        SDL_CloseJoystick(joystick);
    }
    if (appstate != NULL)
    {
        AppState *as = (AppState *)appstate;
        SDL_DestroyRenderer(as->renderer);
        SDL_DestroyWindow(as->window);
        SDL_free(as);
    }

    scene_release();
    api_release();
    js_release();
    gss_release();
}

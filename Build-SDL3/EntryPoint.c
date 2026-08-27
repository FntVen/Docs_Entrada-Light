#include <SDL3/SDL_init.h>
#include <SDL3/SDL_oldnames.h>
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_video.h>
#include <stdio.h>
#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;
SDL_FRect TestRect;

#define WnHeight 600
#define WnWidth 1200

int ErrorArray[7] = {0};/*(1)Bg (2)-- (3)-- (4)-- (5)-- (6)-- */

static float CalcPercent(float NUM, int PERCENT)
{
    float Result = (NUM * PERCENT)/100;
    return Result;
}

SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[])
{
    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    if (!SDL_CreateWindowAndRenderer("Luciano Test 1", WnWidth, WnHeight, SDL_WINDOW_RESIZABLE, &window, &renderer))
    {
        SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }
    SDL_SetRenderLogicalPresentation(renderer, WnWidth, WnHeight, SDL_LOGICAL_PRESENTATION_STRETCH);

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void *appstate)
{
    ErrorArray[0] = SDL_SetRenderDrawColor(renderer, 255, 255, 255,SDL_ALPHA_OPAQUE);

    for(int i=0;i<sizeof(ErrorArray);i++)
    {
        if(ErrorArray[i] == 1)
        {
            SDL_APP_FAILURE;
        }
    }
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    int Height, Width;
    SDL_GetWindowSize(window, &Width, &Height);
    printf("Width: %d | Height %d \n Half Width: %f | Half Height: %f",Width,Height,CalcPercent(Width, 50),CalcPercent(Height, 50));
    TestRect.x = 0;
    TestRect.y = 0;
    TestRect.w = CalcPercent(Width, 50);
    TestRect.h = Height;
    SDL_RenderFillRect(renderer,&TestRect);

    SDL_RenderPresent(renderer);

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
{
    if (event->type == SDL_EVENT_QUIT) {
        return SDL_APP_SUCCESS;  /* end the program, reporting success to the OS. */
    }
    return SDL_APP_CONTINUE;  /* carry on with the program! */
}

void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
    /* SDL will clean up the window/renderer for us. */
}

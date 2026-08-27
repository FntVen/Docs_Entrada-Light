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
SDL_FRect TabRec;
SDL_FRect MainRec;
SDL_FRect OptionsRec;

#define WnHeight 600
#define WnWidth 1200

int ErrorArray[7] = {0};/*(1)Bg (2)-- (3)-- (4)-- (5)-- (6)-- */

static float CalcPercent(float NUM, float PERCENT)
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
    SDL_SetRenderLogicalPresentation(renderer, WnWidth, WnHeight, SDL_LOGICAL_PRESENTATION_DISABLED);

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void *appstate)
{
    ErrorArray[0] = SDL_SetRenderDrawColor(renderer, 40, 40, 40,SDL_ALPHA_OPAQUE);

    for(int i=0;i<sizeof(ErrorArray);i++)
    {
        if(ErrorArray[i] == 1)
        {
            SDL_APP_FAILURE;
        }
    }
    int Height, Width;
    SDL_GetWindowSizeInPixels(window, &Width, &Height);
    //Tab Menu & CO
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 60, 60, 60, SDL_ALPHA_OPAQUE);
    TabRec.x = 40;
    TabRec.y = 20;
    TabRec.w = CalcPercent(Width, 20);
    TabRec.h = CalcPercent(Height, 95);
    SDL_RenderFillRect(renderer,&TabRec);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderLine(renderer, TabRec.x + 6, TabRec.y + 24.5, TabRec.w , TabRec.y + 24.5);
    SDL_SetRenderScale(renderer, 1.5, 1.5);
    SDL_RenderDebugText(renderer, 40,22,"Categorias");
    SDL_SetRenderScale(renderer, 1, 1);
    // -> Tabs
    //Main Screen & Co
    SDL_SetRenderDrawColor(renderer, 60, 60, 60, SDL_ALPHA_OPAQUE);//Diffent color fo debugging
    MainRec.x = TabRec.x + TabRec.w + 20;
    MainRec.y = 20;
    MainRec.w = Width - TabRec.w - 90;
    MainRec.h = CalcPercent(Height, 95);
    SDL_RenderFillRect(renderer,&MainRec);

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

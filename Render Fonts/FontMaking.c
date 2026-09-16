#include "Libs/SDLLV1.h"
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_surface.h>
#include <SDL3_ttf/SDL_ttf.h>

#define WnHeight 600
#define WnWidth 1200

int MouseMove[2] = {0};
float PosX[100] = {0};
float PosY[100] = {0};
SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[])
{
    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    if (!SDL_CreateWindowAndRenderer("Luciano Test 1", WnWidth, WnHeight, SDL_WINDOW_RESIZABLE, &window, &Render))
    {
        SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }
    if(!TTF_Init())
    {
        SDL_Log("Error in the Font Loader");
        return SDL_APP_FAILURE;
    }
    TTF_Font *MainFont = TTF_OpenFont("fonts/PutActualFontHere.ttf", 24);
    if(!MainFont)
    {
        SDL_Log("Error Loading Font");
        return SDL_APP_FAILURE;
    }
    SDL_Color TxtColor = {.a=255,.r=255,.g=255,.b=255};//Probably use my own RGBA thing for consistency
    SDL_Surface *TxtSurface = TTF_RenderText_Blended(MainFont,"Do i need to use this command every time?", 0, TxtColor);
    if(!TxtSurface)
    {
        SDL_Log("Error Loading Font");
        return SDL_APP_FAILURE;
    }
    int LocationINScreenX = 0;
    int LocationINScreenY = 0;
    SDL_FRect TextRectangle = {.x=LocationINScreenX,.y=LocationINScreenY,.w=(float)TxtSurface->w,.h=(float)TxtSurface->h};
    SDL_Texture *TextFinalTexture = SDL_CreateTextureFromSurface(Render, TxtSurface);
    SDL_DestroySurface(TxtSurface);
    SDL_RenderTexture(Render, TextFinalTexture, NULL, &TextRectangle);
    SDL_SetRenderLogicalPresentation(Render, WnWidth, WnHeight, SDL_LOGICAL_PRESENTATION_DISABLED);

    return SDL_APP_CONTINUE;
}
SDL_AppResult SDL_AppIterate(void *appstate)
{
    SDL_SetRenderDrawColor(Render, 0, 0, 0, 255);
    SDL_RenderClear(Render);
    SDL_SetRenderDrawColor(Render, 255, 255, 255, 255);
    SDL_RenderPresent(Render);
    return SDL_APP_CONTINUE;  /* carry on with the program! */
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
{
    if (event->type == SDL_EVENT_QUIT)
    {
        return SDL_APP_SUCCESS;
    }
 return SDL_APP_CONTINUE;
}
void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
    //Implement actual resorce cleanup for textures
    //
    // SDL_DestroyTexture();
    // TTF_CloseFont();
    // TTF_Quit();
    // SDL_DestroyRenderer();
    // SDL_DestroyWindow();
}

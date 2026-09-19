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
TTF_Font *Font;

typedef struct
{
    char *String;
    int Size;
    int X;
    int Y;
    int W;
    int H;
    SDL_Texture *Texture;
    Color ARGB;
}TextData;//Make Text then render;
TextData ContextText[100];
int TextContextUsed = 0;
bool RenderTextContext = false;
SDL_AppResult RenderFont()
{
    if(RenderTextContext)
    {
        for(int i=0; i<TextContextUsed;i++)
        {
            printf("i: %d | X: %d | Y: %d | W: %d | H: %d \n",i,ContextText[i].X,ContextText[i].Y,ContextText[i].W,ContextText[i].H);
            SDL_RenderTexture(Render, ContextText[i].Texture, NULL, &(SDL_FRect){
                .x=ContextText[i].X,
                .y=ContextText[i].Y,
                .w=ContextText[i].W,
                .h=ContextText[i].H
            });
        }
        return SDL_APP_CONTINUE;
    }
    for(int i=0; i<TextContextUsed;i++)
    {
        SDL_Color TxtColor = {.a=ContextText[i].ARGB.A,.r=ContextText[i].ARGB.R,.g=ContextText[i].ARGB.G,.b=ContextText[i].ARGB.B};
        SDL_Surface *TxtSuface = TTF_RenderText_Blended(Font,ContextText[i].String,0,TxtColor);

        ContextText[i].H = TxtSuface->h;
        ContextText[i].W = TxtSuface->w;
        ContextText[i].Texture = SDL_CreateTextureFromSurface(Render,TxtSuface);
        SDL_DestroySurface(TxtSuface);
        SDL_RenderTexture(Render, ContextText[TextContextUsed].Texture, NULL, &(SDL_FRect){
            .x=ContextText[i].X,
            .y=ContextText[i].Y,
            .w=ContextText[i].W,
            .h=ContextText[i].H
        });
    }
    RenderTextContext = true;
    return SDL_APP_CONTINUE;
}
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
    TTF_Font *MainFont = TTF_OpenFont("Fonts/Roboto-Regular.ttf", 24);
    if(!MainFont)
    {
        SDL_Log("Error Loading Font");
        return SDL_APP_FAILURE;
    }
    Font = MainFont;
    SDL_SetRenderLogicalPresentation(Render, WnWidth, WnHeight, SDL_LOGICAL_PRESENTATION_DISABLED);

    return SDL_APP_CONTINUE;
}
SDL_AppResult SDL_AppIterate(void *appstate)
{
    SDL_SetRenderDrawColor(Render, 0, 0, 0, 255);
    SDL_RenderClear(Render);
    SDL_SetRenderDrawColor(Render, 255, 255, 255, 255);
    if(!TextContextUsed)
    {
        ContextText[TextContextUsed] = (TextData){
           .ARGB = {.A=255,.R=255,.G=255,.B=255},
           .String = "Teste",
           .X = 60,
           .Y = 100,
           .Size = 24
        };
        TextContextUsed++;
    }
    RenderFont();
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

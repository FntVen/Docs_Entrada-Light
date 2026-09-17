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
TTF_Font *Font;
float Width = 0;
float Height = 0;
int MouseMove[2] = {0};
float PosX[100] = {0};
float PosY[100] = {0};

typedef struct
{
    SDL_Texture *Texture;
    float X;
    float Y;
    float Width;
    float Height;
    const char *String;
    Color RGB;
}Text;
Text *Labels[100];
int UsedLabels=0;

void SetText(Text *Txt, float X, float Y, Color Color)
{
    Txt->RGB = Color;
    SDL_Color TxtColor = {.a=Txt->RGB.A,.r=Txt->RGB.R,.g=Txt->RGB.G,.b=Txt->RGB.B};//Probably use my own RGBA thing for consistency
    SDL_Surface *TxtSurface = TTF_RenderText_Blended(Font,Txt->String, 0, TxtColor);
    if(!TxtSurface)
    {
        SDL_Log("Error Loading Font");
        return;
    }

    Txt->Width = TxtSurface->w;
    Txt->Height = TxtSurface->h;
    Txt->X = X;
    Txt->Y = Y;

    Txt->Texture = SDL_CreateTextureFromSurface(Render, TxtSurface);
    SDL_DestroySurface(TxtSurface);

    Labels[UsedLabels] = Txt;
    UsedLabels++;
}

SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[])
{
    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    if (!SDL_CreateWindowAndRenderer("Luciano Teste Texto", WnWidth, WnHeight, SDL_WINDOW_RESIZABLE, &window, &Render))
    {
        SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }
    SDL_SetRenderLogicalPresentation(Render, WnWidth, WnHeight, SDL_LOGICAL_PRESENTATION_DISABLED);
    if(!TTF_Init())
    {
        SDL_Log("Error in the Font Loader");
        return SDL_APP_FAILURE;
    }
    TTF_Font *MainFont = TTF_OpenFont("Resources/Roboto-Regular.ttf", 48);
    if(!MainFont)
    {
        SDL_Log("Error Loading Font");
        return SDL_APP_FAILURE;
    }
    Font = MainFont;

    return SDL_APP_CONTINUE;
}
Text TXT_TextoTitulo;//Need to be outside the iterate loop
bool TextTriggers[8] = {false,false,false,false,false,false,false,false};
int Iterate_TriggerCounter = 0;
SDL_AppResult SDL_AppIterate(void *appstate)
{
    SDL_SetRenderDrawColor(Render, 0, 0, 0, 255);

    if(!TextTriggers[Iterate_TriggerCounter])
    {
        TXT_TextoTitulo.String = "Teste";
        SetText(&TXT_TextoTitulo, 30, 40,(Color){.A=255,.R=255,.G=255,.B=255});
    }
    TextTriggers[Iterate_TriggerCounter] = true;
    Iterate_TriggerCounter++;
    SDL_RenderTexture(Render, TXT_TextoTitulo.Texture, NULL, &(SDL_FRect){.x=TXT_TextoTitulo.X,.y=TXT_TextoTitulo.Y,.h=TXT_TextoTitulo.Height,.w=TXT_TextoTitulo.Width});
    SDL_RenderPresent(Render);
    Iterate_TriggerCounter = 0;
    return SDL_APP_CONTINUE;
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
    for(int i=0;i<=UsedLabels;i++)
    {
        SDL_DestroyTexture(Labels[i]->Texture);
    }
    TTF_CloseFont(Font);
    TTF_Quit();
    SDL_DestroyRenderer(Render);
    SDL_DestroyWindow(window);
}

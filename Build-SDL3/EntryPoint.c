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
SDL_FRect Tabs[5];//Cliente - Solar - Local - Contrato - Planta Baixa
char *StringArrays[5] = {"Cliente","Solar","Local","Contrato","P. Baixa"};

#define WnHeight 600
#define WnWidth 1200

int ErrorArray[7] = {0};/*(1)Bg (2)TabOptions (3)CenterMenu (4)-- (5)-- (6)-- */

#define ClientMen 0
#define SolarMen 1
#define LocalMen 2
#define ContractMen 3
#define MainMen 5
#define FinalMen 4
int CurrentMenu = MainMen;

static void MainMenu()
{

}
static void FinalMenu()
{

}

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
    TabRec.x = 30;
    TabRec.y = 20;
    TabRec.w = CalcPercent(Width, 20);
    TabRec.h = CalcPercent(Height, 95);
    SDL_RenderFillRect(renderer,&TabRec);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderLine(renderer, TabRec.x + 6, TabRec.y + 24.8, TabRec.w , TabRec.y + 24.5);
    SDL_SetRenderScale(renderer, 1.5, 1.5);
    SDL_RenderDebugText(renderer, 40,22,"Categorias");
    SDL_SetRenderScale(renderer, 1, 1);
    OptionsRec.y = (TabRec.y + TabRec.h) - 50;
    OptionsRec.h = 40;
    OptionsRec.x = TabRec.x + 10;
    OptionsRec.w = 40;
    SDL_SetRenderDrawColor(renderer, 40, 40, 40,SDL_ALPHA_OPAQUE);
    SDL_RenderFillRect(renderer, &OptionsRec);
    // -> Tabs
    SDL_SetRenderDrawColor(renderer, 65, 65, 65, 255);
    for(int i=0;i<4;i++)
    {
        Tabs[i].y = (TabRec.y + 40) + 40 * i;
        Tabs[i].x = TabRec.x;
        Tabs[i].w = TabRec.w;
        Tabs[i].h = 30;
        if(false)
        {
            printf("Tab[%d].y = %f \n",i,(TabRec.y + 40) + 33 * i);
            printf("Tab[%d].x = %f \n",i,TabRec.x);
            printf("Tab[%d].w = %f \n",i,TabRec.w);
            printf("Tab[%d].h = %f \n",i,Tabs[i].h);
        }
    }
    SDL_RenderFillRects(renderer,Tabs,4);
    SDL_SetRenderDrawColor(renderer, 175, 175, 175, SDL_ALPHA_OPAQUE);
    for(int i=0;i<4;i++)
    {
        SDL_RenderDebugText(renderer, TabRec.x + 9, Tabs[i].y+ 13, StringArrays[i]);
    }

    //Main Screen & Co
    SDL_SetRenderDrawColor(renderer, 60, 60, 60, SDL_ALPHA_OPAQUE);//Diffent color fo debugging
    MainRec.x = TabRec.x + TabRec.w + 20;
    MainRec.y = 20;
    MainRec.w = Width - TabRec.w - 85;
    MainRec.h = CalcPercent(Height, 95);
    SDL_RenderFillRect(renderer,&MainRec);
    //->Inside Screen
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

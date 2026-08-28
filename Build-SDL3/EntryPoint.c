#include <SDL3/SDL_events.h>
#include <SDL3/SDL_filesystem.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_stdinc.h>
#include <SDL3/SDL_surface.h>
#include <SDL3/SDL_video.h>
#include <stdio.h>
#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#if defined(_WIN32) || defined(_WIN64)
	#define OSsep  '\\'
#else
	#define OSsep  '/'
#endif

static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;
static SDL_Texture *CogTexture = NULL;
static int THeight;
static int TWidth;
static SDL_FRect TabRec;
int TabRecIndex;
static SDL_FRect MainRec;
int MainRecIndex;
static SDL_FRect OptionsRec;
int OptionsRecIndex;
static SDL_FRect Tabs[5];//Cliente - Solar - Local - Contrato - Planta Baixa
int TabsIndex[5];
static char *StringArrays[5] = {"Cliente","Solar","Local","Contrato","P. Baixa"};

typedef struct
{
    float R;
    float G;
    float B;
}Color;
Color Bg_Color = {.R = 43, .G = 41, .B = 51};
int UsedBoxesCount = 0;
typedef struct
{
    SDL_FRect *BoxObj[100];
    float X[100];
    float Y[100];
    float Width[100];
    float Height[100];
    Color RGB[100];
}SelBoxes;
SelBoxes UsedBoxes;
typedef struct
{
    char TEvent[15];
    //Find a way to pass event data
}BtnEvent;

static float MPosition[2];
typedef struct
{
    bool HoverState;
    float ContextBounding_X1;
    float ContextBounding_X2;
    float ContextBounding_Y1;
    float ContextBounding_Y2;
    int Index;
    Color ItemRGB;
}MHover;
MHover MouseHover = {.HoverState = false};

int ErrorArray[7] = {0};/*(1)Bg (2)TabOptions (3)CenterMenu (4)-- (5)-- (6)-- */

#define WnHeight 600
#define WnWidth 1200

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

    //Texture Handling
    //->Cog Png
    SDL_Surface *Plane = NULL;
    char *CogPath = NULL;

    SDL_asprintf(&CogPath, "%sResources%cSettings.bmp",SDL_GetBasePath(),OSsep);
    Plane = SDL_LoadBMP(CogPath);
    if(!Plane)
    {
        printf("Erro Mapeando Textura");
        return SDL_APP_FAILURE;
    }

    THeight = Plane->h;
    TWidth = Plane->w;
    CogTexture = SDL_CreateTextureFromSurface(renderer, Plane);
    if(!CogTexture)
    {
        printf("Erro Criando Bitmap da Texture");
        return SDL_APP_FAILURE;
    }
    SDL_free(CogPath);
    SDL_DestroySurface(Plane);
    //Setting Layout
    int Height, Width;
    SDL_GetWindowSizeInPixels(window, &Width, &Height);
    //Tab Box
    TabRec.x = 30;
    TabRec.y = 20;
    TabRec.w = CalcPercent(Width, 20);
    TabRec.h = CalcPercent(Height, 95);
    //Menu Box
    OptionsRec.y = (TabRec.y + TabRec.h) - 50;
    OptionsRec.h = 40;
    OptionsRec.x = TabRec.x + 10;
    OptionsRec.w = 40;
    UsedBoxes.BoxObj[UsedBoxesCount] = &OptionsRec;
    UsedBoxes.X[UsedBoxesCount] = OptionsRec.x;
    UsedBoxes.Y[UsedBoxesCount] = OptionsRec.y;
    UsedBoxes.Width[UsedBoxesCount] = OptionsRec.w;
    UsedBoxes.Height[UsedBoxesCount] = OptionsRec.h;
    UsedBoxes.RGB[UsedBoxesCount].R = 63;
    UsedBoxes.RGB[UsedBoxesCount].G = 61;
    UsedBoxes.RGB[UsedBoxesCount].B = 71;
    OptionsRecIndex = UsedBoxesCount;
    UsedBoxesCount++;
    //Tabs
    for(int i=0;i<4;i++)
    {
        Tabs[i].y = (TabRec.y + 40) + 40 * i;
        Tabs[i].x = TabRec.x;
        Tabs[i].w = TabRec.w;
        Tabs[i].h = 30;
        UsedBoxes.BoxObj[UsedBoxesCount] = &Tabs[i];
        UsedBoxes.X[UsedBoxesCount] = Tabs[i].x;
        UsedBoxes.Y[UsedBoxesCount] = Tabs[i].y;
        UsedBoxes.Width[UsedBoxesCount] = Tabs[i].w;
        UsedBoxes.Height[UsedBoxesCount] = Tabs[i].h;
        UsedBoxes.RGB[UsedBoxesCount].R = 63;//63, 61, 71
        UsedBoxes.RGB[UsedBoxesCount].G = 61;
        UsedBoxes.RGB[UsedBoxesCount].B = 71;
        TabsIndex[i] = UsedBoxesCount;
        UsedBoxesCount++;
    }
    //Main LeftBox
    MainRec.x = TabRec.x + TabRec.w + 20;
    MainRec.y = 20;
    MainRec.w = Width - TabRec.w - 85;
    MainRec.h = CalcPercent(Height, 95);
    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void *appstate)
{
    SDL_SetRenderDrawColor(renderer, Bg_Color.R, Bg_Color.G, Bg_Color.B,SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);
    int Height, Width;
    SDL_GetWindowSizeInPixels(window, &Width, &Height);
    //Tab Menu & CO
    TabRec.x = 30;
    TabRec.y = 20;
    TabRec.w = CalcPercent(Width, 20);
    TabRec.h = CalcPercent(Height, 95);
    SDL_SetRenderDrawColor(renderer,53, 51, 61, SDL_ALPHA_OPAQUE);
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
    UsedBoxes.X[OptionsRecIndex] = OptionsRec.x;
    UsedBoxes.Y[OptionsRecIndex] = OptionsRec.y;
    UsedBoxes.Width[OptionsRecIndex] = OptionsRec.w;
    UsedBoxes.Height[OptionsRecIndex] = OptionsRec.h;
    SDL_SetRenderDrawColor(renderer, UsedBoxes.RGB[OptionsRecIndex].R, UsedBoxes.RGB[OptionsRecIndex].G, UsedBoxes.RGB[OptionsRecIndex].B,SDL_ALPHA_OPAQUE);
    SDL_RenderTexture(renderer, CogTexture, NULL, &OptionsRec);

    // -> Tabs
    for(int i=0;i<4;i++)
    {
        Tabs[i].y = (TabRec.y + 40) + 40 * i;
        Tabs[i].x = TabRec.x;
        Tabs[i].w = TabRec.w;
        Tabs[i].h = 30;
        UsedBoxes.X[TabsIndex[i]] = Tabs[i].x;
        UsedBoxes.Y[TabsIndex[i]] = Tabs[i].y;
        UsedBoxes.Width[TabsIndex[i]] = Tabs[i].w;
        UsedBoxes.Height[TabsIndex[i]] = Tabs[i].h;
        SDL_SetRenderDrawColor(renderer, UsedBoxes.RGB[TabsIndex[i]].R, UsedBoxes.RGB[TabsIndex[i]].G, UsedBoxes.RGB[TabsIndex[i]].B, 255);
        SDL_RenderFillRect(renderer,&Tabs[i]);
        SDL_SetRenderDrawColor(renderer, 175, 175, 175, SDL_ALPHA_OPAQUE);
        SDL_RenderDebugText(renderer, TabRec.x + 9, Tabs[i].y+ 13, StringArrays[i]);
    }

    //Main Screen & Co
    MainRec.x = TabRec.x + TabRec.w + 20;
    MainRec.y = 20;
    MainRec.w = Width - TabRec.w - 85;
    MainRec.h = CalcPercent(Height, 95);
    SDL_SetRenderDrawColor(renderer, 53, 51, 61, SDL_ALPHA_OPAQUE);
    SDL_RenderFillRect(renderer,&MainRec);
    //->Inside Screen
    SDL_RenderPresent(renderer);
    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
{
    if (event->type == SDL_EVENT_QUIT)
    {
        return SDL_APP_SUCCESS;
    }
    if(event->type == SDL_EVENT_MOUSE_MOTION)
    {
        MPosition[0] = event->motion.x;
        MPosition[1] = event->motion.y;
        if(MouseHover.HoverState)
        {
            if(MPosition[0] <= MouseHover.ContextBounding_X1 || MPosition[0] >= MouseHover.ContextBounding_X2 || MPosition[1] <= MouseHover.ContextBounding_Y1 || MPosition[1] >= MouseHover.ContextBounding_Y2)
            {
                UsedBoxes.RGB[MouseHover.Index].R = MouseHover.ItemRGB.R;
                UsedBoxes.RGB[MouseHover.Index].G = MouseHover.ItemRGB.G;
                UsedBoxes.RGB[MouseHover.Index].B = MouseHover.ItemRGB.B;

                MouseHover.ItemRGB.R = 0;
                MouseHover.ItemRGB.G = 0;
                MouseHover.ItemRGB.B = 0;
                MouseHover.ContextBounding_X1 = 0;
                MouseHover.ContextBounding_X2 = 0;
                MouseHover.ContextBounding_Y1 = 0;
                MouseHover.ContextBounding_Y2 = 0;
                MouseHover.HoverState = false;
            }
        }
        for(int i=0;i<=UsedBoxesCount;i++)
        {
            if(MPosition[0] >=UsedBoxes.X[i] && MPosition[0] <=UsedBoxes.X[i] + UsedBoxes.Width[i])//Correct Horizontal
            {
                if(MPosition[1] >=UsedBoxes.Y[i] && MPosition[1] <=UsedBoxes.Y[i] + UsedBoxes.Height[i])
                {
                    if(!MouseHover.HoverState)
                    {
                        MouseHover.HoverState = true;
                        MouseHover.ItemRGB.R = UsedBoxes.RGB[i].R;
                        MouseHover.ItemRGB.G = UsedBoxes.RGB[i].G;
                        MouseHover.ItemRGB.B = UsedBoxes.RGB[i].B;
                        MouseHover.ContextBounding_X1 = UsedBoxes.X[i];
                        MouseHover.ContextBounding_X2 = UsedBoxes.X[i] + UsedBoxes.Width[i];
                        MouseHover.ContextBounding_Y1 = UsedBoxes.Y[i];
                        MouseHover.ContextBounding_Y2 = UsedBoxes.Y[i] + UsedBoxes.Height[i];
                        MouseHover.Index = i;
                        UsedBoxes.RGB[i].R = 63;
                        UsedBoxes.RGB[i].G = 82;
                        UsedBoxes.RGB[i].B = 52;
                    }
                }
            }
        }
    }
    if(event->type == SDL_EVENT_MOUSE_BUTTON_DOWN)
    {
        for(int i=0;i<=UsedBoxesCount;i++)
        {
            if(MPosition[0] >=UsedBoxes.X[i] && MPosition[0] <=UsedBoxes.X[i] + UsedBoxes.Width[i])//Correct Horizontal
            {
                if(MPosition[1] >=UsedBoxes.Y[i] && MPosition[1] <=UsedBoxes.Y[i] + UsedBoxes.Height[i])
                {
                    printf("Press \n");
                }
            }
        }
    }
    return SDL_APP_CONTINUE;  /* carry on with the program! */
}

void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
    /* SDL will clean up the window/renderer for us. */
}

// To-do
// Replace PNGs with SVGs
// (Done) Add Interactivity
// Replace Debug Fonts with actual fonts

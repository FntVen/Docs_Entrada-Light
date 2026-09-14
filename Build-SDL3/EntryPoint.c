#include "Lib/HEHelper&Maker.h"
#include <SDL3/SDL_init.h>

static SDL_Texture *CogTexture = NULL;
int THeight;
int TWidth;
static SDL_FRect TabRec;
int TabRecIndex;
static SDL_FRect MainRec;
int MainRecIndex;
static SDL_FRect OptionsRec;
int OptionsRecIndex;
static SDL_FRect Tabs[5];//Cliente - Solar - Local - Contrato - Planta Baixa
int TabsIndex[5];
static char *StringArrays[5] = {"Cliente","Solar","Local","Contrato","P. Baixa"};

Color HightLightColor[5];//(0)Black | (1) Green | (2) Purple
int UsedBoxesCount = 0;
typedef struct
{
    SDL_FRect *BoxObj[100];
    float X[100];
    float Y[100];
    float Width[100];
    float Height[100];
    bool ColorSwitch[100];
    Color RGB[100];
    int ColorIndex[100];
    SDL_AppResult *EventFunc[100];
}SelBoxes;
SelBoxes GeneralInteract;
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

#define WnHeight 600
#define WnWidth 1200

#define ClientMen 0
#define SolarMen 1
#define LocalMen 2
#define ContractMen 3
#define MainMen 5
#define FinalMen 4
int CurrentMenu = MainMen;

static float CalcPercent(float NUM, float PERCENT)
{
    float Result = (NUM * PERCENT)/100;
    return Result;
}

RadioButton RBtns[3] = {
    (RadioButton)
    {
        .BackColor.R=255,.BackColor.G=255,.BackColor.B=255,.BackColor.A=255,
        .FrontColor.R=0,.FrontColor.G=0,.FrontColor.B=0,.FrontColor.A=255,
        .Size = 10,
        .State = false,
    }
};
int Radio_BoxIndex[3];
static void MainMenu()
{
    SDL_SetRenderDrawColor(Render, 120, 120, 120, 100);
    SDL_FRect BannerBar = {
        .x = MainRec.x,
        .w = MainRec.w,
        .y = MainRec.y + 40,
        .h = 70
    };
    SDL_FRect BannerSide = {
        .y = (MainRec.y + MainRec.h) - 70,
        .h = 50,
        .x = MainRec.x,
        .w = CalcPercent(MainRec.w, 40)
    };
    SDL_RenderFillRect(Render, &BannerBar);
    SDL_RenderFillRect(Render, &BannerSide);
    SDL_SetRenderScale(Render, 1.5, 1.5);
    SDL_SetRenderDrawColor(Render, 255, 255, 255, 255);
    SDL_RenderDebugText(Render,(BannerBar.x + CalcPercent(BannerBar.w, 15))/1.5, (BannerBar.y + (BannerBar.h/2))/1.5, "Selecione a Documentação que Deseja Criar");
    SDL_SetRenderScale(Render, 1, 1);
    SDL_RenderDebugText(Render, BannerSide.x + CalcPercent(BannerSide.w, 10), BannerSide.y + (BannerSide.h/2), "Use as Abas ao Lado para Preencher");
    RBtns[0].XY[0] = MainRec.x + 30;
    RBtns[0].XY[1] = MainRec.h/3;
    RBtns[1].XY[0] = MainRec.x + 30;
    RBtns[1].XY[1] = MainRec.h/2;
    RBtns[1].XY[0] = MainRec.x + 30;
    RBtns[1].XY[1] = MainRec.h/1.5;
    SDL_RenderRadioBtn(RBtns[0]);
}
static void FinalMenu()
{

}
SDL_AppResult TestFunc()
{
    printf("Teste Funcionou\n");
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
    SDL_SetRenderLogicalPresentation(Render, WnWidth, WnHeight, SDL_LOGICAL_PRESENTATION_DISABLED);
    HightLightColor[0].A = 0;
    HightLightColor[0].R = 0;
    HightLightColor[0].G = 0;
    HightLightColor[0].B = 0;

    HightLightColor[1].A = 255;
    HightLightColor[1].R = 63;
    HightLightColor[1].G = 82;
    HightLightColor[1].B = 52;

    HightLightColor[2].A = 255;
    HightLightColor[2].R = 63;
    HightLightColor[2].G = 52;
    HightLightColor[2].B = 82;

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
    CogTexture = SDL_CreateTextureFromSurface(Render, Plane);
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

    //Tabs
    for(int i=0;i<4;i++)
    {
        GeneralInteract.BoxObj[UsedBoxesCount] = &Tabs[i];
        GeneralInteract.X[UsedBoxesCount] = Tabs[i].x;
        GeneralInteract.Y[UsedBoxesCount] = Tabs[i].y;
        GeneralInteract.Width[UsedBoxesCount] = Tabs[i].w;
        GeneralInteract.Height[UsedBoxesCount] = Tabs[i].h;
        GeneralInteract.ColorSwitch[UsedBoxesCount] = true;
        GeneralInteract.RGB[UsedBoxesCount].R = 63;//63, 61, 71
        GeneralInteract.RGB[UsedBoxesCount].G = 61;
        GeneralInteract.RGB[UsedBoxesCount].B = 71;
        GeneralInteract.ColorIndex[UsedBoxesCount] = 1;
        TabsIndex[i] = UsedBoxesCount;
        UsedBoxesCount++;
    }
    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void *appstate)
{
    SDL_SetRenderDrawColor(Render, 43, 41, 51,SDL_ALPHA_OPAQUE);
    SDL_RenderClear(Render);
    int Height, Width;
    SDL_GetWindowSizeInPixels(window, &Width, &Height);
    //Tab Menu & CO
    TabRec.x = 30;
    TabRec.y = 20;
    TabRec.w = CalcPercent(Width, 20);
    TabRec.h = CalcPercent(Height, 95);
    SDL_SetRenderDrawColor(Render,53, 51, 61, SDL_ALPHA_OPAQUE);
    SDL_RenderFillRect(Render,&TabRec);

    SDL_SetRenderDrawColor(Render, 255, 255, 255, 255);
    SDL_RenderLine(Render, TabRec.x + 6, TabRec.y + 24.8, TabRec.w , TabRec.y + 24.5);
    SDL_SetRenderScale(Render, 1.5, 1.5);
    SDL_RenderDebugText(Render, 40,22,"Categorias");
    SDL_SetRenderScale(Render, 1, 1);

    OptionsRec.y = (TabRec.y + TabRec.h) - 50;
    OptionsRec.h = 40;
    OptionsRec.x = TabRec.x + 10;
    OptionsRec.w = 40;
    GeneralInteract.X[OptionsRecIndex] = OptionsRec.x;
    GeneralInteract.Y[OptionsRecIndex] = OptionsRec.y;
    GeneralInteract.Width[OptionsRecIndex] = OptionsRec.w;
    GeneralInteract.Height[OptionsRecIndex] = OptionsRec.h;
    GeneralInteract.ColorSwitch[OptionsRecIndex] = false;
    //GeneralInteract.EventFunc[OptionsRecIndex] = &TestFunc();
    SDL_SetRenderDrawColor(Render, GeneralInteract.RGB[OptionsRecIndex].R, GeneralInteract.RGB[OptionsRecIndex].G, GeneralInteract.RGB[OptionsRecIndex].B,SDL_ALPHA_OPAQUE);
    SDL_RenderTexture(Render, CogTexture, NULL, &OptionsRec);

    // -> Tabs
    for(int i=0;i<4;i++)
    {
        Tabs[i].y = (TabRec.y + 40) + 40 * i;
        Tabs[i].x = TabRec.x;
        Tabs[i].w = TabRec.w;
        Tabs[i].h = 30;
        GeneralInteract.X[TabsIndex[i]] = Tabs[i].x;
        GeneralInteract.Y[TabsIndex[i]] = Tabs[i].y;
        GeneralInteract.Width[TabsIndex[i]] = Tabs[i].w;
        GeneralInteract.Height[TabsIndex[i]] = Tabs[i].h;
        GeneralInteract.ColorSwitch[TabsIndex[i]] = true;
        SDL_SetRenderDrawColor(Render, GeneralInteract.RGB[TabsIndex[i]].R, GeneralInteract.RGB[TabsIndex[i]].G, GeneralInteract.RGB[TabsIndex[i]].B, 255);
        SDL_RenderFillRect(Render,&Tabs[i]);
        SDL_SetRenderDrawColor(Render, 175, 175, 175, SDL_ALPHA_OPAQUE);
        SDL_RenderDebugText(Render, TabRec.x + 9, Tabs[i].y+ 13, StringArrays[i]);
    }

    //Main Screen & Co
    MainRec.x = TabRec.x + TabRec.w + 20;
    MainRec.y = 20;
    MainRec.w = Width - TabRec.w - 85;
    MainRec.h = CalcPercent(Height, 95);
    SDL_SetRenderDrawColor(Render, 53, 51, 61, SDL_ALPHA_OPAQUE);
    SDL_RenderFillRect(Render,&MainRec);
    //->Inside Screen
    switch (CurrentMenu)
    {
        case MainMen:
            MainMenu();
            break;
        case ClientMen:
            break;
        case SolarMen:
            break;
        case LocalMen:
            break;
        case ContractMen:
            break;
        case FinalMen:
            FinalMenu();
            break;

    }

    SDL_RenderPresent(Render);
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
                if(GeneralInteract.ColorSwitch[MouseHover.Index])
                {
                    GeneralInteract.RGB[MouseHover.Index].R = MouseHover.ItemRGB.R;
                    GeneralInteract.RGB[MouseHover.Index].G = MouseHover.ItemRGB.G;
                    GeneralInteract.RGB[MouseHover.Index].B = MouseHover.ItemRGB.B;

                    MouseHover.ItemRGB.R = 0;
                    MouseHover.ItemRGB.G = 0;
                    MouseHover.ItemRGB.B = 0;
                }
                MouseHover.ContextBounding_X1 = 0;
                MouseHover.ContextBounding_X2 = 0;
                MouseHover.ContextBounding_Y1 = 0;
                MouseHover.ContextBounding_Y2 = 0;
                MouseHover.HoverState = false;
            }
        }
        for(int i=0;i<=UsedBoxesCount;i++)
        {
            if(MPosition[0] >=GeneralInteract.X[i] && MPosition[0] <=GeneralInteract.X[i] + GeneralInteract.Width[i])//Correct Horizontal
            {
                if(MPosition[1] >=GeneralInteract.Y[i] && MPosition[1] <=GeneralInteract.Y[i] + GeneralInteract.Height[i])
                {
                    if(!MouseHover.HoverState)
                    {
                        MouseHover.HoverState = true;
                        if(GeneralInteract.ColorSwitch[i])
                        {
                            MouseHover.ItemRGB.R = GeneralInteract.RGB[i].R;
                            MouseHover.ItemRGB.G = GeneralInteract.RGB[i].G;
                            MouseHover.ItemRGB.B = GeneralInteract.RGB[i].B;
                            GeneralInteract.RGB[i].R = HightLightColor[GeneralInteract.ColorIndex[i]].R;
                            GeneralInteract.RGB[i].G = HightLightColor[GeneralInteract.ColorIndex[i]].G;
                            GeneralInteract.RGB[i].B = HightLightColor[GeneralInteract.ColorIndex[i]].B;
                        }
                        MouseHover.ContextBounding_X1 = GeneralInteract.X[i];
                        MouseHover.ContextBounding_X2 = GeneralInteract.X[i] + GeneralInteract.Width[i];
                        MouseHover.ContextBounding_Y1 = GeneralInteract.Y[i];
                        MouseHover.ContextBounding_Y2 = GeneralInteract.Y[i] + GeneralInteract.Height[i];
                        MouseHover.Index = i;

                    }
                }
            }
        }
    }
    if(event->type == SDL_EVENT_MOUSE_BUTTON_DOWN)
    {
        for(int i=0;i<=UsedBoxesCount;i++)
        {
            if(MPosition[0] >=GeneralInteract.X[i] && MPosition[0] <=GeneralInteract.X[i] + GeneralInteract.Width[i])//Correct Horizontal
            {
                if(MPosition[1] >=GeneralInteract.Y[i] && MPosition[1] <=GeneralInteract.Y[i] + GeneralInteract.Height[i])
                {

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

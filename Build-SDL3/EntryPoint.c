#include "Lib/HEHelper&Maker.h"
#include <SDL3/SDL_events.h>
//Start Main Elements
TTF_Font *MainFont;
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
bool _EnabledTabs[5] = {false,false,false,false};
bool _Homologação = false;
bool _Contrato = false;
bool _PBaixa = false;
static char *StringArrays[5] = {"Cliente","Solar","Local","Contrato","P. Baixa"};

//End Main Elements

typedef union
{
    int NUMBER;
    char WORDS[256];
    bool CHOICE;
}EventParams1;
typedef union
{
    int NUMBER;
    char WORDS[256];
    bool CHOICE;
}EventParams2;//Currently copy of EventParam1 but realistically it should be more complementary
enum Function
{
  SWITCHTABS,
  ENABLETABS
};

//Start Structs and Reusable Elements
int ScrollIndex = 0;
int InteractiveCount = 0;
typedef struct
{
    void *BoxObj[100];
    float X[100];
    float Y[100];
    float Width[100];
    float Height[100];
    bool Enabled[100];
    Color BaseColor[100];
    Color HighLightColor[100];
    Color DisabledColor[100];
    Color UsedColor[100];
    int Function[100];
    EventParams1 Param1[100];
    EventParams2 Param2[100];
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
}MHover;
MHover MouseHover = {.HoverState = false};


void CleanFonts()//Erases all used fonts in the main hub and clears the "one use bool"
{
    if(LabelCount == 0)
    {
        return;
    }
    for(int i=0;i<LabelCount;i++)
    {
        SDL_DestroyTexture(ActiveLabels[i].Texture);
        ActiveLabels[i].FontSize = 0;
        ActiveLabels[i].Height = 0;
        ActiveLabels[i].Width = 0;
        ActiveLabels[i].X = 0;
        ActiveLabels[i].Y = 0;
        ActiveLabels[i].RGB = (Color){.A=0,.R=0,.G=0,.B=0};
        ActiveLabels[i].String = "";
    }
    LabelCount = 0;
    HubLoaded = false;
}

//End Structs, Reusable Elements and Functions
//Start Button Functions
SDL_AppResult SwitchTabs(EventParams1 Param1, EventParams2 Param2)
{
    //Param1: Index| Use index to decide how to switch CurrentMenu
    SDL_AppResult Result = SDL_APP_FAILURE;
    switch(Param1.NUMBER)
    {
        case 0:
            if(_Homologação)
            {
                printf("Switch to tab %d\n",Param1.NUMBER);
            }
            Result = SDL_APP_CONTINUE;
            break;
        case 1:
            if(_Homologação)
            {
                printf("Switch to tab %d\n",Param1.NUMBER);
            }
            Result = SDL_APP_CONTINUE;
            break;
        case 2:
            if(_Homologação)
            {
                printf("Switch to tab %d\n",Param1.NUMBER);
            }
            Result = SDL_APP_CONTINUE;
            break;
        case 3:
            if(_Contrato)
            {
                printf("Switch to tab %d\n",Param1.NUMBER);
            }
            Result = SDL_APP_CONTINUE;
            break;
    }//PBaixa não existe ainda
    return Result;
}
SDL_AppResult EnableTabs(EventParams1 Param1, EventParams2 Param2)
{
    //Param1 Which tab to modify| Using kwy words to indicate which tab to change by changing a variable that also affects the radio button visuals
    //Param2 Possible necessary cleanup info if there is any data on that tab?
    SDL_AppResult Result = SDL_APP_FAILURE;
    if(Param1.NUMBER == 0)
    {
        if(_Homologação)
        {
            _Homologação = false;
            _EnabledTabs[0] = false;
            _EnabledTabs[1] = false;
            _EnabledTabs[2] = false;
        }
        else
        {
            _Homologação = true;
            _EnabledTabs[0] = true;
            _EnabledTabs[1] = true;
            _EnabledTabs[2] = true;
        }
        //printf("_Homologação: %b\n",_Homologação);
        Result = SDL_APP_CONTINUE;
    }
    if(Param1.NUMBER == 1)
    {
        if(_Contrato)
        {
            _Contrato = false;
            _EnabledTabs[3] = false;
        }
        else
        {
            _Contrato = true;
            _EnabledTabs[3] = true;
        }
        Result = SDL_APP_CONTINUE;
    }
    if(Param1.NUMBER == 2)
    {
        if(_PBaixa)
        {
            _PBaixa = false;
            _EnabledTabs[4] = false;
        }
        else
        {
            _PBaixa = true;
            _EnabledTabs[4] = true;
        }
        Result = SDL_APP_CONTINUE;
    }
    for(int i = 0;i<=4;i++)
    {
        if(_EnabledTabs[i])
        {
            GeneralInteract.UsedColor[TabsIndex[i]].A = GeneralInteract.BaseColor[TabsIndex[i]].A;
            GeneralInteract.UsedColor[TabsIndex[i]].R = GeneralInteract.BaseColor[TabsIndex[i]].R;
            GeneralInteract.UsedColor[TabsIndex[i]].G = GeneralInteract.BaseColor[TabsIndex[i]].G;
            GeneralInteract.UsedColor[TabsIndex[i]].B = GeneralInteract.BaseColor[TabsIndex[i]].B;
        }
        else
        {
            GeneralInteract.UsedColor[TabsIndex[i]].A = GeneralInteract.DisabledColor[TabsIndex[i]].A;
            GeneralInteract.UsedColor[TabsIndex[i]].R = GeneralInteract.DisabledColor[TabsIndex[i]].R;
            GeneralInteract.UsedColor[TabsIndex[i]].G = GeneralInteract.DisabledColor[TabsIndex[i]].G;
            GeneralInteract.UsedColor[TabsIndex[i]].B = GeneralInteract.DisabledColor[TabsIndex[i]].B;
        }
    }
    return Result;
}
SDL_AppResult ExecFunc(EventParams1 Param1, EventParams2 Param2, int FunctionName)
{
    SDL_AppResult Result = SDL_APP_FAILURE;
    switch(FunctionName)
    {
        case SWITCHTABS:
            Result = SwitchTabs(Param1,Param2);
            break;
        case ENABLETABS:
            Result = EnableTabs(Param1,Param2);
            break;
    }
    return Result;
}
//End Button Functions
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
    },
    (RadioButton)
    {
        .BackColor.R=255,.BackColor.G=255,.BackColor.B=255,.BackColor.A=255,
        .FrontColor.R=0,.FrontColor.G=0,.FrontColor.B=0,.FrontColor.A=255,
        .Size = 10,
    },
    (RadioButton)
    {
        .BackColor.R=255,.BackColor.G=255,.BackColor.B=255,.BackColor.A=255,
        .FrontColor.R=0,.FrontColor.G=0,.FrontColor.B=0,.FrontColor.A=255,
        .Size = 10,
    }
};
int Radio_BoxIndex[3];
int Main_RadioIndex[3];
bool _Main_RBtns = false;
static SDL_AppResult MainMenu()
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
        .w = CalcPercent(MainRec.w, 80)
    };
    SDL_RenderFillRect(Render, &BannerBar);
    SDL_RenderFillRect(Render, &BannerSide);
    SDL_SetRenderDrawColor(Render, 255, 255, 255, 255);

    //SDL_RenderDebugText(Render, BannerSide.x + CalcPercent(BannerSide.w, 10), BannerSide.y + (BannerSide.h/2), "Use as Abas ao Lado para Preencher");
    RBtns[0].XY[0] = MainRec.x + 30;
    RBtns[0].XY[1] = MainRec.h/3;
    RBtns[0].State = _Homologação;
    RBtns[1].State = _Contrato;
    RBtns[1].XY[0] = MainRec.x + 30;
    RBtns[1].XY[1] = MainRec.h/2;
    RBtns[2].XY[0] = MainRec.x + 30;
    RBtns[2].XY[1] = MainRec.h/1.5;
    RBtns[2].State = _PBaixa;

    if(_Main_RBtns){goto Drawing_RadioBTNS;}
    for(int i = 0;i<4;i++)
    {
        GeneralInteract.BoxObj[InteractiveCount] = &RBtns[i];
        GeneralInteract.Function[InteractiveCount] = ENABLETABS;
        GeneralInteract.X[InteractiveCount] = RBtns[i].XY[0] - RBtns[i].Size;
        GeneralInteract.Width[InteractiveCount] = RBtns[i].Size*2;
        GeneralInteract.Y[InteractiveCount] = RBtns[i].XY[1] - RBtns[i].Size;
        GeneralInteract.Height[InteractiveCount] = RBtns[i].Size*2;
        GeneralInteract.Param1[InteractiveCount].NUMBER = i;
        Main_RadioIndex[i] = InteractiveCount;
        InteractiveCount++;
    }
    _Main_RBtns = true;
    Drawing_RadioBTNS:
    SDL_RenderRadioBtn(RBtns[0]);
    SDL_RenderRadioBtn(RBtns[1]);
    SDL_RenderRadioBtn(RBtns[2]);
    if(!HubLoaded)
    {
        ActiveLabels[LabelCount] = (Text){
            .X =  MainRec.x + CalcPercent(BannerBar.w, 25),
            .Y =  (MainRec.y + 40) + (BannerBar.h/2.9),
            .RGB = {.A=255,.R=255,.G=255,.B=255},
            .String = "Selecione a Documentação que Deseja Criar",
            .FontSize = 24
        };
        LabelCount++;

        ActiveLabels[LabelCount] = (Text){
            .X = MainRec.x + CalcPercent(MainRec.w, 1.7),
            .Y = (MainRec.y + MainRec.h) - 58,
            .String = "Use as Abas ao Lado para Preencher os dados referentes a documentação",
            .RGB = {.A=255,.R=255,.G=255,.B=255},
            .FontSize = 20
        };
        LabelCount++;
    }
    SDL_AppResult Result =  RenderFont();
    if(Result != SDL_APP_CONTINUE)
    {
        return Result;
    }
    return SDL_APP_CONTINUE;

}
static void FinalMenu()
{
    //
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
    if(!TTF_Init())
    {
        SDL_Log("Error in the Font Loader");
        return SDL_APP_FAILURE;
    }
    TTF_Font *Font = TTF_OpenFont("Resources/Roboto-Regular.ttf", 24);
    if(!Font)
    {
        SDL_Log("Error Loading Font");
        return SDL_APP_FAILURE;
    }
    MainFont = Font;

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
    for(int i=0;i<=4;i++)
    {
        GeneralInteract.BoxObj[InteractiveCount] = &Tabs[i];
        GeneralInteract.X[InteractiveCount] = Tabs[i].x;
        GeneralInteract.Y[InteractiveCount] = Tabs[i].y;
        GeneralInteract.Width[InteractiveCount] = Tabs[i].w;
        GeneralInteract.Height[InteractiveCount] = Tabs[i].h;
        GeneralInteract.BaseColor[InteractiveCount].A = 255;
        GeneralInteract.BaseColor[InteractiveCount].R = 63;
        GeneralInteract.BaseColor[InteractiveCount].G = 61;
        GeneralInteract.BaseColor[InteractiveCount].B = 71;
        GeneralInteract.HighLightColor[InteractiveCount].A = 255;
        GeneralInteract.HighLightColor[InteractiveCount].R = 63;
        GeneralInteract.HighLightColor[InteractiveCount].G = 82;
        GeneralInteract.HighLightColor[InteractiveCount].B = 52;
        GeneralInteract.DisabledColor[InteractiveCount].A = 255;
        GeneralInteract.DisabledColor[InteractiveCount].R = 43;
        GeneralInteract.DisabledColor[InteractiveCount].G = 41;
        GeneralInteract.DisabledColor[InteractiveCount].B = 51;
        GeneralInteract.UsedColor[InteractiveCount].A = GeneralInteract.DisabledColor[InteractiveCount].A;
        GeneralInteract.UsedColor[InteractiveCount].R = GeneralInteract.DisabledColor[InteractiveCount].R;
        GeneralInteract.UsedColor[InteractiveCount].G = GeneralInteract.DisabledColor[InteractiveCount].G;
        GeneralInteract.UsedColor[InteractiveCount].B = GeneralInteract.DisabledColor[InteractiveCount].B;
        GeneralInteract.Enabled[InteractiveCount] = false;
        TabsIndex[i] = InteractiveCount;
        GeneralInteract.Param1[InteractiveCount].NUMBER = i;
        GeneralInteract.Function[InteractiveCount] = SWITCHTABS;
        InteractiveCount++;
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
    /*
    Text TXT_Categorias ={
        .String = "Categorias",
        .RGB = {.A=255,.R=255,.G=255,.B=25},
        .X = 40,
        .Y = 12
    };
    */

    //LoadFonts(15);
    //SDL_RenderDebugText(Render, 40,22,"Categorias");
    SDL_SetRenderScale(Render, 1, 1);

    OptionsRec.y = (TabRec.y + TabRec.h) - 50;
    OptionsRec.h = 40;
    OptionsRec.x = TabRec.x + 10;
    OptionsRec.w = 40;
    GeneralInteract.X[OptionsRecIndex] = OptionsRec.x;
    GeneralInteract.Y[OptionsRecIndex] = OptionsRec.y;
    GeneralInteract.Width[OptionsRecIndex] = OptionsRec.w;
    GeneralInteract.Height[OptionsRecIndex] = OptionsRec.h;
    SDL_SetRenderDrawColor(Render, GeneralInteract.BaseColor[OptionsRecIndex].R, GeneralInteract.BaseColor[OptionsRecIndex].G, GeneralInteract.BaseColor[OptionsRecIndex].B,SDL_ALPHA_OPAQUE);
    SDL_RenderTexture(Render, CogTexture, NULL, &OptionsRec);

    // -> Tabs
    for(int i=0;i<4;i++)//Redo colors every frame
    {
        Tabs[i].y = (TabRec.y + 40) + 40 * i;
        Tabs[i].x = TabRec.x;
        Tabs[i].w = TabRec.w;
        Tabs[i].h = 30;
        GeneralInteract.X[TabsIndex[i]] = Tabs[i].x;
        GeneralInteract.Y[TabsIndex[i]] = Tabs[i].y;
        GeneralInteract.Width[TabsIndex[i]] = Tabs[i].w;
        GeneralInteract.Height[TabsIndex[i]] = Tabs[i].h;
        GeneralInteract.Enabled[TabsIndex[i]] = _EnabledTabs[i];
        GeneralInteract.Param1[TabsIndex[i]].NUMBER = i;
        GeneralInteract.Function[TabsIndex[i]] = SWITCHTABS;
        SDL_SetRenderDrawColor(Render, GeneralInteract.UsedColor[TabsIndex[i]].R, GeneralInteract.UsedColor[TabsIndex[i]].G, GeneralInteract.UsedColor[TabsIndex[i]].B, GeneralInteract.UsedColor[TabsIndex[i]].A);
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
    SDL_AppResult Res;
    //->Inside Screen
    switch (CurrentMenu)
    {
        case MainMen:
            Res = MainMenu();
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
    return Res;
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
                if(GeneralInteract.Enabled[MouseHover.Index])
                {
                    GeneralInteract.UsedColor[MouseHover.Index].A = GeneralInteract.BaseColor[MouseHover.Index].A;
                    GeneralInteract.UsedColor[MouseHover.Index].R = GeneralInteract.BaseColor[MouseHover.Index].R;
                    GeneralInteract.UsedColor[MouseHover.Index].G = GeneralInteract.BaseColor[MouseHover.Index].G;
                    GeneralInteract.UsedColor[MouseHover.Index].B = GeneralInteract.BaseColor[MouseHover.Index].B;
                    MouseHover.ContextBounding_X1 = 0;
                    MouseHover.ContextBounding_X2 = 0;
                    MouseHover.ContextBounding_Y1 = 0;
                    MouseHover.ContextBounding_Y2 = 0;
                    MouseHover.HoverState = false;
                }
            }
        }
        for(int i=0;i<=InteractiveCount;i++)
        {
            if(MPosition[0] >=GeneralInteract.X[i] && MPosition[0] <=GeneralInteract.X[i] + GeneralInteract.Width[i])//Correct Horizontal
            {
                if(MPosition[1] >=GeneralInteract.Y[i] && MPosition[1] <=GeneralInteract.Y[i] + GeneralInteract.Height[i])
                {
                    if(!MouseHover.HoverState)
                    {
                        if(GeneralInteract.Enabled[i])
                        {
                            MouseHover.HoverState = true;
                            GeneralInteract.UsedColor[i].A = GeneralInteract.HighLightColor[i].A;
                            GeneralInteract.UsedColor[i].R = GeneralInteract.HighLightColor[i].R;
                            GeneralInteract.UsedColor[i].G = GeneralInteract.HighLightColor[i].G;
                            GeneralInteract.UsedColor[i].B = GeneralInteract.HighLightColor[i].B;
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
    }
    if(event->type == SDL_EVENT_MOUSE_BUTTON_DOWN)
    {
        for(int i=0;i<=InteractiveCount;i++)
        {
            if(MPosition[0] >=GeneralInteract.X[i] && MPosition[0] <=GeneralInteract.X[i] + GeneralInteract.Width[i])//Correct Horizontal
            {
                if(MPosition[1] >=GeneralInteract.Y[i] && MPosition[1] <=GeneralInteract.Y[i] + GeneralInteract.Height[i])
                {
                    SDL_AppResult Result = ExecFunc(GeneralInteract.Param1[i], GeneralInteract.Param2[i], GeneralInteract.Function[i]);
                    if(Result != SDL_APP_CONTINUE)
                    {
                        return Result;
                    }
                }
            }
        }
    }
    if(event->type == SDL_EVENT_WINDOW_RESIZED ||event->type == SDL_EVENT_WINDOW_MAXIMIZED)
    {
        CleanFonts();
    }
    return SDL_APP_CONTINUE;  /* carry on with the program! */
}

void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
    /* SDL will clean up the window/renderer for us. */
}

// To-do
// Replace Debug Fonts with actual fonts

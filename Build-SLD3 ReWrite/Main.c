#include "Lib/HEHelper&Maker.h"

int WnWidth = 1200;
int WnHeight = 600;
enum Screens{
    FinalScreen,
    InitScreen,
    Cliente,
    Solar,
    Local,
    Contrato,
    PBaixa
};
int CurrentScreen = InitScreen;
bool _Cliente = false;
bool _Solar = false;
bool _Local = false;
bool _Contrato = false;
bool _PBaixa = false;
bool _R_Homologação = false;
bool _R_Contrato = false;
bool _R_PBaixa = false;
HitBox HitBoxes[100] = {0};
typedef struct{
    Color Normal;
    Color HighLight;
    Color Click;
    Color Disabled;
    Color Used;
    SDL_FRect Box;
    int Function;
    EventParams Param;
}Tab;
Tab Tabs[5];
enum BtnFunctions{
    TABSELECT,
    RADIO_MENUENABLE
};
typedef struct{
    bool HoverTrigger;
    MousePos XY;
    HitBox Box;
}MHover;
MHover HoverHandler;
MousePos MouseState;
bool StaticLoaded = false;
int StaticLabelCount = 0;
Text StaticLabel[100];
bool _NewLoad = true;
//---------------------------------------------------------------------------------//
void TabHighLight(EventParams Param, int Function)
{
    int ScreenID = Param.NUMBER + 2;
    switch(ScreenID)
    {
        case Cliente:
            if(_Cliente)
            {
                if(HoverHandler.HoverTrigger)
                {
                    Tabs[0].Used = Tabs[0].HighLight;
                }
                else
                {
                    Tabs[0].Used = Tabs[0].Normal;
                }
            }
        break;
        case Solar:
            if(_Solar)
            {
                if(HoverHandler.HoverTrigger)
                {
                    Tabs[1].Used = Tabs[1].HighLight;
                }
                else
                {
                    Tabs[1].Used = Tabs[1].Normal;
                }
            }
        break;
        case Local:
            if(_Local)
            {
                if(HoverHandler.HoverTrigger)
                {
                    Tabs[2].Used = Tabs[2].HighLight;
                }
                else
                {
                    Tabs[2].Used = Tabs[2].Normal;
                }
            }
        break;
        case Contrato:
            if(_Contrato)
            {
                if(HoverHandler.HoverTrigger)
                {
                    Tabs[3].Used = Tabs[3].HighLight;
                }
                else
                {
                    Tabs[3].Used = Tabs[3].Normal;
                }
            }
        break;
        case PBaixa:
            if(_PBaixa)
            {
                if(HoverHandler.HoverTrigger)
                {
                    Tabs[4].Used = Tabs[4].HighLight;
                }
                else
                {
                    Tabs[4].Used = Tabs[4].Normal;
                }
            }
        break;
    }
}
SDL_AppResult TabSwitch(EventParams Param)
{
    int ScreenID = Param.NUMBER+2;
     switch(ScreenID)
    {
        case Cliente:
            if(_Cliente)
            {
                CurrentScreen = ScreenID;
            }
        break;
        case Solar:
            if(_Solar)
            {
                CurrentScreen = ScreenID;
            }
        break;
        case Local:
            if(_Local)
            {
                CurrentScreen = ScreenID;
            }
        break;
        case Contrato:
            if(_Contrato)
            {
                CurrentScreen = ScreenID;
            }
        break;
        case PBaixa:
            if(_PBaixa)
            {
                CurrentScreen = ScreenID;
            }
        break;
    }
    return SDL_APP_CONTINUE;
}
SDL_AppResult RADIO_TabEnable(EventParams Param)
{
    switch(Param.NUMBER)
    {
        case 0://Homologação
            if(!_R_Homologação)
            {
                _Cliente = true;
                _Solar = true;
                _Local = true;
                _R_Homologação = true;

                Tabs[0].Used = Tabs[0].Normal;
                Tabs[1].Used = Tabs[1].Normal;
                Tabs[2].Used = Tabs[2].Normal;
            }
            else
            {
                _Cliente = false;
                _Solar = false;
                _Local = false;
                _R_Homologação = false;

                Tabs[0].Used = Tabs[0].Disabled;
                Tabs[1].Used = Tabs[1].Disabled;
                Tabs[2].Used = Tabs[2].Disabled;
            }
        break;
        case 1://Contrato
        break;
        case 2://P.Baixa
        break;
    }

    return SDL_APP_CONTINUE;
}
void CleanPage()
{
    CleanFonts();
    for(int i=0;i<=HitboxCount; i++)
    {
        HitBoxes[HitboxCount].Function = 0;
        HitBoxes[HitboxCount].Parameter.NUMBER = 0;
        HitBoxes[HitboxCount].BoundingBox = (SDL_FRect){.x=0,.y=0,.w=0,.h=0};
    }
}
static float CalcPercent(float NUM, float PERCENT)
{
    float Result = (NUM * PERCENT)/100;
    return Result;
}
//---------------------------------------------------------------------------------//
SDL_AppResult InitStructs()
{
    for(int i=0;i<=4;i++)
    {
        Tabs[i].Normal = (Color){.A=255,.R=63,.G=61,.B=71};
        Tabs[i].HighLight = (Color){.A=255,.R=63,.G=82,.B=52};
        Tabs[i].Disabled = (Color){.A=255,.R=43,.G=41,.B=51};
        Tabs[i].Click = (Color){.A=255,.R=73,.G=71,.B=81};
        Tabs[i].Used = Tabs[i].Disabled;//Change for disabled as default
    }
    HoverHandler.HoverTrigger = false;
    HoverHandler.XY.X = 0;
    HoverHandler.XY.Y = 0;
    MouseState.X = 0;
    MouseState.Y = 0;
    return SDL_APP_CONTINUE;
}
#define TextCount_StaticEle 5
int ReferenceText_STATIC[TextCount_StaticEle] = {0};
SDL_AppResult StaticElements()
{
    SDL_SetRenderDrawColor(Render,53, 51, 61, SDL_ALPHA_OPAQUE);
    SDL_RenderFillRect(Render, &(SDL_FRect){.x=30,.y=20,.w=CalcPercent((float)WnWidth, 20),.h=(float)CalcPercent(WnHeight, 95)});
    SDL_RenderFillRect(Render, &(SDL_FRect){.x = (50+CalcPercent((float)WnWidth,20)),.y = 20,.w= WnWidth - (CalcPercent((float)WnWidth, 20)) - 85,.h=CalcPercent(WnHeight, 95)});

    for(int i=0;i<=4;i++)
    {
        Tabs[i].Box.x = 30;
        Tabs[i].Box.y = (20+40)+40*i;
        Tabs[i].Box.w = CalcPercent((float)WnWidth, 20);
        Tabs[i].Box.h = 30;
        SDL_SetRenderDrawColor(Render, Tabs[i].Used.R, Tabs[i].Used.G, Tabs[i].Used.B, Tabs[i].Used.A);
        SDL_RenderFillRect(Render,&Tabs[i].Box);
        if(!StaticLoaded)
        {
            HitBoxes[HitboxCount].BoundingBox = Tabs[i].Box;
            HitBoxes[HitboxCount].Function = TABSELECT;
            HitBoxes[HitboxCount].Parameter.NUMBER = i;
            HitboxCount++;
        }
    }
    if(!StaticLoaded)
    {
        SDL_AppResult ResultFontRender = SDL_APP_CONTINUE;
        Text TXT_Cliente = {
            .Y = Tabs[0].Box.y + CalcPercent(Tabs[0].Box.h, 20),
            .X = 35 + CalcPercent(Tabs[0].Box.w, 33),
            .FontSize=17,
            .RGB = {.A=255,.R=255,.G=255,.B=255},
            .String = "Cliente"
        };
        Text TXT_Solar = {
            .FontSize=17,
            .RGB = {.A=255,.R=255,.G=255,.B=255},
            .String = "Solar",
            .X = 35 + CalcPercent(Tabs[1].Box.w, 35),
            .Y = Tabs[1].Box.y + CalcPercent(Tabs[1].Box.h, 20),
        };
        Text TXT_Local = {
            .FontSize=17,
            .RGB = {.A=255,.R=255,.G=255,.B=255},
            .String = "Local",
            .X = 35 + CalcPercent(Tabs[2].Box.w, 35),
            .Y = Tabs[2].Box.y + CalcPercent(Tabs[2].Box.h, 20),
        };
        Text TXT_Contrato = {
            .FontSize=17,
            .RGB = {.A=255,.R=255,.G=255,.B=255},
            .String = "Contrato",
            .X = 35 + CalcPercent(Tabs[3].Box.w, 30),
            .Y = Tabs[3].Box.y + CalcPercent(Tabs[3].Box.h, 20),
        };
        Text TXT_PBaixa = {
            .FontSize=17,
            .RGB = {.A=255,.R=255,.G=255,.B=255},
            .String = "P.Baixa",
            .X = 35 + CalcPercent(Tabs[4].Box.w, 32),
            .Y = Tabs[4].Box.y + CalcPercent(Tabs[4].Box.h, 20),
        };
        ReferenceText_STATIC[0] = LabelCount;
        ActiveLabels[LabelCount] = TXT_Cliente;
        LabelCount++;
        ReferenceText_STATIC[1] = LabelCount;
        ActiveLabels[LabelCount] = TXT_Solar;
        LabelCount++;
        ReferenceText_STATIC[2] = LabelCount;
        ActiveLabels[LabelCount] = TXT_Local;
        LabelCount++;
        ReferenceText_STATIC[3] = LabelCount;
        ActiveLabels[LabelCount] = TXT_Contrato;
        LabelCount++;
        ReferenceText_STATIC[4] = LabelCount;
        ActiveLabels[LabelCount] = TXT_PBaixa;
        LabelCount++;
    }
    else
    {
        for(int i=0;i<TextCount_StaticEle;i++)
        {
            if(i==0)
            {
                ActiveLabels[ReferenceText_STATIC[i]].X = 35 + CalcPercent(Tabs[i].Box.w, 33);
                continue;
            }
            if(i==3)
            {
                ActiveLabels[ReferenceText_STATIC[i]].X = 35 + CalcPercent(Tabs[i].Box.w, 33);
                continue;
            }
            if(i==4)
            {
                ActiveLabels[ReferenceText_STATIC[i]].X = 35 + CalcPercent(Tabs[i].Box.w, 33);
                continue;
            }
            ActiveLabels[ReferenceText_STATIC[i]].X = 35 + CalcPercent(Tabs[i].Box.w, 33);
            ActiveLabels[ReferenceText_STATIC[i]].Y = Tabs[i].Box.y + CalcPercent(Tabs[0].Box.h, 20);
        }
    }
    return SDL_APP_CONTINUE;
}
#define TextCount_IniPage 4
int ReferenceText_INIPAGE[TextCount_IniPage] = {0};
RadioButton Radio_BTN[3];
int Radio_BTN_Count = 0;
int Radio_BTN_HitboxIndex[3] = {0};
SDL_AppResult InitialPage()
{
    SDL_SetRenderDrawColor(Render, 63,61,71,255);
    float X = (50+CalcPercent((float)WnWidth,20));
    float Y = 20;
    float W = WnWidth - (CalcPercent((float)WnWidth, 20)) - 85;
    float H = CalcPercent(WnHeight, 95);
    SDL_RenderLine(Render,X+10,80,(X+W)-10,80);
    float XAnchorOptions = X + CalcPercent(W, 15);

    for(int i=1;i<TextCount_IniPage;i++)
    {
        SDL_RenderFillRect(Render, &(SDL_FRect){.x=X,.y=13+CalcPercent(H,20*i),.w=W,.h=35});
    }
    RadioButton RadioBtn_Homologação = {
        .BackColor = {.A=255,.R=225,.B=225,.G=225},
        .FrontColor = {.A=255,.R=0,.B=0,.G=100},
        .Size = 12,
        .State = _R_Homologação,
        .X = X + CalcPercent(W, 6),
        .Y = 31+CalcPercent(H,20),
        .Box = {.Function = RADIO_MENUENABLE,.Parameter.NUMBER=0,.BoundingBox = (SDL_FRect){
            .x = Radio_BTN[0].X - (Radio_BTN[0].Size),
            .y = Radio_BTN[0].Y - (Radio_BTN[0].Size),
            .w = Radio_BTN[0].Size*2,
            .h = Radio_BTN[0].Size*2
        }}
    };
    Radio_BTN[0] = RadioBtn_Homologação;
    if(!HubLoaded)//Set the elements
    {
        HitBoxes[HitboxCount] = Radio_BTN[0].Box;
        Radio_BTN_HitboxIndex[0] = HitboxCount;
        HitboxCount++;
        Radio_BTN_Count++;

        Text TXT_SelecioneOsArquivos ={
          .FontSize = 24,
          .X = X + CalcPercent( W, 30),
          .Y = 50,
          .RGB = {.A=255,.R=255,.G=255,.B=255},
          .String = "Selecione os Arquivos que Deseja Criar"
        };

        Text TXT_DocumentosdeHomologação ={
          .FontSize = 18,
          .RGB = {.A=255,.R=255,.G=255,.B=255},
          .String = "Documentos de Homologação",
          .X = XAnchorOptions,
          .Y = 20 + CalcPercent(H, 20)
        };

        Text TXT_Contrato ={
          .FontSize = 18,
          .RGB = {.A=255,.R=255,.G=255,.B=255},
          .String = "Contrato",
          .X = XAnchorOptions,
          .Y =  20 + CalcPercent(H, 40)
        };

        Text TXT_ParametrosPlantaBaixa ={
          .FontSize = 18,
          .RGB = {.A=255,.R=255,.G=255,.B=255},
          .String = "Parametros para Planta Baixa",
          .X = XAnchorOptions,
          .Y = (20 + CalcPercent(H, 60))
        };
        ActiveLabels[LabelCount] = TXT_SelecioneOsArquivos;
        ReferenceText_INIPAGE[0] = LabelCount;
        LabelCount++;
        ActiveLabels[LabelCount] = TXT_DocumentosdeHomologação;
        ReferenceText_INIPAGE[1] = LabelCount;
        LabelCount++;
        ActiveLabels[LabelCount] = TXT_Contrato;
        ReferenceText_INIPAGE[2] = LabelCount;
        LabelCount++;
        ActiveLabels[LabelCount] = TXT_ParametrosPlantaBaixa;
        ReferenceText_INIPAGE[3] = LabelCount;
        LabelCount++;
    }
    else//Upkeep the elements
    {
        HitBoxes[Radio_BTN_HitboxIndex[0]] = Radio_BTN[0].Box;
        SDL_RenderRadioBtn(Radio_BTN[0]);

        for(int i=0;i<Radio_BTN_Count;i++)
        {
            Radio_BTN[i].X = X + CalcPercent(W, 6);
            Radio_BTN[i].Y = 20 + CalcPercent(H, 21.9);
        }
        for(int i=0;i<TextCount_IniPage;i++)
        {
            if(ActiveLabels[ReferenceText_INIPAGE[i]].FontSize == 24)
            {
                ActiveLabels[ReferenceText_INIPAGE[i]].X = X + CalcPercent( W, 30);
                ActiveLabels[ReferenceText_INIPAGE[i]].Y = 50;
                continue;
            }
            ActiveLabels[ReferenceText_INIPAGE[i]].X = XAnchorOptions;
            ActiveLabels[ReferenceText_INIPAGE[i]].Y = (20 + CalcPercent(H, 20*i));
        }
    }
    return SDL_APP_CONTINUE;
}
SDL_AppResult ClientPage()
{
    Text TXT_TestText ={
        .FontSize = 17,
        .X = 800,
        .Y = 200,
        .String = "CLIENT PAGE",
        .RGB = {.A=255,.R=255,.G=255,.B=255}
    };
    if(!HubLoaded)
    {
        ActiveLabels[LabelCount] = TXT_TestText;
        LabelCount++;
    }
    else
    {

    }
    return SDL_APP_CONTINUE;
}
//---------------------------------------------------------------------------------//
SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[])
{
    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    if (!SDL_CreateWindowAndRenderer("Luciano Test 2", WnWidth, WnHeight, SDL_WINDOW_RESIZABLE, &window, &Render))
    {
        SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }
    SDL_SetRenderLogicalPresentation(Render, WnWidth, WnHeight, SDL_LOGICAL_PRESENTATION_DISABLED);
    SDL_SetWindowMinimumSize(window, WnWidth, WnHeight);
    SDL_AppResult FontResult = SetFont();
    if(FontResult != SDL_APP_CONTINUE)
    {
        return FontResult;
    }
    SDL_AppResult Rs = InitStructs();
    if(Rs != SDL_APP_CONTINUE)
    {
        return Rs;
    }
    return SDL_APP_CONTINUE;
}
SDL_AppResult SDL_AppIterate(void *appstate)
{
    if(_NewLoad)
    {
        StaticLoaded = false;
        HubLoaded = false;
    }
    else
    {
        StaticLoaded = true;
        HubLoaded = true;
    }
    SDL_SetRenderDrawColor(Render, 43, 41, 51,SDL_ALPHA_OPAQUE);
    SDL_RenderClear(Render);
    SDL_AppResult Result = SDL_APP_CONTINUE;
    if(_NewLoad)
    {
        CleanPage();
    }
    StaticElements();
    switch(CurrentScreen)
    {
        case InitScreen:
            Result = InitialPage();
            break;
        case Cliente:
            Result = ClientPage();
            break;
        case Solar:
            break;
        case Local:
            break;
        case Contrato:
            break;
        case PBaixa:
            break;
        case FinalScreen:
            break;
    }
    RenderFont();

    SDL_RenderPresent(Render);
    _NewLoad = false;
    return Result;
}
SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
{
    if(event->type == SDL_EVENT_QUIT)
    {
        return SDL_APP_SUCCESS;
    }
    if(event->type == SDL_EVENT_WINDOW_RESIZED)
    {
        SDL_GetWindowSizeInPixels(window, &WnWidth, &WnHeight);
    }

    if(event->type == SDL_EVENT_MOUSE_MOTION)
    {
        MouseState.X = event->motion.x;
        MouseState.Y = event->motion.y;
        if(HoverHandler.HoverTrigger)
        {
            if(MouseState.X<=HoverHandler.Box.BoundingBox.x || MouseState.X>=HoverHandler.Box.BoundingBox.x+HoverHandler.Box.BoundingBox.w || MouseState.Y<=HoverHandler.Box.BoundingBox.y || MouseState.Y>=HoverHandler.Box.BoundingBox.y+HoverHandler.Box.BoundingBox.h)
            {
                HoverHandler.HoverTrigger = false;
                if(HoverHandler.Box.Function == TABSELECT)
                {
                    TabHighLight(HoverHandler.Box.Parameter, HoverHandler.Box.Function);
                }
                HoverHandler.Box.BoundingBox = (SDL_FRect){.x=0,.y=0,.w=0,.h=0};
                HoverHandler.Box.Function = 0;
                HoverHandler.Box.Parameter.NUMBER = 0;
            }
        }
        if(!HoverHandler.HoverTrigger)
        {
            for(int i=0;i<HitboxCount;i++)
            {
                if(MouseState.X>HitBoxes[i].BoundingBox.x && MouseState.X<HitBoxes[i].BoundingBox.x+HitBoxes[i].BoundingBox.w)
                {
                    if(MouseState.Y>HitBoxes[i].BoundingBox.y && MouseState.Y<HitBoxes[i].BoundingBox.y+HitBoxes[i].BoundingBox.h)
                    {
                        HoverHandler.HoverTrigger = true;
                        HoverHandler.Box.BoundingBox = HitBoxes[i].BoundingBox;
                        HoverHandler.Box.Function = HitBoxes[i].Function;
                        HoverHandler.Box.Parameter = HitBoxes[i].Parameter;
                        if(HitBoxes[i].Function == TABSELECT)
                        {
                            TabHighLight(HitBoxes[i].Parameter, HitBoxes[i].Function);
                        }
                    }
                }
            }
        }
    }
    if(event->type == SDL_EVENT_MOUSE_BUTTON_DOWN)
    {
        if(HoverHandler.HoverTrigger)
        {
            SDL_AppResult Result = SDL_APP_CONTINUE;
            switch(HoverHandler.Box.Function)
            {
                case TABSELECT:
                    Result = TabSwitch(HoverHandler.Box.Parameter);
                break;
                case RADIO_MENUENABLE:
                    Result = RADIO_TabEnable(HoverHandler.Box.Parameter);
                break;
            }
            if(Result != SDL_APP_CONTINUE)
            {
                return Result;
            }
        }
    }
    return SDL_APP_CONTINUE;
}
void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
    CleanPage();
}

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
typedef struct{
    SDL_FRect *BoundingBox;
    int Function;
    EventParams Parameter;
}HitBox;//Anything that can be clicked should have a FRect "Hitbox"
HitBox HitBoxes[100] = {0};
int HitboxCount = 0;
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
void CleanStaticFont()
{
    if(StaticLabelCount == 0)
    {
        goto EndClean;
    }  
    for(int i=0;i<StaticLabelCount;i++)
    {
        SDL_DestroyTexture(StaticLabel[i].Texture);
    }    
    EndClean:
}
SDL_AppResult RenderStaticFont()
{
    if(StaticLabelCount == 0)
    {
        return SDL_APP_CONTINUE;
    }
    if(StaticLoaded)
    {
        for(int i=0; i<StaticLabelCount;i++)
        {
            SDL_SetRenderScale(Render, StaticLabel[i].FontSize/(float)24, StaticLabel[i].FontSize/(float)24);
            SDL_RenderTexture(Render, ActiveLabels[i].Texture, NULL, &(SDL_FRect){
                .x=StaticLabel[i].X*((float)24/StaticLabel[i].FontSize),
                .y=StaticLabel[i].Y*((float)24/StaticLabel[i].FontSize),
                .w=StaticLabel[i].Width,
                .h=StaticLabel[i].Height
            });
        }
        SDL_SetRenderScale(Render, 1, 1);
        return SDL_APP_CONTINUE;
    }
    for(int i=0; i<StaticLabelCount;i++)
    {
        SDL_Color TxtColor = {.a=StaticLabel[i].RGB.A,.r=StaticLabel[i].RGB.R,.g=StaticLabel[i].RGB.G,.b=StaticLabel[i].RGB.B};
        SDL_Surface *TxtSuface = TTF_RenderText_Blended(MainFont,StaticLabel[i].String,0,TxtColor);
        StaticLabel[i].Height = TxtSuface->h;
        StaticLabel[i].Width = TxtSuface->w;
        StaticLabel[i].Texture = SDL_CreateTextureFromSurface(Render,TxtSuface);
        SDL_DestroySurface(TxtSuface);
        SDL_SetRenderScale(Render, StaticLabel[i].FontSize/(float)24, StaticLabel[i].FontSize/(float)24);
        SDL_RenderTexture(Render, StaticLabel[i].Texture, NULL, &(SDL_FRect){
            .x=StaticLabel[i].X*((float)24/StaticLabel[i].FontSize),
            .y=StaticLabel[i].Y*((float)24/StaticLabel[i].FontSize),
            .w=StaticLabel[i].Width,
            .h=StaticLabel[i].Height
        });
    }
    StaticLoaded = true;
    SDL_SetRenderScale(Render, 1, 1);
    return SDL_APP_CONTINUE;
}
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
void CleanPage()
{    
    CleanStaticFont();
    CleanFonts();
    for(int i=0;i<=HitboxCount; i++)
    {
        HitBoxes[HitboxCount].Function = 0;
        HitBoxes[HitboxCount].Parameter.NUMBER = 0;
        HitBoxes[HitboxCount].BoundingBox = &(SDL_FRect){.x=0,.y=0,.w=0,.h=0};
    }
    HubLoaded = false;
    StaticLoaded = false;
}
static float CalcPercent(float NUM, float PERCENT)
{
    float Result = (NUM * PERCENT)/100;
    return Result;
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
#define TextCount_StaticEle 6
int ReferenceText_StaticEle[TextCount_StaticEle] = {0};
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
            HitBoxes[HitboxCount].BoundingBox = &Tabs[i].Box;
            HitBoxes[HitboxCount].Function = TABSELECT;
            HitBoxes[HitboxCount].Parameter.NUMBER = i;
            HitboxCount++;
        }
    }
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
        .Y = Tabs[4].Box.y + CalcPercent(Tabs[4].Box.y, 2.7),
    };
    if(!StaticLoaded)
    {
        ReferenceText_StaticEle[0] = StaticLabelCount;
        StaticLabel[StaticLabelCount] = TXT_Cliente;
        StaticLabelCount++;
        ReferenceText_StaticEle[1] = StaticLabelCount;
        StaticLabel[StaticLabelCount] = TXT_Solar;
        StaticLabelCount++;
        ReferenceText_StaticEle[2] = StaticLabelCount;
        StaticLabel[StaticLabelCount] = TXT_Local;
        StaticLabelCount++;
        ReferenceText_StaticEle[3] = StaticLabelCount;
        StaticLabel[StaticLabelCount] = TXT_Contrato;
        StaticLabelCount++;
        ReferenceText_StaticEle[4] = StaticLabelCount;
        StaticLabel[StaticLabelCount] = TXT_PBaixa;
        StaticLabelCount++;
    }
    else
    {
        for(int i=0;i<StaticLabelCount;i++)
        {
            StaticLabel[i].X = 35 + CalcPercent(Tabs[i].Box.w, 35);
            StaticLabel[i].Y = Tabs[i].Box.y + CalcPercent(Tabs[0].Box.h, 20);
        }
    }
    return SDL_APP_CONTINUE;
}
#define TextCount_IniPage 4
int ReferenceText_INIPAGE[TextCount_IniPage] = {0};
SDL_AppResult InitialPage()
{
    SDL_SetRenderDrawColor(Render, 63,61,71,255);
    float X = (50+CalcPercent((float)WnWidth,20));
    float Y = 20;
    float W = WnWidth - (CalcPercent((float)WnWidth, 20)) - 85;
    float H = CalcPercent(WnHeight, 95);
    SDL_RenderLine(Render,X+10,80,(X+W)-10,80);
    float XAnchorOptions = X + CalcPercent(W, 15);

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
      .String = "Documentos de Homologação"
    };
    TXT_DocumentosdeHomologação.X = XAnchorOptions;
    TXT_DocumentosdeHomologação.Y = 20 + CalcPercent(H, 20);

    Text TXT_Contrato ={
      .FontSize = 18,
      .RGB = {.A=255,.R=255,.G=255,.B=255},
      .String = "Contrato",
    };
    TXT_Contrato.X = XAnchorOptions;
    TXT_Contrato.Y =  20 + CalcPercent(H, 40);;

    Text TXT_ParametrosPlantaBaixa ={
      .FontSize = 18,
      .RGB = {.A=255,.R=255,.G=255,.B=255},
      .String = "Parametros para Planta Baixa",
    };
    TXT_ParametrosPlantaBaixa.X = XAnchorOptions;
    TXT_ParametrosPlantaBaixa.Y = (20 + CalcPercent(H, 60));
    for(int i=1;i<TextCount_IniPage;i++)
    {
        if(i==2)
        {
            SDL_RenderFillRect(Render, &(SDL_FRect){.x=X + CalcPercent(W, 15)-35,.y=20+CalcPercent(H,20*i)-7,.w=117,.h=35});
            continue;
        }
        SDL_RenderFillRect(Render, &(SDL_FRect){.x=X + CalcPercent(W, 15)-35,.y=20+CalcPercent(H,20*i)-7,.w=290,.h=35});
    }

    if(!HubLoaded)
    {
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
    else
    {
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
    SDL_SetRenderDrawColor(Render, 43, 41, 51,SDL_ALPHA_OPAQUE);
    SDL_RenderClear(Render);
    StaticElements();
    SDL_AppResult Result = SDL_APP_CONTINUE;
    switch(CurrentScreen)
    {
        case InitScreen:
            CleanPage();
            Result = InitialPage();
            break;
        case Cliente:
            CleanPage();
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
    RenderStaticFont();
    RenderFont();
    SDL_RenderPresent(Render);
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
            if(MouseState.X<=HoverHandler.Box.BoundingBox->x || MouseState.X>=HoverHandler.Box.BoundingBox->x+HoverHandler.Box.BoundingBox->w || MouseState.Y<=HoverHandler.Box.BoundingBox->y || MouseState.Y>=HoverHandler.Box.BoundingBox->y+HoverHandler.Box.BoundingBox->h)
            {
                HoverHandler.HoverTrigger = false;
                if(HoverHandler.Box.Function == TABSELECT)
                {
                    TabHighLight(HoverHandler.Box.Parameter, HoverHandler.Box.Function);
                }
                HoverHandler.Box.BoundingBox = &(SDL_FRect){.x=0,.y=0,.w=0,.h=0};
                HoverHandler.Box.Function = 0;
                HoverHandler.Box.Parameter.NUMBER = 0;
            }
        }
        if(!HoverHandler.HoverTrigger)
        {
            for(int i=0;i<HitboxCount;i++)
            {
                if(MouseState.X>HitBoxes[i].BoundingBox->x && MouseState.X<HitBoxes[i].BoundingBox->x+HitBoxes[i].BoundingBox->w)
                {
                    if(MouseState.Y>HitBoxes[i].BoundingBox->y && MouseState.Y<HitBoxes[i].BoundingBox->y+HitBoxes[i].BoundingBox->h)
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
            switch(HoverHandler.Box.Function)
            {
                case TABSELECT:
                TabSwitch(HoverHandler.Box.Parameter);
                break;
                case RADIO_MENUENABLE:
                break;
            }
        }
    }
    return SDL_APP_CONTINUE;
}
void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
    
}

#include "Lib/HEHelper&Maker.h"
int WnHeight = 600;
int WnWidth = 1200;
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
typedef struct{
    SDL_FRect BoundingBox;
    int Function;
    EventParams Parameter;
}HitBox;//Anything that can be clicked should have a FRect "Hitbox"
enum Btn_Functions{
  SWITCHTABS,
  ENABLETABS
};
typedef struct{
    Color Normal;
    Color HighLight;
    Color Click;
    Color Disabled;
    SDL_FRect Box;
    int Function;
    EventParams Param;
}Tab;
Tab Tabs[5];
SDL_AppResult InitStructs()
{
    for(int i=0;i<=4;i++)
    {
        Tabs[i].Normal = (Color){.A=255,.R=63,.G=61,.B=71};
        Tabs[i].HighLight = (Color){.A=255,.R=63,.G=82,.B=52};
        Tabs[i].Disabled = (Color){.A=255,.R=43,.G=41,.B=51};
        Tabs[i].Click = (Color){.A=255,.R=73,.G=71,.B=81};
    }
    return SDL_APP_CONTINUE;
}
//Faculdade Semana 1 (Lei de Gauss) - Semana 3 (Magneticos) - Semana 5 (Força Magnética) - Semana 7
bool FirstPass = false;
static float CalcPercent(float NUM, float PERCENT)
{
    float Result = (NUM * PERCENT)/100;
    return Result;
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
        SDL_SetRenderDrawColor(Render, Tabs[i].Normal.R, Tabs[i].Normal.G, Tabs[i].Normal.B, Tabs[i].Normal.A);
        SDL_RenderFillRect(Render,&Tabs[i].Box);
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
    if(!HubLoaded)
    {//For some reason Tabs[0] getting its coordinates zeroed out and then reapplied
        ReferenceText_StaticEle[0] = LabelCount;
        ActiveLabels[LabelCount++] = TXT_Cliente;
        ReferenceText_StaticEle[1] = LabelCount;
        ActiveLabels[LabelCount++] = TXT_Solar;
        ReferenceText_StaticEle[2] = LabelCount;
        ActiveLabels[LabelCount++] = TXT_Local;
        ReferenceText_StaticEle[3] = LabelCount;
        ActiveLabels[LabelCount++] = TXT_Contrato;
        ReferenceText_StaticEle[4] = LabelCount;
        ActiveLabels[LabelCount++] = TXT_PBaixa;
    }
    else
    {
        for(int i=0;i<TextCount_StaticEle;i++)
        {

            if(ReferenceText_StaticEle[i] == 0)//Literal hot glue fix
            {
                ActiveLabels[ReferenceText_StaticEle[i]].X = 35 + CalcPercent(Tabs[1].Box.w, 35);
                ActiveLabels[ReferenceText_StaticEle[i]].Y = (Tabs[1].Box.y-40) + CalcPercent(Tabs[1].Box.h, 20);
                printf("Text %s | X: %f | Y: %f\n",ActiveLabels[ReferenceText_StaticEle[i]].String,Tabs[1].Box.x,Tabs[1].Box.y);
                continue;
            }
            ActiveLabels[ReferenceText_StaticEle[i]].X = 35 + CalcPercent(Tabs[i].Box.w, 35);
            if(i==4)
            {
                ActiveLabels[ReferenceText_StaticEle[i]].Y =Tabs[i].Box.y + CalcPercent(Tabs[i].Box.y, 2.7);
                continue;
            }

            ActiveLabels[ReferenceText_StaticEle[i]].Y = Tabs[i].Box.y + CalcPercent(Tabs[0].Box.h, 20);
        }
    }

    return SDL_APP_CONTINUE;
}
int ReferenceText_INIPAGE[4] = {0};
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
      .Y = 50,
      .RGB = {.A=255,.R=255,.G=255,.B=255},
      .String = "Selecione os Arquivos que Deseja Criar"
    };
    TXT_SelecioneOsArquivos.X = X + CalcPercent( W, 30);

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
    for(int i=1;i<4;i++)
    {
        if(i==2)
        {
            SDL_RenderFillRect(Render, &(SDL_FRect){.x=X + CalcPercent(W, 15)-35,.y=20+CalcPercent(H,20*i)-7,.w=117,.h=35});
            //SDL_RenderLine(Render, (X + CalcPercent(W, 15)-9),(20 + CalcPercent(H, 20*i))+20,X+210,(20 + CalcPercent(H, 20*i))+20);
            continue;
        }
        SDL_RenderFillRect(Render, &(SDL_FRect){.x=X + CalcPercent(W, 15)-35,.y=20+CalcPercent(H,20*i)-7,.w=290,.h=35});
        //SDL_RenderLine(Render,( X + CalcPercent(W, 15)-9),(20 + CalcPercent(H, 20*i))+20,X+380,(20 + CalcPercent(H, 20*i))+20);
    }

    if(!HubLoaded)
    {
        ActiveLabels[LabelCount++] = TXT_SelecioneOsArquivos;
        ReferenceText_INIPAGE[0] = LabelCount;
        ActiveLabels[LabelCount++] = TXT_DocumentosdeHomologação;
        ReferenceText_INIPAGE[1] = LabelCount;
        ActiveLabels[LabelCount++] = TXT_Contrato;
        ReferenceText_INIPAGE[2] = LabelCount;
        ActiveLabels[LabelCount++] = TXT_ParametrosPlantaBaixa;
        ReferenceText_INIPAGE[3] = LabelCount;
    }
    else
    {
        for(int i=0; i<4;i++)
        {
            ActiveLabels[ReferenceText_INIPAGE[i]].X = XAnchorOptions;
            ActiveLabels[ReferenceText_INIPAGE[i]].Y = (20 + CalcPercent(H, 20*(i+1)));
        }
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
    SDL_SetRenderDrawColor(Render, 255, 0, 0,SDL_ALPHA_OPAQUE);
    SDL_RenderPoint(Render, 119, 6);
    SDL_RenderPoint(Render, 35, 66);
    SDL_AppResult Result = SDL_APP_CONTINUE;
    switch(CurrentScreen)
    {
        case InitScreen:
            Result = InitialPage();
            break;
        case Cliente:
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
    return SDL_APP_CONTINUE;
}
void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
    CleanFonts();
}
//              Todo
// - Find a formula to center sentences

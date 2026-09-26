#include "BaseSDLDependency.h"
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_rect.h>
//SDL3 & C23

typedef struct
{
    int A;
    int R;
    int G;
    int B;
}Color;
SDL_AppResult Render_Polygon(float IN_PosX, float IN_PosY,int IN_Radius, int IN_Vertice_Count, Color IN_ShapeColor)
{

    if(IN_Vertice_Count<=2)
    {
        printf("Não Poligono com %d vertices\n",IN_Vertice_Count);
        return SDL_APP_FAILURE;
    }

    IN_ShapeColor.A = IN_ShapeColor.A/255;
    IN_ShapeColor.R = IN_ShapeColor.R/255;
    IN_ShapeColor.G = IN_ShapeColor.G/255;
    IN_ShapeColor.B = IN_ShapeColor.B/255;

    float Angle_Vertices = (float)(180+(180*(IN_Vertice_Count - 3)))/IN_Vertice_Count;
    float AngleStep = 0.0174444444445;
    float Spinning_Angle = AngleStep * (180 - Angle_Vertices);

    SDL_Vertex Vertices[IN_Vertice_Count+1];
    SDL_zeroa(Vertices);
    int Ind[IN_Vertice_Count*3] = {};//Need to find a way to be a const
    int Offset = 0;
    for(int i=2;i<=IN_Vertice_Count;i++)
    {
        if(i==2)
        {
            Vertices[0].position.x = IN_PosX;
            Vertices[0].position.y = IN_PosY;
            Vertices[1].position.x = IN_PosX;
            Vertices[1].position.y = IN_PosY - IN_Radius;
            Vertices[2].position.x = ((Vertices[1].position.x - IN_PosX)*cos(Spinning_Angle * (i-1))-(Vertices[1].position.y - IN_PosY)*sin(Spinning_Angle * (i-1))) + IN_PosX;
            Vertices[2].position.y = ((Vertices[1].position.x - IN_PosX)*sin(Spinning_Angle * (i-1))+(Vertices[1].position.y - IN_PosY)*cos(Spinning_Angle * (i-1))) + IN_PosY;
            Ind[0] = 1;
            Ind[1] = 0;
            Ind[2] = 2;
            Vertices[0].color.a = IN_ShapeColor.A;
            Vertices[0].color.r = IN_ShapeColor.R;
            Vertices[0].color.g = IN_ShapeColor.G;
            Vertices[0].color.b = IN_ShapeColor.B;
            Vertices[1].color.a = IN_ShapeColor.A;
            Vertices[1].color.r = IN_ShapeColor.R;
            Vertices[1].color.g = IN_ShapeColor.G;
            Vertices[1].color.b = IN_ShapeColor.B;
            Vertices[2].color.a = IN_ShapeColor.A;
            Vertices[2].color.r = IN_ShapeColor.R;
            Vertices[2].color.g = IN_ShapeColor.G;
            Vertices[2].color.b = IN_ShapeColor.B;
            continue;
        }
        Vertices[i].position.x = ((Vertices[1].position.x - IN_PosX)*cos(Spinning_Angle * (i-1))-(Vertices[1].position.y - IN_PosY)*sin(Spinning_Angle * (i-1))) + IN_PosX;
        Vertices[i].position.y = ((Vertices[1].position.x - IN_PosX)*sin(Spinning_Angle * (i-1))+(Vertices[1].position.y - IN_PosY)*cos(Spinning_Angle * (i-1))) + IN_PosY;
        Vertices[i].color.a = IN_ShapeColor.A;
        Vertices[i].color.r = IN_ShapeColor.R;
        Vertices[i].color.g = IN_ShapeColor.G;
        Vertices[i].color.b = IN_ShapeColor.B;
        Ind[i + Offset] = i-1;
        Offset++;
        Ind[i + Offset] = 0;
        Offset++;
        Ind[i + Offset] = i;
        if(i==IN_Vertice_Count)
        {
            Offset++;
            Ind[i+Offset] = i;
            Offset++;
            Ind[i+Offset] = 0;
            Offset++;
            Ind[i+Offset] = 1;
        }
    }
    SDL_RenderGeometry(Render, NULL, Vertices, IN_Vertice_Count+1,Ind,IN_Vertice_Count*3);

    return SDL_APP_CONTINUE;
}
typedef struct
{
    bool State;
    SDL_FRect *ColisionBox;
    float XY[2];
    int Size;
    Color BackColor;
    Color FrontColor;
}RadioButton;
TTF_Font *MainFont;
SDL_AppResult SetFont()
{
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
    return  SDL_APP_CONTINUE;
}
SDL_AppResult SDL_RenderRadioBtn(RadioButton IN_RD)
{
    Render_Polygon(IN_RD.XY[0], IN_RD.XY[1], IN_RD.Size, 200, IN_RD.BackColor);
    if(!IN_RD.State)
    {
        IN_RD.FrontColor = IN_RD.BackColor;
    }
    else
    {
        IN_RD.FrontColor = (Color){.A=255,.R=0,.G=0,.B=0};
    }
    Render_Polygon(IN_RD.XY[0], IN_RD.XY[1], IN_RD.Size*0.5, 200, IN_RD.FrontColor);
    return SDL_APP_CONTINUE;
}
typedef struct
{
    SDL_Texture *Texture;
    float X;
    float Y;
    float Width;
    float Height;
    const char *String;
    Color RGB;
    int FontSize;
}Text;
Text ActiveLabels[100];
int LabelCount=0;
bool HubLoaded = false;
SDL_AppResult RenderFont()
{
    if(HubLoaded)
    {
        for(int i=0; i<LabelCount;i++)
        {
            SDL_SetRenderScale(Render, ActiveLabels[i].FontSize/(float)24, ActiveLabels[i].FontSize/(float)24);
            SDL_RenderTexture(Render, ActiveLabels[i].Texture, NULL, &(SDL_FRect){
                .x=ActiveLabels[i].X*((float)24/ActiveLabels[i].FontSize),
                .y=ActiveLabels[i].Y*((float)24/ActiveLabels[i].FontSize),
                .w=ActiveLabels[i].Width,
                .h=ActiveLabels[i].Height
            });
        }
        SDL_SetRenderScale(Render, 1, 1);
        return SDL_APP_CONTINUE;
    }
    for(int i=0; i<LabelCount;i++)
    {
        SDL_Color TxtColor = {.a=ActiveLabels[i].RGB.A,.r=ActiveLabels[i].RGB.R,.g=ActiveLabels[i].RGB.G,.b=ActiveLabels[i].RGB.B};
        SDL_Surface *TxtSuface = TTF_RenderText_Blended(MainFont,ActiveLabels[i].String,0,TxtColor);
        ActiveLabels[i].Height = TxtSuface->h;
        ActiveLabels[i].Width = TxtSuface->w;
        ActiveLabels[i].Texture = SDL_CreateTextureFromSurface(Render,TxtSuface);
        SDL_DestroySurface(TxtSuface);
        SDL_SetRenderScale(Render, ActiveLabels[i].FontSize/(float)24, ActiveLabels[i].FontSize/(float)24);
        SDL_RenderTexture(Render, ActiveLabels[i].Texture, NULL, &(SDL_FRect){
            .x=ActiveLabels[i].X*((float)24/ActiveLabels[i].FontSize),
            .y=ActiveLabels[i].Y*((float)24/ActiveLabels[i].FontSize),
            .w=ActiveLabels[i].Width,
            .h=ActiveLabels[i].Height
        });
    }
    HubLoaded = true;
    SDL_SetRenderScale(Render, 1, 1);
    return SDL_APP_CONTINUE;
}
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
SDL_AppResult RenderTexture()
{
    return SDL_APP_CONTINUE;
}
typedef union
{
    int NUMBER;
    char WORDS[256];
    bool CHOICE;
}EventParams;

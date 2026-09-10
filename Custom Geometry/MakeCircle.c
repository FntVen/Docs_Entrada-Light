#include <stdbool.h>
#include <stdint.h>
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
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>
#include <math.h>

SDL_Window *window = NULL;
SDL_Renderer *Render = NULL;
#define WnHeight 600
#define WnWidth 1200

SDL_AppResult Render_Polygon(float IN_PosX, float IN_PosY,int IN_Radius, int IN_Vertice_Count)
{

    if(IN_Vertice_Count<=2)
    {
        printf("Não Poligono com %d vertices\n",IN_Vertice_Count);
        return SDL_APP_FAILURE;
    }
    SDL_SetRenderDrawColor(Render, 255, 255, 255, 255);
    float Angle_Vertices = (float)(180+(180*(IN_Vertice_Count - 3)))/IN_Vertice_Count;
    float AngleStep = 0.0174444444445;
    float Spinning_Angle = AngleStep * (180 - Angle_Vertices);

    SDL_Vertex Vertices[IN_Vertice_Count+1];
    SDL_zeroa(Vertices);
    int Ind[IN_Vertice_Count*3] = {};//Need to find a way to be a const
    int Offset = 0;
    int Height;//Debug
    int Width;//Debug
    SDL_GetWindowSizeInPixels(window, &Width, &Height);//Debug
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
            Vertices[0].color.a = 1;
            Vertices[0].color.r = 1;
            Vertices[0].color.g = 1;
            Vertices[0].color.b = 1;
            Vertices[1].color.a = 1;
            Vertices[1].color.r = 1;
            Vertices[1].color.g = 1;
            Vertices[1].color.b = 1;
            Vertices[2].color.a = 1;
            Vertices[2].color.r = 1;
            Vertices[2].color.g = 1;
            Vertices[2].color.b = 1;
            continue;
        }
        Vertices[i].position.x = ((Vertices[1].position.x - IN_PosX)*cos(Spinning_Angle * (i-1))-(Vertices[1].position.y - IN_PosY)*sin(Spinning_Angle * (i-1))) + IN_PosX;
        Vertices[i].position.y = ((Vertices[1].position.x - IN_PosX)*sin(Spinning_Angle * (i-1))+(Vertices[1].position.y - IN_PosY)*cos(Spinning_Angle * (i-1))) + IN_PosY;
        Vertices[i].color.a = 1;
        Vertices[i].color.r = 1;
        Vertices[i].color.g = 1;
        Vertices[i].color.b = 1;
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
    for(int i=0;i<=IN_Vertice_Count;i++)
    {
        if(i==5)
        {
            SDL_SetRenderDrawColor(Render, 255, 0, 0, 255);
            SDL_RenderPoint(Render, Vertices[i].position.x, Vertices[i].position.y);
            SDL_SetRenderDrawColor(Render, 255, 255, 255, 255);
        }
        else
        {
            SDL_RenderPoint(Render, Vertices[i].position.x, Vertices[i].position.y);
        }
        SDL_RenderDebugTextFormat(Render, 10, (Height - 10) - (10 * i), "Point %d: X > %f and Y >%f\n",i, Vertices[i].position.x,Vertices[i].position.y);
    }
    //const int Indices[] = {1,0,2,2,0,3,3,0,4,4,0,5,5,0,1};
    SDL_RenderGeometry(Render, NULL, Vertices, IN_Vertice_Count+1,Ind,IN_Vertice_Count*3);

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

    return SDL_APP_CONTINUE;
}
typedef struct
{
    float X1;
    float Y1;
    float X2;
    float Y2;
}Line;
SDL_AppResult SDL_AppIterate(void *appstate)
{

    int WSize[2];
    SDL_GetWindowSizeInPixels(window, &WSize[0], &WSize[1]);
    SDL_SetRenderDrawColor(Render, 0, 0, 0, 255);
    SDL_RenderClear(Render);
    SDL_SetRenderDrawColor(Render, 255, 255, 255, 255);

    Render_Polygon((float)WSize[0]/2, (float)WSize[1]/2, 60, 5);

    SDL_RenderPresent(Render);
    return SDL_APP_CONTINUE;
}
SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
{
    if (event->type == SDL_EVENT_QUIT)
    {
        return SDL_APP_SUCCESS;
    }

    return SDL_APP_CONTINUE;  /* carry on with the program! */
}

void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
    /* SDL will clean up the window/renderer for us. */
}

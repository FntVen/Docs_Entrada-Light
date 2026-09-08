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

void Render_PolyGon(float IN_X, float IN_Y,int IN_Radius, int IN_EdgeNum, SDL_Vertex *OUT_Vert[])
{

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
    /*
    //Simple Triangle
    SDL_Vertex Vertices[3];
    SDL_zeroa(Vertices);//memclear
    Vertices[0].position.x = (int)WSize[0]/2;
    Vertices[0].position.y = (int)WSize[1]/2;
    Vertices[0].color.r = 1;
    Vertices[0].color.g = 1;
    Vertices[0].color.b = 1;
    Vertices[0].color.a = 1;

    Vertices[1].position.x = (int)WSize[0]/1.5;
    Vertices[1].position.y = (int)WSize[1]/1.5;
    Vertices[1].color.r = 1;
    Vertices[1].color.g = 1;
    Vertices[1].color.b = 1;
    Vertices[1].color.a = 1;

    Vertices[2].position.x = (int)WSize[0]/3;
    Vertices[2].position.y = (int)WSize[1]/1.5;
    Vertices[2].color.r = 1;
    Vertices[2].color.g = 1;
    Vertices[2].color.b = 1;
    Vertices[2].color.a = 1;
    SDL_RenderGeometry(Render, NULL, Vertices, 3, NULL, 0);
    */
    int Vertice_Count = 9;
    float Angle_Vertices = (float)(180+(180*(Vertice_Count - 3)))/Vertice_Count;
    float AngleStep = 0.0174444444445;
    float Spinning_Angle = AngleStep * (180 - Angle_Vertices);
    SDL_Vertex Vertex[Vertice_Count];
    SDL_zeroa(Vertex);//memclear
    float X = (float)WSize[0]/2;
    float Y = (float)WSize[1]/2;
    float PointX;
    float PointY;
    int Radius = 80;
    float DeltaAngle;
    bool DirectionUP = true;
    bool RotationPositive = false;

    float Cords[2];
    SDL_SetRenderDrawColor(Render, 255, 0, 0, 255);
    for(int i=0;i<Vertice_Count;i++)
    {
        Vertex[i].color.r = 1;
        Vertex[i].color.g = 1;
        Vertex[i].color.b = 1;
        Vertex[i].color.a = 1;
        if(i==0)
        {
            PointX = X;
            PointY = Y - Radius;
            Cords[0] = X;
            Cords[1] = Y - Radius;
            Vertex[i].position.x = Cords[0];
            Vertex[i].position.y = Cords[1];
            SDL_RenderPoint(Render, Cords[0], Cords[1]);
            //SDL_RenderLine(Render, X, Y, Cords[0], Cords[1]);
            continue;
        }
        PointX = PointX - X;
        PointY = PointY - Y;
        Cords[0] = (PointX*cos(Spinning_Angle * i)-PointY*sin(Spinning_Angle * i)) + X;
        Cords[1] = (PointX*sin(Spinning_Angle * i)+PointY*cos(Spinning_Angle * i)) + Y;
        SDL_RenderPoint(Render, Cords[0], Cords[1]);
        Vertex[i].position.x = Cords[0];
        Vertex[i].position.y = Cords[1];
        //SDL_RenderLine(Render, X, Y, Cords[0], Cords[1]);
        PointX = PointX + X;
        PointY = PointY + Y;
    }
    SDL_RenderGeometry(Render, NULL, Vertex, Vertice_Count, NULL, 0);
    SDL_SetRenderDrawColor(Render, 0, 0, 255, 255);
    SDL_RenderPoint(Render, X, Y);
    SDL_SetRenderDrawColor(Render, 0, 0, 0, 255);

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

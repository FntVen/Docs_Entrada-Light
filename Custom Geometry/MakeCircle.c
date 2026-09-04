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
    float Vertex_Angle = (float)(180+(180*(IN_EdgeNum - 3)))/IN_EdgeNum;
    float Turning_Angle = 180 - Vertex_Angle;
    SDL_Vertex *Vertexes[IN_EdgeNum];
    float Previous_Coords[2] = {0};
    for(int i=0;i<=IN_EdgeNum;i++)
    {
        if(i==0)
        {
            Vertexes[i]->position.x = IN_X;
            Vertexes[i]->position.y = IN_Y - IN_Radius;
            Previous_Coords[0] = Vertexes[i]->position.x;
            Previous_Coords[1] = Vertexes[i]->position.y;
        }
        float OldTemp_X = Previous_Coords[0] - IN_X;
        float OldTemp_Y = Previous_Coords[1] - IN_Y;

        float NewTemp_X = IN_Radius*cos(Turning_Angle)*cos(90 - (Turning_Angle * i))-IN_Radius*sin(Turning_Angle)*sin(90 - (Turning_Angle * i));
        float NewTemp_Y = IN_Radius*sin(Turning_Angle)*cos(90 - (Turning_Angle * i))+IN_Radius*cos(Turning_Angle)*sin(90 - (Turning_Angle * i));

        Vertexes[i]->position.x = NewTemp_X + IN_X;
        Vertexes[i]->position.y = NewTemp_Y + IN_Y;

        Previous_Coords[0] = Vertexes[i]->position.x;
        Previous_Coords[1] = Vertexes[i]->position.y;
    }
    OUT_Vert = Vertexes;//Arrays automatically act as pointers
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

    //Complex Shape
    SDL_SetRenderDrawColor(Render, 255, 255, 255, 255);
    int Vert_Num = 4;
    int Global_Coordinates[2] = {0};
    int Vert_Angle = (180+(180*(Vert_Num -3)))/Vert_Num;
    float Radius = 100;

    SDL_GetWindowSizeInPixels(window, &Global_Coordinates[0], &Global_Coordinates[1]);
    float X = (float)Global_Coordinates[0]/2;
    float Y = (float)Global_Coordinates[1]/2;
    SDL_Vertex *Vertexes[3];
    Render_PolyGon(X, Y, 10, 3, Vertexes);
    for(int i=0;i<3;i++)
    {
        printf("Vertex %d: X-> %f| Y-> %f \n",i,Vertexes[i]->position.x,Vertexes[i]->position.y);
    }
    SDL_RenderGeometry(Render, NULL, *Vertexes, 3, NULL, 0);
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

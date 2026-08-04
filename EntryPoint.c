//UI Goals
// - Main Screen controlled by side bar
// - Side bar contents should be something close to [Cliente - Estrutura - Materiais - Documento]
// - Loading bar when creating files?
// - Maps support ideias [QTMaps - ]
#define CLAY_IMPLEMENTATION
#include "clay.h"
#include "renderers/raylib/clay_renderer_raylib.c"

int main()
{
    //Clay Setup
    Clay_Raylib_Initialize(FLAG_WINDOW_RESIZABLE);
    uint64_t Clay_Mem = Clay_MinMemorySize(); //The minimum memory necessary for an arena
    Clay_Arena Arena = (Clay_Arena)
    {
        .memory = malloc(Clay_Mem),
        .capacity = Clay_Mem
    };
    Clay_Init(Arena, (Clay_Dimensions) //Create MainScreen
    {
        .height = GetScreenHeight(),
        .width = GetScreenWidth()
    });
    //Mainlopp (Immediate mode)
    while (!WindowShouldClose())
    {
        Clay_BeginLayout();
        //Begin UI
        CLAY
        (
          
        ){}
        //End UI
        Clay_RenderCommandArray RCommands = Clay_EndLayout();
        BeginDrawing();
        Clay_Raylib_Render(RCommands);
        EndDrawing();

    }
}

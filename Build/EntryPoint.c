//UI Goals
// - Main Screen controlled by side bar
// - Side bar contents should be something close to [Cliente - Estrutura - Materiais - Documento]
// - Loading bar when creating files?
// - Maps support ideias [QTMaps - ]
#include "Lib/raylib/raylib.h"
#define CLAY_IMPLEMENTATION
#include "Lib/clay.h"
#include "Lib/raylib/clay_renderer_raylib.c"

void Clay_Erno(Clay_ErrorData errorData)
{
    printf("Clay Triggered Error: %s",errorData.errorText.chars);
}

int main()
{
    //Clay Setup
    Clay_Raylib_Initialize(1024, 768, "He-LightBulb", FLAG_VSYNC_HINT | FLAG_WINDOW_RESIZABLE /*| FLAG_MSAA_4X_HINT (Check performance and style later) */);
    uint64_t Clay_Mem = Clay_MinMemorySize(); //The minimum memory necessary for an arena
    Clay_Arena Arena = (Clay_Arena)
    {
        .memory = malloc(Clay_Mem),
        .capacity = Clay_Mem
    };
    Clay_Initialize
    (
        Arena,
        (Clay_Dimensions)
        {
            .height = (float)GetScreenHeight(),
            .width = (float)GetScreenWidth()
        },
        (Clay_ErrorHandler) {Clay_Erno, 0}
    );
    Font font[1];
    font[0] =LoadFont("Resources/Roboto-Regular.ttf");
    //Mainlopp (Immediate mode)
    while (!WindowShouldClose())
    {
        Clay_BeginLayout();
        //Begin UI
        //CLAY
        //(){}
        //End UI
        Clay_RenderCommandArray RCommands = Clay_EndLayout(GetFrameTime());
        BeginDrawing();
        ClearBackground(BLACK);
        Clay_Raylib_Render(RCommands, font);
        EndDrawing();

    }
}

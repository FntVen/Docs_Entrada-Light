#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <stdio.h>
#define SOKOL_IMPL
#define SOKOL_GLCORE
#define CLAY_IMPLEMENTATION
#define SOKOL_CLAY_IMPL
#define FONTSTASH_IMPLEMENTATION
#include "Lib/SOKOL/sokol_gfx.h"
#include "Lib/SOKOL/util/sokol_gl.h"
#include "Lib/SOKOL/sokol_app.h"
#include "Lib/SOKOL/sokol_glue.h"
#include "Lib/SOKOL/sokol_log.h"
#include "Lib/CLAY/clay.h"
#include "Lib/SOKOL/stb_truetype.h"
#include "Lib/SOKOL/fontstash.h"
#include "Lib/SOKOL/util/sokol_fontstash.h"
#include "Lib/CLAY/sokol_clay.h"

static sclay_font_t fonts[1];

static struct {
    float mouse_x;
    float mouse_y;
    bool mouse_down;
} Pointer_State = {0};

static void init()
{
    sg_setup(&(sg_desc){
        .environment = sglue_environment(),
        .logger.func = slog_func,
    });
    sgl_setup(&(sgl_desc_t){
        .logger.func = slog_func,
    });
    sclay_setup();
    uint64_t totalMemorySize = Clay_MinMemorySize();
    Clay_Arena clayMemory = Clay_CreateArenaWithCapacityAndMemory(totalMemorySize, malloc(totalMemorySize));
    Clay_Initialize(clayMemory, (Clay_Dimensions){ (float)sapp_width(), (float)sapp_height() }, (Clay_ErrorHandler){0});
    fonts[0] = sclay_add_font("resources/Roboto-Regular.ttf");
    Clay_SetMeasureTextFunction(sclay_measure_text, NULL);
}

void ClientMenu()/* (1)Nome - (2)CPF/CNPJ - (3)Telefone - (4)Email - (5)Endereço/ImagemLocal */
{}
void SolarMenu()/* (1)Quantidade/Potencia/Marca/Modelo dos Paineis - (2)Quantidade/Potencia/Marca/Modelo dos Inversores - (3) Potencia do Kit (4) Area de Instalaçao */
{}
void InstMenu()/* (_1)Codigo do Cliente - (_2)Codigo da Instalaçao - (3)ART - (4)Diametro dos cabos/terra - (5) Disjuntor - (6) Telha/Solo - (7) Grupo/A/B - (8) Area/Sub */
{}
void MiscMenu()/*(1)Data de Criaçao - (2)Data de Instalaçao*/
{}

void HandleButtonInteraction(Clay_ElementId elementId, Clay_PointerData pointerData, void * userData) {
    if (pointerData.state == CLAY_POINTER_DATA_PRESSED_THIS_FRAME) {
        printf("Sokol + Clay: Botão pressionado!\n");
    }
}




Clay_RenderCommandArray MainPage()
{
    Clay_SetPointerState((Clay_Vector2){ Pointer_State.mouse_x, Pointer_State.mouse_y }, Pointer_State.mouse_down);
    Clay_BeginLayout();
    Clay_Sizing layoutExpand =
    {
        .width = CLAY_SIZING_GROW(0),
        .height = CLAY_SIZING_GROW(0)
    };

    CLAY(//Definition of Parent
        CLAY_ID("Main_Column"),
        {
            .backgroundColor = {43, 41, 51, 255},
            .layout =
            {
                .layoutDirection = CLAY_TOP_TO_BOTTOM,
                .sizing = layoutExpand,
                .padding = {.left = 10, .right = 10, .bottom = 10, .top = 10},
                .childGap = 20
            }
        }
    )
    {//Child Element
        CLAY(
            CLAY_ID("Main_Row"),
            {
                .layout =
                {
                    .layoutDirection = CLAY_LEFT_TO_RIGHT,
                    .sizing = layoutExpand,
                    .padding = {0, 0, 20, 20},
                    .childGap = 20
                }
            }
        )
        {
            CLAY(
                CLAY_ID("Select_Men"),
                {
                    .backgroundColor = {53, 51, 61, 255},
                    .cornerRadius = {15,15,15,15},
                    .layout =
                    {
                        .layoutDirection = CLAY_TOP_TO_BOTTOM,
                        .sizing = {.height = layoutExpand.height, .width = 300},
                        .padding = {.left = 10, .right = 10, .top = 7, .bottom = 7},
                        .childGap = 20,
                    },
                }
            )
            {
                CLAY(
                    CLAY_ID("ClientMen_Btn"),
                    {                                       /*Orange*/                        /*Gray*/
                        .backgroundColor = Clay_Hovered() ? (Clay_Color){255, 167, 73, 255} : (Clay_Color){250, 250, 250, 20},
                        .cornerRadius = {15,15,15,15},
                        .layout =
                        {
                            .layoutDirection = CLAY_LEFT_TO_RIGHT,
                            .sizing = {.height = 33, .width = 280}
                        }
                    }
                )
                {
                    Clay_OnHover(HandleButtonInteraction, 0);
                    CLAY_TEXT(
                    CLAY_STRING("Text"),
                    {
                        .textAlignment = CLAY_TEXT_ALIGN_CENTER,
                        .textColor = {255,255,255,255},
                        .fontSize = 20,
                        .fontId = 0
                    });
                }
            }
            CLAY(
                CLAY_ID("Focus_Men"),
                {
                    .backgroundColor = {53, 51, 61, 255},
                    .cornerRadius = {15,15,15,15},
                    .layout =
                    {
                        .layoutDirection = CLAY_TOP_TO_BOTTOM,
                        .sizing = layoutExpand,
                        .padding = {0, 0, 20, 20},
                        .childGap = 20
                    }
                }
            )
            {

            }
        }
    }
    return Clay_EndLayout(0);
}

static void frame()
{
    sclay_new_frame();
    Clay_RenderCommandArray renderCommands = MainPage();//CornerRadiusTest();// Should return an array of Clay drawings

    sg_begin_pass(&(sg_pass){ .swapchain = sglue_swapchain() });
    sgl_matrix_mode_modelview();
    sgl_load_identity();
    sclay_render(renderCommands, NULL);
    sgl_draw();
    sg_end_pass();
    sg_commit();
}

static void event(const sapp_event *ev)// Handler of Keyboard an mouse events
{
    if(ev->type == SAPP_EVENTTYPE_KEY_DOWN && ev->key_code == SAPP_KEYCODE_D){
        Clay_SetDebugModeEnabled(true);
    } else {
        sclay_handle_event(ev);
    }
    if (ev->type == SAPP_EVENTTYPE_MOUSE_MOVE) {
            Pointer_State.mouse_x = ev->mouse_x;
            Pointer_State.mouse_y = ev->mouse_y;
        }
        else if (ev->type == SAPP_EVENTTYPE_MOUSE_DOWN && ev->mouse_button == SAPP_MOUSEBUTTON_LEFT) {
            Pointer_State.mouse_down = true;
        }
        else if (ev->type == SAPP_EVENTTYPE_MOUSE_UP && ev->mouse_button == SAPP_MOUSEBUTTON_LEFT) {
            Pointer_State.mouse_down = false;
        }

}

static void cleanup()
{
    sclay_shutdown();
    sgl_shutdown();
    sg_shutdown();
}

sapp_desc sokol_main(int argc, char **argv)//This is the new main of the application
{
    return (sapp_desc){
        .init_cb = init,
        .frame_cb = frame,
        .event_cb = event,
        .cleanup_cb = cleanup,
        .window_title = "Luciano Ui Test - 1",
        .width = 1200,
        .height = 600,
        .icon.sokol_default = true,
        .logger.func = slog_func,
    };
}

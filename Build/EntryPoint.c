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
int DebugLevel = 2;
static int FontIndex[5] = {0};
FONScontext *Stash;
static struct {
    float mouse_x;
    float mouse_y;
    bool mouse_down;
} Pointer_State = {0};

static Clay_Dimensions ClayFontCalc(Clay_StringSlice text, Clay_TextElementConfig *config, void *userData)
{
    fonsClearState(Stash);
    fonsSetFont(Stash, (int)config->fontId);
    fonsSetSize(Stash, config->fontSize);
    fonsSetColor(Stash, sfons_rgba((int)config->textColor.r, (int)config->textColor.g, (int)config->textColor.b, (int)config->textColor.a));
    fonsSetAlign(Stash, config -> textAlignment);

    float bounds[4];
    fonsTextBounds(Stash, 0/*Offset*/, 0/*Offset*/, text.chars, text.chars + text.length, bounds);
    float width = bounds[2] - bounds[0];
    float height = config->fontSize;
    if(DebugLevel >= 1)
    {
        printf("FontID: %d \n",config ->fontId);
        printf("FontSize: %d \n",config->fontSize);
        printf("Fontheight: %d | Fontwidth: %d \n",(int)height,(int)width);
        printf("FontColor: R-> %d | G -> %d | B -> %d \n",(int)config->textColor.r,(int)config->textColor.g,(int)config->textColor.b);
        printf("AlignmentID: %d \n",config ->textAlignment);
        printf("TextData: ");
        for(int i = 0; i <=text.length; i++)
        {
            printf("%c",text.chars[i]);
        }
        printf("\n");
    }

    return (Clay_Dimensions){ .width = width, .height = height };
}

void HandleButtonInteraction(Clay_ElementId elementId, Clay_PointerData pointerData, void * userData) {
    if (pointerData.state == CLAY_POINTER_DATA_PRESSED_THIS_FRAME) {
        printf("Please\n");
    }
}

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
    Stash = sfons_create(&(sfons_desc_t){.height = 512, .width = 512});
    FontIndex[0] = fonsAddFont(Stash, "Roboto","resources/Roboto-Regular.ttf");
    if(DebugLevel >= 1)
    {
        FontIndex[1] = fonsAddFont(Stash, "Roboto2","resources/Roboto-Regular.ttf");//test
        for(int i = 0; i <= sizeof(FontIndex) - 1; i++)
        {
            printf("Id of Font: %d At index : %d ",FontIndex[i],i);
        }
        printf("\n");
    }
    Clay_SetMeasureTextFunction(ClayFontCalc, Stash);
}

void ClientMenu()/* (1)Nome - (2)CPF/CNPJ - (3)Telefone - (4)Email - (5)Endereço/ImagemLocal */
{}
void SolarMenu()/* (1)Quantidade/Potencia/Marca/Modelo dos Paineis - (2)Quantidade/Potencia/Marca/Modelo dos Inversores - (3) Potencia do Kit (4) Area de Instalaçao */
{}
void InstMenu()/* (_1)Codigo do Cliente - (_2)Codigo da Instalaçao - (3)ART - (4)Diametro dos cabos/terra - (5) Disjuntor - (6) Telha/Solo - (7) Grupo/A/B - (8) Area/Sub */
{}
void MiscMenu()/*(1)Data de Criaçao - (2)Data de Instalaçao - (3) */
{}

Clay_RenderCommandArray MainPage()
{
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
                            .sizing = {.height = 33, .width = 280},
                        }
                    }
                )
                {
                Clay_OnHover(HandleButtonInteraction, 0);
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
            CLAY_TEXT(
                CLAY_STRING("Hand"),
                CLAY_TEXT_CONFIG({
                    .textAlignment = CLAY_TEXT_ALIGN_CENTER,
                    .textColor = {255,255,255,255},
                    .fontSize = 10,
                    .fontId = FontIndex[0],
                })
            );
            }
        }
    }
    return Clay_EndLayout(0);
}

static void frame()
{
    sclay_new_frame();

    Clay_RenderCommandArray renderCommands = MainPage();//CornerRadiusTest();// Should return an array of Clay drawings
    for(int i = 0; i <= renderCommands.length - 1; i++)
    {
        Clay_RenderCommand *cmd = Clay_RenderCommandArray_Get(&renderCommands, i);

            switch (cmd->commandType)
            {
                case CLAY_RENDER_COMMAND_TYPE_TEXT: {
                    if(DebugLevel >= 2)
                    {
                        printf("In Switch case Type Text\n");
                    }
                    Clay_TextRenderData *textData = &cmd->renderData.text;

                    fonsSetFont(Stash, textData->fontId);
                    fonsSetSize(Stash, textData->fontSize);
                    fonsSetColor(Stash, sfons_rgba(textData->textColor.r, textData->textColor.g, textData->textColor.b, textData->textColor.a));
                    if(DebugLevel >= 2)
                    {
                        printf("Data sent to Stash: \n");
                        printf("FontId: %d \n",textData->fontId);
                        printf("FontSize: %d \n",textData->fontSize);
                        printf("FontColor: R -> %d | G -> %d | B -> %d | A -> %d \n",(int)textData->textColor.r,(int)textData->textColor.g,(int)textData->textColor.b,(int)textData->textColor.a);
                        printf("Text: ");
                        for(int i = 0; i <=textData->stringContents.length; i++)
                        {
                            printf("%c", textData->stringContents.chars[i]);
                        }
                        printf("\n");
                        printf("Last character: %c \n",textData->stringContents.chars[textData->stringContents.length - 1]);
                        printf("Bounding Boxes: PositionX %f SizeX %f | PositionY %f SizeY %f \n",cmd->boundingBox.x,cmd->boundingBox.width,cmd->boundingBox.y,cmd->boundingBox.height);
                    }

                    fonsDrawText(
                        Stash,
                        textData->fontSize,
                        textData->lineHeight + textData->fontSize, // FontStash draws from baseline
                        textData->stringContents.chars,
                        textData->stringContents.chars + textData->stringContents.length
                    );
                    if(DebugLevel >= 2)
                    {
                       printf("End of Switch \n");
                    }
                    break;
                }
    }

    sg_begin_pass(&(sg_pass){ .swapchain = sglue_swapchain() });
    sgl_matrix_mode_modelview();
    sgl_load_identity();
    sclay_render(renderCommands, FontIndex);
    sfons_flush(Stash);
    sgl_draw();
    sg_end_pass();
    sg_commit();
    }
}

static void event(const sapp_event *ev)
{
    if(ev->type == SAPP_EVENTTYPE_KEY_DOWN && ev->key_code == SAPP_KEYCODE_D){
        Clay_SetDebugModeEnabled(true);
    } else {
        sclay_handle_event(ev);
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
        .window_title = "Luciano Ui Test - 2",
        .width = 1200,
        .height = 600,
        .icon.sokol_default = true,
        .logger.func = slog_func,
    };
}

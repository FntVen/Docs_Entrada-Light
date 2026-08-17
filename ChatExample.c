#define SOKOL_IMPL
#define SOKOL_GLCORE33 // Defina seu backend gráfico (ex: SOKOL_METAL, SOKOL_D3D11)
#include "sokol_app.h"
#include "sokol_gfx.h"
#include "sokol_glue.h"

#define SOKOL_GL_IMPL
#include "util/sokol_gl.h" // Facilita o desenho de formas 2D extraídas do Clay

#define CLAY_IMPLEMENTATION
#include "clay.h"

#include <stdio.h>

// Estado global para rastrear o Mouse via Sokol App
static struct {
    float mouse_x;
    float mouse_y;
    bool mouse_down;
} app_state = {0};

// Callback executado quando o botão do Clay é clicado com sucesso
void HandleButtonInteraction(Clay_ElementId elementId, Clay_PointerData pointerData, intptr_t userData) {
    if (pointerData.state == CLAY_POINTER_DATA_PRESSED_THIS_FRAME) {
        printf("Sokol + Clay: Botão pressionado!\n");
    }
}

// Inicialização (Executado uma vez pelo Sokol)
static void init(void) {
    // Inicializa o contexto gráfico básico do Sokol Gfx
    sg_setup(&(sg_desc){ .environment = sglue_environment() });
    sgl_setup(&(sgl_desc){0});

    // Inicializa a memória interna do Clay (Arena Estática)
    uint64_t totalMemorySize = Clay_MinMemorySize();
    Clay_Arena clayArena = Clay_CreateArenaWithCapacity(totalMemorySize, malloc(totalMemorySize));
    
    Clay_Initialize(clayArena, (Clay_Dimensions){ (float)sapp_width(), (float)sapp_height() }, (Clay_ErrorHandler){0});
}

// Captura de Eventos do Sokol (Roteamento para os inputs do Clay)
static void event(const sapp_event* ev) {
    if (ev->type == SAPP_EVENTTYPE_MOUSE_MOVE) {
        app_state.mouse_x = ev->mouse_x;
        app_state.mouse_y = ev->mouse_y;
    } 
    else if (ev->type == SAPP_EVENTTYPE_MOUSE_DOWN && ev->mouse_button == SAPP_MOUSEBUTTON_LEFT) {
        app_state.mouse_down = true;
    } 
    else if (ev->type == SAPP_EVENTTYPE_MOUSE_UP && ev->mouse_button == SAPP_MOUSEBUTTON_LEFT) {
        app_state.mouse_down = false;
    }
}

// Renderização dos comandos primitivos do Clay via Sokol GL
void DrawClayCommands(Clay_RenderCommandArray renderCommands) {
    sgl_defaults();
    sgl_ortho(0.0f, (float)sapp_width(), (float)sapp_height(), 0.0f, -1.0f, 1.0f);

    for (int i = 0; i < renderCommands.length; i++) {
        Clay_RenderCommand *cmd = Clay_GetRenderCommand(&renderCommands, i);

        if (cmd->commandType == CLAY_RENDER_COMMAND_TYPE_RECTANGLE) {
            Clay_RectangleRenderData *rect = &cmd->renderData.rectangle;
            Clay_BoundingBox bounds = cmd->boundingBox;

            // Transpõe a cor do Clay para o Sokol GL (0.0f a 1.0f)
            sgl_c4f(rect->color.r / 255.0f, rect->color.g / 255.0f, rect->color.b / 255.0f, rect->color.a / 255.0f);
            
            // Desenha um retângulo preenchido nas coordenadas estipuladas pelo Clay
            sgl_begin_quads();
            sgl_v2f(bounds.x, bounds.y);
            sgl_v2f(bounds.x + bounds.width, bounds.y);
            sgl_v2f(bounds.x + bounds.width, bounds.y + bounds.height);
            sgl_v2f(bounds.x, bounds.y + bounds.height);
            sgl_end();
        }
        // Nota: Tratativas adicionais para TEXT e SCISSOR (Recorte) viriam aqui.
    }
}

// Atualização de Frame (Loop Principal do Sokol)
static void frame(void) {
    // 1. Atualiza as dimensões da tela e inputs no Clay
    Clay_SetLayoutDimensions((Clay_Dimensions){ (float)sapp_width(), (float)sapp_height() });
    Clay_SetPointerState((Clay_Vector2){ app_state.mouse_x, app_state.mouse_y }, app_state.mouse_down);

    // 2. Constrói a UI declarativa do Clay
    Clay_BeginLayout();
    
    CLAY(
        CLAY_ID("LayoutRoot"), 
        CLAY_LAYOUT({ 
            .layoutDirection = CLAY_TOP_TO_BOTTOM, 
            .padding = { 20, 20, 20, 20 } 
        })
    ) {
        // Declaração do elemento do Botão
        CLAY(
            CLAY_ID("SokolButton"),
            CLAY_LAYOUT({ .padding = { 24, 24, 16, 16 } }),
            CLAY_RECTANGLE({
                // Altera a cor dinamicamente se houver colisão (Hover)
                .color = Clay_Hovered() ? (Clay_Color){ 52, 152, 219, 255 } : (Clay_Color){ 41, 128, 185, 255 }
            })
        ) {
            // Conecta o callback do Sokol ao botão do Clay
            Clay_OnHover(HandleButtonInteraction, 0);
        }
    }

    Clay_RenderCommandArray renderCommands = Clay_EndLayout();

    // 3. Renderização Final no Buffer da Janela
    sg_pass pass = { .action = { .colors[0] = { .load_action = SG_LOADACTION_CLEAR, .clear_value = { 0.2f, 0.2f, 0.2f, 1.0f } } }, .swapchain = sglue_swapchain() };
    sg_begin_pass(&pass);
    
    // Processa os comandos do Clay gerados neste frame
    DrawClayCommands(renderCommands);
    sgl_draw();

    sg_end_pass();
    sg_commit();
}

// Finalização
static void cleanup(void) {
    sgl_shutdown();
    sg_shutdown();
}

// Ponto de Entrada Padrão do Sokol App
sapp_desc sokol_main(int argc, char* argv[]) {
    (void)argc; (void)argv;
    return (sapp_desc){
        .init_cb = init,
        .frame_cb = frame,
        .cleanup_cb = cleanup,
        .event_cb = event,
        .width = 800,
        .height = 600,
        .window_title = "Clay UI + Sokol App",
    };
}

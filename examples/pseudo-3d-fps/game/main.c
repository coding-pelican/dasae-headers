#include "../engine/include/engine.h"
#include "src/State.h"
#include "src/Screen.h"

// main.c
int main(void) {
    // Initialize platform with terminal backend
    let window = ResErr_Ptr_engine_Window_unwrap(
        engine_Window_create(
            PtrConst_engine_PlatformParams_from(
                &(engine_PlatformParams){
                    .backend_type = engine_RenderBackendType_vt100,
                    .window_title = strL("Pseudo 3d FPS"),
                    .width        = 120,
                    .height       = 40 }
            )
        )
    );
    engine_Input_init();
    printf("engine initialized\n");

    // Create canvases
    let game_canvas = ResErr_Ptr_engine_Canvas_unwrap(
        engine_Canvas_create(120, 40, engine_CanvasType_rgba)
    );
    let ui_canvas = ResErr_Ptr_engine_Canvas_unwrap(
        engine_Canvas_create(120, 40, engine_CanvasType_rgba)
    );
    let minimap_canvas = ResErr_Ptr_engine_Canvas_unwrap(
        engine_Canvas_create(30, 30, engine_CanvasType_rgba)
    );
    printf("canvas created\n");

    // Add canvas views
    engine_Window_addCanvasView(window, game_canvas, 0, 0, 120, 40);
    engine_Window_addCanvasView(window, ui_canvas, 0, 0, 120, 40);
    engine_Window_addCanvasView(window, minimap_canvas, 85, 5, 30, 30);
    printf("canvas bind\n");

    let state = ResErr_Ptr_game_State_unwrap(
        game_State_create()
    );
    printf("game state created\n");

    // Game loop
    while (state.addr->is_running) {
        let elapsed_time = 0.016f; // Assume 60 FPS for simplicity

        // Update game state
        game_State_update(state, elapsed_time);

        // Render all views
        game_Screen_renderFirstPersonView(game_canvas, (PtrConst_game_State){ .base = state.base });
        game_Screen_renderUi(ui_canvas, (PtrConst_game_State){ .base = state.base });
        game_Screen_renderMinimap(minimap_canvas, (PtrConst_game_State){ .base = state.base });

        // Present to screen
        engine_Window_present(window);
    }

    engine_Canvas_destroy(minimap_canvas);
    engine_Canvas_destroy(ui_canvas);
    engine_Canvas_destroy(game_canvas);
    engine_Window_destroy(window);

    return 0;
}

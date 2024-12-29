#include "../engine/include/engine.h"
#include "dh/time/Duration.h"
#include "src/State.h"
#include "src/Screen.h"

Err$void dh_main(int argc, const char* argv[]) {
    reserveReturn(Err$void);
    unused(argc);
    unused(argv);

    // Initialize platform with terminal backend
    let window = try(engine_Window_create(
        &(engine_PlatformParams){
            .backend_type = engine_RenderBackendType_vt100,
            .window_title = "Pseudo 3d FPS",
            .width        = 120,
            .height       = 80,
        }
    ));
    engine_Input_init();
    printf("engine initialized\n");

    // Create canvases
    let game_canvas    = try(engine_Canvas_create(120, 80, engine_CanvasType_rgba));
    let ui_canvas      = try(engine_Canvas_create(120, 80, engine_CanvasType_rgba));
    let minimap_canvas = try(engine_Canvas_create(30, 30, engine_CanvasType_rgba));
    printf("canvas created\n");

    // Add canvas views
    engine_Window_addCanvasView(window, game_canvas, 0, 0, 120, 80);
    engine_Window_addCanvasView(window, ui_canvas, 0, 0, 120, 80);
    engine_Window_addCanvasView(window, minimap_canvas, 85, 5, 30, 30);
    printf("canvas bind\n");

    let state = try(game_State_create());
    printf("game state created\n");

    // Game loop
    while (state->is_running) {
        // let elapsed_time = 0.016f; // Assume 60 FPS for simplicity
        let elapsed_time = 1.0f; // Assume 60 FPS for simplicity

        // Update game state
        game_State_update(state, elapsed_time);

        // Render all views
        game_Screen_renderFirstPersonView(game_canvas, state);
        game_Screen_renderUi(ui_canvas, state);
        game_Screen_renderMinimap(minimap_canvas, state);

        // Present to screen
        engine_Window_present(window);
        time_SysTime_sleep(time_Duration_fromSecs_f64(elapsed_time));
    }

    engine_Canvas_destroy(minimap_canvas);
    engine_Canvas_destroy(ui_canvas);
    engine_Canvas_destroy(game_canvas);
    engine_Window_destroy(window);

    return_void();
}

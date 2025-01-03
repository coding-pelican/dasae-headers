#include "dh/main.h"
#include "dh/log.h"

#include "dh/time/SysTime.h"
#include "dh/time/Instant.h"
#include "dh/time/Duration.h"

#include "../engine/include/engine.h"
#include "src/State.h"
#include "src/Screen.h"

// TODO: Testing canvas blit scale behavior, breaking down canvas types and rendering methods

Err$void dh_main(int argc, const char* argv[]) {
    reserveReturn(Err$void);
    unused(argc), unused(argv);

    // Initialize logging to a file
    scope_if(let debug_file = fopen("main-debug.log", "w"), debug_file) {
        log_initWithFile(debug_file);
        // Configure logging behavior
        log_setLevel(log_Level_debug);
        log_showTimestamp(true);
        log_showLevel(true);
        log_showLocation(false);
        log_showFunction(true);
    }

    // Initialize platform with terminal backend
    let window = try(engine_Window_create(
        &(engine_PlatformParams){
            .backend_type = engine_RenderBackendType_vt100,
            .window_title = "Pseudo 3d FPS",
            .width        = 80,
            .height       = 50,
        }
    ));
    log_info("engine initialized\n");

    // Create canvases
    let game_canvas    = try(engine_Canvas_create(80, 50, engine_CanvasType_rgba));
    let ui_canvas      = try(engine_Canvas_create(80, 50, engine_CanvasType_rgba));
    let minimap_canvas = try(engine_Canvas_create(32, 32, engine_CanvasType_rgba));
    log_info("canvas created\n");

    engine_Canvas_clear(game_canvas, Color_blank);
    engine_Canvas_clear(ui_canvas, Color_blank);
    engine_Canvas_clear(minimap_canvas, Color_blank);
    log_info("canvas cleared\n");

    // Add canvas views
    engine_Window_addCanvasView(window, game_canvas, 0, 0, 80, 50);
    engine_Window_addCanvasView(window, ui_canvas, 0, 0, 80, 50);
    engine_Window_addCanvasView(window, minimap_canvas, 1, 1, 32, 32);
    log_info("canvas views added\n");

    let state = try(game_State_create());
    log_info("game state created\n");
    ignore getchar();

    var curr_time   = time_Instant_now();
    var prev_time   = curr_time;
    let target_time = time_Duration_fromSecs_f64(0.016f); // Assume 62.5 FPS for simplicity

    // Game loop
    while (state->is_running) {
        curr_time        = time_Instant_now();
        let elapsed_time = time_Instant_durationSince(curr_time, prev_time);
        let dt           = time_Duration_asSecs_f64(elapsed_time);

        // Process events
        engine_Window_processEvents(window);

        // Update game state
        game_State_update(state, as(f32, dt));

        // Render all views
        game_Screen_renderFirstPersonView(game_canvas, state);
        game_Screen_renderUi(ui_canvas, state);
        game_Screen_renderMinimap(minimap_canvas, state);

        // Present to screen
        engine_Window_present(window);

        // Sleep for the remaining time to maintain FPS
        time_SysTime_sleep(time_Duration_sub(target_time, elapsed_time));
        prev_time = curr_time;
    }

    engine_Canvas_destroy(minimap_canvas);
    engine_Canvas_destroy(ui_canvas);
    engine_Canvas_destroy(game_canvas);
    engine_Window_destroy(window);
    log_fini();

    return_void();
}

#include "dh/main.h"
#include "dh/core.h"
#include "dh/log.h"

#include "dh/time/Duration.h"
#include "dh/time/Instant.h"

#include "engine.h"

Err$void dh_main(int argc, const char* argv[]) {
    reserveReturn(Err$void);
    unused(argc), unused(argv);

    scope_defer {
        // Initialize logging to a file
        scope_if(let debug_file = fopen("sample-draw_box-debug.log", "w"), debug_file) {
            log_initWithFile(debug_file);
            // Configure logging behavior
            log_setLevel(log_Level_debug);
            log_showTimestamp(true);
            log_showLevel(true);
            log_showLocation(false);
            log_showFunction(true);
        }
        defer(log_fini());

        // Initialize platform with terminal backend
        let window = try_defer(engine_Window_create(
            &(engine_PlatformParams){
                .backend_type = engine_RenderBackendType_vt100,
                .window_title = "Draw Box",
                .width        = 80,
                .height       = 50,
            }
        ));
        defer(engine_Window_destroy(window));
        log_info("engine initialized\n");

        // Create canvases
        let game_canvas = catch (engine_Canvas_create(80, 50, engine_CanvasType_rgba), err, {
            log_error("Failed to create canvas: %s\n", err);
            defer_return_err(err);
        });
        defer(engine_Canvas_destroy(game_canvas));
        log_info("canvas created\n");

        engine_Canvas_clear(game_canvas, Color_black);
        log_info("canvas cleared\n");

        // Add canvas views
        engine_Window_addCanvasView(window, game_canvas, 0, 0, 80, 50);
        log_info("canvas views added\n");

        var  zero_time   = time_Instant_now();
        var  curr_time   = zero_time;
        var  prev_time   = curr_time;
        let  target_time = time_Duration_fromSecs_f64(0.016f); // Assume 62.5 FPS for simplicity
        bool is_running  = true;
        log_info("game loop started\n");

        while (is_running) {
            curr_time        = time_Instant_now();
            let elapsed_time = time_Instant_durationSince(curr_time, prev_time);
            let t            = time_Duration_asSecs_f64(time_Instant_durationSince(curr_time, zero_time));

            // Process events
            try_defer(engine_Window_processEvents(window));
            if (engine_Key_pressed(engine_KeyCode_esc)) {
                is_running = false;
            }

            // Update logic
            Vec2f rotation = { .x = (f32)cos(t), .y = (f32)sin(t) };
            Vec2f box_min  = { .x = -48.0f, .y = -24.0f };
            Vec2f box_max  = { .x = 48.0f, .y = 24.0f };
            Vec2f offset   = math_Vec2f_scale(rotation, 20.0f);
            box_min        = math_Vec2f_sub(box_min, offset);
            box_max        = math_Vec2f_add(box_max, offset);

            // Render to canvas
            engine_Canvas_clear(game_canvas, Color_black);
            /* Transform World to Screen Coord */ {
                const f32   canvas_scale  = 0.5f; // cuz logic based on 2x scale (160x100)
                const Vec2f canvas_center = { .x = (f32)game_canvas->width / 2.0f, .y = (f32)game_canvas->height / 2.0f };
                box_min                   = math_Vec2f_scale(box_min, canvas_scale);
                box_min.y                 = -box_min.y;
                box_min                   = math_Vec2f_add(box_min, canvas_center);
                box_max                   = math_Vec2f_scale(box_max, canvas_scale);
                box_max.y                 = -box_max.y;
                box_max                   = math_Vec2f_add(box_max, canvas_center);
            }

            engine_Canvas_drawRect(game_canvas, (i32)box_min.x, (i32)box_min.y, (i32)box_max.x, (i32)box_max.y, Color_white);

            // Present to screen
            engine_Window_present(window);

            // Sleep for the remaining time to maintain FPS
            time_sleep(time_Duration_sub(target_time, elapsed_time));
            prev_time = curr_time;
        }
        defer_return_void();
    }
    return_deferred;
}

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
        scope_if(let debug_file = fopen("sample-rotate_lines-debug.log", "w"), debug_file) {
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
                .window_title = "Rotate Lines",
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

        var curr_time       = time_Instant_now();
        var prev_time       = curr_time;
        let target_time     = time_Duration_fromSecs_f64(0.016f); // Assume 62.5 FPS for simplicity
        var simulation_time = time_Duration_zero;

        bool is_running = true;
        log_info("game loop started\n");

        while (is_running) {
            curr_time        = time_Instant_now();
            let elapsed_time = time_Instant_durationSince(curr_time, prev_time);
            simulation_time  = time_Duration_add(simulation_time, elapsed_time);
            let t            = time_Duration_asSecs_f64(simulation_time);

            // Process events
            try_defer(engine_Window_processEvents(window));
            if (engine_Key_pressed(engine_KeyCode_esc)) {
                is_running = false;
            }

            // Update with render to canvas
            let   r = math_Vec2f_sincos((f32)t * 0.05f);
            Vec2f o = cleared();
            Vec2f p = { .x = 50.0f, .y = 0.0f };

            engine_Canvas_clear(game_canvas, Color_black);
            for (i32 i = 0; i < 32; ++i) {
                Vec2f draw_origin = o;
                Vec2f draw_point  = p;
                /* Transform World to Screen Coord */ {
                    const f32   canvas_scale  = 0.25f; // cuz logic based on 4x scale (320x200)
                    const Vec2f canvas_center = { .x = (f32)game_canvas->width / 2.0f, .y = (f32)game_canvas->height / 2.0f };

                    draw_origin   = math_Vec2f_scale(draw_origin, canvas_scale);
                    draw_origin.y = -draw_origin.y;
                    draw_origin   = math_Vec2f_add(draw_origin, canvas_center);

                    draw_point   = math_Vec2f_scale(draw_point, canvas_scale);
                    draw_point.y = -draw_point.y;
                    draw_point   = math_Vec2f_add(draw_point, canvas_center);
                }
                engine_Canvas_drawLine(game_canvas, (i32)draw_origin.x, (i32)draw_origin.y, (i32)draw_point.x, (i32)draw_point.y, Color_white);

                p = (Vec2f){
                    .x = (r.x * p.x - r.y * p.y),
                    .y = (r.y * p.x + r.x * p.y)
                };
            }

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

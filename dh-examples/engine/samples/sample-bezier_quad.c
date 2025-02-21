#include "dh/main.h"
#include "dh/core.h"
#include "dh/log.h"

#include "dh/time/Duration.h"
#include "dh/time/Instant.h"

#include "engine.h"

static Vec2f bezierQuad(Vec2f a, Vec2f b, Vec2f c, f32 t) {
    // Quadratic Bezier curve formula: B(t) = (1-t)^2 * A + 2t(1-t) * B + t^2 * C
    let t2  = t * t;
    let mt  = 1.0f - t;
    let mt2 = mt * mt;
    return (Vec2f){
        .x = mt2 * a.x + 2.0f * t * mt * b.x + t2 * c.x,
        .y = mt2 * a.y + 2.0f * t * mt * b.y + t2 * c.y
    };
}

Err$void dh_main(int argc, const char* argv[]) {
    unused(argc), unused(argv);
    scope_reserveReturn(Err$void) {
        // Initialize logging to a file
        scope_if(let debug_file = fopen("sample-bezier_quad-debug.log", "w"), debug_file) {
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
        let window = try(engine_Window_create(
            &(engine_PlatformParams){
                .backend_type = engine_RenderBackendType_vt100,
                .window_title = "Bezier Quadratic Curve",
                .width        = 80,
                .height       = 50,
            }
        ));
        defer(engine_Window_destroy(window));
        log_info("engine initialized\n");

        // Create canvas
        let game_canvas = try(engine_Canvas_create(80, 50, engine_CanvasType_rgba));
        defer(engine_Canvas_destroy(game_canvas));
        log_info("canvas created\n");

        engine_Canvas_clear(game_canvas, Color_black);
        log_info("canvas cleared\n");

        // Add canvas views
        engine_Window_addCanvasView(window, game_canvas, 0, 0, 80, 50);
        log_info("canvas views added\n");

        // Control points for the bezier curve
        Vec2f a = { .x = -50.0f, .y = 0.0f };
        Vec2f b = { .x = -50.0f, .y = 50.0f };
        Vec2f c = { .x = 50.0f, .y = 25.0f };

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
            try(engine_Window_processEvents(window));
            if (engine_Key_pressed(engine_KeyCode_esc)) {
                is_running = false;
            }

            // Render to canvas
            engine_Canvas_clear(game_canvas, Color_black);

            // Transform coordinates to screen space
            const f32   canvas_scale  = 0.5f; // cuz logic based on 2x scale (160x100)
            const Vec2f canvas_center = {
                .x = as$(f32, game_canvas->width) / 2.0f,
                .y = as$(f32, game_canvas->height) / 2.0f
            };

            // Draw bezier curve segments
            f32 t0 = 0.0f;
            for (i32 i = 0; i <= 30; ++i) {
                f32   t1 = as$(f32, i) / 30.0f;
                Vec2f p0 = bezierQuad(a, b, c, t0);
                Vec2f p1 = bezierQuad(a, b, c, t1);

                // Transform points to screen space
                p0   = math_Vec2f_scale(p0, canvas_scale);
                p0.y = -p0.y;
                p0   = math_Vec2f_add(p0, canvas_center);

                p1   = math_Vec2f_scale(p1, canvas_scale);
                p1.y = -p1.y;
                p1   = math_Vec2f_add(p1, canvas_center);

                engine_Canvas_drawLine(game_canvas, as$(i32, p0.x), as$(i32, p0.y), as$(i32, p1.x), as$(i32, p1.y), Color_white);
                t0 = t1;
            }

            // Draw animated box along the curve
            f32   pt            = (math_cos((f32)t) + 1.0f) * 0.5f;
            Vec2f box_center    = bezierQuad(a, b, c, pt);
            Vec2f box_half_size = { .x = 2.5f, .y = 2.5f };
            Vec2f box_min       = math_Vec2f_sub(box_center, box_half_size);
            Vec2f box_max       = math_Vec2f_add(box_center, box_half_size);

            // Transform box to screen space
            box_min   = math_Vec2f_scale(box_min, canvas_scale);
            box_min.y = -box_min.y;
            box_min   = math_Vec2f_add(box_min, canvas_center);

            box_max   = math_Vec2f_scale(box_max, canvas_scale);
            box_max.y = -box_max.y;
            box_max   = math_Vec2f_add(box_max, canvas_center);

            engine_Canvas_drawRect(game_canvas, as$(i32, box_min.x), as$(i32, box_min.y), as$(i32, box_max.x), as$(i32, box_max.y), Color_white);

            // Present to screen
            engine_Window_present(window);

            // Sleep for the remaining time to maintain FPS
            if_some(time_Duration_chkdSub(target_time, elapsed_time), sleep_time) {
                time_sleep(sleep_time);
            }
            prev_time = curr_time;
        }
        return_void();
    }
    scope_returnReserved;
}

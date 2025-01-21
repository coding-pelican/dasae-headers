#include "dh/main.h"
#include "dh/debug.h"
#include "dh/log.h"

#include "dh/mem.h"
#include "dh/heap/Classic.h"
#include "dh/heap/Page.h"
#include "dh/ArrList.h"

#include "dh/time.h"
#include "dh/Random.h"

#include "engine.h"
#include "engine/canvas.h"
#include "engine/window.h"

#define window_res_width__320x200  /* template value */ (320)
#define window_res_height__320x200 /* template value */ (200)
#define window_res_width__160x100  /* template value */ (160)
#define window_res_height__160x100 /* template value */ (100)
#define window_res_width__80x50    /* template value */ (80)
#define window_res_height__80x50   /* template value */ (50)
#define window_res_width__40x25    /* template value */ (40)
#define window_res_height__40x25   /* template value */ (25)

#define window_res_width  (window_res_width__160x100)
#define window_res_height (window_res_height__160x100)
#define window_res_size   (as$(usize, window_res_width) * window_res_height)

/* (1.0 / render_target_fps__62_50) ~16ms => ~60 FPS, Assume 62.5 FPS for simplicity */
#define render_target_fps__125_0 /* template value */ (125.0)
#define render_target_fps__62_50 /* template value */ (62.50)
#define render_target_fps__50_00 /* template value */ (50.00)
#define render_target_fps__31_25 /* template value */ (31.25)

#define render_target_fps (render_target_fps__31_25)
#define render_target_spf (1.0 / render_target_fps)


use_Sli$(Vec2f);
use_ArrList$(Vec2f);
typedef ArrList$Vec2f Vec2fs;
use_Sli$(Color);
use_ArrList$(Color);
typedef ArrList$Color Colors;

typedef struct Control {
    engine_KeyCode key;
    Vec2f          vec;
} Control;
use_Sli$(Control);

static SliConst$Control Control_list(void) {
    static const Control controls[] = {
        { .key = engine_KeyCode_w, .vec = math_Vec2f_up },
        { .key = engine_KeyCode_a, .vec = math_Vec2f_left },
        { .key = engine_KeyCode_s, .vec = math_Vec2f_right },
        { .key = engine_KeyCode_d, .vec = math_Vec2f_down },
    };
    return (SliConst$Control){
        .ptr = controls,
        .len = countOf(controls),
    };
}

#define update_target_fps (480.0f)
#define update_target_spf (1.0f / update_target_fps)

#define state_player_speed       (1000.0f)
#define state_gravity            (1000.0f)
#define state_collision_damping  (0.8f)
#define state_objects_cap_inital (256)

Err$void dh_main(int argc, const char* argv[]) { // NOLINT
    unused(argc), unused(argv);
    Random_init();
    scope_reserveReturn(Err$void) {
        // Initialize logging to a file
        scope_if(let debug_file = fopen("subframes-debug.log", "w"), debug_file) {
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
                .backend_type  = engine_RenderBackendType_vt100,
                .window_title  = "Subframes",
                .width         = window_res_width,
                .height        = window_res_height,
                .default_color = (Color){ .packed = 0x181818FF },
            }
        ));
        defer(engine_Window_destroy(window));
        log_info("engine initialized\n");

        // Create canvases
        let game_canvas = try(engine_Canvas_create(window_res_width, window_res_height, engine_CanvasType_rgba));
        defer(engine_Canvas_destroy(game_canvas));
        log_info("canvas created\n");

        engine_Canvas_clearDefault(game_canvas);
        log_info("canvas cleared\n");

        // Add canvas views
        engine_Window_addCanvasView(window, game_canvas, 0, 0, window_res_width, window_res_height);
        log_info("canvas views added\n");

        // Create game state
        var allocator = heap_Page_allocator(&(heap_Page){});
        try(heap_Page_init(allocator));
        defer(heap_Page_fini(allocator));

        var positions = typed(Vec2fs, try(ArrList_initCap(typeInfo(Vec2f), allocator, state_objects_cap_inital)));
        defer(ArrList_fini(&positions.base));
        var velocities = typed(Vec2fs, try(ArrList_initCap(typeInfo(Vec2f), allocator, state_objects_cap_inital)));
        defer(ArrList_fini(&velocities.base));
        var colors = typed(Colors, try(ArrList_initCap(typeInfo(Color), allocator, state_objects_cap_inital)));
        defer(ArrList_fini(&colors.base));

        const f32 w      = window_res_width;
        const f32 h      = window_res_height;
        const f32 radius = 2.5f;
        log_info("game state created\n");
        ignore getchar();

        // Initialize timing variables
        let time_frame_target = time_Duration_fromSecs_f64(render_target_spf);
        var time_frame_prev   = time_Instant_now();
        log_info("game loop started\n");

        // Initialize window variables
        var prev_winpos = math_Vec_as$(Vec2f, engine_Window_getPosition(window));

        bool is_running = true;
        while (is_running) {
            // 1) Capture the start of the frame (and capture the end-of-frame time of prev iteration’s dt includes sleep)
            let time_frame_curr = time_Instant_now();

            // 2) Compute how long since last frame (purely for your dt usage)
            let time_elapsed = time_Instant_durationSince(time_frame_curr, time_frame_prev);
            let time_dt      = as$(f32, time_Duration_asSecs_f64(time_elapsed));

            // 3) Check for window movement
            let winpos  = math_Vec_as$(Vec2f, engine_Window_getPosition(window));
            let dwinpos = math_Vec2f_sub(winpos, prev_winpos);

            // 4) Process input/events
            try(engine_Window_processEvents(window));

            // 5) Update game state and Render all views
            engine_Canvas_clearDefault(game_canvas);

            if (engine_Key_pressed(engine_KeyCode_esc)) {
                is_running = false;
                log_debug("esc pressed\n");
                break;
            }

            if (engine_Mouse_pressed(engine_MouseButton_left) || engine_Key_pressed(engine_KeyCode_space)) {
                log_debug("space pressed\n");
                scope_with(let pos = meta_castPtr$(Vec2f*, try(ArrList_addBackOne(&positions.base)))) {
                    *pos = math_Vec_as$(Vec2f, engine_Mouse_getPosition());
                }

                scope_with(let vel = meta_castPtr$(Vec2f*, try(ArrList_addBackOne(&velocities.base)))) {
                    *vel = eval(
                        let angle = (math_f32_pi / 180.0f) * as$(f32, Random_range_i64(0, 360));
                        let r     = math_Vec2f_sincos(angle);
                        eval_return(math_Vec2f_scale(r, 50));
                    );
                }

                scope_with(let color = meta_castPtr$(Color*, try(ArrList_addBackOne(&colors.base)))) {
                    *color = Color_fromHslOpaque((Hsl){ .channels = { (f32)Random_range_i64(0, 360), 50.0, 80.0 } });
                }
            }

            let ps = positions.items;
            let vs = velocities.items;
            let cs = colors.items;

            for (f32 t = 0.0f; t < time_dt; t += update_target_spf) {
                for (usize i = 0; i < ps.len; ++i) {
                    const f32 f = as$(f32, t / time_dt);

                    ps.ptr[i] = math_Vec2f_sub(ps.ptr[i], math_Vec2f_scale(dwinpos, update_target_spf / time_dt));
                    vs.ptr[i].y += state_gravity * update_target_spf;

                    const f32 nx = ps.ptr[i].x + vs.ptr[i].x * update_target_spf;
                    if ((nx - radius) <= 0) {
                        ps.ptr[i].x = radius;
                        vs.ptr[i].x *= -state_collision_damping;
                        vs.ptr[i].x += dwinpos.x * update_target_spf / time_dt * 30.0f;
                    } else if (w <= (nx + radius)) {
                        ps.ptr[i].x = w - radius;
                        vs.ptr[i].x *= -state_collision_damping;
                        vs.ptr[i].x += dwinpos.x * update_target_spf / time_dt * 30.0f;
                    } else {
                        ps.ptr[i].x = nx;
                    }

                    const f32 ny = ps.ptr[i].y + vs.ptr[i].y * update_target_spf;
                    if ((ny - radius) <= 0) {
                        ps.ptr[i].y = radius;
                        vs.ptr[i].y *= -state_collision_damping;
                        vs.ptr[i].y += dwinpos.y * update_target_spf / time_dt * 30.0f;
                    } else if (h <= (ny + radius)) {
                        ps.ptr[i].y = h - radius;
                        vs.ptr[i].y *= -state_collision_damping;
                        vs.ptr[i].y += dwinpos.y * update_target_spf / time_dt * 30.0f;
                    } else {
                        ps.ptr[i].y = ny;
                    }

                    // Add circle-circle collision detection
                    for (usize j = i + 1; j < ps.len; ++j) {
                        Vec2f dist_vec = math_Vec2f_sub(ps.ptr[j], ps.ptr[i]);
                        f32   distance = math_Vec2f_len(dist_vec);

                        if (distance < radius * 2) {
                            // Normalize the distance vector
                            Vec2f normal  = math_Vec2f_scale(dist_vec, 1.0f / distance);
                            Vec2f tangent = { { -normal.y, normal.x } };

                            // Project velocities
                            f32 dp_tan_1  = math_Vec2f_dot(vs.ptr[i], tangent);
                            f32 dp_tan_2  = math_Vec2f_dot(vs.ptr[j], tangent);
                            f32 dp_norm_1 = math_Vec2f_dot(vs.ptr[i], normal);
                            f32 dp_norm_2 = math_Vec2f_dot(vs.ptr[j], normal);

                            // Calculate new velocities (assuming equal mass)
                            f32 momentum_1 = dp_norm_2;
                            f32 momentum_2 = dp_norm_1;

                            // Update velocities
                            vs.ptr[i] = math_Vec2f_add(
                                math_Vec2f_scale(tangent, dp_tan_1),
                                math_Vec2f_scale(normal, momentum_1 * state_collision_damping)
                            );
                            vs.ptr[j] = math_Vec2f_add(
                                math_Vec2f_scale(tangent, dp_tan_2),
                                math_Vec2f_scale(normal, momentum_2 * state_collision_damping)
                            );

                            // Separate the circles
                            f32   overlap    = radius * 2 - distance;
                            Vec2f separation = math_Vec2f_scale(normal, overlap * 0.5f);
                            ps.ptr[i]        = math_Vec2f_sub(ps.ptr[i], separation);
                            ps.ptr[j]        = math_Vec2f_add(ps.ptr[j], separation);
                        }
                    }

                    engine_Canvas_fillRingByScanlines(
                        game_canvas,
                        (i32)ps.ptr[i].x,
                        (i32)ps.ptr[i].y,
                        (i32)(radius * 0.8f),
                        (i32)radius,
                        eval(
                            var c = cs.ptr[i];
                            c.a *= f;
                            eval_return c;
                        )
                    );
                }
            }
            engine_Window_present(window);

            // 6) (Optional) Display instantaneous FPS
            const f64 time_fps = (0.0 < time_dt) ? (1.0 / time_dt) : 9999.0;
            printf("\033[H"); // Move cursor to top left
            printf("\rFPS: %6.2f", time_fps);
            debug_only(
                // log frame every 1s
                static f64 total_game_time_for_timestamp = 0.0;
                static f64 logging_after_duration        = 0.0;
                total_game_time_for_timestamp += time_dt;
                logging_after_duration += time_dt;
                if (1.0 < logging_after_duration) {
                    logging_after_duration = 0.0;
                    log_debug("[t=%6.2f] dt: %6.2f, fps %6.2f\n", total_game_time_for_timestamp, time_dt, 1.0 / time_dt);
                }
            );

            // 7) Measure how long the update+render actually took
            let time_now        = time_Instant_now();
            let time_frame_used = time_Instant_durationSince(time_now, time_frame_curr);

            // 8) Subtract from our target; clamp to zero if negative
            let time_leftover = time_Duration_sub(time_frame_target, time_frame_used);

            const bool is_positive_time_leftover = 0 < time_leftover.secs_ || 0 < time_leftover.nanos_;
            if (is_positive_time_leftover) {
                time_sleep(time_leftover);
            }
            time_frame_prev = time_frame_curr;
        }
        return_void();
    }
    scope_returnReserved;
}

#include "dh/main.h"
#include "dh/debug.h"
#define LOG_NO_DISABLE_RELEASE (1)
#include "dh/log.h"

#include "dh/mem.h"
#include "dh/heap/Classic.h"
#include "dh/ArrList.h"
#include "dh/defer.h"

#include "dh/time/SysTime.h"
#include "dh/time/Instant.h"
#include "dh/time/Duration.h"
#include "dh/Random.h"

#include "../engine/include/engine.h"

using_Sli$(Vec2f);
using_Sli$(Color);
typedef ArrList ArrList$Vec2f;
typedef ArrList ArrList$Color;

typedef struct Control {
    engine_KeyCode key;
    Vec2f          vec;
} Control;
using_Sli$(Control);

static SliConst$Control Control_list(void) {
    static const Control controls[] = {
        { .key = engine_KeyCode_W, .vec = math_Vec_up$(Vec2f) },
        { .key = engine_KeyCode_A, .vec = math_Vec_lt$(Vec2f) },
        { .key = engine_KeyCode_S, .vec = math_Vec_rt$(Vec2f) },
        { .key = engine_KeyCode_D, .vec = math_Vec_dn$(Vec2f) },
    };
    static const usize controls_len = sizeof(controls) / sizeof(controls[0]);
    return (SliConst$Control){
        .ptr = controls,
        .len = controls_len,
    };
}

#define PLAYER_SPEED      (1000)
#define REAL_FPS          (30)
#define TARGET_FPS        (480)
#define TARGET_DT         (1.0f / (f32)TARGET_FPS)
#define GRAVITY           (1000.0f)
#define COLLISION_DAMPING (0.8f)

Err$void dh_main(int argc, const char* argv[]) { // NOLINT
    reserveReturn(Err$void);
    unused(argc), unused(argv);
    Random_init();
    scope_defer {
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
        let window = try_defer(engine_Window_create(
            &(engine_PlatformParams){
                .backend_type = engine_RenderBackendType_vt100,
                .window_title = "Subframes",
                .width        = 160,
                .height       = 100,
            }
        ));
        defer(engine_Window_destroy(window));
        log_info("engine initialized\n");

        // Create canvases
        // let bg_canvas = try_defer(engine_Canvas_create(160, 100, engine_CanvasType_rgba));
        // defer(engine_Canvas_destroy(bg_canvas));
        let game_canvas = try_defer(engine_Canvas_create(160, 100, engine_CanvasType_rgba));
        defer(engine_Canvas_destroy(game_canvas));
        log_info("canvas created\n");

        // engine_Canvas_clear(bg_canvas, Color_black);
        engine_Canvas_clear(game_canvas, Color_black);
        log_info("canvas cleared\n");

        // Add canvas views
        // engine_Window_addCanvasView(window, bg_canvas, 0, 0, 160, 100);
        engine_Window_addCanvasView(window, game_canvas, 0, 0, 160, 100);
        log_info("canvas views added\n");

        var heap      = (heap_Classic){};
        var allocator = heap_Classic_allocator(&heap);

        var positions = (ArrList$Vec2f)try_defer(ArrList_initCap(typeInfo(Vec2f), allocator, 32));
        defer(ArrList_fini(&positions));
        var velocities = (ArrList$Vec2f)try_defer(ArrList_initCap(typeInfo(Vec2f), allocator, 32));
        defer(ArrList_fini(&velocities));
        var colors = (ArrList$Color)try_defer(ArrList_initCap(typeInfo(Color), allocator, 32));
        defer(ArrList_fini(&colors));

        const f32 w      = 160.0f;
        const f32 h      = 100.0f;
        const f32 radius = 2.5f;

        log_info("game state created\n");
        ignore getchar();

        var  curr_time   = time_Instant_now();
        var  prev_time   = curr_time;
        let  target_time = time_Duration_fromSecs_f64(REAL_FPS / 1000.0f); // Assume 30 FPS for simplicity
        bool is_running  = true;

        var prev_winpos = Vec_as$(Vec2f, engine_Window_getPosition(window));
        while (is_running) {
            let winpos  = Vec_as$(Vec2f, engine_Window_getPosition(window));
            let dwinpos = math_Vec_sub(winpos, prev_winpos);

            curr_time        = time_Instant_now();
            let elapsed_time = time_Instant_durationSince(curr_time, prev_time);
            let real_dt      = (f32)time_Duration_asSecs_f64(elapsed_time);

            try_defer(engine_Window_processEvents(window));
            engine_Canvas_clear(game_canvas, (Color){ .packed = 0xFF181818 });

            if (engine_Key_pressed(engine_KeyCode_Esc)) {
                is_running = false;
                log_debug("esc pressed\n");
                break;
            }

            if (engine_Mouse_pressed(engine_MouseButton_Left) || engine_Key_pressed(engine_KeyCode_Space)) {
                log_debug("space pressed\n");
                const Vec2f pos = Vec_as$(Vec2f, engine_Mouse_getPosition());
                try_defer(ArrList_append(&positions, (meta_Ptr){ .addr = (void*)&pos, .type = typeInfo(Vec2f) }));

                let         angle = (math_f32_pi / 180.0f) * as(f32, Random_range_i64(0, 360));
                const Vec2f dir   = eval(
                    let         r = math_Vec2_sincos$(Vec2f, angle);
                    eval_return math_Vec_scale(r, 50);
                );
                try_defer(ArrList_append(&velocities, (meta_Ptr){ .addr = (void*)&dir, .type = typeInfo(Vec2f) }));

                let color = Color_fromHslOpaque((Hsl){ .channels = { (f32)Random_range_i64(0, 360), 50.0, 80.0 } });
                try_defer(ArrList_append(&colors, (meta_Ptr){ .addr = (void*)&color, .type = typeInfo(Color) }));
            }

            let pos_list   = meta_castSli$(Sli$Vec2f, positions.items);
            let vel_list   = meta_castSli$(Sli$Vec2f, velocities.items);
            let color_list = meta_castSli$(Sli$Color, colors.items);

            for (f32 t = 0.0f; t < real_dt; t += TARGET_DT) {
                for (usize i = 0; i < pos_list.len; ++i) {
                    const f32 f = (f32)(t / real_dt);

                    pos_list.ptr[i] = math_Vec_sub(pos_list.ptr[i], math_Vec_scale(dwinpos, TARGET_DT / real_dt));
                    vel_list.ptr[i].y += GRAVITY * TARGET_DT;

                    const f32 nx = pos_list.ptr[i].x + vel_list.ptr[i].x * TARGET_DT;
                    if ((nx - radius) <= 0) {
                        pos_list.ptr[i].x = radius;
                        vel_list.ptr[i].x *= -COLLISION_DAMPING;
                        vel_list.ptr[i].x += dwinpos.x * TARGET_DT / real_dt * 30.0f;
                    } else if (w <= (nx + radius)) {
                        pos_list.ptr[i].x = w - radius;
                        vel_list.ptr[i].x *= -COLLISION_DAMPING;
                        vel_list.ptr[i].x += dwinpos.x * TARGET_DT / real_dt * 30.0f;
                    } else {
                        pos_list.ptr[i].x = nx;
                    }

                    const f32 ny = pos_list.ptr[i].y + vel_list.ptr[i].y * TARGET_DT;
                    if ((ny - radius) <= 0) {
                        pos_list.ptr[i].y = radius;
                        vel_list.ptr[i].y *= -COLLISION_DAMPING;
                        vel_list.ptr[i].y += dwinpos.y * TARGET_DT / real_dt * 30.0f;
                    } else if (h <= (ny + radius)) {
                        pos_list.ptr[i].y = h - radius;
                        vel_list.ptr[i].y *= -COLLISION_DAMPING;
                        vel_list.ptr[i].y += dwinpos.y * TARGET_DT / real_dt * 30.0f;
                    } else {
                        pos_list.ptr[i].y = ny;
                    }

                    engine_Canvas_fillRingByScanlines(
                        game_canvas,
                        (i32)pos_list.ptr[i].x,
                        (i32)pos_list.ptr[i].y,
                        (i32)(radius * 0.8f),
                        (i32)radius,
                        eval(
                            var c = color_list.ptr[i];
                            c.a *= f;
                            eval_return c;
                        )
                    );
                }
            }

            // Present to screen
            engine_Window_present(window);

            // Sleep for the remaining time to maintain FPS
            time_SysTime_sleep(time_Duration_sub(target_time, elapsed_time));
            prev_time = curr_time;

            prev_winpos = winpos;
        }
        defer_return_ok({});
    }
    return_deferred;
}

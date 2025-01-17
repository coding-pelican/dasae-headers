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

#define PLAYER_SPEED      (1000)
#define REAL_FPS          (30)
#define TARGET_FPS        (480)
#define TARGET_DT         (1.0f / (f32)TARGET_FPS)
#define GRAVITY           (1000.0f)
#define COLLISION_DAMPING (0.8f)

Err$void dh_main(int argc, const char* argv[]) { // NOLINT
    unused(argc), unused(argv);
    scope_reserveReturn(Err$void) {
        Random_init();

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
                .backend_type = engine_RenderBackendType_vt100,
                .window_title = "Subframes",
                .width        = 160,
                .height       = 100,
            }
        ));
        defer(engine_Window_destroy(window));
        log_info("engine initialized\n");

        // Create canvases
        let game_canvas = try(engine_Canvas_create(160, 100, engine_CanvasType_rgba));
        defer(engine_Canvas_destroy(game_canvas));
        log_info("canvas created\n");

        engine_Canvas_clearDefaultColor(game_canvas);
        log_info("canvas cleared\n");

        // Add canvas views
        engine_Window_addCanvasView(window, game_canvas, 0, 0, 160, 100);
        log_info("canvas views added\n");

        // var allocator = heap_Classic_allocator(&(heap_Classic){});
        var allocator = heap_Page_allocator(&(heap_Page){});
        try(heap_Page_init(allocator));
        defer(heap_Page_fini(allocator));

        var positions = typed(Vec2fs, try(ArrList_initCap(typeInfo(Vec2f), allocator, 256)));
        defer(ArrList_fini(&positions.base));
        var velocities = typed(Vec2fs, try(ArrList_initCap(typeInfo(Vec2f), allocator, 256)));
        defer(ArrList_fini(&velocities.base));
        var colors = typed(Colors, try(ArrList_initCap(typeInfo(Color), allocator, 256)));
        defer(ArrList_fini(&colors.base));

        const f32 w      = 160.0f;
        const f32 h      = 100.0f;
        const f32 radius = 2.5f;
        log_info("game state created\n");
        ignore getchar();

        var  curr_time   = time_Instant_now();
        var  prev_time   = curr_time;
        let  target_time = time_Duration_fromSecs_f64(REAL_FPS / 1000.0f); // Assume 30 FPS for simplicity
        bool is_running  = true;

        var prev_winpos = math_Vec_as$(Vec2f, engine_Window_getPosition(window));
        while (is_running) {
            let winpos  = math_Vec_as$(Vec2f, engine_Window_getPosition(window));
            let dwinpos = math_Vec2f_sub(winpos, prev_winpos);

            curr_time        = time_Instant_now();
            let elapsed_time = time_Instant_durationSince(curr_time, prev_time);
            let real_dt      = (f32)time_Duration_asSecs_f64(elapsed_time);

            try(engine_Window_processEvents(window));
            engine_Canvas_clear(game_canvas, (Color){ .packed = 0xFF181818 });

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

            for (f32 t = 0.0f; t < real_dt; t += TARGET_DT) {
                for (usize i = 0; i < ps.len; ++i) {
                    const f32 f = (f32)(t / real_dt);

                    ps.ptr[i] = math_Vec2f_sub(ps.ptr[i], math_Vec2f_scale(dwinpos, TARGET_DT / real_dt));
                    vs.ptr[i].y += GRAVITY * TARGET_DT;

                    const f32 nx = ps.ptr[i].x + vs.ptr[i].x * TARGET_DT;
                    if ((nx - radius) <= 0) {
                        ps.ptr[i].x = radius;
                        vs.ptr[i].x *= -COLLISION_DAMPING;
                        vs.ptr[i].x += dwinpos.x * TARGET_DT / real_dt * 30.0f;
                    } else if (w <= (nx + radius)) {
                        ps.ptr[i].x = w - radius;
                        vs.ptr[i].x *= -COLLISION_DAMPING;
                        vs.ptr[i].x += dwinpos.x * TARGET_DT / real_dt * 30.0f;
                    } else {
                        ps.ptr[i].x = nx;
                    }

                    const f32 ny = ps.ptr[i].y + vs.ptr[i].y * TARGET_DT;
                    if ((ny - radius) <= 0) {
                        ps.ptr[i].y = radius;
                        vs.ptr[i].y *= -COLLISION_DAMPING;
                        vs.ptr[i].y += dwinpos.y * TARGET_DT / real_dt * 30.0f;
                    } else if (h <= (ny + radius)) {
                        ps.ptr[i].y = h - radius;
                        vs.ptr[i].y *= -COLLISION_DAMPING;
                        vs.ptr[i].y += dwinpos.y * TARGET_DT / real_dt * 30.0f;
                    } else {
                        ps.ptr[i].y = ny;
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

            // Present to screen
            engine_Window_present(window);

            // Sleep for the remaining time to maintain FPS
            time_sleep(time_Duration_sub(target_time, elapsed_time));
            prev_time = curr_time;

            prev_winpos = winpos;
        }
        return_ok({});
    }
    scope_returnReserved;
}

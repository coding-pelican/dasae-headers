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
#include "engine/input.h"
#include "engine/window.h"

#define window_res_width__320x200  /* template value */ (320)
#define window_res_height__320x200 /* template value */ (200)
#define window_res_width__160x100  /* template value */ (160)
#define window_res_height__160x100 /* template value */ (100)
#define window_res_width__80x50    /* template value */ (80)
#define window_res_height__80x50   /* template value */ (50)
#define window_res_width__40x25    /* template value */ (40)
#define window_res_height__40x25   /* template value */ (25)

#if debug_comp_enabled
#define window_res_width  (window_res_width__40x25)
#define window_res_height (window_res_height__40x25)
#else /* !debug_comp_enabled */
#define window_res_width  (window_res_width__320x200)
#define window_res_height (window_res_height__320x200)
#endif /* debug_comp_enabled */
#define window_res_size (as$(usize, window_res_width) * window_res_height)

/* (1.0 / render_target_fps__62_50) ~16ms => ~60 FPS, Assume 62.5 FPS for simplicity */
#define render_target_fps__125_0 /* template value */ (125.0)
#define render_target_fps__62_50 /* template value */ (62.50)
#define render_target_fps__50_00 /* template value */ (50.00)
#define render_target_fps__31_25 /* template value */ (31.25)

#define render_target_fps (render_target_fps__31_25)
#define render_target_spf (1.0 / render_target_fps)

#define update_target_fps (480.0f)
#define update_target_spf (1.0f / update_target_fps)


Err$void dh_main(Sli$Str_const args) { // NOLINT
    $unused(args);
    scope_reserveReturn(Err$void) {
        // Initialize logging to a file
        scope_if(let debug_file = fopen("test-engine_mouse-debug.log", "w"), debug_file) {
            log_initWithFile(debug_file);
            // Configure logging behavior
            log_setLevel(log_Level_debug);
            log_showTimestamp(true);
            log_showLevel(true);
            log_showLocation(false);
            log_showFunction(true);
        }

        // Initialize platform with terminal backend
        let window = try_(engine_Window_create(
            &(engine_PlatformParams){
                .backend_type  = engine_RenderBackendType_vt100,
                .window_title  = "test-engine_mouse",
                .width         = window_res_width,
                .height        = window_res_height,
                .default_color = (Color){ .packed = 0x181818FF },
            }
        ));
        defer_(engine_Window_destroy(window));
        log_info("engine initialized\n");

        // Create canvases
        let game_canvas = try_(engine_Canvas_create(window_res_width, window_res_height, engine_CanvasType_rgba));
        defer_(engine_Canvas_destroy(game_canvas));
        log_info("canvas created\n");

        engine_Canvas_clearDefault(game_canvas);
        log_info("canvas cleared\n");

        // Add canvas views
        engine_Window_addCanvasView(window, game_canvas, 0, 0, window_res_width, window_res_height);
        log_info("canvas views added\n");

        // Create game state
        var allocator = heap_Page_allocator(&(heap_Page){});

        log_info("game state created\n");
        $ignore = getchar();

        // Initialize timing variables
        let time_frame_target = time_Duration_fromSecs_f64(render_target_spf);
        var time_frame_prev   = time_Instant_now();
        log_info("game loop started\n");

        bool is_running = true;
        while (is_running) {
            // 1) Capture the start of the frame (and capture the end-of-frame time of prev iteration’s dt includes sleep)
            let time_frame_curr = time_Instant_now();

            // 2) Compute how long since last frame (purely for your dt usage)
            let time_elapsed = time_Instant_durationSince(time_frame_curr, time_frame_prev);
            let time_dt      = as$(f32, time_Duration_asSecs_f64(time_elapsed));

            // 3) Process input/events
            try_(engine_Window_processEvents(window));
            if (engine_Key_pressed(engine_KeyCode_esc)) {
                is_running = false;
                log_debug("esc pressed\n");
                break;
            }

            // 4) Check for window movement
            let winpos      = engine_Window_getPosition(window);
            let mouse       = engine_Mouse_getPosition();
            let mouse_delta = engine_Mouse_getDelta();

            engine_Canvas_clearDefault(game_canvas);
            engine_Canvas_drawPixel(game_canvas, mouse.x, mouse.y, Color_white);
            engine_Window_present(window);

            // 6) (Optional) Display instantaneous FPS
            const f64 time_fps = (0.0 < time_dt) ? (1.0 / time_dt) : 9999.0;
            printf("\033[H\033[40;37m"); // Move cursor to top left
            printf("\rFPS: %6.2f | RES: %dx%d | POS: %.2f,%.2f\n", time_fps, window_res_width, window_res_height, winpos.x, winpos.y);
            printf("\rMOUSE: %d,%d | DELTA: %d,%d\n", mouse.x, mouse.y, mouse_delta.x, mouse_delta.y);

            // 7) Measure how long the update+render actually took
            let time_now        = time_Instant_now();
            let time_frame_used = time_Instant_durationSince(time_now, time_frame_curr);

            // 8) Subtract from our target
            if_some(time_Duration_chkdSub(time_frame_target, time_frame_used), leftover) {
                time_sleep(leftover);
            }
            time_frame_prev = time_frame_curr;
        }
        return_void();
    }
    scope_returnReserved;
}

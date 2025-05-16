#define main_no_args (1)
#include "dh/main.h"
#include "dh/log.h"

#include "dh/mem.h"
#include "dh/heap/Fixed.h"
#include "dh/heap/Arena.h"
#include "dh/ArrList.h"

#include "dh/time.h"
#include "dh/Random.h"

#include "engine.h"

#define window_res_width__320x200  /* template value */ (320)
#define window_res_height__320x200 /* template value */ (200)
#define window_res_width__160x100  /* template value */ (160)
#define window_res_height__160x100 /* template value */ (100)
#define window_res_width__80x50    /* template value */ (80)
#define window_res_height__80x50   /* template value */ (50)
#define window_res_width__40x25    /* template value */ (40)
#define window_res_height__40x25   /* template value */ (25)

#if debug_comp_enabled
#define window_res_width  (window_res_width__80x50)
#define window_res_height (window_res_height__80x50)
#else /* !debug_comp_enabled */
#define window_res_width  (window_res_width__80x50)
#define window_res_height (window_res_height__80x50)
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

#define state_player_speed       (1000.0f)
#define state_gravity            (100.0f)
#define state_collision_damping  (0.8f)
#define state_objects_cap_inital (512)

#define state_vec2f_threshold (make$(Vec2f, .x = 1e-4f, .y = 1e-4f))

/*
 * World Scale = ratio of physical screen size to world space size
 * World Space Size = Window Resolution Size (dimensions correspond to units of physical screen size)
 */

static fn_(getEngineMemory(void), heap_Fixed) {
    static var_(memory, Arr$$(1024 * 16, u8)) = Arr_zero();
    return heap_Fixed_init(Sli_arr$(Sli$u8, memory));
}
static fn_(getGameMemory(void), heap_Fixed) {
    static var_(memory, Arr$$(1024 * 16, u8)) = Arr_zero();
    return heap_Fixed_init(Sli_arr$(Sli$u8, memory));
}

fn_scope_ext(dh_main(void), Err$void) { /* NOLINT(readability-function-cognitive-complexity) */
    Random_init();
    // Initialize logging to a file
    try_(log_init("log/debug.log"));
    defer_(log_fini());
    {
        // Configure logging behavior
        log_setLevel(log_Level_debug);
        log_showTimestamp(true);
        log_showLevel(true);
        log_showLocation(false);
        log_showFunction(true);
    }

    // Initialize heap allocator and page pool
    var memory_engine    = getEngineMemory();
    var arena_engine     = heap_Arena_init(heap_Fixed_allocator(&memory_engine));
    var allocator_engine = heap_Arena_allocator(&arena_engine);
    log_info("allocator reserved for engine");

    var memory_game    = getGameMemory();
    var arena_game     = heap_Arena_init(heap_Fixed_allocator(&memory_game));
    var allocator_game = heap_Arena_allocator(&arena_game);
    log_info("allocator reserved for game");

    // Create window
    let window = try_(engine_Window_init(create$(engine_Window_Config,
        .allocator     = some(allocator_engine),
        .rect_size     = { .x = window_res_width, .y = window_res_height },
        .default_color = some({ .packed = 0x181818FF }),
        .title         = some(Str_l("Subframes")),
    )));
    defer_(engine_Window_fini(window));
    log_info("window created");

    // Create canvases
    let game_canvas = try_(engine_Canvas_init(&(engine_Canvas_Config){
        .allocator     = some(allocator_engine),
        .width         = window_res_width,
        .height        = window_res_height,
        .default_color = none(),
        .type          = some(engine_CanvasType_rgba),
    }));
    defer_(engine_Canvas_fini(game_canvas));
    {
        log_info("canvas created: %s", nameOf(game_canvas));
        engine_Canvas_clear(game_canvas, none$(Opt$Color));
        log_info("canvas cleared: %s", nameOf(game_canvas));
        engine_Window_appendView(
            window,
            &(engine_CanvasView_Config){
                .canvas      = game_canvas,
                .pos         = { .x = 0, .y = 0 },
                .size        = { .x = window_res_width, .y = window_res_height },
                .scale       = { .x = 1.0f, .y = 1.0f },
                .resizable_x = true,
                .resizable_y = true,
                .visible     = true,
            }
        );
        log_info("canvas views added: %s", nameOf(game_canvas));
    }
    let overlay_canvas = try_(engine_Canvas_init(&(engine_Canvas_Config){
        .allocator     = some(allocator_engine),
        .width         = window_res_width,
        .height        = window_res_height,
        .default_color = none$(Opt$Color),
        .type          = some(engine_CanvasType_rgba),
    }));
    defer_(engine_Canvas_fini(overlay_canvas));
    {
        log_info("canvas created: %s", nameOf(overlay_canvas));
        engine_Canvas_clear(overlay_canvas, none$(Opt$Color));
        log_info("canvas cleared: %s", nameOf(overlay_canvas));
        engine_Window_appendView(
            window,
            &(engine_CanvasView_Config){
                .canvas      = overlay_canvas,
                .pos         = { .x = 0, .y = 0 },
                .size        = { .x = window_res_width, .y = window_res_height },
                .scale       = { .x = 1.0f, .y = 1.0f },
                .resizable_x = true,
                .resizable_y = true,
                .visible     = true,
            }
        );
        log_info("canvas views added: %s", nameOf(overlay_canvas));
    }

    // Create input system
    let input = try_(engine_Input_init(allocator_engine));
    defer_(engine_Input_fini(input));

    // Bind engine core
    let core = try_(engine_core_Vt100_init(&(engine_core_Vt100_Config){
        .allocator = some(allocator_engine),
        .window    = window,
        .input     = input,
    }));
    defer_(engine_core_Vt100_fini(core));
    log_info("engine ready");

    // Create game state
    use_ArrList$(Vec2f);
    var_type(positions, ArrList$Vec2f, try_(ArrList_initCap(typeInfo$(Vec2f), allocator_game, state_objects_cap_inital)));
    defer_(ArrList_fini(positions.base));

    var_type(velocities, ArrList$Vec2f, try_(ArrList_initCap(typeInfo$(Vec2f), allocator_game, state_objects_cap_inital)));
    defer_(ArrList_fini(velocities.base));

    use_ArrList$(Color);
    var_type(colors, ArrList$Color, try_(ArrList_initCap(typeInfo$(Color), allocator_game, state_objects_cap_inital)));
    defer_(ArrList_fini(colors.base));

    const f32 radius = 2.5f;
    log_info("game state created");
    $ignore = engine_utils_getch();

    // Initialize timing variables
    let time_frame_target = time_Duration_fromSecs_f64(render_target_spf);
    var time_frame_prev   = time_Instant_now();
    log_info("game loop started");

    // Initialize window variables
    var prev_winpos = math_Vec_as$(Vec2f, engine_Window_getPos(window));

    bool is_running = true;
    while (is_running) {
        // 1) Capture the start of the frame (and capture the end-of-frame time of prev iteration’s dt includes sleep)
        let time_frame_curr = time_Instant_now();

        // 2) Compute how long since last frame (purely for your dt usage)
        let time_elapsed = time_Instant_durationSince(time_frame_curr, time_frame_prev);
        let time_dt      = as$(f32, time_Duration_asSecs_f64(time_elapsed));

        // 3) Check for window movement
        let winpos = math_Vec_as$(Vec2f, engine_Window_getPos(window));
        debug_only({
            if (math_Vec2f_neApx(winpos, prev_winpos, state_vec2f_threshold)) {
                log_info("window moved");
                log_info("old winpos: %.2f %.2f", prev_winpos.x, prev_winpos.y);
                log_info("new winpos: %.2f %.2f", winpos.x, winpos.y);
            }
        });
        let dwinpos = eval({
            // Calculate world scale (ratio of physical screen size to world space size)
            let window_dim = math_Vec_as$(math_Vec2f, engine_Window_getDim(window)); // Physical screen dimensions
            let window_res = math_Vec_as$(math_Vec2f, engine_Window_getRes(window)); // World space dimensions

            // Calculate scale factors for each dimension
            let world_scale = math_Vec2f_from(
                window_dim.x / window_res.x,
                window_dim.y / window_res.y
            );

            // Use world_scale to properly convert window position change to world space
            eval_return math_Vec2f_div(
                math_Vec2f_sub(winpos, prev_winpos),
                world_scale
            );
        });
        prev_winpos = winpos;

        // 4) Process input/events
        try_(engine_Window_update(window));

        // 5) Update game state and Render all views
        engine_Canvas_clear(game_canvas, none$(Opt$Color));

        if (engine_Keyboard_pressed(input->keyboard, engine_KeyCode_esc)) {
            is_running = false;
            log_debug("esc pressed");
        }

        with_(let_(left_space, Arr$$(2, bool)) = {
            engine_Mouse_held(input->mouse, engine_MouseButton_left),
            engine_Keyboard_held(input->keyboard, engine_KeyCode_space)
        }) {
            if (Arr_getAt(left_space, 0) || Arr_getAt(left_space, 1)) {
                debug_only(if (Arr_getAt(left_space, 0)) { log_debug("left mouse pressed"); });
                debug_only(if (Arr_getAt(left_space, 1)) { log_debug("space pressed"); });

                with_(let pos = meta_cast$(Vec2f*, try_(ArrList_addBackOne(positions.base)))) {
                    *pos = math_Vec_as$(Vec2f, engine_Mouse_getPos(input->mouse));
                }

                with_(let vel = meta_cast$(Vec2f*, try_(ArrList_addBackOne(velocities.base)))) {
                    *vel = eval({
                        let angle = (math_f32_pi / 180.0f) * as$(f32, Random_range_i64(0, 360));
                        let r     = math_Vec2f_sincos(angle);
                        eval_return(math_Vec2f_scale(r, 50.0f));
                    });
                }

                with_(let color = meta_cast$(Color*, try_(ArrList_addBackOne(colors.base)))) {
                    *color = Color_fromHslOpaque((Hsl){ .channels = { as$(f64, Random_range_i64(0, 360)), 50.0, 80.0 } });
                }
            }
        }

        let ps = positions.items;
        let vs = velocities.items;
        let cs = colors.items;

        for (f32 t = 0.0f; t < time_dt; t += update_target_spf) {
            for (usize i = 0; i < ps.len; ++i) {
                const f32 w = as$(f32, engine_Window_getRes(window).x);
                const f32 h = as$(f32, engine_Window_getRes(window).y);
                const f32 f = as$(f32, t / time_dt);

                math_Vec2f_subAsg(&ps.ptr[i], math_Vec2f_scale(dwinpos, update_target_spf / time_dt));
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
                        Vec2f tangent = math_Vec2f_perp(normal);

                        // Project velocities
                        f32 dp_tan_1  = math_Vec2f_dot(vs.ptr[i], tangent);
                        f32 dp_tan_2  = math_Vec2f_dot(vs.ptr[j], tangent);
                        f32 dp_norm_1 = math_Vec2f_dot(vs.ptr[i], normal);
                        f32 dp_norm_2 = math_Vec2f_dot(vs.ptr[j], normal);

                        // Calculate new velocities (assuming equal mass)
                        f32 momentum_1 = dp_norm_2;
                        f32 momentum_2 = dp_norm_1;

                        // Update velocities
                        vs.ptr[i] = pipe(math_Vec2f_scale(tangent, dp_tan_1),
                            (math_Vec2f_add,(math_Vec2f_scale(normal, momentum_1 * state_collision_damping)))
                        );
                        vs.ptr[j] = pipe(math_Vec2f_scale(tangent, dp_tan_2),
                            (math_Vec2f_add,(math_Vec2f_scale(normal, momentum_2 * state_collision_damping)))
                        );

                        // Separate the circles
                        f32   overlap    = radius * 2 - distance;
                        Vec2f separation = math_Vec2f_scale(normal, overlap * 0.5f);
                        math_Vec2f_subAsg(&ps.ptr[i], separation);
                        math_Vec2f_addAsg(&ps.ptr[j], separation);
                    }
                }

                engine_Canvas_fillRingByScanlines(
                    game_canvas,
                    as$(i32, ps.ptr[i].x),
                    as$(i32, ps.ptr[i].y),
                    as$(i32, radius * 0.8f),
                    as$(i32, radius),
                    eval({
                        var c = cs.ptr[i];
                        c.a   = as$(u8, (as$(f32, c.a) * f));
                        eval_return c;
                    })
                );
            }
        }
        with_(engine_Canvas_clear(overlay_canvas, none$(Opt$Color))) {
            with_(let win_res = engine_Window_getRes(window)) {
                engine_Canvas_drawRect(overlay_canvas, 0, 0, as$(i32, win_res.x) - 1, as$(i32, win_res.y) - 1, Color_white);
            }
            with_(let mouse_pos = engine_Mouse_getPos(input->mouse)) {
                engine_Canvas_drawPixel(overlay_canvas, mouse_pos.x, mouse_pos.y, Color_white);
            }
        }
        engine_Window_present(window);

        // 6) (Optional) Display instantaneous FPS
        {
            const f64 time_fps = (0.0 < time_dt) ? (1.0 / time_dt) : 9999.0;
            let       win_res  = engine_Window_getRes(window);
            printf("\033[H\033[40;37m"); // Move cursor to top left
            printf("\rFPS: %6.2f RES: %dx%d", time_fps, win_res.x, win_res.y);
            printf(" DPOS: %6.2f,%6.2f", dwinpos.x, dwinpos.y);
            debug_only({ // log frame every 1s
                static f64 total_game_time_for_timestamp = 0.0;
                static f64 logging_after_duration        = 0.0;
                total_game_time_for_timestamp += time_dt;
                logging_after_duration += time_dt;
                if (1.0 < logging_after_duration) {
                    logging_after_duration = 0.0;
                    log_debug("[t=%6.2f] dt: %6.2f, fps %6.2f", total_game_time_for_timestamp, time_dt, 1.0 / time_dt);
                }
            });
        }

        // 7) Measure how long the update+render actually took
        let time_now        = time_Instant_now();
        let time_frame_used = time_Instant_durationSince(time_now, time_frame_curr);

        // 8) Subtract from our target
        if_some(time_Duration_chkdSub(time_frame_target, time_frame_used), leftover) {
            time_sleep(leftover);
        }
        time_frame_prev = time_frame_curr;
    }
    return_ok({});
} unscoped_ext;

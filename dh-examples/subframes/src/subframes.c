#include "dh/main.h"
#include "subframes.h"

$static fn_((getEngineMemory(void))(heap_Fixed)) {
    $static var_(memory, A$$(1024 * 1024, u8)) = A_zero();
    return heap_Fixed_init(A_ref$((S$u8)memory));
};
$static fn_((getGameMemory(void))(heap_Fixed)) {
    $static var_(memory, A$$(1024 * 1024, u8)) = A_zero();
    return heap_Fixed_init(A_ref$((S$u8)memory));
};

fn_((dh_main(S$S_const$u8 args))(E$void) $guard) {
    var rng = Rand_init();
    // Initialize logging to a file
    try_(log_init(".log/debug.log"));
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
    var memory_engine = getEngineMemory();
    var arena_engine = heap_Arena_init(heap_Fixed_allocator(&memory_engine));
    defer_(heap_Arena_fini(arena_engine));
    var gpa_engine = heap_Arena_allocator(&arena_engine);
    log_info("allocator reserved for engine");

    var memory_game = getGameMemory();
    var arena_game = heap_Arena_init(heap_Fixed_allocator(&memory_game));
    defer_(heap_Arena_fini(arena_game));
    var gpa_game = heap_Arena_allocator(&arena_game);
    log_info("allocator reserved for game");

    // Create window
    let window = try_(engine_Window_init((engine_Window_Config){
        .allocator = some(gpa_engine),
        .rect_size = { .x = window_res_width, .y = window_res_height },
        .default_color = some({ .packed = 0x181818FF }),
        .title = some(u8_l("Subframes")),
    }));
    defer_(engine_Window_fini(window));
    log_info("window created");

    // Create canvases
    let game_canvas = try_(engine_Canvas_init((engine_Canvas_Config){
        .allocator = some(gpa_engine),
        .width = window_res_width,
        .height = window_res_height,
        .default_color = none(),
        .type = some(engine_CanvasType_rgba),
    }));
    defer_(engine_Canvas_fini(game_canvas));
    {
        log_info("canvas created: %s", nameOf(game_canvas));
        engine_Canvas_clear(game_canvas, none$((O$Color)));
        log_info("canvas cleared: %s", nameOf(game_canvas));
        engine_Window_appendView(
            window,
            (engine_CanvasView_Config){
                .canvas = game_canvas,
                .pos = { .x = 0, .y = 0 },
                .size = { .x = window_res_width, .y = window_res_height },
                .scale = { .x = 1.0f, .y = 1.0f },
                .resizable_x = true,
                .resizable_y = true,
                .visible = true,
            }
        );
        log_info("canvas views added: %s", nameOf(game_canvas));
    }
    let overlay_canvas = try_(engine_Canvas_init((engine_Canvas_Config){
        .allocator = some(gpa_engine),
        .width = window_res_width,
        .height = window_res_height,
        .default_color = none$((O$Color)),
        .type = some(engine_CanvasType_rgba),
    }));
    defer_(engine_Canvas_fini(overlay_canvas));
    {
        log_info("canvas created: %s", nameOf(overlay_canvas));
        engine_Canvas_clear(overlay_canvas, none$((O$Color)));
        log_info("canvas cleared: %s", nameOf(overlay_canvas));
        engine_Window_appendView(
            window,
            (engine_CanvasView_Config){
                .canvas = overlay_canvas,
                .pos = { .x = 0, .y = 0 },
                .size = { .x = window_res_width, .y = window_res_height },
                .scale = { .x = 1.0f, .y = 1.0f },
                .resizable_x = true,
                .resizable_y = true,
                .visible = true,
            }
        );
        log_info("canvas views added: %s", nameOf(overlay_canvas));
    }

    // Create input system
    let input = try_(engine_Input_init(gpa_engine));
    defer_(engine_Input_fini(input));

    // Bind engine core
    let core = try_(engine_core_VT100Ex_init($init((engine_core_VT100Ex_Config){
        .allocator = some(gpa_engine),
        .window = window,
        .input = input,
        $field((stub_path)$asg(expr_(FieldType $scope)(
            1 < args.len
                ? $break_(some(*S_at((args)[1])))
                : $break_(some(u8_l("tools/VT100Ex.exe")))
        ) $unscoped_(expr))),
        .stub_profile = none(),
    })));
    defer_(engine_core_VT100Ex_fini(core));
    log_info("engine ready");

    // Create game state
    var positions = try_(ArrList_init$m_V2f32(gpa_game, state_objects_cap_inital));
    defer_(ArrList_fini$m_V2f32(&positions, gpa_game));
    var velocities = try_(ArrList_init$m_V2f32(gpa_game, state_objects_cap_inital));
    defer_(ArrList_fini$m_V2f32(&velocities, gpa_game));
    var colors = try_(ArrList_init$Color(gpa_game, state_objects_cap_inital));
    defer_(ArrList_fini$Color(&colors, gpa_game));

    const f32 radius = 2.5f;
    log_info("game state created");
    let_ignore = engine_utils_getch();

    // Initialize timing variables
    let time_frame_target = time_Duration_fromSecs$f64(render_target_spf);
    var time_frame_prev = time_Instant_now();
    log_info("game loop started");

    // Initialize window variables
    var prev_winpos = m_V_as$(m_V2f32, engine_Window_getPos(window));

    bool is_running = true;
    while (is_running) {
        // 1) Capture the start of the frame (and capture the end-of-frame time of prev iteration’s dt includes sleep)
        let time_frame_curr = time_Instant_now();

        // 2) Compute how long since last frame (purely for your dt usage)
        let time_elapsed = time_Instant_durationSince(time_frame_curr, time_frame_prev);
        let time_dt = as$(f32)(time_Duration_asSecs$f64(time_elapsed));

        // 3) Check for window movement
        let winpos = m_V_as$(m_V2f32, engine_Window_getPos(window));
        debug_only({
            if (m_V2f32_neApx(winpos, prev_winpos, state_vec2f_threshold)) {
                log_info("window moved");
                log_info("old winpos: %.2f %.2f", prev_winpos.x, prev_winpos.y);
                log_info("new winpos: %.2f %.2f", winpos.x, winpos.y);
            }
        });
        let dwinpos = blk({
            // Calculate world scale (ratio of physical screen size to world space size)
            let window_dim = m_V_as$(m_V2f32, engine_Window_getDim(window)); // Physical screen dimensions
            let window_res = m_V_as$(m_V2f32, engine_Window_getRes(window)); // World space dimensions

            // Calculate scale factors for each dimension
            let world_scale = m_V2f32_from(
                window_dim.x / window_res.x,
                window_dim.y / window_res.y
            );

            // Use world_scale to properly convert window position change to world space
            blk_return m_V2f32_div(
                m_V2f32_sub(winpos, prev_winpos),
                world_scale
            );
        });
        prev_winpos = winpos;

        // 4) Process input/events
        try_(engine_Window_update(window));

        // 5) Update game state and Render all views
        engine_Canvas_clear(game_canvas, none$((O$Color)));

        if (engine_Keyboard_pressed(input->keyboard, engine_KeyCode_esc)) {
            is_running = false;
            log_debug("esc pressed");
        }

        let left_space = A_from$((bool){
            [0] = engine_Mouse_held(input->mouse, engine_MouseButton_left),
            [1] = engine_Keyboard_held(input->keyboard, engine_KeyCode_space),
        });
        if (*A_at((left_space)[0]) || *A_at((left_space)[1])) {
            debug_only(
                if (*A_at((left_space)[0])) { log_debug("left mouse pressed"); });
            debug_only(
                if (*A_at((left_space)[1])) { log_debug("space pressed"); });

            with_(let pos = try_(ArrList_addBack$m_V2f32(&positions, gpa_game))) {
                *pos = m_V_as$(m_V2f32, engine_Mouse_getPos(input->mouse));
            }

            with_(let vel = try_(ArrList_addBack$m_V2f32(&velocities, gpa_game))) {
                *vel = blk({
                    let angle = (math_f32_pi / 180.0f) * as$(f32)(Rand_rangeIInt(&rng, 0, 360));
                    let r = m_V2f32_sincos(angle);
                    blk_return m_V2f32_scale(r, 50.0f);
                });
            }

            with_(let color = try_(ArrList_addBack$Color(&colors, gpa_game))) {
                *color = Color_fromHslOpaque((Hsl){ .channels = { as$(f64)(Rand_rangeIInt(&rng, 0, 360)), 50.0, 80.0 } });
            }
        }

        let num_steps = as$(i32)(time_dt / update_target_spf + 0.999f);
        for_(($r(0, num_steps))(step) {
            let t = as$(f32)(step)*update_target_spf;
            if (t >= time_dt) { break; }
            for_(($rf(0), $s(positions.items), $s(velocities.items), $s(colors.items))(i, pos, vel, color) {
                let w = as$(f32)(engine_Window_getRes(window).x);
                let h = as$(f32)(engine_Window_getRes(window).y);
                let f = as$(f32)(t / time_dt);

                m_V2f32_subAsg(pos, m_V2f32_scale(dwinpos, update_target_spf / time_dt));
                vel->y += state_gravity * update_target_spf;

                let nx = pos->x + vel->x * update_target_spf;
                if ((nx - radius) <= 0) {
                    pos->x = radius;
                    vel->x *= -state_collision_damping;
                    vel->x += dwinpos.x * update_target_spf / time_dt * 30.0f;
                } else if (w <= (nx + radius)) {
                    pos->x = w - radius;
                    vel->x *= -state_collision_damping;
                    vel->x += dwinpos.x * update_target_spf / time_dt * 30.0f;
                } else {
                    pos->x = nx;
                }

                let ny = pos->y + vel->y * update_target_spf;
                if ((ny - radius) <= 0) {
                    pos->y = radius;
                    vel->y *= -state_collision_damping;
                    vel->y += dwinpos.y * update_target_spf / time_dt * 30.0f;
                } else if (h <= (ny + radius)) {
                    pos->y = h - radius;
                    vel->y *= -state_collision_damping;
                    vel->y += dwinpos.y * update_target_spf / time_dt * 30.0f;
                } else {
                    pos->y = ny;
                }

                // Add circle-circle collision detection
                for_(($s(suffix$S(positions.items, i + 1)), $s(suffix$S(velocities.items, i + 1)))(other_pos, other_vel) {
                    let dist_vec = m_V2f32_sub(*other_pos, *pos);
                    let distance = m_V2f32_len(dist_vec);

                    if (distance < radius * 2) {
                        // Normalize the distance vector
                        let normal = m_V2f32_scale(dist_vec, 1.0f / distance);
                        let tangent = m_V2f32_perp(normal);

                        // Project velocities
                        let dp_tan_1 = m_V2f32_dot(*vel, tangent);
                        let dp_tan_2 = m_V2f32_dot(*other_vel, tangent);
                        let dp_norm_1 = m_V2f32_dot(*vel, normal);
                        let dp_norm_2 = m_V2f32_dot(*other_vel, normal);

                        // Calculate new velocities (assuming equal mass)
                        let momentum_1 = dp_norm_2;
                        let momentum_2 = dp_norm_1;

                        // Update velocities
                        *vel = pipe_(m_V2f32_scale(tangent, dp_tan_1), (m_V2f32_add, (m_V2f32_scale(normal, momentum_1 * state_collision_damping))));
                        *other_vel = pipe_(m_V2f32_scale(tangent, dp_tan_2), (m_V2f32_add, (m_V2f32_scale(normal, momentum_2 * state_collision_damping))));

                        // Separate the circles
                        let overlap = radius * 2 - distance;
                        let separation = m_V2f32_scale(normal, overlap * 0.5f);
                        m_V2f32_subAsg(pos, separation);
                        m_V2f32_addAsg(other_pos, separation);
                    }
                });

                engine_Canvas_fillRingByScanlines(
                    game_canvas,
                    as$(i32)(pos->x), as$(i32)(pos->y),
                    as$(i32)(radius * 0.8f),as$(i32)(radius),
                    ({
                        var c = *color;
                        c.a = as$(u8)(as$(f32)((c.a) * f));
                        blk_return c;
                    })
                );
            });
        });
        with_(engine_Canvas_clear(overlay_canvas, none$((O$Color)))) {
            with_(let win_res = engine_Window_getRes(window)) {
                engine_Canvas_drawRect(overlay_canvas, 0, 0, as$(i32)(win_res.x) - 1, as$(i32)(win_res.y) - 1, Color_white);
            }
            with_(let mouse_pos = engine_Mouse_getPos(input->mouse)) {
                engine_Canvas_drawPixel(overlay_canvas, mouse_pos.x, mouse_pos.y, Color_white);
            }
        }
        engine_Window_present(window);

        // 6) (Optional) Display instantaneous FPS
        {
            const f64 time_fps = (0.0 < time_dt) ? (1.0 / time_dt) : 9999.0;
            let win_res = engine_Window_getRes(window);
            io_stream_print(u8_l("\033[H\033[40;37m")); // Move cursor to top left
            io_stream_print(
                u8_l("\rFPS: {:.2f} RES: {:u}x{:u} DPOS: {:.2f},{:.2f}"),
                time_fps, win_res.x, win_res.y, dwinpos.x, dwinpos.y
            );
            debug_only({ // log frame every 1s
                static f64 total_game_time_for_timestamp = 0.0;
                static f64 logging_after_duration = 0.0;
                total_game_time_for_timestamp += time_dt;
                logging_after_duration += time_dt;
                if (1.0 < logging_after_duration) {
                    logging_after_duration = 0.0;
                    log_debug("[t=%6.2f] dt: %6.2f, fps %6.2f", total_game_time_for_timestamp, time_dt, 1.0 / time_dt);
                }
            });
        }

        // 7) Measure how long the update+render actually took
        let time_now = time_Instant_now();
        let time_frame_used = time_Instant_durationSince(time_now, time_frame_curr);

        // 8) Subtract from our target
        if_some((time_Duration_subChkd(time_frame_target, time_frame_used))(leftover)) {
            debug_only(log_debug("sleeping for %6.2f seconds", time_Duration_asSecs$f64(leftover)));
            pp_if_(state_enable_fps_limits)(
                pp_then_(time_sleep(leftover)),
                pp_else_($unused(leftover))
            );
        }
        time_frame_prev = time_frame_curr;
    }
    return_ok({});
} $unguarded_(fn);

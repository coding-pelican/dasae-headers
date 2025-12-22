#include "barnes-hut.h"
#include "engine.h"

// #define log_comp_disabled_not_debug_comp_enabled (1)
#include "dh/main.h"
#include "dh/log.h"

#include "dh/mem/Allocator.h"
#include "dh/heap/Page.h"
#include "dh/ArrList.h"

#include "dh/time/common.h"
#include "dh/time/Duration.h"
#include "dh/Thrd/common.h"
#include "dh/Thrd/Mtx.h"

#if debug_comp_enabled
#define main_window_res_width (window_res_width__80x50)
#define main_window_res_height (window_res_height__80x50)
#else /* !debug_comp_enabled */
#define main_window_res_width (window_res_width__320x160 - 20)
#define main_window_res_height (window_res_height__320x160 + 40)
#endif /* debug_comp_enabled */
#define main_window_res_size (as$(usize, main_window_res_width) * main_window_res_height)

#define main_target_fps (target_fps__31_250)
#define main_target_spf (1.0 / main_target_fps)

#define main_simulation_n_body (100000)

// Global state without thread synchronization
static struct {
    O$mem_Allocator allocator;
    ArrList$Body spawn_bodies;
    Thrd_Mtx sim_mutex;
    Simulation* sim;
    bool paused;
    bool is_running;
} global_state = {
    .allocator = none(),
    .spawn_bodies = cleared(),
    .sim_mutex = cleared(),
    .paused = false,
    .is_running = false
};
#if debug_comp_enabled
static fn_(global_debug_logSimStateFrontBodiesN(usize n), void);
#endif /* debug_comp_enabled */
static fn_(global_processInput(Visualizer* viz, engine_Window* wnd, engine_Input* input), E$void) $must_check;
static fn_(global_update(Visualizer* viz, Simulation* sim), E$void) $must_check;
static fn_(global_renderStatInfo(Visualizer* viz, Simulation* sim, f64 dt), void);
static Thrd_fn_(Simulation_thread, ({}, E$void));

// Main function
fn_(dh_main(S$S_const$u8 args), E$void $guard) {
    let_ignore = args;

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

    // Create allocator
    var allocator = heap_Page_allocator(&(heap_Page){});
    O_asg(&global_state.allocator, some(allocator));

    // Create window
    let window = try_(engine_Window_init(&(engine_Window_Config){
        .allocator = some(allocator),
        .rect_size = {
            .x = main_window_res_width,
            .y = main_window_res_height,
        },
        .default_color = some(Color_black),
        .title = some(u8_l("Barnes-hut N-Body Simulation")) }));
    defer_(engine_Window_fini(window));
    log_info("engine initialized\n");

    let canvas = try_(engine_Canvas_init(&(engine_Canvas_Config){
        .allocator = some(allocator),
        .width = main_window_res_width,
        .height = main_window_res_height,
        .default_color = none(),
        .type = some(engine_CanvasType_rgba),
    }));
    defer_(engine_Canvas_fini(canvas));
    {
        log_info("canvas created: %s", nameOf(canvas));
        engine_Canvas_clear(canvas, none$(O$Color));
        log_info("canvas cleared: %s", nameOf(canvas));
        engine_Window_appendView(
            window,
            &(engine_CanvasView_Config){
                .canvas = canvas,
                .pos = { .x = 0, .y = 0 },
                .size = { .x = main_window_res_width, .y = main_window_res_height },
                .scale = { .x = 1.0f, .y = 1.0f },
                .resizable_x = true,
                .resizable_y = true,
                .visible = true,
            }
        );
        log_info("canvas views added: %s", nameOf(canvas));
    }

    // Create input system
    let input = try_(engine_Input_init(allocator));
    defer_(engine_Input_fini(input));

    // Bind engine core
    let core = try_(engine_core_VT100_init(&(engine_core_VT100_Config){
        .allocator = some(allocator),
        .window = window,
        .input = input,
    }));
    defer_(engine_core_VT100_fini(core));
    log_info("engine ready");

    // Initialize state
    var_type(spawn_bodies, ArrList$Body, try_(ArrList_initCap(typeInfo$(Body), allocator, main_simulation_n_body)));
    defer_(ArrList_fini(spawn_bodies.base));
    global_state.spawn_bodies = spawn_bodies;

    // Create simulation and Visualizer
    var sim = try_(Simulation_create(allocator, main_simulation_n_body));
    defer_(Simulation_destroy(&sim));
    global_state.sim = &sim;
    log_info("simulation created\n");

    var viz = try_(Visualizer_create(allocator, canvas));
    defer_(Visualizer_destroy(&viz));
    log_info("visualizer created\n");

    let_ignore = engine_utils_getch();

    // Create threads for simulation and visualization
    var sim_mutex = Thrd_Mtx_init();
    defer_(Thrd_Mtx_fini(&sim_mutex));
    global_state.sim_mutex = sim_mutex;
    var sim_thread = try_(Thrd_spawn(Thrd_SpawnCfg_default, Thrd_FnCtx_from(Simulation_thread, {}).base));
    defer_(catch_from(
        Thrd_FnCtx_returned(Simulation_thread, Thrd_join(sim_thread)), err,
        eval({ log_error("Thrd_join with failed: %s", Err_codeToCStr(err)); })
    ));
    global_state.paused = false;
    global_state.is_running = true;
    log_info("threads created\n");

    // Initialize timing variables
    let time_frame_target = time_Duration_fromSecs$f64(main_target_spf);
    var time_frame_prev = time_Instant_now();
    log_info("main loop started\n");

    // Main loop
    while (global_state.is_running) {
        let time_frame_curr = time_Instant_now();
        let time_elapsed = time_Instant_durationSince(time_frame_curr, time_frame_prev);
        let time_dt = time_Duration_asSecs$f64(time_elapsed);

        // 1) Update engine
        try_(engine_Window_update(window));

        // 2) Update simulator
        try_(global_processInput(&viz, window, input));
        try_(global_update(&viz, &sim));

        // 3) Render
        try_(Visualizer_render(&viz));
        engine_Window_present(window);

        // 4) FPS timing
        global_renderStatInfo(&viz, &sim, time_dt);
        debug_only(
            // log frame every 1s
            static f64 total_game_time_for_timestamp = 0.0;
            static f64 logging_after_duration = 0.0;
            total_game_time_for_timestamp += time_dt;
            logging_after_duration += time_dt;
            if (1.0 < logging_after_duration) {
                logging_after_duration = 0.0;
                log_debug("[t=%6.2f] dt: %6.2f, fps %6.2f\n", total_game_time_for_timestamp, time_dt, 1.0 / time_dt);
            });

        // 5) sleep
        let time_now = time_Instant_now();
        let time_frame_used = time_Instant_durationSince(time_now, time_frame_curr);
        if_some(time_Duration_chkdSub(time_frame_target, time_frame_used), time_leftover) {
            time_sleep(time_leftover);
        }
        time_frame_prev = time_frame_curr;
    }
    return_ok({});
}
$unguarded;

#if debug_comp_enabled
fn_(global_debug_logSimStateFrontBodiesN(usize n), void) {
    log_info("SimState:\n");
    log_info("  bodies: %d\n", global_state.sim->bodies.items.len);
    for (usize i = 0; i < prim_min(n, global_state.sim->bodies.items.len); ++i) {
        let body = Sli_at(global_state.sim->bodies.items, i);
        // clang-format off
        log_info("    pos=(%.2f,%.2f) vel=(%.2f,%.2f) acc(%.2f,%.2f) mess=(%.2f)",
            body->pos.x, body->pos.y,
            body->vel.x, body->vel.y,
            body->acc.x, body->acc.y,
            body->mass
        );
        // clang-format on
    }
}
#endif /* debug_comp_enabled */

fn_(global_processInput(Visualizer* viz, engine_Window* wnd, engine_Input* input), E$void $scope) {
    debug_assert_nonnull(viz);
    debug_assert_nonnull(wnd);
    debug_assert_nonnull(input);

    if (!engine_Window_isFocused(wnd)) { return_ok({}); }

    let keyboard = input->keyboard;
    if (engine_Keyboard_pressed(keyboard, engine_KeyCode_esc)) {
        log_debug("esc pressed\n");
        Thrd_Mtx_lock(&global_state.sim_mutex);
        global_state.is_running = false;
        Thrd_Mtx_unlock(&global_state.sim_mutex);
    }
    if (engine_Keyboard_pressed(keyboard, engine_KeyCode_space)) {
        log_debug("space pressed\n");
        Thrd_Mtx_lock(&global_state.sim_mutex);
        global_state.paused = !global_state.paused;
        Thrd_Mtx_unlock(&global_state.sim_mutex);
    }
#if debug_comp_enabled
    if (engine_Keyboard_pressed(keyboard, engine_KeyCode_i)) {
        log_debug("i pressed\n");
        Thrd_Mtx_lock(&global_state.sim_mutex);
        global_debug_logSimStateFrontBodiesN(10);
        Thrd_Mtx_unlock(&global_state.sim_mutex);
    }
#endif /* debug_comp_enabled */

    try_(Visualizer_processInput(viz, wnd, input));
    return_ok({});
}
$unscoped;

fn_(global_update(Visualizer* viz, Simulation* sim), E$void $scope) {
    debug_assert_nonnull(viz);
    debug_assert_nonnull(sim);

    // Transfer confirmed spawns from Visualizer to global_state
    if_some(O_asPtr(&viz->spawn.confirmed), confirmed) {
        try_(ArrList_append(global_state.spawn_bodies.base, meta_refPtr(confirmed)));
        O_asg(&viz->spawn.confirmed, none());
    }

    // Add spawned bodies to simulation
    for_slice(global_state.spawn_bodies.items, body) {
        try_(ArrList_append(sim->bodies.base, meta_refPtr(body)));
    }
    ArrList_clearRetainingCap(global_state.spawn_bodies.base);

    // Update Visualizer's bodies and nodes from simulation
    ArrList_clearRetainingCap(viz->bodies.base);
    try_(ArrList_appendSli(viz->bodies.base, meta_refSli(sim->bodies.items)));

    ArrList_clearRetainingCap(viz->nodes.base);
    try_(ArrList_appendSli(viz->nodes.base, meta_refSli(sim->quadtree.nodes.items)));

    try_(Visualizer_update(viz));
    return_ok({});
}
$unscoped;

fn_(global_renderStatInfo(Visualizer* viz, Simulation* sim, f64 dt), void) {
    printf("\033[H\033[40;37m"); // Move cursor to top left

    const f64 time_fps = (0.0 < dt) ? (1.0 / dt) : 9999.0;
    {
        printf(
            "\rVIZ> FPS:%5.2f | RES:%dx%d | SCALE:%.2f | POS:%.2f,%.2f\n",
            time_fps,
            main_window_res_width,
            main_window_res_height,
            viz->scale,
            viz->pos.x,
            viz->pos.y
        );
    }

    Thrd_Mtx_lock(&global_state.sim_mutex);
    let bodies_len = sim->bodies.items.len;
#if debug_comp_enabled || Simulation_comp_enabled_record_collision_count
    let collision_count = sim->collision_count;
#endif /* debug_comp_enabled || Simulation_comp_enabled_record_collision_count */
    let rects_len = sim->rects.items.len;
    let quadtree_nodes_len = sim->quadtree.nodes.items.len;
    let quadtree_parents_len = sim->quadtree.parents.items.len;
    Thrd_Mtx_unlock(&global_state.sim_mutex);
    {
        printf(
            "\rSIM> N-BODY:%llu"
#if debug_comp_enabled || Simulation_comp_enabled_record_collision_count
            " | COLLI:%5llu"
#endif /* debug_comp_enabled || Simulation_comp_enabled_record_collision_count */
            " | RECTS:%llu\n",
            bodies_len,
#if debug_comp_enabled || Simulation_comp_enabled_record_collision_count
            collision_count,
#endif /* debug_comp_enabled || Simulation_comp_enabled_record_collision_count */
            rects_len
        );
        printf(
            "\rQUADTREE> NODES: %llu | PARENTS:%llu\n",
            quadtree_nodes_len,
            quadtree_parents_len
        );
    }

    printf("\033[0m"); // Reset color
}

$must_check
Thrd_fn_(Simulation_thread, ($ignore, $ignore)$guard) {
    // Initialize timing variables
    let time_update_target = time_Duration_fromSecs$f64(0.001); // 1ms
    log_info("sim loop started\n");

    while (true) {
        // Check if we should stop
        Thrd_Mtx_lock(&global_state.sim_mutex);
        let running = global_state.is_running;
        let paused = global_state.paused;
        Thrd_Mtx_unlock(&global_state.sim_mutex);
        if (!running) { break; } // exit thread

        // If paused, just sleep or do nothing
        if (paused) {
            // Sleep a bit to avoid busy loop
            // Or you can do pthread_cond_wait on some condition
            // log_info("sim_thread paused\n");
            time_sleep(time_Duration_mul_u64(time_update_target, 10)); // 10ms
            continue;
        }

        // Otherwise, do one simulation step
        // (which includes building quadtree, attract, collide, etc.)
        block_defer {
            Thrd_Mtx_lock(&global_state.sim_mutex);
            defer_(Thrd_Mtx_unlock(&global_state.sim_mutex));
            if (global_state.sim) {
                // It's safe to call Simulation_step now
                // Provided Simulation_step modifies only sim's data (and possibly spawn_bodies)
                // In your code, you might also need to lock spawn_bodies
                // if it is shared with other threads.
                // log_info("sim_thread update\n");
                catch_from(Simulation_step(global_state.sim), err, eval({
                               log_error("Simulation_step failed: %s", Err_codeToCStr(err));
                               return_err(err);
                           }));
            }
        }
        block_deferral;

        // Optional small sleep to limit sim speed
        // or you can use a time-based approach
        time_sleep(time_update_target); // 1ms
    }

    return_ok({});
}
$unguarded_Thrd_fn;

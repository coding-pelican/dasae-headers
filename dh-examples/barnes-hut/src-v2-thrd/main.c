#include <pthread.h>
#include <sched.h> // for sched_yield()

// #define log_comp_disabled_not_debug_comp_enabled (1)
#include "dh/main.h"
#include "dh/debug.h"
#include "dh/log.h"

#include "dh/core.h"
#include "dh/mem/Allocator.h"
#include "dh/heap/Classic.h"
#include "dh/heap/Page.h"
#include "dh/ArrList.h"

#include "dh/time/Duration.h"
#include "dh/time/common.h"
#include "engine.h"
#include "engine/window.h"

#include "Simulation.h"
#include "Visualizer.h"
#include "utils.h"

#include "cfg_values.h"

#if debug_comp_enabled
#define main_window_res_width  (window_res_width__80x50)
#define main_window_res_height (window_res_height__80x50)
#else /* !debug_comp_enabled */
#define main_window_res_width  (window_res_width__320x160 - 20)
#define main_window_res_height (window_res_height__320x160 + 40)
#endif /* debug_comp_enabled */
#define main_window_res_size (as$(usize, main_window_res_width) * main_window_res_height)

#define main_target_fps (target_fps__31_250)
#define main_target_spf (1.0 / main_target_fps)

#define main_simulation_n_body (100000)


// Global state without thread synchronization
static struct {
    ArrList$Body    spawn_bodies;
    mem_Allocator   allocator;
    Simulation*     sim;
    pthread_mutex_t sim_mutex;
    Visualizer*     viz;
    bool            paused;
    bool            is_running;
} global_state = {
    .spawn_bodies = cleared(),
    .allocator    = cleared(),
    .sim          = null,
    .sim_mutex    = PTHREAD_MUTEX_INITIALIZER,
    .viz          = null,
    .paused       = false,
    .is_running   = false
};
#if debug_comp_enabled
static void global_debug_logSimStateFrontBodiesN(usize n);
#endif /* debug_comp_enabled */
static Err$void global_processInput(Visualizer* viz, engine_Window* window) must_check;
static Err$void global_update(Visualizer* viz, Simulation* sim) must_check;
static void     global_renderStatInfo(Visualizer* viz, Simulation* sim, f64 dt);

// Thread functions
static anyptr Simulation_thread(anyptr arg);

// Main function
Err$void dh_main(int argc, const char* argv[]) {
    unused(argc), unused(argv);
    scope_reserveReturn(Err$void) {
        // Initialize logging to a file
        scope_if(let debug_file = fopen("debug.log", "w"), debug_file) {
            log_initWithFile(debug_file);
            // Configure logging behavior
            log_setLevel(log_Level_info);
            log_showTimestamp(true);
            log_showLevel(true);
            log_showLocation(false);
            log_showFunction(true);
        }
        defer(log_fini());

        // Create window
        var window = try(engine_Window_create(&(engine_PlatformParams){
            .backend_type  = engine_RenderBackendType_vt100,
            .window_title  = "Barnes-hut N-Body Simulation",
            .width         = main_window_res_width,
            .height        = main_window_res_height,
            .default_color = Color_black,
        }));
        defer(engine_Window_destroy(window));
        log_info("engine initialized\n");

        var canvas = try(engine_Canvas_createWithDefault(main_window_res_width, main_window_res_height, engine_CanvasType_rgba, Color_transparent));
        defer(engine_Canvas_destroy(canvas));
        log_info("canvas created\n");

        engine_Window_addCanvasView(window, canvas, 0, 0, main_window_res_width, main_window_res_height);
        log_info("canvas views added\n");

        // Create allocator
        // var allocator = heap_Classic_allocator(&(heap_Classic){});
        var allocator = heap_Page_allocator(&(heap_Page){});
        try(heap_Classic_init(allocator));
        defer(heap_Classic_fini(allocator));
        global_state.allocator = allocator;

        // Initialize state
        var spawn_bodies = type$(ArrList$Body, try(ArrList_initCap(typeInfo$(Body), allocator, main_simulation_n_body)));
        defer(ArrList_fini(&global_state.spawn_bodies.base));
        global_state.spawn_bodies = spawn_bodies;

        // Create simulation and Visualizer
        var sim = try(Simulation_create(allocator, main_simulation_n_body));
        defer(Simulation_destroy(&sim));
        global_state.sim = &sim;
        log_info("simulation created\n");

        var viz = try(Visualizer_create(allocator, canvas));
        defer(Visualizer_destroy(&viz));
        global_state.viz = &viz;
        log_info("visualizer created\n");

        ignore getchar();

        // Create threads for simulation and visualization
        pthread_t sim_thread = 0;
        // Reserve destroy mutexes and cond vars
        defer(pthread_mutex_destroy(&global_state.sim_mutex));
        global_state.is_running = true;

        // Create threads
        pthread_create(&sim_thread, null, Simulation_thread, null);
        // Reserve wait for threads to finish
        defer(pthread_join(sim_thread, null));
        log_info("threads created\n");

        // Initialize timing variables
        let time_frame_target = time_Duration_fromSecs_f64(main_target_spf);
        var time_frame_prev   = time_Instant_now();
        log_info("main loop started\n");

        // Main loop
        while (global_state.is_running) {
            let time_frame_curr = time_Instant_now();
            let time_elapsed    = time_Instant_durationSince(time_frame_curr, time_frame_prev);
            let time_dt         = time_Duration_asSecs_f64(time_elapsed);

            // 1) Process input
            try(engine_Window_processEvents(window));
            try(global_processInput(&viz, window));

            // 2) Update
            try(global_update(&viz, &sim));

            // 3) Render
            try(Visualizer_render(global_state.viz));
            engine_Window_present(window);

            // 4) FPS timing
            global_renderStatInfo(&viz, &sim, time_dt);
            debug_only( // log frame every 1s
                static f64 total_game_time_for_timestamp = 0.0;
                static f64 logging_after_duration        = 0.0;
                total_game_time_for_timestamp += time_dt;
                logging_after_duration += time_dt;
                if (1.0 < logging_after_duration) {
                    logging_after_duration = 0.0;
                    log_debug("[t=%6.2f] dt: %6.2f, fps %6.2f\n", total_game_time_for_timestamp, time_dt, 1.0 / time_dt);
                }
            );

            // 5) sleep
            let time_now        = time_Instant_now();
            let time_frame_used = time_Instant_durationSince(time_now, time_frame_curr);
            if_some(time_Duration_subChecked(time_frame_target, time_frame_used), time_leftover) {
                time_sleep(time_leftover);
            }
            time_frame_prev = time_frame_curr;
        }
        return_void();
    }
    scope_returnReserved;
}

#if debug_comp_enabled
static void global_debug_logSimStateFrontBodiesN(usize n) {
    log_info("SimState:\n");
    log_info("  bodies: %d\n", global_state.sim->bodies.items.len);
    for (usize i = 0; i < prim_min(n, global_state.sim->bodies.items.len); ++i) {
        let body = Sli_at(global_state.sim->bodies.items, i);
        log_info("    pos=(%.2f,%.2f) vel=(%.2f,%.2f) acc(%.2f,%.2f) mess=(%.2f)", body->pos.x, body->pos.y, body->vel.x, body->vel.y, body->acc.x, body->acc.y, body->mass);
    }
}
#endif

static Err$void global_processInput(Visualizer* viz, engine_Window* window) {
    reserveReturn(Err$void);
    debug_assert_nonnull(viz);
    debug_assert_nonnull(window);

    if (!engine_Window_isFocused(window)) { return_void(); }

    if (engine_Key_pressed(engine_KeyCode_esc)) {
        log_debug("esc pressed\n");
        pthread_mutex_lock(&global_state.sim_mutex);
        global_state.is_running = false;
        pthread_mutex_unlock(&global_state.sim_mutex);
    }
    if (engine_Key_pressed(engine_KeyCode_space)) {
        log_debug("space pressed\n");
        pthread_mutex_lock(&global_state.sim_mutex);
        global_state.paused = !global_state.paused;
        pthread_mutex_unlock(&global_state.sim_mutex);
    }
#if debug_comp_enabled
    if (engine_Key_pressed(engine_KeyCode_i)) {
        log_debug("i pressed\n");
        pthread_mutex_lock(&global_state.sim_mutex);
        global_debug_logSimStateFrontBodiesN(10);
        pthread_mutex_unlock(&global_state.sim_mutex);
    }
#endif /* debug_comp_enabled */

    try(Visualizer_processInput(viz, window));
    return_void();
}

static Err$void must_check global_update(Visualizer* viz, Simulation* sim) {
    scope_reserveReturn(Err$void) {
        debug_assert_nonnull(viz);
        debug_assert_nonnull(sim);

        // Transfer confirmed spawns from Visualizer to global_state
        if_some_mut(viz->spawn.confirmed, confirmed_body) {
            try(ArrList_append(&global_state.spawn_bodies.base, meta_refPtr(confirmed_body)));
            assignNone(viz->spawn.confirmed);
        }

        // Add spawned bodies to simulation
        for_slice(global_state.spawn_bodies.items, body) {
            try(ArrList_append(&sim->bodies.base, meta_refPtr(body)));
        }
        ArrList_clearRetainingCap(&global_state.spawn_bodies.base);

        // Update Visualizer's bodies and nodes from simulation
        ArrList_clearRetainingCap(&viz->bodies.base);
        try(ArrList_appendSlice(&viz->bodies.base, meta_refSli(sim->bodies.items)));

        ArrList_clearRetainingCap(&viz->nodes.base);
        try(ArrList_appendSlice(&viz->nodes.base, meta_refSli(sim->quadtree.nodes.items)));

        try(Visualizer_update(viz));
        return_void();
    }
    scope_returnReserved;
}

static void global_renderStatInfo(Visualizer* viz, Simulation* sim, f64 dt) {
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

    pthread_mutex_lock(&global_state.sim_mutex);
    let bodies_len = sim->bodies.items.len;
#if debug_comp_enabled || Simulation_comp_enabled_record_collision_count
    let collision_count = sim->collision_count;
#endif /* debug_comp_enabled || Simulation_comp_enabled_record_collision_count */
    let rects_len            = sim->rects.items.len;
    let quadtree_nodes_len   = sim->quadtree.nodes.items.len;
    let quadtree_parents_len = sim->quadtree.parents.items.len;
    pthread_mutex_unlock(&global_state.sim_mutex);
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

static anyptr Simulation_thread(anyptr arg) {
    unused(arg);

    // Initialize timing variables
    let time_update_target = time_Duration_fromSecs_f64(0.001); // 1ms
    log_info("sim loop started\n");

    while (true) {
        // Check if we should stop
        pthread_mutex_lock(&global_state.sim_mutex);
        let running = global_state.is_running;
        let paused  = global_state.paused;
        pthread_mutex_unlock(&global_state.sim_mutex);
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
        pthread_mutex_lock(&global_state.sim_mutex);
        if (global_state.sim) {
            // It's safe to call Simulation_step now
            // Provided Simulation_step modifies only sim's data (and possibly spawn_bodies)
            // In your code, you might also need to lock spawn_bodies
            // if it is shared with other threads.
            // log_info("sim_thread update\n");
            catch (Simulation_step(global_state.sim), err, {
                log_error("Simulation_step failed: %s", Err_codeToCStr(err));
                pthread_mutex_unlock(&global_state.sim_mutex);
                break;
            });
        }
        pthread_mutex_unlock(&global_state.sim_mutex);

        // Optional small sleep to limit sim speed
        // or you can use a time-based approach
        time_sleep(time_update_target); // 1ms
        // sched_yield();
    }

    return null;
}

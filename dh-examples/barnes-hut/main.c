#include "dh/main.h"
#include "dh/debug.h"
#include "dh/log.h"

#include "dh/core.h"
#include "dh/mem/Allocator.h"
#include "dh/heap/Classic.h"
#include "dh/heap/Page.h"
#include "dh/ArrList.h"

#include "engine.h"

#include "engine/window.h"
#include "src/Simulation.h"
#include "src/Visualizer.h"
#include "src/utils.h"
#include "src/cfg_values.h"

#define window_res_width  (window_res_width__320x160 - 20)
#define window_res_height (window_res_height__320x160 + 40)
#define window_res_size   (as$(usize, window_res_width) * window_res_height)

#define target_fps (target_fps__31_250)
#define target_spf (1.0 / target_fps)

#define n_body (25000)

// Global state without thread synchronization
static struct {
    ArrList$Body  spawn_bodies;
    mem_Allocator allocator;
    Visualizer*   viz;
    Simulation*   sim;
    bool          paused;
    bool          is_running;
} global_state = { 0 };

static Err$void global_processInput(Visualizer* viz, engine_Window* window) {
    reserveReturn(Err$void);
    debug_assert_nonnull(viz);
    debug_assert_nonnull(window);

    if (!engine_Window_isFocused(window)) { return_void(); }

    if (engine_Key_pressed(engine_KeyCode_esc)) {
        log_debug("esc pressed\n");
        global_state.is_running = false;
    }
    if (engine_Key_pressed(engine_KeyCode_space)) {
        log_debug("space pressed\n");
        global_state.paused = !global_state.paused;
    }

    try(Visualizer_processInput(viz, window));
    return_void();
}

Err$void global_update(Visualizer* viz, Simulation* sim) {
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

        // Simulation step if not paused
        if (!global_state.paused) {
            try(Simulation_step(sim));
        }

        // Update Visualizer's bodies and nodes from simulation
        ArrList_clearRetainingCap(&viz->bodies.base);
        try(ArrList_appendSlice(&viz->bodies.base, meta_refSli(sim->bodies.items)));

        ArrList_clearRetainingCap(&viz->nodes.base);
        try(ArrList_appendSlice(&viz->nodes.base, meta_refSli(sim->quad_tree.nodes.items)));

        try(Visualizer_update(viz));
        return_void();
    }
    scope_returnReserved;
}

Err$void dh_main(int argc, const char* argv[]) {
    unused(argc), unused(argv);
    scope_reserveReturn(Err$void) {
        // Initialize logging to a file
        scope_if(let debug_file = fopen("main-debug.log", "w"), debug_file) {
            log_initWithFile(debug_file);
            // Configure logging behavior
            log_setLevel(log_Level_debug);
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
            .width         = window_res_width,
            .height        = window_res_height,
            .default_color = Color_black,
        }));
        defer(engine_Window_destroy(window));
        log_info("engine initialized\n");

        var canvas = try(engine_Canvas_createWithDefault(window_res_width, window_res_height, engine_CanvasType_rgba, Color_transparent));
        defer(engine_Canvas_destroy(canvas));
        log_info("canvas created\n");

        engine_Window_addCanvasView(window, canvas, 0, 0, window_res_width, window_res_height);
        log_info("canvas views added\n");

        // Create allocator
        // var allocator = heap_Classic_allocator(&(heap_Classic){});
        var allocator = heap_Page_allocator(&(heap_Page){});
        try(heap_Classic_init(allocator));
        defer(heap_Classic_fini(allocator));
        global_state.allocator = allocator;

        // Initialize state
        global_state.is_running   = true;
        global_state.paused       = false;
        global_state.spawn_bodies = typed(ArrList$Body, try(ArrList_initCap(typeInfo(Body), allocator, n_body)));
        defer(ArrList_fini(&global_state.spawn_bodies.base));

        // Create simulation and Visualizer
        var sim = try(Simulation_create(allocator, n_body));
        defer(Simulation_destroy(&sim));
        global_state.sim = &sim;
        log_info("simulation created\n");

        var viz = try(Visualizer_create(allocator, canvas));
        defer(Visualizer_destroy(&viz));
        global_state.viz = &viz;
        log_info("visualizer created\n");

        ignore getchar();

        // Initialize timing variables
        let time_frame_target = time_Duration_fromSecs_f64(target_spf);
        var time_frame_prev   = time_Instant_now();
        log_info("main loop started\n");

        // Main loop
        while (global_state.is_running) {
            let time_frame_curr = time_Instant_now();
            let time_elapsed    = time_Instant_durationSince(time_frame_curr, time_frame_prev);
            let time_dt         = time_Duration_asSecs_f64(time_elapsed);

            // Process input
            try(engine_Window_processEvents(window));
            try(global_processInput(&viz, window));

            // Update simulation
            try(global_update(&viz, &sim));

            // Render frame
            try(Visualizer_render(&viz));
            engine_Window_present(window);

            const f64 time_fps = (0.0 < time_dt) ? (1.0 / time_dt) : 9999.0;
            printf("\033[H\033[40;37m"); // Move cursor to top left
#if Simulation_allows_record_collision_count
            printf(
                "\rFPS: %6.2f | RES: %dx%d | SCALE: %.2f | POS: %.2f,%.2f | N-BODY: %d | COLLI: %llu",
                time_fps,
                window_res_width,
                window_res_height,
                viz.scale,
                viz.pos.x,
                viz.pos.y,
                n_body,
                sim.collision_count
            );
#else
            printf(
                "\rFPS: %6.2f | RES: %dx%d | SCALE: %.2f | POS: %.2f,%.2f | N-BODY: %d",
                time_fps,
                window_res_width,
                window_res_height,
                viz.scale,
                viz.pos.x,
                viz.pos.y,
                n_body
            );
#endif
            printf("\033[0m"); // Reset color

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

#if DEPRECATED_CODE
static void global_debug_printSimulationState(void) {
    log_info("Global State:\n");
    log_info("  paused: %s\n", global_state.paused ? "true" : "false");
    log_info("  bodies: %d\n", global_state.spawn_bodies.items.len);
    for_slice(global_state.spawn_bodies.items, body) {
        log_info("    pos=(%.2f,%.2f) vel=(%.2f,%.2f) mess=(%.2f)", body->pos.x, body->pos.y, body->vel.x, body->vel.y, body->mass);
    }
}
#endif // DEPRECATED_CODE

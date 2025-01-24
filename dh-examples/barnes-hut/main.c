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

/* 16:10 */
#define window_res_width__1920x1200  /* template value */ (1920)
#define window_res_height__1920x1200 /* template value */ (1200)
#define window_res_width__1600x1000  /* template value */ (1600)
#define window_res_height__1600x1000 /* template value */ (1000)
#define window_res_width__1280x800   /* template value */ (1280)
#define window_res_height__1280x800  /* template value */ (800)
#define window_res_width__960x600    /* template value */ (960)
#define window_res_height__960x600   /* template value */ (600)
#define window_res_width__800x500    /* template value */ (800)
#define window_res_height__800x500   /* template value */ (500)
#define window_res_width__640x400    /* template value */ (640)
#define window_res_height__640x400   /* template value */ (400)
#define window_res_width__480x300    /* template value */ (480)
#define window_res_height__480x300   /* template value */ (300)
#define window_res_width__320x200    /* template value */ (320)
#define window_res_height__320x200   /* template value */ (200)
#define window_res_width__160x100    /* template value */ (160)
#define window_res_height__160x100   /* template value */ (100)
#define window_res_width__80x50      /* template value */ (80)
#define window_res_height__80x50     /* template value */ (50)
#define window_res_width__40x25      /* template value */ (40)
#define window_res_height__40x25     /* template value */ (25)
/* 16:9 */
#define window_res_width__1920x1080  /* template value */ (1920)
#define window_res_height__1920x1080 /* template value */ (1080)
#define window_res_width__1600x900   /* template value */ (1600)
#define window_res_height__1600x900  /* template value */ (900)
#define window_res_width__1280x720   /* template value */ (1280)
#define window_res_height__1280x720  /* template value */ (720)
#define window_res_width__960x540    /* template value */ (960)
#define window_res_height__960x540   /* template value */ (540)
#define window_res_width__800x450    /* template value */ (800)
#define window_res_height__800x450   /* template value */ (450)
#define window_res_width__640x360    /* template value */ (640)
#define window_res_height__640x360   /* template value */ (360)
#define window_res_width__480x270    /* template value */ (480)
#define window_res_height__480x270   /* template value */ (270)
#define window_res_width__320x180    /* template value */ (320)
#define window_res_height__320x180   /* template value */ (180)
#define window_res_width__160x90     /* template value */ (160)
#define window_res_height__160x90    /* template value */ (90)
#define window_res_width__80x45      /* template value */ (80)
#define window_res_height__80x45     /* template value */ (45)
#define window_res_width__40x22      /* template value */ (40)
#define window_res_height__40x22     /* template value */ (22)
/* 2:1 */
#define window_res_width__1920x960   /* template value */ (1920)
#define window_res_height__1920x960  /* template value */ (960)
#define window_res_width__1600x800   /* template value */ (1600)
#define window_res_height__1600x800  /* template value */ (800)
#define window_res_width__1280x640   /* template value */ (1280)
#define window_res_height__1280x640  /* template value */ (640)
#define window_res_width__960x480    /* template value */ (960)
#define window_res_height__960x480   /* template value */ (480)
#define window_res_width__800x400    /* template value */ (800)
#define window_res_height__800x400   /* template value */ (400)
#define window_res_width__640x320    /* template value */ (640)
#define window_res_height__640x320   /* template value */ (320)
#define window_res_width__480x240    /* template value */ (480)
#define window_res_height__480x240   /* template value */ (240)
#define window_res_width__320x160    /* template value */ (320)
#define window_res_height__320x160   /* template value */ (160)
#define window_res_width__160x80     /* template value */ (160)
#define window_res_height__160x80    /* template value */ (80)
#define window_res_width__80x40      /* template value */ (80)
#define window_res_height__80x40     /* template value */ (40)
#define window_res_width__40x20      /* template value */ (40)
#define window_res_height__40x20     /* template value */ (20)

#define window_res_width  (window_res_width__320x160)
#define window_res_height (window_res_height__320x160)
#define window_res_size   (as$(usize, window_res_width) * window_res_height)

/* (1.0 / target_fps__62_50) ~16ms => ~60 FPS, Assume 62.5 FPS for simplicity */
#define target_fps__125_0 /* template value */ (125.00)
#define target_fps__100_0 /* template value */ (100.00)
#define target_fps__62_50 /* template value */ (62.50)
#define target_fps__50_00 /* template value */ (50.00)
#define target_fps__31_25 /* template value */ (31.25)

#define target_fps (target_fps__62_50)
#define target_spf (1.0 / target_fps)

#define n_body (5000)

// Global state without thread synchronization
static struct {
    bool          is_running;
    bool          paused;
    Visualizer*   viz;
    Simulation*   sim;
    ArrList$Body  spawn_bodies;
    mem_Allocator allocator;
} global_state = { 0 };

#if DEBUG_ENABLED
static void global_debug_printSimulationState(void) {
    log_info("Global State:\n");
    log_info("  paused: %s\n", global_state.paused ? "true" : "false");
    log_info("  bodies: %d\n", global_state.spawn_bodies.items.len);
    for_slice(global_state.spawn_bodies.items, body) {
        log_info("    pos=(%.2f,%.2f) vel=(%.2f,%.2f) mess=(%.2f)", body->pos.x, body->pos.y, body->vel.x, body->vel.y, body->mass);
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
        global_state.is_running = false;
    }
    if (engine_Key_pressed(engine_KeyCode_space)) {
        log_debug("space pressed\n");
        global_state.paused = !global_state.paused;
    }
#if DEBUG_ENABLED
    if (engine_Key_pressed(engine_KeyCode_i)) {
        log_debug("i pressed\n");
        global_debug_printSimulationState();
    }
#endif
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
            viz->spawn.confirmed = (TypeOf(viz->spawn.confirmed))none();
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
            .backend_type = engine_RenderBackendType_vt100,
            .window_title = "Barnes-hut N-Body Simulation",
            .width        = window_res_width,
            .height       = window_res_height,
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
        var sim          = try(Simulation_create(allocator, n_body));
        global_state.sim = &sim;
        defer(Simulation_destroy(&sim));
        log_info("simulation created\n");

        var viz          = try(Visualizer_create(allocator, canvas));
        global_state.viz = &viz;
        defer(Visualizer_destroy(&viz));
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
            printf(
                "\rFPS: %6.2f | RES: %dx%d | SCALE: %.2f | POS: %.2f,%.2f",
                time_fps,
                window_res_width,
                window_res_height,
                viz.scale,
                viz.pos.x,
                viz.pos.y
            );
            printf("\033[0m"); // Reset color
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

            let time_now        = time_Instant_now();
            let time_frame_used = time_Instant_durationSince(time_now, time_frame_curr);
            let time_leftover   = time_Duration_sub(time_frame_target, time_frame_used);

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

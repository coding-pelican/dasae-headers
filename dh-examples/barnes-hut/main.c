/* TODO: Implement thrd */
/* TODO: Implement thrd_mtx */
/* TODO: Implement atomic */
/* TODO: Implement coroutine */

#include <pthread.h>
#include <stdatomic.h>

#include "Simulation.h"
#include "State.h"
#include "utils.h"

#include "dh/core.h"
#include "dh/mem/Tracker.h"

#include "engine/window.h"
#include "engine/input.h"

// Shared state between simulation and render threads
static struct {
    pthread_mutex_t mutex;
    atomic_bool     paused;
    atomic_bool     update_ready;

    ArrList$Body     spawn_bodies;
    ArrList$Body     bodies;
    ArrList$QuadNode nodes;

    mem_Allocator allocator;
} g_shared_state = { 0 };

// Simulation thread function
static int simulation_thread_func(void* arg) {
    unused(arg);

    // Create simulation
    var simulation = try_ptr(Simulation_create(g_shared_state.allocator));
    defer(Simulation_destroy(simulation));

    while (true) {
        if (atomic_load(&g_shared_state.paused)) {
            pthread_;
            continue;
        }

        // Simulation step
        try(Simulation_step(simulation));

        // Update shared state
        mtx_lock(&g_shared_state.mutex);
        {
            // Handle spawned bodies
            for_array(g_shared_state.spawn_bodies.items, body) {
                try(ArrList_append(&simulation->bodies.base, meta_refPtr(body)));
            }
            ArrList_clearAndFree(&g_shared_state.spawn_bodies.base);

            // Update body and quadtree arrays
            ArrList_clearAndFree(&g_shared_state.bodies.base);
            try(ArrList_appendSlice(&g_shared_state.bodies.base, meta_refSli(simulation->bodies.items)));

            ArrList_clearAndFree(&g_shared_state.nodes.base);
            try(ArrList_appendSlice(&g_shared_state.nodes.base, meta_refSli(simulation->quad_tree.nodes.items)));

            atomic_store(&g_shared_state.update_ready, true);
        }
        mtx_unlock(&g_shared_state.mutex);
    }

    return thrd_success;
}

static void process_input(State* State, engine_Window* window) {
    if (engine_Key_pressed(engine_KeyCode_space)) {
        bool expected = atomic_load(&g_shared_state.paused);
        atomic_store(&g_shared_state.paused, !expected);
    }

    State_processInput(State, window);
}

static Err$void update(State* State) {
    reserveReturn(Err$void);

    // Check for simulation update
    if (atomic_load(&g_shared_state.update_ready)) {
        mtx_lock(&g_shared_state.mutex);
        {
            ArrList_clearAndFree(&State->bodies.base);
            try(ArrList_appendSlice(&State->bodies.base, meta_refSli(g_shared_state.bodies.items)));

            ArrList_clearAndFree(&State->nodes.base);
            try(ArrList_appendSlice(&State->nodes.base, meta_refSli(g_shared_state.nodes.items)));

            atomic_store(&g_shared_state.update_ready, false);
        }
        mtx_unlock(&g_shared_state.mutex);
    }

    State_update(State);
    return_void();
}

Err$void dh_main(int argc, const char* argv[]) {
    reserveReturn(Err$void);
    unused(argc), unused(argv);

    // Create allocator for simulation
    var allocator            = mem_Heap_allocator();
    g_shared_state.allocator = allocator;

    // Initialize shared state
    if (mtx_init(&g_shared_state.mutex, mtx_plain) != thrd_success) {
        claim_unreachable();
    }
    atomic_init(&g_shared_state.paused, false);
    atomic_init(&g_shared_state.update_ready, false);

    g_shared_state.spawn_bodies = typed(ArrList$Body, ArrList_init(typeInfo(Body), allocator));
    g_shared_state.bodies       = typed(ArrList$Body, ArrList_init(typeInfo(Body), allocator));
    g_shared_state.nodes        = typed(ArrList$QuadNode, ArrList_init(typeInfo(QuadNode), allocator));

    // Create window
    var window = try(engine_Window_create(&(engine_PlatformParams){
        .window_title  = "N-Body Simulation",
        .window_width  = 800,
        .window_height = 600,
    }));
    defer(engine_Window_destroy(window));

    // Create State
    var State = try(State_create(allocator));
    defer(State_destroy(&State));

    // Start simulation thread
    thrd_t sim_thread;
    if (thrd_create(&sim_thread, simulation_thread_func, null) != thrd_success) {
        claim_unreachable();
    }

    // Main loop
    while (true) {
        // Process window events
        try(engine_Window_processEvents(window));
        if (!engine_Window_isFocused(window)) {
            thrd_yield();
            continue;
        }

        // Handle input
        process_input(&State, window);

        // Update and render
        try(update(&State));
        try(State_render(&State));

        // Present frame
        engine_Window_present(window);
    }

    // Cleanup
    int thread_result;
    thrd_join(sim_thread, &thread_result);
    mtx_destroy(&g_shared_state.mutex);
    ArrList_fini(&g_shared_state.spawn_bodies.base);
    ArrList_fini(&g_shared_state.bodies.base);
    ArrList_fini(&g_shared_state.nodes.base);

    return_void();
}

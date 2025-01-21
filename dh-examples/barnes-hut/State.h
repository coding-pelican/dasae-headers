#ifndef STATE_INCLUDED
#define STATE_INCLUDED (1)

#include "dh/core.h"
#include "dh/meta.h"
#include "dh/mem/Allocator.h"
#include "dh/math.h"
#include "Body.h"
#include "QuadTree.h"

// Engine includes
#include "engine/window.h"
#include "engine/input.h"
#include "engine/canvas.h"

typedef struct State {
    // View state
    math_Vec2f pos;
    f32        scale;

    // Display flags
    bool settings_window_open;
    bool show_bodies;
    bool show_quad_tree;

    // Quadtree visualization
    struct {
        usize min_depth;
        usize max_depth;
    } depth_range;

    // Body spawn state
    struct {
        Body* body;      // Current spawn body
        f32*  angle;     // Current spawn angle
        f32*  total;     // Total angle rotated
        Body* confirmed; // Confirmed spawn body
    } spawn;

    // Simulation state copies
    ArrList$Body     bodies;
    ArrList$QuadNode nodes;

    // Platform resources
    engine_Canvas* canvas;
    mem_Allocator  allocator;
} State;
use_Err$(State);

// Core functions
extern Err$State State_create(mem_Allocator allocator) must_check;
extern void      State_destroy(State* self);

// Main loop functions
extern Err$void State_processInput(State* self, engine_Window* window) must_check;
extern Err$void State_update(State* self) must_check;
extern Err$void State_render(State* self) must_check;

#endif /* STATE_INCLUDED */

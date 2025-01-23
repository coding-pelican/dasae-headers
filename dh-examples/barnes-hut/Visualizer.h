#ifndef VISUALIZER_INCLUDED
#define VISUALIZER_INCLUDED (1)

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

typedef struct Visualizer {
    // View state
    math_Vec2f pos;
    f32        scale;

    // Display flags
    // bool settings_window_open;
    // bool shows_fps;
    // bool shows_view_state;
    bool shows_bodies;         // 1
    bool shows_bodies_vel_vec; // 2
    bool shows_bodies_acc_vec; // 3
    bool shows_quad_tree;      // 0

    // body visualization
    Color body_color;

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
} Visualizer;
use_Err$(Visualizer);

// Core functions
extern Err$Visualizer Visualizer_create(mem_Allocator allocator, engine_Canvas* canvas) must_check;
extern void           Visualizer_destroy(Visualizer* self);

// Main loop functions
extern Err$void Visualizer_processInput(Visualizer* self, engine_Window* window) must_check;
extern Err$void Visualizer_update(Visualizer* self) must_check;
extern Err$void Visualizer_render(Visualizer* self) must_check;

#endif /* VISUALIZER_INCLUDED */

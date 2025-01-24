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

use_ArrList$(u8);
typedef struct Visualizer {
    // View state
    math_Vec2f pos;
    f32        scale;

    // Display flags
    // bool settings_window_open;
    // bool shows_fps;
    // bool shows_view_state;
    bool shows_bodies;         // toggle '1'
    bool shows_bodies_vel_vec; // toggle '2'
    bool shows_bodies_acc_vec; // toggle '3'
    bool shows_quad_tree;      // toggle 'q'

    // Quadtree visualization
    union {
        usize pair[2];
        struct {
            usize min;
            usize max;
        };
        struct {
            usize first;
            usize second;
        };
    } depth_range; // Quadtree visualization render cache
    struct {
        union {
            ArrList$usize pair[2];
            struct {
                ArrList$usize nodes;
                ArrList$usize depths;
            };
            struct {
                ArrList$usize first;
                ArrList$usize second;
            };
        };
        usize len;
    } stack; // Quadtree visualization render cache

    // Body spawn state
    struct {
        Opt$Body body;      // Current spawn body
        Opt$f32  angle;     // Current spawn angle
        Opt$f32  total;     // Total angle rotated
        Opt$Body confirmed; // Confirmed spawn body
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

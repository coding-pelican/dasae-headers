#ifndef VISUALIZER_INCLUDED
#define VISUALIZER_INCLUDED (1)

#include "dh/core.h"
#include "dh/meta.h"
#include "dh/mem/Allocator.h"
#include "dh/math.h"
#include "Body.h"
#include "QuadTree.h"

// Engine includes
#include "engine/Window.h"
#include "engine/Input.h"
#include "engine/Canvas.h"

typedef struct Visualizer_QuadCache {
    usize node_idx;
    usize depth;
} Visualizer_QuadCache;
use_Sli$(Visualizer_QuadCache);
use_ArrList$(Visualizer_QuadCache);

use_ArrList$(u8);
typedef struct Visualizer {
    // View state
    math_Vec2f pos;   // Camera center in world coords
    f32        scale; // Current zoom factor

    // For panning:
    Vec2i pan_screen_begin; // Mouse's screen-pixel coords when middle-click started
    Vec2f pan_cam_begin;    // 'pos' captured at that same moment
    bool  is_panning;       // is the mouse currently being dragged with middle-click?

    // For zoom anchoring:
    // (Optional if only want to store the 'mouse world' anchor for the duration
    //  of a single scroll step, it can be stored in a local variable,
    // but fields are handy if want more complex behaviour)
    Vec2f zoom_anchor_world;

    // Display flags
    // bool shows_fps;
    // bool shows_view_state;
    bool shows_bodies;         // toggle '1'
    bool shows_bodies_vel_vec; // toggle '2'
    bool shows_bodies_acc_vec; // toggle '3'
    bool shows_quad_tree;      // toggle 'q'

    // QuadTree visualization
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
    } depth_range;                            // QuadTree visualization render cache
    ArrList$Visualizer_QuadCache cache_stack; // QuadTree visualization render cache

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
extern fn_(Visualizer_create(mem_Allocator allocator, engine_Canvas* canvas), Err$Visualizer) $must_check;
extern fn_(Visualizer_destroy(Visualizer* self), void);

// Main loop functions
extern fn_(Visualizer_processInput(Visualizer* self, engine_Window* window, engine_Input* input), Err$void) $must_check;
extern fn_(Visualizer_update(Visualizer* self), Err$void) $must_check;
extern fn_(Visualizer_render(Visualizer* self), Err$void) $must_check;

/* /// Unified coordinate system transformations
/// Screen Coordinates          World Coordinates
/// (0,0)  --------- (w-1,0)    (-w/2*scale, +h/2*scale)
///    |               |                ↑ y+
///    |               |                |
///    |    (CENTER)   |                +--→ x+
///    |               |
/// (0,h-1) -------- (w-1,h-1)  (-w/2*scale, -h/2*scale)
$inline_always Vec2i Visualizer_screenCenter(Visualizer* self) {
    // Calculate screen center offsets (handles even/odd dimensions)
    let center_x = as$(i32, (self->canvas->width - (~self->canvas->width & 1)) >> 1);
    let center_y = as$(i32, (self->canvas->height - (~self->canvas->height & 1)) >> 1);
    return math_Vec2i_from(center_x, center_y);
}
/// Unified coordinate system transformations
/// screen_px == (world_px - screen_center_px) * scale
/// screen_py == (screen_center_py - world_py) * scale
$inline_always Vec2i Visualizer_worldToScreen(Visualizer* self, Vec2f world_pos) {
    let p_sub_center        = math_Vec2f_sub(world_pos, self->pos);
    let p_sub_center_scaled = math_Vec2f_scale(p_sub_center, Visualizer_scale(self));
    return math_Vec2i_from(
        as$(i32, roundf(p_sub_center_scaled.x)),
        as$(i32, roundf(p_sub_center_scaled.y))
    );
}
/// Unified coordinate system transformations
/// world_px == screen_center_px + (screen_px * 1/scale)
/// world_py == screen_center_py - (screen_py * 1/scale)
$inline_always Vec2f Visualizer_screenToWorld(Visualizer* self, Vec2i screen_pos) {
    let p = math_Vec2f_from(
        as$(f32, screen_pos.x),
        as$(f32, -screen_pos.y)
    );
    let p_inv_scaled = math_Vec2f_scale(p, Visualizer_scaleInv(self));
    return math_Vec2f_add(self->pos, p_inv_scaled);
} */

#endif /* VISUALIZER_INCLUDED */

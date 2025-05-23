#ifndef SIMULATION_INCLUDED
#define SIMULATION_INCLUDED (1)

#if !defined(Simulation_comp_enabled_record_collision_count)
#define Simulation_comp_enabled_record_collision_count (1)
#endif /* !defined(Simulation_comp_enabled_record_collision_count) */

#include "dh/core.h"
#include "dh/meta.h"
#include "dh/mem/Allocator.h"
#include "dh/Arr.h"
#include "dh/ArrList.h"
#include "QuadTree.h"
#include "Body.h"

typedef struct Rect {
    math_Vec2f min;
    math_Vec2f max;
} Rect;
use_Sli$(Rect);
use_Arr$(2, Rect);
use_ArrList$(Rect);

decl_ArrList$(ArrList$Body);
typedef struct Simulation {
    f32          dt;
    usize        frame;
    ArrList$Body bodies;
    ArrList$Rect rects; // Cache for collision rects
    QuadTree     quadtree;
    Sli$usize    sort_body_indices_cache;         // Cache for sorting bodies indices
    Sli$u8       sort_rect_indices_cache_as_temp; // Cache for sorting bodies rects
#if debug_comp_enabled || Simulation_comp_enabled_record_collision_count
    usize collision_count;
#endif /* debug_comp_enabled || Simulation_comp_enabled_record_collision_count */
    mem_Allocator allocator;
} Simulation;
use_Err$(Simulation);

// Core functions
extern fn_(Simulation_create(mem_Allocator allocator, usize n), Err$Simulation) $must_check;
extern fn_(Simulation_destroy(Simulation* self), void);

// Step functions
extern fn_(Simulation_step(Simulation* self), Err$void) $must_check;
extern fn_(Simulation_iterate(Simulation* self), void);
extern fn_(Simulation_collide(Simulation* self), Err$void) $must_check;
extern fn_(Simulation_attract(Simulation* self), Err$void) $must_check;
extern fn_(Simulation_resolve(Simulation* self, usize lhs, usize rhs), void);

#endif /* SIMULATION_INCLUDED */

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
    m_V2f32 min;
    m_V2f32 max;
} Rect;
use_S$(Rect);
use_A$(2, Rect);
use_ArrList$(Rect);

decl_ArrList$(ArrList$Body);
typedef struct Simulation {
    f32          dt;
    usize        frame;
    ArrList$Body bodies;
    ArrList$Rect rects; // Cache for collision rects
    QuadTree     quadtree;
    S$usize      sort_body_indices_cache;         // Cache for sorting bodies indices
    S$u8         sort_rect_indices_cache_as_temp; // Cache for sorting bodies rects
#if debug_comp_enabled || Simulation_comp_enabled_record_collision_count
    usize collision_count;
#endif /* debug_comp_enabled || Simulation_comp_enabled_record_collision_count */
    mem_Allocator allocator;
} Simulation;
use_E$(Simulation);

// Core functions
extern E$Simulation Simulation_create(mem_Allocator allocator, usize n) $must_check;
extern void         Simulation_destroy(Simulation* self);

// Step functions
extern E$void Simulation_step(Simulation* self) $must_check;
extern void   Simulation_iterate(Simulation* self);
extern E$void Simulation_collide(Simulation* self) $must_check;
extern E$void Simulation_attract(Simulation* self) $must_check;
extern void   Simulation_resolve(Simulation* self, usize lhs, usize rhs);

#endif /* SIMULATION_INCLUDED */

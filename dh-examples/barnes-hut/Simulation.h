#ifndef SIMULATION_INCLUDED
#define SIMULATION_INCLUDED (1)

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
    f32           dt;
    usize         frame;
    ArrList$Body  bodies;
    ArrList$Rect  rects; // Cache for collision rects
    QuadTree      quad_tree;
    mem_Allocator allocator;
} Simulation;
use_Err$(Simulation);

// Core functions
extern Err$Simulation Simulation_create(mem_Allocator allocator, usize n) must_check;
extern void           Simulation_destroy(Simulation* self);

// Step functions
extern Err$void Simulation_step(Simulation* self) must_check;
extern Err$void Simulation_attract(Simulation* self) must_check;
extern void     Simulation_iterate(Simulation* self);
extern Err$void Simulation_collide(Simulation* self) must_check;

// Private functions (declared for testing)
extern void Simulation_resolve(Simulation* self, usize lhs, usize rhs);

#endif /* SIMULATION_INCLUDED */

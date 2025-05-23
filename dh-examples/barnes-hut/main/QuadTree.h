#ifndef QUAD_TREE_INCLUDED
#define QUAD_TREE_INCLUDED (1)

#include "dh/core.h"
#include "dh/meta.h"
#include "dh/mem/Allocator.h"
#include "dh/Arr.h"
#include "dh/ArrList.h"
#include "dh/Range.h"
#include "Body.h"

typedef struct Quad {
    math_Vec2f center;
    f32        size;
} Quad;
use_Arr$(4, Quad);
use_Sli$(Quad);
use_ArrList$(Quad);

typedef struct QuadNode {
    usize      children;
    usize      next;
    math_Vec2f pos;
    f32        mass;
    Quad       quad;
    Range      bodies;
} QuadNode;
use_Sli$(QuadNode);
use_ArrList$(QuadNode);

use_ArrList$(usize);
typedef struct QuadTree {
    f32              theta_sq;
    f32              eps_sq;
    usize            leaf_cap;
    ArrList$QuadNode nodes;
    ArrList$usize    parents;
    mem_Allocator    allocator;
} QuadTree;
use_Err$(QuadTree);

// Quad functions
extern fn_(Quad_newContaining(const Sli$Body bodies), Quad);
extern fn_(Quad_intoQuadrant(Quad self, usize quadrant), Quad);
extern fn_(Quad_subdivide(const Quad* self), Arr$4$Quad);

// QuadNode functions
extern fn_(QuadNode_new(usize next, Quad quad, Range bodies), QuadNode);
extern fn_(QuadNode_isLeaf(const QuadNode* self), bool);
extern fn_(QuadNode_isBranch(const QuadNode* self), bool);
extern fn_(QuadNode_isEmpty(const QuadNode* self), bool);

// QuadTree functions
extern fn_(QuadTree_create(mem_Allocator allocator, f32 theta, f32 epsilon, usize leaf_cap, usize n), Err$QuadTree) $must_check;
extern fn_(QuadTree_destroy(QuadTree* self), void);
extern fn_(QuadTree_clear(QuadTree* self), void);
extern fn_(QuadTree_build(QuadTree* self, Sli$Body bodies), Err$void) $must_check;
extern fn_(QuadTree_propagate(QuadTree* self), void);
extern fn_(QuadTree_accelerate(const QuadTree* self, math_Vec2f pos, Sli$Body bodies), math_Vec2f);

static const usize QuadTree_root = 0;

#endif /* QUAD_TREE_INCLUDED */

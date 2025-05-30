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
extern Quad       Quad_newContaining(const Sli$Body bodies);
extern Quad       Quad_intoQuadrant(Quad self, usize quadrant);
extern Arr$4$Quad Quad_subdivide(const Quad* self);

// QuadNode functions
extern QuadNode QuadNode_new(usize next, Quad quad, Range bodies);
extern bool     QuadNode_isLeaf(const QuadNode* self);
extern bool     QuadNode_isBranch(const QuadNode* self);
extern bool     QuadNode_isEmpty(const QuadNode* self);

// QuadTree functions
extern Err$QuadTree QuadTree_create(mem_Allocator allocator, f32 theta, f32 epsilon, usize leaf_cap, usize n) $must_check;
extern void         QuadTree_destroy(QuadTree* self);
extern void         QuadTree_clear(QuadTree* self);
extern Err$void     QuadTree_build(QuadTree* self, Sli$Body bodies) $must_check;
extern void         QuadTree_propagate(QuadTree* self);
extern math_Vec2f   QuadTree_accelerate(const QuadTree* self, math_Vec2f pos, Sli$Body bodies);

static const usize QuadTree_root = 0;

#endif /* QUAD_TREE_INCLUDED */

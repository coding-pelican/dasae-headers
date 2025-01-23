#ifndef QUAD_TREE_INCLUDED
#define QUAD_TREE_INCLUDED (1)

#include "dh/core.h"
#include "dh/meta.h"
#include "dh/mem/Allocator.h"
#include "dh/Arr.h"
#include "dh/ArrList.h"
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
} QuadNode;
use_Sli$(QuadNode);
use_ArrList$(QuadNode);

use_ArrList$(usize);
typedef struct QuadTree {
    f32              theta_squared;
    f32              epsilon_squared;
    ArrList$QuadNode nodes;
    ArrList$usize    parents;
    mem_Allocator    allocator;
} QuadTree;
use_Err$(QuadTree);

// Quad functions
extern Quad       Quad_newContaining(const Sli$Body bodies);
extern usize      Quad_findQuadrant(const Quad* self, math_Vec2f pos);
extern Quad       Quad_intoQuadrant(Quad self, usize quadrant);
extern Arr$4$Quad Quad_subdivide(const Quad* self);

// QuadNode functions
extern QuadNode QuadNode_new(usize next, Quad quad);
extern bool     QuadNode_isLeaf(const QuadNode* self);
extern bool     QuadNode_isBranch(const QuadNode* self);
extern bool     QuadNode_isEmpty(const QuadNode* self);

// QuadTree functions
Err$QuadTree QuadTree_create(mem_Allocator allocator, f32 theta, f32 epsilon) must_check;
Err$void     QuadTree_clear(QuadTree* self, Quad quad);
Err$void     QuadTree_insert(QuadTree* self, math_Vec2f pos, f32 mass) must_check;
void         QuadTree_propagate(QuadTree* self);
math_Vec2f   QuadTree_calculateAcceleration(const QuadTree* self, math_Vec2f pos);
void         QuadTree_destroy(QuadTree* self);

static const usize QuadTree_s_root = 0;

#endif /* QUAD_TREE_INCLUDED */

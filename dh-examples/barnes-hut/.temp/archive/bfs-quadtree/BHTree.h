#ifndef BH_TREE_INCLUDED
#define BH_TREE_INCLUDED (1)
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include "dh/math/vec.h"
#include "dh/meta.h"
#include "dh/mem/Allocator.h"
#include "dh/Arr.h"
#include "dh/ArrList.h"

// Adjust as desired
#define LEAF_CAP 16

// Controls how large child node arrays are (4 in 2D).
#define BH_CHILD_COUNT 4

// Your Body data: position, mass, velocity, etc.
typedef struct Body {
    f32 x, y;   // position
    f32 vx, vy; // velocity
    f32 ax, ay; // acceleration
    f32 mass;
    f32 radius; // if you need collisions, etc.
} Body;

// A small struct to store body data in a node leaf
typedef struct LeafBody {
    f32 x, y;
    f32 mass;
} LeafBody;

// Barnes-Hut node in an implicit tree. No pointers: children are at 4*i+1..4*i+4
typedef struct BHNode {
    // bounding box
    f32 center_x, center_y; // center of this region
    f32 half_size;          // half the width/height

    // if 'count > 0', this node is a leaf with 'count' bodies in leaf[]
    // if 'count == 0', this node is an internal (branch) node, and children are at
    // indices 4*i+1..+4 in the global array, if they exist
    u16 count;

    // aggregated center of mass
    f32 com_x, com_y;
    f32 com_mass;

    // store bodies if leaf
    LeafBody leaf[LEAF_CAP];
} BHNode;

// Barnes-Hut tree container
typedef struct BHTree {
    BHNode* nodes;    // array of nodes in BFS order
    usize   capacity; // size of 'nodes' array
    usize   length;   // number of nodes actually used
    f32     theta;    // opening angle
    f32     epsilon;  // softening
} BHTree;

// Initialize or resize the BHTree's node array to hold up to 'max_nodes' nodes
bool BHTree_init(BHTree* tree, usize max_nodes, f32 theta, f32 eps);
// Free 'tree->nodes'
void BHTree_fini(BHTree* tree);
// Builds the tree from an array of bodies (pos+mass) in [bodies, bodies+n).
// bounding box is automatically determined
// returns false if we ran out of space, true if success
bool BHTree_build(BHTree* tree, Body* bodies, usize n);
// For each body, compute acceleration using the Barnes-Hut tree
void BHTree_computeForces(const BHTree* tree, Body* bodies, usize n);

#ifdef __cplusplus
} /* C++ */
#endif /* __cplusplus */
#endif /* BH_TREE_INCLUDED */

#include "barnes-hut/QuadTree.h"

#include "dh/math.h"
#include "dh/ArrList.h"
#include "dh/callback.h"
#include "dh/log.h"
#include "dh/pipe.h"

// Quad implementation
fn_(Quad_newContaining(const S$Body bodies), Quad) {
    debug_assert_nonnull(bodies.ptr);
    var min_x = f32_limit_max;
    var min_y = f32_limit_max;
    var max_x = f32_limit_min;
    var max_y = f32_limit_min;

    for_slice(bodies, body) {
        min_x = fminf(min_x, body->pos.x);
        min_y = fminf(min_y, body->pos.y);
        max_x = fmaxf(max_x, body->pos.x);
        max_y = fmaxf(max_y, body->pos.y);
    }

    let center = pipe(m_V2f32_from(min_x + max_x, min_y + max_y),(m_V2f32_scale,(0.5f)));
    let size   = fmaxf(max_x - min_x, max_y - min_y);
    return (Quad){
        .center = center,
        .size   = size
    };
}
fn_(Quad_intoQuadrant(Quad self, usize quadrant), Quad) {
    self.size *= 0.5f;
    self.center.x += (as$(f32, quadrant & 1) - 0.5f) * self.size;
    self.center.y += (as$(f32, quadrant >> 1) - 0.5f) * self.size;
    return self;
}
fn_(Quad_subdivide(const Quad* self), A$4$Quad) {
    debug_assert_nonnull(self);
    A$4$Quad quads = cleared();
    for_array_indexed(quads, quad, index) {
        *quad = Quad_intoQuadrant(*self, index);
    }
    return quads;
}

// QuadNode implementation
fn_(QuadNode_new(usize next, Quad quad, Range bodies), QuadNode) {
    return (QuadNode){
        .children = 0,
        .next     = next,
        .pos      = m_V2f32_zero,
        .mass     = 0.0f,
        .quad     = quad,
        .bodies   = bodies
    };
}
fn_(QuadNode_isLeaf(const QuadNode* self), bool) { return deref(self).children == 0; }
fn_(QuadNode_isBranch(const QuadNode* self), bool) { return deref(self).children != 0; }
fn_(QuadNode_isEmpty(const QuadNode* self), bool) { return deref(self).mass == 0.0f; }

// QuadTree implementation
fn_(QuadTree_create(mem_Allocator allocator, f32 theta, f32 eps, usize leaf_cap, usize n), E$QuadTree $guard) {
    var_type(nodes, ArrList$QuadNode, try_(ArrList_initCap(typeInfo$(QuadNode), allocator, n)));
    errdefer_(ArrList_fini(nodes.base));
    var_type(parents, ArrList$usize, try_(ArrList_initCap(typeInfo$(usize), allocator, n)));
    errdefer_(ArrList_fini(parents.base));
    return_ok({
        .theta_sq  = theta * theta,
        .eps_sq    = eps * eps,
        .leaf_cap  = leaf_cap,
        .nodes     = nodes,
        .parents   = parents,
        .allocator = allocator,
    });
}
$unguarded;
fn_(QuadTree_destroy(QuadTree* self), void) {
    debug_assert_nonnull(self);
    ArrList_fini(self->nodes.base);
    ArrList_fini(self->parents.base);
}
fn_(QuadTree_clear(QuadTree* self), void) {
    debug_assert_nonnull(self);
    // Clear and free the nodes and parents lists
    ArrList_clearRetainingCap(self->nodes.base);
    ArrList_clearRetainingCap(self->parents.base);
}

// Partition function
use_Callback(Callback$partition, (const Body* self, m_V2f32 center), bool);
static $inline_always fn_(partition(S$Body self, Callback$partition predFn, m_V2f32 center), usize) {
    if (self.len == 0) { return 0; }
    usize lhs = 0;
    usize rhs = self.len - 1;
    while (true) {
        while (lhs <= rhs && invoke(predFn, Sli_at(self, lhs), center)) { lhs++; }
        while (lhs < rhs && !invoke(predFn, Sli_at(self, rhs), center)) { rhs--; }
        if (rhs <= lhs) { break; }

        prim_swap(*Sli_at(self, lhs), *Sli_at(self, rhs));
        lhs++;
        rhs--;
    }
    return lhs;
}
// Predicates
$maybe_unused
$static $inline_always fn_(predLtX(const Body* body, m_V2f32 center), bool) {
    return body->pos.x < center.x;
}
$maybe_unused
$static $inline_always fn_(predLtY(const Body* body, m_V2f32 center), bool) {
    return body->pos.y < center.y;
}
static fn_(QuadTree_subdivide(QuadTree* self, usize node, S$Body bodies, Range range), E$void $scope) {
    debug_assert_nonnull(self);
    let   center  = Sli_at(self->nodes.items, node)->quad.center;
    usize split[] = { range.begin, 0, 0, 0, range.end };

    // predLtY
    let predLtY = wrapLam$(Callback$partition, lam_((const Body* body, m_V2f32 center), bool) { return body->pos.y < center.y; });
    split[2]    = split[0] + partition(Sli_slice(bodies, (split[0], split[4])), predLtY, center);

    // predLtX
    let predLtX = wrapLam$(Callback$partition, lam_((const Body* body, m_V2f32 center), bool) { return body->pos.x < center.x; });
    split[1]    = split[0] + partition(Sli_slice(bodies, (split[0], split[2])), predLtX, center);
    split[3]    = split[2] + partition(Sli_slice(bodies, (split[2], split[4])), predLtX, center);

    // Append the parent node index
    try_(ArrList_append(self->parents.base, meta_refPtr(&node)));

    // Get the current length of the nodes list
    let children = self->nodes.items.len;

    // Update the children index of the current node
    Sli_at(self->nodes.items, node)->children = children;

    // Create the next indices for the new nodes
    use_A$(4, usize);
    A$4$usize nexts_arr = A_init({
        children + 1,
        children + 2,
        children + 3,
        Sli_at(self->nodes.items, node)->next,
    });

    let nexts = Sli_arr$(S$usize, nexts_arr);
    // Subdivide the current node's quad
    let quads = Quad_subdivide(&Sli_at(self->nodes.items, node)->quad);
    // Append the new nodes
    for (isize index = 0; index < 4; ++index) {
        let bodies = Range_from(split[index], split[index + 1]);
        try_(ArrList_append(
            self->nodes.base,
            meta_refPtr(createFrom$(QuadNode, QuadNode_new(*Sli_at(nexts, index), *A_at(quads, index), bodies)))
        ));
    }
    return_ok({});
} $unscoped;
fn_(QuadTree_propagate(QuadTree* self), void) {
    debug_assert_nonnull(self);
    // Propagate masses and center of mass upward through the tree
    for_slice_rev (self->parents.items, node) {
        let idx = Sli_at(self->nodes.items, *node)->children;

        // Get children nodes
        let curr = Sli_at(self->nodes.items, *node);
        let c0   = Sli_at(self->nodes.items, idx);
        let c1   = Sli_at(self->nodes.items, idx + 1);
        let c2   = Sli_at(self->nodes.items, idx + 2);
        let c3   = Sli_at(self->nodes.items, idx + 3);

        // Calculate total mass and weighted position for all children
        curr->pos  = pipe(c0->pos,
            (m_V2f32_add,(c1->pos)),
            (m_V2f32_add,(c2->pos)),
            (m_V2f32_add,(c3->pos)));
        curr->mass = c0->mass
                   + c1->mass
                   + c2->mass
                   + c3->mass;
    }
    for_slice (self->nodes.items, node) {
        m_V2f32_scaleInvAsg(&node->pos, prim_max(node->mass, f32_limit_min));
    }
}
fn_(QuadTree_build(QuadTree* self, S$Body bodies), E$void $scope) {
    debug_assert_nonnull(self);
    QuadTree_clear(self);

    let quad = Quad_newContaining(bodies);
    try_(ArrList_append(
        self->nodes.base,
        meta_refPtr(createFrom$(QuadNode, QuadNode_new(0, quad, Range_from(0, bodies.len))))
    ));

    usize node = 0;
    while (node < self->nodes.items.len) {
        let range = Sli_at(self->nodes.items, node)->bodies;
        if (self->leaf_cap < Range_len(range)) {
            QuadTree_subdivide(self, node, bodies, range);
        } else {
            for (usize idx = range.begin; idx < range.end; ++idx) {
                m_V2f32_addAsg(
                    &Sli_at(self->nodes.items, node)->pos,
                    m_V2f32_scale(Sli_at(bodies, idx)->pos, Sli_at(bodies, idx)->mass)
                );
                Sli_at(self->nodes.items, node)->mass += Sli_at(bodies, idx)->mass;
            }
        }
        node++;
    }

    QuadTree_propagate(self);
    return_ok({});
} $unscoped;
fn_(QuadTree_accelerate(const QuadTree* self, m_V2f32 pos, S$Body bodies), m_V2f32) {
    debug_assert_nonnull(self);
    var acc  = m_V2f32_zero;
    var node = QuadTree_root;
    while (true) {
        let n = *Sli_at(self->nodes.items, node);

        let d    = m_V2f32_sub(n.pos, pos);
        let d_sq = m_V2f32_lenSq(d);

        const bool is_leaf            = QuadNode_isLeaf(&n);
        const bool is_far_enough_away = (n.quad.size * n.quad.size) < (d_sq * self->theta_sq);

        if (is_far_enough_away || is_leaf) {
            // Node is either a leaf or is far enough to approximate by one mass
            if (is_far_enough_away) {
                // add force from the node's total mass
                let denom = (d_sq + self->eps_sq) * sqrtf(d_sq);
                m_V2f32_addAsg(&acc, m_V2f32_scale(d, n.mass / denom));
            } else {
                // It's a leaf, sum contributions from the actual bodies
                for (usize idx = n.bodies.begin; idx < n.bodies.end; ++idx) {
                    let body = Sli_at(bodies, idx);
                    let d    = m_V2f32_sub(body->pos, pos);
                    let d_sq = m_V2f32_lenSq(d);

                    let denom = (d_sq + self->eps_sq) * sqrtf(d_sq);
                    m_V2f32_addAsg(&acc, m_V2f32_scale(d, prim_min(body->mass / denom, f32_limit_max)));
                }
            }

            if (n.next == QuadTree_root) { break; }
            // Go sideways (to next sibling or parent's sibling)
            node = n.next;
        } else {
            // Not leaf and not far enough → subdivide further
            node = n.children;
        }
    }
    return acc;
}

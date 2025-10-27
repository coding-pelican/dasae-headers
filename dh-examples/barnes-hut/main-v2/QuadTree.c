#include "QuadTree.h"
#include "Body.h"
#include "dh/ArrList.h"
#include "dh/log.h"
#include "dh/math.h"
#include "dh/meta/common.h"

// Quad implementation
Quad Quad_newContaining(const S$Body bodies) {
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

    let center = m_V2f32_scale(m_V2f32_from(min_x + max_x, min_y + max_y), 0.5f);
    let size   = fmaxf(max_x - min_x, max_y - min_y);

    return (Quad){
        .center = center,
        .size   = size
    };
}

Quad Quad_intoQuadrant(Quad self, usize quadrant) {
    self.size *= 0.5f;
    self.center.x += (as$(f32, quadrant & 1) - 0.5f) * self.size;
    self.center.y += (as$(f32, quadrant >> 1) - 0.5f) * self.size;
    return self;
}

A$4$Quad Quad_subdivide(const Quad* self) {
    debug_assert_nonnull(self);

    A$4$Quad quads = cleared();
    for_array_indexed(quads, quad, index) {
        *quad = Quad_intoQuadrant(*self, index);
    }
    return quads;
}

// QuadNode implementation
QuadNode QuadNode_new(usize next, Quad quad, Range bodies) {
    return (QuadNode){
        .children = 0,
        .next     = next,
        .pos      = m_V2f32_zero,
        .mass     = 0.0f,
        .quad     = quad,
        .bodies   = bodies
    };
}

bool QuadNode_isLeaf(const QuadNode* self) {
    debug_assert_nonnull(self);
    return self->children == 0;
}

bool QuadNode_isBranch(const QuadNode* self) {
    debug_assert_nonnull(self);
    return self->children != 0;
}

bool QuadNode_isEmpty(const QuadNode* self) {
    debug_assert_nonnull(self);
    return self->mass == 0.0f;
}

// QuadTree implementation
E$QuadTree QuadTree_create(mem_Allocator allocator, f32 theta, f32 eps, usize leaf_cap, usize n) {
    scope_reserveReturn(E$QuadTree) {
        var nodes = type$(ArrList$QuadNode, try(ArrList_initCap(typeInfo$(QuadNode), allocator, n)));
        errdefer(ArrList_fini(&nodes.base));
        var parents = type$(ArrList$usize, try(ArrList_initCap(typeInfo$(usize), allocator, n)));
        errdefer(ArrList_fini(&parents.base));

        return_ok((QuadTree){
            .theta_sq  = theta * theta,
            .eps_sq    = eps * eps,
            .leaf_cap  = leaf_cap,
            .nodes     = nodes,
            .parents   = parents,
            .allocator = allocator,
        });
    }
    scope_returnReserved;
}

void QuadTree_destroy(QuadTree* self) {
    debug_assert_nonnull(self);

    ArrList_fini(&self->nodes.base);
    ArrList_fini(&self->parents.base);
}

void QuadTree_clear(QuadTree* self) {
    debug_assert_nonnull(self);

    // Clear and free the nodes and parents lists
    ArrList_clearRetainingCap(&self->nodes.base);
    ArrList_clearRetainingCap(&self->parents.base);
}

// Partition function
$inline_always usize partition(S$Body self, bool (*predFn)(const Body* self, m_V2f32 center), m_V2f32 center) {
    if (self.len == 0) { return 0; }
    usize lhs = 0;
    usize rhs = self.len - 1;
    while (true) {
        while (lhs <= rhs && predFn(Sli_at(self, lhs), center)) { lhs++; }
        while (lhs < rhs && !predFn(Sli_at(self, rhs), center)) { rhs--; }
        if (rhs <= lhs) { break; }

        prim_swap(*Sli_at(self, lhs), *Sli_at(self, rhs));
        lhs++;
        rhs--;
    }
    return lhs;
}
// Predicates
static $inline bool predLtX(const Body* body, m_V2f32 center) {
    return body->pos.x < center.x;
}
static $inline bool predLtY(const Body* body, m_V2f32 center) {
    return body->pos.y < center.y;
}

static E$void QuadTree_subdivide(QuadTree* self, usize node, S$Body bodies, Range range) {
    reserveReturn(E$void);
    debug_assert_nonnull(self);

    let   center  = Sli_at(self->nodes.items, node)->quad.center;
    usize split[] = { range.begin, 0, 0, 0, range.end };

    // predLtY
    split[2] = split[0] + partition((S$Body)Sli_range(bodies.ptr, split[0], split[4]), predLtY, center);

    // predLtX
    split[1] = split[0] + partition((S$Body)Sli_range(bodies.ptr, split[0], split[2]), predLtX, center);
    split[3] = split[2] + partition((S$Body)Sli_range(bodies.ptr, split[2], split[4]), predLtX, center);

    // Append the parent node index
    try(ArrList_append(&self->parents.base, meta_refPtr(&node)));

    // Get the current length of the nodes list
    let children = self->nodes.items.len;

    // Update the children index of the current node
    Sli_at(self->nodes.items, node)->children = children;

    // Create the next indices for the new nodes
    usize nexts_arr[] = {
        children + 1,
        children + 2,
        children + 3,
        Sli_at(self->nodes.items, node)->next,
    };
    const S$usize nexts = Sli_arr(nexts_arr);
    // Subdivide the current node's quad
    let           quads = Quad_subdivide(&Sli_at(self->nodes.items, node)->quad);
    // Append the new nodes
    for (isize index = 0; index < 4; ++index) {
        let bodies = Range_from(split[index], split[index + 1]);
        try(ArrList_append(
            &self->nodes.base,
            meta_refPtr(createFrom$(
                QuadNode, QuadNode_new(*Sli_at(nexts, index), *A_at(quads, index), bodies)
            ))
        ));
    }
    return_void();
}

void QuadTree_propagate(QuadTree* self) {
    debug_assert_nonnull(self);

    // Propagate masses and center of mass upward through the tree
    for_slice_rev(self->parents.items, node) {
        let idx = Sli_at(self->nodes.items, *node)->children;

        // Get children nodes
        let curr = Sli_at(self->nodes.items, *node);
        let c0   = Sli_at(self->nodes.items, idx);
        let c1   = Sli_at(self->nodes.items, idx + 1);
        let c2   = Sli_at(self->nodes.items, idx + 2);
        let c3   = Sli_at(self->nodes.items, idx + 3);

        // Calculate total mass and weighted position for all children
        curr->pos = eval({
            var p = c0->pos;
            m_V2f32_addTo(&p, c1->pos);
            m_V2f32_addTo(&p, c2->pos);
            m_V2f32_addTo(&p, c3->pos);
            eval_return p;
        });

        curr->mass = c0->mass
                   + c1->mass
                   + c2->mass
                   + c3->mass;
    }
    for_slice(self->nodes.items, node) {
        m_V2f32_scaleInvTo(&node->pos, prim_max(node->mass, f32_limit_min));
    }
}

E$void QuadTree_build(QuadTree* self, S$Body bodies) {
    reserveReturn(E$void);
    debug_assert_nonnull(self);
    QuadTree_clear(self);

    let quad = Quad_newContaining(bodies);
    try(ArrList_append(
        &self->nodes.base,
        meta_refPtr(createFrom$(
            QuadNode, QuadNode_new(0, quad, Range_from(0, bodies.len))
        ))
    ));

    usize node = 0;
    while (node < self->nodes.items.len) {
        let range = Sli_at(self->nodes.items, node)->bodies;
        if (self->leaf_cap < Range_len(range)) {
            QuadTree_subdivide(self, node, bodies, range);
        } else {
            for (usize idx = range.begin; idx < range.end; ++idx) {
                m_V2f32_addTo(
                    &Sli_at(self->nodes.items, node)->pos,
                    m_V2f32_scale(Sli_at(bodies, idx)->pos, Sli_at(bodies, idx)->mass)
                );
                Sli_at(self->nodes.items, node)->mass += Sli_at(bodies, idx)->mass;
            }
        }
        node++;
    }

    QuadTree_propagate(self);
    return_void();
}

m_V2f32 QuadTree_accelerate(const QuadTree* self, m_V2f32 pos, S$Body bodies) {
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
                m_V2f32_addTo(&acc, m_V2f32_scale(d, n.mass / denom));
            } else {
                // It's a leaf, sum contributions from the actual bodies
                for (usize idx = n.bodies.begin; idx < n.bodies.end; ++idx) {
                    let body = Sli_at(bodies, idx);
                    let d    = m_V2f32_sub(body->pos, pos);
                    let d_sq = m_V2f32_lenSq(d);

                    let denom = (d_sq + self->eps_sq) * sqrtf(d_sq);
                    m_V2f32_addTo(&acc, m_V2f32_scale(d, prim_min(body->mass / denom, f32_limit_max)));
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

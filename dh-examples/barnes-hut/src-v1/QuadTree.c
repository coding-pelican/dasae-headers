#include "QuadTree.h"
#include "Body.h"
#include "dh/ArrList.h"
#include "dh/log.h"
#include "dh/math.h"
#include "dh/meta/common.h"

// Quad implementation
Quad Quad_newContaining(const Sli$Body bodies) {
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

    let center = math_Vec2f_scale(math_Vec2f_from(min_x + max_x, min_y + max_y), 0.5f);
    let size   = fmaxf(max_x - min_x, max_y - min_y);

    return (Quad){
        .center = center,
        .size   = size
    };
}

usize Quad_findQuadrant(const Quad* self, math_Vec2f pos) {
    debug_assert_nonnull(self);
    return (as$(usize, pos.y > self->center.y) << 1) | (as$(usize, pos.x > self->center.x));
}

Quad Quad_intoQuadrant(Quad self, usize quadrant) {
    self.size *= 0.5f;
    self.center.x += (as$(f32, quadrant & 1) - 0.5f) * self.size;
    self.center.y += (as$(f32, quadrant >> 1) - 0.5f) * self.size;
    return self;
}

Arr$4$Quad Quad_subdivide(const Quad* self) {
    debug_assert_nonnull(self);

    Arr$4$Quad quads = cleared();
    for_array_indexed(quads, quad, index) {
        *quad = Quad_intoQuadrant(*self, index);
    }
    return quads;
}

// QuadNode implementation
QuadNode QuadNode_new(usize next, Quad quad) {
    return (QuadNode){
        .children = 0,
        .next     = next,
        .pos      = math_Vec2f_zero,
        .mass     = 0.0f,
        .quad     = quad
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
Err$QuadTree QuadTree_create(mem_Allocator allocator, f32 theta, f32 epsilon, usize n) {
    scope_reserveReturn(Err$QuadTree) {
        var nodes = typed(ArrList$QuadNode, try(ArrList_initCap(typeInfo(QuadNode), allocator, n)));
        errdefer(ArrList_fini(&nodes.base));
        var parents = typed(ArrList$usize, try(ArrList_initCap(typeInfo(usize), allocator, n)));
        errdefer(ArrList_fini(&parents.base));

        return_ok((QuadTree){
            .theta_sq  = theta * theta,
            .eps_sq    = epsilon * epsilon,
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

Err$void QuadTree_clear(QuadTree* self, Quad quad) {
    reserveReturn(Err$void);
    debug_assert_nonnull(self);

    // Clear and free the nodes and parents lists
    ArrList_clearRetainingCap(&self->nodes.base);
    ArrList_clearRetainingCap(&self->parents.base);
    // Initialize the root node
    try(ArrList_append(
        &self->nodes.base,
        meta_refPtr(createFrom$(QuadNode, QuadNode_new(QuadTree_root, quad)))
    ));

    return_void();
}

static Err$usize QuadTree_subdivide(QuadTree* self, usize node) {
    reserveReturn(Err$usize);
    debug_assert_nonnull(self);

    // Append the parent node index
    try(ArrList_append(
        &self->parents.base,
        meta_refPtr(&node)
    ));

    // Get the current length of the nodes list
    let children = self->nodes.items.len;

    // Update the children index of the current node
    Sli_at(self->nodes.items, node)->children = children;

    // Create the next indices for the new nodes
    const Sli$usize nexts = Sli_arr(
        (usize[]){
            children + 1,
            children + 2,
            children + 3,
            Sli_at(self->nodes.items, node)->next,
        }
    );

    // Subdivide the current node's quad
    let quads = Quad_subdivide(&Sli_at(self->nodes.items, node)->quad);

    // Append the new nodes
    for_array_indexed(quads, quad, index) {
        try(ArrList_append(
            &self->nodes.base,
            meta_refPtr(createFrom$(QuadNode, QuadNode_new(*Sli_at(nexts, index), *quad)))
        ));
    }

    return_ok(children);
}

Err$void QuadTree_insert(QuadTree* self, math_Vec2f pos, f32 mass) {
    reserveReturn(Err$void);
    debug_assert_nonnull(self);

    var node = QuadTree_root;

    // Find the leaf node containing the position
    while (QuadNode_isBranch(Sli_at(self->nodes.items, node))) {
        let quadrant = Quad_findQuadrant(&Sli_at(self->nodes.items, node)->quad, pos);
        node         = Sli_at(self->nodes.items, node)->children + quadrant;
    }

    let phys_data = eval({
        let curr_node = Sli_at(self->nodes.items, node);
        // If node is empty, simply store the mass and position
        if (QuadNode_isEmpty(curr_node)) {
            curr_node->pos  = pos;
            curr_node->mass = mass;
            return_void();
        };

        // Save existing node data
        let p = curr_node->pos;
        let m = curr_node->mass;

        // Handle exact same position by adding masses
        if (math_Vec2f_eq(pos, p)) {
            curr_node->mass += mass;
            return_void();
        };

        eval_return make$(
            struct {
                const TypeOf(p) p;
                const TypeOf(m) m;
            },
            .p = p,
            .m = m
        );
    });

    // Subdivide until particles can be separated
    while (true) {
        let children  = try(QuadTree_subdivide(self, node));
        let curr_node = Sli_at(self->nodes.items, node);

        let q1 = Quad_findQuadrant(&curr_node->quad, phys_data.p);
        let q2 = Quad_findQuadrant(&curr_node->quad, pos);

        if (q1 == q2) {
            node = children + q1;
            continue;
        }
        // Store particles in their respective quadrants
        let n1 = Sli_at(self->nodes.items, children + q1);
        let n2 = Sli_at(self->nodes.items, children + q2);

        n1->pos  = phys_data.p;
        n1->mass = phys_data.m;
        n2->pos  = pos;
        n2->mass = mass;
        break;
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
            var p = math_Vec2f_scale(c0->pos, c0->mass);
            math_Vec2f_addTo(&p, math_Vec2f_scale(c1->pos, c1->mass));
            math_Vec2f_addTo(&p, math_Vec2f_scale(c2->pos, c2->mass));
            math_Vec2f_addTo(&p, math_Vec2f_scale(c3->pos, c3->mass));
            eval_return p;
        });

        curr->mass = c0->mass
                   + c1->mass
                   + c2->mass
                   + c3->mass;

        // Normalize position by total mass
        math_Vec2f_scaleInvTo(&curr->pos, curr->mass);
    }
}

math_Vec2f QuadTree_accelerate(const QuadTree* self, math_Vec2f pos) {
    debug_assert_nonnull(self);

    var acc  = math_Vec2f_zero;
    var node = QuadTree_root;

    while (true) {
        let n = Sli_at(self->nodes.items, node);

        let d    = math_Vec2f_sub(n->pos, pos);
        let d_sq = math_Vec2f_lenSq(d);

        const bool is_leaf_or_far_enough_away = QuadNode_isLeaf(n) || n->quad.size * n->quad.size < d_sq * self->theta_sq;
        if (!is_leaf_or_far_enough_away) {
            // Traverse deeper into the tree
            node = n->children;
            continue;
        }
        // Calculate gravitational force if node is a leaf or far enough away
        let denom = (d_sq + self->eps_sq) * sqrtf(d_sq);
        math_Vec2f_addTo(&acc, math_Vec2f_scale(d, fminf(n->mass / denom, f32_limit_max)));

        if (n->next == 0) { break; }
        node = n->next;
    }

    return acc;
}

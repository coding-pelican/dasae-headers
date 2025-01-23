#include "Simulation.h"
#include "utils.h"
#include "dh/math.h"

Err$Simulation Simulation_create(mem_Allocator allocator, usize n_body) {
    scope_reserveReturn(Err$Simulation) {
        const f32   dt      = 0.05f;
        const usize n       = n_body; // 100000
        const f32   theta   = 1.0f;
        const f32   epsilon = 1.0f;

        var bodies = try(utils_uniformDisc(allocator, n));
        errdefer(ArrList_fini(&bodies.base));
        var rects = typed(
            ArrList$Rect,
            try(ArrList_initCap(
                typeInfo(Rect),
                allocator,
                n
            ))
        );
        errdefer(ArrList_fini(&rects.base));

        try(ArrList_resize(&rects.base, n));
        var quad_tree = try(QuadTree_create(allocator, theta, epsilon));
        errdefer(QuadTree_destroy(&quad_tree));

        return_ok((Simulation){
            .dt        = dt,
            .frame     = 0,
            .bodies    = bodies,
            .rects     = rects,
            .quad_tree = quad_tree,
            .allocator = allocator,
        });
    }
    scope_returnReserved;
}

void Simulation_destroy(Simulation* self) {
    debug_assert_nonnull(self);
    ArrList_fini(&self->bodies.base);
    ArrList_fini(&self->rects.base);
    QuadTree_destroy(&self->quad_tree);
}

Err$void Simulation_step(Simulation* self) {
    reserveReturn(Err$void);
    debug_assert_nonnull(self);

    Simulation_iterate(self);
    try(Simulation_collide(self));
    try(Simulation_attract(self));
    self->frame += 1;

    return_void();
}

Err$void Simulation_attract(Simulation* self) {
    reserveReturn(Err$void);
    debug_assert_nonnull(self);

    // Create quad tree for current frame
    let quad = Quad_newContaining(self->bodies.items);
    try(QuadTree_clear(&self->quad_tree, quad));

    // Insert all bodies
    for_slice(self->bodies.items, body) {
        try(QuadTree_insert(&self->quad_tree, body->pos, body->mass));
    }

    // Propagate masses upward
    QuadTree_propagate(&self->quad_tree);

    // Calculate accelerations
    for_slice(self->bodies.items, body) {
        body->acc = QuadTree_calculateAcceleration(&self->quad_tree, body->pos);
    }

    return_void();
}

void Simulation_iterate(Simulation* self) {
    debug_assert_nonnull(self);

    for_slice(self->bodies.items, body) {
        Body_update(body, self->dt);
    }
}

Err$void Simulation_collide(Simulation* self) {
    reserveReturn(Err$void);
    debug_assert_nonnull(self);

    // Update collision rects for current frame
    for_slice_indexed(self->bodies.items, body, index) {
        let pos        = body->pos;
        let radius     = body->radius;
        let radius_vec = math_Vec2f_scale(math_Vec2f_one, radius);

        let rect  = Sli_at(self->rects.items, index);
        rect->min = math_Vec2f_sub(pos, radius_vec);
        rect->max = math_Vec2f_add(pos, radius_vec);
    }

    // Note: In a real implementation we would use a spatial partitioning system here
    // For now we'll do a simple N^2 check
    for (usize lhs = 0; lhs < self->bodies.items.len; ++lhs) {
        for (usize rhs = lhs + 1; rhs < self->bodies.items.len; ++rhs) {
            Simulation_resolve(self, lhs, rhs);
        }
    }

    return_void();
}

void Simulation_resolve(Simulation* self, usize lhs, usize rhs) {
    debug_assert_nonnull(self);
    debug_assert(lhs < self->bodies.items.len);
    debug_assert(rhs < self->bodies.items.len);

    // Get references to the colliding bodies
    let b1 = Sli_at(self->bodies.items, lhs);
    let b2 = Sli_at(self->bodies.items, rhs);

    // Calculate distances and radiuses
    let p1 = b1->pos;
    let p2 = b2->pos;
    let d  = math_Vec2f_sub(p2, p1);

    let r1 = b1->radius;
    let r2 = b2->radius;
    let r  = r1 + r2;

    // Early exit if not colliding
    scope_if(const bool collides = math_Vec2f_lenSq(d) <= r * r, !collides) { return; }

    // Get velocities and masses
    let v1 = b1->vel;
    let v2 = b2->vel;
    let v  = math_Vec2f_sub(v2, v1);

    let d_dot_v = math_Vec2f_dot(d, v);

    let m1      = b1->mass;
    let m2      = b2->mass;
    let weight1 = m2 / (m1 + m2);
    let weight2 = m1 / (m1 + m2);

    // Handle collision based on relative motion
    if (0.0f <= d_dot_v && !math_Vec2f_eq(d, math_Vec2f_zero)) {
        let tmp = math_Vec2f_scale(
            d,
            r / math_Vec2f_len(d) - 1.0f
        );
        b1->pos = math_Vec2f_sub(b1->pos, math_Vec2f_scale(tmp, weight1));
        b2->pos = math_Vec2f_add(b2->pos, math_Vec2f_scale(tmp, weight2));
        return;
    }

    let v_sq = math_Vec2f_lenSq(v);
    let d_sq = math_Vec2f_lenSq(d);
    let r_sq = r * r;

    let t = (d_dot_v + sqrtf(fmaxf(d_dot_v * d_dot_v - v_sq * (d_sq - r_sq), 0.0f))) / v_sq;

    // Move to collision point
    b1->pos = math_Vec2f_sub(b1->pos, math_Vec2f_scale(v1, t));
    b2->pos = math_Vec2f_sub(b2->pos, math_Vec2f_scale(v2, t));

    // Update distances post-collision
    let p1_new      = b1->pos;
    let p2_new      = b2->pos;
    let d_new       = math_Vec2f_sub(p2_new, p1_new);
    let d_dot_v_new = math_Vec2f_dot(d_new, v);
    let d_sq_new    = math_Vec2f_lenSq(d_new);

    // Calculate collision response
    let tmp = math_Vec2f_scale(
        d_new,
        1.5f * d_dot_v_new / d_sq_new
    );
    let v1_new = math_Vec2f_add(v1, math_Vec2f_scale(tmp, weight1));
    let v2_new = math_Vec2f_sub(v2, math_Vec2f_scale(tmp, weight2));

    // Update velocities and positions
    b1->vel = v1_new;
    b2->vel = v2_new;
    b1->pos = math_Vec2f_add(b1->pos, math_Vec2f_scale(v1_new, t));
    b2->pos = math_Vec2f_add(b2->pos, math_Vec2f_scale(v2_new, t));
}

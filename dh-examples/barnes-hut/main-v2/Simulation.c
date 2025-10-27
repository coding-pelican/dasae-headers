#include "Simulation.h"
#include "dh/core/cmp.h"
#include "utils.h"
#include "dh/math.h"

// n target: 100000
E$Simulation Simulation_create(mem_Allocator allocator, usize n) {
    scope_reserveReturn(E$Simulation) {
        const f32   dt       = 0.05f;
        const f32   theta    = 1.0f;
        const f32   eps      = 1.0f;
        const usize leaf_cap = 16;

        var bodies = try(utils_uniformDisc(allocator, n));
        errdefer(ArrList_fini(&bodies.base));
        var rects = type$(ArrList$Rect, try(ArrList_initCap(typeInfo$(Rect), allocator, n)));
        errdefer(ArrList_fini(&rects.base));

        try(ArrList_resize(&rects.base, n));
        var quadtree = try(QuadTree_create(allocator, theta, eps, leaf_cap, n));
        errdefer(QuadTree_destroy(&quadtree));

        // Sort body indices based on their AABB's min.x to enable sweep and prune
        var sort_body_indices_cache = meta_cast$(S$usize, try(mem_Allocator_alloc(allocator, typeInfo$(usize), n)));
        errdefer(mem_Allocator_free(allocator, anySli(sort_body_indices_cache)));

        // Sort body rects based on their AABB's min.x to enable sweep and prune
        var sort_rect_indices_cache = meta_cast$(S$u8, try(mem_Allocator_alloc(allocator, typeInfo$(u8), n * sizeOf$(usize))));
        errdefer(mem_Allocator_free(allocator, anySli(sort_rect_indices_cache)));

        return_ok((Simulation){
            .dt                              = dt,
            .frame                           = 0,
            .bodies                          = bodies,
            .rects                           = rects,
            .quadtree                        = quadtree,
            .sort_body_indices_cache         = sort_body_indices_cache,
            .sort_rect_indices_cache_as_temp = sort_rect_indices_cache,
#if debug_comp_enabled || Simulation_comp_enabled_record_collision_count
            .collision_count = 0,
#endif
            .allocator = allocator,
        });
    }
    scope_returnReserved;
}

void Simulation_destroy(Simulation* self) {
    debug_assert_nonnull(self);

    ArrList_fini(&self->bodies.base);
    ArrList_fini(&self->rects.base);

    QuadTree_destroy(&self->quadtree);

    mem_Allocator_free(self->allocator, anySli(self->sort_body_indices_cache));
    mem_Allocator_free(self->allocator, anySli(self->sort_rect_indices_cache_as_temp));
}

E$void Simulation_step(Simulation* self) {
    reserveReturn(E$void);
    debug_assert_nonnull(self);

    Simulation_iterate(self);
    try(Simulation_collide(self));
    try(Simulation_attract(self));
    self->frame += 1;

    return_void();
}

void Simulation_iterate(Simulation* self) {
    debug_assert_nonnull(self);

    for_slice(self->bodies.items, body) {
        Body_update(body, self->dt);
    }
}

#define CollideMethod_simply_o_n_pow_2 (0)
#define CollideMethod_sweep_and_prune  (1)
#define COLLIDE_METHOD                 CollideMethod_sweep_and_prune
#if COLLIDE_METHOD == (CollideMethod_simply_o_n_pow_2 || CollideMethod_sweep_and_prune)
#if COLLIDE_METHOD == CollideMethod_simply_o_n_pow_2
E$void Simulation_collide(Simulation* self) {
    reserveReturn(E$void);
    debug_assert_nonnull(self);
#if debug_comp_enabled
    self->collision_count = 0;
#endif
    // Update collision rects for current frame
    for_slice_indexed(self->bodies.items, body, index) {
        let pos        = body->pos;
        let radius     = body->radius;
        let radius_vec = m_V2f32_scale(m_V2f32_one, radius);

        let rect  = Sli_at(self->rects.items, index);
        rect->min = m_V2f32_sub(pos, radius_vec);
        rect->max = m_V2f32_add(pos, radius_vec);
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
#endif /* CollideMethod_simply_o_n_pow_2 */
#if COLLIDE_METHOD == CollideMethod_sweep_and_prune
// Comparison function for qsort (C-style)
static cmp_Ord compareRects(P_const$raw lhs, P_const$raw rhs, P_const$raw arg) {
    let self     = as$(const Simulation*, arg);
    let idx_lhs  = *as$(const usize*, lhs);
    let idx_rhs  = *as$(const usize*, rhs);
    let rect_lhs = Sli_at(self->rects.items, idx_lhs); // Access rects using array indexing
    let rect_rhs = Sli_at(self->rects.items, idx_rhs); // Access rects using array indexing
    if (rect_lhs->min.x < rect_rhs->min.x) { return cmp_Ord_lt; }
    if (rect_lhs->min.x > rect_rhs->min.x) { return cmp_Ord_gt; }
    return cmp_Ord_eq;
}
/// optimizations:
///     Sweep and Prune Algorithm:
///         Sorting: Bodies are sorted based on the minimum x-coordinate of their AABB. This allows efficient pruning of non-overlapping pairs.
///         Early Exit: For each body, subsequent bodies are checked only until their AABB's minimum x exceeds the current body's maximum x, reducing the number of checks.
///         Y-axis Overlap Check: After x-axis overlap is confirmed, a quick y-axis check further filters non-colliding pairs.
///     AABB Checks Before Distance Calculation:
///         Modified Simulation_resolve to first check AABB overlap (already integrated into the sweep and prune), which is computationally cheaper than distance checks.
/// This approach reduces the complexity from O(n^2) to O(n log n) for sorting plus O(n + k) for checks, where k is the number of overlapping pairs.
E$void Simulation_collide(Simulation* self) {
    scope_reserveReturn(E$void) {
        debug_assert_nonnull(self);
#if debug_comp_enabled || Simulation_comp_enabled_record_collision_count
        self->collision_count = 0;
#endif
        if (self->rects.items.len == 0) {
            return_void();
        }

        // Update collision rects for current frame
        for_slice_indexed(self->bodies.items, body, index) {
            let pos        = body->pos;
            let radius     = body->radius;
            let radius_vec = m_V2f32_scale(m_V2f32_one, radius);

            let rect  = Sli_at(self->rects.items, index);
            rect->min = m_V2f32_sub(pos, radius_vec);
            rect->max = m_V2f32_add(pos, radius_vec);
        }

        let indices = self->sort_body_indices_cache;
        for_slice_indexed(indices, index, i) {
            *index = i;
        }

        // Sort indices using stableSort with comparison function and self pointer
        try(utils_stableSortWithArgUsingTemp(self->sort_rect_indices_cache_as_temp, meta_refSli(indices), compareRects, self));
        // try(stableSort(indices.ptr, indices.len, sizeof(usize), compareRects, self, self->allocator));

        // Sweep through sorted indices and check for AABB overlaps
        for (usize current = 0; current < indices.len; ++current) {
            let current_idx  = *Sli_at(indices, current);
            let current_rect = Sli_at(self->rects.items, current_idx);
            for (usize other = current + 1; other < indices.len; ++other) {
                let other_idx  = *Sli_at(indices, other);
                let other_rect = Sli_at(self->rects.items, other_idx);

                // Exit early if no overlap in x-axis
                if (current_rect->max.x < other_rect->min.x) { break; }

                // Check for overlap in y-axis
                const bool y_overlap = !(current_rect->max.y < other_rect->min.y || other_rect->max.y < current_rect->min.y);
                if (!y_overlap) { continue; }

                // NOTE: 이것 때문에 충돌 처리 대부분이 스킵되어버림...
                // vvv
                // // Ensure each pair is checked only once (current_idx < other_idx)
                // // if (other_idx <= current_idx) { continue; }

                Simulation_resolve(self, current_idx, other_idx);
            }
        }

        return_void();
    }
    scope_returnReserved;
}
#endif /* CollideMethod_sweep_and_prune */
#endif /* COLLIDE_METHOD */

E$void Simulation_attract(Simulation* self) {
    reserveReturn(E$void);
    debug_assert_nonnull(self);

    try(QuadTree_build(&self->quadtree, self->bodies.items));

    // Calculate accelerations
    for_slice(self->bodies.items, body) {
        body->acc = QuadTree_accelerate(&self->quadtree, body->pos, self->bodies.items);
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
    let d  = m_V2f32_sub(p2, p1);

    let r1 = b1->radius;
    let r2 = b2->radius;
    let r  = r1 + r2;

    // Early exit if not colliding
    scope_if(const bool collides = m_V2f32_lenSq(d) < r * r, !collides) { return; }
#if debug_comp_enabled || Simulation_comp_enabled_record_collision_count
    self->collision_count++;
#endif

    // Get velocities and masses
    let v1 = b1->vel;
    let v2 = b2->vel;
    let v  = m_V2f32_sub(v2, v1);

    let d_dot_v = m_V2f32_dot(d, v);

    let m1      = b1->mass;
    let m2      = b2->mass;
    let weight1 = m2 / (m1 + m2);
    let weight2 = m1 / (m1 + m2);

    // Handle collision based on relative motion
    if (0.0f <= d_dot_v && m_V2f32_ne(d, m_V2f32_zero)) {
        let tmp = m_V2f32_scale(d, r / m_V2f32_len(d) - 1.0f);
        m_V2f32_subTo(&b1->pos, m_V2f32_scale(tmp, weight1));
        m_V2f32_addTo(&b2->pos, m_V2f32_scale(tmp, weight2));
        return;
    }

    let v_sq = m_V2f32_lenSq(v);
    let d_sq = m_V2f32_lenSq(d);
    let r_sq = r * r;

    let t = (d_dot_v + sqrtf(fmaxf(d_dot_v * d_dot_v - v_sq * (d_sq - r_sq), 0.0f))) / v_sq;

    // Move to collision point
    m_V2f32_subTo(&b1->pos, m_V2f32_scale(v1, t));
    m_V2f32_subTo(&b2->pos, m_V2f32_scale(v2, t));

    // Update distances post-collision
    let p1_new      = b1->pos;
    let p2_new      = b2->pos;
    let d_new       = m_V2f32_sub(p2_new, p1_new);
    let d_dot_v_new = m_V2f32_dot(d_new, v);
    let d_sq_new    = m_V2f32_lenSq(d_new);

    // Calculate collision response
    let tmp    = m_V2f32_scale(d_new, 1.5f * d_dot_v_new / d_sq_new);
    let v1_new = m_V2f32_add(v1, m_V2f32_scale(tmp, weight1));
    let v2_new = m_V2f32_sub(v2, m_V2f32_scale(tmp, weight2));

    // Update velocities and positions
    b1->vel = v1_new;
    b2->vel = v2_new;
    m_V2f32_addTo(&b1->pos, m_V2f32_scale(v1_new, t));
    m_V2f32_addTo(&b2->pos, m_V2f32_scale(v2_new, t));
}

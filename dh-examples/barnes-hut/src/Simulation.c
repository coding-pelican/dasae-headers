#include "Simulation.h"
#include "dh/core/cmp.h"
#include "utils.h"
#include "dh/math.h"

Err$Simulation Simulation_create(mem_Allocator allocator, usize n_body) {
    scope_reserveReturn(Err$Simulation) {
        const f32   dt      = 0.05f;
        const usize n       = n_body; // target: 100000
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
        var quad_tree = try(QuadTree_create(allocator, theta, epsilon, n_body));
        errdefer(QuadTree_destroy(&quad_tree));

        // Sort body indices based on their AABB's min.x to enable sweep and prune
        var sort_cache = meta_cast$(Sli$usize, try(mem_Allocator_alloc(allocator, typeInfo(usize), n_body)));
        errdefer(mem_Allocator_free(allocator, anySli(sort_cache)));

        return_ok((Simulation){
            .dt         = dt,
            .frame      = 0,
            .bodies     = bodies,
            .rects      = rects,
            .quad_tree  = quad_tree,
            .sort_cache = sort_cache,
#if DEBUG_ENABLED || Simulation_allows_record_collision_count
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
    QuadTree_destroy(&self->quad_tree);
    mem_Allocator_free(self->allocator, anySli(self->sort_cache));
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

#define COLLIDE_TYPE 1
#if COLLIDE_TYPE == 0
Err$void Simulation_collide(Simulation* self) {
    reserveReturn(Err$void);
    debug_assert_nonnull(self);
#if DEBUG_ENABLED
    self->collision_count = 0;
#endif
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
#endif // COLLIDE_TYPE == 0

#if COLLIDE_TYPE == 1
/// optimizations:
///     Sweep and Prune Algorithm:
///         Sorting: Bodies are sorted based on the minimum x-coordinate of their AABB. This allows efficient pruning of non-overlapping pairs.
///         Early Exit: For each body, subsequent bodies are checked only until their AABB's minimum x exceeds the current body's maximum x, reducing the number of checks.
///         Y-axis Overlap Check: After x-axis overlap is confirmed, a quick y-axis check further filters non-colliding pairs.
///     AABB Checks Before Distance Calculation:
///         Modified Simulation_resolve to first check AABB overlap (already integrated into the sweep and prune), which is computationally cheaper than distance checks.
/// This approach reduces the complexity from O(n^2) to O(n log n) for sorting plus O(n + k) for checks, where k is the number of overlapping pairs.

#if DEPRECATED_CODE
// Helper function to perform a safe multiplication, avoiding potential overflow
use_Err(MulErr, Overflow);
force_inline Err$usize mulSafe(usize lhs, usize rhs) {
    reserveReturn(Err$usize);
    if (0 < lhs && SIZE_MAX / lhs < rhs) {
        // Multiplication would overflow
        return_err(MulErr_err(MulErrType_Overflow));
    }
    return_ok(lhs * rhs);
}
// Modernized merge sort with temporary buffer (stable sort)
static Err$void mergeSortWithTmpRecur( // NOLINT
    anyptr base,
    usize  num,
    usize  size,
    cmp_Ord (*comp)(anyptr_const lhs, anyptr_const rhs, anyptr_const arg),
    anyptr arg,
    Sli$u8 temp_buffer
) {
    scope_reserveReturn(Err$void) {
        if (num <= 1) { return_void(); /* Nothing to sort */ }

        let mid        = num / 2;
        let base_bytes = as$(u8*, base); // For pointer arithmetic
        let temp_bytes = as$(u8*, temp_buffer.ptr);

        // Sort each half recursively
        try(mergeSortWithTmpRecur(base_bytes, mid, size, comp, arg, temp_buffer));
        try(mergeSortWithTmpRecur(base_bytes + mid * size, num - mid, size, comp, arg, temp_buffer));

        // Merge the sorted halves using the temporary buffer
        usize left_index  = 0;
        usize right_index = mid;
        usize temp_index  = 0;

        while (left_index < mid && right_index < num) {
            if (comp(base_bytes + left_index * size, base_bytes + right_index * size, arg) <= 0) {
                memcpy(temp_bytes + temp_index * size, base_bytes + left_index * size, size);
                left_index++;
            } else {
                memcpy(temp_bytes + temp_index * size, base_bytes + right_index * size, size);
                right_index++;
            }
            temp_index++;
        }

        // Copy remaining elements
        if (left_index < mid) {
            let remaining = mid - left_index;
            memcpy(temp_bytes + temp_index * size, base_bytes + left_index * size, remaining * size);
            temp_index += remaining;
        }
        if (right_index < num) {
            let remaining = num - right_index;
            memcpy(temp_bytes + temp_index * size, base_bytes + right_index * size, remaining * size);
            temp_index += remaining;
        }

        // Copy all merged elements back
        memcpy(base_bytes, temp_bytes, temp_index * size);

        return_void();
    }
    scope_returnReserved;
}
// Modernized stable sort (using merge sort)
static Err$void stableSort(
    anyptr base,
    usize  num,
    usize  size,
    cmp_Ord (*comp)(anyptr_const lhs, anyptr_const rhs, anyptr_const arg),
    anyptr        arg,
    mem_Allocator allocator
) {
    scope_reserveReturn(Err$void) {
        // Allocate temporary buffer using the provided allocator
        let checked_size = try(mulSafe(num, size));
        let temp_buffer  = meta_cast$(Sli$u8, try(mem_Allocator_alloc(allocator, typeInfo(u8), checked_size)));
        defer(mem_Allocator_free(allocator, anySli(temp_buffer))); // Ensure cleanup

        // Perform merge sort
        try(mergeSortWithTmpRecur(base, num, size, comp, arg, temp_buffer));
        return_void();
    }
    scope_returnReserved;
}
#endif // DEPRECATED_CODE
// Comparison function for qsort (C-style)
static cmp_Ord compareRects(anyptr_const lhs, anyptr_const rhs, anyptr_const arg) {
    let self     = as$(const Simulation*, arg);
    let idx_lhs  = *as$(const usize*, lhs);
    let idx_rhs  = *as$(const usize*, rhs);
    let rect_lhs = Sli_at(self->rects.items, idx_lhs); // Access rects using array indexing
    let rect_rhs = Sli_at(self->rects.items, idx_rhs); // Access rects using array indexing
    if (rect_lhs->min.x < rect_rhs->min.x) { return cmp_Ord_lt; }
    if (rect_lhs->min.x > rect_rhs->min.x) { return cmp_Ord_gt; }
    return cmp_Ord_eq;
}
Err$void Simulation_collide(Simulation* self) {
    scope_reserveReturn(Err$void) {
        debug_assert_nonnull(self);
#if DEBUG_ENABLED || Simulation_allows_record_collision_count
        self->collision_count = 0;
#endif
        // Update collision rects for current frame
        for_slice_indexed(self->bodies.items, body, index) {
            let pos        = body->pos;
            let radius     = body->radius;
            let radius_vec = math_Vec2f_scale(math_Vec2f_one, radius);

            let rect  = Sli_at(self->rects.items, index);
            rect->min = math_Vec2f_sub(pos, radius_vec);
            rect->max = math_Vec2f_add(pos, radius_vec);
        }

        let indices = self->sort_cache;
        for_slice_indexed(indices, index, i) {
            *index = i;
        }

        // Sort indices using stableSort with comparison function and self pointer
        try(utils_stableSortWithArg(self->allocator, meta_refSli(indices), compareRects, self));
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
#endif // COLLIDE_TYPE == 1

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
    scope_if(const bool collides = math_Vec2f_lenSq(d) < r * r, !collides) { return; }
#if DEBUG_ENABLED || Simulation_allows_record_collision_count
    self->collision_count++;
#endif

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

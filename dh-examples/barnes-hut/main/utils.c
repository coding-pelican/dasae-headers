#include "utils.h"
#include "dh/core/cmp.h"
#include "dh/math.h"
#include "dh/Random.h"
#include "dh/mem/cfg.h"

static $inline cmp_Ord compareBodyDistance(anyptr_const lhs, anyptr_const rhs) {
    let lhs_body = as$(const Body*, lhs);
    let rhs_body = as$(const Body*, rhs);
    let lhs_dist = math_Vec2f_lenSq(lhs_body->pos);
    let rhs_dist = math_Vec2f_lenSq(rhs_body->pos);
    if (lhs_dist < rhs_dist) { return cmp_Ord_lt; }
    if (lhs_dist > rhs_dist) { return cmp_Ord_gt; }
    return cmp_Ord_eq;
}

fn_ext_scope(utils_uniformDisc(mem_Allocator allocator, usize n), Err$ArrList$Body) {
    // Initialize random seed
    Random_initWithSeed(0);

    // Set up parameters
    const f32 inner_radius = 25.0f;
    const f32 outer_radius = sqrtf(as$(f32, n)) * 5.0f;

    // Create bodies array
    var bodies = type$(ArrList$Body, try_(ArrList_initCap(typeInfo$(Body), allocator, n)));

    // Add center black hole
    const f32 m = 1e6f;
    try_(ArrList_append(
        bodies.base,
        meta_refPtr(createFrom$(Body,
            Body_new(math_Vec2f_zero, math_Vec2f_zero, m, inner_radius)
        ))
    ));

    // Generate outer bodies
    while (bodies.items.len < n) {
        // Random angle
        let a    = Random_f32() * math_f32_tau;
        let sc_a = math_Vec2f_sincos(a);

        // Random radius with inner cutoff
        let t = inner_radius / outer_radius;
        let r = Random_f32() * (1.0f - t * t) + t * t;

        // Calculate position and initial velocity direction
        let pos = math_Vec2f_scale(sc_a, outer_radius * sqrtf(r));
        let vel = math_Vec2f_from(sc_a.y, -sc_a.x);

        // Set mass and radius
        const f32 mass   = 1.0f;
        const f32 radius = cbrtf(mass);

        // Create and add the body
        try_(ArrList_append(
            bodies.base,
            meta_refPtr(createFrom$(Body,
                Body_new(pos, vel, mass, radius)
            ))
        ));
    }

    // Sort by distance from center and adjust velocities
    try_(sort_stableSort(
        allocator,
        meta_refSli(bodies.items),
        wrapLam$(sort_CmpFn, lam_((anyptr_const lhs, anyptr_const rhs), cmp_Ord) {
            let lhs_body = as$(const Body*, lhs);
            let rhs_body = as$(const Body*, rhs);
            let lhs_dist = math_Vec2f_lenSq(lhs_body->pos);
            let rhs_dist = math_Vec2f_lenSq(rhs_body->pos);
            if (lhs_dist < rhs_dist) { return cmp_Ord_lt; }
            if (lhs_dist > rhs_dist) { return cmp_Ord_gt; }
            return cmp_Ord_eq;
        })
    ));

    // Adjust velocities based on enclosed mass
    var mass = 0.0f;
    for (usize index = 0; index < n; ++index) {
        mass += Sli_at(bodies.items, index)->mass;

        // Skip center body
        if (math_Vec2f_eq(Sli_at(bodies.items, index)->pos, math_Vec2f_zero)) { continue; }

        // Calculate orbital velocity based on enclosed mass
        let dist = math_Vec2f_len(Sli_at(bodies.items, index)->pos);
        let v    = sqrtf(mass / dist);

        math_Vec2f_scaleAsg(&Sli_at(bodies.items, index)->vel, v);
    }

    return_ok(bodies);
}
ext_unscoped;

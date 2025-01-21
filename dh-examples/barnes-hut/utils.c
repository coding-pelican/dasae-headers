#include "utils.h"
#include "dh/math.h"
#include "dh/Random.h"

static void utils_sortBodiesByDistanceFromOrigin(Sli$Body bodies) {
    debug_assert_nonnull(bodies.ptr);

    // Simple selection sort - could be optimized with a better algorithm
    for (usize lhs = 0; lhs < bodies.len; ++lhs) {
        var min_idx  = lhs;
        let dist_lhs = math_Vec2f_lenSq(Sli_at(bodies, lhs)->pos);

        for (usize rhs = lhs + 1; rhs < bodies.len; ++rhs) {
            let dist_rhs = math_Vec2f_lenSq(Sli_at(bodies, rhs)->pos);
            if (dist_lhs <= dist_rhs) { continue; }
            min_idx = rhs;
        }

        if (min_idx == lhs) { continue; }
        prim_swap(*Sli_at(bodies, lhs), *Sli_at(bodies, min_idx));
    }
}

Err$ArrList$Body utils_uniformDisc(mem_Allocator allocator, usize n) {
    reserveReturn(Err$ArrList$Body);

    // Initialize random seed
    Random_initWithSeed(0);

    // Set up parameters
    const f32 inner_radius = 25.0f;
    const f32 outer_radius = sqrtf(as$(f32, n)) * 5.0f;

    // Create bodies array
    var bodies = typed(
        ArrList$Body,
        try(ArrList_initCap(
            typeInfo(Body),
            allocator,
            n
        ))
    );

    // Add center black hole
    const f32 m = 1e6f;
    try(ArrList_append(
        &bodies.base,
        meta_refPtr(createFrom$(
            Body,
            Body_new(
                math_Vec2f_zero,
                math_Vec2f_zero,
                m,
                inner_radius
            )
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
        let pos = math_Vec2f_scale(
            sc_a,
            outer_radius * sqrtf(r)
        );
        let vel = math_Vec2f_from(sc_a.y, -sc_a.x);

        // Set mass and radius
        const f32 mass   = 1.0f;
        const f32 radius = cbrtf(mass);

        // Create and add the body
        try(ArrList_append(
            &bodies.base,
            meta_refPtr(createFrom$(
                Body,
                Body_new(
                    pos,
                    vel,
                    mass,
                    radius
                )
            ))
        ));
    }

    // Sort by distance from center and adjust velocities
    utils_sortBodiesByDistanceFromOrigin(bodies.items);

    // Adjust velocities based on enclosed mass
    var mass = 0.0f;
    for (usize i = 0; i < n; ++i) {
        mass += Sli_at(bodies.items, i)->mass;

        // Skip center body
        if (math_Vec2f_eq(Sli_at(bodies.items, i)->pos, math_Vec2f_zero)) { continue; }

        // Calculate orbital velocity based on enclosed mass
        let dist = math_Vec2f_len(Sli_at(bodies.items, i)->pos);
        let v    = sqrtf(mass / dist);

        Sli_at(bodies.items, i)->vel = math_Vec2f_scale(Sli_at(bodies.items, i)->vel, v);
    }

    return_ok(bodies);
}

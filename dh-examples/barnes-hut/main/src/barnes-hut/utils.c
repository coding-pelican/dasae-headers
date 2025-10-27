#include "barnes-hut/utils.h"

#include "dh/sort.h"
#include "dh/core/cmp.h"
#include "dh/math.h"
#include "dh/Rand.h"
#include "dh/mem/cfg.h"

$maybe_unused static $inline fn_(compareBodyDist(P_const$raw lhs, P_const$raw rhs), cmp_Ord) {
    let lhs_body = as$(const Body*, lhs);
    let rhs_body = as$(const Body*, rhs);
    let lhs_dist = m_V2f32_lenSq(lhs_body->pos);
    let rhs_dist = m_V2f32_lenSq(rhs_body->pos);
    if (lhs_dist < rhs_dist) { return cmp_Ord_lt; }
    if (lhs_dist > rhs_dist) { return cmp_Ord_gt; }
    return cmp_Ord_eq;
}

fn_(utils_uniformDisc(mem_Allocator allocator, usize n), E$ArrList$Body $guard) {
    // Initialize random seed
    Rand_initWithSeed(0);

    // Set up parameters
    const f32 inner_radius = 25.0f;
    const f32 outer_radius = sqrtf(as$(f32, n)) * 5.0f;

    // Create bodies array
    var bodies = type$(ArrList$Body, try_(ArrList_initCap(typeInfo$(Body), allocator, n)));
    errdefer_(ArrList_fini(bodies.base));

    // Add center black hole
    const f32 m = 1e6f;
    try_(ArrList_append(
        bodies.base,
        meta_refPtr(createFrom$(Body, Body_new(m_V2f32_zero, m_V2f32_zero, m, inner_radius)))
    ));

    // Generate outer bodies
    while (bodies.items.len < n) {
        // Rand angle
        let a    = Rand_f32() * math_f32_tau;
        let sc_a = m_V2f32_sincos(a);

        // Rand radius with inner cutoff
        let t = inner_radius / outer_radius;
        let r = Rand_f32() * (1.0f - t * t) + t * t;

        // Calculate position and initial velocity direction
        let pos = m_V2f32_scale(sc_a, outer_radius * sqrtf(r));
        let vel = m_V2f32_from(sc_a.y, -sc_a.x);

        // Set mass and radius
        const f32 mass   = 1.0f;
        const f32 radius = cbrtf(mass);

        // Create and add the body
        try_(ArrList_append(
            bodies.base,
            meta_refPtr(createFrom$(Body, Body_new(pos, vel, mass, radius)))
        ));
    }

    // Sort by distance from center and adjust velocities
    try_(sort_stableSort(
        allocator,
        meta_refSli(bodies.items),
        wrapLam$(sort_CmpFn, lam_((P_const$raw lhs, P_const$raw rhs), cmp_Ord) {
            let lhs_body = as$(const Body*, lhs);
            let rhs_body = as$(const Body*, rhs);
            let lhs_dist = m_V2f32_lenSq(lhs_body->pos);
            let rhs_dist = m_V2f32_lenSq(rhs_body->pos);
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
        if (m_V2f32_eq(Sli_at(bodies.items, index)->pos, m_V2f32_zero)) { continue; }

        // Calculate orbital velocity based on enclosed mass
        let dist = m_V2f32_len(Sli_at(bodies.items, index)->pos);
        let v    = sqrtf(mass / dist);

        m_V2f32_scaleAsg(&Sli_at(bodies.items, index)->vel, v);
    }

    return_ok(bodies);
}
$unguarded;

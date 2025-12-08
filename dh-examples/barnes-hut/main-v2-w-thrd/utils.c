#include "utils.h"
#include "dh/math.h"
#include "dh/Rand.h"
#include "dh/mem/cfg.h"
#include <malloc.h>

// Swap two elements of given size
$inline_always
void utils_swapBytes(u8* const lhs, u8* const rhs, usize byte_len) {
    let tmp = as$(u8*, alloca(byte_len));
    prim_memcpy(tmp, lhs, byte_len);
    prim_memcpy(lhs, rhs, byte_len);
    prim_memcpy(rhs, tmp, byte_len);
}

void utils_insertionSort(
    meta_S base_slice,
    cmp_Ord (*compareFn)(P_const$raw lhs, P_const$raw rhs)
) {
    let ptr = as$(u8*, base_slice.addr);
    let len = base_slice.len;
    let size = base_slice.type.size;
    for (usize unsorted_index = 1; unsorted_index < len; ++unsorted_index) {
        var curr = ptr + (unsorted_index * size);
        var sorted_backward_index = unsorted_index;
        while (0 < sorted_backward_index) {
            var prev = curr - size;
            if (compareFn(prev, curr) <= cmp_Ord_eq) { break; }
            utils_swapBytes(prev, curr, size);
            curr = prev;
            sorted_backward_index--;
        }
    }
}

void utils_insertionSortWithArg(
    meta_S base_slice,
    cmp_Ord (*compareFn)(P_const$raw lhs, P_const$raw rhs, P_const$raw arg),
    P_const$raw arg
) {
    let ptr = as$(u8*, base_slice.addr);
    let len = base_slice.len;
    let size = base_slice.type.size;
    for (usize unsorted_index = 1; unsorted_index < len; ++unsorted_index) {
        var current = ptr + (unsorted_index * size);
        var sorted_backward_index = unsorted_index;
        while (0 < sorted_backward_index) {
            var prev = current - size;
            if (compareFn(prev, current, arg) <= cmp_Ord_eq) { break; }
            utils_swapBytes(prev, current, size);
            current = prev;
            sorted_backward_index--;
        }
    }
}

// TODO: 와 큰일 날 뻔... 직접적인 meta_S 생성은 금지해야겠다. meta_S_slice 함수 작성이 시급하다.
E$void utils_mergeSortUsingTempRecur( // NOLINT
    S$u8 temp_buffer,
    meta_S base_slice,
    cmp_Ord (*compareFn)(P_const$raw lhs, P_const$raw rhs)
) {
    reserveReturn(E$void);
    if (base_slice.len <= utils_stableSort_threshold_merge_to_insertion) {
        utils_insertionSort(base_slice, compareFn);
        return_void();
    }
    let base_type = base_slice.type;
    let base_bytes = as$(u8*, base_slice.addr);
    let base_len = base_slice.len;
    let base_size = base_slice.type.size;
    let mid_idx = base_len / 2;

    /* Sort each half recursively */
    try(utils_mergeSortUsingTempRecur(
        temp_buffer,
        make$(meta_S, .type = base_type, .addr = base_bytes, .len = mid_idx),
        compareFn
    ));
    try(utils_mergeSortUsingTempRecur(
        temp_buffer,
        make$(meta_S, .type = base_type, .addr = base_bytes + (mid_idx * base_size), .len = base_len - mid_idx),
        compareFn
    ));

    /* Check if merging is necessary */ {
        let left_last = base_bytes + ((mid_idx - 1) * base_size);
        let right_first = base_bytes + (mid_idx * base_size);
        if (compareFn(left_last, right_first) <= cmp_Ord_eq) {
            return_void(); /* Already ordered, no merge needed */
        }
    }

    /* Merge the sorted halves using the temporary buffer */
    var left_ptr = base_bytes;
    var left_end = left_ptr + mid_idx * base_size;
    var right_ptr = base_bytes + mid_idx * base_size;
    var right_end = base_bytes + base_len * base_size;
    var temp_ptr = temp_buffer.ptr;

    while (left_ptr < left_end && right_ptr < right_end) {
        if (compareFn(left_ptr, right_ptr) <= cmp_Ord_eq) {
            prim_memcpy(temp_ptr, left_ptr, base_size);
            left_ptr += base_size;
        } else {
            prim_memcpy(temp_ptr, right_ptr, base_size);
            right_ptr += base_size;
        }
        temp_ptr += base_size;
    }

    /* Copy remaining elements */
    if (left_ptr < left_end) {
        temp_ptr += eval({
            let bytes_left = left_end - left_ptr;
            prim_memcpy(temp_ptr, left_ptr, bytes_left);
            eval_return bytes_left;
        });
    }
    if (right_ptr < right_end) {
        temp_ptr += eval({
            let bytes_right = right_end - right_ptr;
            prim_memcpy(temp_ptr, right_ptr, bytes_right);
            eval_return bytes_right;
        });
    }

    /* Copy merged elements back to the original array */
    let total_bytes = temp_ptr - temp_buffer.ptr;
    prim_memcpy(base_bytes, temp_buffer.ptr, total_bytes);
    return_void();
}

E$void utils_mergeSortWithArgUsingTempRecur( // NOLINT
    S$u8 temp_buffer,
    meta_S base_slice,
    cmp_Ord (*compareFn)(P_const$raw lhs, P_const$raw rhs, P_const$raw arg),
    P_const$raw arg
) {
    reserveReturn(E$void);
    if (base_slice.len <= utils_stableSort_threshold_merge_to_insertion) {
        utils_insertionSortWithArg(base_slice, compareFn, arg);
        return_void();
    }
    let base_type = base_slice.type;
    let base_bytes = as$(u8*, base_slice.addr);
    let base_len = base_slice.len;
    let base_size = base_slice.type.size;
    let mid_idx = base_len / 2;

    /* Sort each half recursively */
    try(utils_mergeSortWithArgUsingTempRecur(
        temp_buffer,
        make$(meta_S, .type = base_type, .addr = base_bytes, .len = mid_idx),
        compareFn,
        arg
    ));
    try(utils_mergeSortWithArgUsingTempRecur(
        temp_buffer,
        make$(meta_S, .type = base_type, .addr = base_bytes + (mid_idx * base_size), .len = base_len - mid_idx),
        compareFn,
        arg
    ));

    /* Check if merging is necessary */ {
        let left_last = base_bytes + ((mid_idx - 1) * base_size);
        let right_first = base_bytes + (mid_idx * base_size);
        if (compareFn(left_last, right_first, arg) <= cmp_Ord_eq) {
            return_void(); /* Already ordered, no merge needed */
        }
    }

    /* Merge the sorted halves using the temporary buffer */
    var left_ptr = base_bytes;
    var left_end = left_ptr + mid_idx * base_size;
    var right_ptr = base_bytes + mid_idx * base_size;
    var right_end = base_bytes + base_len * base_size;
    var temp_ptr = temp_buffer.ptr;

    while (left_ptr < left_end && right_ptr < right_end) {
        if (compareFn(left_ptr, right_ptr, arg) <= cmp_Ord_eq) {
            prim_memcpy(temp_ptr, left_ptr, base_size);
            left_ptr += base_size;
        } else {
            prim_memcpy(temp_ptr, right_ptr, base_size);
            right_ptr += base_size;
        }
        temp_ptr += base_size;
    }

    /* Copy remaining elements */
    if (left_ptr < left_end) {
        temp_ptr += eval({
            let bytes_left = left_end - left_ptr;
            prim_memcpy(temp_ptr, left_ptr, bytes_left);
            eval_return bytes_left;
        });
    }
    if (right_ptr < right_end) {
        temp_ptr += eval({
            let bytes_right = right_end - right_ptr;
            prim_memcpy(temp_ptr, right_ptr, bytes_right);
            eval_return bytes_right;
        });
    }

    /* Copy merged elements back to the original array */
    let total_bytes = temp_ptr - temp_buffer.ptr;
    prim_memcpy(base_bytes, temp_buffer.ptr, total_bytes);
    return_void();
}

E$void utils_stableSort(
    mem_Allocator allocator,
    meta_S base_slice,
    cmp_Ord (*compareFn)(P_const$raw lhs, P_const$raw rhs)
) {
    scope_reserveReturn(E$void) {
        let checked_size = try(utils_usize_mulSafe(base_slice.len, base_slice.type.size));
        let temp_buffer = meta_cast$(S$u8, try(mem_Allocator_alloc(allocator, typeInfo$(u8), checked_size)));
        defer(mem_Allocator_free(allocator, anySli(temp_buffer)));
        try(utils_mergeSortUsingTempRecur(temp_buffer, base_slice, compareFn));
        return_void();
    }
    scope_returnReserved;
}

E$void utils_stableSortWithArg(
    mem_Allocator allocator,
    meta_S base_slice,
    cmp_Ord (*compareFn)(P_const$raw lhs, P_const$raw rhs, P_const$raw arg),
    P_const$raw arg
) {
    scope_reserveReturn(E$void) {
        let checked_size = try(utils_usize_mulSafe(base_slice.len, base_slice.type.size));
        let temp_buffer = meta_cast$(S$u8, try(mem_Allocator_alloc(allocator, typeInfo$(u8), checked_size)));
        defer(mem_Allocator_free(allocator, anySli(temp_buffer)));
        try(utils_mergeSortWithArgUsingTempRecur(temp_buffer, base_slice, compareFn, arg));
        return_void();
    }
    scope_returnReserved;
}

E$void utils_stableSortUsingTemp(
    S$u8 temp_buffer,
    meta_S base_slice,
    cmp_Ord (*compareFn)(P_const$raw lhs, P_const$raw rhs)
) {
    reserveReturn(E$void);
    let checked_size = try(utils_usize_mulSafe(base_slice.len, base_slice.type.size));
    if (temp_buffer.len < checked_size) {
        return_err(mem_AllocE_OutOfMemory());
    }
    try(utils_mergeSortUsingTempRecur(temp_buffer, base_slice, compareFn));
    return_void();
}

E$void utils_stableSortWithArgUsingTemp(
    S$u8 temp_buffer,
    meta_S base_slice,
    cmp_Ord (*compareFn)(P_const$raw lhs, P_const$raw rhs, P_const$raw arg),
    P_const$raw arg
) {
    reserveReturn(E$void);
    let checked_size = try(utils_usize_mulSafe(base_slice.len, base_slice.type.size));
    if (temp_buffer.len < checked_size) {
        return_err(mem_AllocE_OutOfMemory());
    }
    try(utils_mergeSortWithArgUsingTempRecur(temp_buffer, base_slice, compareFn, arg));
    return_void();
}

static $inline
cmp_Ord compareBodyDistance(P_const$raw lhs, P_const$raw rhs) {
    let lhs_body = as$(const Body*, lhs);
    let rhs_body = as$(const Body*, rhs);
    let lhs_dist = m_V2f32_lenSq(lhs_body->pos);
    let rhs_dist = m_V2f32_lenSq(rhs_body->pos);
    if (lhs_dist < rhs_dist) { return cmp_Ord_lt; }
    if (lhs_dist > rhs_dist) { return cmp_Ord_gt; }
    return cmp_Ord_eq;
}

E$ArrList$Body utils_uniformDisc(mem_Allocator allocator, usize n) {
    reserveReturn(E$ArrList$Body);

    // Initialize random seed
    Rand_initWithSeed(0);

    // Set up parameters
    const f32 inner_radius = 25.0f;
    const f32 outer_radius = sqrtf(as$(f32, n)) * 5.0f;

    // Create bodies array
    var bodies = type$(ArrList$Body, try(ArrList_initCap(typeInfo$(Body), allocator, n)));

    // Add center black hole
    const f32 m = 1e6f;
    try(ArrList_append(
        &bodies.base,
        meta_refPtr(createFrom$(
            Body, Body_new(m_V2f32_zero, m_V2f32_zero, m, inner_radius)
        ))
    ));

    // Generate outer bodies
    while (bodies.items.len < n) {
        // Rand angle
        let a = Rand_f32() * math_f32_tau;
        let sc_a = m_V2f32_sincos(a);

        // Rand radius with inner cutoff
        let t = inner_radius / outer_radius;
        let r = Rand_f32() * (1.0f - t * t) + t * t;

        // Calculate position and initial velocity direction
        let pos = m_V2f32_scale(sc_a, outer_radius * sqrtf(r));
        let vel = m_V2f32_from(sc_a.y, -sc_a.x);

        // Set mass and radius
        const f32 mass = 1.0f;
        const f32 radius = cbrtf(mass);

        // Create and add the body
        try(ArrList_append(
            &bodies.base,
            meta_refPtr(createFrom$(Body, Body_new(pos, vel, mass, radius)))
        ));
    }

    // Sort by distance from center and adjust velocities
    try(utils_stableSort(allocator, meta_refSli(bodies.items), compareBodyDistance));

    // Adjust velocities based on enclosed mass
    var mass = 0.0f;
    for (usize index = 0; index < n; ++index) {
        mass += Sli_at(bodies.items, index)->mass;

        // Skip center body
        if (m_V2f32_eq(Sli_at(bodies.items, index)->pos, m_V2f32_zero)) { continue; }

        // Calculate orbital velocity based on enclosed mass
        let dist = m_V2f32_len(Sli_at(bodies.items, index)->pos);
        let v = sqrtf(mass / dist);

        m_V2f32_scaleTo(&Sli_at(bodies.items, index)->vel, v);
    }

    return_ok(bodies);
}

#if DEPRECATED_CODE
// Helper function to perform a safe multiplication, avoiding potential overflow
use_Err(MulErr, Overflow);
$inline_always
E$usize mulSafe(usize lhs, usize rhs) {
    reserveReturn(E$usize);
    if (0 < lhs && SIZE_MAX / lhs < rhs) {
        // Multiplication would overflow
        return_err(MulE_err(MulErrCode_Overflow));
    }
    return_ok(lhs * rhs);
}
// Modernized merge sort with temporary buffer (stable sort)
static E$void mergeSortWithTmpRecur( // NOLINT
    P$raw base,
    usize num,
    usize size,
    cmp_Ord (*comp)(P_const$raw lhs, P_const$raw rhs, P_const$raw arg),
    P$raw arg,
    S$u8 temp_buffer
) {
    scope_reserveReturn(E$void) {
        if (num <= 1) { return_void(); /* Nothing to sort */ }

        let mid = num / 2;
        let base_bytes = as$(u8*, base); // For pointer arithmetic
        let temp_bytes = as$(u8*, temp_buffer.ptr);

        // Sort each half recursively
        try(mergeSortWithTmpRecur(base_bytes, mid, size, comp, arg, temp_buffer));
        try(mergeSortWithTmpRecur(base_bytes + mid * size, num - mid, size, comp, arg, temp_buffer));

        // Merge the sorted halves using the temporary buffer
        usize left_index = 0;
        usize right_index = mid;
        usize temp_index = 0;

        while (left_index < mid && right_index < num) {
            if (comp(base_bytes + left_index * size, base_bytes + right_index * size, arg) <= 0) {
                prim_memcpy(temp_bytes + temp_index * size, base_bytes + left_index * size, size);
                left_index++;
            } else {
                prim_memcpy(temp_bytes + temp_index * size, base_bytes + right_index * size, size);
                right_index++;
            }
            temp_index++;
        }

        // Copy remaining elements
        if (left_index < mid) {
            let remaining = mid - left_index;
            prim_memcpy(temp_bytes + temp_index * size, base_bytes + left_index * size, remaining * size);
            temp_index += remaining;
        }
        if (right_index < num) {
            let remaining = num - right_index;
            prim_memcpy(temp_bytes + temp_index * size, base_bytes + right_index * size, remaining * size);
            temp_index += remaining;
        }

        // Copy all merged elements back
        prim_memcpy(base_bytes, temp_bytes, temp_index * size);

        return_void();
    }
    scope_returnReserved;
}

// Modernized stable sort (using merge sort)
static E$void stableSort(
    P$raw base,
    usize num,
    usize size,
    cmp_Ord (*comp)(P_const$raw lhs, P_const$raw rhs, P_const$raw arg),
    P$raw arg,
    mem_Allocator allocator
) {
    scope_reserveReturn(E$void) {
        // Allocate temporary buffer using the provided allocator
        let checked_size = try(mulSafe(num, size));
        let temp_buffer = meta_cast$(S$u8, try(mem_Allocator_alloc(allocator, typeInfo$(u8), checked_size)));
        defer(mem_Allocator_free(allocator, anySli(temp_buffer))); // Ensure cleanup

        // Perform merge sort
        try(mergeSortWithTmpRecur(base, num, size, comp, arg, temp_buffer));
        return_void();
    }
    scope_returnReserved;
}
#endif // DEPRECATED_CODE

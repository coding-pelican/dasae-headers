#include "dh/sort.h"
#include "dh/mem/common.h"

/* TODO: Eliminate raw pointer operations */

$attr($inline_always)
$static fn_((sort__ord(sort_OrdFn ordFn, u_P_const$raw lhs, u_P_const$raw rhs))(cmp_Ord)) {
    return invoke(ordFn, u_load(u_deref(lhs)), u_load(u_deref(rhs)));
};
$attr($inline_always)
$static fn_((sort__ordCtx(sort_OrdCtxFn ordFn, u_P_const$raw lhs, u_P_const$raw rhs, u_P_const$raw ctx))(cmp_Ord)) {
    return invoke(ordFn, u_load(u_deref(lhs)), u_load(u_deref(rhs)), u_load(u_deref(ctx)));
};

fn_((sort_insert(u_S$raw base, sort_OrdFn ordFn))(void)) {
    for (usize unsorted_idx = 1; unsorted_idx < base.len; ++unsorted_idx) {
        var curr_ptr = u_atS(base, unsorted_idx);
        var sorted_bwd_idx = unsorted_idx;
        while (0 < sorted_bwd_idx) {
            let prev_ptr = u_atS(base, sorted_bwd_idx - 1);
            if (sort__ord(ordFn, prev_ptr.as_const, curr_ptr.as_const) <= cmp_Ord_eq) { break; }
            mem_swap(u_prefixP(prev_ptr, 1), u_prefixP(curr_ptr, 1));
            curr_ptr = prev_ptr;
            sorted_bwd_idx--;
        }
    }
};

fn_((sort_insertCtx(u_S$raw base, sort_OrdCtxFn ordFn, u_P_const$raw ctx))(void)) {
    for (usize unsorted_idx = 1; unsorted_idx < base.len; ++unsorted_idx) {
        var curr_ptr = u_atS(base, unsorted_idx);
        var sorted_bwd_idx = unsorted_idx;
        while (0 < sorted_bwd_idx) {
            let prev_ptr = u_atS(base, sorted_bwd_idx - 1);
            if (sort__ordCtx(ordFn, prev_ptr.as_const, curr_ptr.as_const, ctx) <= cmp_Ord_eq) { break; }
            mem_swap(u_prefixP(prev_ptr, 1), u_prefixP(curr_ptr, 1));
            curr_ptr = prev_ptr;
            sorted_bwd_idx--;
        }
    }
};

// NOLINTNEXTLINE(misc-no-recursion)
fn_((sort_mergeTempRecur(
    S$u8 temp,
    u_S$raw base,
    sort_OrdFn ordFn
))(sort_mem_Err$S$u8) $scope) {
    if (base.len <= sort_stable_sort_threshold_merge_to_insert) {
        sort_insert(base, ordFn);
        return_ok({});
    }
    /* Sort each half recursively */
    let mid_idx = base.len / 2;
    try_(sort_mergeTempRecur(temp, u_prefixS(base, mid_idx), ordFn));
    try_(sort_mergeTempRecur(temp, u_suffixS(base, mid_idx), ordFn));

    /* Check if merging is necessary */ {
        let l_last = u_atS(base, mid_idx - 1);
        let r_first = u_atS(base, mid_idx);
        if (sort__ord(ordFn, l_last.as_const, r_first.as_const) <= cmp_Ord_eq) {
            return_ok({}); /* Already ordered, no merge needed */
        }
    }

    /* Merge the sorted halves using the temporary buffer */
    var l_it = u_atS(base, 0);
    let l_end = u_atS(base, mid_idx);
    var r_it = l_end;
    let r_end = u_atS(base, base.len);
    var_(temp_cursor, usize) = 0;
    while (l_it.raw < l_end.raw && r_it.raw < r_end.raw) {
        if (sort__ord(ordFn, l_it.as_const, r_it.as_const) <= cmp_Ord_eq) {
            prim_memcpy(S_at((temp)[temp_cursor]), l_it.raw, base.type.size);
            l_it.raw += base.type.size;
        } else {
            prim_memcpy(S_at((temp)[temp_cursor]), r_it.raw, base.type.size);
            r_it.raw += base.type.size;
        }
        temp_cursor += base.type.size;
    }

    /* Copy remaining elements */
    if (l_it.raw < l_end.raw) {
        temp_cursor += blk({
            let bytes_left = l_end.raw - l_it.raw;
            prim_memcpy(S_at((temp)[temp_cursor]), l_it.raw, bytes_left);
            blk_return bytes_left;
        });
    }
    if (r_it.raw < r_end.raw) {
        temp_cursor += blk({
            let bytes_right = r_end.raw - r_it.raw;
            prim_memcpy(S_at((temp)[temp_cursor]), r_it.raw, bytes_right);
            blk_return bytes_right;
        });
    }

    /* Copy merged elements back to the original array */
    let total_bytes = temp_cursor;
    prim_memcpy(base.ptr, temp.ptr, total_bytes);
    return_ok(prefixS(temp, total_bytes));
} $unscoped_(fn);

// NOLINTNEXTLINE(misc-no-recursion)
fn_((sort_mergeCtxTempRecur(
    S$u8 temp,
    u_S$raw base,
    sort_OrdCtxFn ordFn,
    u_P_const$raw ctx
))(sort_mem_Err$S$u8) $scope) {
    if (base.len <= sort_stable_sort_threshold_merge_to_insert) {
        sort_insertCtx(base, ordFn, ctx);
        return_ok({});
    }
    /* Sort each half recursively */
    let mid_idx = base.len / 2;
    try_(sort_mergeCtxTempRecur(temp, u_prefixS(base, mid_idx), ordFn, ctx));
    try_(sort_mergeCtxTempRecur(temp, u_suffixS(base, mid_idx), ordFn, ctx));

    /* Check if merging is necessary */ {
        let l_last = u_atS(base, mid_idx - 1);
        let r_first = u_atS(base, mid_idx);
        if (sort__ordCtx(ordFn, l_last.as_const, r_first.as_const, ctx) <= cmp_Ord_eq) {
            return_ok({}); /* Already ordered, no merge needed */
        }
    }

    /* Merge the sorted halves using the temporary buffer */
    var l_it = u_atS(base, 0);
    let l_end = u_atS(base, mid_idx);
    var r_it = l_end;
    let r_end = u_atS(base, base.len);
    var_(temp_cursor, usize) = 0;
    while (l_it.raw < l_end.raw && r_it.raw < r_end.raw) {
        if (sort__ordCtx(ordFn, l_it.as_const, r_it.as_const, ctx) <= cmp_Ord_eq) {
            prim_memcpy(S_at((temp)[temp_cursor]), l_it.raw, base.type.size);
            l_it.raw += base.type.size;
        } else {
            prim_memcpy(S_at((temp)[temp_cursor]), r_it.raw, base.type.size);
            r_it.raw += base.type.size;
        }
        temp_cursor += base.type.size;
    }

    /* Copy remaining elements */
    if (l_it.raw < l_end.raw) {
        temp_cursor += blk({
            let bytes_left = l_end.raw - l_it.raw;
            prim_memcpy(S_at((temp)[temp_cursor]), l_it.raw, bytes_left);
            blk_return bytes_left;
        });
    }
    if (r_it.raw < r_end.raw) {
        temp_cursor += blk({
            let bytes_right = r_end.raw - r_it.raw;
            prim_memcpy(S_at((temp)[temp_cursor]), r_it.raw, bytes_right);
            blk_return bytes_right;
        });
    }

    /* Copy merged elements back to the original array */
    let total_bytes = temp_cursor;
    prim_memcpy(base.ptr, temp.ptr, total_bytes);
    return_ok(prefixS(temp, total_bytes));
} $unscoped_(fn);

fn_((sort_stable(mem_Allocator gpa, u_S$raw base, sort_OrdFn ordFn))(mem_Err$void) $guard) {
    let checked_size = unwrap_(usize_mulChkd(base.len, base.type.size));
    T_use_E$($set(mem_Err)(S$u8));
    let temp = try_(u_castE$((mem_Err$S$u8)(mem_Allocator_alloc(gpa, typeInfo$(u8), checked_size))));
    defer_(mem_Allocator_free(gpa, u_anyS(temp)));
    let_ignore = try_(sort_mergeTempRecur(temp, base, ordFn));
    return_ok({});
} $unguarded_(fn);

fn_((sort_stableCtx(mem_Allocator gpa, u_S$raw base, sort_OrdCtxFn ordFn, u_P_const$raw ctx))(mem_Err$void) $guard) {
    let checked_size = unwrap_(usize_mulChkd(base.len, base.type.size));
    T_use_E$($set(mem_Err)(S$u8));
    let temp = try_(u_castE$((mem_Err$S$u8)(mem_Allocator_alloc(gpa, typeInfo$(u8), checked_size))));
    defer_(mem_Allocator_free(gpa, u_anyS(temp)));
    let_ignore = try_(sort_mergeCtxTempRecur(temp, base, ordFn, ctx));
    return_ok({});
} $unguarded_(fn);

fn_((sort_stableTemp(S$u8 temp, u_S$raw base, sort_OrdFn ordFn))(sort_mem_Err$S$u8) $scope) {
    let checked_size = unwrap_(usize_mulChkd(base.len, base.type.size));
    if (temp.len < checked_size) { return_err(mem_Err_OutOfMemory()); }
    return_ok(try_(sort_mergeTempRecur(temp, base, ordFn)));
} $unscoped_(fn);

fn_((sort_stableCtxTemp(S$u8 temp, u_S$raw base, sort_OrdCtxFn ordFn, u_P_const$raw ctx))(sort_mem_Err$S$u8) $scope) {
    let checked_size = unwrap_(usize_mulChkd(base.len, base.type.size));
    if (temp.len < checked_size) { return_err(mem_Err_OutOfMemory()); }
    return_ok(try_(sort_mergeCtxTempRecur(temp, base, ordFn, ctx)));
} $unscoped_(fn);

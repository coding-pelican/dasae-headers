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

$attr($inline_always)
$static fn_((sort__lt(sort_OrdFn ordFn, u_P_const$raw lhs, u_P_const$raw rhs))(cmp_Ord)) {
    return cmp_Ord_isLt(sort__ord(ordFn, lhs, rhs));
};
$attr($inline_always)
$static fn_((sort__ltCtx(sort_OrdCtxFn ordFn, u_P_const$raw lhs, u_P_const$raw rhs, u_P_const$raw ctx))(cmp_Ord)) {
    return cmp_Ord_isLt(sort__ordCtx(ordFn, lhs, rhs, ctx));
};

fn_((sort_inOrd(u_S_const$raw seq, sort_OrdFn ordFn))(bool)) {
    for (usize i = 1; i < seq.len; ++i) {
        if (sort__lt(ordFn, u_atS(seq, i), u_atS(seq, i - 1))) { return false; }
    }
    return true;
};

fn_((sort_inOrdCtx(u_S_const$raw seq, sort_OrdCtxFn ordFn, u_P_const$raw ctx))(bool)) {
    for (usize i = 1; i < seq.len; ++i) {
        if (sort__ltCtx(ordFn, u_atS(seq, i), u_atS(seq, i - 1), ctx)) { return false; }
    }
    return true;
};

fn_((sort_insert(u_S$raw seq, sort_OrdFn ordFn))(void)) {
    for (usize unsorted_idx = 1; unsorted_idx < seq.len; ++unsorted_idx) {
        var curr_ptr = u_atS(seq, unsorted_idx);
        var sorted_bwd_idx = unsorted_idx;
        while (0 < sorted_bwd_idx) {
            let prev_ptr = u_atS(seq, sorted_bwd_idx - 1);
            if (sort__ord(ordFn, prev_ptr.as_const, curr_ptr.as_const) <= cmp_Ord_eq) { break; }
            mem_swap(u_prefixP(prev_ptr, 1), u_prefixP(curr_ptr, 1));
            curr_ptr = prev_ptr;
            sorted_bwd_idx--;
        }
    }
};

fn_((sort_insertCtx(u_S$raw seq, sort_OrdCtxFn ordFn, u_P_const$raw ctx))(void)) {
    for (usize unsorted_idx = 1; unsorted_idx < seq.len; ++unsorted_idx) {
        var curr_ptr = u_atS(seq, unsorted_idx);
        var sorted_bwd_idx = unsorted_idx;
        while (0 < sorted_bwd_idx) {
            let prev_ptr = u_atS(seq, sorted_bwd_idx - 1);
            if (sort__ordCtx(ordFn, prev_ptr.as_const, curr_ptr.as_const, ctx) <= cmp_Ord_eq) { break; }
            mem_swap(u_prefixP(prev_ptr, 1), u_prefixP(curr_ptr, 1));
            curr_ptr = prev_ptr;
            sorted_bwd_idx--;
        }
    }
};

// NOLINTNEXTLINE(misc-no-recursion)
fn_((sort_mergeTmpRecur(
    S$u8 tmp,
    u_S$raw seq,
    sort_OrdFn ordFn
))(sort_mem_Err$S$u8) $scope) {
    if (seq.len <= sort_stable_sort_threshold_merge_to_insert) {
        sort_insert(seq, ordFn);
        return_ok({});
    }
    /* Sort each half recursively */
    let mid_idx = seq.len / 2;
    try_(sort_mergeTmpRecur(tmp, u_prefixS(seq, mid_idx), ordFn));
    try_(sort_mergeTmpRecur(tmp, u_suffixS(seq, mid_idx), ordFn));

    /* Check if merging is necessary */ {
        let l_last = u_atS(seq, mid_idx - 1);
        let r_first = u_atS(seq, mid_idx);
        if (sort__ord(ordFn, l_last.as_const, r_first.as_const) <= cmp_Ord_eq) {
            return_ok({}); /* Already ordered, no merge needed */
        }
    }

    /* Merge the sorted halves using the temporary buffer */
    var l_it = u_atS(seq, 0);
    let l_end = u_atS(seq, mid_idx);
    var r_it = l_end;
    let r_end = u_atS(seq, seq.len);
    var_(temp_cursor, usize) = 0;
    while (l_it.raw < l_end.raw && r_it.raw < r_end.raw) {
        if (sort__ord(ordFn, l_it.as_const, r_it.as_const) <= cmp_Ord_eq) {
            prim_memcpy(S_at((tmp)[temp_cursor]), l_it.raw, seq.type.size);
            l_it.raw += seq.type.size;
        } else {
            prim_memcpy(S_at((tmp)[temp_cursor]), r_it.raw, seq.type.size);
            r_it.raw += seq.type.size;
        }
        temp_cursor += seq.type.size;
    }

    /* Copy remaining elements */
    if (l_it.raw < l_end.raw) {
        temp_cursor += blk({
            let bytes_left = l_end.raw - l_it.raw;
            prim_memcpy(S_at((tmp)[temp_cursor]), l_it.raw, bytes_left);
            blk_return_(bytes_left);
        });
    }
    if (r_it.raw < r_end.raw) {
        temp_cursor += blk({
            let bytes_right = r_end.raw - r_it.raw;
            prim_memcpy(S_at((tmp)[temp_cursor]), r_it.raw, bytes_right);
            blk_return_(bytes_right);
        });
    }

    /* Copy merged elements back to the original array */
    let total_bytes = temp_cursor;
    prim_memcpy(seq.ptr, tmp.ptr, total_bytes);
    return_ok(prefixS(tmp, total_bytes));
} $unscoped_(fn);

// NOLINTNEXTLINE(misc-no-recursion)
fn_((sort_mergeCtxTmpRecur(
    S$u8 tmp,
    u_S$raw seq,
    sort_OrdCtxFn ordFn,
    u_P_const$raw ctx
))(sort_mem_Err$S$u8) $scope) {
    if (seq.len <= sort_stable_sort_threshold_merge_to_insert) {
        sort_insertCtx(seq, ordFn, ctx);
        return_ok({});
    }
    /* Sort each half recursively */
    let mid_idx = seq.len / 2;
    try_(sort_mergeCtxTmpRecur(tmp, u_prefixS(seq, mid_idx), ordFn, ctx));
    try_(sort_mergeCtxTmpRecur(tmp, u_suffixS(seq, mid_idx), ordFn, ctx));

    /* Check if merging is necessary */ {
        let l_last = u_atS(seq, mid_idx - 1);
        let r_first = u_atS(seq, mid_idx);
        if (sort__ordCtx(ordFn, l_last.as_const, r_first.as_const, ctx) <= cmp_Ord_eq) {
            return_ok({}); /* Already ordered, no merge needed */
        }
    }

    /* Merge the sorted halves using the temporary buffer */
    var l_it = u_atS(seq, 0);
    let l_end = u_atS(seq, mid_idx);
    var r_it = l_end;
    let r_end = u_atS(seq, seq.len);
    var_(temp_cursor, usize) = 0;
    while (l_it.raw < l_end.raw && r_it.raw < r_end.raw) {
        if (sort__ordCtx(ordFn, l_it.as_const, r_it.as_const, ctx) <= cmp_Ord_eq) {
            prim_memcpy(S_at((tmp)[temp_cursor]), l_it.raw, seq.type.size);
            l_it.raw += seq.type.size;
        } else {
            prim_memcpy(S_at((tmp)[temp_cursor]), r_it.raw, seq.type.size);
            r_it.raw += seq.type.size;
        }
        temp_cursor += seq.type.size;
    }

    /* Copy remaining elements */
    if (l_it.raw < l_end.raw) {
        temp_cursor += blk({
            let bytes_left = l_end.raw - l_it.raw;
            prim_memcpy(S_at((tmp)[temp_cursor]), l_it.raw, bytes_left);
            blk_return_(bytes_left);
        });
    }
    if (r_it.raw < r_end.raw) {
        temp_cursor += blk({
            let bytes_right = r_end.raw - r_it.raw;
            prim_memcpy(S_at((tmp)[temp_cursor]), r_it.raw, bytes_right);
            blk_return_(bytes_right);
        });
    }

    /* Copy merged elements back to the original array */
    let total_bytes = temp_cursor;
    prim_memcpy(seq.ptr, tmp.ptr, total_bytes);
    return_ok(prefixS(tmp, total_bytes));
} $unscoped_(fn);

fn_((sort_stable(mem_Allocator gpa, u_S$raw seq, sort_OrdFn ordFn))(mem_Err$void) $guard) {
    let tmp_len_required = unwrap_(usize_mulChkd(seq.len, seq.type.size));
    T_use_E$($set(mem_Err)(S$u8));
    let tmp = try_(u_castE$((mem_Err$S$u8)(mem_Allocator_alloc(gpa, typeInfo$(u8), tmp_len_required))));
    defer_(mem_Allocator_free(gpa, u_anyS(tmp)));
    let_ignore = try_(sort_mergeTmpRecur(tmp, seq, ordFn));
    return_ok({});
} $unguarded_(fn);

fn_((sort_stableCtx(mem_Allocator gpa, u_S$raw seq, sort_OrdCtxFn ordFn, u_P_const$raw ctx))(mem_Err$void) $guard) {
    let tmp_len_required = unwrap_(usize_mulChkd(seq.len, seq.type.size));
    T_use_E$($set(mem_Err)(S$u8));
    let tmp = try_(u_castE$((mem_Err$S$u8)(mem_Allocator_alloc(gpa, typeInfo$(u8), tmp_len_required))));
    defer_(mem_Allocator_free(gpa, u_anyS(tmp)));
    let_ignore = try_(sort_mergeCtxTmpRecur(tmp, seq, ordFn, ctx));
    return_ok({});
} $unguarded_(fn);

fn_((sort_stableTmp(S$u8 tmp, u_S$raw seq, sort_OrdFn ordFn))(sort_mem_Err$S$u8) $scope) {
    let tmp_len_required = unwrap_(usize_mulChkd(seq.len, seq.type.size));
    if (tmp.len < tmp_len_required) { return_err(mem_Err_OutOfMemory()); }
    return_ok(try_(sort_mergeTmpRecur(tmp, seq, ordFn)));
} $unscoped_(fn);

fn_((sort_stableCtxTmp(S$u8 tmp, u_S$raw seq, sort_OrdCtxFn ordFn, u_P_const$raw ctx))(sort_mem_Err$S$u8) $scope) {
    let tmp_len_required = unwrap_(usize_mulChkd(seq.len, seq.type.size));
    if (tmp.len < tmp_len_required) { return_err(mem_Err_OutOfMemory()); }
    return_ok(try_(sort_mergeCtxTmpRecur(tmp, seq, ordFn, ctx)));
} $unscoped_(fn);

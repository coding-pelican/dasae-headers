#include "dh/sort.h"
#include "dh/mem/cfg.h"

$inline_always
fn_((mem_swapBytes(S$u8 lhs, S$u8 rhs))(void)) {
    debug_assert_true(lhs.len == rhs.len);
    let tmp_len = lhs.len;
    let tmp_ptr = as$((u8*)(alloca(tmp_len)));
    prim_memcpy(tmp_ptr, lhs.ptr, lhs.len);
    prim_memcpy(lhs.ptr, rhs.ptr, rhs.len);
    prim_memcpy(rhs.ptr, tmp_ptr, tmp_len);
}

/// Swap two elements of given size
$inline_always
fn_((sort_swapBytes(u8* const lhs, u8* const rhs, usize byte_len))(void)) {
    let tmp = as$((u8*)(alloca(byte_len)));
    prim_memcpy(tmp, lhs, byte_len);
    prim_memcpy(lhs, rhs, byte_len);
    prim_memcpy(rhs, tmp, byte_len);
}

fn_((sort_insertionSort(
    u_S$raw base_sli,
    sort_CmpFn cmpFn
))(void)) {
    let ptr = as$((u8*)(base_sli.ptr));
    let len = base_sli.len;
    usize size = base_sli.type.size;
    for (usize unsorted_index = 1; unsorted_index < len; ++unsorted_index) {
        var curr = ptr + (unsorted_index * size);
        var sorted_backward_index = unsorted_index;
        while (0 < sorted_backward_index) {
            var prev = curr - size;
            if (invoke(cmpFn, prev, curr) <= cmp_Ord_eq) { break; }
            sort_swapBytes(prev, curr, size);
            curr = prev;
            sorted_backward_index--;
        }
    }
}

fn_((sort_insertionSortWithArg(
    u_S$raw base_sli,
    sort_CmpWithArgFn cmpFn,
    P_const$raw arg
))(void)) {
    let ptr = as$((u8*)(base_sli.ptr));
    let len = base_sli.len;
    usize size = base_sli.type.size;
    for (usize unsorted_index = 1; unsorted_index < len; ++unsorted_index) {
        var current = ptr + (unsorted_index * size);
        var sorted_backward_index = unsorted_index;
        while (0 < sorted_backward_index) {
            var prev = current - size;
            if (invoke(cmpFn, prev, current, arg) <= cmp_Ord_eq) { break; }
            sort_swapBytes(prev, current, size);
            current = prev;
            sorted_backward_index--;
        }
    }
}

// TODO: 와 큰일 날 뻔... 직접적인 u_S$raw 생성은 금지해야겠다. meta_S_slice 함수 작성이 시급하다.
// NOLINTNEXTLINE(misc-no-recursion)
fn_((sort_mergeSortUsingTempRecur(
    S$u8 temp_buf,
    u_S$raw base_sli,
    sort_CmpFn cmpFn
))(mem_Err$void) $scope) {
    if (base_sli.len <= sort_stableSort_threshold_merge_to_insertion) {
        sort_insertionSort(base_sli, cmpFn);
        return_ok({});
    }
    let base_type = base_sli.type;
    let base_bytes = as$((u8*)(base_sli.ptr));
    let base_len = base_sli.len;
    let mid_idx = base_len / 2;

    /* Sort each half recursively */
    try_(sort_mergeSortUsingTempRecur(
        temp_buf,
        make$((u_S$raw){
            .type = base_type,
            .ptr = base_bytes,
            .len = mid_idx,
        }),
        cmpFn
    ));
    try_(sort_mergeSortUsingTempRecur(
        temp_buf,
        make$((u_S$raw){
            .type = base_type,
            .ptr = base_bytes + (mid_idx * base_type.size),
            .len = base_len - mid_idx,
        }),
        cmpFn
    ));

    /* Check if merging is necessary */ {
        let left_last = base_bytes + ((mid_idx - 1) * base_type.size);
        let right_first = base_bytes + (mid_idx * base_type.size);
        if (invoke(cmpFn, left_last, right_first) <= cmp_Ord_eq) {
            return_ok({}); /* Already ordered, no merge needed */
        }
    }

    /* Merge the sorted halves using the temporary buffer */
    var left_ptr = base_bytes;
    var left_end = left_ptr + mid_idx * base_type.size;
    var right_ptr = base_bytes + mid_idx * base_type.size;
    var right_end = base_bytes + base_len * base_type.size;
    var temp_ptr = temp_buf.ptr;

    while (left_ptr < left_end && right_ptr < right_end) {
        if (invoke(cmpFn, left_ptr, right_ptr) <= cmp_Ord_eq) {
            prim_memcpy(temp_ptr, left_ptr, base_type.size);
            left_ptr += base_type.size;
        } else {
            prim_memcpy(temp_ptr, right_ptr, base_type.size);
            right_ptr += base_type.size;
        }
        temp_ptr += base_type.size;
    }

    /* Copy remaining elements */
    if (left_ptr < left_end) {
        temp_ptr += blk({
            let bytes_left = left_end - left_ptr;
            prim_memcpy(temp_ptr, left_ptr, bytes_left);
            blk_return bytes_left;
        });
    }
    if (right_ptr < right_end) {
        temp_ptr += blk({
            let bytes_right = right_end - right_ptr;
            prim_memcpy(temp_ptr, right_ptr, bytes_right);
            blk_return bytes_right;
        });
    }

    /* Copy merged elements back to the original array */
    let total_bytes = temp_ptr - temp_buf.ptr;
    prim_memcpy(base_bytes, temp_buf.ptr, total_bytes);
    return_ok({});
} $unscoped_(fn);

// NOLINTNEXTLINE(misc-no-recursion)
fn_((sort_mergeSortWithArgUsingTempRecur(
    S$u8 temp_buf,
    u_S$raw base_sli,
    sort_CmpWithArgFn cmpFn,
    P_const$raw arg
))(mem_Err$void) $scope) {
    if (base_sli.len <= sort_stableSort_threshold_merge_to_insertion) {
        sort_insertionSortWithArg(base_sli, cmpFn, arg);
        return_ok({});
    }
    let base_type = base_sli.type;
    let base_bytes = as$((u8*)(base_sli.ptr));
    let base_len = base_sli.len;
    let mid_idx = base_len / 2;

    /* Sort each half recursively */
    try_(sort_mergeSortWithArgUsingTempRecur(
        temp_buf,
        make$((u_S$raw){
            .type = base_type,
            .ptr = base_bytes,
            .len = mid_idx,
        }),
        cmpFn,
        arg
    ));
    try_(sort_mergeSortWithArgUsingTempRecur(
        temp_buf,
        make$((u_S$raw){
            .type = base_type,
            .ptr = base_bytes + (mid_idx * base_type.size),
            .len = base_len - mid_idx,
        }),
        cmpFn,
        arg
    ));

    /* Check if merging is necessary */ {
        let left_last = base_bytes + ((mid_idx - 1) * base_type.size);
        let right_first = base_bytes + (mid_idx * base_type.size);
        if (invoke(cmpFn, left_last, right_first, arg) <= cmp_Ord_eq) {
            return_ok({}); /* Already ordered, no merge needed */
        }
    }

    /* Merge the sorted halves using the temporary buffer */
    var left_ptr = base_bytes;
    var left_end = left_ptr + (mid_idx * base_type.size);
    var right_ptr = base_bytes + (mid_idx * base_type.size);
    var right_end = base_bytes + (base_len * base_type.size);
    var temp_ptr = temp_buf.ptr;

    while (left_ptr < left_end && right_ptr < right_end) {
        if (invoke(cmpFn, left_ptr, right_ptr, arg) <= cmp_Ord_eq) {
            prim_memcpy(temp_ptr, left_ptr, base_type.size);
            left_ptr += base_type.size;
        } else {
            prim_memcpy(temp_ptr, right_ptr, base_type.size);
            right_ptr += base_type.size;
        }
        temp_ptr += base_type.size;
    }

    /* Copy remaining elements */
    if (left_ptr < left_end) {
        temp_ptr += blk({
            let bytes_left = left_end - left_ptr;
            prim_memcpy(temp_ptr, left_ptr, bytes_left);
            blk_return bytes_left;
        });
    }
    if (right_ptr < right_end) {
        temp_ptr += blk({
            let bytes_right = right_end - right_ptr;
            prim_memcpy(temp_ptr, right_ptr, bytes_right);
            blk_return bytes_right;
        });
    }

    /* Copy merged elements back to the original array */
    let total_bytes = temp_ptr - temp_buf.ptr;
    prim_memcpy(base_bytes, temp_buf.ptr, total_bytes);
    return_ok({});
} $unscoped_(fn);

fn_((sort_stableSort(
    mem_Allocator allocator,
    u_S$raw base_sli,
    sort_CmpFn cmpFn
))(mem_Err$void) $guard) {
    let checked_size = unwrap_(usize_mulChkd(base_sli.len, base_sli.type.size));
    T_use_E$($set(mem_Err)(S$u8));
    let temp_buf = try_(u_castE$((mem_Err$S$u8)(mem_Allocator_alloc(allocator, typeInfo$(u8), checked_size))));
    defer_(mem_Allocator_free(allocator, u_anyS(temp_buf)));
    return_ok(try_(sort_mergeSortUsingTempRecur(temp_buf, base_sli, cmpFn)));
} $unguarded_(fn);

fn_((sort_stableSortWithArg(
    mem_Allocator allocator,
    u_S$raw base_sli,
    sort_CmpWithArgFn cmpFn,
    P_const$raw arg
))(mem_Err$void) $guard) {
    let checked_size = unwrap_(usize_mulChkd(base_sli.len, base_sli.type.size));
    T_use_E$($set(mem_Err)(S$u8));
    let temp_buf = try_(u_castE$((mem_Err$S$u8)(mem_Allocator_alloc(allocator, typeInfo$(u8), checked_size))));
    defer_(mem_Allocator_free(allocator, u_anyS(temp_buf)));
    return_ok(try_(sort_mergeSortWithArgUsingTempRecur(temp_buf, base_sli, cmpFn, arg)));
} $unguarded_(fn);

fn_((sort_stableSortUsingTemp(
    S$u8 temp_buf,
    u_S$raw base_sli,
    sort_CmpFn cmpFn
))(mem_Err$void) $scope) {
    let checked_size = unwrap_(usize_mulChkd(base_sli.len, base_sli.type.size));
    if (temp_buf.len < checked_size) {
        return_err(mem_Err_OutOfMemory());
    }
    return_ok(try_(sort_mergeSortUsingTempRecur(temp_buf, base_sli, cmpFn)));
} $unscoped_(fn);

fn_((sort_stableSortWithArgUsingTemp(
    S$u8 temp_buf,
    u_S$raw base_sli,
    sort_CmpWithArgFn cmpFn,
    P_const$raw arg
))(mem_Err$void) $scope) {
    let checked_size = unwrap_(usize_mulChkd(base_sli.len, base_sli.type.size));
    if (temp_buf.len < checked_size) {
        return_err(mem_Err_OutOfMemory());
    }
    return_ok(try_(sort_mergeSortWithArgUsingTempRecur(temp_buf, base_sli, cmpFn, arg)));
} $unscoped_(fn);

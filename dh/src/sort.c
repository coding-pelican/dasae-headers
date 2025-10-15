#include "dh/sort.h"
#include "dh/int.h"
#include "dh/mem/cfg.h"

$inline_always
fn_(mem_swapBytes(Sli$u8 lhs, Sli$u8 rhs), void) {
    debug_assert_true(lhs.len == rhs.len);
    let tmp_len = lhs.len;
    let tmp_ptr = as$(u8*, bti_alloca(tmp_len));
    memcpy(tmp_ptr, lhs.ptr, lhs.len);
    memcpy(lhs.ptr, rhs.ptr, rhs.len);
    memcpy(rhs.ptr, tmp_ptr, tmp_len);
}

/// Swap two elements of given size
$inline_always
fn_(sort_swapBytes(u8* const lhs, u8* const rhs, usize byte_len), void) {
    let tmp = as$(u8*, bti_alloca(byte_len));
    memcpy(tmp, lhs, byte_len);
    memcpy(lhs, rhs, byte_len);
    memcpy(rhs, tmp, byte_len);
}

fn_(sort_insertionSort(
        meta_Sli   base_sli,
        sort_CmpFn cmpFn
    ),
    void) {
    let ptr  = as$(u8*, base_sli.addr);
    let len  = base_sli.len;
    let size = base_sli.type.size;
    for (usize unsorted_index = 1; unsorted_index < len; ++unsorted_index) {
        var curr                  = ptr + (unsorted_index * size);
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

fn_(sort_insertionSortWithArg(
        meta_Sli          base_sli,
        sort_CmpWithArgFn cmpFn,
        anyptr_const      arg
    ),
    void) {
    let ptr  = as$(u8*, base_sli.addr);
    let len  = base_sli.len;
    let size = base_sli.type.size;
    for (usize unsorted_index = 1; unsorted_index < len; ++unsorted_index) {
        var current               = ptr + (unsorted_index * size);
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

// TODO: 와 큰일 날 뻔... 직접적인 meta_Sli 생성은 금지해야겠다. meta_Sli_slice 함수 작성이 시급하다.
// NOLINTNEXTLINE(misc-no-recursion)
fn_(sort_mergeSortUsingTempRecur(
        Sli$u8     temp_buf,
        meta_Sli   base_sli,
        sort_CmpFn cmpFn
    ),
    Err$void $scope) {
    if (base_sli.len <= sort_stableSort_threshold_merge_to_insertion) {
        sort_insertionSort(base_sli, cmpFn);
        return_ok({});
    }
    let base_type  = base_sli.type;
    let base_bytes = as$(u8*, base_sli.addr);
    let base_len   = base_sli.len;
    let base_size  = base_sli.type.size;
    let mid_idx    = base_len / 2;

    /* Sort each half recursively */
    try_(sort_mergeSortUsingTempRecur(
        temp_buf,
        make$(meta_Sli, .type = base_type, .addr = base_bytes, .len = mid_idx),
        cmpFn
    ));
    try_(sort_mergeSortUsingTempRecur(
        temp_buf,
        make$(meta_Sli, .type = base_type, .addr = base_bytes + (mid_idx * base_size), .len = base_len - mid_idx),
        cmpFn
    ));

    /* Check if merging is necessary */ {
        let left_last   = base_bytes + ((mid_idx - 1) * base_size);
        let right_first = base_bytes + (mid_idx * base_size);
        if (invoke(cmpFn, left_last, right_first) <= cmp_Ord_eq) {
            return_ok({}); /* Already ordered, no merge needed */
        }
    }

    /* Merge the sorted halves using the temporary buffer */
    var left_ptr  = base_bytes;
    var left_end  = left_ptr + mid_idx * base_size;
    var right_ptr = base_bytes + mid_idx * base_size;
    var right_end = base_bytes + base_len * base_size;
    var temp_ptr  = temp_buf.ptr;

    while (left_ptr < left_end && right_ptr < right_end) {
        if (invoke(cmpFn, left_ptr, right_ptr) <= cmp_Ord_eq) {
            memcpy(temp_ptr, left_ptr, base_size);
            left_ptr += base_size;
        } else {
            memcpy(temp_ptr, right_ptr, base_size);
            right_ptr += base_size;
        }
        temp_ptr += base_size;
    }

    /* Copy remaining elements */
    if (left_ptr < left_end) {
        temp_ptr += eval({
            let bytes_left = left_end - left_ptr;
            memcpy(temp_ptr, left_ptr, bytes_left);
            eval_return bytes_left;
        });
    }
    if (right_ptr < right_end) {
        temp_ptr += eval({
            let bytes_right = right_end - right_ptr;
            memcpy(temp_ptr, right_ptr, bytes_right);
            eval_return bytes_right;
        });
    }

    /* Copy merged elements back to the original array */
    let total_bytes = temp_ptr - temp_buf.ptr;
    memcpy(base_bytes, temp_buf.ptr, total_bytes);
    return_ok({});
} $unscoped;

// NOLINTNEXTLINE(misc-no-recursion)
fn_(sort_mergeSortWithArgUsingTempRecur(
        Sli$u8            temp_buf,
        meta_Sli          base_sli,
        sort_CmpWithArgFn cmpFn,
        anyptr_const      arg
    ),
    Err$void $scope) {
    if (base_sli.len <= sort_stableSort_threshold_merge_to_insertion) {
        sort_insertionSortWithArg(base_sli, cmpFn, arg);
        return_ok({});
    }
    let base_type  = base_sli.type;
    let base_bytes = as$(u8*, base_sli.addr);
    let base_len   = base_sli.len;
    let base_size  = base_sli.type.size;
    let mid_idx    = base_len / 2;

    /* Sort each half recursively */
    try_(sort_mergeSortWithArgUsingTempRecur(
        temp_buf,
        make$(meta_Sli, .type = base_type, .addr = base_bytes, .len = mid_idx),
        cmpFn,
        arg
    ));
    try_(sort_mergeSortWithArgUsingTempRecur(
        temp_buf,
        make$(meta_Sli, .type = base_type, .addr = base_bytes + (mid_idx * base_size), .len = base_len - mid_idx),
        cmpFn,
        arg
    ));

    /* Check if merging is necessary */ {
        let left_last   = base_bytes + ((mid_idx - 1) * base_size);
        let right_first = base_bytes + (mid_idx * base_size);
        if (invoke(cmpFn, left_last, right_first, arg) <= cmp_Ord_eq) {
            return_ok({}); /* Already ordered, no merge needed */
        }
    }

    /* Merge the sorted halves using the temporary buffer */
    var left_ptr  = base_bytes;
    var left_end  = left_ptr + (mid_idx * base_size);
    var right_ptr = base_bytes + (mid_idx * base_size);
    var right_end = base_bytes + (base_len * base_size);
    var temp_ptr  = temp_buf.ptr;

    while (left_ptr < left_end && right_ptr < right_end) {
        if (invoke(cmpFn, left_ptr, right_ptr, arg) <= cmp_Ord_eq) {
            memcpy(temp_ptr, left_ptr, base_size);
            left_ptr += base_size;
        } else {
            memcpy(temp_ptr, right_ptr, base_size);
            right_ptr += base_size;
        }
        temp_ptr += base_size;
    }

    /* Copy remaining elements */
    if (left_ptr < left_end) {
        temp_ptr += eval({
            let bytes_left = left_end - left_ptr;
            memcpy(temp_ptr, left_ptr, bytes_left);
            eval_return bytes_left;
        });
    }
    if (right_ptr < right_end) {
        temp_ptr += eval({
            let bytes_right = right_end - right_ptr;
            memcpy(temp_ptr, right_ptr, bytes_right);
            eval_return bytes_right;
        });
    }

    /* Copy merged elements back to the original array */
    let total_bytes = temp_ptr - temp_buf.ptr;
    memcpy(base_bytes, temp_buf.ptr, total_bytes);
    return_ok({});
} $unscoped;

fn_(sort_stableSort(
        mem_Allocator allocator,
        meta_Sli      base_sli,
        sort_CmpFn    cmpFn
    ),
    Err$void $guard) {
    let checked_size = unwrap(usize_chkdMul(base_sli.len, base_sli.type.size));
    let temp_buf     = meta_cast$(Sli$u8, try_(mem_Allocator_alloc(allocator, typeInfo$(u8), checked_size)));
    defer_(mem_Allocator_free(allocator, anySli(temp_buf)));
    try_(sort_mergeSortUsingTempRecur(temp_buf, base_sli, cmpFn));
    return_ok({});
} $unguarded;

fn_(sort_stableSortWithArg(
        mem_Allocator     allocator,
        meta_Sli          base_sli,
        sort_CmpWithArgFn cmpFn,
        anyptr_const      arg
    ),
    Err$void $guard) {
    let checked_size = unwrap(usize_chkdMul(base_sli.len, base_sli.type.size));
    let temp_buf     = meta_cast$(Sli$u8, try_(mem_Allocator_alloc(allocator, typeInfo$(u8), checked_size)));
    defer_(mem_Allocator_free(allocator, anySli(temp_buf)));
    try_(sort_mergeSortWithArgUsingTempRecur(temp_buf, base_sli, cmpFn, arg));
    return_ok({});
} $unguarded;

fn_(sort_stableSortUsingTemp(
        Sli$u8     temp_buf,
        meta_Sli   base_sli,
        sort_CmpFn cmpFn
    ),
    Err$void $scope) {
    let checked_size = unwrap(usize_chkdMul(base_sli.len, base_sli.type.size));
    if (temp_buf.len < checked_size) {
        return_err(mem_Allocator_Err_OutOfMemory());
    }
    try_(sort_mergeSortUsingTempRecur(temp_buf, base_sli, cmpFn));
    return_ok({});
} $unscoped;

fn_(sort_stableSortWithArgUsingTemp(
        Sli$u8            temp_buf,
        meta_Sli          base_sli,
        sort_CmpWithArgFn cmpFn,
        anyptr_const      arg
    ),
    Err$void $scope) {
    let checked_size = unwrap(usize_chkdMul(base_sli.len, base_sli.type.size));
    if (temp_buf.len < checked_size) {
        return_err(mem_Allocator_Err_OutOfMemory());
    }
    try_(sort_mergeSortWithArgUsingTempRecur(temp_buf, base_sli, cmpFn, arg));
    return_ok({});
} $unscoped;

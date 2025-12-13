/**
 * @copyright Copyright (c) 2025 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    sort.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2025-01-22 (date of creation)
 * @updated 2025-03-09 (date of last update)
 * @version v0.1-alpha.1
 * @ingroup dasae-headers(dh)
 * @prefix  sort
 *
 * @brief   Sorting utilities
 * @details Provides sorting functions for arrays and slices.
 *          Includes insertion sort, merge sort, and stable sort.
 *          Supports custom comparison functions and optional memory allocation.
 */
#ifndef sort__included
#define sort__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "prl.h"
#include "cmp.h"
#include "mem/Allocator.h"

/*========== Macros and Declarations ========================================*/

#define sort_stable_sort_threshold_merge_to_insert (32)
typedef E$$($set(mem_Err)(S$u8)) sort_mem_Err$S$u8;

/// Callable types for sorting functions
use_Callable(sort_OrdFn, (u_V$raw lhs, u_V$raw rhs), cmp_Ord);
/// Callable types for sorting functions that take an argument
use_Callable(sort_OrdCtxFn, (u_V$raw lhs, u_V$raw rhs, u_V$raw ctx), cmp_Ord);

/// Checks if the sequence is ordered in ascending order according to `ordFn`.
/// - Returns `true` if ordered, `false` otherwise.
/// - Time Complexity: `O(n)`
$attr($must_check)
$extern fn_((sort_inOrd(u_S_const$raw seq, sort_OrdFn ordFn))(bool));
$attr($must_check)
$extern fn_((sort_inOrdCtx(u_S_const$raw seq, sort_OrdCtxFn ordFn, u_P_const$raw ctx))(bool));

/// Insertion sort for small arrays
/// - Time Complexity: `O(n^2)`
/// - Space Complexity: `O(1)`
$extern fn_((sort_insert(u_S$raw seq, sort_OrdFn ordFn))(void));
$extern fn_((sort_insertCtx(u_S$raw seq, sort_OrdCtxFn ordFn, u_P_const$raw ctx))(void));

/* TODO: Improve better interface for sorting functions */
/// Merge sort using temporary buffer instead of allocating new memory
/// - Time Complexity: `O(n log n)`
/// - Space Complexity: `O(n)`
$attr($must_check)
$extern fn_((sort_mergeTmpRecur(
    S$u8 tmp,
    u_S$raw seq,
    sort_OrdFn ordFn
))(sort_mem_Err$S$u8));
$attr($must_check)
$extern fn_((sort_mergeTmpCtxRecur(
    S$u8 tmp,
    u_S$raw seq,
    sort_OrdCtxFn ordFn,
    u_P_const$raw ctx
))(sort_mem_Err$S$u8));

/// Stable sort (based merge sort with insertion sort)
/// - Time Complexity: `O(n log n)`
/// - Space Complexity: `O(n)`
$attr($must_check)
$extern fn_((sort_stable(mem_Allocator gpa, u_S$raw seq, sort_OrdFn ordFn))(mem_Err$void));
$attr($must_check)
$extern fn_((sort_stableCtx(mem_Allocator gpa, u_S$raw seq, sort_OrdCtxFn ordFn, u_P_const$raw ctx))(mem_Err$void));
/// Stable sort using temporary buffer (based merge sort with insertion sort)
/// - Time Complexity: `O(n log n)`
/// - Space Complexity: `O(n)`
$attr($must_check)
$extern fn_((sort_stableTmp(S$u8 tmp, u_S$raw seq, sort_OrdFn ordFn))(sort_mem_Err$S$u8));
$attr($must_check)
$extern fn_((sort_stableTmpCtx(S$u8 tmp, u_S$raw seq, sort_OrdCtxFn ordFn, u_P_const$raw ctx))(sort_mem_Err$S$u8));

#if defined(__cplusplus)
} /* $extern "C" */
#endif /* defined(__cplusplus) */
#endif /* sort__included */

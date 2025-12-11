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
 *          Includes insertion sort, merge sort, and stable sort
 * implementations. Supports custom comparison functions and optional memory
 * allocation.
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

/*========== Macros and Definitions =========================================*/

#define sort_stable_sort_threshold_merge_to_insert (32)

/// Callable types for sorting functions
use_Callable(sort_OrdFn, (u_V$raw lhs, u_V$raw rhs), cmp_Ord);
/// Callable types for sorting functions that take an argument
use_Callable(sort_OrdCtxFn, (u_V$raw lhs, u_V$raw rhs, u_V$raw ctx), cmp_Ord);

/*========== Function Prototypes ============================================*/

typedef E$$($set(mem_Err)(S$u8)) sort_mem_Err$S$u8;

/// Insertion sort for small arrays
$extern fn_((sort_insert(u_S$raw base, sort_OrdFn ordFn))(void));
/// Insertion sort with arg
$extern fn_((sort_insertCtx(u_S$raw base, sort_OrdCtxFn ordFn, u_P_const$raw ctx))(void));
/// Modernized merge sort using temporary buffer instead of allocating new memory
$attr($must_check)
$extern fn_((sort_mergeTempRecur(
    S$u8 temp,
    u_S$raw base,
    sort_OrdFn ordFn
))(sort_mem_Err$S$u8));
/// Modernized merge sort using temporary buffer with arg
$attr($must_check)
$extern fn_((sort_mergeTempCtxRecur(
    S$u8 temp,
    u_S$raw base,
    sort_OrdCtxFn ordFn,
    u_P_const$raw ctx
))(sort_mem_Err$S$u8));
/// Modernized stable sort (using merge sort with insertion sort)
$attr($must_check)
$extern fn_((sort_stable(mem_Allocator gpa, u_S$raw base, sort_OrdFn ordFn))(mem_Err$void));
/// Modernized stable sort with arg (using merge sort with insertion sort)
$attr($must_check)
$extern fn_((sort_stableCtx(mem_Allocator gpa, u_S$raw base, sort_OrdCtxFn ordFn, u_P_const$raw ctx))(mem_Err$void));
/// Modernized stable sort (using merge sort with insertion sort)
$attr($must_check)
$extern fn_((sort_stableTemp(S$u8 temp, u_S$raw base, sort_OrdFn ordFn))(sort_mem_Err$S$u8));
/// Modernized stable sort with arg (using merge sort with insertion sort)
$attr($must_check)
$extern fn_((sort_stableTempCtx(S$u8 temp, u_S$raw base, sort_OrdCtxFn ordFn, u_P_const$raw ctx))(sort_mem_Err$S$u8));

#if defined(__cplusplus)
} /* $extern "C" */
#endif /* defined(__cplusplus) */
#endif /* sort__included */

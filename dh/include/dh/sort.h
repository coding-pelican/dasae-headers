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
#include "mem/Allocator.h"

/*========== Macros and Definitions =========================================*/

#define sort_stableSort_threshold_merge_to_insertion (32)

/// Callable types for sorting functions
use_Callable(sort_CmpFn, (P_const$raw lhs, P_const$raw rhs), cmp_Ord);
/// Callable types for sorting functions that take an argument
use_Callable(sort_CmpWithArgFn, (P_const$raw lhs, P_const$raw rhs, P_const$raw arg), cmp_Ord);

/*========== Function Prototypes ============================================*/

/// Insertion sort for small arrays
$extern fn_((sort_insertionSort(
    u_S$raw base_sli,
    sort_CmpFn cmpFn
))(void));
/// Insertion sort with arg
$extern fn_((sort_insertionSortWithArg(
    u_S$raw base_sli,
    sort_CmpWithArgFn cmpFn,
    P_const$raw arg
))(void));
/// Modernized merge sort using temporary buffer instead of allocating new memory
$attr($must_check)
$extern fn_((sort_mergeSortUsingTempRecur(
    S$u8 temp_buf,
    u_S$raw base_sli,
    sort_CmpFn cmpFn
))(mem_Err$void));
/// Modernized merge sort using temporary buffer with arg
$attr($must_check)
$extern fn_((sort_mergeSortWithArgUsingTempRecur(
    S$u8 temp_buf,
    u_S$raw base_sli,
    sort_CmpWithArgFn cmpFn,
    P_const$raw arg
))(mem_Err$void));
/// Modernized stable sort (using merge sort with insertion sort)
$attr($must_check)
$extern fn_((sort_stableSort(
    mem_Allocator allocator,
    u_S$raw base_sli,
    sort_CmpFn cmpFn
))(mem_Err$void));
/// Modernized stable sort with arg (using merge sort with insertion sort)
$attr($must_check)
$extern fn_((sort_stableSortWithArg(
    mem_Allocator allocator,
    u_S$raw base_sli,
    sort_CmpWithArgFn cmpFn,
    P_const$raw arg
))(mem_Err$void));
/// Modernized stable sort (using merge sort with insertion sort)
$attr($must_check)
$extern fn_((sort_stableSortUsingTemp(
    S$u8 temp_buf,
    u_S$raw base_sli,
    sort_CmpFn cmpFn
))(mem_Err$void));
/// Modernized stable sort with arg (using merge sort with insertion sort)
$attr($must_check)
$extern fn_((sort_stableSortWithArgUsingTemp(
    S$u8 temp_buf,
    u_S$raw base_sli,
    sort_CmpWithArgFn cmpFn,
    P_const$raw arg
))(mem_Err$void));

#if defined(__cplusplus)
} /* $extern "C" */
#endif /* defined(__cplusplus) */
#endif /* sort__included */

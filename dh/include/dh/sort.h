/**
 * @copyright Copyright (c) 2025 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    sort.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
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

#ifndef SORT_INCLUDED
#define SORT_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "dh/prl.h"
#include "dh/mem/Allocator.h"
#include "dh/meta.h"

/*========== Macros and Definitions =========================================*/

#define sort_stableSort_threshold_merge_to_insertion (32)

/// Callback types for sorting functions
use_Callback(sort_CmpFn, (anyptr_const lhs, anyptr_const rhs), cmp_Ord);
/// Callback types for sorting functions that take an argument
use_Callback(sort_CmpWithArgFn, (anyptr_const lhs, anyptr_const rhs, anyptr_const arg), cmp_Ord);

/*========== Function Prototypes ============================================*/

/// Insertion sort for small arrays
$extern
fn_((sort_insertionSort(
    meta_Sli   base_sli,
    sort_CmpFn cmpFn
))(void));
$extern
/// Insertion sort with arg
fn_((sort_insertionSortWithArg(
    meta_Sli          base_sli,
    sort_CmpWithArgFn cmpFn,
    anyptr_const      arg
))(void));
/// Modernized merge sort using temporary buffer instead of allocating new memory
$extern $must_check
fn_((sort_mergeSortUsingTempRecur(
    Sli$u8     temp_buf,
    meta_Sli   base_sli,
    sort_CmpFn cmpFn
))(Err$void));
/// Modernized merge sort using temporary buffer with arg
$extern $must_check
fn_((sort_mergeSortWithArgUsingTempRecur(
    Sli$u8            temp_buf,
    meta_Sli          base_sli,
    sort_CmpWithArgFn cmpFn,
    anyptr_const      arg
))(Err$void));
/// Modernized stable sort (using merge sort with insertion sort)
$extern $must_check
fn_((sort_stableSort(
    mem_Allocator allocator,
    meta_Sli      base_sli,
    sort_CmpFn    cmpFn
))(Err$void));
/// Modernized stable sort with arg (using merge sort with insertion sort)
$extern $must_check
fn_((sort_stableSortWithArg(
    mem_Allocator     allocator,
    meta_Sli          base_sli,
    sort_CmpWithArgFn cmpFn,
    anyptr_const      arg
))(Err$void));
/// Modernized stable sort (using merge sort with insertion sort)
$extern $must_check
fn_((sort_stableSortUsingTemp(
    Sli$u8     temp_buf,
    meta_Sli   base_sli,
    sort_CmpFn cmpFn
))(Err$void));
/// Modernized stable sort with arg (using merge sort with insertion sort)
$extern $must_check
fn_((sort_stableSortWithArgUsingTemp(
    Sli$u8            temp_buf,
    meta_Sli          base_sli,
    sort_CmpWithArgFn cmpFn,
    anyptr_const      arg
))(Err$void));

#if defined(__cplusplus)
} /* $extern "C" */
#endif /* defined(__cplusplus) */
#endif /* SORT_INCLUDED */

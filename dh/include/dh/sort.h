/**
 * @copyright Copyright 2025. Gyeongtae Kim All rights reserved.
 *
 * @file    sort.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2025-01-22 (date of creation)
 * @updated 2025-03-09 (date of last update)
 * @version v0.1-alpha.1
 * @ingroup dasae-headers(dh)
 * @prefix  sort
 */

#ifndef SORT_INCLUDED
#define SORT_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "dh/core.h"
#include "dh/meta.h"
#include "dh/callback.h"
#include "dh/mem/Allocator.h"

/*========== Macros and Definitions =========================================*/

#define sort_stableSort_threshold_merge_to_insertion (32)

/// Callback types for sorting functions
use_Callback(sort_CmpFn, (anyptr_const lhs, anyptr_const rhs), cmp_Ord);
/// Callback types for sorting functions that take an argument
use_Callback(sort_CmpWithArgFn, (anyptr_const lhs, anyptr_const rhs, anyptr_const arg), cmp_Ord);

/*========== Function Prototypes ============================================*/

/// Insertion sort for small arrays
extern fn_(sort_insertionSort(
    meta_Sli   base_sli,
    sort_CmpFn cmpFn
), void);
/// Insertion sort with arg
extern fn_(sort_insertionSortWithArg(
    meta_Sli          base_sli,
    sort_CmpWithArgFn cmpFn,
    anyptr_const      arg
), void);
/// Modernized merge sort using temporary buffer instead of allocating new memory
extern fn_(sort_mergeSortUsingTempRecur(
    Sli$u8     temp_buf,
    meta_Sli   base_sli,
    sort_CmpFn cmpFn
), $must_check Err$void);
/// Modernized merge sort using temporary buffer with arg
extern fn_(sort_mergeSortWithArgUsingTempRecur(
    Sli$u8            temp_buf,
    meta_Sli          base_sli,
    sort_CmpWithArgFn cmpFn,
    anyptr_const      arg
), $must_check Err$void);
/// Modernized stable sort (using merge sort with insertion sort)
extern fn_(sort_stableSort(
    mem_Allocator allocator,
    meta_Sli      base_sli,
    sort_CmpFn    cmpFn
), $must_check Err$void);
/// Modernized stable sort with arg (using merge sort with insertion sort)
extern fn_(sort_stableSortWithArg(
    mem_Allocator     allocator,
    meta_Sli          base_sli,
    sort_CmpWithArgFn cmpFn,
    anyptr_const      arg
), $must_check Err$void);
/// Modernized stable sort (using merge sort with insertion sort)
extern fn_(sort_stableSortUsingTemp(
    Sli$u8     temp_buf,
    meta_Sli   base_sli,
    sort_CmpFn cmpFn
), $must_check Err$void);
/// Modernized stable sort with arg (using merge sort with insertion sort)
extern fn_(sort_stableSortWithArgUsingTemp(
    Sli$u8            temp_buf,
    meta_Sli          base_sli,
    sort_CmpWithArgFn cmpFn,
    anyptr_const      arg
), $must_check Err$void);

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* SORT_INCLUDED */

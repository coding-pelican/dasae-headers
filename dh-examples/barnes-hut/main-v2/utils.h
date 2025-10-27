#ifndef UTILS_INCLUDED
#define UTILS_INCLUDED (1)

#include "dh/core.h"
#include "dh/meta.h"
#include "dh/mem/Allocator.h"
#include "dh/ArrList.h"
#include "Body.h"

// Insertion sort for small arrays
extern void utils_insertionSort(
    meta_S base_slice,
    cmp_Ord (*compareFn)(P_const$raw lhs, P_const$raw rhs)
);

// Insertion sort with arg
extern void utils_insertionSortWithArg(
    meta_S base_slice,
    cmp_Ord (*compareFn)(P_const$raw lhs, P_const$raw rhs, P_const$raw arg),
    P_const$raw arg
);

// Modernized merge sort using temporary buffer instead of allocating new memory
extern E$void utils_mergeSortUsingTempRecur(
    S$u8   temp_buffer,
    meta_S base_slice,
    cmp_Ord (*compareFn)(P_const$raw lhs, P_const$raw rhs)
) $must_check;

// Modernized merge sort using temporary buffer with arg
extern E$void utils_mergeSortWithArgUsingTempRecur(
    S$u8   temp_buffer,
    meta_S base_slice,
    cmp_Ord (*compareFn)(P_const$raw lhs, P_const$raw rhs, P_const$raw arg),
    P_const$raw arg
) $must_check;

#define utils_stableSort_threshold_merge_to_insertion 32

// Modernized stable sort (using merge sort with insertion sort)
extern E$void utils_stableSort(
    mem_Allocator allocator,
    meta_S        base_slice,
    cmp_Ord (*compareFn)(P_const$raw lhs, P_const$raw rhs)
) $must_check;

// Modernized stable sort with arg (using merge sort with insertion sort)
extern E$void utils_stableSortWithArg(
    mem_Allocator allocator,
    meta_S        base_slice,
    cmp_Ord (*compareFn)(P_const$raw lhs, P_const$raw rhs, P_const$raw arg),
    P_const$raw arg
) $must_check;

// Modernized stable sort (using merge sort with insertion sort)
extern E$void utils_stableSortUsingTemp(
    S$u8   temp_buffer,
    meta_S base_slice,
    cmp_Ord (*compareFn)(P_const$raw lhs, P_const$raw rhs)
) $must_check;

// Modernized stable sort with arg (using merge sort with insertion sort)
extern E$void utils_stableSortWithArgUsingTemp(
    S$u8   temp_buffer,
    meta_S base_slice,
    cmp_Ord (*compareFn)(P_const$raw lhs, P_const$raw rhs, P_const$raw arg),
    P_const$raw arg
) $must_check;

decl_ArrList$(Body);
decl_E$(ArrList$Body);
extern E$ArrList$Body utils_uniformDisc(mem_Allocator allocator, usize n) $must_check;

#endif /* UTILS_INCLUDED */

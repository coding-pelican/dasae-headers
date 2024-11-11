/**
 * @copyright Copyright 2024. Gyeongtae Kim All rights reserved.
 *
 * @file    cmp.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2024-11-02 (date of creation)
 * @updated 2024-11-05 (date of last update)
 * @version v1.0.0
 * @ingroup dasae-headers(dh)
 * @prefix  cmp
 *
 * @brief   Header of some software
 * @details Some detailed explanation
 */


#ifndef CMP_INCLUDED
#define CMP_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */


/*========== Includes =======================================================*/

#include "dh/builtin/comp.h"
#include "dh/core/prim/ptr.h"
#include "dh/core.h"

/*========== Macros and Definitions =========================================*/

/* Comparison ordering */
typedef enum cmp_EOrdering {
    cmp_Ordering_less    = -1,
    cmp_Ordering_equal   = 0,
    cmp_Ordering_greater = 1
} cmp_Ordering;

/* Generic comparison function type */
typedef cmp_Ordering (*cmp_Compare)(const anyptr lhs, const anyptr rhs);

/* Compare two values and return their ordering */
static_inline cmp_Ordering cmp_compare(const anyptr lhs, const anyptr rhs, const cmp_Compare compare) {
    return compare(lhs, rhs);
}

/* Check if lhs is equal to rhs */
static_inline bool cmp_isEq(const anyptr lhs, const anyptr rhs, const cmp_Compare compare) {
    return cmp_compare(lhs, rhs, compare) == cmp_Ordering_equal;
}

/* Check if lhs is not equal to rhs */
static_inline bool cmp_isNe(const anyptr lhs, const anyptr rhs, const cmp_Compare compare) {
    return cmp_compare(lhs, rhs, compare) != cmp_Ordering_equal;
}

/* Check if lhs is less than rhs */
static_inline bool cmp_isLt(const anyptr lhs, const anyptr rhs, const cmp_Compare compare) {
    return cmp_compare(lhs, rhs, compare) == cmp_Ordering_less;
}

/* Check if lhs is less than or equal to rhs */
static_inline bool cmp_isLe(const anyptr lhs, const anyptr rhs, const cmp_Compare compare) {
    return cmp_compare(lhs, rhs, compare) != cmp_Ordering_greater;
}

/* Check if lhs is greater than rhs */
static_inline bool cmp_isGt(const anyptr lhs, const anyptr rhs, const cmp_Compare compare) {
    return cmp_compare(lhs, rhs, compare) == cmp_Ordering_greater;
}

/* Check if lhs is greater than or equal to rhs */
static_inline bool cmp_isGe(const anyptr lhs, const anyptr rhs, const cmp_Compare compare) {
    return cmp_compare(lhs, rhs, compare) != cmp_Ordering_less;
}


#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* CMP_INCLUDED */

/**
 * @copyright Copyright 2024. Gyeongtae Kim All rights reserved.
 *
 * @file    cmp.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2024-11-02 (date of creation)
 * @updated 2024-11-05 (date of last update)
 * @version v1.0.0
 * @ingroup dasae-headers(dh)
 * @prefix  NONE
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

#include "core/core.h"

/*========== Macros and Definitions =========================================*/

typedef enum ECmpOrdering {
    CmpOrdering_less    = -1,
    CmpOrdering_equal   = 0,
    CmpOrdering_greater = 1
} CmpOrdering;

// Generic comparison function type
typedef CmpOrdering (*CmpFn)(const anyptr lhs, const anyptr rhs);

// Compare two values and return their ordering
static_inline CmpOrdering Cmp(const anyptr lhs, const anyptr rhs, const CmpFn cmp_fn) {
    return cmp_fn(lhs, rhs);
}

// Check if lhs is equal to rhs
static_inline bool Cmp_isEq(const anyptr lhs, const anyptr rhs, const CmpFn cmp_fn) {
    return Cmp(lhs, rhs, cmp_fn) == CmpOrdering_equal;
}

// Check if lhs is not equal to rhs
static_inline bool Cmp_isNe(const anyptr lhs, const anyptr rhs, const CmpFn cmp_fn) {
    return Cmp(lhs, rhs, cmp_fn) != CmpOrdering_equal;
}

// Check if lhs is less than rhs
static_inline bool Cmp_isLt(const anyptr lhs, const anyptr rhs, const CmpFn cmp_fn) {
    return Cmp(lhs, rhs, cmp_fn) == CmpOrdering_less;
}

// Check if lhs is less than or equal to rhs
static_inline bool Cmp_isLe(const anyptr lhs, const anyptr rhs, const CmpFn cmp_fn) {
    return Cmp(lhs, rhs, cmp_fn) != CmpOrdering_greater;
}

// Check if lhs is greater than rhs
static_inline bool Cmp_isGt(const anyptr lhs, const anyptr rhs, const CmpFn cmp_fn) {
    return Cmp(lhs, rhs, cmp_fn) == CmpOrdering_greater;
}

// Check if lhs is greater than or equal to rhs
static_inline bool Cmp_isGe(const anyptr lhs, const anyptr rhs, const CmpFn cmp_fn) {
    return Cmp(lhs, rhs, cmp_fn) != CmpOrdering_less;
}


#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* CMP_INCLUDED */

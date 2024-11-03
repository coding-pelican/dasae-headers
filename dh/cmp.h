/**
 * @copyright Copyright 2024. Gyeongtae Kim All rights reserved.
 *
 * @file    cmp.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2024-11-02 (date of creation)
 * @updated 2024-11-02 (date of last update)
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

#include "compat/compat_inline.h"
#include "types.h"

#include <stddef.h>

/*========== Macros and Definitions =========================================*/

typedef enum cmpEOrdering {
    cmpOrd_Less    = -1,
    cmpOrd_Equal   = 0,
    cmpOrd_Greater = 1
} cmpOrdering;

// Generic comparison function type
typedef cmpOrdering (*cmpFn)(const anyptr lhs, const anyptr rhs);

// Compare two values and return their ordering
force_inline cmpOrdering cmp_cmp(const anyptr lhs, const anyptr rhs, cmpFn cmp_fn) {
    return cmp_fn(lhs, rhs);
}

// Check if lhs is equal to rhs
force_inline bool cmp_isEq(const anyptr lhs, const anyptr rhs, cmpFn cmp_fn) {
    return cmp_cmp(lhs, rhs, cmp_fn) == cmpOrd_Equal;
}

// Check if lhs is not equal to rhs
force_inline bool cmp_isNe(const anyptr lhs, const anyptr rhs, cmpFn cmp_fn) {
    return cmp_cmp(lhs, rhs, cmp_fn) != cmpOrd_Equal;
}

// Check if lhs is less than rhs
force_inline bool cmp_isLt(const anyptr lhs, const anyptr rhs, cmpFn cmp_fn) {
    return cmp_cmp(lhs, rhs, cmp_fn) == cmpOrd_Less;
}

// Check if lhs is less than or equal to rhs
force_inline bool cmp_isLe(const anyptr lhs, const anyptr rhs, cmpFn cmp_fn) {
    return cmp_cmp(lhs, rhs, cmp_fn) != cmpOrd_Greater;
}

// Check if lhs is greater than rhs
force_inline bool cmp_isGt(const anyptr lhs, const anyptr rhs, cmpFn cmp_fn) {
    return cmp_cmp(lhs, rhs, cmp_fn) == cmpOrd_Greater;
}

// Check if lhs is greater than or equal to rhs
force_inline bool cmp_isGe(const anyptr lhs, const anyptr rhs, cmpFn cmp_fn) {
    return cmp_cmp(lhs, rhs, cmp_fn) != cmpOrd_Less;
}


#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* CMP_INCLUDED */

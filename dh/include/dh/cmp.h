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

#include "dh/builtin/comp.h"
#include "dh/core/prim/ptr.h"
#include "dh/core.h"

/*========== Macros and Definitions =========================================*/

/* Comparison ordering */

typedef enum EOrd {
    EOrd_less    = -1,
    EOrd_equal   = 0,
    EOrd_greater = 1
} EOrd;

/* Generic comparison function type */
typedef EOrd (*Cmp)(const anyptr lhs, const anyptr rhs);

/* Compare two values and return their ordering */
static_inline EOrd cmp(const anyptr lhs, const anyptr rhs, Cmp fn) {
    return fn(lhs, rhs);
}

/* Check if lhs is equal to rhs */
static_inline bool isEq(const anyptr lhs, const anyptr rhs, Cmp fn) {
    return cmp(lhs, rhs, fn) == EOrd_equal;
}

/* Check if lhs is not equal to rhs */
static_inline bool isNe(const anyptr lhs, const anyptr rhs, Cmp fn) {
    return cmp(lhs, rhs, fn) != EOrd_equal;
}

/* Check if lhs is less than rhs */
static_inline bool isLt(const anyptr lhs, const anyptr rhs, Cmp fn) {
    return cmp(lhs, rhs, fn) == EOrd_less;
}

/* Check if lhs is less than or equal to rhs */
static_inline bool isLe(const anyptr lhs, const anyptr rhs, Cmp fn) {
    return cmp(lhs, rhs, fn) != EOrd_greater;
}

/* Check if lhs is greater than rhs */
static_inline bool isGt(const anyptr lhs, const anyptr rhs, Cmp fn) {
    return cmp(lhs, rhs, fn) == EOrd_greater;
}

/* Check if lhs is greater than or equal to rhs */
static_inline bool isGe(const anyptr lhs, const anyptr rhs, Cmp fn) {
    return cmp(lhs, rhs, fn) != EOrd_less;
}


#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* CMP_INCLUDED */

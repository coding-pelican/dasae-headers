/**
 * @copyright Copyright 2024. Gyeongtae Kim All rights reserved.
 *
 * @file    core.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2024-11-06 (date of creation)
 * @updated 2024-11-06 (date of last update)
 * @version v1.0.0
 * @ingroup dasae-headers(dh)
 * @prefix  NONE
 *
 * @brief   Header of some software
 * @details Some detailed explanation
 */


#ifndef CORE_INCLUDED
#define CORE_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */


/*========== Includes =======================================================*/

#include "core/cfg.h"
#include "core/prim.h"
#include "core/pp.h"

/*========== Macros and Definitions =========================================*/

#define swap(TYPE, LHS, RHS) \
    RETURN_swap(TYPE, LHS, RHS)

#define RETURN_swap(TYPE, LHS, RHS) pp_func( \
    Ptr(TYPE) __lhs  = addr(LHS);            \
    Ptr(TYPE) __rhs  = addr(RHS);            \
    TYPE __tmp       = ptrAccess(__lhs);     \
    ptrAccess(__lhs) = ptrAccess(__rhs);     \
    ptrAccess(__rhs) = __tmp;                \
)


#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* CORE_INCLUDED */

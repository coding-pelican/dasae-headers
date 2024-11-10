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

#define swap(TYPE, LHS_VAR, RHS_VAR) \
    RETURN_swap(TYPE, LHS_VAR, RHS_VAR)

#define foreach(TYPE, NAME, LIST_PTR) \
    RETURN_foreach(TYPE, NAME, LIST_PTR)


// NOLINTBEGIN
#define RETURN_swap(TYPE, LHS_VAR, RHS_VAR) pp_func( \
    Ptr(TYPE) __lhs  = addr(LHS_VAR);                \
    Ptr(TYPE) __rhs  = addr(RHS_VAR);                \
    TYPE __tmp       = ptrAccess(__lhs);             \
    ptrAccess(__lhs) = ptrAccess(__rhs);             \
    ptrAccess(__rhs) = __tmp;                        \
)

#define RETURN_foreach(TYPE, NAME, LIST_PTR)                                                  \
    for (                                                                                     \
        TYPE* NAME = (LIST_PTR)->data, * const __end = (LIST_PTR)->data + (LIST_PTR)->length; \
        NAME < __end;                                                                         \
        ++NAME                                                                                \
    )
// NOLINTEND

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* CORE_INCLUDED */

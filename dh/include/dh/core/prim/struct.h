/**
 * @copyright Copyright 2024. Gyeongtae Kim All rights reserved.
 *
 * @file    struct.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2024-10-21 (date of creation)
 * @updated 2024-10-27 (date of last update)
 * @version v0.1-alpha
 * @ingroup dasae-headers(dh)/core/prim
 * @prefix  NONE
 *
 * @brief   Header of some software
 * @details Some detailed explanation
 */

#ifndef CORE_PRIM_STRUCT_INCLUDED
#define CORE_PRIM_STRUCT_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "cfg.h"
#include "int.h"

/*========== Macros and Definitions =========================================*/

#define typedef_struct(Alias)   \
    typedef struct Alias Alias; \
    struct Alias

/* Scheduled to be deprecated */
#define make(T, _Inital...)    deprecated_IMPL_make(T, _Inital)
#define makeCleared(T)         deprecated_IMPL_makeCleared(T)
#define create(T, _Initial...) deprecated_IMPL_create(T, _Initial)
#define createCleared(T)       deprecated_IMPL_createCleared(T)
#define createFrom(T, _var...) deprecated_IMPL_createFrom(T, _var)

/*========== Macros Implementation ==========================================*/

// NOLINTBEGIN(bugprone-macro-parentheses)
#define deprecated_IMPL_make(T, _Inital...) \
    ((T){ _Inital })

#define deprecated_IMPL_makeCleared(T) \
    ((T){ 0 })

#define deprecated_IMPL_create(T, _Inital...) \
    ((T[1]){ (T){ _Inital } })

#define deprecated_IMPL_createCleared(T) \
    ((T[1]){ (T){ 0 } })

#define deprecated_IMPL_createFrom(T, _var...) \
    ((T[1]){ _var })
// NOLINTEND(bugprone-macro-parentheses)

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* CORE_PRIM_STRUCT_INCLUDED */

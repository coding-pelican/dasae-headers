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

#define initial(_Inital...)    IMPL_initial(_Inital)
#define cleared()              IMPL_cleared()
#define make(T, _Inital...)    IMPL_make(T, _Inital)
#define makeCleared(T)         IMPL_makeCleared(T)
#define create(T, _Initial...) IMPL_create(T, _Initial)
#define createCleared(T)       IMPL_createCleared(T)
#define createFrom(T, _var...) IMPL_createFrom(T, _var)

/*========== Macros Implementation ==========================================*/

#define IMPL_initial(_Inital...) \
    { _Inital }

#define IMPL_cleared() \
    { 0 }

// NOLINTBEGIN(bugprone-macro-parentheses)
#define IMPL_make(T, _Inital...) \
    ((T){ _Inital })

#define IMPL_makeCleared(T) \
    ((T){ 0 })

#define IMPL_create(T, _Inital...) \
    ((T[1]){ (T){ _Inital } })

#define IMPL_createCleared(T) \
    ((T[1]){ (T){ 0 } })

#define IMPL_createFrom(T, _var...) \
    ((T[1]){ _var })
// NOLINTEND(bugprone-macro-parentheses)

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* CORE_PRIM_STRUCT_INCLUDED */

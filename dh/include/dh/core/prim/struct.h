/**
 * @copyright Copyright 2024. Gyeongtae Kim All rights reserved.
 *
 * @file    struct.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2024-10-21 (date of creation)
 * @updated 2024-10-27 (date of last update)
 * @version v1.0.0
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
#include "dh/core/pp.h"

/*========== Macros and Definitions =========================================*/

#define typedef_struct(Alias)   \
    typedef struct Alias Alias; \
    struct Alias

#define initial(_Inital...)  IMPL_initial(_Inital)
#define cleared()            IMPL_cleared()
#define make(_T, _Inital...) IMPL_make(_T, _Inital)
#define makeCleared(_T)      IMPL_makeCleared(_T)
// #define create(_T, _Initial...) IMPL_create(_T, _Initial)
// #define createCleared(_T)       IMPL_createCleared(_T)
// #define createFrom(_T, _var)    IMPL_createFrom(_T, _var)

/*========== Macros Implementation ==========================================*/

#define IMPL_initial(_Inital...) \
    { _Inital }

#define IMPL_cleared() \
    { 0 }

// NOLINTBEGIN(bugprone-macro-parentheses)
#define IMPL_make(_T, _Inital...) \
    ((_T){ _Inital })

#define IMPL_makeCleared(_T) \
    ((_T){ 0 })

// #define IMPL_create(_T, _Inital...) \
//     ((_T[1]){ _Inital })

// #define IMPL_createCleared(_T) \
//     ((_T[1]){ 0 })

// #define IMPL_createFrom(_T, _var) \
//     ((_T[1]){ _var })
// NOLINTEND(bugprone-macro-parentheses)

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* CORE_PRIM_STRUCT_INCLUDED */

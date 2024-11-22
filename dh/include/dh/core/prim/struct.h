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

#define make(_T)                 IMPL_make(_T)
#define makeWith(_T, _Inital...) IMPL_makeWith(_T, _Inital)

#define create(_T)                 IMPL_create(_T)
#define createWith(_T, _Inital...) IMPL_createWith(_T, _Inital)

/*========== Macros Implementation ==========================================*/

#define IMPL_make(_T) \
    literal(_T, 0)
#define IMPL_makeWith(_T, _Inital...) \
    literal(_T, _Inital)

// NOLINTBEGIN(bugprone-macro-parentheses)
#define IMPL_create(_T) ( \
    (_T[1]){              \
        literal(_T, 0) }  \
)
#define IMPL_createWith(_T, _Inital...) ( \
    (_T[1]){                              \
        literal(_T, _Inital) }            \
)
// NOLINTEND(bugprone-macro-parentheses)

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* CORE_PRIM_STRUCT_INCLUDED */

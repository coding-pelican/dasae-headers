/**
 * @copyright Copyright 2024. Gyeongtae Kim All rights reserved.
 *
 * @file    core.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2024-11-06 (date of creation)
 * @updated 2024-12-23 (date of last update)
 * @version v0.1
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
#include "core/op.h"
#include "core/cmp.h"
#include "core/type_info.h"

/*========== Macros and Definitions =========================================*/

// #define orelse(TPtr, var_ptr, val_default)          \
//     /**                                             \
//      * @brief Return val_default if var_ptr is null \
//      */                                             \
//     IMPL_orelse(TPtr, var_ptr, val_default)

// #define orclaim(TPtr, var_ptr)                                                \
//     /**                                                                       \
//      * @brief Equivalent to: `orelse(TPtr, var_ptr, unreachable_val(anyptr))` \
//      */                                                                       \
//     IMPL_orclaim(TPtr, var_ptr)

// #define foreach(TParam, decl_iter, var_list_ptr) \
//     IMPL_foreach(                                \
//         pp_uniqueToken(end),                     \
//         TParam,                                  \
//         decl_iter,                               \
//         var_list_ptr                             \
//     )

/*========== Macros Implementation ==========================================*/

// #define IMPL_orelse(TPtr, var_ptr, val_default) \
//     ((TPtr*)orelse_anyptr(var_ptr, val_default))

// #define IMPL_orclaim(TPtr, var_ptr) \
//     ((TPtr*)orclaim_anyptr(var_ptr))

// force_inline anyptr orelse_anyptr(anyptr var_ptr, anyptr val_default) {
//     return var_ptr ? var_ptr : val_default;
// }

// force_inline anyptr orclaim_anyptr(anyptr var_ptr) {
//     return var_ptr ? var_ptr : claim_unreachable_val(anyptr);
// }

// #define IMPL_foreach(_iter_end, TParam, decl_iter, var_list_ptr)                                                               \
//     for (                                                                                                                      \
//         rawptr(TParam) used(decl_iter) = (var_list_ptr)->data, * const _iter_end = (var_list_ptr)->data + (var_list_ptr)->len; \
//         used(decl_iter) < used(_iter_end);                                                                                     \
//         ++used(decl_iter)                                                                                                      \
//     )
// NOLINTEND(bugprone-macro-parentheses)

/*========== Externalized Static Functions Prototypes (Unit Test) ===========*/

#ifdef UNIT_TEST
#endif /* UNIT_TEST */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* CORE_INCLUDED */

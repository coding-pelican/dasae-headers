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
#include "core/pp.h"
#include "core/prim.h"
// #include "core/Ptr.h"
// #include "core/Slice.h"
#include "core/ops.h"
#include "core/cmp.h"

/*========== Macros and Definitions =========================================*/

#define swap(TParam, var_lhs, var_rhs) \
    IMPL_swap(                         \
        pp_uniqueToken(lhs),           \
        pp_uniqueToken(rhs),           \
        pp_uniqueToken(tmp),           \
        TParam,                        \
        var_lhs,                       \
        var_rhs                        \
    )

#define foreach(TParam, decl_iter, var_list_ptr) \
    IMPL_foreach(                                \
        pp_uniqueToken(end),                     \
        TParam,                                  \
        decl_iter,                               \
        var_list_ptr                             \
    )

/*========== Macros Implementation ==========================================*/

#define IMPL_swap(_lhs, _rhs, _tmp, TParam, var_lhs, var_rhs) pp_func( \
    rawptr(TParam) used(_lhs) = rawaddr(var_lhs);                      \
    rawptr(TParam) used(_rhs) = rawaddr(var_rhs);                      \
    TParam _tmp               = rawDeref(_lhs);                        \
    rawDeref(_lhs)            = rawDeref(_rhs);                        \
    rawDeref(_rhs)            = _tmp;                                  \
)

#define IMPL_foreach(_iter_end, TParam, decl_iter, var_list_ptr)                                                               \
    for (                                                                                                                      \
        rawptr(TParam) used(decl_iter) = (var_list_ptr)->data, * const _iter_end = (var_list_ptr)->data + (var_list_ptr)->len; \
        used(decl_iter) < used(_iter_end);                                                                                     \
        ++used(decl_iter)                                                                                                      \
    )
// NOLINTEND(bugprone-macro-parentheses)

/*========== Externalized Static Functions Prototypes (Unit Test) ===========*/

#ifdef UNIT_TEST
#endif /* UNIT_TEST */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* CORE_INCLUDED */

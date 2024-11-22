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

/*========== Macros and Definitions =========================================*/

#define swap(_T, _lhs_var, _rhs_var) \
    IMPL_swap(                       \
        pp_uniqueToken(lhs),         \
        pp_uniqueToken(rhs),         \
        pp_uniqueToken(tmp),         \
        _T,                          \
        _lhs_var,                    \
        _rhs_var                     \
    )

#define foreach(_T, _name, _list_ptr) \
    IMPL_foreach(                     \
        pp_uniqueToken(end),          \
        _T,                           \
        _name,                        \
        _list_ptr                     \
    )

/*========== Macros Implementation ==========================================*/

// NOLINTBEGIN(bugprone-macro-parentheses)
#define IMPL_swap(_lhs, _rhs, _tmp, _T, _lhs_var, _rhs_var) pp_func( \
    rawptr(_T) _lhs = rawaddr(_lhs_var);                             \
    rawptr(_T) _rhs = rawaddr(_rhs_var);                             \
    _T _tmp         = accessPtr(_lhs);                               \
    accessPtr(_lhs) = accessPtr(_rhs);                               \
    accessPtr(_rhs) = _tmp;                                          \
)

// TODO: Make foreach type generic using SizedPtr with Begin Len/End
#define IMPL_foreach(_end, _T, _name, _list_ptr)                                            \
    for (                                                                                   \
        _T* _name = (_list_ptr)->data, * const _end = (_list_ptr)->data + (_list_ptr)->len; \
        _name < _end;                                                                       \
        ++_name                                                                             \
    )
// NOLINTEND(bugprone-macro-parentheses)

/*========== Externalized Static Functions Prototypes (Unit Test) ===========*/

#ifdef UNIT_TEST
#endif /* UNIT_TEST */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* CORE_INCLUDED */

/**
 * @copyright Copyright 2024. Gyeongtae Kim All rights reserved.
 *
 * @file    scope.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2024-11-14 (date of creation)
 * @updated 2025-01-02 (date of last update)
 * @version v0.1
 * @ingroup dasae-headers(dh)
 * @prefix  scope
 *
 * @brief   Header of some software
 * @details Some detailed explanation
 */

#ifndef SCOPE_INCLUDED
#define SCOPE_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "core.h"

/*========== Macros and Definitions =========================================*/

#define scope_allows_reserved_return_with_defer (1)

#define scope_with(_Init_Statement...) \
    IMPL_scope_with(pp_uniqueToken(run_once), pp_uniqueToken(init_once), _Init_Statement)

#define scope_with_fini(_Init_Statement, _Fini_Statement...) \
    IMPL_scope_with_fini(pp_uniqueToken(run_once), pp_uniqueToken(init_once), _Init_Statement, _Fini_Statement)

#define scope_var(_Init_Statement...) \
    IMPL_scope_var(pp_uniqueToken(run_once), pp_uniqueToken(init_once), _Init_Statement)

#define scope_let(_Init_Statement...) \
    IMPL_scope_let(pp_uniqueToken(run_once), pp_uniqueToken(init_once), _Init_Statement)

#define scope_if(_Init_Statement, _Condition) \
    IMPL_scope_if(pp_uniqueToken(run_once), pp_uniqueToken(init_once), _Init_Statement, _Condition)

#define scope_else(_Init_Statement...) \
    IMPL_scope_else(pp_uniqueToken(run_once), pp_uniqueToken(init_once), _Init_Statement)

#define scope_switch(_Init_Statement, _Condition) \
    IMPL_scope_switch(pp_uniqueToken(run_once), pp_uniqueToken(init_once), _Init_Statement, _Condition)

#define scope_while(_Init_Statement, _Condition) \
    IMPL_scope_while(pp_uniqueToken(run_once), pp_uniqueToken(init_once), _Init_Statement, _Condition)

#define scope_va_list(_Init_Statement) \
    /* TODO: Implement scope_va_list */

#define scope_reserved_return(T) \
    IMPL_scope_reserved_return(T)

/*========== Macros Implementation ==========================================*/

// NOLINTBEGIN
#define IMPL_scope_with(_run_once, _init_once, _Init_Statement...)               \
    for (bool _run_once = true, _init_once = true; _run_once; _run_once = false) \
        for (_Init_Statement; _init_once; _init_once = false)

#define IMPL_scope_with_fini(_run_once, _init_once, _Init_Statement, _Fini_Statement...)          \
    for (bool _run_once = true, _init_once = true; _run_once; _run_once = false, _Fini_Statement) \
        for (_Init_Statement; _init_once; _init_once = false)

#define IMPL_scope_var(_run_once, _init_once, _Init_Statement...)                \
    for (bool _run_once = true, _init_once = true; _run_once; _run_once = false) \
        for (var _Init_Statement; _init_once; _init_once = false)

#define IMPL_scope_let(_run_once, _init_once, _Init_Statement...)                \
    for (bool _run_once = true, _init_once = true; _run_once; _run_once = false) \
        for (let _Init_Statement; _init_once; _init_once = false)

#define IMPL_scope_if(_run_once, _init_once, _Init_Statement, _Condition) \
    IMPL_scope_with(_run_once, _init_once, _Init_Statement) if (_Condition)

#define IMPL_scope_else(_run_once, _init_once, _Init_Statement...) \
    else IMPL_scope_with(_run_once, _init_once, _Init_Statement)

#define IMPL_scope_switch(_run_once, _init_once, _Init_Statement, _Condition) \
    IMPL_scope_with(_run_once, _init_once, _Init_Statement) switch (_Condition)

#define IMPL_scope_while(_run_once, _init_once, _Init_Statement, _Condition) \
    IMPL_scope_with(_run_once, _init_once, _Init_Statement) while (_Condition)

#if scope_allows_reserved_return_with_defer

#define IMPL_scope_reserved_return(T) \
    reserveReturn(T);                 \
    struct {                          \
        bool has_defer;               \
    } _scope = {                      \
        .has_defer = false,           \
    };                                \
    scope_defer

#else

#define IMPL_scope_reserved_return(_run_once, _init_once, T) \
    IMPL_scope_with(                                         \
        _run_once,                                           \
        _init_once,                                          \
        struct {                                             \
            T*   reserved_return;                            \
            bool has_defer;                                  \
        } _scope                                             \
        = {                                                  \
            .reserved_return = null,                         \
            .has_defer       = false,                        \
        }                                                    \
    )

#endif
// NOLINTEND

/*========== Externalized Static Functions Prototypes (Unit Test) ===========*/

#ifdef UNIT_TEST
#endif /* UNIT_TEST */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* SCOPE_INCLUDED */

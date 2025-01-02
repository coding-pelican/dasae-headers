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

#define scope_with(_Init_Statement...) \
    IMPL_scope_with(pp_uniqueToken(run_once), pp_uniqueToken(init_once), _Init_Statement)

#define scope_var(_Init_Statement...) \
    IMPL_scope_var(pp_uniqueToken(run_once), pp_uniqueToken(init_once), _Init_Statement)

#define scope_let(_Init_Statement...) \
    IMPL_scope_let(pp_uniqueToken(run_once), pp_uniqueToken(init_once), _Init_Statement)

#define scope_if(_Init_Statement, _Condition) \
    IMPL_scope_if(pp_uniqueToken(run_once), pp_uniqueToken(init_once), _Init_Statement, _Condition)

// #define scope_if_var(_Init_Statement, _Condition) \
//     IMPL_scope_if_var(pp_uniqueToken(run_once), pp_uniqueToken(init_once), _Init_Statement, _Condition)

// #define scope_if_let(_Init_Statement, _Condition) \
//     IMPL_scope_if_let(pp_uniqueToken(run_once), pp_uniqueToken(init_once), _Init_Statement, _Condition)

#define scope_switch(_Init_Statement, _Condition) \
    IMPL_scope_switch(pp_uniqueToken(run_once), pp_uniqueToken(init_once), _Init_Statement, _Condition)

// #define scope_switch_var(_Init_Statement, _Condition) \
//     IMPL_scope_switch_var(pp_uniqueToken(run_once), pp_uniqueToken(init_once), _Init_Statement, _Condition)

// #define scope_switch_let(_Init_Statement, _Condition) \
//     IMPL_scope_switch_let(pp_uniqueToken(run_once), pp_uniqueToken(init_once), _Init_Statement, _Condition)

#define scope_while(_Init_Statement, _Condition) \
    IMPL_scope_while(pp_uniqueToken(run_once), pp_uniqueToken(init_once), _Init_Statement, _Condition)

// #define scope_while_var(_Init_Statement, _Condition) \
//     IMPL_scope_while_var(pp_uniqueToken(run_once), pp_uniqueToken(init_once), _Init_Statement, _Condition)

// #define scope_while_let(_Init_Statement, _Condition) \
//     IMPL_scope_while_let(pp_uniqueToken(run_once), pp_uniqueToken(init_once), _Init_Statement, _Condition)

#define scope_va_list(_Init_Statement) \
    /* TODO: Implement scope_va_list */

/*========== Macros Implementation ==========================================*/

// NOLINTBEGIN
#define IMPL_scope_with(_run_once, _init_once, _Init_Statement...)               \
    for (bool _run_once = true, _init_once = true; _run_once; _run_once = false) \
        for (_Init_Statement; _init_once; _init_once = false)

#define IMPL_scope_var(_run_once, _init_once, _Init_Statement...)                \
    for (bool _run_once = true, _init_once = true; _run_once; _run_once = false) \
        for (var _Init_Statement; _init_once; _init_once = false)

#define IMPL_scope_let(_run_once, _init_once, _Init_Statement...)                \
    for (bool _run_once = true, _init_once = true; _run_once; _run_once = false) \
        for (let _Init_Statement; _init_once; _init_once = false)

#define IMPL_scope_if(_run_once, _init_once, _Init_Statement, _Condition)        \
    for (bool _run_once = true, _init_once = true; _run_once; _run_once = false) \
        for (_Init_Statement; _init_once; _init_once = false)                    \
            if (_Condition)

// #define IMPL_scope_if_var(_run_once, _init_once, _Init_Statement, _Condition)    \
//     for (bool _run_once = true, _init_once = true; _run_once; _run_once = false) \
//         for (var _Init_Statement; _init_once; _init_once = false)                \
//             if (_Condition)

// #define IMPL_scope_if_let(_run_once, _init_once, _Init_Statement, _Condition)    \
//     for (bool _run_once = true, _init_once = true; _run_once; _run_once = false) \
//         for (let _Init_Statement; _init_once; _init_once = false)                \
//             if (_Condition)

#define IMPL_scope_switch(_run_once, _init_once, _Init_Statement, _Condition)    \
    for (bool _run_once = true, _init_once = true; _run_once; _run_once = false) \
        for (_Init_Statement; _init_once; _init_once = false)                    \
            switch (_Condition)

// #define IMPL_scope_switch_var(_run_once, _init_once, _Init_Statement, _Condition) \
//     for (bool _run_once = true, _init_once = true; _run_once; _run_once = false)  \
//         for (var _Init_Statement; _init_once; _init_once = false)                 \
//             switch (_Condition)

// #define IMPL_scope_switch_let(_run_once, _init_once, _Init_Statement, _Condition) \
//     for (bool _run_once = true, _init_once = true; _run_once; _run_once = false)  \
//         for (let _Init_Statement; _init_once; _init_once = false)                 \
//             switch (_Condition)

#define IMPL_scope_while(_run_once, _init_once, _Init_Statement, _Condition)     \
    for (bool _run_once = true, _init_once = true; _run_once; _run_once = false) \
        for (_Init_Statement; _init_once; _init_once = false)                    \
            while (_Condition)

// #define IMPL_scope_while_var(_run_once, _init_once, _Init_Statement, _Condition) \
//     for (bool _run_once = true, _init_once = true; _run_once; _run_once = false) \
//         for (var _Init_Statement; _init_once; _init_once = false)                \
//             while (_Condition)

// #define IMPL_scope_while_let(_run_once, _init_once, _Init_Statement, _Condition) \
//     for (bool _run_once = true, _init_once = true; _run_once; _run_once = false) \
//         for (let _Init_Statement; _init_once; _init_once = false)                \
//             while (_Condition)
// NOLINTEND

/*========== Externalized Static Functions Prototypes (Unit Test) ===========*/

#ifdef UNIT_TEST
#endif /* UNIT_TEST */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* SCOPE_INCLUDED */

/**
 * @copyright Copyright (c) 2025 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    common.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2025-01-13 (date of creation)
 * @updated 2025-02-01 (date of last update)
 * @version v0.1-alpha.2
 * @ingroup dasae-headers(dh)/scope
 * @prefix  scope
 *
 * @brief   Common scope management utilities
 * @details Provides macros and functions for scope-based resource management:
 *          - Scoped variable declarations and initialization
 *          - Resource acquisition and cleanup
 *          - Conditional scope execution
 *          - Loop constructs with initialization
 */

#ifndef SCOPE_COMMON_INCLUDED
#define SCOPE_COMMON_INCLUDED (1)
#if defined(__cplusplus)
extern "C"
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "cfg.h"

/*========== Macros and Definitions =========================================*/

#define with_(_Init...)             scope_with(_Init)
#define with_fini_(_Init, _Fini...) scope_with_fini(_Init, _Fini)
#define if_(_Init, _Cond)           scope_if(_Init, _Cond)
#define else_(_Init...)             scope_else(_Init)

#define $fwd                                          $_fwd,
#define $rev                                          $_rev,
#define $asc                                          $_asc,
#define $desc                                         $_desc,
#define for_(/*<$dir_type>(_begin, _end), _iter*/...) pp_overload(__for, __VA_ARGS__)(__VA_ARGS__)
#define __for_2(_range, _iter)                        __for_$_fwd(_range, _iter)
#define __for_3(_dir_type, _range, _iter)             pp_cat(__for_, _dir_type)(_range, _iter)
#define __for_$_fwd(_range, _iter) \
    for (struct { bool once; R range; usize iter; } __state = { \
             .once  = false, \
             .range = (_range), \
             .iter  = 0 }; \
         !__state.once; __state.once = true) \
        for (__state.iter = __state.range.begin; \
             __state.iter < __state.range.end; \
             ++__state.iter) with_(let_(_iter, usize) = __state.iter)
#define __for_$_rev(_range, _iter) \
    for (struct { bool once; R range; usize iter; } __state = { \
             .once  = false, \
             .range = (_range), \
             .iter  = 0 }; \
         !__state.once; __state.once = true) \
        for (__state.iter = __state.range.end; \
             __state.iter > __state.range.begin; \
             --__state.iter) with_(let_(_iter, usize) = (__state.iter - 1))
#define __for_$_asc(_range, _iter)  __for_$_fwd(_range, _iter)
#define __for_$_desc(_range, _iter) __for_$_rev(_range, _iter)

#define while_(_Init, _Cond, /*_Cont*/...)  scope_while(_Init, _Cond, __VA_OPT__(, ) __VA_ARGS__)
#define switch_(_Init, _Cond, /*_Body*/...) scope_switch(_Init, _Cond, __VA_ARGS__)

// case, default, ...

#define scope_with(_Init...) \
    SYN__scope_with(pp_uniqTok(run_once), pp_uniqTok(init_once), _Init)

#define scope_with_fini(_Init, _Fini...) \
    SYN__scope_with_fini(pp_uniqTok(run_once), pp_uniqTok(init_once), _Init, _Fini)

#define scope_var(_Init...) \
    SYN__scope_var(pp_uniqTok(run_once), pp_uniqTok(init_once), _Init)

#define scope_let(_Init...) \
    SYN__scope_let(pp_uniqTok(run_once), pp_uniqTok(init_once), _Init)

#define scope_if(_Init, _Cond) \
    SYN__scope_if(pp_uniqTok(run_once), pp_uniqTok(init_once), _Init, _Cond)

#define scope_else(_Init...) \
    SYN__scope_else(pp_uniqTok(run_once), pp_uniqTok(init_once), _Init)

#define scope_switch(_Init, _Cond, ...) \
    SYN__scope_switch(_Init, _Cond, __VA_ARGS__)

#define scope_while(_Init, _Cond, ...) \
    SYN__scope_while(_Init, _Cond __VA_OPT__(, ) __VA_ARGS__)

#define scope_va_list(_Init) \
    /* TODO: Implement scope_va_list */

/*========== Macros Implementation ==========================================*/

// NOLINTBEGIN
#define SYN__scope_with(__run_once, __init_once, _Init...) \
    for (bool __run_once = false, __init_once = false; !__run_once; __run_once = true) \
        for (_Init; !__init_once; __init_once = true)

#define SYN__scope_with_fini(__run_once, __init_once, _Init, _Fini...) \
    for (bool __run_once = false, __init_once = false; !__run_once; __run_once = true) \
        for (_Init; !__init_once; ({ __init_once = true; _Fini; }))

#define SYN__scope_var(__run_once, __init_once, _Init...) \
    for (bool __run_once = false, __init_once = false; !__run_once; __run_once = true) \
        for (var _Init; !__init_once; __init_once = true)

#define SYN__scope_let(__run_once, __init_once, _Init...) \
    for (bool __run_once = false, __init_once = false; !__run_once; __run_once = true) \
        for (let _Init; !__init_once; __init_once = true)

#define SYN__scope_if(__run_once, __init_once, _Init, _Cond) \
    SYN__scope_with(__run_once, __init_once, _Init) if (_Cond)

#define SYN__scope_else(__run_once, __init_once, _Init...) \
    else SYN__scope_with(__run_once, __init_once, _Init)

#define SYN__scope_switch(_Init, _Cond, ...) ({ \
    _Init; \
    switch (_Cond) \
        __VA_ARGS__ \
})

#define SYN__scope_while(_Init, _Cond, _Cont...) \
    for (_Init; _Cond; _Cont) \
    // NOLINTEND

#if defined(__cplusplus)
/* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* SCOPE_COMMON_INCLUDED */

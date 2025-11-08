/**
 * @copyright Copyright (c) 2025 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    common.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
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

#include "../prim.h"
#include "../range.h"

/*========== Macros and Definitions =========================================*/

/* with: declarations =======================================================*/
#define with_(_Init...)             scope_with(_Init)
#define with_fini_(_Init, _Fini...) scope_with_fini(_Init, _Fini)

/* if-else: declarations ====================================================*/
#define if_(_Init, _Cond) scope_if(_Init, _Cond)
#define else_(_Init...)   scope_else(_Init)

/* for: declaration =========================================================*/
#define $rf(_expr...) $r(_expr, usize_limit_max)
#define $rt(_expr...) $r(0, _expr)
#define $a(_a...)     ($A, (_a))
#define $s(_s...)     ($S, (_s))
#define $ua(_a...)    ($u_A, (_a))
#define $us(_s...)    ($u_S, (_s))

#define $fwd  $_fwd,
#define $bwd  $_bwd,
#define $rev  $_rev,
#define $asc  $_asc,
#define $desc $_desc,

/* TODO: add $scope-$unscoped version that doesn't require curly braces inside the calling */
#define for_(/*<$dir_type>(_iter...)(_capture...) { ... }*/...) pp_overload(__for, __VA_ARGS__)(__VA_ARGS__)
#define __for_1(/*(_iter...)(_capture...) { ... }*/...) \
    __exec__for_(pp_defer(__emit__for_$_fwd)(__sep0__for_ __VA_ARGS__))
#define __for_2(_$dir_type, /*(_iter...)(_capture...) { ... }*/...) \
    __exec__for_(pp_defer(pp_cat(__emit__for_, _$dir_type))(__sep0__for_ __VA_ARGS__))
#define __exec__for_(...)                                __VA_ARGS__
#define __sep0__for_(_iters...)                          (_iters), __sep1__for_
#define __sep1__for_(_captures...)                       (_captures),
#define __emit__for_$_fwd(_iters, _captures, _block...)  __emitNext__for_(_iters, _captures, (_block))
// #define __emitNext__for_(_iters, _captures, _block) \
//     0(_iters), 1(_captures), 2(_block)
#define __emit__for_$_bwd(_iters, _captures, _block...)  __emitNext__for_$_rev(_iters, _captures, (_block))
// #define __emitNext__for_$_bwd(_iters, _captures, _block) \
//     0(_iters), 1(_captures), 2(_block)
#define __emit__for_$_rev(_iters, _captures, _block...)  __emitNext__for_$_rev(_iters, _captures, (_block))
// #define __emitNext__for_$_rev(_iters, _captures, _block) \
//     0(_iters), 1(_captures), 2(_block)
#define __emit__for_$_asc(_iters, _captures, _block...)  __emitNext__for_(_iters, _captures, (_block))
#define __emit__for_$_desc(_iters, _captures, _block...) __emitNext__for_$_rev(_iters, _captures, (_block))

#if UNUSED_CODE
#define $fwd                                          $_fwd,
#define $rev                                          $_rev,
#define $asc                                          $_asc,
#define $desc                                         $_desc,
#define for_(/*<$dir_type>(_begin, _end), _iter*/...) pp_overload(__for, __VA_ARGS__)(__VA_ARGS__)
#define __for_2(_range, _iter)                        __for_$_fwd(_range, _iter)
#define __for_3(_dir_type, _range, _iter)             pp_cat(__for_, _dir_type)(_range, _iter)
#define __for_$_fwd(_range, _iter) \
    for (struct { bool once; R range; usize iter; } __state = { \
             .once = false, \
             .range = (_range), \
             .iter = 0 }; \
         !__state.once; __state.once = true) \
        for (__state.iter = __state.range.begin; \
             __state.iter < __state.range.end; \
             ++__state.iter) with_(let_(_iter, usize) = __state.iter)
#define __for_$_rev(_range, _iter) \
    for (struct { bool once; R range; usize iter; } __state = { \
             .once = false, \
             .range = (_range), \
             .iter = 0 }; \
         !__state.once; __state.once = true) \
        for (__state.iter = __state.range.end; \
             __state.iter > __state.range.begin; \
             --__state.iter) with_(let_(_iter, usize) = (__state.iter - 1))
#define __for_$_asc(_range, _iter)  __for_$_fwd(_range, _iter)
#define __for_$_desc(_range, _iter) __for_$_rev(_range, _iter)
#endif /* UNUSED_CODE */

/* while: declaration =======================================================*/
#define while_(_Init, _Cond, /*_Cont*/...) scope_while(_Init, _Cond, __VA_OPT__(, ) __VA_ARGS__)

/* switch: declaration ======================================================*/
// #define switch_(_Init, _Cond, /*_Body*/...) scope_switch(_Init, _Cond, __VA_ARGS__)

// case, default, ...

/*========== Macros Implementation ==========================================*/

#define scope_with(_Init...) \
    SYN__scope_with(pp_uniqTok(run_once), _Init)

#define scope_with_fini(_Init, _Fini...) \
    SYN__scope_with_fini(pp_uniqTok(run_once), _Init, _Fini)

#define scope_var(_Init...) \
    SYN__scope_var(pp_uniqTok(run_once), _Init)

#define scope_let(_Init...) \
    SYN__scope_let(pp_uniqTok(run_once), _Init)

#define scope_if(_Init, _Cond) \
    SYN__scope_if(pp_uniqTok(run_once), _Init, _Cond)

#define scope_else(_Init...) \
    SYN__scope_else(pp_uniqTok(run_once), _Init)

#define scope_switch(_Init, _Cond, ...) \
    SYN__scope_switch(_Init, _Cond, __VA_ARGS__)

#define scope_while(_Init, _Cond, ...) \
    SYN__scope_while(_Init, _Cond __VA_OPT__(, ) __VA_ARGS__)

#define scope_va_list(_Init) \
    /* TODO: Implement scope_va_list */

/* NOLINTBEGIN */
#define SYN__scope_with(__run_once, _Init...) \
    for (bool __run_once = false; !__run_once;) \
        for (_Init; !__run_once; __run_once = true)

#define SYN__scope_with_fini(__run_once, _Init, _Fini...) \
    for (bool __run_once = false; !__run_once;) \
        for (_Init; !__run_once; ({ __run_once = true; _Fini; }))

#define SYN__scope_var(__run_once, _Init...) \
    for (bool __run_once = false; !__run_once;) \
        for (var _Init; !__run_once; __run_once = true)

#define SYN__scope_let(__run_once, _Init...) \
    for (bool __run_once = false; !__run_once;) \
        for (let _Init; !__run_once; __run_once = true)

#define SYN__scope_if(__run_once, _Init, _Cond) \
    SYN__scope_with(__run_once, _Init) if (_Cond)

#define SYN__scope_else(__run_once, _Init...) \
    else SYN__scope_with(__run_once, _Init)

#define SYN__scope_switch(_Init, _Cond, ...) ({ \
    _Init; \
    switch (_Cond) \
        __VA_ARGS__ \
})

#define SYN__scope_while(_Init, _Cond, _Cont...) \
    for (_Init; _Cond; _Cont) \
/* NOLINTEND */

/* for: implementation ======================================================*/
/* for - common functions ===================================================*/
#define __for__expandIters(_iters...)                   _iters
#define __for__expandIter(/*<_iter>|<_tag,_iter>*/...)  __VA_ARGS__
#define __for__expandIterIds(__ids...)                  __ids
#define __for__expandIterId(__id)                       __id
#define __for__expandCaptures(_captures...)             _captures
#define __for__expandCapture(/*<_capture>|$ignore*/...) __VA_ARGS__
#define __for__expandBlock(_block...)                   _block

#define __for__initIter(__iter_id, _iter) \
    pp_overload(__for__initIter, __for__expandIter _iter)(__iter_id, __for__expandIter _iter)
#define __for__initIter_1(__iter_id, ...) __for__initIter_1Emit(__iter_id, __VA_ARGS__)
#define __for__initIter_1Emit(__iter_id, _iter) \
    const R __iter_id = _iter
#define __for__initIter_2(__iter_id, ...) __for__initIter_2Emit(__iter_id, __VA_ARGS__)
#define __for__initIter_2Emit(__iter_id, _tag, _iter...) \
    let __iter_id = _iter

#define __for__lenIter(_iter) \
    pp_overload(__for__lenIter, __for__expandIter _iter)(__for__expandIter _iter)
#define __for__lenIter_1(...)             __for__lenIter_1Emit(__VA_ARGS__)
#define __for__lenIter_1Emit(_iter)       len$R
#define __for__lenIter_2(...)             __for__lenIter_2Emit(__VA_ARGS__)
#define __for__lenIter_2Emit(_tag, _iter) pp_cat(len, _tag)

#define __for__atIter(__step, __iter_id, _iter) \
    pp_overload(__for__atIter, __for__expandIter _iter)(__step, __iter_id, __for__expandIter _iter)
#define __for__atIter_1(...) __for__atIter_1Emit(__VA_ARGS__)
#define __for__atIter_1Emit(__step, __iter_id, _iter) \
    at$R(__iter_id, __step)
#define __for__atIter_2(...) __for__atIter_2Emit(__VA_ARGS__)
#define __for__atIter_2Emit(__step, __iter_id, _tag, _iter) \
    pp_cat(at, _tag)(__iter_id, __step)

/* for - emitters (forward) =================================================*/
#define __emitNext__for_(_iters, _captures, _block) \
    pp_overload(__emitNext__for, __for__expandIters _iters)(_iters, _captures, _block)
#define __emitNext__for_1(_iters, _captures, _block) __emit__for_1( \
    pp_uniqTok(len), pp_uniqTok(step), (pp_uniqTok(iter_id0)), \
    _iters, _captures, _block \
)
#define __emitNext__for_2(_iters, _captures, _block) __emit__for_2( \
    pp_uniqTok(len), pp_uniqTok(step), \
    ( \
        pp_uniqTok(iter_id0), \
        pp_uniqTok(iter_id1) \
    ), \
    _iters, _captures, _block \
)
#define __emitNext__for_3(_iters, _captures, _block) __emit__for_3( \
    pp_uniqTok(len), pp_uniqTok(step), \
    ( \
        pp_uniqTok(iter_id0), \
        pp_uniqTok(iter_id1), \
        pp_uniqTok(iter_id2) \
    ), \
    _iters, _captures, _block \
)
#define __emitNext__for_4(_iters, _captures, _block) __emit__for_4( \
    pp_uniqTok(len), pp_uniqTok(step), \
    ( \
        pp_uniqTok(iter_id0), \
        pp_uniqTok(iter_id1), \
        pp_uniqTok(iter_id2), \
        pp_uniqTok(iter_id3) \
    ), \
    _iters, _captures, _block \
)

// #define __emit__for_1(__len, __step, __iter_ids, _iters, _captures, _block) \
//     0(__len), 1(__step), 2(__iter_ids), 3(_iters), 4(_captures), 5(_block)
#define __emit__for_1(__len, __step, __iter_ids, _iters, _captures, _block...) ({ \
    __for_1__initIters(__for__expandIterIds __iter_ids, __for__expandIters _iters); \
    const usize __len = __for_1__measureLen(__for__expandIterIds __iter_ids, __for__expandIters _iters); \
    for (usize __step = 0; __step < __len; ++__step) { \
        __for_1__captureIters(__step, __for__expandCaptures _captures, __for__expandIterIds __iter_ids, __for__expandIters _iters); \
        __for__expandBlock _block; \
    } \
})
#define __for_1__initIters(__iter_id0, _iter0) \
    __for__initIter(__iter_id0, _iter0)
#define __for_1__measureLen(__iter_id0, _iter0...) \
    __for__lenIter(_iter0)(__iter_id0)
#define __for_1__captureIters(__step, _capture0, __iter_id0, _iter0...) \
    let _capture0 = __for__atIter(__step, __iter_id0, _iter0)

// #define __emit__for_2(__len, __step, __iter_ids, _iters, _captures, _block) \
//     0(__len), 1(__step), 2(__iter_ids), 3(_iters), 4(_captures), 5(_block)
#define __emit__for_2(__len, __step, __iter_ids, _iters, _captures, _block...) ({ \
    __for_2__initIters(__for__expandIterIds __iter_ids, __for__expandIters _iters); \
    const usize __len = __for_2__measureLen(__for__expandIterIds __iter_ids, __for__expandIters _iters); \
    for (usize __step = 0; __step < __len; ++__step) { \
        __for_2__captureIters(__step, __for__expandCaptures _captures, __for__expandIterIds __iter_ids, __for__expandIters _iters); \
        __for__expandBlock _block; \
    } \
})
#define __for_2__initIters(...) __for_2__initItersNext(__VA_ARGS__)
#define __for_2__initItersNext(__iter_id0, __iter_id1, _iter0, _iter1) \
    __for__initIter(__iter_id0, _iter0); \
    __for__initIter(__iter_id1, _iter1)
#define __for_2__measureLen(...) __for_2__measureLenNext(__VA_ARGS__)
#define __for_2__measureLenNext(__iter_id0, __iter_id1, _iter0, _iter1) \
    prim_min2( \
        __for__lenIter(_iter0)(__iter_id0), \
        __for__lenIter(_iter1)(__iter_id1) \
    )
#define __for_2__captureIters(...) __for_2__captureItersNext(__VA_ARGS__)
#define __for_2__captureItersNext(__step, _capture0, _capture1, __iter_id0, __iter_id1, _iter0, _iter1) \
    let _capture0 = __for__atIter(__step, __iter_id0, _iter0); \
    let _capture1 = __for__atIter(__step, __iter_id1, _iter1)

// #define __emit__for_3(__len, __step, __iter_ids, _iters, _captures, _block) \
//     0(__len), 1(__step), 2(__iter_ids), 3(_iters), 4(_captures), 5(_block)
#define __emit__for_3(__len, __step, __iter_ids, _iters, _captures, _block...) ({ \
    __for_3__initIters(__for__expandIterIds __iter_ids, __for__expandIters _iters); \
    const usize __len = __for_3__measureLen(__for__expandIterIds __iter_ids, __for__expandIters _iters); \
    for (usize __step = 0; __step < __len; ++__step) { \
        __for_3__captureIters(__step, __for__expandCaptures _captures, __for__expandIterIds __iter_ids, __for__expandIters _iters); \
        __for__expandBlock _block; \
    } \
})
#define __for_3__initIters(...) __for_3__initItersNext(__VA_ARGS__)
#define __for_3__initItersNext(__iter_id0, __iter_id1, __iter_id2, _iter0, _iter1, _iter2) \
    __for__initIter(__iter_id0, _iter0); \
    __for__initIter(__iter_id1, _iter1); \
    __for__initIter(__iter_id2, _iter2)
#define __for_3__measureLen(...) __for_3__measureLenNext(__VA_ARGS__)
#define __for_3__measureLenNext(__iter_id0, __iter_id1, __iter_id2, _iter0, _iter1, _iter2) \
    prim_min3( \
        __for__lenIter(_iter0)(__iter_id0), \
        __for__lenIter(_iter1)(__iter_id1), \
        __for__lenIter(_iter2)(__iter_id2) \
    )
#define __for_3__captureIters(...) __for_3__captureItersNext(__VA_ARGS__)
#define __for_3__captureItersNext(__step, _capture0, _capture1, _capture2, __iter_id0, __iter_id1, __iter_id2, _iter0, _iter1, _iter2) \
    let _capture0 = __for__atIter(__step, __iter_id0, _iter0); \
    let _capture1 = __for__atIter(__step, __iter_id1, _iter1); \
    let _capture2 = __for__atIter(__step, __iter_id2, _iter2)

// #define __emit__for_4(__len, __step, __iter_ids, _iters, _captures, _block) \
//     0(__len), 1(__step), 2(__iter_ids), 3(_iters), 4(_captures), 5(_block)
#define __emit__for_4(__len, __step, __iter_ids, _iters, _captures, _block...) ({ \
    __for_4__initIters(__for__expandIterIds __iter_ids, __for__expandIters _iters); \
    const usize __len = __for_4__measureLen(__for__expandIterIds __iter_ids, __for__expandIters _iters); \
    for (usize __step = 0; __step < __len; ++__step) { \
        __for_4__captureIters(__step, __for__expandCaptures _captures, __for__expandIterIds __iter_ids, __for__expandIters _iters); \
        __for__expandBlock _block; \
    } \
})
#define __for_4__initIters(...) __for_4__initItersNext(__VA_ARGS__)
#define __for_4__initItersNext(__iter_id0, __iter_id1, __iter_id2, __iter_id3, _iter0, _iter1, _iter2, _iter3) \
    __for__initIter(__iter_id0, _iter0); \
    __for__initIter(__iter_id1, _iter1); \
    __for__initIter(__iter_id2, _iter2); \
    __for__initIter(__iter_id3, _iter3)
#define __for_4__measureLen(...) __for_4__measureLenNext(__VA_ARGS__)
#define __for_4__measureLenNext(__iter_id0, __iter_id1, __iter_id2, __iter_id3, _iter0, _iter1, _iter2, _iter3) \
    prim_min4( \
        __for__lenIter(_iter0)(__iter_id0), \
        __for__lenIter(_iter1)(__iter_id1), \
        __for__lenIter(_iter2)(__iter_id2), \
        __for__lenIter(_iter3)(__iter_id3) \
    )
#define __for_4__captureIters(...) __for_4__captureItersNext(__VA_ARGS__)
#define __for_4__captureItersNext(__step, _capture0, _capture1, _capture2, _capture3, __iter_id0, __iter_id1, __iter_id2, __iter_id3, _iter0, _iter1, _iter2, _iter3) \
    let _capture0 = __for__atIter(__step, __iter_id0, _iter0); \
    let _capture1 = __for__atIter(__step, __iter_id1, _iter1); \
    let _capture2 = __for__atIter(__step, __iter_id2, _iter2); \
    let _capture3 = __for__atIter(__step, __iter_id3, _iter3)

/* for - emitters (reverse) =================================================*/
#define __emitNext__for_$_rev(_iters, _captures, _block) \
    pp_overload(__emitNext__for_$_rev, __for__expandIters _iters)(_iters, _captures, _block)
#define __emitNext__for_$_rev_1(_iters, _captures, _block) __emit__for_$_rev_1( \
    pp_uniqTok(len), pp_uniqTok(step), (pp_uniqTok(iter_id0)), \
    _iters, _captures, _block \
)
#define __emitNext__for_$_rev_2(_iters, _captures, _block) __emit__for_$_rev_2( \
    pp_uniqTok(len), pp_uniqTok(step), \
    ( \
        pp_uniqTok(iter_id0), \
        pp_uniqTok(iter_id1) \
    ), \
    _iters, _captures, _block \
)
#define __emitNext__for_$_rev_3(_iters, _captures, _block) __emit__for_$_rev_3( \
    pp_uniqTok(len), pp_uniqTok(step), \
    ( \
        pp_uniqTok(iter_id0), \
        pp_uniqTok(iter_id1), \
        pp_uniqTok(iter_id2) \
    ), \
    _iters, _captures, _block \
)
#define __emitNext__for_$_rev_4(_iters, _captures, _block) __emit__for_$_rev_4( \
    pp_uniqTok(len), pp_uniqTok(step), \
    ( \
        pp_uniqTok(iter_id0), \
        pp_uniqTok(iter_id1), \
        pp_uniqTok(iter_id2), \
        pp_uniqTok(iter_id3) \
    ), \
    _iters, _captures, _block \
)

// #define __emit__for_$_rev_1(__len, __step, __iter_ids, _iters, _captures, _block) \
//     0(__len), 1(__step), 2(__iter_ids), 3(_iters), 4(_captures), 5(_block)
#define __emit__for_$_rev_1(__len, __step, __iter_ids, _iters, _captures, _block...) ({ \
    __for_1__initIters(__for__expandIterIds __iter_ids, __for__expandIters _iters); \
    const usize __len = __for_1__measureLen(__for__expandIterIds __iter_ids, __for__expandIters _iters); \
    for (usize __step = __len; __step > 0; --__step) { \
        __for_1__captureIters(__step - 1, __for__expandCaptures _captures, __for__expandIterIds __iter_ids, __for__expandIters _iters); \
        __for__expandBlock _block; \
    } \
})

// #define __emit__for_$_rev_2(__len, __step, __iter_ids, _iters, _captures, _block) \
//     0(__len), 1(__step), 2(__iter_ids), 3(_iters), 4(_captures), 5(_block)
#define __emit__for_$_rev_2(__len, __step, __iter_ids, _iters, _captures, _block...) ({ \
    __for_2__initIters(__for__expandIterIds __iter_ids, __for__expandIters _iters); \
    const usize __len = __for_2__measureLen(__for__expandIterIds __iter_ids, __for__expandIters _iters); \
    for (usize __step = __len; __step > 0; --__step) { \
        __for_2__captureIters(__step - 1, __for__expandCaptures _captures, __for__expandIterIds __iter_ids, __for__expandIters _iters); \
        __for__expandBlock _block; \
    } \
})

// #define __emit__for_$_rev_3(__len, __step, __iter_ids, _iters, _captures, _block) \
//     0(__len), 1(__step), 2(__iter_ids), 3(_iters), 4(_captures), 5(_block)
#define __emit__for_$_rev_3(__len, __step, __iter_ids, _iters, _captures, _block...) ({ \
    __for_3__initIters(__for__expandIterIds __iter_ids, __for__expandIters _iters); \
    const usize __len = __for_3__measureLen(__for__expandIterIds __iter_ids, __for__expandIters _iters); \
    for (usize __step = __len; __step > 0; --__step) { \
        __for_3__captureIters(__step - 1, __for__expandCaptures _captures, __for__expandIterIds __iter_ids, __for__expandIters _iters); \
        __for__expandBlock _block; \
    } \
})

// #define __emit__for_$_rev_4(__len, __step, __iter_ids, _iters, _captures, _block) \
//     0(__len), 1(__step), 2(__iter_ids), 3(_iters), 4(_captures), 5(_block)
#define __emit__for_$_rev_4(__len, __step, __iter_ids, _iters, _captures, _block...) ({ \
    __for_4__initIters(__for__expandIterIds __iter_ids, __for__expandIters _iters); \
    const usize __len = __for_4__measureLen(__for__expandIterIds __iter_ids, __for__expandIters _iters); \
    for (usize __step = __len; __step > 0; --__step) { \
        __for_4__captureIters(__step - 1, __for__expandCaptures _captures, __for__expandIterIds __iter_ids, __for__expandIters _iters); \
        __for__expandBlock _block; \
    } \
})

#if defined(__cplusplus)
/* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* SCOPE_COMMON_INCLUDED */

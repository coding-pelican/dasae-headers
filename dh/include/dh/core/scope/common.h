/**
 * @copyright Copyright (c) 2025-2026 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    common.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2025-01-13 (date of creation)
 * @updated 2026-01-24 (date of last update)
 * @ingroup dasae-headers(dh)/core/scope
 * @prefix  (none)
 *
 * @brief   Common scope management utilities
 * @details Provides macros and functions for scope-based resource management:
 *          - Scoped variable declarations and initialization
 *          - Resource acquisition and cleanup
 *          - Conditional scope execution
 *          - Loop constructs with initialization
 */
#ifndef core_scope_common__included
#define core_scope_common__included 1
#if defined(__cplusplus)
extern "C"
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "../pri.h"
#include "../range.h"

/*========== Macros and Definitions =========================================*/

/* using: declarations ======================================================*/
#define using_(_Init...) __step__using_(_Init)
#define using_fini_(_Init, _Fini...) __step__using_fini_(_Init, _Fini)

/* if-else: declarations ====================================================*/
#define if_(_Init, _Cond) __step__if_(_Init, _Cond)
#define else_(_Init...) __step__else_(_Init)

/* for: declaration =========================================================*/
#define $a(_a...) ($A, (_a))
#define $s(_s...) ($S, (_s))
#define $ua(_a...) ($u_A, (_a))
#define $us(_s...) ($u_S, (_s))

#define $fwd $_fwd,
#define $bwd $_bwd,
#define $rev $_rev,
#define $asc $_asc,
#define $desc $_desc,

/* TODO: add $scope-$unscoped version that doesn't require curly braces inside the calling */
#define for_(/*<$dir_type>(_iter...)(_capture...)*/...) pp_overload(__for, __VA_ARGS__)(__VA_ARGS__)
#define __for_1(/*(_iter...)(_capture...)*/...) __exec__for_(pp_defer(__emit__for_$_fwd)(__sep0__for_ __VA_ARGS__))
#define __for_2(_$dir_type, /*(_iter...)(_capture...)*/...) __exec__for_(pp_defer(pp_cat(__emit__for_, _$dir_type))(__sep0__for_ __VA_ARGS__))
#define __exec__for_(...) __VA_ARGS__
#define __sep0__for_(_iters...) (_iters), __sep1__for_
#define __sep1__for_(_captures...) (_captures)
#define __emit__for_$_fwd(_iters, _captures...) __emitNext__for_(_iters, _captures)
// #define __emitNext__for_(_iters, _captures, _block) \
//     0(_iters), 1(_captures), 2(_block)
#define __emit__for_$_bwd(_iters, _captures...) __emitNext__for_$_rev(_iters, _captures)
// #define __emitNext__for_$_bwd(_iters, _captures, _block) \
//     0(_iters), 1(_captures), 2(_block)
#define __emit__for_$_rev(_iters, _captures...) __emitNext__for_$_rev(_iters, _captures)
// #define __emitNext__for_$_rev(_iters, _captures, _block) \
//     0(_iters), 1(_captures), 2(_block)
#define __emit__for_$_asc(_iters, _captures...) __emitNext__for_(_iters, _captures)
#define __emit__for_$_desc(_iters, _captures...) __emitNext__for_$_rev(_iters, _captures)
#define $end_for __stmt__$end_for

#define loop_labeled(_label, _stmt...) __stmt__loop_labeled(_label, _stmt)
#define loop_continue_(_label...) __stmt__loop_continue_(_label)
#define loop_break_(_label...) __stmt__loop_break_(_label)
#define $end_loop_labeled __stmt__$end_loop_labeled
#define __stmt__loop_labeled(_label, _stmt...) \
    _stmt { \
        local_label pp_cat(__continue_, _label); \
        local_label pp_cat(__break_, _label); \
        if (false) loop_continue_(_label); \
        if (false) pp_cat(__continue_, _label) \
            : continue; \
        if (false) loop_break_(_label); \
        if (false) pp_cat(__break_, _label) \
            : break;
#define __stmt__loop_continue_(_label...) goto pp_cat(__continue_, _label)
#define __stmt__loop_break_(_label...) goto pp_cat(__break_, _label)
#define __stmt__$end_loop_labeled \
    }

#define local_labeled(_label, _BreakType_and_Body...) pp_overload(__local_, _BreakType_and_Body)(_label, _BreakType_and_Body)
#define __local_labeled1(_label, _Body...) comp_syn__local_labeled((_label, Void), _Body)
#define __local_labeled2(_label, _RetType, _Body...) comp_syn__local_labeled((_label, _RetType), _Body)
#define local_break_(_label, ...) comp_syn__local_break_(pp_cat(__reserved_val_, _label), _label, __VA_ARGS__)
// clang-format off
#define comp_syn__local_labeled(_Label_and_RetType, _Body...) local_({ \
    local_label pp_Tuple_get1st _Label_and_RetType; \
    var pp_cat(__reserved_val_, pp_Tuple_get1st _Label_and_RetType) = _Generic( \
        TypeOf(pp_Tuple_get2nd _Label_and_RetType), \
        void: (Void){}, \
        default: (pp_Tuple_get2nd _Label_and_RetType){} \
    ); \
    _Body; \
    pp_Tuple_get1st _Label_and_RetType: \
    local_return_(pp_cat(__reserved_val_, pp_Tuple_get1st _Label_and_RetType)); \
})
#define comp_syn__local_break_(__reserved_val, _label, ...) \
    __reserved_val = *(TypeOf(__reserved_val)[1]){ [0] = __VA_ARGS__ }; \
    goto _label
// clang-format on

/* while: declaration =======================================================*/
#define while_(_Init, _Cond, /*_Cont*/...) scope_while(_Init, _Cond, __VA_OPT__(, ) __VA_ARGS__)

/* switch: declaration ======================================================*/
// #define switch_(_Init, _Cond, /*_Body*/...) scope_switch(_Init, _Cond, __VA_ARGS__)

// case, default, ...

/*========== Macros Implementation ==========================================*/

#define __step__using_(_Init...) ____using_(pp_uniqTok(run_once), _Init)
#define __step__using_fini_(_Init, _Fini...) ____using_fini_(pp_uniqTok(run_once), _Init, _Fini)
#define __step__using_var_(_Init...) ____using_var_(pp_uniqTok(run_once), _Init)
#define __step__using_let_(_Init...) ____using_let_(pp_uniqTok(run_once), _Init)
#define __step__if_(_Init, _Cond) ____if_(pp_uniqTok(run_once), _Init, _Cond)
#define __step__else_(_Init...) ____else_(pp_uniqTok(run_once), _Init)

#define scope_switch(_Init, _Cond, ...) SYN__scope_switch(_Init, _Cond, __VA_ARGS__)
#define scope_while(_Init, _Cond, ...) SYN__scope_while(_Init, _Cond __VA_OPT__(, ) __VA_ARGS__)
#define scope_va_list(_Init) /* TODO: Implement scope_va_list */

/* NOLINTBEGIN */
#define ____using_(__run_once, _Init...) \
    for (bool __run_once = false; !__run_once;) \
        for (_Init; !__run_once; __run_once = true)
#define ____using_fini_(__run_once, _Init, _Fini...) \
    for (bool __run_once = false; !__run_once;) \
        for (_Init; !__run_once; ({ \
                 __run_once = true; \
                 _Fini; \
             }))
#define ____using_var_(__run_once, _Init...) \
    for (bool __run_once = false; !__run_once;) \
        for (var _Init; !__run_once; __run_once = true)
#define ____using_let_(__run_once, _Init...) \
    for (bool __run_once = false; !__run_once;) \
        for (let _Init; !__run_once; __run_once = true)
#define ____if_(__run_once, _Init, _Cond) ____using_(__run_once, _Init) if (_Cond)
#define ____else_(__run_once, _Init...) else ____using_(__run_once, _Init)

#define SYN__scope_switch(_Init, _Cond, ...) \
    ({ \
        _Init; \
        switch (_Cond) \
            __VA_ARGS__ \
    })

#define SYN__scope_while(_Init, _Cond, _Cont...) \
    for (_Init; _Cond; _Cont) /* NOLINTEND */

/* for: implementation ======================================================*/
/* for - common functions ===================================================*/
#define __for__expandIters(_iters...) _iters
#define __for__expandIter(/*<_iter>|<_tag,_iter>*/...) __VA_ARGS__
#define __for__expandIterIds(__ids...) __ids
#define __for__expandIterId(__id) __id
#define __for__expandCaptures(_captures...) _captures
#define __for__expandCapture(/*<_capture>|$ignore*/...) __VA_ARGS__

#define __for__initIter(__iter_id, _iter) pp_overload(__for__initIter, __for__expandIter _iter)(__iter_id, __for__expandIter _iter)
#define __for__initIter_1(__iter_id, ...) __for__initIter_1Emit(__iter_id, __VA_ARGS__)
#define __for__initIter_1Emit(__iter_id, _iter) const R __iter_id = _iter
#define __for__initIter_2(__iter_id, ...) __for__initIter_2Emit(__iter_id, __VA_ARGS__)
#define __for__initIter_2Emit(__iter_id, _tag, _iter...) let __iter_id = _iter

#define __for__lenIter(_iter) pp_overload(__for__lenIter, __for__expandIter _iter)(__for__expandIter _iter)
#define __for__lenIter_1(...) __for__lenIter_1Emit(__VA_ARGS__)
#define __for__lenIter_1Emit(_iter) len$R
#define __for__lenIter_2(...) __for__lenIter_2Emit(__VA_ARGS__)
#define __for__lenIter_2Emit(_tag, _iter) pp_cat(len, _tag)

#define __for__atIter(__step, __iter_id, _iter) pp_overload(__for__atIter, __for__expandIter _iter)(__step, __iter_id, __for__expandIter _iter)
#define __for__atIter_1(...) __for__atIter_1Emit(__VA_ARGS__)
#define __for__atIter_1Emit(__step, __iter_id, _iter) at$R(__iter_id, __step)
#define __for__atIter_2(...) __for__atIter_2Emit(__VA_ARGS__)
#define __for__atIter_2Emit(__step, __iter_id, _tag, _iter) pp_cat(at, _tag)(__iter_id, __step)

/* for - emitters (forward) =================================================*/
#define __emitNext__for_(_iters, _captures...) \
    pp_overload(__emitNext__for, __for__expandIters _iters)(_iters, _captures)
#define __emitNext__for_1(_iters, _captures...) \
    __emit__for_1( \
        pp_uniqTok(len), pp_uniqTok(step), \
        (pp_uniqTok(iter_id0)), \
        _iters, _captures \
    )
#define __emitNext__for_2(_iters, _captures...) \
    __emit__for_2( \
        pp_uniqTok(len), pp_uniqTok(step), \
        (pp_uniqTok(iter_id0), pp_uniqTok(iter_id1)), \
        _iters, _captures \
    )
#define __emitNext__for_3(_iters, _captures...) \
    __emit__for_3( \
        pp_uniqTok(len), pp_uniqTok(step), \
        (pp_uniqTok(iter_id0), pp_uniqTok(iter_id1), pp_uniqTok(iter_id2)), \
        _iters, _captures \
    )
#define __emitNext__for_4(_iters, _captures...) \
    __emit__for_4( \
        pp_uniqTok(len), pp_uniqTok(step), \
        (pp_uniqTok(iter_id0), pp_uniqTok(iter_id1), pp_uniqTok(iter_id2), pp_uniqTok(iter_id3)), \
        _iters, _captures \
    )

// #define __emit__for_1(__len, __step, __iter_ids, _iters, _captures, _block) \
//     0(__len), 1(__step), 2(__iter_ids), 3(_iters), 4(_captures), 5(_block)
#define __emit__for_1(__len, __step, __iter_ids, _iters, _captures...) \
    { \
        __for_1__initIters(__for__expandIterIds __iter_ids, __for__expandIters _iters); \
        const usize __len = __for_1__measureLen(__for__expandIterIds __iter_ids, __for__expandIters _iters); \
        for (usize __step = 0; __step < __len; ++__step) { \
            __for_1__captureIters(__step, __for__expandCaptures _captures, __for__expandIterIds __iter_ids, __for__expandIters _iters);

#define __for_1__initIters(__iter_id0, _iter0) __for__initIter(__iter_id0, _iter0)
#define __for_1__measureLen(__iter_id0, _iter0...) __for__lenIter(_iter0)(__iter_id0)
#define __for_1__captureIters(__step, _capture0, __iter_id0, _iter0...) let _capture0 = __for__atIter(__step, __iter_id0, _iter0)

// #define __emit__for_2(__len, __step, __iter_ids, _iters, _captures, _block) \
//     0(__len), 1(__step), 2(__iter_ids), 3(_iters), 4(_captures), 5(_block)
#define __emit__for_2(__len, __step, __iter_ids, _iters, _captures...) \
    { \
        __for_2__initIters(__for__expandIterIds __iter_ids, __for__expandIters _iters); \
        const usize __len = __for_2__measureLen(__for__expandIterIds __iter_ids, __for__expandIters _iters); \
        for (usize __step = 0; __step < __len; ++__step) { \
            __for_2__captureIters(__step, __for__expandCaptures _captures, __for__expandIterIds __iter_ids, __for__expandIters _iters);

#define __for_2__initIters(...) __for_2__initItersNext(__VA_ARGS__)
#define __for_2__initItersNext(__iter_id0, __iter_id1, _iter0, _iter1) \
    __for__initIter(__iter_id0, _iter0); \
    __for__initIter(__iter_id1, _iter1)
#define __for_2__measureLen(...) __for_2__measureLenNext(__VA_ARGS__)
#define __for_2__measureLenNext(__iter_id0, __iter_id1, _iter0, _iter1) pri_min2(__for__lenIter(_iter0)(__iter_id0), __for__lenIter(_iter1)(__iter_id1))
#define __for_2__captureIters(...) __for_2__captureItersNext(__VA_ARGS__)
#define __for_2__captureItersNext(__step, _capture0, _capture1, __iter_id0, __iter_id1, _iter0, _iter1) \
    let _capture0 = __for__atIter(__step, __iter_id0, _iter0); \
    let _capture1 = __for__atIter(__step, __iter_id1, _iter1)

// #define __emit__for_3(__len, __step, __iter_ids, _iters, _captures, _block) \
//     0(__len), 1(__step), 2(__iter_ids), 3(_iters), 4(_captures), 5(_block)
#define __emit__for_3(__len, __step, __iter_ids, _iters, _captures...) \
    { \
        __for_3__initIters(__for__expandIterIds __iter_ids, __for__expandIters _iters); \
        const usize __len = __for_3__measureLen(__for__expandIterIds __iter_ids, __for__expandIters _iters); \
        for (usize __step = 0; __step < __len; ++__step) { \
            __for_3__captureIters(__step, __for__expandCaptures _captures, __for__expandIterIds __iter_ids, __for__expandIters _iters);

#define __for_3__initIters(...) __for_3__initItersNext(__VA_ARGS__)
#define __for_3__initItersNext(__iter_id0, __iter_id1, __iter_id2, _iter0, _iter1, _iter2) \
    __for__initIter(__iter_id0, _iter0); \
    __for__initIter(__iter_id1, _iter1); \
    __for__initIter(__iter_id2, _iter2)
#define __for_3__measureLen(...) __for_3__measureLenNext(__VA_ARGS__)
#define __for_3__measureLenNext(__iter_id0, __iter_id1, __iter_id2, _iter0, _iter1, _iter2) \
    pri_min3(__for__lenIter(_iter0)(__iter_id0), __for__lenIter(_iter1)(__iter_id1), __for__lenIter(_iter2)(__iter_id2))
#define __for_3__captureIters(...) __for_3__captureItersNext(__VA_ARGS__)
#define __for_3__captureItersNext(__step, _capture0, _capture1, _capture2, __iter_id0, __iter_id1, __iter_id2, _iter0, _iter1, _iter2) \
    let _capture0 = __for__atIter(__step, __iter_id0, _iter0); \
    let _capture1 = __for__atIter(__step, __iter_id1, _iter1); \
    let _capture2 = __for__atIter(__step, __iter_id2, _iter2)

// #define __emit__for_4(__len, __step, __iter_ids, _iters, _captures, _block) \
//     0(__len), 1(__step), 2(__iter_ids), 3(_iters), 4(_captures), 5(_block)
#define __emit__for_4(__len, __step, __iter_ids, _iters, _captures...) \
    { \
        __for_4__initIters(__for__expandIterIds __iter_ids, __for__expandIters _iters); \
        const usize __len = __for_4__measureLen(__for__expandIterIds __iter_ids, __for__expandIters _iters); \
        for (usize __step = 0; __step < __len; ++__step) { \
            __for_4__captureIters(__step, __for__expandCaptures _captures, __for__expandIterIds __iter_ids, __for__expandIters _iters);

#define __for_4__initIters(...) __for_4__initItersNext(__VA_ARGS__)
#define __for_4__initItersNext(__iter_id0, __iter_id1, __iter_id2, __iter_id3, _iter0, _iter1, _iter2, _iter3) \
    __for__initIter(__iter_id0, _iter0); \
    __for__initIter(__iter_id1, _iter1); \
    __for__initIter(__iter_id2, _iter2); \
    __for__initIter(__iter_id3, _iter3)
#define __for_4__measureLen(...) __for_4__measureLenNext(__VA_ARGS__)
#define __for_4__measureLenNext(__iter_id0, __iter_id1, __iter_id2, __iter_id3, _iter0, _iter1, _iter2, _iter3) \
    pri_min4(__for__lenIter(_iter0)(__iter_id0), __for__lenIter(_iter1)(__iter_id1), __for__lenIter(_iter2)(__iter_id2), __for__lenIter(_iter3)(__iter_id3))
#define __for_4__captureIters(...) __for_4__captureItersNext(__VA_ARGS__)
#define __for_4__captureItersNext( \
    __step, _capture0, _capture1, _capture2, _capture3, __iter_id0, __iter_id1, __iter_id2, __iter_id3, _iter0, _iter1, _iter2, _iter3 \
) \
    let _capture0 = __for__atIter(__step, __iter_id0, _iter0); \
    let _capture1 = __for__atIter(__step, __iter_id1, _iter1); \
    let _capture2 = __for__atIter(__step, __iter_id2, _iter2); \
    let _capture3 = __for__atIter(__step, __iter_id3, _iter3)

/* for - emitters (reverse) =================================================*/
#define __emitNext__for_$_rev(_iters, _captures...) \
    pp_overload(__emitNext__for_$_rev, __for__expandIters _iters)(_iters, _captures)
#define __emitNext__for_$_rev_1(_iters, _captures...) \
    __emit__for_$_rev_1( \
        pp_uniqTok(len), pp_uniqTok(step), \
        (pp_uniqTok(iter_id0)), \
        _iters, _captures \
    )
#define __emitNext__for_$_rev_2(_iters, _captures...) \
    __emit__for_$_rev_2( \
        pp_uniqTok(len), pp_uniqTok(step), \
        (pp_uniqTok(iter_id0), pp_uniqTok(iter_id1)), \
        _iters, _captures \
    )
#define __emitNext__for_$_rev_3(_iters, _captures...) \
    __emit__for_$_rev_3( \
        pp_uniqTok(len), pp_uniqTok(step), \
        (pp_uniqTok(iter_id0), pp_uniqTok(iter_id1), pp_uniqTok(iter_id2)), \
        _iters, _captures \
    )
#define __emitNext__for_$_rev_4(_iters, _captures...) \
    __emit__for_$_rev_4( \
        pp_uniqTok(len), pp_uniqTok(step), \
        (pp_uniqTok(iter_id0), pp_uniqTok(iter_id1), pp_uniqTok(iter_id2), pp_uniqTok(iter_id3)), \
        _iters, _captures \
    )

// #define __emit__for_$_rev_1(__len, __step, __iter_ids, _iters, _captures, _block) \
//     0(__len), 1(__step), 2(__iter_ids), 3(_iters), 4(_captures), 5(_block)
#define __emit__for_$_rev_1(__len, __step, __iter_ids, _iters, _captures...) \
    { \
        __for_1__initIters(__for__expandIterIds __iter_ids, __for__expandIters _iters); \
        const usize __len = __for_1__measureLen(__for__expandIterIds __iter_ids, __for__expandIters _iters); \
        for (usize __step = __len; __step-- > 0;) { \
            __for_1__captureIters(__step, __for__expandCaptures _captures, __for__expandIterIds __iter_ids, __for__expandIters _iters);

// #define __emit__for_$_rev_2(__len, __step, __iter_ids, _iters, _captures, _block) \
//     0(__len), 1(__step), 2(__iter_ids), 3(_iters), 4(_captures), 5(_block)
#define __emit__for_$_rev_2(__len, __step, __iter_ids, _iters, _captures...) \
    { \
        __for_2__initIters(__for__expandIterIds __iter_ids, __for__expandIters _iters); \
        const usize __len = __for_2__measureLen(__for__expandIterIds __iter_ids, __for__expandIters _iters); \
        for (usize __step = __len; __step-- > 0;) { \
            __for_2__captureIters(__step, __for__expandCaptures _captures, __for__expandIterIds __iter_ids, __for__expandIters _iters);

// #define __emit__for_$_rev_3(__len, __step, __iter_ids, _iters, _captures, _block) \
//     0(__len), 1(__step), 2(__iter_ids), 3(_iters), 4(_captures), 5(_block)
#define __emit__for_$_rev_3(__len, __step, __iter_ids, _iters, _captures...) \
    { \
        __for_3__initIters(__for__expandIterIds __iter_ids, __for__expandIters _iters); \
        const usize __len = __for_3__measureLen(__for__expandIterIds __iter_ids, __for__expandIters _iters); \
        for (usize __step = __len; __step-- > 0;) { \
            __for_3__captureIters(__step, __for__expandCaptures _captures, __for__expandIterIds __iter_ids, __for__expandIters _iters);

// #define __emit__for_$_rev_4(__len, __step, __iter_ids, _iters, _captures, _block) \
//     0(__len), 1(__step), 2(__iter_ids), 3(_iters), 4(_captures), 5(_block)
#define __emit__for_$_rev_4(__len, __step, __iter_ids, _iters, _captures...) \
    { \
        __for_4__initIters(__for__expandIterIds __iter_ids, __for__expandIters _iters); \
        const usize __len = __for_4__measureLen(__for__expandIterIds __iter_ids, __for__expandIters _iters); \
        for (usize __step = __len; __step-- > 0;) { \
            __for_4__captureIters(__step, __for__expandCaptures _captures, __for__expandIterIds __iter_ids, __for__expandIters _iters);

#define __stmt__$end_for \
    } \
    }

#if defined(__cplusplus)
/* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* core_scope_common__included */

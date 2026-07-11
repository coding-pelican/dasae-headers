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
#pragma once
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
#define using_(_$Init...) __step__using_(_$Init)
#define using_fini_(_$Init, _$Fini...) __step__using_fini_(_$Init, _$Fini)

/* if-else: declarations ====================================================*/
#define if_(_$Init, _$Cond) __step__if_(_$Init, _$Cond)
#define else_(_$Init...) __step__else_(_$Init)

#define when_(_$cond...) /*(provide_(_$then...) instead_(_$else...))*/ __inline__when_(_$cond)
#define __inline__when_(_$cond...) (_$cond) __step__when___parseProvideInstead
#define __step__when___parseProvideInstead(_provide, _instead...) _provide _instead
#define provide_(_$then...) ? (_$then)
#define instead_(_$else...) : (_$else)

/* for: declaration =========================================================*/
#define $a(_$a...) ($A, (_$a))
#define $s(_$s...) ($S, (_$s))
#define $ua(_$a...) ($u_A, (_$a))
#define $us(_$s...) ($u_S, (_$s))

#define $fwd $_fwd,
#define $bwd $_bwd,
#define $rev $_rev,
#define $asc $_asc,
#define $desc $_desc,

#define $__hint_loop_none $___hint_loop_none
#define $__hint_loop_unroll $___hint_loop_unroll
#define $__hint_loop_inline $___hint_loop_inline
#define $__hint_loop_rolled $___hint_loop_rolled

/* TODO: add $scope-$unscoped version that doesn't require curly braces inside the calling */
#define for_(/*<$dir_type>(_$iter...)(_$capture...)*/...) pp_overload(__for, __VA_ARGS__)($__hint_loop_none, __VA_ARGS__)
#define __for_1(_$__hint_loop, /*(_$iter...)(_$capture...)*/...) \
    __exec__for_(pp_defer(__emit__for_$_fwd)(_$__hint_loop, __sep0__for_ __VA_ARGS__))
#define __for_2(_$__hint_loop, _$dir_type, /*(_$iter...)(_$capture...)*/...) \
    __exec__for_(pp_defer(pp_cat(__emit__for_, _$dir_type))(_$__hint_loop, __sep0__for_ __VA_ARGS__))
#define __exec__for_(...) __VA_ARGS__
#define __sep0__for_(_$iters...) (_$iters), __sep1__for_
#define __sep1__for_(_$captures...) (_$captures)
#define __emit__for_$_fwd(_$__hint_loop, _$iters, _$captures...) __emitNext__for_(_$__hint_loop, _$iters, _$captures)
// #define __emitNext__for_(_$iters, _$captures, _block) \
//     0(_$iters), 1(_$captures), 2(_block)
#define __emit__for_$_bwd(_$__hint_loop, _$iters, _$captures...) __emitNext__for_$_rev(_$__hint_loop, _$iters, _$captures)
// #define __emitNext__for_$_bwd(_$iters, _$captures, _block) \
//     0(_$iters), 1(_$captures), 2(_block)
#define __emit__for_$_rev(_$__hint_loop, _$iters, _$captures...) __emitNext__for_$_rev(_$__hint_loop, _$iters, _$captures)
// #define __emitNext__for_$_rev(_$iters, _$captures, _block) \
//     0(_$iters), 1(_$captures), 2(_block)
#define __emit__for_$_asc(_$__hint_loop, _$iters, _$captures...) __emitNext__for_(_$__hint_loop, _$iters, _$captures)
#define __emit__for_$_desc(_$__hint_loop, _$iters, _$captures...) __emitNext__for_$_rev(_$__hint_loop, _$iters, _$captures)
#define $end_for __stmt__$end_for

#define loop_unroll_(_$loop_type...) pp_cat(__loop_unroll_, _$loop_type)()
#define loop_inline_(_$loop_type...) pp_cat(__loop_inline_, _$loop_type)()
#define loop_rolled_(_$loop_type...) pp_cat(__loop_rolled_, _$loop_type)()

#define loop_labeled(_$label, _$stmt...) __stmt__loop_labeled(_$label, _$stmt)
#define loop_continue_(_$label...) __stmt__loop_continue_(_$label)
#define loop_break_(_$label...) __stmt__loop_break_(_$label)
#define $end_loop_labeled __stmt__$end_loop_labeled
#define __stmt__loop_labeled(_$label, _$stmt...) \
    _$stmt { \
        local_label pp_cat(__continue_, _$label); \
        local_label pp_cat(__break_, _$label); \
        if (false) loop_continue_(_$label); \
        if (false) pp_cat(__continue_, _$label) \
            : continue; \
        if (false) loop_break_(_$label); \
        if (false) pp_cat(__break_, _$label) \
            : break;
#define __stmt__loop_continue_(_$label...) goto pp_cat(__continue_, _$label)
#define __stmt__loop_break_(_$label...) goto pp_cat(__break_, _$label)
#define __stmt__$end_loop_labeled \
    }

#define local_labeled(_$label, _$BreakType_and_Body...) pp_overload(__local_, _$BreakType_and_Body)(_$label, _$BreakType_and_Body)
#define __local_labeled1(_$label, _$Body...) comp_syn__local_labeled((_$label, Void), _$Body)
#define __local_labeled2(_$label, _$RetType, _$Body...) comp_syn__local_labeled((_$label, _$RetType), _$Body)
#define local_break_(_$label, ...) comp_syn__local_break_(pp_cat(__reserved_val_, _$label), _$label, __VA_ARGS__)
// clang-format off
#define comp_syn__local_labeled(_$Label_and_RetType, _$Body...) local_({ \
    local_label pp_Tuple_get1st _$Label_and_RetType; \
    var pp_cat(__reserved_val_, pp_Tuple_get1st _$Label_and_RetType) = _Generic( \
        TypeOf(pp_Tuple_get2nd _$Label_and_RetType), \
        void: (Void){}, \
        default: (pp_Tuple_get2nd _$Label_and_RetType){} \
    ); \
    _$Body; \
    pp_Tuple_get1st _$Label_and_RetType: \
    local_return_(pp_cat(__reserved_val_, pp_Tuple_get1st _$Label_and_RetType)); \
})
#define comp_syn__local_break_(__reserved_val, _$label, ...) \
    __reserved_val = *(TypeOf(__reserved_val)[1]){ [0] = __VA_ARGS__ }; \
    goto _$label
// clang-format on

/* while: declaration =======================================================*/
#define while_(_$Init, _$Cond, /*_$Cont*/...) scope_while(_$Init, _$Cond __VA_OPT__(, __VA_ARGS__))

/* switch: declaration ======================================================*/
// #define switch_(_$Init, _$Cond, /*_$Body*/...) scope_switch(_$Init, _$Cond, __VA_ARGS__)

// case, default, ...

/*========== Macros Implementation ==========================================*/

#define __step__using_(_$Init...) ____using_(pp_uniqTok(run_once), _$Init)
#define __step__using_fini_(_$Init, _$Fini...) ____using_fini_(pp_uniqTok(run_once), _$Init, _$Fini)
#define __step__using_var_(_$Init...) ____using_var_(pp_uniqTok(run_once), _$Init)
#define __step__using_let_(_$Init...) ____using_let_(pp_uniqTok(run_once), _$Init)
#define __step__if_(_$Init, _$Cond) ____if_(pp_uniqTok(run_once), _$Init, _$Cond)
#define __step__else_(_$Init...) ____else_(pp_uniqTok(run_once), _$Init)

#define scope_switch(_$Init, _$Cond, ...) SYN__scope_switch(_$Init, _$Cond, __VA_ARGS__)
#define scope_while(_$Init, _$Cond, ...) SYN__scope_while(_$Init, _$Cond __VA_OPT__(, ) __VA_ARGS__)

/* NOLINTBEGIN */
#define ____using_(__run_once, _$Init...) \
    for (bool __run_once = false; !__run_once;) \
        for (_$Init; !__run_once; __run_once = true)
#define ____using_fini_(__run_once, _$Init, _$Fini...) \
    for (bool __run_once = false; !__run_once;) \
        for (_$Init; !__run_once; ({ \
                 __run_once = true; \
                 _$Fini; \
             }))
#define ____using_var_(__run_once, _$Init...) \
    for (bool __run_once = false; !__run_once;) \
        for (var _$Init; !__run_once; __run_once = true)
#define ____using_let_(__run_once, _$Init...) \
    for (bool __run_once = false; !__run_once;) \
        for (let _$Init; !__run_once; __run_once = true)
#define ____if_(__run_once, _$Init, _$Cond) ____using_(__run_once, _$Init) if (_$Cond)
#define ____else_(__run_once, _$Init...) else ____using_(__run_once, _$Init)

#define SYN__scope_switch(_$Init, _$Cond, ...) \
    ({ \
        _$Init; \
        switch (_$Cond) \
            __VA_ARGS__ \
    })

#define SYN__scope_while(_$Init, _$Cond, _$Cont...) \
    for (_$Init; _$Cond; _$Cont) /* NOLINTEND */

/* for: implementation ======================================================*/
/* for - common functions ===================================================*/
#define __for__emitHintLoop(_$__hint_loop) pp_cat(__for__emitHintLoop, _$__hint_loop)
#define __for__emitHintLoop$___hint_loop_none(...)
#define __for__emitHintLoop$___hint_loop_unroll(...) $loop_unroll
#define __for__emitHintLoop$___hint_loop_inline(...) $loop_inline
#define __for__emitHintLoop$___hint_loop_rolled(...) $loop_rolled

#define __for__expandIters(_$iters...) _$iters
#define __for__expandIter(/*<_$iter>|<_$tag,_$iter>*/...) __VA_ARGS__
#define __for__expandIterIds(__ids...) __ids
#define __for__expandIterId(__id) __id
#define __for__expandCaptures(_$captures...) _$captures
#define __for__expandCapture(/*<_$capture>|$ignore*/...) __VA_ARGS__

#define __for__initIter(__iter_id, _$iter) \
    pp_overload(__for__initIter, __for__expandIter _$iter)(__iter_id, __for__expandIter _$iter)
#define __for__initIter_1(__iter_id, ...) __for__initIter_1Emit(__iter_id, __VA_ARGS__)
#define __for__initIter_1Emit(__iter_id, _$iter) const R __iter_id = _$iter
#define __for__initIter_2(__iter_id, ...) __for__initIter_2Emit(__iter_id, __VA_ARGS__)
#define __for__initIter_2Emit(__iter_id, _$tag, _$iter...) let __iter_id = _$iter

#define __for__lenIter(_$iter) pp_overload(__for__lenIter, __for__expandIter _$iter)(__for__expandIter _$iter)
#define __for__lenIter_1(...) __for__lenIter_1Emit(__VA_ARGS__)
#define __for__lenIter_1Emit(_$iter) R_len
#define __for__lenIter_2(...) __for__lenIter_2Emit(__VA_ARGS__)
#define __for__lenIter_2Emit(_$tag, _$iter) pp_cat(__for__lenIter, _$tag)
#define __for__lenIter$A(_$iter) A_len(_$iter)
#define __for__lenIter$S(_$iter) S_len(_$iter)
#define __for__lenIter$u_S(_$iter) u_lenS(_$iter)

#define __for__atIter(__step, __iter_id, _$iter) \
    pp_overload(__for__atIter, __for__expandIter _$iter)(__step, __iter_id, __for__expandIter _$iter)
#define __for__atIter_1(...) __for__atIter_1Emit(__VA_ARGS__)
#define __for__atIter_1Emit(__step, __iter_id, _$iter) R_at(__iter_id, __step)
#define __for__atIter_2(...) __for__atIter_2Emit(__VA_ARGS__)
#define __for__atIter_2Emit(__step, __iter_id, _$tag, _$iter) pp_cat(__for__atIter, _$tag)(__iter_id, __step)
#define __for__atIter$A(__iter_id, __step) A_at((__iter_id)[__step])
#define __for__atIter$S(__iter_id, __step) S_at((__iter_id)[__step])
#define __for__atIter$u_S(__iter_id, __step) u_atS(__iter_id, __step)

/* for - emitters (forward) =================================================*/
#define __emitNext__for_(_$__hint_loop, _$iters, _$captures...) \
    pp_overload(__emitNext__for, __for__expandIters _$iters)(_$__hint_loop, _$iters, _$captures)
#define __emitNext__for_1(_$__hint_loop, _$iters, _$captures...) \
    __emit__for_1( \
        _$__hint_loop, pp_uniqTok(len), pp_uniqTok(step), \
        (pp_uniqTok(iter_id0)), \
        _$iters, _$captures \
    )
#define __emitNext__for_2(_$__hint_loop, _$iters, _$captures...) \
    __emit__for_2( \
        _$__hint_loop, pp_uniqTok(len), pp_uniqTok(step), \
        (pp_uniqTok(iter_id0), pp_uniqTok(iter_id1)), \
        _$iters, _$captures \
    )
#define __emitNext__for_3(_$__hint_loop, _$iters, _$captures...) \
    __emit__for_3( \
        _$__hint_loop, pp_uniqTok(len), pp_uniqTok(step), \
        (pp_uniqTok(iter_id0), pp_uniqTok(iter_id1), pp_uniqTok(iter_id2)), \
        _$iters, _$captures \
    )
#define __emitNext__for_4(_$__hint_loop, _$iters, _$captures...) \
    __emit__for_4( \
        _$__hint_loop, pp_uniqTok(len), pp_uniqTok(step), \
        (pp_uniqTok(iter_id0), pp_uniqTok(iter_id1), pp_uniqTok(iter_id2), pp_uniqTok(iter_id3)), \
        _$iters, _$captures \
    )
#define __emitNext__for_5(_$__hint_loop, _$iters, _$captures...) \
    __emit__for_5( \
        _$__hint_loop, pp_uniqTok(len), pp_uniqTok(step), \
        ( \
            pp_uniqTok(iter_id0), pp_uniqTok(iter_id1), pp_uniqTok(iter_id2), pp_uniqTok(iter_id3), \
            pp_uniqTok(iter_id4) \
        ), \
        _$iters, _$captures \
    )
#define __emitNext__for_6(_$__hint_loop, _$iters, _$captures...) \
    __emit__for_6( \
        _$__hint_loop, pp_uniqTok(len), pp_uniqTok(step), \
        ( \
            pp_uniqTok(iter_id0), pp_uniqTok(iter_id1), pp_uniqTok(iter_id2), pp_uniqTok(iter_id3), \
            pp_uniqTok(iter_id4), pp_uniqTok(iter_id5) \
        ), \
        _$iters, _$captures \
    )
#define __emitNext__for_7(_$__hint_loop, _$iters, _$captures...) \
    __emit__for_7( \
        _$__hint_loop, pp_uniqTok(len), pp_uniqTok(step), \
        ( \
            pp_uniqTok(iter_id0), pp_uniqTok(iter_id1), pp_uniqTok(iter_id2), pp_uniqTok(iter_id3), \
            pp_uniqTok(iter_id4), pp_uniqTok(iter_id5), pp_uniqTok(iter_id6) \
        ), \
        _$iters, _$captures \
    )
#define __emitNext__for_8(_$__hint_loop, _$iters, _$captures...) \
    __emit__for_8( \
        _$__hint_loop, pp_uniqTok(len), pp_uniqTok(step), \
        ( \
            pp_uniqTok(iter_id0), pp_uniqTok(iter_id1), pp_uniqTok(iter_id2), pp_uniqTok(iter_id3), \
            pp_uniqTok(iter_id4), pp_uniqTok(iter_id5), pp_uniqTok(iter_id6), pp_uniqTok(iter_id7) \
        ), \
        _$iters, _$captures \
    )

// #define __emit__for_1(__len, __step, __iter_ids, _$iters, _$captures, _block) \
//     0(__len), 1(__step), 2(__iter_ids), 3(_$iters), 4(_$captures), 5(_block)
#define __emit__for_1(_$__hint_loop, __len, __step, __iter_ids, _$iters, _$captures...) \
    { \
        __for_1__initIters(__for__expandIterIds __iter_ids, __for__expandIters _$iters); \
        const usize __len = __for_1__measureLen(__for__expandIterIds __iter_ids, __for__expandIters _$iters); \
        __for__emitHintLoop(_$__hint_loop)() for (usize __step = 0; __step < __len; ++__step) { \
            __for_1__captureIters( \
                __step, __for__expandCaptures _$captures, __for__expandIterIds __iter_ids, __for__expandIters _$iters \
            );

#define __for_1__initIters(__iter_id0, _$iter0) __for__initIter(__iter_id0, _$iter0)
#define __for_1__measureLen(__iter_id0, _$iter0...) __for__lenIter(_$iter0)(__iter_id0)
#define __for_1__captureIters(__step, _$capture0, __iter_id0, _$iter0...) \
    let _$capture0 = __for__atIter(__step, __iter_id0, _$iter0)

// #define __emit__for_2(__len, __step, __iter_ids, _$iters, _$captures, _block) \
//     0(__len), 1(__step), 2(__iter_ids), 3(_$iters), 4(_$captures), 5(_block)
#define __emit__for_2(_$__hint_loop, __len, __step, __iter_ids, _$iters, _$captures...) \
    { \
        __for_2__initIters(__for__expandIterIds __iter_ids, __for__expandIters _$iters); \
        const usize __len = __for_2__measureLen(__for__expandIterIds __iter_ids, __for__expandIters _$iters); \
        __for__emitHintLoop(_$__hint_loop)() for (usize __step = 0; __step < __len; ++__step) { \
            __for_2__captureIters( \
                __step, __for__expandCaptures _$captures, __for__expandIterIds __iter_ids, __for__expandIters _$iters \
            );

#define __for_2__initIters(...) __for_2__initItersNext(__VA_ARGS__)
#define __for_2__initItersNext(__iter_id0, __iter_id1, _$iter0, _$iter1) \
    __for__initIter(__iter_id0, _$iter0); \
    __for__initIter(__iter_id1, _$iter1)
#define __for_2__measureLen(...) __for_2__measureLenNext(__VA_ARGS__)
#define __for_2__measureLenNext(__iter_id0, __iter_id1, _$iter0, _$iter1) \
    pri_min2(__for__lenIter(_$iter0)(__iter_id0), __for__lenIter(_$iter1)(__iter_id1))
#define __for_2__captureIters(...) __for_2__captureItersNext(__VA_ARGS__)
#define __for_2__captureItersNext(__step, _$capture0, _$capture1, __iter_id0, __iter_id1, _$iter0, _$iter1) \
    let _$capture0 = __for__atIter(__step, __iter_id0, _$iter0); \
    let _$capture1 = __for__atIter(__step, __iter_id1, _$iter1)

// #define __emit__for_3(__len, __step, __iter_ids, _$iters, _$captures, _block) \
//     0(__len), 1(__step), 2(__iter_ids), 3(_$iters), 4(_$captures), 5(_block)
#define __emit__for_3(_$__hint_loop, __len, __step, __iter_ids, _$iters, _$captures...) \
    { \
        __for_3__initIters(__for__expandIterIds __iter_ids, __for__expandIters _$iters); \
        const usize __len = __for_3__measureLen(__for__expandIterIds __iter_ids, __for__expandIters _$iters); \
        __for__emitHintLoop(_$__hint_loop)() for (usize __step = 0; __step < __len; ++__step) { \
            __for_3__captureIters( \
                __step, __for__expandCaptures _$captures, __for__expandIterIds __iter_ids, __for__expandIters _$iters \
            );

#define __for_3__initIters(...) __for_3__initItersNext(__VA_ARGS__)
#define __for_3__initItersNext( \
    __iter_id0, __iter_id1, __iter_id2, \
    _$iter0, _$iter1, _$iter2 \
) \
    __for__initIter(__iter_id0, _$iter0); \
    __for__initIter(__iter_id1, _$iter1); \
    __for__initIter(__iter_id2, _$iter2)
#define __for_3__measureLen(...) __for_3__measureLenNext(__VA_ARGS__)
#define __for_3__measureLenNext( \
    __iter_id0, __iter_id1, __iter_id2, \
    _$iter0, _$iter1, _$iter2 \
) \
    pri_min3( \
        __for__lenIter(_$iter0)(__iter_id0), \
        __for__lenIter(_$iter1)(__iter_id1), \
        __for__lenIter(_$iter2)(__iter_id2) \
    )
#define __for_3__captureIters(...) __for_3__captureItersNext(__VA_ARGS__)
#define __for_3__captureItersNext( \
    __step, \
    _$capture0, _$capture1, _$capture2, \
    __iter_id0, __iter_id1, __iter_id2, \
    _$iter0, _$iter1, _$iter2 \
) \
    let _$capture0 = __for__atIter(__step, __iter_id0, _$iter0); \
    let _$capture1 = __for__atIter(__step, __iter_id1, _$iter1); \
    let _$capture2 = __for__atIter(__step, __iter_id2, _$iter2)

// #define __emit__for_4(__len, __step, __iter_ids, _$iters, _$captures, _block) \
//     0(__len), 1(__step), 2(__iter_ids), 3(_$iters), 4(_$captures), 5(_block)
#define __emit__for_4(_$__hint_loop, __len, __step, __iter_ids, _$iters, _$captures...) \
    { \
        __for_4__initIters(__for__expandIterIds __iter_ids, __for__expandIters _$iters); \
        const usize __len = __for_4__measureLen(__for__expandIterIds __iter_ids, __for__expandIters _$iters); \
        __for__emitHintLoop(_$__hint_loop)() for (usize __step = 0; __step < __len; ++__step) { \
            __for_4__captureIters( \
                __step, __for__expandCaptures _$captures, __for__expandIterIds __iter_ids, __for__expandIters _$iters \
            );

#define __for_4__initIters(...) __for_4__initItersNext(__VA_ARGS__)
#define __for_4__initItersNext( \
    __iter_id0, __iter_id1, __iter_id2, __iter_id3, \
    _$iter0, _$iter1, _$iter2, _$iter3 \
) \
    __for__initIter(__iter_id0, _$iter0); \
    __for__initIter(__iter_id1, _$iter1); \
    __for__initIter(__iter_id2, _$iter2); \
    __for__initIter(__iter_id3, _$iter3)
#define __for_4__measureLen(...) __for_4__measureLenNext(__VA_ARGS__)
#define __for_4__measureLenNext( \
    __iter_id0, __iter_id1, __iter_id2, __iter_id3, \
    _$iter0, _$iter1, _$iter2, _$iter3 \
) \
    pri_min4( \
        __for__lenIter(_$iter0)(__iter_id0), \
        __for__lenIter(_$iter1)(__iter_id1), \
        __for__lenIter(_$iter2)(__iter_id2), \
        __for__lenIter(_$iter3)(__iter_id3) \
    )
#define __for_4__captureIters(...) __for_4__captureItersNext(__VA_ARGS__)
#define __for_4__captureItersNext( \
    __step, \
    _$capture0, _$capture1, _$capture2, _$capture3, \
    __iter_id0, __iter_id1, __iter_id2, __iter_id3, \
    _$iter0, _$iter1, _$iter2, _$iter3 \
) \
    let _$capture0 = __for__atIter(__step, __iter_id0, _$iter0); \
    let _$capture1 = __for__atIter(__step, __iter_id1, _$iter1); \
    let _$capture2 = __for__atIter(__step, __iter_id2, _$iter2); \
    let _$capture3 = __for__atIter(__step, __iter_id3, _$iter3)

#define __emit__for_5(_$__hint_loop, __len, __step, __iter_ids, _$iters, _$captures...) \
    { \
        __for_5__initIters(__for__expandIterIds __iter_ids, __for__expandIters _$iters); \
        const usize __len = __for_5__measureLen(__for__expandIterIds __iter_ids, __for__expandIters _$iters); \
        __for__emitHintLoop(_$__hint_loop)() for (usize __step = 0; __step < __len; ++__step) { \
            __for_5__captureIters( \
                __step, __for__expandCaptures _$captures, __for__expandIterIds __iter_ids, __for__expandIters _$iters \
            );

#define __for_5__initIters(...) __for_5__initItersNext(__VA_ARGS__)
#define __for_5__initItersNext( \
    __iter_id0, __iter_id1, __iter_id2, __iter_id3, \
    __iter_id4, \
    _$iter0, _$iter1, _$iter2, _$iter3, \
    _$iter4 \
) \
    __for__initIter(__iter_id0, _$iter0); \
    __for__initIter(__iter_id1, _$iter1); \
    __for__initIter(__iter_id2, _$iter2); \
    __for__initIter(__iter_id3, _$iter3); \
    __for__initIter(__iter_id4, _$iter4)
#define __for_5__measureLen(...) __for_5__measureLenNext(__VA_ARGS__)
#define __for_5__measureLenNext( \
    __iter_id0, __iter_id1, __iter_id2, __iter_id3, \
    __iter_id4, \
    _$iter0, _$iter1, _$iter2, _$iter3, \
    _$iter4 \
) \
    pri_min5( \
        __for__lenIter(_$iter0)(__iter_id0), \
        __for__lenIter(_$iter1)(__iter_id1), \
        __for__lenIter(_$iter2)(__iter_id2), \
        __for__lenIter(_$iter3)(__iter_id3), \
        __for__lenIter(_$iter4)(__iter_id4) \
    )
#define __for_5__captureIters(...) __for_5__captureItersNext(__VA_ARGS__)
#define __for_5__captureItersNext( \
    __step, \
    _$capture0, _$capture1, _$capture2, _$capture3, \
    _$capture4, \
    __iter_id0, __iter_id1, __iter_id2, __iter_id3, \
    __iter_id4, \
    _$iter0, _$iter1, _$iter2, _$iter3, \
    _$iter4 \
) \
    let _$capture0 = __for__atIter(__step, __iter_id0, _$iter0); \
    let _$capture1 = __for__atIter(__step, __iter_id1, _$iter1); \
    let _$capture2 = __for__atIter(__step, __iter_id2, _$iter2); \
    let _$capture3 = __for__atIter(__step, __iter_id3, _$iter3); \
    let _$capture4 = __for__atIter(__step, __iter_id4, _$iter4)

#define __emit__for_6(_$__hint_loop, __len, __step, __iter_ids, _$iters, _$captures...) \
    { \
        __for_6__initIters(__for__expandIterIds __iter_ids, __for__expandIters _$iters); \
        const usize __len = __for_6__measureLen(__for__expandIterIds __iter_ids, __for__expandIters _$iters); \
        __for__emitHintLoop(_$__hint_loop)() for (usize __step = 0; __step < __len; ++__step) { \
            __for_6__captureIters( \
                __step, __for__expandCaptures _$captures, __for__expandIterIds __iter_ids, __for__expandIters _$iters \
            );

#define __for_6__initIters(...) __for_6__initItersNext(__VA_ARGS__)
#define __for_6__initItersNext( \
    __iter_id0, __iter_id1, __iter_id2, __iter_id3, \
    __iter_id4, __iter_id5, \
    _$iter0, _$iter1, _$iter2, _$iter3, \
    _$iter4, _$iter5 \
) \
    __for__initIter(__iter_id0, _$iter0); \
    __for__initIter(__iter_id1, _$iter1); \
    __for__initIter(__iter_id2, _$iter2); \
    __for__initIter(__iter_id3, _$iter3); \
    __for__initIter(__iter_id4, _$iter4); \
    __for__initIter(__iter_id5, _$iter5)
#define __for_6__measureLen(...) __for_6__measureLenNext(__VA_ARGS__)
#define __for_6__measureLenNext( \
    __iter_id0, __iter_id1, __iter_id2, __iter_id3, \
    __iter_id4, __iter_id5, \
    _$iter0, _$iter1, _$iter2, _$iter3, \
    _$iter4, _$iter5 \
) \
    pri_min2( \
        pri_min4( \
            __for__lenIter(_$iter0)(__iter_id0), \
            __for__lenIter(_$iter1)(__iter_id1), \
            __for__lenIter(_$iter2)(__iter_id2), \
            __for__lenIter(_$iter3)(__iter_id3) \
        ), \
        pri_min2( \
            __for__lenIter(_$iter4)(__iter_id4), \
            __for__lenIter(_$iter5)(__iter_id5) \
        ) \
    )
#define __for_6__captureIters(...) __for_6__captureItersNext(__VA_ARGS__)
#define __for_6__captureItersNext( \
    __step, \
    _$capture0, _$capture1, _$capture2, _$capture3, \
    _$capture4, _$capture5, \
    __iter_id0, __iter_id1, __iter_id2, __iter_id3, \
    __iter_id4, __iter_id5, \
    _$iter0, _$iter1, _$iter2, _$iter3, \
    _$iter4, _$iter5 \
) \
    let _$capture0 = __for__atIter(__step, __iter_id0, _$iter0); \
    let _$capture1 = __for__atIter(__step, __iter_id1, _$iter1); \
    let _$capture2 = __for__atIter(__step, __iter_id2, _$iter2); \
    let _$capture3 = __for__atIter(__step, __iter_id3, _$iter3); \
    let _$capture4 = __for__atIter(__step, __iter_id4, _$iter4); \
    let _$capture5 = __for__atIter(__step, __iter_id5, _$iter5)

#define __emit__for_7(_$__hint_loop, __len, __step, __iter_ids, _$iters, _$captures...) \
    { \
        __for_7__initIters(__for__expandIterIds __iter_ids, __for__expandIters _$iters); \
        const usize __len = __for_7__measureLen(__for__expandIterIds __iter_ids, __for__expandIters _$iters); \
        __for__emitHintLoop(_$__hint_loop)() for (usize __step = 0; __step < __len; ++__step) { \
            __for_7__captureIters( \
                __step, __for__expandCaptures _$captures, __for__expandIterIds __iter_ids, __for__expandIters _$iters \
            );

#define __for_7__initIters(...) __for_7__initItersNext(__VA_ARGS__)
#define __for_7__initItersNext( \
    __iter_id0, __iter_id1, __iter_id2, __iter_id3, \
    __iter_id4, __iter_id5, __iter_id6, \
    _$iter0, _$iter1, _$iter2, _$iter3, \
    _$iter4, _$iter5, _$iter6 \
) \
    __for__initIter(__iter_id0, _$iter0); \
    __for__initIter(__iter_id1, _$iter1); \
    __for__initIter(__iter_id2, _$iter2); \
    __for__initIter(__iter_id3, _$iter3); \
    __for__initIter(__iter_id4, _$iter4); \
    __for__initIter(__iter_id5, _$iter5); \
    __for__initIter(__iter_id6, _$iter6)
#define __for_7__measureLen(...) __for_7__measureLenNext(__VA_ARGS__)
#define __for_7__measureLenNext( \
    __iter_id0, __iter_id1, __iter_id2, __iter_id3, \
    __iter_id4, __iter_id5, __iter_id6, \
    _$iter0, _$iter1, _$iter2, _$iter3, \
    _$iter4, _$iter5, _$iter6 \
) \
    pri_min2( \
        pri_min4( \
            __for__lenIter(_$iter0)(__iter_id0), \
            __for__lenIter(_$iter1)(__iter_id1), \
            __for__lenIter(_$iter2)(__iter_id2), \
            __for__lenIter(_$iter3)(__iter_id3) \
        ), \
        pri_min3( \
            __for__lenIter(_$iter4)(__iter_id4), \
            __for__lenIter(_$iter5)(__iter_id5), \
            __for__lenIter(_$iter6)(__iter_id6) \
        ) \
    )
#define __for_7__captureIters(...) __for_7__captureItersNext(__VA_ARGS__)
#define __for_7__captureItersNext( \
    __step, \
    _$capture0, _$capture1, _$capture2, _$capture3, \
    _$capture4, _$capture5, _$capture6, \
    __iter_id0, __iter_id1, __iter_id2, __iter_id3, \
    __iter_id4, __iter_id5, __iter_id6, \
    _$iter0, _$iter1, _$iter2, _$iter3, \
    _$iter4, _$iter5, _$iter6 \
) \
    let _$capture0 = __for__atIter(__step, __iter_id0, _$iter0); \
    let _$capture1 = __for__atIter(__step, __iter_id1, _$iter1); \
    let _$capture2 = __for__atIter(__step, __iter_id2, _$iter2); \
    let _$capture3 = __for__atIter(__step, __iter_id3, _$iter3); \
    let _$capture4 = __for__atIter(__step, __iter_id4, _$iter4); \
    let _$capture5 = __for__atIter(__step, __iter_id5, _$iter5); \
    let _$capture6 = __for__atIter(__step, __iter_id6, _$iter6)

#define __emit__for_8(_$__hint_loop, __len, __step, __iter_ids, _$iters, _$captures...) \
    { \
        __for_8__initIters(__for__expandIterIds __iter_ids, __for__expandIters _$iters); \
        const usize __len = __for_8__measureLen(__for__expandIterIds __iter_ids, __for__expandIters _$iters); \
        __for__emitHintLoop(_$__hint_loop)() for (usize __step = 0; __step < __len; ++__step) { \
            __for_8__captureIters( \
                __step, __for__expandCaptures _$captures, __for__expandIterIds __iter_ids, __for__expandIters _$iters \
            );

#define __for_8__initIters(...) __for_8__initItersNext(__VA_ARGS__)
#define __for_8__initItersNext( \
    __iter_id0, __iter_id1, __iter_id2, __iter_id3, \
    __iter_id4, __iter_id5, __iter_id6, __iter_id7, \
    _$iter0, _$iter1, _$iter2, _$iter3, \
    _$iter4, _$iter5, _$iter6, _$iter7 \
) \
    __for__initIter(__iter_id0, _$iter0); \
    __for__initIter(__iter_id1, _$iter1); \
    __for__initIter(__iter_id2, _$iter2); \
    __for__initIter(__iter_id3, _$iter3); \
    __for__initIter(__iter_id4, _$iter4); \
    __for__initIter(__iter_id5, _$iter5); \
    __for__initIter(__iter_id6, _$iter6); \
    __for__initIter(__iter_id7, _$iter7)
#define __for_8__measureLen(...) __for_8__measureLenNext(__VA_ARGS__)
#define __for_8__measureLenNext( \
    __iter_id0, __iter_id1, __iter_id2, __iter_id3, \
    __iter_id4, __iter_id5, __iter_id6, __iter_id7, \
    _$iter0, _$iter1, _$iter2, _$iter3, \
    _$iter4, _$iter5, _$iter6, _$iter7 \
) \
    pri_min2( \
        pri_min4( \
            __for__lenIter(_$iter0)(__iter_id0), \
            __for__lenIter(_$iter1)(__iter_id1), \
            __for__lenIter(_$iter2)(__iter_id2), \
            __for__lenIter(_$iter3)(__iter_id3) \
        ), \
        pri_min4( \
            __for__lenIter(_$iter4)(__iter_id4), \
            __for__lenIter(_$iter5)(__iter_id5), \
            __for__lenIter(_$iter6)(__iter_id6), \
            __for__lenIter(_$iter7)(__iter_id7) \
        ) \
    )
#define __for_8__captureIters(...) __for_8__captureItersNext(__VA_ARGS__)
#define __for_8__captureItersNext( \
    __step, \
    _$capture0, _$capture1, _$capture2, _$capture3, \
    _$capture4, _$capture5, _$capture6, _$capture7, \
    __iter_id0, __iter_id1, __iter_id2, __iter_id3, \
    __iter_id4, __iter_id5, __iter_id6, __iter_id7, \
    _$iter0, _$iter1, _$iter2, _$iter3, \
    _$iter4, _$iter5, _$iter6, _$iter7 \
) \
    let _$capture0 = __for__atIter(__step, __iter_id0, _$iter0); \
    let _$capture1 = __for__atIter(__step, __iter_id1, _$iter1); \
    let _$capture2 = __for__atIter(__step, __iter_id2, _$iter2); \
    let _$capture3 = __for__atIter(__step, __iter_id3, _$iter3); \
    let _$capture4 = __for__atIter(__step, __iter_id4, _$iter4); \
    let _$capture5 = __for__atIter(__step, __iter_id5, _$iter5); \
    let _$capture6 = __for__atIter(__step, __iter_id6, _$iter6); \
    let _$capture7 = __for__atIter(__step, __iter_id7, _$iter7)

/* for - emitters (reverse) =================================================*/
#define __emitNext__for_$_rev(_$__hint_loop, _$iters, _$captures...) \
    pp_overload(__emitNext__for_$_rev, __for__expandIters _$iters)(_$__hint_loop, _$iters, _$captures)
#define __emitNext__for_$_rev_1(_$__hint_loop, _$iters, _$captures...) \
    __emit__for_$_rev_1( \
        _$__hint_loop, pp_uniqTok(len), pp_uniqTok(step), \
        (pp_uniqTok(iter_id0)), \
        _$iters, _$captures \
    )
#define __emitNext__for_$_rev_2(_$__hint_loop, _$iters, _$captures...) \
    __emit__for_$_rev_2( \
        _$__hint_loop, pp_uniqTok(len), pp_uniqTok(step), \
        (pp_uniqTok(iter_id0), pp_uniqTok(iter_id1)), \
        _$iters, _$captures \
    )
#define __emitNext__for_$_rev_3(_$__hint_loop, _$iters, _$captures...) \
    __emit__for_$_rev_3( \
        _$__hint_loop, pp_uniqTok(len), pp_uniqTok(step), \
        (pp_uniqTok(iter_id0), pp_uniqTok(iter_id1), pp_uniqTok(iter_id2)), \
        _$iters, _$captures \
    )
#define __emitNext__for_$_rev_4(_$__hint_loop, _$iters, _$captures...) \
    __emit__for_$_rev_4( \
        _$__hint_loop, pp_uniqTok(len), pp_uniqTok(step), \
        (pp_uniqTok(iter_id0), pp_uniqTok(iter_id1), pp_uniqTok(iter_id2), pp_uniqTok(iter_id3)), \
        _$iters, _$captures \
    )
#define __emitNext__for_$_rev_5(_$__hint_loop, _$iters, _$captures...) \
    __emit__for_$_rev_5( \
        _$__hint_loop, pp_uniqTok(len), pp_uniqTok(step), \
        ( \
            pp_uniqTok(iter_id0), pp_uniqTok(iter_id1), pp_uniqTok(iter_id2), pp_uniqTok(iter_id3), \
            pp_uniqTok(iter_id4) \
        ), \
        _$iters, _$captures \
    )
#define __emitNext__for_$_rev_6(_$__hint_loop, _$iters, _$captures...) \
    __emit__for_$_rev_6( \
        _$__hint_loop, pp_uniqTok(len), pp_uniqTok(step), \
        ( \
            pp_uniqTok(iter_id0), pp_uniqTok(iter_id1), pp_uniqTok(iter_id2), pp_uniqTok(iter_id3), \
            pp_uniqTok(iter_id4), pp_uniqTok(iter_id5) \
        ), \
        _$iters, _$captures \
    )
#define __emitNext__for_$_rev_7(_$__hint_loop, _$iters, _$captures...) \
    __emit__for_$_rev_7( \
        _$__hint_loop, pp_uniqTok(len), pp_uniqTok(step), \
        ( \
            pp_uniqTok(iter_id0), pp_uniqTok(iter_id1), pp_uniqTok(iter_id2), pp_uniqTok(iter_id3), \
            pp_uniqTok(iter_id4), pp_uniqTok(iter_id5), pp_uniqTok(iter_id6) \
        ), \
        _$iters, _$captures \
    )
#define __emitNext__for_$_rev_8(_$__hint_loop, _$iters, _$captures...) \
    __emit__for_$_rev_8( \
        _$__hint_loop, pp_uniqTok(len), pp_uniqTok(step), \
        ( \
            pp_uniqTok(iter_id0), pp_uniqTok(iter_id1), pp_uniqTok(iter_id2), pp_uniqTok(iter_id3), \
            pp_uniqTok(iter_id4), pp_uniqTok(iter_id5), pp_uniqTok(iter_id6), pp_uniqTok(iter_id7) \
        ), \
        _$iters, _$captures \
    )

// #define __emit__for_$_rev_1(__len, __step, __iter_ids, _$iters, _$captures, _block) \
//     0(__len), 1(__step), 2(__iter_ids), 3(_$iters), 4(_$captures), 5(_block)
#define __emit__for_$_rev_1(_$__hint_loop, __len, __step, __iter_ids, _$iters, _$captures...) \
    { \
        __for_1__initIters(__for__expandIterIds __iter_ids, __for__expandIters _$iters); \
        const usize __len = __for_1__measureLen(__for__expandIterIds __iter_ids, __for__expandIters _$iters); \
        __for__emitHintLoop(_$__hint_loop)() for (usize __step = __len; __step-- > 0;) { \
            __for_1__captureIters( \
                __step, __for__expandCaptures _$captures, __for__expandIterIds __iter_ids, __for__expandIters _$iters \
            );

// #define __emit__for_$_rev_2(__len, __step, __iter_ids, _$iters, _$captures, _block) \
//     0(__len), 1(__step), 2(__iter_ids), 3(_$iters), 4(_$captures), 5(_block)
#define __emit__for_$_rev_2(_$__hint_loop, __len, __step, __iter_ids, _$iters, _$captures...) \
    { \
        __for_2__initIters(__for__expandIterIds __iter_ids, __for__expandIters _$iters); \
        const usize __len = __for_2__measureLen(__for__expandIterIds __iter_ids, __for__expandIters _$iters); \
        __for__emitHintLoop(_$__hint_loop)() for (usize __step = __len; __step-- > 0;) { \
            __for_2__captureIters( \
                __step, __for__expandCaptures _$captures, __for__expandIterIds __iter_ids, __for__expandIters _$iters \
            );

// #define __emit__for_$_rev_3(__len, __step, __iter_ids, _$iters, _$captures, _block) \
//     0(__len), 1(__step), 2(__iter_ids), 3(_$iters), 4(_$captures), 5(_block)
#define __emit__for_$_rev_3(_$__hint_loop, __len, __step, __iter_ids, _$iters, _$captures...) \
    { \
        __for_3__initIters(__for__expandIterIds __iter_ids, __for__expandIters _$iters); \
        const usize __len = __for_3__measureLen(__for__expandIterIds __iter_ids, __for__expandIters _$iters); \
        __for__emitHintLoop(_$__hint_loop)() for (usize __step = __len; __step-- > 0;) { \
            __for_3__captureIters( \
                __step, __for__expandCaptures _$captures, __for__expandIterIds __iter_ids, __for__expandIters _$iters \
            );

// #define __emit__for_$_rev_4(__len, __step, __iter_ids, _$iters, _$captures, _block) \
//     0(__len), 1(__step), 2(__iter_ids), 3(_$iters), 4(_$captures), 5(_block)
#define __emit__for_$_rev_4(_$__hint_loop, __len, __step, __iter_ids, _$iters, _$captures...) \
    { \
        __for_4__initIters(__for__expandIterIds __iter_ids, __for__expandIters _$iters); \
        const usize __len = __for_4__measureLen(__for__expandIterIds __iter_ids, __for__expandIters _$iters); \
        __for__emitHintLoop(_$__hint_loop)() for (usize __step = __len; __step-- > 0;) { \
            __for_4__captureIters( \
                __step, __for__expandCaptures _$captures, __for__expandIterIds __iter_ids, __for__expandIters _$iters \
            );

#define __emit__for_$_rev_5(_$__hint_loop, __len, __step, __iter_ids, _$iters, _$captures...) \
    { \
        __for_5__initIters(__for__expandIterIds __iter_ids, __for__expandIters _$iters); \
        const usize __len = __for_5__measureLen(__for__expandIterIds __iter_ids, __for__expandIters _$iters); \
        __for__emitHintLoop(_$__hint_loop)() for (usize __step = __len; __step-- > 0;) { \
            __for_5__captureIters( \
                __step, __for__expandCaptures _$captures, __for__expandIterIds __iter_ids, __for__expandIters _$iters \
            );

#define __emit__for_$_rev_6(_$__hint_loop, __len, __step, __iter_ids, _$iters, _$captures...) \
    { \
        __for_6__initIters(__for__expandIterIds __iter_ids, __for__expandIters _$iters); \
        const usize __len = __for_6__measureLen(__for__expandIterIds __iter_ids, __for__expandIters _$iters); \
        __for__emitHintLoop(_$__hint_loop)() for (usize __step = __len; __step-- > 0;) { \
            __for_6__captureIters( \
                __step, __for__expandCaptures _$captures, __for__expandIterIds __iter_ids, __for__expandIters _$iters \
            );

#define __emit__for_$_rev_7(_$__hint_loop, __len, __step, __iter_ids, _$iters, _$captures...) \
    { \
        __for_7__initIters(__for__expandIterIds __iter_ids, __for__expandIters _$iters); \
        const usize __len = __for_7__measureLen(__for__expandIterIds __iter_ids, __for__expandIters _$iters); \
        __for__emitHintLoop(_$__hint_loop)() for (usize __step = __len; __step-- > 0;) { \
            __for_7__captureIters( \
                __step, __for__expandCaptures _$captures, __for__expandIterIds __iter_ids, __for__expandIters _$iters \
            );

#define __emit__for_$_rev_8(_$__hint_loop, __len, __step, __iter_ids, _$iters, _$captures...) \
    { \
        __for_8__initIters(__for__expandIterIds __iter_ids, __for__expandIters _$iters); \
        const usize __len = __for_8__measureLen(__for__expandIterIds __iter_ids, __for__expandIters _$iters); \
        __for__emitHintLoop(_$__hint_loop)() for (usize __step = __len; __step-- > 0;) { \
            __for_8__captureIters( \
                __step, __for__expandCaptures _$captures, __for__expandIterIds __iter_ids, __for__expandIters _$iters \
            );

#define __stmt__$end_for \
    } \
    }

/* loop unroll, inline, rolled ==============================================*/

#define __loop_unroll_do() $loop_unroll do
#define __loop_inline_do() $loop_inline do
#define __loop_rolled_do() $loop_rolled do
#define __loop_unroll_while() $loop_unroll while
#define __loop_inline_while() $loop_inline while
#define __loop_rolled_while() $loop_rolled while
#define __loop_unroll_for() $loop_unroll for
#define __loop_inline_for() $loop_inline for
#define __loop_rolled_for() $loop_rolled for

#define __loop_unroll_for_() ____loop_unroll_for_
#define ____loop_unroll_for_(...) pp_overload(__for, __VA_ARGS__)($__hint_loop_unroll, __VA_ARGS__)
#define __loop_inline_for_() ____loop_inline_for_
#define ____loop_inline_for_(...) pp_overload(__for, __VA_ARGS__)($__hint_loop_inline, __VA_ARGS__)
#define __loop_rolled_for_() ____loop_rolled_for_
#define ____loop_rolled_for_(...) pp_overload(__for, __VA_ARGS__)($__hint_loop_rolled, __VA_ARGS__)

#define __loop_unroll_while_() $loop_unroll while_
#define __loop_inline_while_() $loop_inline while_
#define __loop_rolled_while_() $loop_rolled while_

#if defined(__cplusplus)
/* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* core_scope_common__included */

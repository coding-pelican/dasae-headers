/**
 * @copyright Copyright (c) 2025 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    fn.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2025-03-04 (date of creation)
 * @updated 2025-04-02 (date of last update)
 * @version v0.1-alpha
 * @ingroup dasae-headers(dh)/core
 * @prefix  (none)
 *
 * @brief   Function utilities
 * @details Provides utilities for function operations, including function pointers,
 *          function declarations, and function implementations.
 */
#pragma once
#ifndef core_fn__included
#define core_fn__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "pri.h"

/*========== Macros and Declarations ========================================*/

#define $scope , $_scope
#define $guard , $_guard
#define $T , $_T
#define $void

#define fn_(/*(_$ident(_$Params...))(_$T_Return) <$ext> | (_$Params...)(_$T_Return) $T*/...) \
    __step__fn_(__param_expand__fn_ __VA_ARGS__)
#define __step__fn_(...) __step__fn___emit(__VA_ARGS__)
#define __step__fn___emit(...) __block_inline__fn_(__VA_ARGS__)
#define __param_expand__fn_(...) (__VA_ARGS__), __param_expand__fn___next
#define __param_expand__fn___next(...) __VA_ARGS__
#define __block_inline__fn_(...) pp_overload(__block_inline__fn, __VA_ARGS__)(__VA_ARGS__)
#define __block_inline__fn_2(_$ident_w_Params, _$T_Return...) comp_syn__fn_(__param_extract__fn_ _$ident_w_Params, _$T_Return)
#define __block_inline__fn_3(_$ident_w_Params_OR_Params, _$T_Return, _$ext...) pp_join(_, __block_inline__fn_3, _$ext)(_$ident_w_Params_OR_Params, _$T_Return)
#define __block_inline__fn_3_$_scope(_$ident_w_Params, _$T_Return...) comp_syn__fn_$_scope(__param_extract__fn_ _$ident_w_Params, _$T_Return)
#define __block_inline__fn_3_$_guard(_$ident_w_Params, _$T_Return...) comp_syn__fn_$_guard(__param_extract__fn_ _$ident_w_Params, _$T_Return)
#define __block_inline__fn_3_$_T(_$Params, _$T_Return...) comp_syn__fn_$_T(_$Params, _$T_Return)
#define __param_extract__fn_(...) __VA_ARGS__

#define fn_$_scope(_$ident_w_Params, _$T_Return...) comp_syn__fn_$_scope(_$ident_w_Params, _$T_Return)
#define $unscoped_fn comp_syn__$unscoped_fn

#define fn_$_guard(_$ident_w_Params, _$T_Return...) comp_syn__fn_$_guard(_$ident_w_Params, _$T_Return)
#define $unguarded_fn comp_syn__$unguarded_fn

#define return_(_$Expr...) comp_syn__return_(_$Expr)
#define return_void(_$Expr...) pp_overload(comp_syn__return_void, _$Expr)(_$Expr)

#define reservedReturn() $suppress_pointer_arith((__reserved_return[0]))
// #define ReturnType       TypeOf(reservedReturn())
#define reservedBreak() $suppress_pointer_arith((__reserved_break[0]))
// #define BreakType        TypeOf(reservedBreak())

/*
#define return_ok(_$Expr...)   comp_syn__return_ok(_$Expr)
#define return_err(_$Expr...)  comp_syn__return_err(_$Expr)
#define return_some(_$Expr...) comp_syn__return_some(_$Expr)
#define return_none()         comp_syn__return_none()
*/

#define defer_(_$Expr...) comp_syn__defer(_$Expr)

#define blk_defer comp_syn__blk_defer
#define blk_deferral comp_syn__blk_deferral
#define loop_defer comp_syn__loop_defer
#define loop_deferral comp_syn__loop_deferral

/*
#define errdefer_(_$Expr...) comp_syn__errdefer_(_$Expr)
*/

/*========== Macros and Definitions =========================================*/

#define comp_syn__fn_$_T(_$Params, _$T_Return...) TypeOf(_$T_Return _$Params)

#define comp_syn__fn_(_$ident_w_Params, _$T_Return...) _$T_Return _$ident_w_Params

#define comp_syn__fn_$_scope(_$ident_w_Params, _$T_Return...) /* clang-format off */ \
_$T_Return _$ident_w_Params { \
    $alignAs(alignOf$(_$T_Return)) var_(__reserved_buf, A$$(sizeOf$(_$T_Return), u8)) = A_zero(); \
    let __reserved_return = ptrCast$((_$T_Return*)(A_ptr(__reserved_buf))); \
    $maybe_unused typedef TypeOf(*__reserved_return) ReturnType; \
    $maybe_unused typedef ReturnType ReturnT; \
    if (false) { __step_return: goto __step_unscope; } \
    do
#define comp_syn__$unscoped_fn \
    while (false); \
    T_switch$((ReturnT)( \
        T_case$((void)({ goto __step_return; })), \
        T_case$((Void)({ goto __step_return; })), \
        T_default_(({})) \
    )); \
    if (false) { __step_unscope: \
        $pragma_guard_( \
            "clang diagnostic push", "clang diagnostic ignored \"-Wgnu-pointer-arith\"", "clang diagnostic pop", \
            if (T_switch$((ReturnT)( \
                T_case$((void)(false)), \
                T_default_(true) \
            ))) { return reservedReturn(); } \
        ); \
    } \
} /* clang-format on */

#define fn__FlowCursor_is_returning_bits 1
#define fn__FlowCursor_line_bits ((arch_bits_wide / 2) - fn__FlowCursor_is_returning_bits)
typedef u32 fn__FlowCursorPacked;
struct fn__FlowCursor {
    union {
        struct {
            fn__FlowCursorPacked curr_line    : fn__FlowCursor_line_bits;
            fn__FlowCursorPacked is_returning : fn__FlowCursor_is_returning_bits;
        };
        fn__FlowCursorPacked packed;
    };
};
claim_assert_static((fn__FlowCursor_line_bits + fn__FlowCursor_is_returning_bits) == (arch_bits_wide / 2));
#define comp_syn__fn_$_guard(_$ident_w_Params, _$T_Return...) /* clang-format off */ \
_$T_Return _$ident_w_Params { \
    $alignAs(alignOf$(_$T_Return)) volatile var_(__reserved_buf, A$$(sizeOf$(_$T_Return), u8)) = A_zero(); \
    let __reserved_return = ptrQualCast$((_$T_Return*)(A_ptr(__reserved_buf))); \
    $maybe_unused typedef TypeOf(*__reserved_return) ReturnType; \
    $maybe_unused typedef ReturnType ReturnT; \
    var_(__flow_cursor, struct fn__FlowCursor) = { \
        .is_returning = false, .curr_line = __LINE__ \
    }; \
    if (false) { __step_return: \
        __flow_cursor.is_returning = true; \
        goto __step_deferred; \
    } \
__step_deferred: switch (__flow_cursor.curr_line) { \
    default: { goto __step_unscope; } break; \
    case __LINE__: __flow_cursor.curr_line = __LINE__ - 1;
#define comp_syn__$unguarded_fn \
        break; \
    } \
    T_switch$((ReturnT)( \
        T_case$((void)({ goto __step_return; })), \
        T_case$((Void)({ goto __step_return; })), \
        T_default_(({})) \
    )); \
    if (false) { __step_unscope: \
        $pragma_guard_( \
            "clang diagnostic push", "clang diagnostic ignored \"-Wgnu-pointer-arith\"", "clang diagnostic pop", \
            if (T_switch$((ReturnT)( \
                T_case$((void)(false)), \
                T_default_(true) \
            ))) { return reservedReturn(); } \
        ); \
    } \
} /* clang-format on */

#if !in_comptime
#define fn__memset(_$dst, _$val, _$len...) __fn_memset__no_hinting(_$dst, _$val, _$len)
extern fn_((__fn_memset__no_hinting(void*, u32, usize))(void*));
#define fn__memcpy(_$dst, _$src, _$len...) __fn_memcpy__no_hinting(_$dst, _$src, _$len)
extern fn_((__fn_memcpy__no_hinting(void*, const void*, usize))(void*));
#define fn__memmove(_$dst, _$src, _$len...) __fn_memmove__no_hinting(_$dst, _$src, _$len)
extern fn_((__fn_memmove__no_hinting(void*, const void*, usize))(void*));
#else
#define fn__memset(_$dst, _$val, _$len...) raw_memset(_$dst, _$val, _$len)
#define fn__memcpy(_$dst, _$src, _$len...) raw_memcpy(_$dst, _$src, _$len)
#define fn__memmove(_$dst, _$src, _$len...) raw_memmove(_$dst, _$src, _$len)
#endif /* !in_comptime */

/* #define comp_syn__return_(_$Expr...) local_({ \
    $ignore_void fn__memcpy( \
        as$(u8*)(__reserved_return), \
        as$(u8*)((TypeOf (*__reserved_return)[1]){ [0] = _$Expr }), \
        sizeOf$(*__reserved_return) \
    ); \
    goto __step_return; \
}) */
#define comp_syn__return_(_$Expr...) ({ \
    $ignore_void fn__memcpy( \
        ptrCast$((u8*)(__reserved_return)), \
        ptrCast$((u8*)((ReturnT[1]){ [0] = _$Expr })), \
        sizeOf$(ReturnT) \
    ); \
    goto __step_return; \
})
#define comp_syn__return_void_0() ({ \
    claim_assert_static( \
        eqlType$(ReturnT, void) \
        || eqlType$(ReturnT, Void) \
    ); \
    goto __step_return; \
})
#define comp_syn__return_void_1(_$Expr...) ({ \
    claim_assert_static( \
        eqlType$(ReturnT, void) \
        || eqlType$(ReturnT, Void) \
    ); \
    claim_assert_static( \
        eqlType$(TypeOf(({ _$Expr; })), void) \
        || eqlType$(TypeOf(({ _$Expr; })), Void) \
    ); \
    $ignore_void _$Expr; \
    goto __step_return; \
})

#define comp_syn__defer(_$Expr...) comp_syn__defer__op_snapshot(_$Expr; goto __step_deferred)

/* Block-local defer boundary. A raw break/continue inside the body exits this
 * synthetic block, so it does not target an outer loop. */
#define comp_syn__blk_defer /* clang-format off */ { \
    do { \
        comp_syn__defer__op_snapshot( \
            if (__flow_cursor.is_returning) { \
                goto __step_deferred; \
            } else { \
                continue; \
            } \
        ); \
        do
#define comp_syn__blk_deferral \
        while (false); \
        goto __step_deferred; \
    } while (false); \
} /* clang-format on */

/* Loop-iteration defer boundary. Direct raw continue/break in the body run the
 * iteration defers before continuing or breaking the enclosing loop. If another
 * synthetic boundary such as blk_defer is nested inside, its break/continue
 * remains local to that nested boundary. */
#define comp_syn__loop_defer /* clang-format off */ { \
    bool __loop_defer_is_breaking = true; \
    do { \
        comp_syn__defer__op_snapshot( \
            if (__flow_cursor.is_returning) { \
                goto __step_deferred; \
            } else { \
                continue; \
            } \
        ); \
        do
#define comp_syn__loop_deferral \
        while ((__loop_defer_is_breaking = false), false); \
        goto __step_deferred; \
    } while (false); \
    if (__loop_defer_is_breaking) { break; } \
} /* clang-format on */

#define comp_syn__defer__op_snapshot(_$Expr...) \
    { \
        const fn__FlowCursorPacked __flow_cursor_prev_line = __flow_cursor.curr_line; \
        __flow_cursor.curr_line = __LINE__; \
        if (false) { \
        case __LINE__: \
            __flow_cursor.curr_line = __flow_cursor_prev_line; \
            _$Expr; \
        } \
    }

#define call(/*(_$ident)(_$Args...)*/...) pp_expand(pp_exec_defer(__exec_call)()(pp_Tuple_unwrapSufComma __VA_ARGS__))
#define __exec_call() __call
#define __call(_$ident, _$Args...) (ensureNonnull(_$ident) _$Args)

// clang-format off
/* if-else as expression block */
#define expr_(/*<_$T_Break $ext>|<else>*/...) __expr_(__VA_ARGS__) __expr_block
#define __expr_block(...) __VA_ARGS__
#define __expr_(...) pp_overload(__expr_, __VA_ARGS__)(__VA_ARGS__)
#define __expr__1(_$else...) _$else
#define __expr__2(_$T_Break, _$ext...) pp_cat(comp_syn__expr_, _$ext)(_$T_Break)
#define comp_syn__expr_$_scope(_$T_Break...) ({ \
    local_label __step_break, __step_unscope; \
    $alignAs(alignOf$(_$T_Break)) var_(__reserved_buf, A$$(sizeOf$(_$T_Break), u8)) = A_zero(); \
    let __reserved_break = ptrCast$((_$T_Break*)(A_ptr(__reserved_buf))); \
    $maybe_unused typedef TypeOfUnqual(*__reserved_break) BreakType; \
    $maybe_unused typedef BreakType BreakT; \
    $maybe_unused bool __has_broken = false; /* for integration with `eval_` */ \
    if (false) { __step_break: goto __step_unscope; } \
    /* do */
#define $unscoped_expr comp_syn__expr_$unscoped
#define comp_syn__expr_$unscoped \
    /* while (false) */; \
__step_unscope: \
    T_switch$((BreakT)( \
        T_case$((void)({})), \
        T_default_(reservedBreak()) \
    )); \
})
#define comp_syn__expr_$_guard(_$T_Break...) ({ \
    local_label __step_return_inner, __step_break, __step_deferred, __step_unscope; \
    if (false) { __step_return_inner: goto __step_return; } \
    $alignAs(alignOf$(_$T_Break)) volatile var_(__reserved_buf, A$$(sizeOf$(_$T_Break), u8)) = A_zero(); \
    let __reserved_break = ptrQualCast$((_$T_Break*)(A_ptr(__reserved_buf))); \
    $maybe_unused typedef TypeOfUnqual(*__reserved_break) BreakType; \
    $maybe_unused typedef BreakType BreakT; \
    var __flow_cursor = (struct fn__FlowCursor){ \
        .is_returning = false, .curr_line = __LINE__ \
    }; \
    bool __has_broken = false; { \
        local_label __step_return; \
        if (false) { goto __step_return; __step_return: goto __step_break; __step_break: \
            __flow_cursor.is_returning = true; \
            goto __step_deferred; \
        } \
__step_deferred: switch (__flow_cursor.curr_line) { \
        default: { goto __step_unscope; } break; \
        case __LINE__: __flow_cursor.curr_line = __LINE__ - 1; \
            /* do */
#define $unguarded_expr comp_syn__expr_$unguarded
#define comp_syn__expr_$unguarded \
            /* while (false) */; \
            break; \
        } \
    } \
    __step_unscope: \
    if (!__has_broken) { goto __step_return_inner; } \
    T_switch$((BreakT)( \
        T_case$((void)({})), \
        T_default_(reservedBreak()) \
    )); \
})
// clang-format on

// clang-format off
#define $break_(_$Expr...) ({ \
    $ignore_void fn__memcpy( \
        ptrCast$((u8*)(__reserved_break)), \
        ptrCast$((u8*)((BreakT[1]){ [0] = _$Expr })), \
        sizeOf$(BreakT) \
    ); \
    __has_broken = true; \
    goto __step_break; \
})

#define $break_void(_$Expr...) pp_overload(comp_syn__$break_void, _$Expr)(_$Expr)
#define comp_syn__$break_void_0() local_({ \
    claim_assert_static( \
        eqlType$(BreakT, void) \
        || eqlType$(BreakT, Void) \
    ); \
    goto __step_break; \
})
#define comp_syn__$break_void_1(_$Expr...) local_({ \
    claim_assert_static( \
        eqlType$(BreakT, void) \
        || eqlType$(BreakT, Void) \
    ); \
    claim_assert_static( \
        eqlType$(TypeOf(({ _$Expr; })), void) \
        || eqlType$(TypeOf(({ _$Expr; })), Void) \
    ); \
    $ignore_void _$Expr; \
    goto __step_break; \
})

#define $continue_(...) pp_overload(comp_syn__$continue, __VA_ARGS__)(__VA_ARGS__)
#define comp_syn__$continue_0() continue
#define comp_syn__$continue_1(_$Expr...) local_({ \
    claim_assert_static( \
        eqlType$(TypeOf(({ _$Expr; })), void) \
        || eqlType$(TypeOf(({ _$Expr; })), Void) \
    ); \
    $ignore_void _$Expr; \
    local_return_(continue); \
})
// clang-format on

/* TODO: make it compile error if it does not end with else */
// clang-format off
/* [for|while|switch|match]-else as expression block (supports else) */
#define eval_(/*<_$T_Break $ext>|<else>*/...) inline__eval_(__VA_ARGS__) pp_expand
#define inline__eval_(...) pp_overload(inline__eval, __VA_ARGS__)(__VA_ARGS__)
#define inline__eval_1(_$else...) ; if (__has_broken) { goto __step_break; } _$else
#define inline__eval_2(_$T_Break, _$ext...) pp_cat(inline__eval_2, _$ext)(_$T_Break)
#define inline__eval_2$_scope(_$T_Break...) ({ \
    local_label __step_break; \
    $alignAs(alignOf$(_$T_Break)) var_(__reserved_buf, A$$(sizeOf$(_$T_Break), u8)) = A_zero(); \
    let __reserved_break = ptrQualCast$((_$T_Break*)(A_ptr(__reserved_buf))); \
    $maybe_unused typedef TypeOfUnqual(*__reserved_break) BreakType; \
    $maybe_unused typedef BreakType BreakT; \
    $maybe_unused bool __has_broken = false;\
    /* do */
#define $unscoped_eval comp_syn__eval_$unscoped
#define comp_syn__eval_$unscoped \
    /* while (false) */; \
    __step_break: \
    T_switch$((BreakT)( \
        T_case$((void)({})), \
        T_default_(reservedBreak()) \
    )); \
})
// clang-format on

#define $un(_$keyword) pp_cat(inline__$un, _$keyword)()
#define inline__$unscoped() $unscoped_fn
#define inline__$unguarded() $unguarded_fn

#define $unscoped(_$keyword) pp_cat(inline__$unscoped_, _$keyword)()
#define inline__$unscoped_fn() $unscoped_fn
#define inline__$unscoped_TEST_fn() $unscoped_TEST_fn
#define inline__$unscoped_thrd_fn() $unscoped_thrd_fn
#define inline__$unscoped_async_fn() $unscoped_async_fn
#define inline__$unscoped_la() $unscoped_la
#define inline__$unscoped_expr() $unscoped_expr
#define inline__$unscoped_eval() $unscoped_eval

#define $unguarded(_$keyword) pp_cat(inline__$unguarded_, _$keyword)()
#define inline__$unguarded_fn() $unguarded_fn
#define inline__$unguarded_TEST_fn() $unguarded_TEST_fn
#define inline__$unguarded_thrd_fn() $unguarded_thrd_fn
#define inline__$unguarded_async_fn() $unguarded_async_fn
#define inline__$unguarded_la() $unguarded_la
#define inline__$unguarded_expr() $unguarded_expr
#define inline__$unguarded_eval() $unguarded_eval

/* TODO: move to `using` */
#define $defer , $_defer
#define $defer_(/*_$stmt|_$expr*/...) , $_defer_(__VA_ARGS__)
#define $errdefer_(/*(_$capt)(_$stmt|_$expr)*/...) , $_errdefer_(__VA_ARGS__)

#define $end(_$keyword) \
    ; \
    pp_cat($end_, _$keyword)
#define $deferral(_$keyword) \
    ; \
    pp_cat($deferral_, _$keyword)

/*========== Example usage ==================================================*/

#if EXAMPLE_USAGE
#include "dh-main.h"
#include "dh/m-math.h"

/* declarations =============================================================*/
use_ErrSet$(math_E, i32);
$static $must_check
    fn_((math_divideSafe(i32 lhs, i32 rhs))(math_E$i32));

/* main */
fn_((main(S$S_const$u8 args))(E$void) $scope) {
    debug_assert_true(0 < args.len);
    debug_assert_true(try_(math_divideSafe(10, 2)) == 5);
    catch_((math_divideSafe(10, 0))(err, {
        let err_code = Err_codeToStr(err);
        debug_assert_true(mem_eqlBytes(err_code, u8_l("DivisionByZero")));
        return_err(err);
    }));
    return_ok({});
} $unscoped(fn);

/* definitions */
fn_((math_divideSafe(i32 lhs, i32 rhs))(math_E$i32) $scope) {
    if (rhs == 0) {
        return_err(math_E_DivisionByZero());
    }
    return_ok(lhs / rhs);
} $unscoped(fn);
#endif /* EXAMPLE_USAGE */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* core_fn__included */

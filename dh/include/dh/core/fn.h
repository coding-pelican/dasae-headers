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
#ifndef core_fn__included
#define core_fn__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "prim.h"

/*========== Macros and Declarations ========================================*/

#define $scope , $_scope
#define $guard , $_guard
#define $T , $_T
#define $void

#define fn_(/*(_ident(_Params...))(_T_Return) <$ext> | (_Params...)(_T_Return) $T*/...) \
    __step__fn_(__param_expand__fn_ __VA_ARGS__)
#define __step__fn_(...) __step__fn___emit(__VA_ARGS__)
#define __step__fn___emit(...) __block_inline__fn_(__VA_ARGS__)
#define __param_expand__fn_(...) (__VA_ARGS__), __param_expand__fn___next
#define __param_expand__fn___next(...) __VA_ARGS__
#define __block_inline__fn_(...) pp_overload(__block_inline__fn, __VA_ARGS__)(__VA_ARGS__)
#define __block_inline__fn_2(_ident_w_Params, _T_Return...) comp_syn__fn_(__param_extract__fn_ _ident_w_Params, _T_Return)
#define __block_inline__fn_3(_ident_w_Params_OR_Params, _T_Return, _$ext...) pp_join(_, __block_inline__fn_3, _$ext)(_ident_w_Params_OR_Params, _T_Return)
#define __block_inline__fn_3_$_scope(_ident_w_Params, _T_Return...) comp_syn__fn_$_scope(__param_extract__fn_ _ident_w_Params, _T_Return)
#define __block_inline__fn_3_$_guard(_ident_w_Params, _T_Return...) comp_syn__fn_$_guard(__param_extract__fn_ _ident_w_Params, _T_Return)
#define __block_inline__fn_3_$_T(_Params, _T_Return...) comp_syn__fn_$_T(_Params, _T_Return)
#define __param_extract__fn_(...) __VA_ARGS__

#define fn_$_scope(_ident_w_Params, _T_Return...) comp_syn__fn_$_scope(_ident_w_Params, _T_Return)
#define $unscoped_fn comp_syn__$unscoped_fn
#define $unscoped /* deprecated */ $unscoped_fn

#define fn_$_guard(_ident_w_Params, _T_Return...) comp_syn__fn_$_guard(_ident_w_Params, _T_Return)
#define $unguarded_fn comp_syn__$unguarded_fn
#define $unguarded /* deprecated */ $unguarded_fn

#define return_(_Expr...) comp_syn__return_(_Expr)
#define return_void(_Expr...) pp_overload(comp_syn__return_void, _Expr)(_Expr)

#define reservedReturn() (__reserved_return[0])
// #define ReturnType       TypeOf(reservedReturn())
#define reservedBreak() (__reserved_break[0])
// #define BreakType        TypeOf(reservedBreak())

/*
#define return_ok(_Expr...)   comp_syn__return_ok(_Expr)
#define return_err(_Expr...)  comp_syn__return_err(_Expr)
#define return_some(_Expr...) comp_syn__return_some(_Expr)
#define return_none()         comp_syn__return_none()
*/

#define defer_(_Expr...) comp_syn__defer(_Expr)

#define blk_defer comp_syn__blk_defer
#define blk_defer_ comp_syn__blk_defer
#define blk_deferral comp_syn__blk_deferral

#define break_defer comp_syn__break_defer

/*
#define errdefer_(_Expr...) comp_syn__errdefer_(_Expr)
*/

/*========== Macros and Definitions =========================================*/

#define comp_syn__fn_$_T(_Params, _T_Return...) TypeOf(_T_Return _Params)

#define comp_syn__fn_(_ident_w_Params, _T_Return...) _T_Return _ident_w_Params

// clang-format off
#define comp_syn__fn_$_scope(_ident_w_Params, _T_Return...) \
_T_Return _ident_w_Params { \
    let __reserved_return = ptrAlignCast$((_T_Return*)(A_ptr((A$$(sizeOf$(_T_Return), u8)){}))); \
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
}
// clang-format on

struct fn__ScopeCounter {
    u32 is_returning : 1;
    u32 current_line : 31;
};
// clang-format off
#define comp_syn__fn_$_guard(_ident_w_Params, _T_Return...) \
_T_Return _ident_w_Params { \
    volatile let __reserved_return = ptrAlignCast$((_T_Return*)(A_ptr((A$$(sizeOf$(_T_Return), u8)){}))); \
    $maybe_unused typedef TypeOf(*__reserved_return) ReturnType; \
    $maybe_unused typedef ReturnType ReturnT; \
    var __scope_counter   = (struct fn__ScopeCounter){ \
        .is_returning = false, .current_line = __LINE__ \
    }; \
    if (false) { __step_return: \
        __scope_counter.is_returning = true; \
        goto __step_deferred; \
    } \
__step_deferred: switch (__scope_counter.current_line) { \
    default: { goto __step_unscope; } break; \
    case __LINE__: __scope_counter.current_line = __LINE__ - 1;
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
}
// clang-format on

#if !on_comptime
#define fn__memset(_dst, _val, _len...) __fn_memset__no_hinting(_dst, _val, _len)
extern fn_((__fn_memset__no_hinting(void*, u32, usize))(void*));
#define fn__memcpy(_dst, _src, _len...) __fn_memcpy__no_hinting(_dst, _src, _len)
extern fn_((__fn_memcpy__no_hinting(void*, const void*, usize))(void*));
#define fn__memmove(_dst, _src, _len...) __fn_memmove__no_hinting(_dst, _src, _len)
extern fn_((__fn_memmove__no_hinting(void*, const void*, usize))(void*));
#else
#define fn__memset(_dst, _val, _len...) prim_memset(_dst, _val, _len)
#define fn__memcpy(_dst, _src, _len...) prim_memcpy(_dst, _src, _len)
#define fn__memmove(_dst, _src, _len...) prim_memmove(_dst, _src, _len)
#endif /* !on_comptime */

/* #define comp_syn__return_(_Expr...) blk({ \
    fn__memcpy( \
        as$(u8*)(__reserved_return), \
        as$(u8*)((TypeOf (*__reserved_return)[1]){ [0] = _Expr }), \
        sizeOf$(*__reserved_return) \
    ); \
    goto __step_return; \
}) */
#define comp_syn__return_(_Expr...) ({ \
    fn__memcpy( \
        as$(u8*)(__reserved_return), \
        as$(u8*)((ReturnT[1]){ [0] = _Expr }), \
        sizeOf$(ReturnT) \
    ); \
    goto __step_return; \
})
#define comp_syn__return_void_0() ({ \
    claim_assert_static( \
        Type_eq$(ReturnT, void) \
        || Type_eq$(ReturnT, Void) \
    ); \
    goto __step_return; \
})
#define comp_syn__return_void_1(_Expr...) ({ \
    claim_assert_static( \
        Type_eq$(ReturnT, void) \
        || Type_eq$(ReturnT, Void) \
    ); \
    claim_assert_static( \
        Type_eq$(TypeOf(({ _Expr; })), void) \
        || Type_eq$(TypeOf(({ _Expr; })), Void) \
    ); \
    $ignore_void _Expr; \
    goto __step_return; \
})

#define comp_syn__defer(_Expr...) comp_syn__defer__op_snapshot(_Expr; goto __step_deferred)

// clang-format off
#define comp_syn__blk_defer__expand(...) __VA_ARGS__
#define comp_syn__blk_defer { do { \
    comp_syn__defer__op_snapshot( \
        if (__scope_counter.is_returning) { \
            goto __step_deferred; \
        } else { \
            continue; \
        } \
    ) \
    do comp_syn__blk_defer__expand
#define comp_syn__blk_deferral \
    while(false); \
    goto __step_deferred; \
} while (false); }
// clang-format on

#define comp_syn__break_defer goto __step_deferred

#define comp_syn__defer__op_snapshot(_Expr...) \
    { \
        const u32 __scope_counter_previous_line = __scope_counter.current_line; \
        __scope_counter.current_line = __LINE__; \
        if (false) { \
        case __LINE__: \
            __scope_counter.current_line = __scope_counter_previous_line; \
            _Expr; \
        } \
    }

#define call(/*(_ident)(_Args...)*/...) pp_expand(pp_exec_defer(__exec_call)()(pp_Tuple_unwrapSufComma __VA_ARGS__))
#define __exec_call() __call
#define __call(_ident, _Args...) (ensureNonnull(_ident)(_Args))

// clang-format off
/* if-else as expression block */
#define expr_(/*<_T_Break $ext>|<else>*/...) __expr_(__VA_ARGS__) __expr_block
#define __expr_block(...) __VA_ARGS__
#define __expr_(...) pp_overload(__expr_, __VA_ARGS__)(__VA_ARGS__)
#define __expr__1(_else...) _else
#define __expr__2(_T_Break, _ext...) pp_cat(comp_syn__expr_, _ext)(_T_Break)
#define comp_syn__expr_$_scope(_T_Break...) ({ \
    $local_label __step_break, __step_unscope; \
    let __reserved_break = ptrAlignCast$((_T_Break*)(A_ptr((A$$(sizeOf$(_T_Break), u8)){}))); \
    $maybe_unused typedef TypeOfUnqual(*__reserved_break) BreakType; \
    $maybe_unused typedef BreakType BreakT; \
    $maybe_unused bool __has_broken = false; /* for integration with `eval_` */ \
    if (false) { __step_break: goto __step_unscope; } \
    /* do */
#define $unscoped_expr comp_syn__expr_$unscoped
#define comp_syn__expr_$unscoped \
    /* while(false) */; \
__step_unscope: \
    $supress_pointer_arith(T_switch$((BreakT)( \
        T_case$((void)({})), \
        T_default_(reservedBreak()) \
    ))); \
})
#define comp_syn__expr_$_guard(_T_Break...) ({ \
    $local_label __step_return_inner, __step_break, __step_deferred, __step_unscope; \
    if (false) { __step_return_inner: goto __step_return; } \
    volatile let __reserved_break = ptrAlignCast$((_T_Break*)(A_ptr((A$$(sizeOf$(_T_Break), u8)){}))); \
    $maybe_unused typedef TypeOfUnqual(*__reserved_break) BreakType; \
    $maybe_unused typedef BreakType BreakT; \
    var __scope_counter = (struct fn__ScopeCounter){ \
        .is_returning = false, .current_line = __LINE__ \
    }; \
    bool __has_broken = false; { \
        $local_label __step_return; \
        if (false) { goto __step_return; __step_return: goto __step_break; __step_break: \
            __scope_counter.is_returning = true; \
            goto __step_deferred; \
        } \
__step_deferred: switch (__scope_counter.current_line) { \
        default: { goto __step_unscope; } break; \
        case __LINE__: __scope_counter.current_line = __LINE__ - 1; \
            /* do */
#define $unguarded_expr comp_syn__expr_$unguarded
#define comp_syn__expr_$unguarded \
            /* while(false) */; \
            break; \
        } \
    } \
    __step_unscope: \
    if (!__has_broken) { goto __step_return_inner; } \
    $supress_pointer_arith(T_switch$((BreakT)( \
        T_case$((void)({})), \
        T_default_(reservedBreak()) \
    ))); \
})
// clang-format on

// clang-format off
#define $break_(_Expr...) ({ \
    fn__memcpy( \
        as$(u8*)(__reserved_break), \
        as$(u8*)((BreakT[1]){ [0] = _Expr }), \
        sizeOf$(BreakT) \
    ); \
    __has_broken = true; \
    goto __step_break; \
})

#define $break_void(_Expr...) pp_overload(comp_syn__$break_void, _Expr)(_Expr)
#define comp_syn__$break_void_0() blk({ \
    claim_assert_static( \
        Type_eq$(BreakT, void) \
        || Type_eq$(BreakT, Void) \
    ); \
    goto __step_break; \
})
#define comp_syn__$break_void_1(_Expr...) blk({ \
    claim_assert_static( \
        Type_eq$(BreakT, void) \
        || Type_eq$(BreakT, Void) \
    ); \
    claim_assert_static( \
        Type_eq$(TypeOf(({ _Expr; })), void) \
        || Type_eq$(TypeOf(({ _Expr; })), Void) \
    ); \
    _Expr; \
    goto __step_break; \
})
// clang-format on

/* TODO: else로 끝나지 않으면 컴파일 에러를 발생시키도록 만들기 */
// clang-format off
/* [for|while|switch|match]-else as expression block (supports else) */
#define eval_(/*<_T_Break $ext>|<else>*/...) inline__eval_(__VA_ARGS__) pp_expand
#define inline__eval_(...) pp_overload(inline__eval, __VA_ARGS__)(__VA_ARGS__)
#define inline__eval_1(_else...) ; if (__has_broken) { goto __step_break; } _else
#define inline__eval_2(_T_Break, _ext...) pp_cat(inline__eval_2, _ext)(_T_Break)
#define inline__eval_2$_scope(_T_Break...) ({ \
    $local_label __step_break; \
    let __reserved_break = ptrAlignCast$((_T_Break*)(A_ptr((A$$(sizeOf$(_T_Break), u8)){}))); \
    $maybe_unused typedef TypeOfUnqual(*__reserved_break) BreakType; \
    $maybe_unused typedef BreakType BreakT; \
    $maybe_unused bool __has_broken = false;\
    /* do */
#define $unscoped_eval comp_syn__eval_$unscoped
#define comp_syn__eval_$unscoped \
    /* while(false) */; \
    __step_break: \
    $supress_pointer_arith(T_switch$((BreakT)( \
        T_case$((void)({})), \
        T_default_(reservedBreak()) \
    ))); \
})
// clang-format on

#define $un_(_keyword) pp_cat(inline__$un_, _keyword)()
#define inline__$un_scoped() $unscoped_
#define inline__$un_guarded() $unguarded_

#define $unscoped_(_keyword) pp_cat(inline__$unscoped_, _keyword)()
#define inline__$unscoped_fn() $unscoped
#define inline__$unscoped_TEST_fn() $unscoped_TEST_fn
#define inline__$unscoped_Thrd_fn() $unscoped_Thrd_fn
#define inline__$unscoped_async_fn() $unscoped_async_fn
#define inline__$unscoped_la() $unscoped_la
#define inline__$unscoped_expr() $unscoped_expr
#define inline__$unscoped_eval() $unscoped_eval

#define $unguarded_(_keyword) pp_cat(inline__$unguarded_, _keyword)()
#define inline__$unguarded_fn() $unguarded
#define inline__$unguarded_TEST_fn() $unguarded_TEST_fn
#define inline__$unguarded_Thrd_fn() $unguarded_Thrd_fn
#define inline__$unguarded_async_fn() $unguarded_async_fn
#define inline__$unguarded_la() $unguarded_la
#define inline__$unguarded_expr() $unguarded_expr
#define inline__$unguarded_eval() $unguarded_eval

#define __cf_expand_fn_(_Expr...) auto __cf_expand_fn_expandAndIgnoreRest _Expr
#define __cf_expand_fn_expandAndIgnoreRest(_Expr...) _Expr pp_ignore;

/*========== Example usage ==================================================*/

#if EXAMPLE_USAGE
#include "dh/main.h"
#include "dh/math.h"

/* declarations =============================================================*/
use_ErrSet$(math_Err, i32);
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
} $unscoped_(fn);

/* definitions */
fn_((math_divideSafe(i32 lhs, i32 rhs))(math_E$i32) $scope) {
    if (rhs == 0) {
        return_err(math_Err_DivisionByZero());
    }
    return_ok(lhs / rhs);
} $unscoped_(fn);
#endif /* EXAMPLE_USAGE */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* core_fn__included */

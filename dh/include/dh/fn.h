/**
 * @copyright Copyright (c) 2025 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    fn.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2025-03-04 (date of creation)
 * @updated 2025-04-02 (date of last update)
 * @version v0.1-alpha
 * @ingroup dasae-headers(dh)
 * @prefix  NONE
 *
 * @brief   Function utilities
 * @details Provides utilities for function operations, including function pointers,
 *          function declarations, and function implementations.
 */

#ifndef FN_INCLUDED
#define FN_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "dh/scope.h"

/*========== Macros and Declarations ========================================*/

#define fn_(_Name_With_Params, T_Return...) comp_syn__fn_(_Name_With_Params, T_Return)

#define fn_scope(_Name_With_Params, T_Return...) comp_syn__fn_scope(_Name_With_Params, T_Return)
#define unscoped                                 comp_syn__unscoped

#define fn_scope_ext(_Name_With_Params, T_Return...) comp_syn__fn_scope_ext(_Name_With_Params, T_Return)
#define unscoped_ext                                 comp_syn__unscoped_ext

#define return_(_Expr...) comp_syn__return_(_Expr)
#define return_void()     comp_syn__return_void()

/*
#define return_ok(_Expr...)   comp_syn__return_ok(_Expr)
#define return_err(_Expr...)  comp_syn__return_err(_Expr)
#define return_some(_Expr...) comp_syn__return_some(_Expr)
#define return_none()         comp_syn__return_none()
 */

#define defer_(_Expr...) comp_syn__defer(_Expr)

#define block_defer    comp_syn__block_defer
#define block_deferral comp_syn__block_deferral

#define defer_break comp_syn__defer_break

/*
#define errdefer_(_Expr...) comp_syn__errdefer_(_Expr)
 */

/*========== Macros and Definitions =========================================*/

#define comp_syn__fn_(_Name_With_Params, T_Return...) \
    T_Return _Name_With_Params

// clang-format off
#define comp_syn__fn_scope(_Name_With_Params, T_Return...) \
T_Return _Name_With_Params {                               \
    let __reserved_return = as$(T_Return*,                 \
        (u8[bti_Generic_match$(T_Return,                   \
            bti_Generic_pattern$(void) 0,                  \
            bti_Generic_fallback_ sizeOf$(T_Return))]){}); \
    if (false) { __step_return: goto __step_unscope; }     \
    do
#define comp_syn__unscoped                    \
    while (false);                            \
    if (false) { __step_unscope:              \
        if (bti_Generic_match$(TypeOf(*__reserved_return),  \
            bti_Generic_pattern$(void) false, \
            bti_Generic_fallback_ true)       \
        ) { return __reserved_return[0]; }    \
    }                                         \
}
// clang-format on

struct fn__ScopeCounter {
    u32 is_returning : 1;
    u32 current_line : 31;
};
// clang-format off
#define comp_syn__fn_scope_ext(_Name_With_Params, T_Return...) \
T_Return _Name_With_Params {                                   \
    let __reserved_return = as$(T_Return*,                     \
        (u8[bti_Generic_match$(T_Return,                       \
            bti_Generic_pattern$(void) 0,                      \
            bti_Generic_fallback_ sizeOf$(T_Return))]){});     \
    var __scope_counter   = (struct fn__ScopeCounter){           \
        .is_returning = false, .current_line = __LINE__        \
    };                                                         \
    if (false) { __step_return:                                \
        __scope_counter.is_returning = true;                   \
        goto __step_deferred;                                  \
    }                                                          \
__step_deferred: switch (__scope_counter.current_line) {       \
    default: { goto __step_unscope; } break;                   \
    case __LINE__: __scope_counter.current_line = __LINE__ - 1;
#define comp_syn__unscoped_ext                             \
        break;                                             \
    }                                                      \
    if (false) { __step_unscope:                           \
        if (bti_Generic_match$(TypeOf(*__reserved_return), \
            bti_Generic_pattern$(void) false,              \
            bti_Generic_fallback_ true)                    \
        ) { return __reserved_return[0]; }                 \
    }                                                      \
}
// clang-format on

#define comp_syn__return_(_Expr...) eval({                         \
    bti_memcpy(                                                    \
        as$(u8*, __reserved_return),                               \
        as$(u8*, (TypeOf (*__reserved_return)[1]){ [0] = _Expr }), \
        sizeOf$(*__reserved_return)                                \
    );                                                             \
    goto __step_return;                                            \
})
#define comp_syn__return_void() eval({                                         \
    claim_assert_static(isSameType(TypeOf(*__reserved_return), TypeOf(void))); \
    goto __step_return;                                                        \
})

#define comp_syn__defer(_Expr...) \
    comp_syn__defer__op_snapshot(_Expr; goto __step_deferred)

// clang-format off
#define comp_syn__block_defer                   \
    do {                                        \
        comp_syn__defer__op_snapshot(               \
            if (__scope_counter.is_returning) { \
                goto __step_deferred;           \
            } else {                            \
                continue;                       \
            }                                   \
        )
#define comp_syn__block_deferral \
        goto __step_deferred;    \
    } while (false)
// clang-format on

#define comp_syn__defer_break \
    goto __step_deferred

#define comp_syn__defer__op_snapshot(_Expr...)                                  \
    {                                                                           \
        const u32 __scope_counter_previous_line = __scope_counter.current_line; \
        __scope_counter.current_line            = __LINE__;                     \
        if (false) {                                                            \
        case __LINE__:                                                          \
            __scope_counter.current_line = __scope_counter_previous_line;       \
            _Expr;                                                              \
        }                                                                       \
    }

/*========== Example usage ==================================================*/

#if EXAMPLE_USAGE
#include "dh/main.h"
#include "dh/math.h"
#include "dh/Str.h"

/* declarations =============================================================*/
use_ErrSet$(math_Err, i32);
static fn_(math_divideSafe(i32 lhs, i32 rhs), math_Err$i32) $must_check;

/* main */
fn_scope(dh_main(Sli$Str_const args), Err$void) {
    debug_assert_true(0 < args.len);
    debug_assert_true(try_(math_divideSafe(10, 2)) == 5);
    catch_from(math_divideSafe(10, 0), err, {
        let err_code = Str_viewZ(as$(const u8*, Err_codeToCStr(err)));
        debug_assert_true(Str_const_eq(err_code, Str_l("DivisionByZero")));
        return_err(err);
    });
    return_(ok({}));
} unscoped;

/* definitions */
fn_scope(math_divideSafe(i32 lhs, i32 rhs), math_Err$i32) {
    if (rhs == 0) {
        return_(err(math_Err_DivisionByZero()));
    }
    return_ok(lhs / rhs);
} unscoped;
#endif /* EXAMPLE_USAGE */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* FN_INCLUDED */

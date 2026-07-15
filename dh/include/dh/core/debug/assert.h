/**
 * @copyright Copyright (c) 2024-2026 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    assert.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2024-10-22 (date of creation)
 * @updated 2026-05-22 (date of last update)
 * @ingroup dasae-headers(dh)/core/debug
 * @prefix  debug_assert
 *
 * @brief   Conditional debug assertion utilities
 * @details Provides conditional utilities for runtime debug assertions
 *          that only execute when debug mode is enabled.
 */
#pragma once
#ifndef core_debug_assert__included
#define core_debug_assert__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "cfg.h"
#include "common.h"
#include "assert_static.h"

/*========== Macros and Declarations ========================================*/

#define debug_assert(_$Expr) __step__debug_assert((_$Expr), #_$Expr)
#define debug_assert_trap() __step__debug_assert_trap()
#define debug_assert_true(_$Expr) __step__debug_assert_true((_$Expr), #_$Expr)
#define debug_assert_false(_$Expr) __step__debug_assert_false((_$Expr), #_$Expr)
#define debug_assert_eq(_$Expr1, _$Expr2) __step__debug_assert_eq((_$Expr1), (_$Expr2), #_$Expr1, #_$Expr2)
#define debug_assert_ne(_$Expr1, _$Expr2) __step__debug_assert_ne((_$Expr1), (_$Expr2), #_$Expr1, #_$Expr2)
#define debug_assert_zero(_$Expr) __step__debug_assert_zero((_$Expr), #_$Expr)
#define debug_assert_nonzero(_$Expr) __step__debug_assert_nonzero((_$Expr), #_$Expr)
#define debug_assert_null(_$Expr) __step__debug_assert_null((_$Expr), #_$Expr)
#define debug_assert_nullS(_$Expr) __step__debug_assert_nullS((_$Expr), #_$Expr)
#define debug_assert_nonnull(_$Expr) __step__debug_assert_nonnull((_$Expr), #_$Expr)
#define debug_assert_nonnullS(_$Expr) __step__debug_assert_nonnullS((_$Expr), #_$Expr)

#define debug_assert_eqBy(_$Expr1, _$Expr2, _$eq) __step__debug_assert_eqBy((_$Expr1), (_$Expr2), _$eq, #_$Expr1, #_$Expr2)
#define debug_assert_neBy(_$Expr1, _$Expr2, _$ne) __step__debug_assert_neBy((_$Expr1), (_$Expr2), _$ne, #_$Expr1, #_$Expr2)

#define debug_assert_msg(_$Expr, _$msg) __step__debug_assert_msg((_$Expr), #_$Expr, _$msg)
#define debug_assert_trap_msg(_$msg) __step__debug_assert_trap_msg(_$msg)
#define debug_assert_true_msg(_$Expr, _$msg) __step__debug_assert_true_msg((_$Expr), #_$Expr, _$msg)
#define debug_assert_false_msg(_$Expr, _$msg) __step__debug_assert_false_msg((_$Expr), #_$Expr, _$msg)
#define debug_assert_eq_msg(_$Expr1, _$Expr2, _$msg) __step__debug_assert_eq_msg((_$Expr1), (_$Expr2), #_$Expr1, #_$Expr2, _$msg)
#define debug_assert_ne_msg(_$Expr1, _$Expr2, _$msg) __step__debug_assert_ne_msg((_$Expr1), (_$Expr2), #_$Expr1, #_$Expr2, _$msg)
#define debug_assert_zero_msg(_$Expr, _$msg) __step__debug_assert_zero_msg((_$Expr), #_$Expr, _$msg)
#define debug_assert_nonzero_msg(_$Expr, _$msg) __step__debug_assert_nonzero_msg((_$Expr), #_$Expr, _$msg)
#define debug_assert_null_msg(_$Expr, _$msg) __step__debug_assert_null_msg((_$Expr), #_$Expr, _$msg)
#define debug_assert_nullS_msg(_$Expr, _$msg) __step__debug_assert_nullS_msg((_$Expr), #_$Expr, _$msg)
#define debug_assert_nonnull_msg(_$Expr, _$msg) __step__debug_assert_nonnull_msg((_$Expr), #_$Expr, _$msg)
#define debug_assert_nonnullS_msg(_$Expr, _$msg) __step__debug_assert_nonnullS_msg((_$Expr), #_$Expr, _$msg)

#define debug_assert_fmt(_$Expr, _$fmt...) __step__debug_assert_fmt((_$Expr), #_$Expr, _$fmt)
#define debug_assert_trap_fmt(_$fmt...) __step__debug_assert_trap_fmt(_$fmt)
#define debug_assert_true_fmt(_$Expr, _$fmt...) __step__debug_assert_true_fmt((_$Expr), #_$Expr, _$fmt)
#define debug_assert_false_fmt(_$Expr, _$fmt...) __step__debug_assert_false_fmt((_$Expr), #_$Expr, _$fmt)
#define debug_assert_eq_fmt(_$Expr1, _$Expr2, _$fmt...) __step__debug_assert_eq_fmt((_$Expr1), (_$Expr2), #_$Expr1, #_$Expr2, _$fmt)
#define debug_assert_ne_fmt(_$Expr1, _$Expr2, _$fmt...) __step__debug_assert_ne_fmt((_$Expr1), (_$Expr2), #_$Expr1, #_$Expr2, _$fmt)
#define debug_assert_zero_fmt(_$Expr, _$fmt...) __step__debug_assert_zero_fmt((_$Expr), #_$Expr, _$fmt)
#define debug_assert_nonzero_fmt(_$Expr, _$fmt...) __step__debug_assert_nonzero_fmt((_$Expr), #_$Expr, _$fmt)
#define debug_assert_null_fmt(_$Expr, _$fmt...) __step__debug_assert_null_fmt((_$Expr), #_$Expr, _$fmt)
#define debug_assert_nonnull_fmt(_$Expr, _$fmt...) __step__debug_assert_nonnull_fmt((_$Expr), #_$Expr, _$fmt)
#define debug_assert_nonnullS_fmt(_$Expr, _$fmt...) __step__debug_assert_nonnullS_fmt((_$Expr), #_$Expr, _$fmt)

/*========== Macros and Definitions =========================================*/

#if debug_enabled
#if in_comptime
/* clang-format off */
#define __step__debug_assert(_$Expr, _$ExprStr...) $ignore_void(\
    (!!_$Expr) || (({ \
        debug_assert_static_msg( \
            comp_when_(isComptimeExpr(_$Expr))(comp_provide_(_$Expr), comp_instead_(true)), \
            _$ExprStr \
        ); \
        $debug_point debug_assert_failLog(\
            _$ExprStr, __func__, __FILE__, u32_(__LINE__) \
        ); \
        $unreachable; \
    }), 0) \
)
#define __step__debug_assert_msg(_$Expr, _$ExprStr, _$msg...) $ignore_void( \
    (!!_$Expr) || (({ \
        debug_assert_static_msg( \
            comp_when_(isComptimeExpr(_$Expr))(comp_provide_(_$Expr), comp_instead_(true)), \
            _$msg \
        ); \
        $debug_point debug_assert_failLogMsg( \
            _$ExprStr, __func__, __FILE__, u32_(__LINE__), _$msg \
        ); \
        $unreachable; \
    }), 0) \
)
#define __step__debug_assert_fmt(_$Expr, _$ExprStr, _$fmt...) $ignore_void(\
    (!!_$Expr) || (({ \
        debug_assert_static_msg( \
            comp_when_(isComptimeExpr(_$Expr))(comp_provide_(_$Expr), comp_instead_(true)), \
            _$ExprStr \
        ); \
        $debug_point debug_assert_failLogFmt( \
            _$ExprStr, __func__, __FILE__, u32_(__LINE__), _$fmt \
        ); \
        $unreachable; \
    }), 0) \
)

#define __step__debug_assert_trap() $ignore_void(({ \
    $debug_point debug_assert_failLog( \
        "(none)", __func__, __FILE__, u32_(__LINE__) \
    ); \
    $unreachable; \
}), 0)
#define __step__debug_assert_trap_msg(_$msg...) $ignore_void(({ \
    $debug_point debug_assert_failLogMsg( \
        "(none)", __func__, __FILE__, u32_(__LINE__), _$msg \
    ); \
    $unreachable; \
}), 0)
#define __step__debug_assert_trap_fmt(_$fmt...) $ignore_void(({ \
    $debug_point debug_assert_failLogFmt( \
        "(none)", __func__, __FILE__, u32_(__LINE__), _$fmt \
    ); \
    $unreachable; \
}), 0)
/* clang-format on */
#else /* !in_comptime */
/* clang-format off */
#define __step__debug_assert(_$Expr, _$ExprStr...) $dispatch_in_comptime $ignore_void( \
    (!!_$Expr) || (({ \
        debug_assert_static_msg( \
            comp_when_(isComptimeExpr(_$Expr))(comp_provide_(_$Expr), comp_instead_(true)), \
            _$ExprStr \
        ); \
        $debug_point $unreachable; \
    }), 0) \
)
#define __step__debug_assert_msg(_$Expr, _$ExprStr, _$msg...) $dispatch_in_comptime $ignore_void( \
    (!!_$Expr) || (({ \
        debug_assert_static_msg( \
            comp_when_(isComptimeExpr(_$Expr))(comp_provide_(_$Expr), comp_instead_(true)), \
            _$msg \
        ); \
        $debug_point $unreachable; \
    }), 0) \
)
#define __step__debug_assert_fmt(_$Expr, _$ExprStr, _$fmt...) $dispatch_in_comptime $ignore_void( \
    (!!_$Expr) || (({ \
        debug_assert_static_msg( \
            comp_when_(isComptimeExpr(_$Expr))(comp_provide_(_$Expr), comp_instead_(true)), \
            _$ExprStr \
        ); \
        $debug_point $unreachable; \
    }), 0) \
)

#define __step__debug_assert_trap()            $dispatch_in_comptime $ignore_void($debug_point $unreachable, 0)
#define __step__debug_assert_trap_msg(_$msg...) $dispatch_in_comptime $ignore_void($debug_point $unreachable, 0)
#define __step__debug_assert_trap_fmt(_$fmt...) $dispatch_in_comptime $ignore_void($debug_point $unreachable, 0)
/* clang-format on */
#endif /* in_comptime */

#define __step__debug_assert_true(_$Expr, _$ExprStr...) __step__debug_assert_msg(((_$Expr) == true), _$ExprStr " != true", _$ExprStr " is not true")
#define __step__debug_assert_false(_$Expr, _$ExprStr...) __step__debug_assert_msg(((_$Expr) == false), _$ExprStr " != false", _$ExprStr " is not false")
#define __step__debug_assert_eq(_$Expr1, _$Expr2, _$Expr1Str, _$Expr2Str...) \
    __step__debug_assert_msg(((_$Expr1) == (_$Expr2)), _$Expr1Str " == " _$Expr2Str, _$Expr1Str " is not equal to " _$Expr2Str)
#define __step__debug_assert_ne(_$Expr1, _$Expr2, _$Expr1Str, _$Expr2Str...) \
    __step__debug_assert_msg(((_$Expr1) != (_$Expr2)), _$Expr1Str " != " _$Expr2Str, _$Expr1Str " is equal to " _$Expr2Str)
#define __step__debug_assert_zero(_$Expr, _$ExprStr...) __step__debug_assert_msg(((_$Expr) == 0), _$ExprStr " != 0", _$ExprStr " is not zero")
#define __step__debug_assert_nonzero(_$Expr, _$ExprStr...) __step__debug_assert_msg(((_$Expr) != 0), _$ExprStr " == 0", _$ExprStr " is zero")
#define __step__debug_assert_null(_$Expr, _$ExprStr...) __step__debug_assert_msg(((_$Expr) == null$(TypeOf(_$Expr))), _$ExprStr " != null", _$ExprStr " is nonnull")
#define __step__debug_assert_nullS(_$Expr, _$ExprStr...) __step__debug_assert_msg(((_$Expr).ptr == null$(TypeOf((_$Expr).ptr))), _$ExprStr " != null", _$ExprStr " is nonnull")
#define __step__debug_assert_nonnull(_$Expr, _$ExprStr...) __step__debug_assert_msg(((_$Expr) != null$(TypeOf(_$Expr))), _$ExprStr " == null", _$ExprStr " is null")
#define __step__debug_assert_nonnullS(_$Expr, _$ExprStr...) __step__debug_assert_msg(((_$Expr).ptr != null$(TypeOf((_$Expr).ptr))), _$ExprStr " == null", _$ExprStr " is null")

#define __step__debug_assert_eqBy(_$Expr1, _$Expr2, _$eq, _$Expr1Str, _$Expr2Str...) \
    __step__debug_assert_msg((_$eq(_$Expr1, _$Expr2)), _$Expr1Str " == " _$Expr2Str, _$Expr1Str " is not equal to " _$Expr2Str)
#define __step__debug_assert_neBy(_$Expr1, _$Expr2, _$ne, _$Expr1Str, _$Expr2Str...) \
    __step__debug_assert_msg((_$ne(_$Expr1, _$Expr2)), _$Expr1Str " != " _$Expr2Str, _$Expr1Str " is equal to " _$Expr2Str)

#define __step__debug_assert_true_msg(_$Expr, _$ExprStr, _$msg...) __step__debug_assert_msg(((_$Expr) == true), _$ExprStr " != true", _$msg)
#define __step__debug_assert_false_msg(_$Expr, _$ExprStr, _$msg...) __step__debug_assert_msg(((_$Expr) == false), _$ExprStr " != false", _$msg)
#define __step__debug_assert_eq_msg(_$Expr1, _$Expr2, _$Expr1Str, _$Expr2Str, _$msg...) \
    __step__debug_assert_msg(((_$Expr1) == (_$Expr2)), _$Expr1Str " == " _$Expr2Str, _$msg)
#define __step__debug_assert_ne_msg(_$Expr1, _$Expr2, _$Expr1Str, _$Expr2Str, _$msg...) \
    __step__debug_assert_msg(((_$Expr1) != (_$Expr2)), _$Expr1Str " != " _$Expr2Str, _$msg)
#define __step__debug_assert_zero_msg(_$Expr, _$ExprStr, _$msg...) __step__debug_assert_msg(((_$Expr) == 0), _$ExprStr " != 0", _$msg)
#define __step__debug_assert_nonzero_msg(_$Expr, _$ExprStr, _$msg...) __step__debug_assert_msg(((_$Expr) != 0), _$ExprStr " == 0", _$msg)
#define __step__debug_assert_null_msg(_$Expr, _$ExprStr, _$msg...) __step__debug_assert_msg(((_$Expr) == null$(TypeOf(_$Expr))), _$ExprStr " != null", _$msg)
#define __step__debug_assert_nullS_msg(_$Expr, _$ExprStr, _$msg...) __step__debug_assert_msg(((_$Expr).ptr == null$(TypeOf((_$Expr).ptr))), _$ExprStr " != null", _$msg)
#define __step__debug_assert_nonnull_msg(_$Expr, _$ExprStr, _$msg...) __step__debug_assert_msg(((_$Expr) != null$(TypeOf(_$Expr))), _$ExprStr " == null", _$msg)
#define __step__debug_assert_nonnullS_msg(_$Expr, _$ExprStr, _$msg...) __step__debug_assert_msg(((_$Expr).ptr != null$(TypeOf((_$Expr).ptr))), _$ExprStr " == null", _$msg)

#define __step__debug_assert_true_fmt(_$Expr, _$ExprStr, _$fmt...) __step__debug_assert_fmt(((_$Expr) == true), _$ExprStr " != true", _$fmt)
#define __step__debug_assert_false_fmt(_$Expr, _$ExprStr, _$fmt...) __step__debug_assert_fmt(((_$Expr) == false), _$ExprStr " != false", _$fmt)
#define __step__debug_assert_eq_fmt(_$Expr1, _$Expr2, _$Expr1Str, _$Expr2Str, _$fmt...) \
    __step__debug_assert_fmt(((_$Expr1) == (_$Expr2)), _$Expr1Str " == " _$Expr2Str, _$fmt)
#define __step__debug_assert_ne_fmt(_$Expr1, _$Expr2, _$Expr1Str, _$Expr2Str, _$fmt...) \
    __step__debug_assert_fmt(((_$Expr1) != (_$Expr2)), _$Expr1Str " != " _$Expr2Str, _$fmt)
#define __step__debug_assert_zero_fmt(_$Expr, _$ExprStr, _$fmt...) __step__debug_assert_fmt(((_$Expr) == 0), _$ExprStr " != 0", _$fmt)
#define __step__debug_assert_nonzero_fmt(_$Expr, _$ExprStr, _$fmt...) __step__debug_assert_fmt(((_$Expr) != 0), _$ExprStr " == 0", _$fmt)
#define __step__debug_assert_null_fmt(_$Expr, _$ExprStr, _$fmt...) __step__debug_assert_fmt(((_$Expr) == null$(TypeOf(_$Expr))), _$ExprStr " != null", _$fmt)
#define __step__debug_assert_nullS_fmt(_$Expr, _$ExprStr, _$fmt...) __step__debug_assert_fmt(((_$Expr).ptr == null$(TypeOf((_$Expr).ptr))), _$ExprStr " != null", _$fmt)
#define __step__debug_assert_nonnull_fmt(_$Expr, _$ExprStr, _$fmt...) __step__debug_assert_fmt(((_$Expr) != null$(TypeOf(_$Expr))), _$ExprStr " == null", _$fmt)
#define __step__debug_assert_nonnullS_fmt(_$Expr, _$ExprStr, _$fmt...) __step__debug_assert_fmt(((_$Expr).ptr != null$(TypeOf((_$Expr).ptr))), _$ExprStr " == null", _$fmt)

#else /* !debug_enabled */

#define __step__debug_assert(_$Expr, _$ExprStr...) $unused(0)
#define __step__debug_assert_msg(_$Expr, _$ExprStr, _$msg...) $unused(0)
#define __step__debug_assert_fmt(_$Expr, _$ExprStr, _$fmt...) $unused(0)

#define __step__debug_assert_trap() $unused(0)
#define __step__debug_assert_trap_msg(_$msg...) $unused(0)
#define __step__debug_assert_trap_fmt(_$fmt...) $unused(0)

#define __step__debug_assert_true(_$Expr, _$ExprStr...) $unused(0)
#define __step__debug_assert_false(_$Expr, _$ExprStr...) $unused(0)
#define __step__debug_assert_eq(_$Expr1, _$Expr2, _$Expr1Str, _$Expr2Str...) $unused(0)
#define __step__debug_assert_ne(_$Expr1, _$Expr2, _$Expr1Str, _$Expr2Str...) $unused(0)
#define __step__debug_assert_zero(_$Expr, _$ExprStr...) $unused(0)
#define __step__debug_assert_nonzero(_$Expr, _$ExprStr...) $unused(0)
#define __step__debug_assert_null(_$Expr, _$ExprStr...) $unused(0)
#define __step__debug_assert_nullS(_$Expr, _$ExprStr...) $unused(0)
#define __step__debug_assert_nonnull(_$Expr, _$ExprStr...) $unused(0)
#define __step__debug_assert_nonnullS(_$Expr, _$ExprStr...) $unused(0)

#define __step__debug_assert_eqBy(_$Expr1, _$Expr2, _$eq, _$Expr1Str, _$Expr2Str...) $unused(0)
#define __step__debug_assert_neBy(_$Expr1, _$Expr2, _$ne, _$Expr1Str, _$Expr2Str...) $unused(0)

#define __step__debug_assert_true_msg(_$Expr, _$ExprStr, _$msg...) $unused(0)
#define __step__debug_assert_false_msg(_$Expr, _$ExprStr, _$msg...) $unused(0)
#define __step__debug_assert_eq_msg(_$Expr1, _$Expr2, _$Expr1Str, _$Expr2Str, _$msg...) $unused(0)
#define __step__debug_assert_ne_msg(_$Expr1, _$Expr2, _$Expr1Str, _$Expr2Str, _$msg...) $unused(0)
#define __step__debug_assert_zero_msg(_$Expr, _$ExprStr, _$msg...) $unused(0)
#define __step__debug_assert_nonzero_msg(_$Expr, _$ExprStr, _$msg...) $unused(0)
#define __step__debug_assert_null_msg(_$Expr, _$ExprStr, _$msg...) $unused(0)
#define __step__debug_assert_nullS_msg(_$Expr, _$ExprStr, _$msg...) $unused(0)
#define __step__debug_assert_nonnull_msg(_$Expr, _$ExprStr, _$msg...) $unused(0)
#define __step__debug_assert_nonnullS_msg(_$Expr, _$ExprStr, _$msg...) $unused(0)

#define __step__debug_assert_true_fmt(_$Expr, _$ExprStr, _$fmt...) $unused(0)
#define __step__debug_assert_false_fmt(_$Expr, _$ExprStr, _$fmt...) $unused(0)
#define __step__debug_assert_eq_fmt(_$Expr1, _$Expr2, _$Expr1Str, _$Expr2Str, _$fmt...) $unused(0)
#define __step__debug_assert_ne_fmt(_$Expr1, _$Expr2, _$Expr1Str, _$Expr2Str, _$fmt...) $unused(0)
#define __step__debug_assert_zero_fmt(_$Expr, _$ExprStr, _$fmt...) $unused(0)
#define __step__debug_assert_nonzero_fmt(_$Expr, _$ExprStr, _$fmt...) $unused(0)
#define __step__debug_assert_null_fmt(_$Expr, _$ExprStr, _$fmt...) $unused(0)
#define __step__debug_assert_nullS_fmt(_$Expr, _$ExprStr, _$fmt...) $unused(0)
#define __step__debug_assert_nonnull_fmt(_$Expr, _$ExprStr, _$fmt...) $unused(0)
#define __step__debug_assert_nonnullS_fmt(_$Expr, _$ExprStr, _$fmt...) $unused(0)
#endif /* debug_enabled */

/*========== Extern Function Prototypes =====================================*/

#if in_comptime
/**
 * @brief Logs an assertion failure with the given expression, function, file, and line.
 *
 * @param expr The expression that failed the assertion.
 * @param func The function where the assertion failed.
 * @param file The file where the assertion failed.
 * @param line The line number where the assertion failed.
 */
$attr($branch_cold)
$extern fn_((debug_assert_failLog(const char* expr, const char* func, const char* file, u32 line))(void));
/**
 * @brief Logs an assertion failure with the given expression, function, file, line, and message.
 *
 * @param expr The expression that failed the assertion.
 * @param func The function where the assertion failed.
 * @param file The file where the assertion failed.
 * @param line The line number where the assertion failed.
 * @param msg The message to include in the assertion failure.
 */
$attr($branch_cold)
$extern fn_((debug_assert_failLogMsg(const char* expr, const char* func, const char* file, u32 line, const char* msg))(void));
/**
 * @brief Logs an assertion failure with the given expression, function, file, line, and formatted message.
 *
 * @param expr The expression that failed the assertion.
 * @param func The function where the assertion failed.
 * @param file The file where the assertion failed.
 * @param line The line number where the assertion failed.
 * @param fmt The formatted message to include in the assertion failure.
 * @param ... The arguments for the formatted message.
 */
$attr($branch_cold)
$extern fn_((debug_assert_failLogFmt(const char* expr, const char* func, const char* file, u32 line, const char* fmt, ...))(void));
#else /* !in_comptime */
$attr($branch_cold)
$extern fn_((debug_assert_failLog(const char*, const char*, const char*, u32))(void));
$attr($branch_cold)
$extern fn_((debug_assert_failLogMsg(const char*, const char*, const char*, u32, const char*))(void));
$attr($branch_cold)
$extern fn_((debug_assert_failLogFmt(const char*, const char*, const char*, u32, const char*, ...))(void));
#endif /* in_comptime */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* core_debug_assert__included */

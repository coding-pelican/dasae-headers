/**
 * @copyright Copyright (c) 2024-2025 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    assert.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2024-11-21 (date of creation)
 * @updated 2025-02-02 (date of last update)
 * @version v0.1-alpha.1
 * @ingroup dasae-headers(dh)/core/claim
 * @prefix  claim_assert
 *
 * @brief   Unconditional assertion utilities
 * @details Provides unconditional utilities for runtime assertions
 */

#ifndef core_claim_assert__included
#define core_claim_assert__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "cfg.h"
#include "unreachable.h"
#include "assert_static.h"

/*========== Macros and Declarations ========================================*/

#define claim_assert(_Expr) \
    __step__claim_assert((_Expr), #_Expr)
#define claim_assert_trap() \
    __step__claim_assert_trap()
#define claim_assert_true(_Expr) \
    __step__claim_assert_true((_Expr), #_Expr)
#define claim_assert_false(_Expr) \
    __step__claim_assert_false((_Expr), #_Expr)
#define claim_assert_eq(_Expr1, _Expr2) \
    __step__claim_assert_eq((_Expr1), (_Expr2), #_Expr1, #_Expr2)
#define claim_assert_ne(_Expr1, _Expr2) \
    __step__claim_assert_ne((_Expr1), (_Expr2), #_Expr1, #_Expr2)
#define claim_assert_null(_Expr) \
    __step__claim_assert_null((_Expr), #_Expr)
#define claim_assert_nonnull(_Expr) \
    __step__claim_assert_nonnull((_Expr), #_Expr)

#define claim_assert_msg(_Expr, _msg) \
    __step__claim_assert_msg((_Expr), #_Expr, _msg)
#define claim_assert_trap_msg(_msg) \
    __step__claim_assert_trap_msg(_msg)
#define claim_assert_true_msg(_Expr, _msg) \
    __step__claim_assert_true_msg((_Expr), #_Expr, _msg)
#define claim_assert_false_msg(_Expr, _msg) \
    __step__claim_assert_false_msg((_Expr), #_Expr, _msg)
#define claim_assert_eq_msg(_Expr1, _Expr2, _msg) \
    __step__claim_assert_eq_msg((_Expr1), (_Expr2), #_Expr1, #_Expr2, _msg)
#define claim_assert_ne_msg(_Expr1, _Expr2, _msg) \
    __step__claim_assert_ne_msg((_Expr1), (_Expr2), #_Expr1, #_Expr2, _msg)
#define claim_assert_null_msg(_Expr, _msg) \
    __step__claim_assert_null_msg((_Expr), #_Expr, _msg)
#define claim_assert_nonnull_msg(_Expr, _msg) \
    __step__claim_assert_nonnull_msg((_Expr), #_Expr, _msg)

#define claim_assert_fmt(_Expr, _fmt...) \
    __step__claim_assert_fmt((_Expr), #_Expr, _fmt)
#define claim_assert_trap_fmt(_fmt...) \
    __step__claim_assert_trap_fmt(_fmt)
#define claim_assert_true_fmt(_Expr, _fmt...) \
    __step__claim_assert_true_fmt((_Expr), #_Expr, _fmt)
#define claim_assert_false_fmt(_Expr, _fmt...) \
    __step__claim_assert_false_fmt((_Expr), #_Expr, _fmt)
#define claim_assert_eq_fmt(_Expr1, _Expr2, _fmt...) \
    __step__claim_assert_eq_fmt((_Expr1), (_Expr2), #_Expr1, #_Expr2, _fmt)
#define claim_assert_ne_fmt(_Expr1, _Expr2, _fmt...) \
    __step__claim_assert_ne_fmt((_Expr1), (_Expr2), #_Expr1, #_Expr2, _fmt)
#define claim_assert_null_fmt(_Expr, _fmt...) \
    __step__claim_assert_null_fmt((_Expr), #_Expr, _fmt)
#define claim_assert_nonnull_fmt(_Expr, _fmt...) \
    __step__claim_assert_nonnull_fmt((_Expr), #_Expr, _fmt)

/*========== Macros and Definitions =========================================*/

#if on_comptime
/* clang-format off */
#define __step__claim_assert(_Expr, _ExprStr...) $ignore_void(\
    (!!_Expr) || (({ \
        claim_assert_static_msg(isCompTimeFoldable(_Expr) ? _Expr : true, _ExprStr); \
        claim_assert_failLog(_ExprStr, __func__, __FILE__, __LINE__); \
        claim_unreachable; \
    }), 0) \
)
#define __step__claim_assert_msg(_Expr, _ExprStr, _msg...) $ignore_void( \
    (!!_Expr) || (({ \
        claim_assert_static_msg(isCompTimeFoldable(_Expr) ? _Expr : true, _msg); \
        claim_assert_failLogMsg(_ExprStr, __func__, __FILE__, __LINE__, _msg); \
        claim_unreachable; \
    }), 0) \
)
#define __step__claim_assert_fmt(_Expr, _ExprStr, _fmt...) $ignore_void(\
    (!!_Expr) || (({ \
        claim_assert_static_msg(isCompTimeFoldable(_Expr) ? _Expr : true, _ExprStr); \
        claim_assert_failLogFmt(_ExprStr, __func__, __FILE__, __LINE__, _fmt); \
        claim_unreachable; \
    }), 0) \
)

#define __step__claim_assert_trap() $ignore_void(({ \
    claim_assert_failLog("(none)", __func__, __FILE__, __LINE__); \
    claim_unreachable; \
}), 0)
#define __step__claim_assert_trap_msg(_msg...) $ignore_void(({ \
    claim_assert_failLogMsg("(none)", __func__, __FILE__, __LINE__, _msg); \
    claim_unreachable; \
}), 0)
#define __step__claim_assert_trap_fmt(_fmt...) $ignore_void(({ \
    claim_assert_failLogFmt("(none)", __func__, __FILE__, __LINE__, _fmt); \
    claim_unreachable; \
}), 0)
/* clang-format on */
#else  /* !on_comptime */
/* clang-format off */
#define __step__claim_assert(_Expr, _ExprStr...) $dispatch_on_comptime $ignore_void( \
    (!!_Expr) || (({ \
        claim_assert_static_msg(isCompTimeFoldable(_Expr) ? _Expr : true, _ExprStr); \
        unreachable; \
    }), 0) \
)
#define __step__claim_assert_msg(_Expr, _ExprStr, _msg...) $dispatch_on_comptime $ignore_void( \
    (!!_Expr) || (({ \
        claim_assert_static_msg(isCompTimeFoldable(_Expr) ? _Expr : true, _msg); \
        unreachable; \
    }), 0) \
)
#define __step__claim_assert_fmt(_Expr, _ExprStr, fmt...) $dispatch_on_comptime $ignore_void( \
    (!!_Expr) || (({ \
        claim_assert_static_msg(isCompTimeFoldable(_Expr) ? _Expr : true, _ExprStr); \
        unreachable; \
    }), 0) \
)

#define __step__claim_assert_trap()            $dispatch_on_comptime $ignore_void(unreachable, 0)
#define __step__claim_assert_trap_msg(_msg...) $dispatch_on_comptime $ignore_void(unreachable, 0)
#define __step__claim_assert_trap_fmt(_fmt...) $dispatch_on_comptime $ignore_void(unreachable, 0)
/* clang-format on */
#endif /* on_comptime */

#define __step__claim_assert_true(_Expr, _ExprStr...)                    __step__claim_assert_msg(((_Expr) == true), _ExprStr " != true", _ExprStr " is not true")
#define __step__claim_assert_false(_Expr, _ExprStr...)                   __step__claim_assert_msg(((_Expr) == false), _ExprStr " != false", _ExprStr " is not false")
#define __step__claim_assert_eq(_Expr1, _Expr2, _Expr1Str, _Expr2Str...) __step__claim_assert_msg(((_Expr1) == (_Expr2)), _Expr1Str " == " _Expr2Str, _Expr1Str " is not equal to " _Expr2Str)
#define __step__claim_assert_ne(_Expr1, _Expr2, _Expr1Str, _Expr2Str...) __step__claim_assert_msg(((_Expr1) != (_Expr2)), _Expr1Str " != " _Expr2Str, _Expr1Str " is equal to " _Expr2Str)
#define __step__claim_assert_null(_Expr, _ExprStr...)                    __step__claim_assert_msg(((_Expr) == null), _ExprStr " != null", _ExprStr " is nonnull")
#define __step__claim_assert_nonnull(_Expr, _ExprStr...)                 __step__claim_assert_msg(((_Expr) != null), _ExprStr " == null", _ExprStr " is null")

#define __step__claim_assert_true_msg(_Expr, _ExprStr, _msg...)                    __step__claim_assert_msg(((_Expr) == true), _ExprStr " != true", _msg)
#define __step__claim_assert_false_msg(_Expr, _ExprStr, _msg...)                   __step__claim_assert_msg(((_Expr) == false), _ExprStr " != false", _msg)
#define __step__claim_assert_eq_msg(_Expr1, _Expr2, _Expr1Str, _Expr2Str, _msg...) __step__claim_assert_msg(((_Expr1) == (_Expr2)), _Expr1Str " == " _Expr2Str, _msg)
#define __step__claim_assert_ne_msg(_Expr1, _Expr2, _Expr1Str, _Expr2Str, _msg...) __step__claim_assert_msg(((_Expr1) != (_Expr2)), _Expr1Str " != " _Expr2Str, _msg)
#define __step__claim_assert_null_msg(_Expr, _ExprStr, _msg...)                    __step__claim_assert_msg(((_Expr) == null), _ExprStr " != null", _msg)
#define __step__claim_assert_nonnull_msg(_Expr, _ExprStr, _msg...)                 __step__claim_assert_msg(((_Expr) != null), _ExprStr " == null", _msg)

#define __step__claim_assert_true_fmt(_Expr, _ExprStr, _fmt...)                    __step__claim_assert_fmt(((_Expr) == true), _ExprStr " != true", _fmt)
#define __step__claim_assert_false_fmt(_Expr, _ExprStr, _fmt...)                   __step__claim_assert_fmt(((_Expr) == false), _ExprStr " != false", _fmt)
#define __step__claim_assert_eq_fmt(_Expr1, _Expr2, _Expr1Str, _Expr2Str, _fmt...) __step__claim_assert_fmt(((_Expr1) == (_Expr2)), _Expr1Str " == " _Expr2Str, _fmt)
#define __step__claim_assert_ne_fmt(_Expr1, _Expr2, _Expr1Str, _Expr2Str, _fmt...) __step__claim_assert_fmt(((_Expr1) != (_Expr2)), _Expr1Str " != " _Expr2Str, _fmt)
#define __step__claim_assert_null_fmt(_Expr, _ExprStr, _fmt...)                    __step__claim_assert_fmt(((_Expr) == null), _ExprStr " != null", _fmt)
#define __step__claim_assert_nonnull_fmt(_Expr, _ExprStr, _fmt...)                 __step__claim_assert_fmt(((_Expr) != null), _ExprStr " == null", _fmt)

/*========== Extern Function Prototypes =====================================*/

#if claim_fail_logging_comp_enabled
#if on_comptime
/**
 * @brief Logs an assertion failure with the given expression, function, file, and line.
 *
 * @param expr The expression that failed the assertion.
 * @param func The function where the assertion failed.
 * @param file The file where the assertion failed.
 * @param line The line number where the assertion failed.
 */
$extern fn_((claim_assert_failLog(const char* expr, const char* func, const char* file, i32 line))(void));
/**
 * @brief Logs an assertion failure with the given expression, function, file, line, and message.
 *
 * @param expr The expression that failed the assertion.
 * @param func The function where the assertion failed.
 * @param file The file where the assertion failed.
 * @param line The line number where the assertion failed.
 * @param msg The message to include in the assertion failure.
 */
$extern fn_((claim_assert_failLogMsg(const char* expr, const char* func, const char* file, i32 line, const char* msg))(void));
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
$extern fn_((claim_assert_failLogFmt(const char* expr, const char* func, const char* file, i32 line, const char* fmt, ...))(void));
#else  /* !on_comptime */
$extern fn_((claim_assert_failLog(const char*, const char*, const char*, i32))(void));
$extern fn_((claim_assert_failLogMsg(const char*, const char*, const char*, i32, const char*))(void));
$extern fn_((claim_assert_failLogFmt(const char*, const char*, const char*, i32, const char*, ...))(void));
#endif /* on_comptime */
#else  /* !claim_fail_logging_comp_enabled */
#define claim_assert_failLog(_expr, _func, _file, _line)               $unused(0)
#define claim_assert_failLogMsg(_expr, _func, _file, _line, _msg)      $unused(0)
#define claim_assert_failLogFmt(_expr, _func, _file, _line, _fmt, ...) $unused(0)
#endif /* claim_fail_logging_comp_enabled */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* core_claim_assert__included */

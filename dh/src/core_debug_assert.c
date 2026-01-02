/**
 * @copyright Copyright 2024-2025. Gyeongtae Kim All rights reserved.
 *
 * @file    debug_assert.c
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2024-11-02 (date of creation)
 * @updated 2025-02-02 (date of last update)
 * @version v0.1-alpha.1
 * @ingroup dasae-headers(dh)/debug
 * @prefix  debug_assert
 *
 * @brief   Source for debug assertion checks
 * @details Implementation of debug assertion functions
 */

/*========== Includes =======================================================*/

#include "dh/core/debug/assert.h"
#if debug_comp_enabled
#include <stdarg.h> /* For va_list, va_start(), va_end() */
#include <stdio.h> /* For fprintf(), stderr */
/* TODO: Remove printing based stdio and fprintf */

/*========== Extern Function Implementations ================================*/

$static fn_((debug_assert__output(void))(FILE*)) { return stderr; }

$static let_(debug_assert__fmt_default, const char*) = "Assertion failed: %s, in function %s, at file %s, line %d\n";
$static let_(debug_assert__fmt_msg, const char*) = "Assertion failed: %s\n";
$static let_(debug_assert__fmt_loc, const char*) = "\nin function %s, at file %s, line %d\n";

fn_((debug_assert_failLog(const char* expr, const char* func, const char* file, i32 line))(void)) {
    let output = debug_assert__output();
    let_ignore = fprintf(
        output, debug_assert__fmt_default,
        expr, func, file, line
    );
}

fn_((debug_assert_failLogMsg(const char* expr, const char* func, const char* file, i32 line, const char* msg))(void)) {
    let output = debug_assert__output();
    let_ignore = fprintf(output, debug_assert__fmt_msg, expr);
    let_ignore = fprintf(output, "%s", msg);
    let_ignore = fprintf(
        output, debug_assert__fmt_loc,
        func, file, line
    );
}

fn_((debug_assert_failLogFmt(const char* expr, const char* func, const char* file, i32 line, const char* fmt, ...))(void)) {
    let output = debug_assert__output();
    let_ignore = fprintf(output, debug_assert__fmt_msg, expr);
    va_list args = {};
    va_start(args, fmt);
    let_ignore = vfprintf(
        output, fmt,
        args
    );
    va_end(args);
    let_ignore = fprintf(
        output, debug_assert__fmt_loc,
        func, file, line
    );
}
#endif /* debug_comp_enabled */

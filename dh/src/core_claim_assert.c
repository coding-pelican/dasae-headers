/**
 * @copyright Copyright 2024-2025. Gyeongtae Kim All rights reserved.
 *
 * @file    claim_assert.c
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2024-11-21 (date of creation)
 * @updated 2025-02-02 (date of last update)
 * @version v0.1-alpha.1
 * @ingroup dasae-headers(dh)/claim
 * @prefix  claim_assert
 *
 * @brief   Source for unconditional assertion checks
 * @details Implementation of claim assertion functions
 */

/*========== Includes =======================================================*/

#include "dh/core/claim/assert.h"
#if claim_fail_logging_comp_enabled
#include <stdarg.h> /* For va_list, va_start(), va_end() */
#include <stdio.h>  /* For fprintf(), stderr */
                    /* TODO: Remove printing based stdio and fprintf */

/*========== Extern Function Implementations ================================*/

$static fn_((claim_assert__output(void))(FILE*)) { return stderr; }

$static let_(claim_assert__fmt_default, char*) = "Assertion failed: %s, in function %s, at file %s, line %d\n";
$static let_(claim_assert__fmt_msg, char*) = "Assertion failed: %s\n";
$static let_(claim_assert__fmt_loc, char*) = "\nin function %s, at file %s, line %d\n";

fn_((claim_assert_failLog(const char* expr, const char* func, const char* file, i32 line))(void)) {
    let output = claim_assert__output();
    let_ignore = fprintf(
        output, claim_assert__fmt_default,
        expr, func, file, line
    );
}

fn_((claim_assert_failLogMsg(const char* expr, const char* func, const char* file, i32 line, const char* msg))(void)) {
    let output = claim_assert__output();
    let_ignore = fprintf(output, claim_assert__fmt_msg, expr);
    let_ignore = fprintf(output, "%s", msg);
    let_ignore = fprintf(
        output, claim_assert__fmt_loc,
        func, file, line
    );
}

fn_((claim_assert_failLogFmt(const char* expr, const char* func, const char* file, i32 line, const char* fmt, ...))(void)) {
    let output = claim_assert__output();
    let_ignore = fprintf(output, claim_assert__fmt_msg, expr);
    va_list args = {};
    va_start(args, fmt);
    let_ignore = vfprintf(
        output, fmt,
        args
    );
    va_end(args);
    let_ignore = fprintf(
        output, claim_assert__fmt_loc,
        func, file, line
    );
}
#endif /* claim_fail_logging_comp_enabled */

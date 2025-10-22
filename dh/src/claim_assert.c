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

#include "dh/claim/assert.h"
#include "dh/claim/cfg.h"
#include "dh/debug/common.h" /* For debug_break() */

#include <stdarg.h> /* For va_list, va_start(), va_end() */
#include <stdio.h>  /* For fprintf(), stderr */
#include <stdlib.h> /* For abort() */

/*========== Extern Function Implementations ================================*/

void claim_assert_fail(const char* expr, const char* func, const char* file, i32 line) {
    let_ignore = fprintf(
        CLAIM_OUTPUT,
        CLAIM_ASSERT_MSG_FORMAT,
        expr,
        func,
        file,
        line
    );

    debug_break();
    abort();
}

void claim_assert_fail_fmt(const char* expr, const char* func, const char* file, i32 line, const char* fmt, ...) {
    let_ignore = fprintf(
        CLAIM_OUTPUT,
        CLAIM_ASSERT_FMT_MSG_FORMAT,
        expr
    );

    va_list args = null;
    va_start(args, fmt);
    let_ignore = vfprintf(
        CLAIM_OUTPUT,
        fmt,
        args
    );
    va_end(args);

    let_ignore = fprintf(
        CLAIM_OUTPUT,
        CLAIM_ASSERT_FMT_LOC_FORMAT,
        func,
        file,
        line
    );

    debug_break();
    abort();
}

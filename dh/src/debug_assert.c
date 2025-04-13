/**
 * @copyright Copyright 2024-2025. Gyeongtae Kim All rights reserved.
 *
 * @file    debug_assert.c
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
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

#include "dh/debug/cfg.h"
#include "dh/debug/assert.h"

#include <stdarg.h> /* For va_list, va_start(), va_end() */
#include <stdio.h>  /* For fprintf(), stderr */
#include <stdlib.h> /* For abort() */

/*========== Extern Function Implementations ================================*/

void debug_assert_fail(const char* expr, const char* func, const char* file, i32 line) {
    $ignore fprintf(
        DEBUG_OUTPUT,
        DEBUG_ASSERT_MSG_FORMAT,
        expr,
        func,
        file,
        line
    );

    DEBUG_BREAK();
}

void debug_assert_fail_fmt(const char* expr, const char* func, const char* file, i32 line, const char* fmt, ...) {
    $ignore fprintf(
        DEBUG_OUTPUT,
        DEBUG_ASSERT_FMT_MSG_FORMAT,
        expr
    );

    va_list args = null;
    va_start(args, fmt);
    $ignore vfprintf(
        DEBUG_OUTPUT,
        fmt,
        args
    );
    va_end(args);

    $ignore fprintf(
        DEBUG_OUTPUT,
        DEBUG_ASSERT_FMT_LOC_FORMAT,
        func,
        file,
        line
    );

    DEBUG_BREAK();
}

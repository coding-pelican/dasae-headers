/**
 * @copyright Copyright 2024. Gyeongtae Kim All rights reserved.
 *
 * @file    debug_assert.c
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2024-11-02 (date of creation)
 * @updated 2024-11-02 (date of last update)
 * @version v1.0.0
 * @ingroup dasae-headers(dh)/debug
 * @prefix  debug
 *
 * @brief   Source of some software
 * @details Some detailed explanation
 */


/*========== Includes =======================================================*/

#include "dh/debug/assert.h" /* For `debug_break()` */
#include "dh/debug/common.h"

#include <stdarg.h> /* For `va_list`, `va_start()`, `va_end()` */
#include <stdio.h>  /* For `fprintf()`, `stderr` */

/*========== Extern Function Implementations ================================*/

void debug__assertFail(const char* expr, const char* func, const char* file, i32 line) {
    ignore fprintf(
        stderr,
        "Assertion failed: %s, in function %s, at file %s, line %d\n",
        expr,
        func,
        file,
        line
    );

    debug_break();
}

void debug__assertFailFmt(const char* expr, const char* func, const char* file, i32 line, const char* fmt, ...) {
    ignore fprintf(
        stderr,
        "Assertion failed: %s\n",
        expr
    );

    va_list args = null;
    va_start(args, fmt);
    ignore vfprintf(
        stderr,
        fmt,
        args
    );
    va_end(args);

    ignore fprintf(
        stderr,
        "\nin function %s, at file %s, line %d\n",
        func,
        file,
        line
    );

    debug_break();
}

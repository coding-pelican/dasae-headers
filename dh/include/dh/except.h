/**
 * @copyright Copyright 2024. Gyeongtae Kim All rights reserved.
 *
 * @file    except.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2024-11-28 (date of creation)
 * @updated 2024-11-28 (date of last update)
 * @version v1.0.0
 * @ingroup dasae-headers(dh)
 * @prefix  NONE
 *
 * @brief   Header of some software
 * @details Some detailed explanation
 */

#ifndef EXCEPT_INCLUDE
#define EXCEPT_INCLUDE (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

#include "core.h"

#include <setjmp.h>
#include <stdio.h>
#include <stdlib.h>

// Maximum number of nested try blocks
#define except_try_limit (32)

// Exception types
typedef enum ExceptionType {
    ExceptionType_no_exception = 0,
    ExceptionType_invalid_argument,
    ExceptionType_null_pointer,
    ExceptionType_out_of_memory,
    ExceptionType_divide_by_zero,
    ExceptionType_index_out_of_bounds
} ExceptionType;

// Exception structure
typedef struct Exception {
    ExceptionType type;
    const char*   message;
    const char*   file;
    i32           line;
} Exception;

// Thread-local storage for exception handling context
extern __thread struct ExceptionContext {
    jmp_buf   buf[except_try_limit];
    Exception exception;
    i32       try_index;
} exception_context;

// Macro definitions for try-catch
#define try                                                                      \
    do {                                                                         \
        if (exception_context.try_index >= except_try_limit) {                   \
            ignore fprintf(stderr, "Error: Maximum try block depth exceeded\n"); \
            exit(1);                                                             \
        }                                                                        \
        if (setjmp(exception_context.buf[exception_context.try_index++]) == 0)

#define catch(var_ex)                                   \
    else {                                              \
        Exception var_ex = exception_context.exception; \
        exception_context.try_index--;

#define tried                            \
    }                                    \
    if (exception_context.try_index > 0) \
        exception_context.try_index--;   \
    }                                    \
    while (false)

// Convenience macro for throwing exceptions
#define throw(type, message) throw_exception(type, message, __FILE__, __LINE__)

// Function to throw an exception
extern void throw_exception(ExceptionType type, const char* message, const char* file, i32 line);

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* EXCEPT_INCLUDE */

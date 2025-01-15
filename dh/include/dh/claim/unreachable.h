/**
 * @copyright Copyright 2024-2025. Gyeongtae Kim All rights reserved.
 *
 * @file    unreachable.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2024-10-28 (date of creation)
 * @updated 2025-01-15 (date of last update)
 * @version v0.1-alpha
 * @ingroup dasae-headers(dh)/claim
 * @prefix  claim_unreachable
 *
 * @brief   Unreachable code assertions
 * @details Provides macros for marking and asserting unreachable code paths
 */

#ifndef CLAIM_UNREACHABLE_INCLUDED
#define CLAIM_UNREACHABLE_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "dh/debug/assert.h"

#include <stdnoreturn.h>

/*========== Definitions ====================================================*/

// Compiler-specific macros for optimization hints
#if defined(__GNUC__) || defined(__clang__)
#define BUILTIN_UNREACHABLE __builtin_unreachable()
#elif defined(_MSC_VER)
#define BUILTIN_UNREACHABLE __assume(0)
#else
/* TODO: Add support for other compilers */
#define BUILTIN_UNREACHABLE unused(0)
#endif

#define claim_unreachable                                                                    \
    /**                                                                                      \
     * @brief Runtime unreachable - analogous to Rust's unreachable!() macro                 \
     *                                                                                       \
     * This should be used in situations where you know a code path is impossible,           \
     * but the compiler cannot prove it. Using this incorrectly leads to undefined behavior. \
     *                                                                                       \
     * In debug builds, this will trigger an assertion.                                      \
     * In release builds, this tells the compiler the code is unreachable for optimization.  \
     */                                                                                      \
    SYN__claim_unreachable

#define claim_unreachable_msg(_msg)                                                  \
    /**                                                                              \
     * @brief Runtime unreachable with message - provides more context for debugging \
     *                                                                               \
     * @param _msg The message to display if this is reached in debug mode           \
     */                                                                              \
    FUNC__claim_unreachable_msg(_msg)

#define claim_unreachable_fmt(_msg, _fmt...)                                                   \
    /**                                                                                        \
     * @brief Runtime unreachable with formatted message - provides more context for debugging \
     *                                                                                         \
     * @param _msg The message to display if this is reached in debug mode                     \
     * @param _fmt The format string and arguments for the message                             \
     */                                                                                        \
    FUNC__claim_unreachable_fmt(_msg, _fmt)

#define claim_unreachable_val(_T)                                                   \
    /**                                                                             \
     * @brief Function implementation of unreachable for use in expression contexts \
     *                                                                              \
     * This is useful when you need an expression rather than a statement.          \
     * Returns a value of the specified type, though it will never actually return. \
     *                                                                              \
     * @param _T The type to "return" (only used for type checking)                 \
     * @return Value of type _T (never actually returns)                            \
     */                                                                             \
    FUNC__claim_unreachable_val(_T)

/*========== Implementations ================================================*/

#define SYN__claim_unreachable \
    (debug_assert_fmt(false, "Reached unreachable code"), BUILTIN_UNREACHABLE)

#define FUNC__claim_unreachable_msg(_msg) \
    (debug_assert_fmt(false, _msg), BUILTIN_UNREACHABLE)

#define FUNC__claim_unreachable_fmt(_msg, _fmt...) \
    (debug_assert_fmt(false, _msg, _fmt), BUILTIN_UNREACHABLE)

#define FUNC__claim_unreachable_val(_T) \
    ((_T)(claim_unreachable, literal(_T, 0)))

/*========== Example Usage (Disabled to prevent compilation) ================*/

#if 0  /* Example Usage */
typedef enum Status {
    Status_OK,
    Status_ERROR
} Status;

int exampleFunction(Status status) {
    switch (status) {
    case Status_OK:
        return 1;
    case Status_ERROR:
        return 0;
    default:
        claim_unreachable;
    }
}

int unsafeDivide(int a, int b) {
    if (b == 0) {
        claim_unreachable_msg("Division by zero should never happen");
    }
    return a / b;
}

int getPositive(int x) {
    if (x <= 0) {
        return claim_unreachable_val(int); // Use in expression context
    }
    return x;
}
#endif /* Example Usage */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* CLAIM_UNREACHABLE_INCLUDED */

/**
 * @copyright Copyright 2024. Gyeongtae Kim All rights reserved.
 *
 * @file    unreachable.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2024-10-28 (date of creation)
 * @updated 2024-11-21 (date of last update)
 * @version v1.0.0
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

#include "dh/claim/assert.h"

#include <stdnoreturn.h>

/*========== Macros and Definitions =========================================*/

// Compiler-specific macros for optimization hints
#if defined(__GNUC__) || defined(__clang__)
#define CLAIM_UNREACHABLE_BUILTIN __builtin_unreachable()
#elif defined(_MSC_VER)
#define CLAIM_UNREACHABLE_BUILTIN __assume(0)
#else
/* TODO: Add support for other compilers */
#define CLAIM_UNREACHABLE_BUILTIN unused(0)
#endif

#define claim_unreachable()                                                                  \
    /**                                                                                      \
     * @brief Runtime unreachable - analogous to Rust's unreachable!() macro                 \
     *                                                                                       \
     * This should be used in situations where you know a code path is impossible,           \
     * but the compiler cannot prove it. Using this incorrectly leads to undefined behavior. \
     *                                                                                       \
     * In debug builds, this will trigger an assertion.                                      \
     * In release builds, this tells the compiler the code is unreachable for optimization.  \
     */                                                                                      \
    (claim_assert_fmt(false, "Reached unreachable code"), CLAIM_UNREACHABLE_BUILTIN)

#define claim_unreachable_static()                                                         \
    /**                                                                                    \
     * @brief Compile-time unreachable check - analogous to Zig's unreachable              \
     *                                                                                     \
     * This version is intended for use in compile-time constant evaluation contexts,      \
     * like switch statements where you know all cases are covered.                        \
     *                                                                                     \
     * @note This is safer than unreachable() as it will fail at compile time if reachable \
     */                                                                                    \
    do {                                                                                   \
        switch (0) {                                                                       \
        case 0:                                                                            \
        case 0:                                                                            \
        }                                                                                  \
        claim_unreachable();                                                               \
    } while (false)

#define claim_unreachable_msg(_msg)                                                  \
    /**                                                                              \
     * @brief Runtime unreachable with message - provides more context for debugging \
     *                                                                               \
     * @param _msg The message to display if this is reached in debug mode           \
     */                                                                              \
    (claim_assert_fmt(false, _msg), CLAIM_UNREACHABLE_BUILTIN)

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
    ((_T)(claim_unreachable(), literal(_T, 0)))

#define claim_unreachable_switch(_T, _val)                          \
    /**                                                             \
     * @brief Verify at compile-time that a switch covers all cases \
     *                                                              \
     * @param _T The type of the value                              \
     * @param _val The value being switched on                      \
     */                                                             \
    IMPL_claim_unreachable_switch(pp_uniqueToken(unreachable_tmp), _T, _val)

#define IMPL_claim_unreachable_switch(_unreachable_tmp, _T, _val) \
    do {                                                          \
        _T _unreachable_tmp = (_val);                             \
        unused(_unreachable_tmp);                                 \
        claim_unreachable_static();                               \
    } while (false)

/*========== Externalized Static Functions Prototypes (Unit Test) ===========*/

#ifdef UNIT_TEST
#endif /* UNIT_TEST */

/*========== Example Usage (Disabled to prevent compilation) ================*/

#if 0 /* Example Usage */

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
    }
    // Compiler knows all cases are handled
    claim_unreachable_switch(Status, status);
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

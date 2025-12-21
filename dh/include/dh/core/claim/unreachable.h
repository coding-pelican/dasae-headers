/**
 * @copyright Copyright (c) 2024-2025 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    unreachable.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2024-10-28 (date of creation)
 * @updated 2025-01-15 (date of last update)
 * @version v0.1-alpha
 * @ingroup dasae-headers(dh)/claim
 * @prefix  claim_unreachable
 *
 * @brief   Unreachable code assertions
 * @details Provides macros for marking and asserting unreachable code paths
 */
#ifndef claim_unreachable__included
#define claim_unreachable__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "dh/builtin/comp.h"
#include "../debug/assert.h"

/*========== Macros and Declarations ========================================*/

#define claim_unreachable __step__claim_unreachable
#define claim_unreachable_msg(_msg) __step__claim_unreachable_msg(_msg)
#define claim_unreachable_fmt(_fmt...) __step__claim_unreachable_fmt(_fmt)
#define claim_unreachable_val$(_T...) __step__claim_unreachable_val$(_T)

/*========== Macros and Definitions =========================================*/

#define __step__claim_unreachable (debug_assert_trap_msg("Reached unreachable code"), $unreachable)
#define __step__claim_unreachable_msg(_msg...) (debug_assert_trap_msg(_msg), $unreachable)
#define __step__claim_unreachable_fmt(_fmt...) (debug_assert_trap_fmt(_fmt), $unreachable)
#define __step__claim_unreachable_val$(_T...) (claim_unreachable, lit0$((_T)))

/*========== Example Usage (Disabled to prevent compilation) ================*/

#if EXAMPLE_USAGE
typedef enum Status {
    Status_OK,
    Status_ERROR,
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
    if (b == 0) claim_unreachable_msg("Division by zero should never happen");
    return a / b;
}

int getPositive(int x) {
    if (x <= 0) return claim_unreachable_val$(int); // Use in expression context
    return x;
}
#endif /* EXAMPLE_USAGE */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* claim_unreachable__included */

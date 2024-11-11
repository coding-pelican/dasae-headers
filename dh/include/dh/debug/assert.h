/**
 * @copyright Copyright 2024. Gyeongtae Kim All rights reserved.
 *
 * @file    assert.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2024-10-22 (date of creation)
 * @updated 2024-11-02 (date of last update)
 * @version v1.0.0
 * @ingroup dasae-headers(dh)/debug
 * @prefix  debug
 *
 * @brief   Header of some software
 * @details Some detailed explanation
 */


#ifndef DEBUG_ASSERT_INCLUDED
#define DEBUG_ASSERT_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "cfg.h" /* For `DEBUG_ENABLED` */
#include "../core/prim.h"
#include "../core/pp.h"

/*========== Macros and Definitions =========================================*/

#define debug_assert(EXPR)   \
    /* Assert expression. */ \
    IMPL_debug_assert(EXPR)
#define debug_assertFmt(EXPR, FMT...)               \
    /* Assert expression with formatted message. */ \
    IMPL_debug_assertFmt(EXPR, FMT)

#define debug_assertTrue(EXPR)            \
    /* Assert that expression is true. */ \
    IMPL_debug_assertTrue(EXPR)
#define debug_assertFalse(EXPR)            \
    /* Assert that expression is false. */ \
    IMPL_debug_assertFalse(EXPR)
#define debug_assertEq(EXPR1, EXPR2)             \
    /* Assert that two expressions are equal. */ \
    IMPL_debug_assertEq(EXPR1, EXPR2)
#define debug_assertNe(EXPR1, EXPR2)                 \
    /* Assert that two expressions are not equal. */ \
    IMPL_debug_assertNe(EXPR1, EXPR2)
#define debug_assertNull(EXPR)              \
    /* Assert that expressions is null.  */ \
    IMPL_debug_assertNull(EXPR)
#define debug_assertNotNull(EXPR)               \
    /* Assert that expressions is not null.  */ \
    IMPL_debug_assertNotNull(EXPR)

#define debug_assertTrueFmt(EXPR, FMT...)                        \
    /* Assert that expression is true with formatted message. */ \
    IMPL_debug_assertTrueFmt(EXPR, FMT)
#define debug_assertFalseFmt(EXPR, FMT...)                        \
    /* Assert that expression is false with formatted message. */ \
    IMPL_debug_assertFalseFmt(EXPR, FMT)
#define debug_assertEqFmt(EXPR1, EXPR2, FMT...)                         \
    /* Assert that two expressions are equal with formatted message. */ \
    IMPL_debug_assertEqFmt(EXPR1, EXPR2, FMT)
#define debug_assertNeFmt(EXPR1, EXPR2, FMT...)                             \
    /* Assert that two expressions are not equal with formatted message. */ \
    IMPL_debug_assertNeFmt(EXPR1, EXPR2, FMT)
#define debug_assertNullFmt(EXPR, FMT...)                         \
    /* Assert that expressions is null with formatted message. */ \
    IMPL_debug_assertNullFmt(EXPR, FMT)
#define debug_assertNotNullFmt(EXPR, FMT...)                          \
    /* Assert that expressions is not null with formatted message. */ \
    IMPL_debug_assertNotNullFmt(EXPR, FMT)

/*========== Macros Implementation ==========================================*/

#if defined(DEBUG_ENABLED) && DEBUG_ENABLED

#define IMPL_debug_assert(EXPR) \
    pp_ignore((!!(EXPR)) || (debug__assertFail(#EXPR, __func__, __FILE__, __LINE__), 0))

#define IMPL_debug_assertFmt(EXPR, FMT...) \
    pp_ignore((!!(EXPR)) || (debug__assertFailFmt(#EXPR, __func__, __FILE__, __LINE__, FMT), 0))

#define IMPL_debug_assertTrue(EXPR)       IMPL_debug_assertFmt((EXPR) == true, "%s is not true", #EXPR)
#define IMPL_debug_assertFalse(EXPR)      IMPL_debug_assertFmt((EXPR) == false, "%s is not false", #EXPR)
#define IMPL_debug_assertEq(EXPR1, EXPR2) IMPL_debug_assertFmt((EXPR1) == (EXPR2), "%s is not equal to %s", #EXPR1, #EXPR2)
#define IMPL_debug_assertNe(EXPR1, EXPR2) IMPL_debug_assertFmt((EXPR1) != (EXPR2), "%s is equal to %s", #EXPR1, #EXPR2)
#define IMPL_debug_assertNull(EXPR)       IMPL_debug_assertFmt((EXPR) == null, "%s is not null", #EXPR)
#define IMPL_debug_assertNotNull(EXPR)    IMPL_debug_assertFmt((EXPR) != null, "%s is null", #EXPR)

#define IMPL_debug_assertTrueFmt(EXPR, ...)       IMPL_debug_assertFmt((EXPR) == true, __VA_ARGS__)
#define IMPL_debug_assertFalseFmt(EXPR, ...)      IMPL_debug_assertFmt((EXPR) == false, __VA_ARGS__)
#define IMPL_debug_assertEqFmt(EXPR1, EXPR2, ...) IMPL_debug_assertFmt((EXPR1) == (EXPR2), __VA_ARGS__)
#define IMPL_debug_assertNeFmt(EXPR1, EXPR2, ...) IMPL_debug_assertFmt((EXPR1) != (EXPR2), __VA_ARGS__)
#define IMPL_debug_assertNullFmt(EXPR, ...)       IMPL_debug_assertFmt((EXPR) == null, __VA_ARGS__)
#define IMPL_debug_assertNotNullFmt(EXPR, ...)    IMPL_debug_assertFmt((EXPR) != null, __VA_ARGS__)

#else

#define IMPL_debug_assert(EXPR)         pp_unused(0)
#define IMPL_debug_assertFmt(EXPR, ...) pp_unused(0)

#define IMPL_debug_assertTrue(EXPR)       pp_unused(0)
#define IMPL_debug_assertFalse(EXPR)      pp_unused(0)
#define IMPL_debug_assertEq(EXPR1, EXPR2) pp_unused(0)
#define IMPL_debug_assertNe(EXPR1, EXPR2) pp_unused(0)
#define IMPL_debug_assertNull(EXPR)       pp_unused(0)
#define IMPL_debug_assertNotNull(EXPR)    pp_unused(0)

#define IMPL_debug_assertTrueFmt(EXPR, ...)       pp_unused(0)
#define IMPL_debug_assertFalseFmt(EXPR, ...)      pp_unused(0)
#define IMPL_debug_assertEqFmt(EXPR1, EXPR2, ...) pp_unused(0)
#define IMPL_debug_assertNeFmt(EXPR1, EXPR2, ...) pp_unused(0)
#define IMPL_debug_assertNullFmt(EXPR, ...)       pp_unused(0)
#define IMPL_debug_assertNotNullFmt(EXPR, ...)    pp_unused(0)

#endif /* defined(DEBUG_ENABLED) && DEBUG_ENABLED */

/*========== Extern Function Prototypes =====================================*/

extern void debug__assertFail(const char* expr, const char* func, const char* file, i32 line);
extern void debug__assertFailFmt(const char* expr, const char* func, const char* file, i32 line, const char* fmt, ...);


#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* DEBUG_ASSERT_INCLUDED */

/**
 * @copyright Copyright 2024. Gyeongtae Kim All rights reserved.
 *
 * @file    debug_debug_assert.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2024-10-22 (date of creation)
 * @updated 2024-10-22 (date of last update)
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


#include "debug.h" /* For `debug_break()` */


/* MACROS DECLARATION */
#define debug_assert(_expr)  \
    /* Assert expression. */ \
    RETURN_debug_assert(_expr)
#define debug_assertFmt(_expr, _fmt...)             \
    /* Assert expression with formatted message. */ \
    RETURN_debug_assertFmt(_expr, _fmt)


#define debug_assertTrue(_expr)           \
    /* Assert that expression is true. */ \
    RETURN_debug_assertTrue(_expr)
#define debug_assertFalse(_expr)           \
    /* Assert that expression is false. */ \
    RETURN_debug_assertFalse(_expr)
#define debug_assertEq(_expr1, _expr2)           \
    /* Assert that two expressions are equal. */ \
    RETURN_debug_assertEq(_expr1, _expr2)
#define debug_assertNe(_expr1, _expr2)               \
    /* Assert that two expressions are not equal. */ \
    RETURN_debug_assertNe(_expr1, _expr2)
#define debug_assertNull(_expr)             \
    /* Assert that expressions is null.  */ \
    RETURN_debug_assertNull(_expr)
#define debug_assertNotNull(_expr)              \
    /* Assert that expressions is not null.  */ \
    RETURN_debug_assertNotNull(_expr)


#define debug_assertTrueFmt(_expr, _fmt...)                      \
    /* Assert that expression is true with formatted message. */ \
    RETURN_debug_assertTrueFmt(_expr, _fmt)
#define debug_assertFalseFmt(_expr, _fmt...)                      \
    /* Assert that expression is false with formatted message. */ \
    RETURN_debug_assertFalseFmt(_expr, _fmt)
#define debug_assertEqFmt(_expr1, _expr2, _fmt...)                      \
    /* Assert that two expressions are equal with formatted message. */ \
    RETURN_debug_assertEqFmt(_expr1, _expr2, _fmt)
#define debug_assertNeFmt(_expr1, _expr2, _fmt...)                          \
    /* Assert that two expressions are not equal with formatted message. */ \
    RETURN_debug_assertNeFmt(_expr1, _expr2, _fmt)
#define debug_assertNullFmt(_expr, _fmt...)                       \
    /* Assert that expressions is null with formatted message. */ \
    RETURN_debug_assertNullFmt(_expr, _fmt)
#define debug_assertNotNullFmt(_expr, _fmt...)                        \
    /* Assert that expressions is not null with formatted message. */ \
    RETURN_debug_assertNotNullFmt(_expr, _fmt)

// TODO(dev-dasae, debug): Add Static Assertions


/* MACROS IMPLEMENTATION */
#if defined(DEBUG_ENABLED) && DEBUG_ENABLED

#include "../compat/compat_bool.h" /* For `true` and `false` */
#include "../compat/compat_null.h" /* For `null` */
#include "../preprocessor.h"       /* For `pp_func()` `pp_ignore` */
#include <stdio.h>                 /* For `fprintf()`, `stderr` */


#define RETURN_debug_assert(_expr) pp_func(                                \
    if (!(_expr)) {                                                        \
        pp_ignore fprintf(                                                 \
            stderr,                                                        \
            "Assertion failed: %s, in function %s, at file %s, line %d\n", \
            #_expr,                                                        \
            __func__,                                                      \
            __FILE__,                                                      \
            __LINE__                                                       \
        );                                                                 \
        debug_break();                                                     \
    }                                                                      \
)

#define RETURN_debug_assertFmt(_expr, ...) pp_func(  \
    if (!(_expr)) {                                  \
        pp_ignore fprintf(                           \
            stderr,                                  \
            "Assertion failed: %s\n",                \
            #_expr                                   \
        );                                           \
        pp_ignore fprintf(                           \
            stderr,                                  \
            __VA_ARGS__                              \
        );                                           \
        pp_ignore fprintf(                           \
            stderr,                                  \
            "in function %s, at file %s, line %d\n", \
            __func__,                                \
            __FILE__,                                \
            __LINE__                                 \
        );                                           \
        debug_break();                               \
    }                                                \
)

#define RETURN_debug_assertTrue(_expr)        RETURN_debug_assertFmt((_expr) == true, "%s is not true", #_expr)
#define RETURN_debug_assertFalse(_expr)       RETURN_debug_assertFmt((_expr) == false, "%s is not false", #_expr)
#define RETURN_debug_assertEq(_expr1, _expr2) RETURN_debug_assertFmt((_expr1) == (_expr2), "%s is not equal to %s", #_expr1, #_expr2)
#define RETURN_debug_assertNe(_expr1, _expr2) RETURN_debug_assertFmt((_expr1) != (_expr2), "%s is equal to %s", #_expr1, #_expr2)
#define RETURN_debug_assertNull(_expr)        RETURN_debug_assertFmt((_expr) == null, "%s is not null", #_expr)
#define RETURN_debug_assertNotNull(_expr)     RETURN_debug_assertFmt((_expr) != null, "%s is null", #_expr)

#define RETURN_debug_assertTrueFmt(_expr, ...)        RETURN_debug_assertFmt((_expr) == true, __VA_ARGS__)
#define RETURN_debug_assertFalseFmt(_expr, ...)       RETURN_debug_assertFmt((_expr) == false, __VA_ARGS__)
#define RETURN_debug_assertEqFmt(_expr1, _expr2, ...) RETURN_debug_assertFmt((_expr1) == (_expr2), __VA_ARGS__)
#define RETURN_debug_assertNeFmt(_expr1, _expr2, ...) RETURN_debug_assertFmt((_expr1) != (_expr2), __VA_ARGS__)
#define RETURN_debug_assertNullFmt(_expr, ...)        RETURN_debug_assertFmt((_expr) == null, __VA_ARGS__)
#define RETURN_debug_assertNotNullFmt(_expr, ...)     RETURN_debug_assertFmt((_expr) != null, __VA_ARGS__)

#else

#include "../preprocessor.h" /* For `pp_unused()` */


#define RETURN_debug_assert(_expr)         pp_unused(0)
#define RETURN_debug_assertFmt(_expr, ...) pp_unused(0)

#define RETURN_debug_assertTrue(_expr)        pp_unused(0)
#define RETURN_debug_assertFalse(_expr)       pp_unused(0)
#define RETURN_debug_assertEq(_expr1, _expr2) pp_unused(0)
#define RETURN_debug_assertNe(_expr1, _expr2) pp_unused(0)
#define RETURN_debug_assertNull(_expr)        pp_unused(0)
#define RETURN_debug_assertNotNull(_expr)     pp_unused(0)

#define RETURN_debug_assertTrueFmt(_expr, ...)        pp_unused(0)
#define RETURN_debug_assertFalseFmt(_expr, ...)       pp_unused(0)
#define RETURN_debug_assertEqFmt(_expr1, _expr2, ...) pp_unused(0)
#define RETURN_debug_assertNeFmt(_expr1, _expr2, ...) pp_unused(0)
#define RETURN_debug_assertNullFmt(_expr, ...)        pp_unused(0)
#define RETURN_debug_assertNotNullFmt(_expr, ...)     pp_unused(0)

#endif /* defined(DEBUG_ENABLED) && DEBUG_ENABLED */


#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* DEBUG_ASSERT_INCLUDED */

/**
 * @copyright Copyright 2024. Gyeongtae Kim All rights reserved.
 *
 * @file    assert.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2024-10-22 (date of creation)
 * @updated 2024-11-02 (date of last update)
 * @version v1.0.0
 * @ingroup dasae-headers(dh)/debug
 * @prefix  debug_assert
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
#include "dh/core/prim.h"
#include "dh/core/pp.h"

/*========== Macros and Definitions =========================================*/

#define debug_assert(_Expr)  \
    /* Assert expression. */ \
    IMPL_debug_assert(_Expr)
#define debug_assertFmt(_Expr, _fmt...)             \
    /* Assert expression with formatted message. */ \
    IMPL_debug_assertFmt(_Expr, _fmt)

#define debug_assertTrue(_Expr)           \
    /* Assert that expression is true. */ \
    IMPL_debug_assertTrue(_Expr)
#define debug_assertFalse(_Expr)           \
    /* Assert that expression is false. */ \
    IMPL_debug_assertFalse(_Expr)
#define debug_assertEq(_Expr1, _Expr2)           \
    /* Assert that two expressions are equal. */ \
    IMPL_debug_assertEq(_Expr1, _Expr2)
#define debug_assertNe(_Expr1, _Expr2)               \
    /* Assert that two expressions are not equal. */ \
    IMPL_debug_assertNe(_Expr1, _Expr2)
#define debug_assertNull(_Expr)             \
    /* Assert that expressions is null.  */ \
    IMPL_debug_assertNull(_Expr)
#define debug_assertNonNull(_Expr)              \
    /* Assert that expressions is non null.  */ \
    IMPL_debug_assertNonNull(_Expr)

#define debug_assertTrueFmt(_Expr, _fmt...)                      \
    /* Assert that expression is true with formatted message. */ \
    IMPL_debug_assertTrueFmt(_Expr, _fmt)
#define debug_assertFalseFmt(_Expr, _fmt...)                      \
    /* Assert that expression is false with formatted message. */ \
    IMPL_debug_assertFalseFmt(_Expr, _fmt)
#define debug_assertEqFmt(_Expr1, _Expr2, _fmt...)                      \
    /* Assert that two expressions are equal with formatted message. */ \
    IMPL_debug_assertEqFmt(_Expr1, _Expr2, _fmt)
#define debug_assertNeFmt(_Expr1, _Expr2, _fmt...)                          \
    /* Assert that two expressions are not equal with formatted message. */ \
    IMPL_debug_assertNeFmt(_Expr1, _Expr2, _fmt)
#define debug_assertNullFmt(_Expr, _fmt...)                       \
    /* Assert that expressions is null with formatted message. */ \
    IMPL_debug_assertNullFmt(_Expr, _fmt)
#define debug_assertNonNullFmt(_Expr, _fmt...)                        \
    /* Assert that expressions is non null with formatted message. */ \
    IMPL_debug_assertNonNullFmt(_Expr, _fmt)

/*========== Macros Implementation ==========================================*/

#if defined(DEBUG_ENABLED) && DEBUG_ENABLED

#define IMPL_debug_assert(_Expr) \
    ignore((!!(_Expr)) || (debug__assertFail(#_Expr, __func__, __FILE__, __LINE__), 0))

#define IMPL_debug_assertFmt(_Expr, ...) \
    ignore((!!(_Expr)) || (debug__assertFailFmt(#_Expr, __func__, __FILE__, __LINE__, __VA_ARGS__), 0))

#define IMPL_debug_assertTrue(_Expr)        IMPL_debug_assertFmt((_Expr) == true, "%s is not true", #_Expr)
#define IMPL_debug_assertFalse(_Expr)       IMPL_debug_assertFmt((_Expr) == false, "%s is not false", #_Expr)
#define IMPL_debug_assertEq(_Expr1, _Expr2) IMPL_debug_assertFmt((_Expr1) == (_Expr2), "%s is not equal to %s", #_Expr1, #_Expr2)
#define IMPL_debug_assertNe(_Expr1, _Expr2) IMPL_debug_assertFmt((_Expr1) != (_Expr2), "%s is equal to %s", #_Expr1, #_Expr2)
#define IMPL_debug_assertNull(_Expr)        IMPL_debug_assertFmt((_Expr) == null, "%s is non null", #_Expr)
#define IMPL_debug_assertNonNull(_Expr)     IMPL_debug_assertFmt((_Expr) != null, "%s is null", #_Expr)

#define IMPL_debug_assertTrueFmt(_Expr, ...)        IMPL_debug_assertFmt((_Expr) == true, __VA_ARGS__)
#define IMPL_debug_assertFalseFmt(_Expr, ...)       IMPL_debug_assertFmt((_Expr) == false, __VA_ARGS__)
#define IMPL_debug_assertEqFmt(_Expr1, _Expr2, ...) IMPL_debug_assertFmt((_Expr1) == (_Expr2), __VA_ARGS__)
#define IMPL_debug_assertNeFmt(_Expr1, _Expr2, ...) IMPL_debug_assertFmt((_Expr1) != (_Expr2), __VA_ARGS__)
#define IMPL_debug_assertNullFmt(_Expr, ...)        IMPL_debug_assertFmt((_Expr) == null, __VA_ARGS__)
#define IMPL_debug_assertNonNullFmt(_Expr, ...)     IMPL_debug_assertFmt((_Expr) != null, __VA_ARGS__)

#else

#define IMPL_debug_assert(_Expr)         unused(0)
#define IMPL_debug_assertFmt(_Expr, ...) unused(0)

#define IMPL_debug_assertTrue(_Expr)        unused(0)
#define IMPL_debug_assertFalse(_Expr)       unused(0)
#define IMPL_debug_assertEq(_Expr1, _Expr2) unused(0)
#define IMPL_debug_assertNe(_Expr1, _Expr2) unused(0)
#define IMPL_debug_assertNull(_Expr)        unused(0)
#define IMPL_debug_assertNonNull(_Expr)     unused(0)

#define IMPL_debug_assertTrueFmt(_Expr, ...)        unused(0)
#define IMPL_debug_assertFalseFmt(_Expr, ...)       unused(0)
#define IMPL_debug_assertEqFmt(_Expr1, _Expr2, ...) unused(0)
#define IMPL_debug_assertNeFmt(_Expr1, _Expr2, ...) unused(0)
#define IMPL_debug_assertNullFmt(_Expr, ...)        unused(0)
#define IMPL_debug_assertNonNullFmt(_Expr, ...)     unused(0)

#endif /* defined(DEBUG_ENABLED) && DEBUG_ENABLED */

/*========== Extern Function Prototypes =====================================*/

extern void debug__assertFail(const char* expr, const char* func, const char* file, i32 line);
extern void debug__assertFailFmt(const char* expr, const char* func, const char* file, i32 line, const char* fmt, ...);


#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* DEBUG_ASSERT_INCLUDED */

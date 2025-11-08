/**
 * @copyright Copyright (c) 2024-2025 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    static_assert.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2024-10-28 (date of creation)
 * @updated 2025-02-01 (date of last update)
 * @version v0.1-alpha.1
 * @ingroup dasae-headers(dh)/builtin
 * @prefix  static_assert
 *
 * @brief   Static assertion utilities
 * @details Provides utilities for compile-time assertions
 */

#ifndef builtin_static_assert__included
#define builtin_static_assert__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Macros and Declarations ========================================*/

#define static_assert(_Expr)           __step__static_assert(_Expr)
#define static_assert_msg(_Expr, _msg) __step__static_assert_msg(_Expr, _msg)

/*========== Macros and Definitions =========================================*/

#if defined(__cplusplus) && (__cplusplus >= 201103L)
/* C++11 or later - static_assert is available */
#define __step__static_assert(_Expr)           static_assert((_Expr), "Failed assertion")
#define __step__static_assert_msg(_Expr, _msg) static_assert((_Expr), "Failed assertion: " _msg)
#elif defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 201112L)
/* C11 or later - _Static_assert is available */
#define __step__static_assert(_Expr)           _Static_assert((_Expr), "Failed assertion")
#define __step__static_assert_msg(_Expr, _msg) _Static_assert((_Expr), "Failed assertion: " _msg)
#else
/* Older versions - emulate static assert */
#include "pp.h"
#ifdef __COUNTER__
#define __step__static_assert(_Expr) \
    typedef int pp_join(__, __step__static_assert, __COUNTER__)[(_Expr) ? 1 : -1]
#define __step__static_assert_msg(_Expr, _msg) \
    typedef int pp_join(__, __step__static_assert_msg, __COUNTER__)[(_Expr) ? 1 : -1]
#else
#define __step__static_assert(_Expr) \
    typedef int pp_join(__, __step__static_assert, __LINE__)[(_Expr) ? 1 : -1]
#define __step__static_assert_msg(_Expr, _msg) \
    typedef int pp_join(__, __step__static_assert_msg, __LINE__)[(_Expr) ? 1 : -1]
#endif
#endif

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* builtin_static_assert__included */

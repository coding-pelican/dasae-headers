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
#pragma once
#ifndef builtin_static_assert__included
#define builtin_static_assert__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "pp.h"

/*========== Macros and Declarations ========================================*/

#define static_assert(_$Expr) __step__static_assert(_$Expr)
#define static_assert_trap() __step__static_assert_trap()
#define static_assert_msg(_$Expr, _$msg) __step__static_assert_msg(_$Expr, _$msg)
#define static_assert_trap_msg(_$msg) __step__static_assert_trap_msg(_$msg)

/*========== Macros and Definitions =========================================*/

#if defined(__cplusplus) && (__cplusplus >= 201103L)
/* C++11 or later - static_assert is available */
#define __step__static_assert(_$Expr) static_assert((_$Expr), "Failed assertion")
#define __step__static_assert_msg(_$Expr, _$msg) static_assert((_$Expr), "Failed assertion: " _$msg)
#define __step__static_assert_trap() static_assert(false, "Failed assertion")
#define __step__static_assert_trap_msg(_$msg) static_assert(false, "Failed assertion: " _$msg)
#elif defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 201112L)
/* C11 or later - _Static_assert is available */
#define __step__static_assert(_$Expr) _Static_assert((_$Expr), "Failed assertion")
#define __step__static_assert_msg(_$Expr, _$msg) _Static_assert((_$Expr), "Failed assertion: " _$msg)
#define __step__static_assert_trap() _Static_assert(false, "Failed assertion")
#define __step__static_assert_trap_msg(_$msg) _Static_assert(false, "Failed assertion: " _$msg)
#else
/* Older versions - emulate static assert */
#ifdef __COUNTER__
#define __step__static_assert(_$Expr) typedef int pp_join(__, __step__static_assert, __COUNTER__)[(_$Expr) ? 1 : -1]
#define __step__static_assert_msg(_$Expr, _$msg) typedef int pp_join(__, __step__static_assert_msg, __COUNTER__)[(_$Expr) ? 1 : -1]
#define __step__static_assert_trap() typedef int pp_join(__, __step__static_assert_trap, __COUNTER__)[0]
#define __step__static_assert_trap_msg(_$msg) typedef int pp_join(__, __step__static_assert_trap_msg, __COUNTER__)[0]
#else
#define __step__static_assert(_$Expr) typedef int pp_join(__, __step__static_assert, __LINE__)[(_$Expr) ? 1 : -1]
#define __step__static_assert_msg(_$Expr, _$msg) typedef int pp_join(__, __step__static_assert_msg, __LINE__)[(_$Expr) ? 1 : -1]
#define __step__static_assert_trap() typedef int pp_join(__, __step__static_assert_trap, __LINE__)[0]
#define __step__static_assert_trap_msg(_$msg) typedef int pp_join(__, __step__static_assert_trap_msg, __LINE__)[0]
#endif
#endif

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* builtin_static_assert__included */

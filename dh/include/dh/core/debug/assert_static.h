/**
 * @copyright Copyright (c) 2024-2025 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    assert_static.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2024-10-28 (date of creation)
 * @updated 2025-02-01 (date of last update)
 * @version v0.1-alpha.1
 * @ingroup dasae-headers(dh)/core/debug
 * @prefix  debug_assert_static
 *
 * @brief   Static assertion utilities
 * @details Provides assertion utilities for compile-time assertions
 */
#pragma once
#ifndef core_debug_assert_static__included
#define core_debug_assert_static__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "cfg.h"
#include "dh/builtin/static_assert.h"

/*========== Macros and Declarations ========================================*/

#if debug_enabled
#define debug_assert_static(_$Expr) static_assert(_$Expr)
#define debug_assert_static_trap() static_assert_trap()
#define debug_assert_static_msg(_$Expr, _$msg) static_assert_msg(_$Expr, _$msg)
#define debug_assert_static_trap_msg(_$msg) static_assert_trap_msg(_$msg)
#else /* !debug_enabled */
#define debug_assert_static(_$Expr)
#define debug_assert_static_trap()
#define debug_assert_static_msg(_$Expr, _$msg)
#define debug_assert_static_trap_msg(_$msg)
#endif /* debug_enabled */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* core_debug_assert_static__included */

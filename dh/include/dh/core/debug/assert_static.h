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
#ifndef core_debug_assert_static__included
#define core_debug_assert_static__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "cfg.h"
#include "dh/builtin/static_assert.h"

/*========== Macros and Declarations ========================================*/

#if debug_comp_enabled
#define debug_assert_static(_Expr) __step__static_assert(_Expr)
#define debug_assert_static_msg(_Expr, _msg) __step__static_assert_msg(_Expr, _msg)
#else /* !debug_comp_enabled */
#define debug_assert_static(_Expr)
#define debug_assert_static_msg(_Expr, _msg)
#endif /* debug_comp_enabled */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* core_debug_assert_static__included */

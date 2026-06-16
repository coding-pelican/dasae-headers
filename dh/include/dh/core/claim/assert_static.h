/**
 * @copyright Copyright (c) 2024-2026 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    assert_static.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2024-10-28 (date of creation)
 * @updated 2026-06-16 (date of last update)
 * @ingroup dasae-headers(dh)/core/claim
 * @prefix  claim_assert_static
 *
 * @brief   Static assertion utilities
 * @details Provides assertion utilities for compile-time assertions
 */
#pragma once
#ifndef core_claim_assert_static__included
#define core_claim_assert_static__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "dh/builtin/static_assert.h"

/*========== Macros and Declarations ========================================*/

#define claim_assert_static(_Expr) __step__static_assert(_Expr)
#define claim_assert_static_trap() __step__static_assert_trap()
#define claim_assert_static_msg(_Expr, _msg) __step__static_assert_msg(_Expr, _msg)
#define claim_assert_static_trap_msg(_msg) __step__static_assert_trap_msg(_msg)

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* core_claim_assert_static__included */

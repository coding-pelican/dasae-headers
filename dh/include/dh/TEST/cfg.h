/**
 * @copyright Copyright (c) 2025-2026 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    cfg.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2025-03-26 (date of creation)
 * @updated 2026-06-03 (date of last update)
 * @ingroup dasae-headers(dh)/TEST
 * @prefix  TEST
 *
 * @brief   Test framework configuration
 * @details This header provides a testing framework configuration.
 */
#pragma once
#ifndef TEST_cfg__included
#define TEST_cfg__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "dh/builtin/pp.h"

/*========== Macros and Declarations ========================================*/

#if !defined(TEST_enabled)
#define TEST_enabled __comp_bool__TEST_enabled
#endif /* !defined(TEST_enabled) */
#define TEST__enabled_default __comp_flag__TEST__enabled_default

#define TEST_only(_$inner...) __comp_syn__TEST_only(_$inner)
#define TEST_unless(_$inner...) __comp_syn__TEST_unless(_$inner)

/*========== Macros and Definitions =========================================*/

/* Default values */

/* TODO: Add edittime decision */
// #define __comp_flag__TEST__enabled_default pp_not(in_comptime)
#define __comp_bool__TEST_enabled TEST__enabled_default
#define __comp_flag__TEST__enabled_default pp_false

/* Override values */

#if defined(COMP_TEST)
#undef __comp_flag__TEST__enabled_default
#define __comp_flag__TEST__enabled_default pp_true
#endif /* defined(COMP_TEST) */

#define __comp_syn__TEST_only(_$inner...) pp_if_(TEST_enabled)(pp_then_(_$inner))
#define __comp_syn__TEST_unless(_$inner...) pp_if_(pp_not(TEST_enabled))(pp_then_(_$inner))

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* TEST_cfg__included */

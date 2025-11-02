/**
 * @copyright Copyright (c) 2025 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    cfg.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2025-03-26 (date of creation)
 * @updated 2025-03-26 (date of last update)
 * @version v0.1-alpha
 * @ingroup dasae-headers(dh)/TEST
 * @prefix  TEST
 *
 * @brief   Test framework configuration
 * @details This header provides a testing framework configuration.
 */

#ifndef TEST_cfg__included
#define TEST_cfg__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Macros and Declarations ========================================*/

#define TEST__comp_enabled_default __comp_flag__TEST__comp_enabled_default
#define TEST_comp_enabled          __comp_bool__TEST_comp_enabled

/*========== Macros and Definitions =========================================*/

/* Default values */

/* TODO: Add edittime decision */
// #define __comp_flag__TEST__comp_enabled_default pp_not_(on_comptime)
#define __comp_flag__TEST__comp_enabled_default 0
#define __comp_bool__TEST_comp_enabled          TEST__comp_enabled_default

/* Override values */

#if defined(COMP_TEST)
#undef __comp_flag__TEST__comp_enabled_default
#define __comp_flag__TEST__comp_enabled_default 1
#endif /* defined(COMP_TEST) */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* TEST_cfg__included */

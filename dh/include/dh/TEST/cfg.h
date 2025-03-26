/**
 * @copyright Copyright 2025. Gyeongtae Kim All rights reserved.
 *
 * @file    cfg.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2025-03-26 (date of creation)
 * @updated 2025-03-26 (date of last update)
 * @version v0.1-alpha
 * @ingroup dasae-headers(dh)/TEST
 * @prefix  TEST
 *
 * @brief   Test framework configuration
 * @details This header provides a testing framework configuration.
 */

#ifndef TEST_CFG_INCLUDED
#define TEST_CFG_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Macros and Declarations ========================================*/

#define TEST_comp_enabled comp_flag__TEST_comp_enabled

/*========== Macros and Definitions =========================================*/

#if defined(COMP_TEST)
#define comp_flag__TEST_comp_enabled (1)
#else /* !defined(COMP_TEST) */
#define comp_flag__TEST_comp_enabled (0)
#endif /* !defined(COMP_TEST) */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* TEST_CFG_INCLUDED */

/**
 * @copyright Copyright (c) 2024-2025 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    cfg.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2024-11-21 (date of creation)
 * @updated 2024-11-21 (date of last update)
 * @version v1.0.0
 * @ingroup dasae-headers(dh)/core/claim
 * @prefix  claim
 *
 * @brief   Configuration synthesis for claim assertions
 * @details Combines default and custom configurations for claim assertions
 */

#ifndef core_claim_cfg__included
#define core_claim_cfg__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Macros and Declarations ========================================*/

#define claim__fail_logging_default_enabled __comp_flag__claim__fail_logging_default_enabled
#define claim_fail_logging_comp_enabled     __comp_bool__claim_fail_logging_comp_enabled

/*========== Macros and Definitions =========================================*/

/* Default values */

#define __comp_flag__claim__fail_logging_default_enabled 1
#define __comp_bool__claim_fail_logging_comp_enabled     claim__fail_logging_default_enabled

/* Override values */

// #if defined(NDEBUG) && defined(NCLAIM_FAIL_LOGGING)
// #undef __comp_flag__claim__fail_logging_default_enabled
// #define __comp_flag__claim__fail_logging_default_enabled 0
// #endif /* defined(NDEBUG) && defined(NCLAIM_FAIL_LOGGING) */

#if defined(NDEBUG)
#undef __comp_flag__claim__fail_logging_default_enabled
#define __comp_flag__claim__fail_logging_default_enabled 0
#endif /* defined(NDEBUG) */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* core_claim_cfg__included */

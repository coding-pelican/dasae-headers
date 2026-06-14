/**
 * @copyright Copyright (c) 2024-2026 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    cfg.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2024-11-21 (date of creation)
 * @updated 2026-06-03 (date of last update)
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

/*========== Includes =======================================================*/

#include "dh/builtin/pp.h"
#include "../debug/cfg.h"

/*========== Macros and Declarations ========================================*/

#if !defined(claim_fail_printing_enabled)
#define claim_fail_printing_enabled __comp_bool__claim_fail_printing_enabled
#endif /* !defined(claim_fail_printing_enabled) */
#define claim_fail_printing__enabled_default __comp_flag__claim_fail_printing__enabled_default

/*========== Macros and Definitions =========================================*/

/* Default values */

#define __comp_bool__claim_fail_printing_enabled claim_fail_printing__enabled_default
#define __comp_flag__claim_fail_printing__enabled_default debug_enabled

/* Override values */

// #if defined(NDEBUG) && defined(NCLAIM_FAIL_LOGGING)
// #undef __comp_flag__claim_fail_printing__enabled_default
// #define __comp_flag__claim_fail_printing__enabled_default 0
// #endif /* defined(NDEBUG) && defined(NCLAIM_FAIL_LOGGING) */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* core_claim_cfg__included */

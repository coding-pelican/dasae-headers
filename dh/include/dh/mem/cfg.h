/**
 * @copyright Copyright (c) 2024-2026 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    cfg.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2024-11-06 (date of creation)
 * @updated 2026-02-26 (date of last update)
 * @ingroup dasae-headers(dh)/mem
 * @prefix  mem
 *
 * @brief   Memory configuration settings
 * @details Defines compile-time options and configuration parameters
 *          for memory-related operations and utilities.
 */
#ifndef mem_cfg__included
#define mem_cfg__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "dh/prl.h"

/*========== Macros and Declarations ========================================*/

#if !defined(mem_tracing_comp_enabled)
#define mem_tracing_comp_enabled __comp_bool__mem_tracing_comp_enabled
#endif /* !defined(mem_tracing_comp_enabled) */
#define mem_tracing__comp_enabled_default __comp_flag__mem_tracing__comp_enabled_default

errset_((mem_E)(OutOfMemory));

/*========== Macros and Definitions =========================================*/

#define __comp_bool__mem_tracing_comp_enabled \
    mem_tracing__comp_enabled_default
#define __comp_flag__mem_tracing__comp_enabled_default \
    debug_comp_enabled

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* mem_cfg__included */

/**
 * @copyright Copyright (c) 2024 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    cfg.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2024-11-06 (date of creation)
 * @updated 2024-12-29 (date of last update)
 * @version v0.1-alpha
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

/*========== Macros and Definitions =========================================*/

#if !defined(NMEM_TRACE)
#define MEM_TRACE_ENABLED (1)
#else
#define MEM_TRACE_ENABLED (0)
#endif

#define mem_trace_enabled (MEM_TRACE_ENABLED)

errset_((mem_Err)(OutOfMemory));

#if defined(__cplusplus)
}
#endif /* defined(__cplusplus) */
#endif /* mem_cfg__included */

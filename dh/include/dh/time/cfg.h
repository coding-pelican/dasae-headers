/**
 * @copyright Copyright (c) 2024-2025 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    cfg.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2024-11-10 (date of creation)
 * @updated 2025-02-13 (date of last update)
 * @version v0.1-alpha.1
 * @ingroup dasae-headers(dh)/time
 * @prefix  time
 *
 * @brief   Configuration settings for time management
 * @details Defines compile-time options and configuration parameters
 *          for time-related operations and utilities.
 */
#ifndef time_cfg__included
#define time_cfg__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "dh/prl.h"
#if plat_is_windows
#include "dh/os/windows/common.h"
#else /* posix */
#include <sys/time.h>
#endif
#include <time.h>

/*========== Macros and Declarations ========================================*/

typedef pp_if_(plat_is_windows)(
    pp_then_(LARGE_INTEGER),
    pp_else_(struct timespec)
) time_SysTime_Impl;
typedef time_SysTime_Impl time_SysTimePlatform;

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* time_cfg__included */

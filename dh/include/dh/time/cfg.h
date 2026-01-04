/**
 * @copyright Copyright (c) 2024-2026 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    cfg.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2024-11-10 (date of creation)
 * @updated 2026-01-03 (date of last update)
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
#include "dh/os/windows/sysinfo.h"
#endif /* plat_is_windows */
#if plat_based_unix
#include <sys/time.h>
#endif /* plat_based_unix */

/*========== Macros and Declarations ========================================*/

/* --- Monotonic Clock Platform Type (for time_Instant) --- */
typedef pp_if_(plat_is_windows)(
    pp_then_(LARGE_INTEGER),
    pp_else_(pp_if_(plat_based_unix)(
        pp_then_(struct timespec),
        pp_else_(Void)
    ))) time_Instant__Impl;
typedef time_Instant__Impl time_InstantPlatform;

/* --- Wall-Clock Platform Type (for time_SysTime) --- */
typedef pp_if_(plat_is_windows)(
    pp_then_(FILETIME),
    pp_else_(pp_if_(plat_based_unix)(
        pp_then_(struct timespec),
        pp_else_(Void)
    ))) time_SysTime__Impl;
typedef time_SysTime__Impl time_SysTimePlatform;

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* time_cfg__included */

/**
 * @copyright Copyright 2024-2025. Gyeongtae Kim All rights reserved.
 *
 * @file    cfg.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2024-11-10 (date of creation)
 * @updated 2025-02-13 (date of last update)
 * @version v0.1-alpha.1
 * @ingroup dasae-headers(dh)/time
 * @prefix  time
 *
 * @brief   Source of some software
 * @details Some detailed explanation
 */

#ifndef TIME_CFG_INCLUDED
#define TIME_CFG_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "dh/core.h"
#include "dh/opt.h"

#include <time.h>

#if bti_plat_windows
#include "dh/os/windows/common.h"
#else /* posix */
#include <sys/time.h>
#endif /* posix */

/*========== Macros and Definitions =========================================*/

#if bti_plat_windows
typedef LARGE_INTEGER       time_SysTimeWindows;
typedef time_SysTimeWindows time_SysTimePlatform;
#else  /* posix */
typedef struct timespec  time_SysTimeUnix;
typedef time_SysTimeUnix time_SysTimePlatform;
#endif /* posix */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* TIME_CFG_INCLUDED */

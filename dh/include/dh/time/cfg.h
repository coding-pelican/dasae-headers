/**
 * @copyright Copyright 2024. Gyeongtae Kim All rights reserved.
 *
 * @file    cfg.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2024-11-10 (date of creation)
 * @updated 2024-11-15 (date of last update)
 * @version v1.0.0
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

#if defined(_WIN32) || defined(_WIN64)
/* Windows */
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#else
/* Unix */
#include <sys/time.h>
#endif
#include <time.h>

/*========== Macros and Definitions =========================================*/

#if defined(_WIN32) || defined(_WIN64)
typedef LARGE_INTEGER       time_SysTimeWindows;
typedef time_SysTimeWindows time_SysTimePlatform;
#else
typedef struct timespec  time_SysTimeUnix;
typedef time_SysTimeUnix time_SysTimePlatform;
#endif

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* TIME_CFG_INCLUDED */

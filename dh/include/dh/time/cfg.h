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
#include <time.h>
#endif

/*========== Macros and Definitions =========================================*/

typedef struct time_Duration time_Duration;
typedef struct time_Instant  time_Instant;

#if defined(_WIN32) || defined(_WIN64)
typedef LARGE_INTEGER          time_SystemTimeWindows;
typedef time_SystemTimeWindows time_SystemTime;
#else
typedef struct timespec     time_SystemTimeUnix;
typedef time_SystemTimeUnix time_SystemTime;
#endif


static const u32 time_nanos_per_sec   = 1000u * 1000u * 1000u;
static const u32 time_nanos_per_milli = 1000u * 1000u;
static const u32 time_nanos_per_micro = 1000u;
static const u32 time_millis_per_sec  = 1000u;
static const u32 time_micros_per_sec  = 1000u * 1000u;

static const u64 time_secs_per_minute = 60ull;
static const u64 time_mins_per_hour   = 60ull;
static const u64 time_hours_per_day   = 24ull;
static const u64 time_days_per_week   = 7ull;


#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* TIME_CFG_INCLUDED */

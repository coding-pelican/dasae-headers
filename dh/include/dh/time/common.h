/**
 * @copyright Copyright 2024. Gyeongtae Kim All rights reserved.
 *
 * @file    common.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2024-11-16 (date of creation)
 * @updated 2024-11-16 (date of last update)
 * @version v1.0.0
 * @ingroup dasae-headers(dh)/time
 * @prefix  time
 *
 * @brief   Source of some software
 * @details Some detailed explanation
 */


#ifndef TIME_COMMON_INCLUDED
#define TIME_COMMON_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */


/*========== Includes =======================================================*/

#include "cfg.h"

/*========== Macros and Definitions =========================================*/

typedef struct time_Duration time_Duration;
typedef struct time_Instant  time_Instant;
typedef time_SysTimePlatform time_SysTime;

static const u32 time_NANOS_PER_SEC   = 1000u * 1000u * 1000u;
static const u32 time_NANOS_PER_MILLI = 1000u * 1000u;
static const u32 time_NANOS_PER_MICRO = 1000u;
static const u32 time_MILLIS_PER_SEC  = 1000u;
static const u32 time_MICROS_PER_SEC  = 1000u * 1000u;

static const u64 time_SECS_PER_MIN  = 60ull;
static const u64 time_MINS_PER_HOUR = 60ull;
static const u64 time_HOURS_PER_DAY = 24ull;
static const u64 time_DAYS_PER_WEEK = 7ull;


#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* TIME_COMMON_INCLUDED */

/**
 * @copyright Copyright 2024. Gyeongtae Kim All rights reserved.
 *
 * @file    common.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2024-11-16 (date of creation)
 * @updated 2024-12-23 (date of last update)
 * @version v0.1-alpha
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

static const u32 time_nanos_per_sec   = 1000u * 1000u * 1000u;
static const u32 time_nanos_per_milli = 1000u * 1000u;
static const u32 time_nanos_per_micro = 1000u;
static const u32 time_millis_per_sec  = 1000u;
static const u32 time_micros_per_sec  = 1000u * 1000u;

// static const u32 time_ns_per_s  = time_nanos_per_sec;
// static const u32 time_ns_per_ms = time_nanos_per_milli;
// static const u32 time_ns_per_us = time_nanos_per_micro;
// static const u32 time_ms_per_s  = time_millis_per_sec;
// static const u32 time_us_per_s  = time_micros_per_sec;

static const u64 time_secs_per_min  = 60ull;
static const u64 time_mins_per_hour = 60ull;
static const u64 time_hours_per_day = 24ull;
static const u64 time_days_per_week = 7ull;

/* Sleep */
extern void time_sleep(time_Duration duration);
extern void time_sleepSecs(u64 secs);
extern void time_sleepMillis(u64 millis);
extern void time_sleepMicros(u64 micros);
extern void time_sleepNanos(u32 nanos);

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* TIME_COMMON_INCLUDED */

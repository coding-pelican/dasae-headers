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

/* Data Types */
typedef struct time_Duration time_Duration;
decl_Opt$(time_Duration);
typedef struct time_Instant time_Instant;
decl_Opt$(time_Instant);
typedef struct time_SysTime time_SysTime;
decl_Opt$(time_SysTime);

/* Constants */
static const u32 time_nanos_per_sec   = lit_num(1, 000, 000, 000u);
static const u32 time_nanos_per_milli = lit_num(1, 000, 000);
static const u32 time_nanos_per_micro = lit_num(1, 000u);
static const u32 time_millis_per_sec  = lit_num(1, 000u);
static const u32 time_micros_per_sec  = lit_num(1, 000, 000u);

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

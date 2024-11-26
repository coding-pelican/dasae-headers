/**
 * @copyright Copyright 2024. Gyeongtae Kim All rights reserved.
 *
 * @file    SysTime.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2024-11-10 (date of creation)
 * @updated 2024-11-15 (date of last update)
 * @version v1.0.0
 * @ingroup dasae-headers(dh)/time
 * @prefix  time_SysTime
 *
 * @brief   Source of some software
 * @details Some detailed explanation
 */

#ifndef TIME_SYS_TIME_INCLUDED
#define TIME_SYS_TIME_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "cfg.h"
#include "common.h"

/*========== Macros and Definitions =========================================*/
/*
 * // Static variables for performance counter
 * static time_SysTime s_performance_frequency;
 * static f64          s_frequency_inverse;
 * static time_SysTime s_offset_value;
 * static bool         s_initialized;
 * // Initialize performance counter frequency
 * static void __attribute__((constructor)) init(void);
 */

/* Accessors */
extern time_SysTime time_SysTime_frequency(void);
extern f64          time_SysTime_frequencyInv(void);
extern time_SysTime time_SysTime_value(void);
extern time_SysTime time_SysTime_offset(void);

/* Operations */
extern time_SysTime  time_SysTime_now(void);
extern time_Duration time_SysTime_elapsed(time_SysTime self);
extern time_Duration time_SysTime_durationSince(time_SysTime self, time_SysTime earlier);

/* Arithmetic */
extern bool         ops_try_add_other(time_SysTime, time_Duration, time_SysTime);
extern bool         ops_try_sub_other(time_SysTime, time_Duration, time_SysTime);
extern time_SysTime ops_add_other(time_SysTime, time_Duration);
extern time_SysTime ops_sub_other(time_SysTime, time_Duration);

/* Comparison */
extern cmp_Ord time_SysTime_cmp(time_SysTime self, time_SysTime other);
cmp_eq_impl(time_SysTime);
cmp_ne_impl(time_SysTime);
cmp_lt_impl(time_SysTime);
cmp_gt_impl(time_SysTime);
cmp_le_impl(time_SysTime);
cmp_ge_impl(time_SysTime);

/* Sleep */
extern void time_SysTime_sleep(time_Duration duration);
extern void time_SysTime_sleepSecs(u64 secs);
extern void time_SysTime_sleepMillis(u64 millis);
extern void time_SysTime_sleepMicros(u64 micros);
extern void time_SysTime_sleepNanos(u32 nanos);

// /* Constants for time conversion */
// #define INTERVALS_PER_SEC       (10000000ULL)    /* 100ns intervals per second */
// #define SECS_TO_UNIX_EPOCH      (11644473600ULL) /* seconds between Windows epoch (1601) and Unix epoch (1970) */
// #define INTERVALS_TO_UNIX_EPOCH (SECS_TO_UNIX_EPOCH * INTERVALS_PER_SEC)

// /* UNIX Epoch constant definition
//  * Windows FILETIME is measured in 100-nanosecond intervals since January 1, 1601 UTC
//  * Unix Epoch is January 1, 1970 UTC
//  * We need to represent Unix Epoch in Windows FILETIME format
//  */
extern const time_SysTime time_SysTime_unix_epoch;

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* TIME_SYS_TIME_INCLUDED */

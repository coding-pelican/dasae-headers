/**
 * @copyright Copyright (c) 2024 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    common.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2024-11-16 (date of creation)
 * @updated 2024-12-23 (date of last update)
 * @version v0.1-alpha
 * @ingroup dasae-headers(dh)/time
 * @prefix  time
 *
 * @brief   Common time utilities
 * @details Provides common time utilities and functions for:
 *          - Time duration operations
 *          - Timestamp and interval measurements
 *          - Time formatting and parsing
 *          - Platform-independent time operations
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
T_decl_O$(time_Duration);
typedef struct time_Instant time_Instant;
T_decl_O$(time_Instant);
typedef struct time_SysTime time_SysTime;
T_decl_O$(time_SysTime);

/* Constants */
static const u32 time_nanos_per_sec = lit_num(1, 000, 000, 000u);
static const u32 time_nanos_per_milli = lit_num(1, 000, 000);
static const u32 time_nanos_per_micro = lit_num(1, 000u);
static const u32 time_millis_per_sec = lit_num(1, 000u);
static const u32 time_micros_per_sec = lit_num(1, 000, 000u);

static const u64 time_secs_per_min = 60ull;
static const u64 time_mins_per_hour = 60ull;
static const u64 time_hours_per_day = 24ull;
static const u64 time_days_per_week = 7ull;

/* Sleep */
extern fn_((time_sleep(time_Duration duration))(void));
extern fn_((time_sleepSecs(u64 secs))(void));
extern fn_((time_sleepMillis(u64 millis))(void));
extern fn_((time_sleepMicros(u64 micros))(void));
extern fn_((time_sleepNanos(u32 nanos))(void));

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* TIME_COMMON_INCLUDED */

/**
 * @copyright Copyright (c) 2024-2026 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    common.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2024-11-16 (date of creation)
 * @updated 2026-01-03 (date of last update)
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
#ifndef time_common__included
#define time_common__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "cfg.h"

/*========== Macros and Declarations ========================================*/

/* --- Data Types --- */
typedef struct time_Duration time_Duration;
T_decl_O$(time_Duration);
typedef struct time_Instant time_Instant;
T_decl_O$(time_Instant);
typedef struct time_SysTime time_SysTime;
T_decl_O$(time_SysTime);

/* --- Constants --- */
#define time_nanos_per_nano __comp_const__time_nanos_per_nano
#define time_nanos_per_micro __comp_const__time_nanos_per_micro
#define time_nanos_per_milli __comp_const__time_nanos_per_milli
#define time_nanos_per_sec __comp_const__time_nanos_per_sec
#define time_micros_per_sec __comp_const__time_micros_per_sec
#define time_millis_per_sec __comp_const__time_millis_per_sec

#define time_secs_per_sec __comp_const__time_secs_per_sec
#define time_secs_per_min __comp_const__time_secs_per_min
#define time_mins_per_hour __comp_const__time_mins_per_hour
#define time_hours_per_day __comp_const__time_hours_per_day
#define time_days_per_week __comp_const__time_days_per_week

/* --- Sleep --- */
$extern fn_((time_sleep(time_Duration duration))(void));
$extern fn_((time_sleepSecs(u64 secs))(void));
$extern fn_((time_sleepMillis(u64 millis))(void));
$extern fn_((time_sleepMicros(u64 micros))(void));
$extern fn_((time_sleepNanos(u32 nanos))(void));

/*========== Macros and Definitions =========================================*/

#define __comp_const__time_nanos_per_nano (lit_n$(u32)(1u))
#define __comp_const__time_nanos_per_micro (lit_n$(u32)(1, 000u))
#define __comp_const__time_nanos_per_milli (lit_n$(u32)(1, 000, 000))
#define __comp_const__time_nanos_per_sec (lit_n$(u32)(1, 000, 000, 000u))
#define __comp_const__time_micros_per_sec (lit_n$(u32)(1, 000, 000u))
#define __comp_const__time_millis_per_sec (lit_n$(u32)(1, 000u))

#define __comp_const__time_secs_per_sec (lit_n$(u64)(1ull))
#define __comp_const__time_secs_per_min (lit_n$(u64)(60ull))
#define __comp_const__time_mins_per_hour (lit_n$(u64)(60ull))
#define __comp_const__time_hours_per_day (lit_n$(u64)(24ull))
#define __comp_const__time_days_per_week (lit_n$(u64)(7ull))

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* time_common__included */

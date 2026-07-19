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
#pragma once
#ifndef time_common__included
#define time_common__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "base.h"

/*========== Macros and Declarations ========================================*/

#define time_nanos_per_nano __comp_const__time_nanos_per_nano
#define time_nanos_per_micro __comp_const__time_nanos_per_micro
#define time_nanos_per_milli __comp_const__time_nanos_per_milli
#define time_nanos_per_sec __comp_const__time_nanos_per_sec
#define time_nanos_per_min __comp_const__time_nanos_per_min
#define time_nanos_per_hour __comp_const__time_nanos_per_hour
#define time_nanos_per_day __comp_const__time_nanos_per_day
#define time_nanos_per_week __comp_const__time_nanos_per_week

#define time_micros_per_micro __comp_const__time_micros_per_micro
#define time_micros_per_milli __comp_const__time_micros_per_milli
#define time_micros_per_sec __comp_const__time_micros_per_sec
#define time_micros_per_min __comp_const__time_micros_per_min
#define time_micros_per_hour __comp_const__time_micros_per_hour
#define time_micros_per_day __comp_const__time_micros_per_day
#define time_micros_per_week __comp_const__time_micros_per_week

#define time_millis_per_milli __comp_const__time_millis_per_milli
#define time_millis_per_sec __comp_const__time_millis_per_sec
#define time_millis_per_min __comp_const__time_millis_per_min
#define time_millis_per_hour __comp_const__time_millis_per_hour
#define time_millis_per_day __comp_const__time_millis_per_day
#define time_millis_per_week __comp_const__time_millis_per_week

#define time_secs_per_sec __comp_const__time_secs_per_sec
#define time_secs_per_min __comp_const__time_secs_per_min
#define time_secs_per_hour __comp_const__time_secs_per_hour
#define time_secs_per_day __comp_const__time_secs_per_day
#define time_secs_per_week __comp_const__time_secs_per_week

#define time_mins_per_min __comp_const__time_mins_per_min
#define time_mins_per_hour __comp_const__time_mins_per_hour
#define time_mins_per_day __comp_const__time_mins_per_day
#define time_mins_per_week __comp_const__time_mins_per_week

#define time_hours_per_hour __comp_const__time_hours_per_hour
#define time_hours_per_day __comp_const__time_hours_per_day
#define time_hours_per_week __comp_const__time_hours_per_week

#define time_days_per_day __comp_const__time_days_per_day
#define time_days_per_week __comp_const__time_days_per_week

/*========== Macros and Definitions =========================================*/

#define __comp_const__time_nanos_per_nano \
    (u32_(1u))
#define __comp_const__time_nanos_per_micro \
    (u32_(1, 000u))
#define __comp_const__time_nanos_per_milli \
    (u32_(1, 000, 000u))
#define __comp_const__time_nanos_per_sec \
    (u32_(1, 000, 000, 000u))
#define __comp_const__time_nanos_per_min \
    (u64_(60, 000, 000, 000ull))
#define __comp_const__time_nanos_per_hour \
    (u64_(3, 600, 000, 000, 000ull))
#define __comp_const__time_nanos_per_day \
    (u64_(86, 400, 000, 000, 000ull))
#define __comp_const__time_nanos_per_week \
    (u64_(604, 800, 000, 000, 000ull))

#define __comp_const__time_micros_per_micro \
    (u32_(1u))
#define __comp_const__time_micros_per_milli \
    (u32_(1, 000u))
#define __comp_const__time_micros_per_sec \
    (u32_(1, 000, 000u))
#define __comp_const__time_micros_per_min \
    (u64_(60, 000, 000ull))
#define __comp_const__time_micros_per_hour \
    (u64_(3, 600, 000, 000ull))
#define __comp_const__time_micros_per_day \
    (u64_(86, 400, 000, 000ull))
#define __comp_const__time_micros_per_week \
    (u64_(604, 800, 000, 000ull))

#define __comp_const__time_millis_per_milli \
    (u32_(1u))
#define __comp_const__time_millis_per_sec \
    (u32_(1, 000u))
#define __comp_const__time_millis_per_min \
    (u32_(60, 000u))
#define __comp_const__time_millis_per_hour \
    (u32_(3, 600, 000u))
#define __comp_const__time_millis_per_day \
    (u32_(86, 400, 000u))
#define __comp_const__time_millis_per_week \
    (u32_(604, 800, 000u))

#define __comp_const__time_secs_per_sec \
    (u64_(1ull))
#define __comp_const__time_secs_per_min \
    (u64_(60ull))
#define __comp_const__time_secs_per_hour \
    (u64_(3, 600ull))
#define __comp_const__time_secs_per_day \
    (u64_(86, 400ull))
#define __comp_const__time_secs_per_week \
    (u64_(604, 800ull))

#define __comp_const__time_mins_per_min \
    (u64_(1ull))
#define __comp_const__time_mins_per_hour \
    (u64_(60ull))
#define __comp_const__time_mins_per_day \
    (u64_(1, 440ull))
#define __comp_const__time_mins_per_week \
    (u64_(10, 080ull))

#define __comp_const__time_hours_per_hour \
    (u64_(1ull))
#define __comp_const__time_hours_per_day \
    (u64_(24ull))
#define __comp_const__time_hours_per_week \
    (u64_(168ull))

#define __comp_const__time_days_per_day \
    (u64_(1ull))
#define __comp_const__time_days_per_week \
    (u64_(7ull))

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* time_common__included */

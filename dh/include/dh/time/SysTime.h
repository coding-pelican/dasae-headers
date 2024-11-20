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
// #include "dh/cmp.h"
// #include "dh/arith.h"

/*========== Macros and Definitions =========================================*/
/*
 * #if defined(_WIN32) || defined(_WIN64)
 * static time_SysTime   time_SysTime__s_performance_frequency  = 0;
 * static f64               time_SysTime__s_frequency_inverse      = 0.0;
 * static time_SysTime   time_SysTime__s_offset_value_frequency = 0;
 * static bool              time_SysTime__s_frequency_initialized  = false;
 * static void __attribute__((constructor)) time_SysTime__init(void);
 * static inline bool time_SysTime_checkedCmp(
 *     time_SysTime lhs,
 *     time_SysTime rhs,
 *     EOrd* out_result
 * );
 * static inline bool time_SysTime_checkedDurationCalc(
 *     time_SysTime lhs,
 *     time_SysTime rhs,
 *     time_Duration* out_duration,
 *     bool allow_negative
 * );
 * static inline bool time_SysTime_checkedArith(
 *     time_SysTime t,
 *     time_Duration d,
 *     time_SysTime* out_time,
 *     bool is_add
 * );
 * #endif // defined(_WIN32) || defined(_WIN64)
 */

time_SysTime time_SysTime_frequency(void);
f64          time_SysTime_frequency_inv(void);
time_SysTime time_SysTime_value(void);
time_SysTime time_SysTime_offset(void);
time_SysTime time_SysTime_now(void);
f64          time_SysTime_now_f64(void);

time_Duration time_SysTime_elapsed(time_SysTime t);
bool          time_SysTime_checkedElapsed(time_SysTime t, time_Duration* out_duration);

time_Duration time_SysTime_durationSince(time_SysTime t, time_SysTime earlier);
bool          time_SysTime_checkedDurationSince(time_SysTime t, time_SysTime earlier, time_Duration* const out_duration);

time_SysTime time_SysTime_addDuration(time_SysTime t, time_Duration duration);
bool         time_SysTime_checkedAddDuration(time_SysTime t, time_Duration duration, time_SysTime* out_time);
time_SysTime time_SysTime_subDuration(time_SysTime t, time_Duration duration);
bool         time_SysTime_checkedSubDuration(time_SysTime t, time_Duration duration, time_SysTime* out_time);

bool time_SysTime_eq(time_SysTime lhs, time_SysTime rhs);
bool time_SysTime_ne(time_SysTime lhs, time_SysTime rhs);
bool time_SysTime_lt(time_SysTime lhs, time_SysTime rhs);
bool time_SysTime_le(time_SysTime lhs, time_SysTime rhs);
bool time_SysTime_gt(time_SysTime lhs, time_SysTime rhs);
bool time_SysTime_ge(time_SysTime lhs, time_SysTime rhs);

void time_SysTime_sleep(time_Duration duration);
void time_SysTime_sleep_s(u64 secs);
void time_SysTime_sleep_s_f64(f64 secs);
void time_SysTime_sleep_ms(u64 millis);
void time_SysTime_sleep_us(u64 micros);
void time_SysTime_sleep_ns(u32 nanos);

static const time_SysTime time_SysTime_UNIX_EPOCH = make(time_SysTime);


#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* TIME_SYS_TIME_INCLUDED */

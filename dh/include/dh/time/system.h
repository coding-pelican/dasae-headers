/**
 * @copyright Copyright 2024. Gyeongtae Kim All rights reserved.
 *
 * @file    system.h
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


#ifndef TIME_SYSTEM_INCLUDED
#define TIME_SYSTEM_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */


/*========== Includes =======================================================*/

#include "cfg.h"

/*========== Macros and Definitions =========================================*/

// #if defined(_WIN32) || defined(_WIN64)
// static time_SystemTime   time_SystemTime__s_performance_frequency  = 0;
// static f64               time_SystemTime__s_frequency_inverse      = 0.0;
// static time_SystemTime   time_SystemTime__s_offset_value_frequency = 0;
// static bool              time_SystemTime__s_frequency_initialized  = false;
// static void              time_SystemTime__init(void);
// #endif /* defined(_WIN32) || defined(_WIN64) */

time_SystemTime time_SystemTime_frequency(void);
f64             time_SystemTime_frequency_inv(void);
time_SystemTime time_SystemTime_value(void);
time_SystemTime time_SystemTime_offset(void);
time_SystemTime time_SystemTime_now(void);
f64             time_SystemTime_now_f64(void);

void time_SystemTime_sleep(time_Duration duration);
void time_SystemTime_sleep_s(u64 secs);
void time_SystemTime_sleep_s_f64(f64 secs);
void time_SystemTime_sleep_ms(u64 millis);
void time_SystemTime_sleep_us(u64 micros);
void time_SystemTime_sleep_ns(u32 nanos);


#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* TIME_SYSTEM_INCLUDED */

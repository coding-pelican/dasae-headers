/**
 * @copyright Copyright 2024. Gyeongtae Kim All rights reserved.
 *
 * @file    system.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2024-11-10 (date of creation)
 * @updated 2024-11-10 (date of last update)
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


#if defined(_WIN32) || defined(_WIN64)
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#else // UNIX
#include <sys/time.h>
#include <time.h>
#endif


// #if defined(_WIN32) || defined(_WIN64)
// static SystemTime SystemTime__s_performance_frequency = { 0 };
// static bool       SystemTime__s_frequency_initialized = false;
// static void       SystemTime__initFrequency(void);
// #endif /* defined(_WIN32) || defined(_WIN64) */
SystemTime SystemTime_now(void);
f64        SystemTime_toSecs(SystemTime time);
f64        SystemTime_toMillis(SystemTime time);
f64        SystemTime_toMicros(SystemTime time);
u64        SystemTime_toNanos(SystemTime time);
void       SystemTime_sleep(Duration duration);
void       SystemTime_sleep_s(f64 secs);
void       SystemTime_sleep_ms(f64 millis);
void       SystemTime_sleep_us(f64 micros);
void       SystemTime_sleep_ns(u64 nanos);


#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* TIME_SYSTEM_INCLUDED */

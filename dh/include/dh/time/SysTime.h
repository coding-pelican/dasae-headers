/**
 * @copyright Copyright 2024. Gyeongtae Kim All rights reserved.
 *
 * @file    SysTime.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2024-11-10 (date of creation)
 * @updated 2024-12-23 (date of last update)
 * @version v0.1-alpha
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
 * static time_SysTime time_SysTime_s_performance_frequency;
 * static f64          time_SysTime_s_frequency_inverse;
 * static time_SysTime time_SysTime_s_offset_value;
 * static bool         time_SysTime_s_initialized;
 * // Initialize performance counter frequency
 * static void __attribute__((constructor)) init(void);
 */
use_Opt$(time_SysTime);

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
extern Opt$time_SysTime time_SysTime_addDurationChecked(time_SysTime self, time_Duration other);
extern Opt$time_SysTime time_SysTime_subDurationChecked(time_SysTime self, time_Duration other);

extern time_SysTime op_fnAddBy(time_SysTime, time_Duration);
extern time_SysTime op_fnSubBy(time_SysTime, time_Duration);

/* Comparison */
extern cmp_fnCmp(time_SysTime);
cmp_fnEq_default(time_SysTime);
cmp_fnNe_default(time_SysTime);
cmp_fnLt_default(time_SysTime);
cmp_fnGt_default(time_SysTime);
cmp_fnLe_default(time_SysTime);
cmp_fnGe_default(time_SysTime);

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

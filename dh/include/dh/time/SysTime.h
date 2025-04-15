/**
 * @copyright Copyright (c) 2024-2025 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    SysTime.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2024-11-10 (date of creation)
 * @updated 2025-02-08 (date of last update)
 * @version v0.1-alpha.2
 * @ingroup dasae-headers(dh)/time
 * @prefix  time_SysTime
 *
 * @brief   System time utilities
 * @details Provides functionality for:
 *          - Time measurement and duration tracking
 *          - High-precision timestamps and intervals
 *          - Time formatting and parsing
 *          - Platform-independent time operations
 */

#ifndef TIME_SYS_TIME_INCLUDED
#define TIME_SYS_TIME_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "cfg.h"
#include "common.h"
#include "Duration.h"

/*========== Macros and Definitions =========================================*/

static const u64 time_SysTime_nanos_per_sec           = lit_num(1, 000, 000, 000ull);        // 1ns intervals per second
static const u64 time_SysTime_intervals_per_sec       = time_SysTime_nanos_per_sec / 100ull; // 100ns intervals per second
static const u64 time_SysTime_intervals_to_unix_epoch = lit_num(11, 644, 473, 600ull) * time_SysTime_intervals_per_sec;

/*========== Structures =====================================================*/

struct time_SysTime {
    time_SysTimePlatform impl_;
};
impl_Opt$(time_SysTime);

/*========== Accessors ======================================================*/

/// Get the frequency of the performance counter in ticks per second.
extern time_SysTime time_SysTime_freq(void);
/// Get the inverse of the performance counter frequency.
extern f64          time_SysTime_freqInv(void);
/// Get the offset value of the performance counter (relative time).
extern time_SysTime time_SysTime_offset(void);
/// Get the current system time in high resolution.
extern time_SysTime time_SysTime_value(void);

/*========== Operations =====================================================*/

/// Get the current time as a system time object.
extern time_SysTime      time_SysTime_now(void);
/// Get the elapsed duration from a given time.
extern time_Duration     time_SysTime_elapsed(time_SysTime self);
/// Get the duration since another time point.
extern time_Duration     time_SysTime_durationSince(time_SysTime later, time_SysTime earlier);
/// Get the duration since another time point with overflow checking.
extern Opt$time_Duration time_SysTime_chkdDurationSince(time_SysTime later, time_SysTime earlier);

/*========== Arithmetic Operations ==========================================*/

/// Add a duration to the time.
extern time_SysTime         op_fnAddBy(time_SysTime, time_Duration);
extern time_SysTime         op_fnAddAsgBy(time_SysTime, time_Duration);
$inline_always time_SysTime op_fnWrapAddBy(addDuration, time_SysTime, time_Duration);
$inline_always time_SysTime op_fnWrapAddAsgBy(addAsgDuration, time_SysTime, time_Duration);
/// Sub a duration from the time.
extern time_SysTime         op_fnSubBy(time_SysTime, time_Duration);
extern time_SysTime         op_fnSubAsgBy(time_SysTime, time_Duration);
$inline_always time_SysTime op_fnWrapSubBy(subDuration, time_SysTime, time_Duration);
$inline_always time_SysTime op_fnWrapSubAsgBy(subAsgDuration, time_SysTime, time_Duration);
/// Add a duration to the time with overflow checking.
extern Opt$time_SysTime     time_SysTime_chkdAddDuration(time_SysTime lhs, time_Duration rhs);
/// Sub a duration from the time with underflow checking.
extern Opt$time_SysTime     time_SysTime_chkdSubDuration(time_SysTime lhs, time_Duration rhs);

/*========== Time Conversion to/from Unix Epoch =============================*/

/// Convert system time to Unix epoch time.
extern time_SysTime time_SysTime_fromUnixEpoch(u64 secs);
/// Convert system time to Unix epoch seconds (useful for comparisons).
extern u64          time_SysTime_toUnixEpoch(time_SysTime self);

/*========== Comparison =====================================================*/

/// Compare two system times.
extern cmp_fnCmp(time_SysTime);
cmp_fnEq_default(time_SysTime);
cmp_fnNe_default(time_SysTime);
cmp_fnLt_default(time_SysTime);
cmp_fnGt_default(time_SysTime);
cmp_fnLe_default(time_SysTime);
cmp_fnGe_default(time_SysTime);

/*========== Constants ======================================================*/

static const time_SysTime time_SysTime_unix_epoch = {
    .impl_ = {
#if bti_plat_windows && (bti_plat_32bit || bti_plat_64bit)
        .QuadPart = as$(LONGLONG, time_SysTime_intervals_to_unix_epoch),
#else /* bti_plat_unix && (bti_plat_linux || bti_plat_bsd || bti_plat_darwin) */
        .tv_sec  = as$(time_t, time_SysTime_intervals_to_unix_epoch),
        .tv_nsec = 0,
#endif
    }
};

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* TIME_SYS_TIME_INCLUDED */

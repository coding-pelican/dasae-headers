/**
 * @copyright Copyright (c) 2024-2025 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    SysTime.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
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
#ifndef time_SysTime__included
#define time_SysTime__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "cfg.h"
#include "common.h"
#include "Duration.h"

/*========== Macros and Declarations ========================================*/

/* --- Structures --- */

struct time_SysTime {
    time_SysTimePlatform impl;
};
T_impl_O$(time_SysTime);

/* --- Constants --- */

#define time_SysTime_nanos_per_sec /* 1ns intervals per second */ __comp_const__time_SysTime_nanos_per_sec
#define time_SysTime_intervals_per_sec /* 100ns intervals per second */ __comp_const__time_SysTime_intervals_per_sec
#define time_SysTime_intervals_to_unix_epoch __comp_const__time_SysTime_intervals_to_unix_epoch
#define time_SysTime_unix_epoch __comp_const__time_SysTime_unix_epoch

/* --- Accessors --- */

/// Get the frequency of the performance counter in ticks per second.
$extern fn_((time_SysTime_freq(void))(time_SysTime));
/// Get the inverse of the performance counter frequency.
$extern fn_((time_SysTime_freqInv(void))(f64));
/// Get the offset value of the performance counter (relative time).
$extern fn_((time_SysTime_offset(void))(time_SysTime));
/// Get the current system time in high resolution.
$extern fn_((time_SysTime_value(void))(time_SysTime));

/* --- Operations --- */

/// Get the current time as a system time object.
$extern fn_((time_SysTime_now(void))(time_SysTime));
/// Get the elapsed duration from a given time.
$extern fn_((time_SysTime_elapsed(time_SysTime self))(time_Duration));
/// Get the duration since another time point.
$extern fn_((time_SysTime_durationSince(time_SysTime later, time_SysTime earlier))(time_Duration));
/// Get the duration since another time point with overflow checking.
$extern fn_((time_SysTime_durationSinceChkd(time_SysTime later, time_SysTime earlier))(O$time_Duration));

/* --- Arithmetic Operations --- */

/// Add a duration to the time.
$extern op_fn_addWith$(((time_SysTime, time_Duration)(lhs, rhs))(time_SysTime));
$static op_fn_addWith$(addDuration, ((time_SysTime, time_Duration)(lhs, rhs))(time_SysTime));
$extern op_fn_addAsgWith$(((time_SysTime, time_Duration)(lhs, rhs))(time_SysTime*));
$static op_fn_addAsgWith$(addAsgDuration, ((time_SysTime, time_Duration)(lhs, rhs))(time_SysTime*));
/// Sub a duration from the time.
$extern op_fn_subWith$(((time_SysTime, time_Duration)(lhs, rhs))(time_SysTime));
$static op_fn_subWith$(subDuration, ((time_SysTime, time_Duration)(lhs, rhs))(time_SysTime));
$extern op_fn_subAsgWith$(((time_SysTime, time_Duration)(lhs, rhs))(time_SysTime*));
$static op_fn_subAsgWith$(subAsgDuration, ((time_SysTime, time_Duration)(lhs, rhs))(time_SysTime*));
/// Add a duration to the time with overflow checking.
$extern fn_((time_SysTime_addChkdDuration(time_SysTime lhs, time_Duration rhs))(O$time_SysTime));
/// Sub a duration from the time with underflow checking.
$extern fn_((time_SysTime_subChkdDuration(time_SysTime lhs, time_Duration rhs))(O$time_SysTime));

/* --- Time Conversion to/from Unix Epoch --- */

/// Convert system time to Unix epoch time.
$extern fn_((time_SysTime_fromUnixEpoch(u64 secs))(time_SysTime));
/// Convert system time to Unix epoch seconds (useful for comparisons).
$extern fn_((time_SysTime_toUnixEpoch(time_SysTime self))(u64));

/* --- Comparison --- */

$extern cmp_fn_ord$((time_SysTime)(lhs, rhs));
$extern cmp_fn_eq$((time_SysTime)(lhs, rhs));
$extern cmp_fn_ne$((time_SysTime)(lhs, rhs));
$extern cmp_fn_lt$((time_SysTime)(lhs, rhs));
$extern cmp_fn_gt$((time_SysTime)(lhs, rhs));
$extern cmp_fn_le$((time_SysTime)(lhs, rhs));
$extern cmp_fn_ge$((time_SysTime)(lhs, rhs));
$extern cmp_fn_ordCtx$((time_SysTime)(lhs, rhs, ctx));
$extern cmp_fn_eqCtx$((time_SysTime)(lhs, rhs, ctx));
$extern cmp_fn_neCtx$((time_SysTime)(lhs, rhs, ctx));
$extern cmp_fn_ltCtx$((time_SysTime)(lhs, rhs, ctx));
$extern cmp_fn_gtCtx$((time_SysTime)(lhs, rhs, ctx));
$extern cmp_fn_leCtx$((time_SysTime)(lhs, rhs, ctx));
$extern cmp_fn_geCtx$((time_SysTime)(lhs, rhs, ctx));

$extern cmp_fn_eql$((time_SysTime)(lhs, rhs));
$extern cmp_fn_neq$((time_SysTime)(lhs, rhs));
$extern cmp_fn_eqlCtx$((time_SysTime)(lhs, rhs, ctx));
$extern cmp_fn_neqCtx$((time_SysTime)(lhs, rhs, ctx));

/*========== Macros and Definitions =========================================*/

#define __comp_const__time_SysTime_nanos_per_sec \
    (lit_n$(u64)(1, 000, 000, 000ull))
#define __comp_const__time_SysTime_intervals_per_sec \
    (lit_n$(u64)(time_SysTime_nanos_per_sec / 100ull))
#define __comp_const__time_SysTime_intervals_to_unix_epoch \
    (lit_n$(u64)(11, 644, 473, 600ull) * time_SysTime_intervals_per_sec)

#if plat_is_windows && (arch_bits_is_32bit || arch_bits_is_64bit)
#define __comp_const__time_SysTime_unix_epoch lit$((time_SysTime){ \
    .impl = { .QuadPart = as$(LONGLONG)(time_SysTime_intervals_to_unix_epoch) }, \
})
#else /* plat_based_unix && (plat_is_linux || plat_is_darwin) */
#define __comp_const__time_SysTime_unix_epoch lit$((time_SysTime){ \
    .impl = { .tv_sec = as$(time_t)(time_SysTime_intervals_to_unix_epoch), .tv_nsec = 0 }, \
})
#endif

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* time_SysTime__included */

/**
 * @copyright Copyright (c) 2024-2026 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    Instant.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2024-11-10 (date of creation)
 * @updated 2026-01-02 (date of last update)
 * @ingroup dasae-headers(dh)/time
 * @prefix  time_Instant
 *
 * @brief   Monotonic clock utilities
 * @details Provides high-precision monotonic timing:
 *          - Uses QueryPerformanceCounter on Windows
 *          - Uses clock_gettime(CLOCK_MONOTONIC) on Unix
 *          - Guaranteed to never go backwards
 *          - Suitable for measuring elapsed time
 *
 * @note    Unlike time_SysTime (wall-clock), time_Instant uses a monotonic
 *          clock that cannot be converted to/from Unix epoch.
 *          Use time_SysTime for wall-clock time with Unix epoch support.
 */
#ifndef time_Instant__included
#define time_Instant__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "cfg.h"
#include "common.h"
#include "Duration.h"

/*========== Macros and Declarations ========================================*/

/* --- Structures --- */

struct time_Instant {
    time_InstantPlatform impl;
};
T_impl_O$(time_Instant);

/* --- Constants --- */

#define time_Instant_nanos_per_sec __comp_const__time_Instant_nanos_per_sec
#define time_Instant_intervals_per_sec __comp_const__time_Instant_intervals_per_sec

/* --- Accessors --- */

/// Get the frequency of the performance counter in ticks per second.
$extern fn_((time_Instant_freq(void))(time_Instant));
/// Get the inverse of the performance counter frequency.
$extern fn_((time_Instant_freqInv(void))(f64));
/// Get the offset value of the performance counter (relative time).
$extern fn_((time_Instant_offset(void))(time_Instant));
/// Get the raw tick value as u64 (useful for seeding, hashing, etc.).
$extern fn_((time_Instant_ticks(time_Instant self))(u64));

/* --- Operations --- */

/// Get the current monotonic time.
$extern fn_((time_Instant_now(void))(time_Instant));
/// Get the elapsed duration from a given time.
$extern fn_((time_Instant_elapsed(time_Instant self))(time_Duration));
/// Get the duration since another time point.
$extern fn_((time_Instant_durationSince(time_Instant later, time_Instant earlier))(time_Duration));
/// Get the duration since another time point with overflow checking.
$extern fn_((time_Instant_durationSinceChkd(time_Instant later, time_Instant earlier))(O$time_Duration));

/* --- Arithmetic Operations --- */

$extern op_fn_addWith$(((time_Instant, time_Duration)(lhs, rhs))(time_Instant));
$static op_fn_addWith$(addDuration, ((time_Instant, time_Duration)(lhs, rhs))(time_Instant));
$extern op_fn_addAsgWith$(((time_Instant, time_Duration)(lhs, rhs))(time_Instant*));
$static op_fn_addAsgWith$(addAsgDuration, ((time_Instant, time_Duration)(lhs, rhs))(time_Instant*));
$extern op_fn_subWith$(((time_Instant, time_Duration)(lhs, rhs))(time_Instant));
$static op_fn_subWith$(subDuration, ((time_Instant, time_Duration)(lhs, rhs))(time_Instant));
$extern op_fn_subAsgWith$(((time_Instant, time_Duration)(lhs, rhs))(time_Instant*));
$static op_fn_subAsgWith$(subAsgDuration, ((time_Instant, time_Duration)(lhs, rhs))(time_Instant*));
$extern fn_((time_Instant_addChkdDuration(time_Instant lhs, time_Duration rhs))(O$time_Instant));
$extern fn_((time_Instant_subChkdDuration(time_Instant lhs, time_Duration rhs))(O$time_Instant));

/* --- Comparison --- */

$extern cmp_fn_ord$((time_Instant)(lhs, rhs));
$extern cmp_fn_eq$((time_Instant)(lhs, rhs));
$extern cmp_fn_ne$((time_Instant)(lhs, rhs));
$extern cmp_fn_lt$((time_Instant)(lhs, rhs));
$extern cmp_fn_gt$((time_Instant)(lhs, rhs));
$extern cmp_fn_le$((time_Instant)(lhs, rhs));
$extern cmp_fn_ge$((time_Instant)(lhs, rhs));
$extern cmp_fn_ordCtx$((time_Instant)(lhs, rhs, ctx));
$extern cmp_fn_eqCtx$((time_Instant)(lhs, rhs, ctx));
$extern cmp_fn_neCtx$((time_Instant)(lhs, rhs, ctx));
$extern cmp_fn_ltCtx$((time_Instant)(lhs, rhs, ctx));
$extern cmp_fn_gtCtx$((time_Instant)(lhs, rhs, ctx));
$extern cmp_fn_leCtx$((time_Instant)(lhs, rhs, ctx));
$extern cmp_fn_geCtx$((time_Instant)(lhs, rhs, ctx));

$extern cmp_fn_eql$((time_Instant)(lhs, rhs));
$extern cmp_fn_neq$((time_Instant)(lhs, rhs));
$extern cmp_fn_eqlCtx$((time_Instant)(lhs, rhs, ctx));
$extern cmp_fn_neqCtx$((time_Instant)(lhs, rhs, ctx));

/*========== Macros and Definitions =========================================*/

#define __comp_const__time_Instant_nanos_per_sec \
    (lit_n$(u64)(1, 000, 000, 000ull))
#define __comp_const__time_Instant_intervals_per_sec \
    (lit_n$(u64)(time_Instant_nanos_per_sec / 100ull))

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* time_Instant__included */

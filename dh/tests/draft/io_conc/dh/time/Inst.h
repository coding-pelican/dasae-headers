/**
 * @copyright Copyright (c) 2024-2026 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    Instant.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2024-11-10 (date of creation)
 * @updated 2026-01-02 (date of last update)
 * @ingroup dasae-headers(dh)/time
 * @prefix  time_Inst
 *
 * @brief   Monotonic clock utilities
 * @details Provides high-precision monotonic timing:
 *          - Uses QueryPerformanceCounter on Windows
 *          - Uses clock_gettime(CLOCK_MONOTONIC) on Unix
 *          - Guaranteed to never go backwards
 *          - Suitable for measuring elapsed time
 *
 * @note    Unlike time_SysTime (wall-clock), time_Inst uses a monotonic
 *          clock that cannot be converted to/from Unix epoch.
 *          Use time_SysTime for wall-clock time with Unix epoch support.
 */
#ifndef time_Inst__included
#define time_Inst__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "Self.h"
#include "Dur.h"

/*========== Macros and Declarations ========================================*/

/* --- Structures --- */

struct time_Inst {
    time_InstPlatform impl;
};
T_impl_O$(time_Inst);

/* --- Constants --- */

#define time_Inst_nanos_per_sec __comp_const__time_Inst_nanos_per_sec
#define time_Inst_intervals_per_sec __comp_const__time_Inst_intervals_per_sec

/* --- Accessors --- */

/// Get the frequency of the performance counter in ticks per second.
$extern fn_((time_Inst_freq(time_Self time))(time_Inst));
/// Get the inverse of the performance counter frequency.
$extern fn_((time_Inst_freqInv(time_Self time))(f64));
/// Get the offset value of the performance counter (relative time).
$extern fn_((time_Inst_offset(time_Self time))(time_Inst));
/// Get the raw tick value as u64 (useful for seeding, hashing, etc.).
$extern fn_((time_Inst_ticks(time_Inst self))(u64));

/* --- Operations --- */

/// Get the current monotonic time.
$extern fn_((time_Inst_now(time_Self time))(time_Inst));
/// Get the elapsed duration from a given time.
$extern fn_((time_Inst_elapsed(time_Inst self, time_Self time))(time_Dur));
/// Get the duration since another time point.
$extern fn_((time_Inst_durSince(time_Inst later, time_Inst earlier))(time_Dur));
/// Get the duration since another time point with overflow checking.
$extern fn_((time_Inst_durSinceChkd(time_Inst later, time_Inst earlier))(O$time_Dur));

/* --- Arithmetic Operations --- */

$extern op_fn_addWith$(((time_Inst, time_Dur)(lhs, rhs))(time_Inst));
$static op_fn_addWith$(addDur, ((time_Inst, time_Dur)(lhs, rhs))(time_Inst));
$extern op_fn_addAsgWith$(((time_Inst, time_Dur)(lhs, rhs))(time_Inst*));
$static op_fn_addAsgWith$(addAsgDur, ((time_Inst, time_Dur)(lhs, rhs))(time_Inst*));
$extern op_fn_subWith$(((time_Inst, time_Dur)(lhs, rhs))(time_Inst));
$static op_fn_subWith$(subDur, ((time_Inst, time_Dur)(lhs, rhs))(time_Inst));
$extern op_fn_subAsgWith$(((time_Inst, time_Dur)(lhs, rhs))(time_Inst*));
$static op_fn_subAsgWith$(subAsgDur, ((time_Inst, time_Dur)(lhs, rhs))(time_Inst*));
$extern fn_((time_Inst_addChkdDur(time_Inst lhs, time_Dur rhs))(O$time_Inst));
$extern fn_((time_Inst_subChkdDur(time_Inst lhs, time_Dur rhs))(O$time_Inst));

/* --- Comparison --- */

$extern cmp_fn_ord$((time_Inst)(lhs, rhs));
$extern cmp_fn_eq$((time_Inst)(lhs, rhs));
$extern cmp_fn_ne$((time_Inst)(lhs, rhs));
$extern cmp_fn_lt$((time_Inst)(lhs, rhs));
$extern cmp_fn_gt$((time_Inst)(lhs, rhs));
$extern cmp_fn_le$((time_Inst)(lhs, rhs));
$extern cmp_fn_ge$((time_Inst)(lhs, rhs));
$extern cmp_fn_ordCtx$((time_Inst)(lhs, rhs, ctx));
$extern cmp_fn_eqCtx$((time_Inst)(lhs, rhs, ctx));
$extern cmp_fn_neCtx$((time_Inst)(lhs, rhs, ctx));
$extern cmp_fn_ltCtx$((time_Inst)(lhs, rhs, ctx));
$extern cmp_fn_gtCtx$((time_Inst)(lhs, rhs, ctx));
$extern cmp_fn_leCtx$((time_Inst)(lhs, rhs, ctx));
$extern cmp_fn_geCtx$((time_Inst)(lhs, rhs, ctx));

$extern cmp_fn_eql$((time_Inst)(lhs, rhs));
$extern cmp_fn_neq$((time_Inst)(lhs, rhs));
$extern cmp_fn_eqlCtx$((time_Inst)(lhs, rhs, ctx));
$extern cmp_fn_neqCtx$((time_Inst)(lhs, rhs, ctx));

/*========== Macros and Definitions =========================================*/

#define __comp_const__time_Inst_nanos_per_sec \
    (n$(u64)(1, 000, 000, 000ull))
#define __comp_const__time_Inst_intervals_per_sec \
    (n$(u64)(time_Inst_nanos_per_sec / 100ull))

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* time_Inst__included */

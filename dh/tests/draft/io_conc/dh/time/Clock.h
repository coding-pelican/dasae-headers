/**
 * @copyright Copyright (c) 2024-2026 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    SysTime.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2024-11-10 (date of creation)
 * @updated 2026-01-03 (date of last update)
 * @ingroup dasae-headers(dh)/time
 * @prefix  time_Clock
 *
 * @brief   Wall-clock time utilities
 * @details Provides functionality for wall-clock time operations:
 *          - Current wall-clock time retrieval
 *          - Unix epoch conversion (to/from)
 *          - Duration calculations between system times
 *          - Platform-independent wall-clock operations
 *
 * @note    Unlike time_Instant which uses a monotonic clock,
 *          time_Clock uses the system's wall clock and can
 *          go backwards (e.g., due to NTP synchronization).
 *          Use time_Instant for measuring elapsed time.
 */
#ifndef time_Clock__included
#define time_Clock__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "Self.h"
#include "Dur.h"

/*========== Macros and Declarations ========================================*/

/* --- Structures --- */

struct time_Clock {
    time_ClockPlatform impl;
};
T_impl_O$(time_Clock);

/* --- Constants --- */

/// Number of 100-nanosecond intervals per second (Windows FILETIME unit).
#define time_Clock_intervals_per_sec __comp_const__time_Clock_intervals_per_sec
/// Number of 100-nanosecond intervals from Windows epoch (1601) to Unix epoch (1970).
#define time_Clock_intervals_to_unix_epoch __comp_const__time_Clock_intervals_to_unix_epoch
/// The Unix epoch (January 1, 1970 00:00:00 UTC) as a SysTime.
#define time_Clock_unix_epoch __comp_const__time_Clock_unix_epoch

/* --- Operations --- */

/// Get the current wall-clock time.
$extern fn_((time_Clock_now(time_Self time))(time_Clock));
/// Get the elapsed duration from a given time until now.
$extern fn_((time_Clock_elapsed(time_Clock self, time_Self time))(time_Dur));
/// Get the duration between two system times (later - earlier).
$extern fn_((time_Clock_durSince(time_Clock later, time_Clock earlier))(time_Dur));
/// Get the duration between two system times with underflow checking.
$extern fn_((time_Clock_durSinceChkd(time_Clock later, time_Clock earlier))(O$time_Dur));

/* --- Arithmetic Operations --- */

/// Add a duration to the time.
$extern op_fn_addWith$(((time_Clock, time_Dur)(lhs, rhs))(time_Clock));
$static op_fn_addWith$(addDur, ((time_Clock, time_Dur)(lhs, rhs))(time_Clock));
/// Add a duration to the time (in-place).
$extern op_fn_addAsgWith$(((time_Clock, time_Dur)(lhs, rhs))(time_Clock*));
$static op_fn_addAsgWith$(addAsgDur, ((time_Clock, time_Dur)(lhs, rhs))(time_Clock*));
/// Subtract a duration from the time.
$extern op_fn_subWith$(((time_Clock, time_Dur)(lhs, rhs))(time_Clock));
$static op_fn_subWith$(subDur, ((time_Clock, time_Dur)(lhs, rhs))(time_Clock));
/// Subtract a duration from the time (in-place).
$extern op_fn_subAsgWith$(((time_Clock, time_Dur)(lhs, rhs))(time_Clock*));
$static op_fn_subAsgWith$(subAsgDur, ((time_Clock, time_Dur)(lhs, rhs))(time_Clock*));
/// Add a duration with overflow checking.
$extern fn_((time_Clock_addChkdDur(time_Clock lhs, time_Dur rhs))(O$time_Clock));
/// Subtract a duration with underflow checking.
$extern fn_((time_Clock_subChkdDur(time_Clock lhs, time_Dur rhs))(O$time_Clock));

/* --- Unix Epoch Conversion --- */

/// Create a SysTime from Unix epoch seconds.
$extern fn_((time_Clock_fromUnixEpoch(u64 secs))(time_Clock));
/// Convert SysTime to Unix epoch seconds.
/// Returns the number of seconds since January 1, 1970 00:00:00 UTC.
$extern fn_((time_Clock_toUnixEpoch(time_Clock self))(u64));
/// Get duration since Unix epoch (for sub-second precision).
$extern fn_((time_Clock_durSinceUnixEpoch(time_Clock self))(time_Dur));

/* --- Comparison --- */

$extern cmp_fn_ord$((time_Clock)(lhs, rhs));
$extern cmp_fn_eq$((time_Clock)(lhs, rhs));
$extern cmp_fn_ne$((time_Clock)(lhs, rhs));
$extern cmp_fn_lt$((time_Clock)(lhs, rhs));
$extern cmp_fn_gt$((time_Clock)(lhs, rhs));
$extern cmp_fn_le$((time_Clock)(lhs, rhs));
$extern cmp_fn_ge$((time_Clock)(lhs, rhs));
$extern cmp_fn_ordCtx$((time_Clock)(lhs, rhs, ctx));
$extern cmp_fn_eqCtx$((time_Clock)(lhs, rhs, ctx));
$extern cmp_fn_neCtx$((time_Clock)(lhs, rhs, ctx));
$extern cmp_fn_ltCtx$((time_Clock)(lhs, rhs, ctx));
$extern cmp_fn_gtCtx$((time_Clock)(lhs, rhs, ctx));
$extern cmp_fn_leCtx$((time_Clock)(lhs, rhs, ctx));
$extern cmp_fn_geCtx$((time_Clock)(lhs, rhs, ctx));

$extern cmp_fn_eql$((time_Clock)(lhs, rhs));
$extern cmp_fn_neq$((time_Clock)(lhs, rhs));
$extern cmp_fn_eqlCtx$((time_Clock)(lhs, rhs, ctx));
$extern cmp_fn_neqCtx$((time_Clock)(lhs, rhs, ctx));

/*========== Macros and Definitions =========================================*/

/// 100-nanosecond intervals per second.
#define __comp_const__time_Clock_intervals_per_sec \
    (n$(u64)(10, 000, 000ull))

/// Number of 100-nanosecond intervals from 1601-01-01 to 1970-01-01.
/// = 11644473600 seconds * 10,000,000 intervals/second
#define __comp_const__time_Clock_intervals_to_unix_epoch \
    (n$(u64)(116, 444, 736, 000, 000, 000ull))

#if plat_is_windows
#define __comp_const__time_Clock_unix_epoch l$((time_Clock){ \
    .impl = { \
        .dwLowDateTime = as$(DWORD)(time_Clock_intervals_to_unix_epoch & 0xFFFFFFFFull), \
        .dwHighDateTime = as$(DWORD)(time_Clock_intervals_to_unix_epoch >> 32ull), \
    }, \
})
#else /* plat_based_unix */
#define __comp_const__time_Clock_unix_epoch l$((time_Clock){ \
    .impl = { .tv_sec = 0, .tv_nsec = 0 }, \
})
#endif

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* time_Clock__included */

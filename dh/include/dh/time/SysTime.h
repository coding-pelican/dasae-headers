/**
 * @copyright Copyright (c) 2024-2026 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    SysTime.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2024-11-10 (date of creation)
 * @updated 2026-01-02 (date of last update)
 * @ingroup dasae-headers(dh)/time
 * @prefix  time_SysTime
 *
 * @brief   Wall-clock time utilities
 * @details Provides functionality for wall-clock time operations:
 *          - Current wall-clock time retrieval
 *          - Unix epoch conversion (to/from)
 *          - Duration calculations between system times
 *          - Platform-independent wall-clock operations
 *
 * @note    Unlike time_Instant which uses a monotonic clock,
 *          time_SysTime uses the system's wall clock and can
 *          go backwards (e.g., due to NTP synchronization).
 *          Use time_Instant for measuring elapsed time.
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

/// Number of 100-nanosecond intervals per second (Windows FILETIME unit).
#define time_SysTime_intervals_per_sec __comp_const__time_SysTime_intervals_per_sec
/// Number of 100-nanosecond intervals from Windows epoch (1601) to Unix epoch (1970).
#define time_SysTime_intervals_to_unix_epoch __comp_const__time_SysTime_intervals_to_unix_epoch
/// The Unix epoch (January 1, 1970 00:00:00 UTC) as a SysTime.
#define time_SysTime_UNIX_EPOCH __comp_const__time_SysTime_UNIX_EPOCH

/* --- Operations --- */

/// Get the current wall-clock time.
$extern fn_((time_SysTime_now(void))(time_SysTime));
/// Get the elapsed duration from a given time until now.
$extern fn_((time_SysTime_elapsed(time_SysTime self))(time_Duration));
/// Get the duration between two system times (later - earlier).
$extern fn_((time_SysTime_durationSince(time_SysTime later, time_SysTime earlier))(time_Duration));
/// Get the duration between two system times with underflow checking.
$extern fn_((time_SysTime_durationSinceChkd(time_SysTime later, time_SysTime earlier))(O$time_Duration));

/* --- Arithmetic Operations --- */

/// Add a duration to the time.
$extern op_fn_addWith$(((time_SysTime, time_Duration)(lhs, rhs))(time_SysTime));
$static op_fn_addWith$(addDuration, ((time_SysTime, time_Duration)(lhs, rhs))(time_SysTime));
/// Add a duration to the time (in-place).
$extern op_fn_addAsgWith$(((time_SysTime, time_Duration)(lhs, rhs))(time_SysTime*));
$static op_fn_addAsgWith$(addAsgDuration, ((time_SysTime, time_Duration)(lhs, rhs))(time_SysTime*));
/// Subtract a duration from the time.
$extern op_fn_subWith$(((time_SysTime, time_Duration)(lhs, rhs))(time_SysTime));
$static op_fn_subWith$(subDuration, ((time_SysTime, time_Duration)(lhs, rhs))(time_SysTime));
/// Subtract a duration from the time (in-place).
$extern op_fn_subAsgWith$(((time_SysTime, time_Duration)(lhs, rhs))(time_SysTime*));
$static op_fn_subAsgWith$(subAsgDuration, ((time_SysTime, time_Duration)(lhs, rhs))(time_SysTime*));
/// Add a duration with overflow checking.
$extern fn_((time_SysTime_addChkdDuration(time_SysTime lhs, time_Duration rhs))(O$time_SysTime));
/// Subtract a duration with underflow checking.
$extern fn_((time_SysTime_subChkdDuration(time_SysTime lhs, time_Duration rhs))(O$time_SysTime));

/* --- Unix Epoch Conversion --- */

/// Create a SysTime from Unix epoch seconds.
$extern fn_((time_SysTime_fromUnixEpoch(u64 secs))(time_SysTime));
/// Convert SysTime to Unix epoch seconds.
/// Returns the number of seconds since January 1, 1970 00:00:00 UTC.
$extern fn_((time_SysTime_toUnixEpoch(time_SysTime self))(u64));
/// Get duration since Unix epoch (for sub-second precision).
$extern fn_((time_SysTime_durationSinceUnixEpoch(time_SysTime self))(time_Duration));

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

/// 100-nanosecond intervals per second.
#define __comp_const__time_SysTime_intervals_per_sec \
    (lit_n$(u64)(10, 000, 000ull))

/// Number of 100-nanosecond intervals from 1601-01-01 to 1970-01-01.
/// = 11644473600 seconds * 10,000,000 intervals/second
#define __comp_const__time_SysTime_intervals_to_unix_epoch \
    (lit_n$(u64)(116, 444, 736, 000, 000, 000ull))

#if plat_is_windows && (arch_bits_is_32bit || arch_bits_is_64bit)
#define __comp_const__time_SysTime_UNIX_EPOCH lit$((time_SysTime){ \
    .impl = { \
        .dwLowDateTime = as$(DWORD)(time_SysTime_intervals_to_unix_epoch & 0xFFFFFFFFull), \
        .dwHighDateTime = as$(DWORD)(time_SysTime_intervals_to_unix_epoch >> 32ull), \
    }, \
})
#else /* plat_based_unix && (plat_is_linux || plat_is_darwin) */
#define __comp_const__time_SysTime_UNIX_EPOCH lit$((time_SysTime){ \
    .impl = { .tv_sec = 0, .tv_nsec = 0 }, \
})
#endif

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* time_SysTime__included */

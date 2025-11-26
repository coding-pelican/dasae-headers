/**
 * @copyright Copyright (c) 2024-2025 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    Instant.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2024-11-10 (date of creation)
 * @updated 2024-02-08 (date of last update)
 * @version v0.1-alpha.2
 * @ingroup dasae-headers(dh)/time
 * @prefix  time_Instant
 *
 * @brief   Time instant utilities
 * @details Provides functionality for:
 *          - Time measurement and duration tracking
 *          - High-precision timestamps and intervals
 *          - Time formatting and parsing
 *          - Platform-independent time operations
 */

#ifndef time_Instant__included
#define time_Instant__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "cfg.h"
#include "common.h"
#include "SysTime.h"

/*========== Macros and Declarations ========================================*/

/* --- Structures --- */
struct time_Instant {
    time_SysTime point;
};
T_impl_O$(time_Instant);

/* --- Constants --- */
#define time_Instant_unix_epoch __comp_const__time_Instant_unix_epoch

/* --- Operations --- */
$extern fn_((time_Instant_now(void))(time_Instant));
$extern fn_((time_Instant_elapsed(time_Instant self))(time_Duration));
$extern fn_((time_Instant_durationSince(time_Instant later, time_Instant earlier))(time_Duration));
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

/* --- Time Conversion to/from Unix Epoch --- */
$extern fn_((time_Instant_fromUnixEpoch(u64 secs))(time_Instant));
$extern fn_((time_Instant_toUnixEpoch(time_Instant self))(u64));

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

#define __comp_const__time_Instant_unix_epoch lit$((time_Instant){ .point = time_SysTime_unix_epoch })

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* time_Instant__included */

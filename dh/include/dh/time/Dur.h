/**
 * @copyright Copyright (c) 2024-2026 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    Duration.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2024-11-10 (date of creation)
 * @updated 2026-01-03 (date of last update)
 * @ingroup dasae-headers(dh)/time
 * @prefix  time_Dur
 *
 * @brief   Time duration utilities
 * @details Provides functionality for:
 *          - Time duration operations
 *          - Timestamp and interval measurements
 *          - Time formatting and parsing
 *          - Platform-independent time operations
 */
#ifndef time_Dur__included
#define time_Dur__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "common.h"

/*========== Macros and Declarations ========================================*/

/* --- Structures --- */
struct time_Dur {
    u64 secs;
    u32 nanos;
};
T_impl_O$(time_Dur);

/* --- Constants --- */
#define time_Dur_zero __comp_const__time_Dur_zero
#define time_Dur_sec __comp_const__time_Dur_sec
#define time_Dur_milli __comp_const__time_Dur_milli
#define time_Dur_micro __comp_const__time_Dur_micro
#define time_Dur_nano __comp_const__time_Dur_nano

/* --- Constructors --- */
#define time_Dur_from_static(_secs, _nanos) __comp_fn__time_Dur_from_static(_secs, _nanos)
#define time_Dur_fromSecs_static(_secs) __comp_fn__time_Dur_fromSecs_static(_secs)
#define time_Dur_fromSecs$f64_static(_secs) __comp_fn__time_Dur_fromSecs$f64_static(_secs)
#define time_Dur_fromSecs$f32_static(_secs) __comp_fn__time_Dur_fromSecs$f32_static(_secs)
#define time_Dur_fromMillis_static(_millis) __comp_fn__time_Dur_fromMillis_static(_millis)
#define time_Dur_fromMicros_static(_micros) __comp_fn__time_Dur_fromMicros_static(_micros)
#define time_Dur_fromNanos_static(_nanos) __comp_fn__time_Dur_fromNanos_static(_nanos)

$extern fn_((time_Dur_from(u64 secs, u32 nanos))(time_Dur));
$extern fn_((time_Dur_fromSecs(u64 secs))(time_Dur));
$extern fn_((time_Dur_fromSecs$f64(f64 secs))(time_Dur));
$extern fn_((time_Dur_fromSecs$f32(f32 secs))(time_Dur));
$extern fn_((time_Dur_fromMillis(u64 millis))(time_Dur));
$extern fn_((time_Dur_fromMicros(u64 micros))(time_Dur));
$extern fn_((time_Dur_fromNanos(u64 nanos))(time_Dur));

/* --- Accessors --- */
$extern fn_((time_Dur_asSecs(time_Dur self))(u64));
$extern fn_((time_Dur_asSecs$f64(time_Dur self))(f64));
$extern fn_((time_Dur_asSecs$f32(time_Dur self))(f32));
$extern fn_((time_Dur_subsecMillis(time_Dur self))(u32));
$extern fn_((time_Dur_subsecMicros(time_Dur self))(u32));
$extern fn_((time_Dur_subsecNanos(time_Dur self))(u32));

/* --- Validation --- */
$extern fn_((time_Dur_isZero(time_Dur self))(bool));

/* --- Arithmetic --- */
$extern op_fn_add$(((time_Dur)(lhs, rhs))(time_Dur));
$extern op_fn_addAsg$(((time_Dur)(lhs, rhs))(time_Dur*));
$extern op_fn_sub$(((time_Dur)(lhs, rhs))(time_Dur));
$extern op_fn_subAsg$(((time_Dur)(lhs, rhs))(time_Dur*));
$extern op_fn_mulWith$(((time_Dur, u32)(lhs, rhs))(time_Dur));
$extern op_fn_mulAsgWith$(((time_Dur, u32)(lhs, rhs))(time_Dur*));
$extern op_fn_divWith$(((time_Dur, u32)(lhs, rhs))(time_Dur));
$extern op_fn_divAsgWith$(((time_Dur, u32)(lhs, rhs))(time_Dur*));
$extern fn_((time_Dur_addChkd(time_Dur lhs, time_Dur rhs))(O$time_Dur));
$extern fn_((time_Dur_subChkd(time_Dur lhs, time_Dur rhs))(O$time_Dur));
$extern fn_((time_Dur_mulChkd$u32(time_Dur lhs, u32 rhs))(O$time_Dur));
$extern fn_((time_Dur_divChkd$u32(time_Dur lhs, u32 rhs))(O$time_Dur));
$extern fn_((time_Dur_addSat(time_Dur lhs, time_Dur rhs))(time_Dur));
$extern fn_((time_Dur_subSat(time_Dur lhs, time_Dur rhs))(time_Dur));
$extern fn_((time_Dur_mulSat$u32(time_Dur lhs, u32 rhs))(time_Dur));

/* --- Comparison --- */
$attr($inline_always)
$static cmp_fn_ord$((time_Dur)(lhs, rhs));
$attr($inline_always)
$static cmp_fn_eq$((time_Dur)(lhs, rhs));
$attr($inline_always)
$static cmp_fn_ne$((time_Dur)(lhs, rhs));
$attr($inline_always)
$static cmp_fn_lt$((time_Dur)(lhs, rhs));
$attr($inline_always)
$static cmp_fn_gt$((time_Dur)(lhs, rhs));
$attr($inline_always)
$static cmp_fn_le$((time_Dur)(lhs, rhs));
$attr($inline_always)
$static cmp_fn_ge$((time_Dur)(lhs, rhs));

$attr($inline_always)
$static cmp_fn_ordCtx$((time_Dur)(lhs, rhs, ctx));
$attr($inline_always)
$static cmp_fn_eqCtx$((time_Dur)(lhs, rhs, ctx));
$attr($inline_always)
$static cmp_fn_neCtx$((time_Dur)(lhs, rhs, ctx));
$attr($inline_always)
$static cmp_fn_ltCtx$((time_Dur)(lhs, rhs, ctx));
$attr($inline_always)
$static cmp_fn_gtCtx$((time_Dur)(lhs, rhs, ctx));
$attr($inline_always)
$static cmp_fn_leCtx$((time_Dur)(lhs, rhs, ctx));
$attr($inline_always)
$static cmp_fn_geCtx$((time_Dur)(lhs, rhs, ctx));

$attr($inline_always)
$static cmp_fn_eql$((time_Dur)(lhs, rhs));
$attr($inline_always)
$static cmp_fn_neq$((time_Dur)(lhs, rhs));
$attr($inline_always)
$static cmp_fn_eqlCtx$((time_Dur)(lhs, rhs, ctx));
$attr($inline_always)
$static cmp_fn_neqCtx$((time_Dur)(lhs, rhs, ctx));

/*========== Macros and Definitions =========================================*/

#define __comp_const__time_Dur_zero time_Dur_fromNanos_static(0)
#define __comp_const__time_Dur_sec time_Dur_fromSecs_static(1)
#define __comp_const__time_Dur_milli time_Dur_fromMillis_static(1)
#define __comp_const__time_Dur_micro time_Dur_fromMicros_static(1)
#define __comp_const__time_Dur_nano time_Dur_fromNanos_static(1)

#define __comp_fn__time_Dur_from_static(_secs, _nanos) l$((time_Dur){ \
    .secs = (_secs), \
    .nanos = (_nanos), \
})
#define __comp_fn__time_Dur_fromSecs_static(_secs) l$((time_Dur){ \
    .secs = (_secs) == 0 ? 0 : (_secs), \
    .nanos = 0, \
})
#define __comp_fn__time_Dur_fromSecs$f64_static(_secs) l$((time_Dur){ \
    .secs = as$(u64)(_secs), \
    .nanos = as$(u32)(((_secs) - as$(f64)(as$(u64)(_secs))) * as$(f64)(time_nanos_per_sec)), \
})
#define __comp_fn__time_Dur_fromSecs$f32_static(_secs) l$((time_Dur){ \
    .secs = as$(u64)(_secs), \
    .nanos = as$(u32)(((_secs) - as$(f32)(as$(u64)(_secs))) * as$(f32)(time_nanos_per_sec)), \
})
#define __comp_fn__time_Dur_fromMillis_static(_millis) l$((time_Dur){ \
    .secs = (_millis) == 0 ? 0 : (_millis) / time_millis_per_sec, \
    .nanos = (_millis) == 0 ? 0 : ((_millis) % time_millis_per_sec) * time_nanos_per_milli, \
})
#define __comp_fn__time_Dur_fromMicros_static(_micros) l$((time_Dur){ \
    .secs = (_micros) == 0 ? 0 : (_micros) / time_micros_per_sec, \
    .nanos = (_micros) == 0 ? 0 : ((_micros) % time_micros_per_sec) * time_nanos_per_micro, \
})
#define __comp_fn__time_Dur_fromNanos_static(_nanos) l$((time_Dur){ \
    .secs = (_nanos) == 0 ? 0 : (_nanos) / time_nanos_per_sec, \
    .nanos = (_nanos) == 0 ? 0 : (_nanos) % time_nanos_per_sec, \
})

$attr($inline_always)
$static cmp_fn_ord$((time_Dur)(lhs, rhs)) {
    if (lhs.secs < rhs.secs) { return cmp_Ord_lt; }
    if (lhs.secs > rhs.secs) { return cmp_Ord_gt; }
    if (lhs.nanos < rhs.nanos) { return cmp_Ord_lt; }
    if (lhs.nanos > rhs.nanos) { return cmp_Ord_gt; }
    return cmp_Ord_eq;
};
$attr($inline_always)
$static cmp_fn_eq_default$((time_Dur)(lhs, rhs));
$attr($inline_always)
$static cmp_fn_ne_default$((time_Dur)(lhs, rhs));
$attr($inline_always)
$static cmp_fn_lt_default$((time_Dur)(lhs, rhs));
$attr($inline_always)
$static cmp_fn_gt_default$((time_Dur)(lhs, rhs));
$attr($inline_always)
$static cmp_fn_le_default$((time_Dur)(lhs, rhs));
$attr($inline_always)
$static cmp_fn_ge_default$((time_Dur)(lhs, rhs));
$attr($inline_always)
$static cmp_fn_ordCtx$((time_Dur)(lhs, rhs, ctx)) {
    let_ignore = ctx;
    return cmp_ord$(time_Dur)(lhs, rhs);
};
$attr($inline_always)
$static cmp_fn_eqCtx_default$((time_Dur)(lhs, rhs, ctx));
$attr($inline_always)
$static cmp_fn_neCtx_default$((time_Dur)(lhs, rhs, ctx));
$attr($inline_always)
$static cmp_fn_ltCtx_default$((time_Dur)(lhs, rhs, ctx));
$attr($inline_always)
$static cmp_fn_gtCtx_default$((time_Dur)(lhs, rhs, ctx));
$attr($inline_always)
$static cmp_fn_leCtx_default$((time_Dur)(lhs, rhs, ctx));
$attr($inline_always)
$static cmp_fn_geCtx_default$((time_Dur)(lhs, rhs, ctx));

$attr($inline_always)
$static cmp_fn_eql$((time_Dur)(lhs, rhs)) {
    return cmp_ord$(time_Dur)(lhs, rhs) == cmp_Ord_eq;
};
$attr($inline_always)
$static cmp_fn_neq_default$((time_Dur)(lhs, rhs));
$attr($inline_always)
$static cmp_fn_eqlCtx$((time_Dur)(lhs, rhs, ctx)) {
    let_ignore = ctx;
    return cmp_eql$(time_Dur)(lhs, rhs);
};
$attr($inline_always)
$static cmp_fn_neqCtx_default$((time_Dur)(lhs, rhs, ctx));

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* time_Dur__included */

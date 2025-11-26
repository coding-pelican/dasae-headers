/**
 * @copyright Copyright (c) 2024-2025 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    Duration.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2024-11-10 (date of creation)
 * @updated 2025-02-08 (date of last update)
 * @version v0.1-alpha.1
 * @ingroup dasae-headers(dh)/time
 * @prefix  time_Duration
 *
 * @brief   Time duration utilities
 * @details Provides functionality for:
 *          - Time duration operations
 *          - Timestamp and interval measurements
 *          - Time formatting and parsing
 *          - Platform-independent time operations
 */

#ifndef time_Duration__included
#define time_Duration__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "cfg.h"
#include "common.h"

/*========== Macros and Declarations ========================================*/

/* --- Structures --- */
struct time_Duration {
    u64 secs;
    u32 nanos;
};
T_impl_O$(time_Duration);

/* --- Constants --- */
#define time_Duration_zero __comp_const__time_Duration_zero
#define time_Duration_sec __comp_const__time_Duration_sec
#define time_Duration_milli __comp_const__time_Duration_milli
#define time_Duration_micro __comp_const__time_Duration_micro
#define time_Duration_nano __comp_const__time_Duration_nano

/* --- Constructors --- */
#define time_Duration_from_static(_secs, _nanos) __comp_fn__time_Duration_from_static(_secs, _nanos)
#define time_Duration_fromSecs_static(_secs) __comp_fn__time_Duration_fromSecs_static(_secs)
#define time_Duration_fromSecs$f64_static(_secs) __comp_fn__time_Duration_fromSecs$f64_static(_secs)
#define time_Duration_fromMillis_static(_millis) __comp_fn__time_Duration_fromMillis_static(_millis)
#define time_Duration_fromMicros_static(_micros) __comp_fn__time_Duration_fromMicros_static(_micros)
#define time_Duration_fromNanos_static(_nanos) __comp_fn__time_Duration_fromNanos_static(_nanos)

$extern fn_((time_Duration_from(u64 secs, u32 nanos))(time_Duration));
$extern fn_((time_Duration_fromSecs(u64 secs))(time_Duration));
$extern fn_((time_Duration_fromSecs$f64(f64 secs))(time_Duration));
$extern fn_((time_Duration_fromMillis(u64 millis))(time_Duration));
$extern fn_((time_Duration_fromMicros(u64 micros))(time_Duration));
$extern fn_((time_Duration_fromNanos(u64 nanos))(time_Duration));

/* --- Accessors --- */
$extern fn_((time_Duration_asSecs(time_Duration self))(u64));
$extern fn_((time_Duration_asSecs$f64(time_Duration self))(f64));
$extern fn_((time_Duration_subsecMillis(time_Duration self))(u32));
$extern fn_((time_Duration_subsecMicros(time_Duration self))(u32));
$extern fn_((time_Duration_subsecNanos(time_Duration self))(u32));

/* --- Validation --- */
$extern fn_((time_Duration_isZero(time_Duration self))(bool));

/* --- Arithmetic --- */
$extern op_fn_add$(((time_Duration)(lhs, rhs))(time_Duration));
$extern op_fn_addAsg$(((time_Duration)(lhs, rhs))(time_Duration*));
$extern op_fn_sub$(((time_Duration)(lhs, rhs))(time_Duration));
$extern op_fn_subAsg$(((time_Duration)(lhs, rhs))(time_Duration*));
$extern op_fn_mulWith$(((time_Duration, u64)(lhs, rhs))(time_Duration));
$extern op_fn_mulAsgWith$(((time_Duration, u64)(lhs, rhs))(time_Duration*));
$extern op_fn_divWith$(((time_Duration, u64)(lhs, rhs))(time_Duration));
$extern op_fn_divAsgWith$(((time_Duration, u64)(lhs, rhs))(time_Duration*));
$extern fn_((time_Duration_addChkd(time_Duration lhs, time_Duration rhs))(O$time_Duration));
$extern fn_((time_Duration_subChkd(time_Duration lhs, time_Duration rhs))(O$time_Duration));
$extern fn_((time_Duration_mulChkd$u64(time_Duration lhs, u64 rhs))(O$time_Duration));
$extern fn_((time_Duration_divChkd$u64(time_Duration lhs, u64 rhs))(O$time_Duration));

/* --- Comparison --- */
$inline_always
$static cmp_fn_ord$((time_Duration)(lhs, rhs));
$inline_always
$static cmp_fn_eq$((time_Duration)(lhs, rhs));
$inline_always
$static cmp_fn_ne$((time_Duration)(lhs, rhs));
$inline_always
$static cmp_fn_lt$((time_Duration)(lhs, rhs));
$inline_always
$static cmp_fn_gt$((time_Duration)(lhs, rhs));
$inline_always
$static cmp_fn_le$((time_Duration)(lhs, rhs));
$inline_always
$static cmp_fn_ge$((time_Duration)(lhs, rhs));

$inline_always
$static cmp_fn_ordCtx$((time_Duration)(lhs, rhs, ctx));
$inline_always
$static cmp_fn_eqCtx$((time_Duration)(lhs, rhs, ctx));
$inline_always
$static cmp_fn_neCtx$((time_Duration)(lhs, rhs, ctx));
$inline_always
$static cmp_fn_ltCtx$((time_Duration)(lhs, rhs, ctx));
$inline_always
$static cmp_fn_gtCtx$((time_Duration)(lhs, rhs, ctx));
$inline_always
$static cmp_fn_leCtx$((time_Duration)(lhs, rhs, ctx));
$inline_always
$static cmp_fn_geCtx$((time_Duration)(lhs, rhs, ctx));

$inline_always
$static cmp_fn_eql$((time_Duration)(lhs, rhs));
$inline_always
$static cmp_fn_neq$((time_Duration)(lhs, rhs));
$inline_always
$static cmp_fn_eqlCtx$((time_Duration)(lhs, rhs, ctx));
$inline_always
$static cmp_fn_neqCtx$((time_Duration)(lhs, rhs, ctx));

/*========== Macros and Implementations =====================================*/

#define __comp_const__time_Duration_zero time_Duration_fromNanos_static(0)
#define __comp_const__time_Duration_sec time_Duration_fromSecs_static(1)
#define __comp_const__time_Duration_milli time_Duration_fromMillis_static(1)
#define __comp_const__time_Duration_micro time_Duration_fromMicros_static(1)
#define __comp_const__time_Duration_nano time_Duration_fromNanos_static(1)

#define __comp_fn__time_Duration_from_static(_secs, _nanos) lit$((time_Duration){ \
    .secs = (_secs), \
    .nanos = (_nanos), \
})
#define __comp_fn__time_Duration_fromSecs_static(_secs) lit$((time_Duration){ \
    .secs = (_secs) == 0 ? 0 : (_secs), \
    .nanos = 0, \
})
#define __comp_fn__time_Duration_fromSecs$f64_static(_secs) lit$((time_Duration){ \
    .secs = as$(u64)(_secs), \
    .nanos = as$(u32)(((_secs) - as$(f64)(as$(u64)(_secs))) * as$(f64)(time_nanos_per_sec)), \
})
#define __comp_fn__time_Duration_fromMillis_static(_millis) lit$((time_Duration){ \
    .secs = (_millis) == 0 ? 0 : (_millis) / time_millis_per_sec, \
    .nanos = (_millis) == 0 ? 0 : ((_millis) % time_millis_per_sec) * time_nanos_per_milli, \
})
#define __comp_fn__time_Duration_fromMicros_static(_micros) lit$((time_Duration){ \
    .secs = (_micros) == 0 ? 0 : (_micros) / time_micros_per_sec, \
    .nanos = (_micros) == 0 ? 0 : ((_micros) % time_micros_per_sec) * time_nanos_per_micro, \
})
#define __comp_fn__time_Duration_fromNanos_static(_nanos) lit$((time_Duration){ \
    .secs = (_nanos) == 0 ? 0 : (_nanos) / time_nanos_per_sec, \
    .nanos = (_nanos) == 0 ? 0 : (_nanos) % time_nanos_per_sec, \
})

$inline_always
$static cmp_fn_ord$((time_Duration)(lhs, rhs)) {
    if (lhs.secs < rhs.secs) { return cmp_Ord_lt; }
    if (lhs.secs > rhs.secs) { return cmp_Ord_gt; }
    if (lhs.nanos < rhs.nanos) { return cmp_Ord_lt; }
    if (lhs.nanos > rhs.nanos) { return cmp_Ord_gt; }
    return cmp_Ord_eq;
};
$inline_always
$static cmp_fn_eq_default$((time_Duration)(lhs, rhs));
$inline_always
$static cmp_fn_ne_default$((time_Duration)(lhs, rhs));
$inline_always
$static cmp_fn_lt_default$((time_Duration)(lhs, rhs));
$inline_always
$static cmp_fn_gt_default$((time_Duration)(lhs, rhs));
$inline_always
$static cmp_fn_le_default$((time_Duration)(lhs, rhs));
$inline_always
$static cmp_fn_ge_default$((time_Duration)(lhs, rhs));
$inline_always
$static cmp_fn_ordCtx$((time_Duration)(lhs, rhs, ctx)) {
    let_ignore = ctx;
    return cmp_ord$(time_Duration)(lhs, rhs);
}
$inline_always
$static cmp_fn_eqCtx_default$((time_Duration)(lhs, rhs, ctx));
$inline_always
$static cmp_fn_neCtx_default$((time_Duration)(lhs, rhs, ctx));
$inline_always
$static cmp_fn_ltCtx_default$((time_Duration)(lhs, rhs, ctx));
$inline_always
$static cmp_fn_gtCtx_default$((time_Duration)(lhs, rhs, ctx));
$inline_always
$static cmp_fn_leCtx_default$((time_Duration)(lhs, rhs, ctx));
$inline_always
$static cmp_fn_geCtx_default$((time_Duration)(lhs, rhs, ctx));

$inline_always
$static cmp_fn_eql$((time_Duration)(lhs, rhs)) {
    return cmp_ord$(time_Duration)(lhs, rhs) == cmp_Ord_eq;
}
$inline_always
$static cmp_fn_neq_default$((time_Duration)(lhs, rhs));
$inline_always
$static cmp_fn_eqlCtx$((time_Duration)(lhs, rhs, ctx)) {
    let_ignore = ctx;
    return cmp_eql$(time_Duration)(lhs, rhs);
}
$inline_always
$static cmp_fn_neqCtx_default$((time_Duration)(lhs, rhs, ctx));

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* time_Duration__included */

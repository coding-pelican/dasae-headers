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

#ifndef TIME_DURATION_INCLUDED
#define TIME_DURATION_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "cfg.h"
#include "common.h"

/*========== Macros and Definitions =========================================*/

struct time_Duration {
    u64 secs;
    u32 nanos;
};
T_impl_O$(time_Duration);

/* Constructors */
extern time_Duration time_Duration_from(u64 secs, u32 nanos);

extern time_Duration time_Duration_fromSecs(u64 secs);
extern time_Duration time_Duration_fromMillis(u64 millis);
extern time_Duration time_Duration_fromMicros(u64 micros);
extern time_Duration time_Duration_fromNanos(u64 nanos);

/* Accessors */
extern u64 time_Duration_asSecs(time_Duration self);
extern u32 time_Duration_subsecMillis(time_Duration self);
extern u32 time_Duration_subsecMicros(time_Duration self);
extern u32 time_Duration_subsecNanos(time_Duration self);

/* Validation */
extern bool time_Duration_isZero(time_Duration self);

/* Conversion */
extern time_Duration time_Duration_fromSecs$f64(f64 secs);
extern f64           time_Duration_asSecs$f64(time_Duration self);

/* Arithmetic */
extern time_Duration op_fnAdd(time_Duration);
extern time_Duration op_fnAddAsg(time_Duration);
extern time_Duration op_fnSub(time_Duration);
extern time_Duration op_fnSubAsg(time_Duration);
extern time_Duration op_fnMulBy(time_Duration, u64);
extern time_Duration op_fnMulAsgBy(time_Duration, u64);
extern time_Duration op_fnDivBy(time_Duration, u64);
extern time_Duration op_fnDivAsgBy(time_Duration, u64);

extern O$time_Duration time_Duration_addChkd(time_Duration lhs, time_Duration rhs);
extern O$time_Duration time_Duration_subChkd(time_Duration lhs, time_Duration rhs);
extern O$time_Duration time_Duration_mulChkd$u64(time_Duration lhs, u64 rhs);
extern O$time_Duration time_Duration_divChkd$u64(time_Duration lhs, u64 rhs);

/* Comparison */
$inline_always cmp_fnCmp(time_Duration) {
    if (self.secs < other.secs) { return cmp_Ord_lt; }
    if (self.secs > other.secs) { return cmp_Ord_gt; }
    if (self.nanos < other.nanos) { return cmp_Ord_lt; }
    if (self.nanos > other.nanos) { return cmp_Ord_gt; }
    return cmp_Ord_eq;
}
cmp_fnEq_default(time_Duration);
cmp_fnNe_default(time_Duration);
cmp_fnLt_default(time_Duration);
cmp_fnGt_default(time_Duration);
cmp_fnLe_default(time_Duration);
cmp_fnGe_default(time_Duration);

/* Literal */
#define literal_time_Duration_from(_secs, _nanos) \
    lit$((time_Duration){ \
        .secs  = (_secs), \
        .nanos = (_nanos), \
    })
#define literal_time_Duration_fromSecs(_secs) \
    lit$((time_Duration){ \
        .secs  = (_secs) == 0 ? 0 : (_secs), \
        .nanos = 0, \
    })
#define literal_time_Duration_fromMillis(_millis) \
    lit$((time_Duration){ \
        .secs  = (_millis) == 0 ? 0 : (_millis) / time_millis_per_sec, \
        .nanos = (_millis) == 0 ? 0 : ((_millis) % time_millis_per_sec) * time_nanos_per_milli, \
    })
#define literal_time_Duration_fromMicros(_micros) \
    lit$((time_Duration){ \
        .secs  = (_micros) == 0 ? 0 : (_micros) / time_micros_per_sec, \
        .nanos = (_micros) == 0 ? 0 : ((_micros) % time_micros_per_sec) * time_nanos_per_micro, \
    })
#define literal_time_Duration_fromNanos(_nanos) \
    lit$((time_Duration){ \
        .secs  = (_nanos) == 0 ? 0 : (_nanos) / time_nanos_per_sec, \
        .nanos = (_nanos) == 0 ? 0 : (_nanos) % time_nanos_per_sec, \
    })

/* Constants */
static const time_Duration time_Duration_zero   = literal_time_Duration_fromNanos(0);
static const time_Duration time_Duration_secs   = literal_time_Duration_fromSecs(1);
static const time_Duration time_Duration_millis = literal_time_Duration_fromMillis(1);
static const time_Duration time_Duration_micros = literal_time_Duration_fromMicros(1);
static const time_Duration time_Duration_nanos  = literal_time_Duration_fromNanos(1);

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* TIME_DURATION_INCLUDED */

/**
 * @copyright Copyright 2024. Gyeongtae Kim All rights reserved.
 *
 * @file    Duration.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2024-11-10 (date of creation)
 * @updated 2024-12-23 (date of last update)
 * @version v0.1-alpha
 * @ingroup dasae-headers(dh)/time
 * @prefix  time_Duration
 *
 * @brief   Source of some software
 * @details Some detailed explanation
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
    u64 secs_;
    u32 nanos_;
};

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
extern time_Duration time_Duration_fromSecs_f64(f64 secs);
extern f64           time_Duration_asSecs_f64(time_Duration self);

/* Arithmetic */
extern time_Duration op_fnAdd(time_Duration);
extern time_Duration op_fnSub(time_Duration);
extern time_Duration op_fnMulBy(time_Duration, u64);
extern time_Duration op_fnDivBy(time_Duration, u64);

/* Comparison */
force_inline cmp_fnCmp(time_Duration) {
    if (self.secs_ < other.secs_) { return cmp_Ord_less; }
    if (self.secs_ > other.secs_) { return cmp_Ord_greater; }
    if (self.nanos_ < other.nanos_) { return cmp_Ord_less; }
    if (self.nanos_ > other.nanos_) { return cmp_Ord_greater; }
    return cmp_Ord_equal;
}
cmp_fnEq_default(time_Duration);
cmp_fnNe_default(time_Duration);
cmp_fnLt_default(time_Duration);
cmp_fnGt_default(time_Duration);
cmp_fnLe_default(time_Duration);
cmp_fnGe_default(time_Duration);

/* Literal */
#define literal_time_Duration_from(_secs, _nanos) \
    make(                                         \
        time_Duration,                            \
        .secs_  = (_secs),                        \
        .nanos_ = (_nanos)                        \
    )
#define literal_time_Duration_fromSecs(_secs) \
    make(                                     \
        time_Duration,                        \
        .secs_  = (_secs) == 0 ? 0 : (_secs), \
        .nanos_ = 0                           \
    )
#define literal_time_Duration_fromMillis(_millis)                                               \
    make(                                                                                       \
        time_Duration,                                                                          \
        .secs_  = (_millis) == 0 ? 0 : (_millis) / time_millis_per_sec,                         \
        .nanos_ = (_millis) == 0 ? 0 : ((_millis) % time_millis_per_sec) * time_nanos_per_milli \
    )
#define literal_time_Duration_fromMicros(_micros)                                               \
    make(                                                                                       \
        time_Duration,                                                                          \
        .secs_  = (_micros) == 0 ? 0 : (_micros) / time_micros_per_sec,                         \
        .nanos_ = (_micros) == 0 ? 0 : ((_micros) % time_micros_per_sec) * time_nanos_per_micro \
    )
#define literal_time_Duration_fromNanos(_nanos)                      \
    make(                                                            \
        time_Duration,                                               \
        .secs_  = (_nanos) == 0 ? 0 : (_nanos) / time_nanos_per_sec, \
        .nanos_ = (_nanos) == 0 ? 0 : (_nanos) % time_nanos_per_sec  \
    )

/* Constants */
static const time_Duration time_Duration_zero        = literal_time_Duration_fromNanos(0);
static const time_Duration time_Duration_second      = literal_time_Duration_fromSecs(1);
static const time_Duration time_Duration_millisecond = literal_time_Duration_fromMillis(1);
static const time_Duration time_Duration_microsecond = literal_time_Duration_fromMicros(1);
static const time_Duration time_Duration_nanosecond  = literal_time_Duration_fromNanos(1);

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* TIME_DURATION_INCLUDED */

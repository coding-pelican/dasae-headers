/**
 * @copyright Copyright 2024. Gyeongtae Kim All rights reserved.
 *
 * @file    duration.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2024-11-10 (date of creation)
 * @updated 2024-11-10 (date of last update)
 * @version v1.0.0
 * @ingroup dasae-headers(dh)/time
 * @prefix  time
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

/*========== Macros and Definitions =========================================*/

struct Duration {
    u64 secs;
    u32 nanos;
};
Duration Duration_from(u64 secs, u32 nanos);
Duration Duration_fromSecs(f64 secs);
Duration Duration_fromMillis(f64 millis);
Duration Duration_fromMicros(f64 micros);
Duration Duration_fromSecs_u64(u64 secs);
Duration Duration_fromMillis_u64(u64 millis);
Duration Duration_fromMicros_u64(u64 micros);
Duration Duration_fromNanos(u64 nanos);
f64      Duration_toSecs(Duration d);
f64      Duration_toMillis(Duration d);
f64      Duration_toMicros(Duration d);
u64      Duration_toNanos(Duration d);

Duration Duration_add(Duration lhs, Duration rhs);
Duration Duration_sub(Duration lhs, Duration rhs);
Duration Duration_mul(Duration d, u64 scalar);
Duration Duration_div(Duration d, u64 scalar);

bool Duration_eq(Duration lhs, Duration rhs);
bool Duration_ne(Duration lhs, Duration rhs);
bool Duration_lt(Duration lhs, Duration rhs);
bool Duration_le(Duration lhs, Duration rhs);
bool Duration_gt(Duration lhs, Duration rhs);
bool Duration_ge(Duration lhs, Duration rhs);
bool Duration_isZero(Duration duration);

#define comptime_Duration_from(_secs, _nanos) \
    makeWith(                                 \
        Duration,                             \
        .secs  = (_secs),                     \
        .nanos = (_nanos)                     \
    )
#define comptime_Duration_fromSecs(_secs)    \
    makeWith(                                \
        Duration,                            \
        .secs  = (_secs) == 0 ? 0 : (_secs), \
        .nanos = 0                           \
    )
#define comptime_Duration_fromMillis(_millis)                                                  \
    makeWith(                                                                                  \
        Duration,                                                                              \
        .secs  = (_millis) == 0 ? 0 : (_millis) / time_millis_per_sec,                         \
        .nanos = (_millis) == 0 ? 0 : ((_millis) % time_millis_per_sec) * time_nanos_per_milli \
    )
#define comptime_Duration_fromMicros(_micros)                                                  \
    makeWith(                                                                                  \
        Duration,                                                                              \
        .secs  = (_micros) == 0 ? 0 : (_micros) / time_micros_per_sec,                         \
        .nanos = (_micros) == 0 ? 0 : ((_micros) % time_micros_per_sec) * time_nanos_per_micro \
    )
#define comptime_Duration_fromNanos(_nanos)                         \
    makeWith(                                                       \
        Duration,                                                   \
        .secs  = (_nanos) == 0 ? 0 : (_nanos) / time_nanos_per_sec, \
        .nanos = (_nanos) == 0 ? 0 : (_nanos) % time_nanos_per_sec  \
    )

static const Duration Duration_zero        = comptime_Duration_fromNanos(0);
static const Duration Duration_second      = comptime_Duration_fromSecs(1);
static const Duration Duration_millisecond = comptime_Duration_fromMillis(1);
static const Duration Duration_microsecond = comptime_Duration_fromMicros(1);
static const Duration Duration_nanosecond  = comptime_Duration_fromNanos(1);


#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* TIME_DURATION_INCLUDED */

/**
 * @copyright Copyright 2024. Gyeongtae Kim All rights reserved.
 *
 * @file    duration.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2024-11-10 (date of creation)
 * @updated 2024-11-15 (date of last update)
 * @version v1.0.0
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

/*========== Macros and Definitions =========================================*/

struct time_Duration {
    u64 secs;
    u32 nanos;
};
time_Duration time_Duration_from(u64 secs, u32 nanos);

time_Duration time_Duration_fromSecs(u64 secs);
time_Duration time_Duration_fromMillis(u64 millis);
time_Duration time_Duration_fromMicros(u64 micros);
time_Duration time_Duration_fromNanos(u64 nanos);

u64 time_Duration_asSecs(time_Duration d);
u32 time_Duration_subsecMillis(time_Duration d);
u32 time_Duration_subsecMicros(time_Duration d);
u32 time_Duration_subsecNanos(time_Duration d);

time_Duration time_Duration_fromSecs_f64(f64 secs);
f64           time_Duration_asSecs_f64(time_Duration d);

time_Duration time_Duration_add(time_Duration lhs, time_Duration rhs);
time_Duration time_Duration_sub(time_Duration lhs, time_Duration rhs);
time_Duration time_Duration_mul(time_Duration d, u64 scalar);
time_Duration time_Duration_div(time_Duration d, u64 scalar);

bool time_Duration_eq(time_Duration lhs, time_Duration rhs);
bool time_Duration_ne(time_Duration lhs, time_Duration rhs);
bool time_Duration_lt(time_Duration lhs, time_Duration rhs);
bool time_Duration_le(time_Duration lhs, time_Duration rhs);
bool time_Duration_gt(time_Duration lhs, time_Duration rhs);
bool time_Duration_ge(time_Duration lhs, time_Duration rhs);
bool time_Duration_isZero(time_Duration duration);

#define literal_time_Duration_from(_secs, _nanos) \
    makeWith(                                     \
        time_Duration,                            \
        .secs  = (_secs),                         \
        .nanos = (_nanos)                         \
    )
#define literal_time_Duration_fromSecs(_secs) \
    makeWith(                                 \
        time_Duration,                        \
        .secs  = (_secs) == 0 ? 0 : (_secs),  \
        .nanos = 0                            \
    )
#define literal_time_Duration_fromMillis(_millis)                                              \
    makeWith(                                                                                  \
        time_Duration,                                                                         \
        .secs  = (_millis) == 0 ? 0 : (_millis) / time_millis_per_sec,                         \
        .nanos = (_millis) == 0 ? 0 : ((_millis) % time_millis_per_sec) * time_nanos_per_milli \
    )
#define literal_time_Duration_fromMicros(_micros)                                              \
    makeWith(                                                                                  \
        time_Duration,                                                                         \
        .secs  = (_micros) == 0 ? 0 : (_micros) / time_micros_per_sec,                         \
        .nanos = (_micros) == 0 ? 0 : ((_micros) % time_micros_per_sec) * time_nanos_per_micro \
    )
#define literal_time_Duration_fromNanos(_nanos)                     \
    makeWith(                                                       \
        time_Duration,                                              \
        .secs  = (_nanos) == 0 ? 0 : (_nanos) / time_nanos_per_sec, \
        .nanos = (_nanos) == 0 ? 0 : (_nanos) % time_nanos_per_sec  \
    )

static const time_Duration time_Duration_zero        = literal_time_Duration_fromNanos(0);
static const time_Duration time_Duration_second      = literal_time_Duration_fromSecs(1);
static const time_Duration time_Duration_millisecond = literal_time_Duration_fromMillis(1);
static const time_Duration time_Duration_microsecond = literal_time_Duration_fromMicros(1);
static const time_Duration time_Duration_nanosecond  = literal_time_Duration_fromNanos(1);


#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* TIME_DURATION_INCLUDED */

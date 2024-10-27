/**
 * @copyright Copyright 2024. Gyeongtae Kim All rights reserved.
 *
 * @file    time.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2024-10-23 (date of creation)
 * @updated 2024-10-27 (date of last update)
 * @version v1.0.0
 * @ingroup dasae-header(dh)
 * @prefix  NONE
 *
 * @brief   Source of some software
 * @details Some detailed explanation
 */


#ifndef TIME_INCLUDED
#define TIME_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */


#include "compat/compat_primitive.h"
#include "types.h"


#if defined(_WIN32) || defined(_WIN64)
#  ifndef WIN32_LEAN_AND_MEAN
#    define WIN32_LEAN_AND_MEAN
#  endif
#  ifndef NOMINMAX
#    define NOMINMAX
#  endif
#  include <windows.h>
#else // UNIX
#  include <sys/time.h>
#  include <time.h>
#endif


typedef struct Duration Duration;
typedef struct Instant  Instant;

#if defined(_WIN32) || defined(_WIN64)
typedef LARGE_INTEGER     SystemTimeWindows;
typedef SystemTimeWindows SystemTime;
#else
typedef struct timespec SystemTimeUnix;
typedef SystemTimeUnix  SystemTime;
#endif


static const u32 Time_nanos_per_sec   = 1000u * 1000u * 1000u;
static const u32 Time_nanos_per_milli = 1000u * 1000u;
static const u32 Time_nanos_per_micro = 1000u;
static const u32 Time_millis_per_sec  = 1000u;
static const u32 Time_micros_per_sec  = 1000u * 1000u;

static const u64 Time_secs_per_minute = 60ull;
static const u64 Time_mins_per_hour   = 60ull;
static const u64 Time_hours_per_day   = 24ull;
static const u64 Time_days_per_week   = 7ull;


struct Duration {
    u64 secs;
    u32 nanos;
};
#define Duration_(...) makeWith(Duration, __VA_ARGS__)
Duration Duration_from(u64 secs, u32 nanos);
Duration Duration_fromSecs(u64 secs);
Duration Duration_fromMillis(u64 millis);
Duration Duration_fromMicros(u64 micros);
Duration Duration_fromNanos(u64 nanos);

Duration Duration_add(Duration lhs, Duration rhs);
Duration Duration_sub(Duration lhs, Duration rhs);
Duration Duration_mul(Duration d, u64 scalar);

bool Duration_eq(Duration lhs, Duration rhs);
bool Duration_ne(Duration lhs, Duration rhs);
bool Duration_lt(Duration lhs, Duration rhs);
bool Duration_le(Duration lhs, Duration rhs);
bool Duration_gt(Duration lhs, Duration rhs);
bool Duration_ge(Duration lhs, Duration rhs);
bool Duration_isZero(Duration duration);

#define comptime_Duration_from(_secs, _nanos) \
    Duration_(                                \
            .secs  = (_secs),                 \
            .nanos = (_nanos)                 \
    )
#define comptime_Duration_fromSecs(_secs)        \
    Duration_(                                   \
            .secs  = (_secs) == 0 ? 0 : (_secs), \
            .nanos = 0                           \
    )
#define comptime_Duration_fromMillis(_millis)                                                      \
    Duration_(                                                                                     \
            .secs  = (_millis) == 0 ? 0 : (_millis) / Time_millis_per_sec,                         \
            .nanos = (_millis) == 0 ? 0 : ((_millis) % Time_millis_per_sec) * Time_nanos_per_milli \
    )
#define comptime_Duration_fromMicros(_micros)                                                      \
    Duration_(                                                                                     \
            .secs  = (_micros) == 0 ? 0 : (_micros) / Time_micros_per_sec,                         \
            .nanos = (_micros) == 0 ? 0 : ((_micros) % Time_micros_per_sec) * Time_nanos_per_micro \
    )
#define comptime_Duration_fromNanos(_nanos)                             \
    Duration_(                                                          \
            .secs  = (_nanos) == 0 ? 0 : (_nanos) / Time_nanos_per_sec, \
            .nanos = (_nanos) == 0 ? 0 : (_nanos) % Time_nanos_per_sec  \
    )

static const Duration Duration_zero        = comptime_Duration_fromNanos(0);
static const Duration Duration_second      = comptime_Duration_fromSecs(1);
static const Duration Duration_millisecond = comptime_Duration_fromMillis(1);
static const Duration Duration_microsecond = comptime_Duration_fromMicros(1);
static const Duration Duration_nanosecond  = comptime_Duration_fromNanos(1);


struct Instant {
    SystemTime time_;
};
#define Instant_(...) makeWith(Instant, __VA_ARGS__)
Instant  Instant_now();
Duration Instant_elapsed(Instant start);
Duration Instant_durationSince(Instant start, Instant earlier);

bool Instant_eq(Instant lhs, Instant rhs);
bool Instant_ne(Instant lhs, Instant rhs);
bool Instant_lt(Instant lhs, Instant rhs);
bool Instant_le(Instant lhs, Instant rhs);
bool Instant_gt(Instant lhs, Instant rhs);
bool Instant_ge(Instant lhs, Instant rhs);
bool Instant_isValid(Instant instant);


#if defined(_WIN32) || defined(_WIN64)
static LARGE_INTEGER SystemTime__s_performance_frequency = { 0 };
static bool          SystemTime__s_frequency_initialized = false;
static void          SystemTime__initFrequency();
#else
#endif


#if defined(__cplusplus)
}
#endif /* defined(__cplusplus) */
#endif /* TIME_INCLUDED */

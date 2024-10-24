/**
 * @copyright Copyright 2024. Gyeongtae Kim All rights reserved.
 *
 * @file    time.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2024-10-23 (date of creation)
 * @updated 2024-10-23 (date of last update)
 * @version v1.0.0
 * @ingroup SOME_GROUP
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


#include "primitive_types.h"

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


static const u32 Time_nanosPerSec   = 1000u * 1000u * 1000u;
static const u32 Time_nanosPerMilli = 1000u * 1000u;
static const u32 Time_nanosPerMicro = 1000u;
static const u32 Time_millisPerSec  = 1000u;
static const u32 Time_microsPerSec  = 1000u * 1000u;

static const u64 Time_secsPerMinute = 60ull;
static const u64 Time_minsPerHour   = 60ull;
static const u64 Time_hoursPerDay   = 24ull;
static const u64 Time_daysPerWeek   = 7ull;


struct Duration {
    u64 secs;
    u32 nanos;
};
#define Duration_(...)                 ((Duration){ __VA_ARGS__ })
#define Duration_make_(_secs, _nanos)  ((Duration){ .secs = (_secs), .nanos = (_nanos) })
#define Duration_from_Secs_(_secs)     Duration_make_((_secs), 0)
#define Duration_from_Millis_(_millis) Duration_make_((_millis) / 1000, ((_millis) % 1000) * 1000000)
#define Duration_from_Micros_(_micros) Duration_make_((_micros) / 1000000, ((_micros) % 1000000) * 1000)
#define Duration_from_Nanos_(_nanos)   Duration_make_((_nanos) / 1000000000, (_nanos) % 1000000000)
Duration Duration_make(u64 secs, u32 nanos);
Duration Duration_from_Secs(u64 secs);
Duration Duration_from_Millis(u64 millis);
Duration Duration_from_Micros(u64 micros);
Duration Duration_from_Nanos(u64 nanos);

Duration Duration_add(Duration a, Duration b);
Duration Duration_sub(Duration a, Duration b);
Duration Duration_mul(Duration a, u64 scalar);

bool Duration_eq(Duration a, Duration b);
bool Duration_ne(Duration a, Duration b);
bool Duration_lt(Duration a, Duration b);
bool Duration_le(Duration a, Duration b);
bool Duration_gt(Duration a, Duration b);
bool Duration_ge(Duration a, Duration b);
bool Duration_IsZero(Duration duration);

static const Duration Duration_zero        = Duration_from_Nanos_(0);
static const Duration Duration_second      = Duration_from_Secs_(1);
static const Duration Duration_millisecond = Duration_from_Millis_(1);
static const Duration Duration_microsecond = Duration_from_Micros_(1);
static const Duration Duration_nanosecond  = Duration_from_Nanos_(1);


struct Instant {
    SystemTime time_;
};
#define Instant_(...) ((Instant){ __VA_ARGS__ })
Instant  Instant_Now();
Duration Instant_Elapsed(Instant start);
Duration Instant_DurationSince(Instant start, Instant earlier);

bool Instant_eq(Instant a, Instant b);
bool Instant_ne(Instant a, Instant b);
bool Instant_lt(Instant a, Instant b);
bool Instant_le(Instant a, Instant b);
bool Instant_gt(Instant a, Instant b);
bool Instant_ge(Instant a, Instant b);
bool Instant_IsValid(Instant instant);


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

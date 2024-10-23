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
#else
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
#define Duration_(...)                ((Duration){ __VA_ARGS__ })
#define Duration_new_(_secs, _nanos)  ((Duration){ .secs = (_secs), .nanos = (_nanos) })
#define Duration_fromSecs_(_secs)     Duration_new_((_secs), 0)
#define Duration_fromMillis_(_millis) Duration_new_((_millis) / 1000, ((_millis) % 1000) * 1000000)
#define Duration_fromMicros_(_micros) Duration_new_((_micros) / 1000000, ((_micros) % 1000000) * 1000)
#define Duration_fromNanos_(_nanos)   Duration_new_((_nanos) / 1000000000, (_nanos) % 1000000000)
Duration Duration_new(u64 secs, u32 nanos);
Duration Duration_fromSecs(u64 secs);
Duration Duration_fromMillis(u64 millis);
Duration Duration_fromMicros(u64 micros);
Duration Duration_fromNanos(u64 nanos);

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

static const Duration Duration_zero        = Duration_fromNanos_(0);
static const Duration Duration_second      = Duration_fromSecs_(1);
static const Duration Duration_millisecond = Duration_fromMillis_(1);
static const Duration Duration_microsecond = Duration_fromMicros_(1);
static const Duration Duration_nanosecond  = Duration_fromNanos_(1);


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
static LARGE_INTEGER SystemTime__s_performanceFrequency = { 0 };
static bool          SystemTime__s_frequencyInitialized = false;
static void          SystemTime__InitFrequency();
#else
#endif


#if defined(__cplusplus)
}
#endif /* defined(__cplusplus) */
#endif /* TIME_INCLUDED */


#if defined(DH_IMPL) && !defined(TIME_IMPL)
#define TIME_IMPL
#endif
#ifdef TIME_IMPL
#define TIME_IMPL_INCLUDED (1)

#include "assert.h"

Duration Duration_new(u64 secs, u32 nanos) {
    Duration duration = Duration_(secs, nanos);
    if (duration.nanos >= 1000000000) {
        duration.secs += duration.nanos / 1000000000;
        duration.nanos %= 1000000000;
    }
    return duration;
}

Duration Duration_fromSecs(u64 secs) {
    return Duration_new(secs, 0);
}

Duration Duration_fromMillis(u64 millis) {
    return Duration_new(millis / 1000, (u32)((millis % 1000) * 1000000));
}

Duration Duration_fromMicros(u64 micros) {
    return Duration_new(micros / 1000000, (u32)((micros % 1000000) * 1000));
}

Duration Duration_fromNanos(u64 nanos) {
    return Duration_new(nanos / 1000000000, (u32)(nanos % 1000000000));
}

Duration Duration_add(Duration a, Duration b) {
    return Duration_new(a.secs + b.secs, a.nanos + b.nanos);
}

Duration Duration_sub(Duration a, Duration b) {
    if (a.nanos < b.nanos) {
        Assert(0 < a.secs);
        return Duration_new(a.secs - b.secs - 1, a.nanos + 1000000000 - b.nanos);
    }
    return Duration_new(a.secs - b.secs, a.nanos - b.nanos);
}

Duration Duration_mul(Duration a, u64 scalar) {
    u64 total_nanos = a.nanos * scalar;
    return Duration_new(a.secs * scalar + total_nanos / 1000000000, (u32)(total_nanos % 1000000000));
}

bool Duration_eq(Duration a, Duration b) {
    return a.secs == b.secs && a.nanos == b.nanos;
}

bool Duration_ne(Duration a, Duration b) {
    return a.secs != b.secs || a.nanos != b.nanos;
}

bool Duration_lt(Duration a, Duration b) {
    return a.secs < b.secs || (a.secs == b.secs && a.nanos < b.nanos);
}

bool Duration_le(Duration a, Duration b) {
    return a.secs < b.secs || (a.secs == b.secs && a.nanos <= b.nanos);
}

bool Duration_gt(Duration a, Duration b) {
    return a.secs > b.secs || (a.secs == b.secs && a.nanos > b.nanos);
}

bool Duration_ge(Duration a, Duration b) {
    return a.secs > b.secs || (a.secs == b.secs && a.nanos >= b.nanos);
}

bool Duration_IsZero(Duration duration) {
    return duration.secs == 0 && duration.nanos == 0;
}

Instant Instant_Now(void) {
    Instant instant = { 0 };
#if defined(_WIN32) || defined(_WIN64)
    SystemTime__InitFrequency();
    QueryPerformanceCounter(&instant.time_);
#else
    clock_gettime(CLOCK_MONOTONIC, &instant.time_);
#endif
    return instant;
}

Duration Instant_Elapsed(Instant start) {
    return Instant_DurationSince(Instant_Now(), start);
}

Duration Instant_DurationSince(Instant start, Instant earlier) {
#if defined(_WIN32) || defined(_WIN64)
    SystemTime__InitFrequency();
    u64 diff      = start.time_.QuadPart - earlier.time_.QuadPart;
    u64 secs      = diff / SystemTime__s_performanceFrequency.QuadPart;
    u64 remainder = diff % SystemTime__s_performanceFrequency.QuadPart;
    u32 nanos     = (u32)((remainder * 1000000000) / SystemTime__s_performanceFrequency.QuadPart);
#else
    u64 secs     = start.time_.tv_sec - earlier.time_.tv_sec;
    i64 nanoDiff = start.time_.tv_nsec - earlier.time_.tv_nsec;
    if (nanoDiff < 0) {
        secs--;
        nanoDiff += 1000000000;
    }
    u32 nanos = (u32)nanoDiff;
#endif
    return Duration_new(secs, nanos);
}

bool Instant_eq(Instant a, Instant b) {
#if defined(_WIN32) || defined(_WIN64)
    return a.time_.QuadPart == b.time_.QuadPart;
#else
    return a.time_.tv_sec == b.time_.tv_sec && a.time_.tv_nsec == b.time_.tv_nsec;
#endif
}

bool Instant_ne(Instant a, Instant b) {
#if defined(_WIN32) || defined(_WIN64)
    return a.time_.QuadPart != b.time_.QuadPart;
#else
    return a.time_.tv_sec != b.time_.tv_sec || a.time_.tv_nsec != b.time_.tv_nsec;
#endif
}

bool Instant_lt(Instant a, Instant b) {
#if defined(_WIN32) || defined(_WIN64)
    return a.time_.QuadPart < b.time_.QuadPart;
#else
    return (a.time_.tv_sec < b.time_.tv_sec) ||
           (a.time_.tv_sec == b.time_.tv_sec && a.time_.tv_nsec < b.time_.tv_nsec);
#endif
}

bool Instant_le(Instant a, Instant b) {
#if defined(_WIN32) || defined(_WIN64)
    return a.time_.QuadPart <= b.time_.QuadPart;
#else
    return (a.time_.tv_sec < b.time_.tv_sec) ||
           (a.time_.tv_sec == b.time_.tv_sec && a.time_.tv_nsec <= b.time_.tv_nsec);
#endif
}

bool Instant_gt(Instant a, Instant b) {
#if defined(_WIN32) || defined(_WIN64)
    return a.time_.QuadPart > b.time_.QuadPart;
#else
    return (a.time_.tv_sec > b.time_.tv_sec) ||
           (a.time_.tv_sec == b.time_.tv_sec && a.time_.tv_nsec > b.time_.tv_nsec);
#endif
}

bool Instant_ge(Instant a, Instant b) {
#if defined(_WIN32) || defined(_WIN64)
    return a.time_.QuadPart >= b.time_.QuadPart;
#else
    return (a.time_.tv_sec > b.time_.tv_sec) ||
           (a.time_.tv_sec == b.time_.tv_sec && a.time_.tv_nsec >= b.time_.tv_nsec);
#endif
}

bool Instant_IsValid(Instant instant) {
#if defined(_WIN32) || defined(_WIN64)
    return 0 < instant.time_.QuadPart;
#else
    return 0 < instant.time_.tv_sec;
#endif
}

#if defined(_WIN32) || defined(_WIN64)
void SystemTime__InitFrequency() {
    if (SystemTime__s_frequencyInitialized) { return; }
    QueryPerformanceFrequency(&SystemTime__s_performanceFrequency);
    SystemTime__s_frequencyInitialized = true;
}
#endif

#endif /* TIME_IMPL */

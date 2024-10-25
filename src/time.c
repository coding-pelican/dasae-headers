#include "time.h"
#include "assert.h"
#include "primitive_types.h"


Duration Duration_from(u64 secs, u32 nanos) {
    Duration duration = Duration_(
            .secs  = secs,
            .nanos = nanos
    );
    if (Time_nanos_per_sec <= duration.nanos) {
        duration.secs += duration.nanos / Time_nanos_per_sec;
        duration.nanos %= Time_nanos_per_sec;
    }
    return duration;
}

Duration Duration_fromSecs(u64 secs) {
    return comptime_Duration_fromSecs(secs);
}

Duration Duration_fromMillis(u64 millis) {
    return comptime_Duration_fromMillis(millis);
}

Duration Duration_fromMicros(u64 micros) {
    return comptime_Duration_fromMicros(micros);
}

Duration Duration_fromNanos(u64 nanos) {
    return comptime_Duration_fromNanos(nanos);
}

Duration Duration_add(Duration lhs, Duration rhs) {
    return Duration_(lhs.secs + rhs.secs, lhs.nanos + rhs.nanos);
}

Duration Duration_sub(Duration lhs, Duration rhs) {
    if (lhs.nanos < rhs.nanos) {
        assert(0 < lhs.secs);
        return Duration_(lhs.secs - rhs.secs - 1, lhs.nanos + Time_nanos_per_sec - rhs.nanos);
    }
    return Duration_(lhs.secs - rhs.secs, lhs.nanos - rhs.nanos);
}

Duration Duration_mul(Duration d, u64 scalar) {
    u64 total_nanos = d.nanos * scalar;
    return Duration_(d.secs * scalar + total_nanos / Time_nanos_per_sec, as(u32, (total_nanos % Time_nanos_per_sec)));
}

bool Duration_eq(Duration lhs, Duration rhs) {
    return lhs.secs == rhs.secs && lhs.nanos == rhs.nanos;
}

bool Duration_ne(Duration lhs, Duration rhs) {
    return lhs.secs != rhs.secs || lhs.nanos != rhs.nanos;
}

bool Duration_lt(Duration lhs, Duration rhs) {
    return lhs.secs < rhs.secs || (lhs.secs == rhs.secs && lhs.nanos < rhs.nanos);
}

bool Duration_le(Duration lhs, Duration rhs) {
    return lhs.secs < rhs.secs || (lhs.secs == rhs.secs && lhs.nanos <= rhs.nanos);
}

bool Duration_gt(Duration lhs, Duration rhs) {
    return lhs.secs > rhs.secs || (lhs.secs == rhs.secs && lhs.nanos > rhs.nanos);
}

bool Duration_ge(Duration lhs, Duration rhs) {
    return lhs.secs > rhs.secs || (lhs.secs == rhs.secs && lhs.nanos >= rhs.nanos);
}

bool Duration_isZero(Duration d) {
    return d.secs == 0 && d.nanos == 0;
}

Instant Instant_now() {
    Instant instant = makeCleared(Instant);
#if defined(_WIN32) || defined(_WIN64)
    SystemTime__initFrequency();
    QueryPerformanceCounter(&instant.time_);
#else // UNIX
    clock_gettime(CLOCK_MONOTONIC, &instant.time_);
#endif
    return instant;
}

Duration Instant_elapsed(Instant start) {
    return Instant_durationSince(Instant_now(), start);
}

Duration Instant_durationSince(Instant start, Instant earlier) {
#if defined(_WIN32) || defined(_WIN64)
    SystemTime__initFrequency();
    u64 diff      = start.time_.QuadPart - earlier.time_.QuadPart;
    u64 secs      = diff / SystemTime__s_performance_frequency.QuadPart;
    u64 remainder = diff % SystemTime__s_performance_frequency.QuadPart;
    u32 nanos     = as(u32, ((remainder * Time_nanos_per_sec) / SystemTime__s_performance_frequency.QuadPart));
#else // UNIX
    u64 secs      = start.time_.tv_sec - earlier.time_.tv_sec;
    i64 nano_diff = start.time_.tv_nsec - earlier.time_.tv_nsec;
    if (nano_diff < 0) {
        secs--;
        nano_diff += Time_nanos_per_sec;
    }
    u32 nanos = as(u32, nano_diff);
#endif
    return Duration_from(secs, nanos);
}

bool Instant_eq(Instant lhs, Instant rhs) {
#if defined(_WIN32) || defined(_WIN64)
    return lhs.time_.QuadPart == rhs.time_.QuadPart;
#else // UNIX
    return lhs.time_.tv_sec == rhs.time_.tv_sec && lhs.time_.tv_nsec == rhs.time_.tv_nsec;
#endif
}

bool Instant_ne(Instant lhs, Instant rhs) {
#if defined(_WIN32) || defined(_WIN64)
    return lhs.time_.QuadPart != rhs.time_.QuadPart;
#else // UNIX
    return lhs.time_.tv_sec != rhs.time_.tv_sec || lhs.time_.tv_nsec != rhs.time_.tv_nsec;
#endif
}

bool Instant_lt(Instant lhs, Instant rhs) {
#if defined(_WIN32) || defined(_WIN64)
    return lhs.time_.QuadPart < rhs.time_.QuadPart;
#else // UNIX
    return (lhs.time_.tv_sec < rhs.time_.tv_sec) ||
           (lhs.time_.tv_sec == rhs.time_.tv_sec && lhs.time_.tv_nsec < rhs.time_.tv_nsec);
#endif
}

bool Instant_le(Instant lhs, Instant rhs) {
#if defined(_WIN32) || defined(_WIN64)
    return lhs.time_.QuadPart <= rhs.time_.QuadPart;
#else // UNIX
    return (lhs.time_.tv_sec < rhs.time_.tv_sec) ||
           (lhs.time_.tv_sec == rhs.time_.tv_sec && lhs.time_.tv_nsec <= rhs.time_.tv_nsec);
#endif
}

bool Instant_gt(Instant lhs, Instant rhs) {
#if defined(_WIN32) || defined(_WIN64)
    return lhs.time_.QuadPart > rhs.time_.QuadPart;
#else // UNIX
    return (lhs.time_.tv_sec > rhs.time_.tv_sec) ||
           (lhs.time_.tv_sec == rhs.time_.tv_sec && lhs.time_.tv_nsec > rhs.time_.tv_nsec);
#endif
}

bool Instant_ge(Instant lhs, Instant rhs) {
#if defined(_WIN32) || defined(_WIN64)
    return lhs.time_.QuadPart >= rhs.time_.QuadPart;
#else // UNIX
    return (lhs.time_.tv_sec > rhs.time_.tv_sec) ||
           (lhs.time_.tv_sec == rhs.time_.tv_sec && lhs.time_.tv_nsec >= rhs.time_.tv_nsec);
#endif
}

bool Instant_isValid(Instant i) {
#if defined(_WIN32) || defined(_WIN64)
    return 0 < i.time_.QuadPart;
#else // UNIX
    return 0 < i.time_.tv_sec;
#endif
}

#if defined(_WIN32) || defined(_WIN64)
void SystemTime__initFrequency() {
    if (SystemTime__s_frequency_initialized) { return; }
    QueryPerformanceFrequency(&SystemTime__s_performance_frequency);
    SystemTime__s_frequency_initialized = true;
}
#endif

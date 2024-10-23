#include "time.h"
#include "assert.h"


Duration Duration_make(u64 secs, u32 nanos) {
    Duration duration = Duration_(secs, nanos);
    if (duration.nanos >= 1000000000) {
        duration.secs += duration.nanos / 1000000000;
        duration.nanos %= 1000000000;
    }
    return duration;
}

Duration Duration_from_Secs(u64 secs) {
    return Duration_make(secs, 0);
}

Duration Duration_from_Millis(u64 millis) {
    return Duration_make(millis / 1000, (u32)((millis % 1000) * 1000000));
}

Duration Duration_from_Micros(u64 micros) {
    return Duration_make(micros / 1000000, (u32)((micros % 1000000) * 1000));
}

Duration Duration_from_Nanos(u64 nanos) {
    return Duration_make(nanos / 1000000000, (u32)(nanos % 1000000000));
}

Duration Duration_add(Duration a, Duration b) {
    return Duration_make(a.secs + b.secs, a.nanos + b.nanos);
}

Duration Duration_sub(Duration a, Duration b) {
    if (a.nanos < b.nanos) {
        Assert(0 < a.secs);
        return Duration_make(a.secs - b.secs - 1, a.nanos + 1000000000 - b.nanos);
    }
    return Duration_make(a.secs - b.secs, a.nanos - b.nanos);
}

Duration Duration_mul(Duration a, u64 scalar) {
    u64 total_nanos = a.nanos * scalar;
    return Duration_make(a.secs * scalar + total_nanos / 1000000000, (u32)(total_nanos % 1000000000));
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
    SystemTime__initFrequency();
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
    SystemTime__initFrequency();
    u64 diff      = start.time_.QuadPart - earlier.time_.QuadPart;
    u64 secs      = diff / SystemTime__s_performance_frequency.QuadPart;
    u64 remainder = diff % SystemTime__s_performance_frequency.QuadPart;
    u32 nanos     = (u32)((remainder * 1000000000) / SystemTime__s_performance_frequency.QuadPart);
#else
    u64 secs      = start.time_.tv_sec - earlier.time_.tv_sec;
    i64 nano_diff = start.time_.tv_nsec - earlier.time_.tv_nsec;
    if (nano_diff < 0) {
        secs--;
        nano_diff += 1000000000;
    }
    u32 nanos = (u32)nano_diff;
#endif
    return Duration_make(secs, nanos);
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
void SystemTime__initFrequency() {
    if (SystemTime__s_frequency_initialized) { return; }
    QueryPerformanceFrequency(&SystemTime__s_performance_frequency);
    SystemTime__s_frequency_initialized = true;
}
#endif

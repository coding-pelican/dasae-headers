/* TODO: Fill in the time_Instant_unix.c file */

#if !(defined(_WIN32) || defined(_WIN64))

#include "dh/time/instant.h"
#include "dh/time/system.h"

/* TODO: Apply modified version of time_SysTime_now() */

time_SysTime time_SysTime_now(void) {
    time_SysTime sys_time = make(time_SysTime);
    clock_gettime(CLOCK_MONOTONIC, &sys_time);
    return sys_time;
}

f64 time_SysTime_toSecs(time_SysTime time) {
    return (f64)time.tv_sec + ((f64)time.tv_nsec / (f64)time_nanos_per_sec);
}

f64 time_SysTime_toMillis(time_SysTime time) {
    return (f64)time.tv_sec * (f64)time_millis_per_sec + ((f64)time.tv_nsec / (f64)time_nanos_per_milli);
}

f64 time_SysTime_toMicros(time_SysTime time) {
    return (f64)time.tv_sec * (f64)time_micros_per_sec + ((f64)time.tv_nsec / (f64)time_nanos_per_micro);
}

u64 time_SysTime_toNanos(time_SysTime time) {
    return (u64)time.tv_sec * (u64)time_nanos_per_sec + (u64)time.tv_nsec;
}

void time_sleep(Duration duration) {
    time_sleep_ns((duration.secs * time_nanos_per_sec) + duration.nanos);
}

void time_sleep_s(f64 secs) {
    time_sleep_ns((u64)(secs * time_nanos_per_sec));
}

void time_sleep_ms(f64 millis) {
    time_sleep_ns((u64)(millis * time_nanos_per_milli));
}

void time_sleep_us(f64 micros) {
    time_sleep_ns((u64)(micros * time_nanos_per_micro));
}

void time_sleep_ns(u64 nanos) {
    if (nanos < 100) { return; }

    struct timespec req = make(struct timespec);
    struct timespec rem = make(struct timespec);
    req.tv_sec          = nanos / time_nanos_per_sec;
    req.tv_nsec         = nanos % time_nanos_per_sec;

    while (nanosleep(&req, &rem) == -1) {
        if (errno == EINTR) {
            req = rem;
        } else {
            break;
        }
    }
}

time_Instant time_Instant_now(void) {
    return make(time_Instant, .time_ = time_SysTime_now());
}

Duration time_Instant_elapsed(time_Instant start) {
    return time_Instant_durationSince(time_Instant_now(), start);
}

Duration time_Instant_durationSince(time_Instant start, time_Instant earlier) {
    u64 secs      = start.time_.tv_sec - earlier.time_.tv_sec;
    i64 nano_diff = start.time_.tv_nsec - earlier.time_.tv_nsec;
    if (nano_diff < 0) {
        secs--;
        nano_diff += time_nanos_per_sec;
    }
    u32 nanos = prim_as(u32, nano_diff);
    return time_Duration_from(secs, nanos);
}

bool time_Instant_eq(time_Instant lhs, time_Instant rhs) {
    return lhs.time_.tv_sec == rhs.time_.tv_sec && lhs.time_.tv_nsec == rhs.time_.tv_nsec;
}

bool time_Instant_ne(time_Instant lhs, time_Instant rhs) {
    return lhs.time_.tv_sec != rhs.time_.tv_sec || lhs.time_.tv_nsec != rhs.time_.tv_nsec;
}

bool time_Instant_lt(time_Instant lhs, time_Instant rhs) {
    return (lhs.time_.tv_sec < rhs.time_.tv_sec) || (lhs.time_.tv_sec == rhs.time_.tv_sec && lhs.time_.tv_nsec < rhs.time_.tv_nsec);
}

bool time_Instant_le(time_Instant lhs, time_Instant rhs) {
    return (lhs.time_.tv_sec < rhs.time_.tv_sec) || (lhs.time_.tv_sec == rhs.time_.tv_sec && lhs.time_.tv_nsec <= rhs.time_.tv_nsec);
}

bool time_Instant_gt(time_Instant lhs, time_Instant rhs) {
    return (lhs.time_.tv_sec > rhs.time_.tv_sec) || (lhs.time_.tv_sec == rhs.time_.tv_sec && lhs.time_.tv_nsec > rhs.time_.tv_nsec);
}

bool time_Instant_ge(time_Instant lhs, time_Instant rhs) {
    return (lhs.time_.tv_sec > rhs.time_.tv_sec) || (lhs.time_.tv_sec == rhs.time_.tv_sec && lhs.time_.tv_nsec >= rhs.time_.tv_nsec);
}

bool time_Instant_isValid(time_Instant i) {
    return 0 < i.time_.tv_sec;
}

#endif /* !(defined(_WIN32) || defined(_WIN64)) */

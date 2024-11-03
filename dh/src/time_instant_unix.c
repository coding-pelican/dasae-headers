#if !(defined(_WIN32) || defined(_WIN64))

#include "../time.h"


SystemTime SystemTime_now(void) {
    SystemTime sys_time = make(SystemTime);
    clock_gettime(CLOCK_MONOTONIC, &sys_time);
    return sys_time;
}

f64 SystemTime_toSecs(SystemTime time) {
    return (f64)time.tv_sec + ((f64)time.tv_nsec / (f64)Time_nanos_per_sec);
}

f64 SystemTime_toMillis(SystemTime time) {
    return (f64)time.tv_sec * (f64)Time_millis_per_sec + ((f64)time.tv_nsec / (f64)Time_nanos_per_milli);
}

f64 SystemTime_toMicros(SystemTime time) {
    return (f64)time.tv_sec * (f64)Time_micros_per_sec + ((f64)time.tv_nsec / (f64)Time_nanos_per_micro);
}

u64 SystemTime_toNanos(SystemTime time) {
    return (u64)time.tv_sec * (u64)Time_nanos_per_sec + (u64)time.tv_nsec;
}

void SystemTime_sleep(Duration duration) {
    SystemTime_sleep_ns((duration.secs * Time_nanos_per_sec) + duration.nanos);
}

void SystemTime_sleep_s(f64 secs) {
    SystemTime_sleep_ns((u64)(secs * Time_nanos_per_sec));
}

void SystemTime_sleep_ms(f64 millis) {
    SystemTime_sleep_ns((u64)(millis * Time_nanos_per_milli));
}

void SystemTime_sleep_us(f64 micros) {
    SystemTime_sleep_ns((u64)(micros * Time_nanos_per_micro));
}

void SystemTime_sleep_ns(u64 nanos) {
    if (nanos < 100) { return; }

    struct timespec req = make(struct timespec);
    struct timespec rem = make(struct timespec);
    req.tv_sec  = nanos / Time_nanos_per_sec;
    req.tv_nsec = nanos % Time_nanos_per_sec;

    while (nanosleep(&req, &rem) == -1) {
        if (errno == EINTR) {
            req = rem;
        } else {
            break;
        }
    }
}

Instant Instant_now(void) {
    return makeWith(Instant, .time_ = SystemTime_now());
}

Duration Instant_elapsed(Instant start) {
    return Instant_durationSince(Instant_now(), start);
}

Duration Instant_durationSince(Instant start, Instant earlier) {
    u64 secs      = start.time_.tv_sec - earlier.time_.tv_sec;
    i64 nano_diff = start.time_.tv_nsec - earlier.time_.tv_nsec;
    if (nano_diff < 0) {
        secs--;
        nano_diff += Time_nanos_per_sec;
    }
    u32 nanos = prim_as(u32, nano_diff);
    return Duration_from(secs, nanos);
}

bool Instant_eq(Instant lhs, Instant rhs) {
    return lhs.time_.tv_sec == rhs.time_.tv_sec && lhs.time_.tv_nsec == rhs.time_.tv_nsec;
}

bool Instant_ne(Instant lhs, Instant rhs) {
    return lhs.time_.tv_sec != rhs.time_.tv_sec || lhs.time_.tv_nsec != rhs.time_.tv_nsec;
}

bool Instant_lt(Instant lhs, Instant rhs) {
    return (lhs.time_.tv_sec < rhs.time_.tv_sec) || (lhs.time_.tv_sec == rhs.time_.tv_sec && lhs.time_.tv_nsec < rhs.time_.tv_nsec);
}

bool Instant_le(Instant lhs, Instant rhs) {
    return (lhs.time_.tv_sec < rhs.time_.tv_sec) || (lhs.time_.tv_sec == rhs.time_.tv_sec && lhs.time_.tv_nsec <= rhs.time_.tv_nsec);
}

bool Instant_gt(Instant lhs, Instant rhs) {
    return (lhs.time_.tv_sec > rhs.time_.tv_sec) || (lhs.time_.tv_sec == rhs.time_.tv_sec && lhs.time_.tv_nsec > rhs.time_.tv_nsec);
}

bool Instant_ge(Instant lhs, Instant rhs) {
    return (lhs.time_.tv_sec > rhs.time_.tv_sec) || (lhs.time_.tv_sec == rhs.time_.tv_sec && lhs.time_.tv_nsec >= rhs.time_.tv_nsec);
}

bool Instant_isValid(Instant i) {
    return 0 < i.time_.tv_sec;
}

#endif /* !(defined(_WIN32) || defined(_WIN64)) */

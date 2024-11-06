#include "../include/dh/time/time.h"

#include "../include/dh/debug/debug_assert.h"


Duration Duration_from(u64 secs, u32 nanos) {
    Duration duration = makeWith(Duration, .secs = secs, .nanos = nanos);
    if (Time_nanos_per_sec <= duration.nanos) {
        duration.secs += duration.nanos / Time_nanos_per_sec;
        duration.nanos %= Time_nanos_per_sec;
    }
    return duration;
}

Duration Duration_fromSecs(f64 secs) {
    return Duration_from((u64)secs, (u32)((secs - (f64)(u64)secs) * Time_nanos_per_sec));
}

Duration Duration_fromMillis(f64 millis) {
    return Duration_from((u64)(millis / Time_millis_per_sec), (u32)((millis - (f64)(u64)millis) * Time_nanos_per_milli));
}

Duration Duration_fromMicros(f64 micros) {
    return Duration_from((u64)(micros / Time_micros_per_sec), (u32)((micros - (f64)(u64)micros) * Time_nanos_per_micro));
}

Duration Duration_fromSecs_u64(u64 secs) {
    return comptime_Duration_fromSecs(secs);
}

Duration Duration_fromMillis_u64(u64 millis) {
    return comptime_Duration_fromMillis(millis);
}

Duration Duration_fromMicros_u64(u64 micros) {
    return comptime_Duration_fromMicros(micros);
}

Duration Duration_fromNanos(u64 nanos) {
    return comptime_Duration_fromNanos(nanos);
}

f64 Duration_toSecs(Duration d) {
    return (f64)d.secs + (f64)d.nanos / (f64)Time_nanos_per_sec;
}

f64 Duration_toMillis(Duration d) {
    return (f64)d.secs * (f64)Time_millis_per_sec + (f64)d.nanos / (f64)Time_nanos_per_milli;
}

f64 Duration_toMicros(Duration d) {
    return (f64)d.secs * (f64)Time_micros_per_sec + (f64)d.nanos / (f64)Time_nanos_per_micro;
}

u64 Duration_toNanos(Duration d) {
    return (u64)d.secs * (u64)Time_nanos_per_sec + (u64)d.nanos;
}

Duration Duration_add(Duration lhs, Duration rhs) {
    u64 total_nanos = (u64)lhs.nanos + rhs.nanos;
    return makeWith(
        Duration,
        lhs.secs + rhs.secs + (total_nanos >= Time_nanos_per_sec),
        (u32)(total_nanos >= Time_nanos_per_sec ? total_nanos - Time_nanos_per_sec : total_nanos)
    );
}

Duration Duration_sub(Duration lhs, Duration rhs) {
    return makeWith(
        Duration,
        lhs.secs - rhs.secs - (lhs.nanos < rhs.nanos),
        lhs.nanos < rhs.nanos ? lhs.nanos + Time_nanos_per_sec - rhs.nanos : lhs.nanos - rhs.nanos
    );
}

Duration Duration_mul(Duration d, u64 scalar) {
    u64 total_nanos = d.nanos * scalar;
    return makeWith(
        Duration,
        d.secs * scalar + total_nanos / Time_nanos_per_sec,
        (u32)(total_nanos % Time_nanos_per_sec)
    );
}

Duration Duration_div(Duration d, u64 scalar) {
    u64 total_nanos = d.nanos / scalar;
    return makeWith(
        Duration,
        d.secs / scalar + total_nanos / Time_nanos_per_sec,
        (u32)(total_nanos % Time_nanos_per_sec)
    );
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

#include "dh/time/Duration.h"
#include "dh/core/prim/flt.h"
#include "dh/debug/assert.h"


time_Duration time_Duration_from(u64 secs, u32 nanos) {
    return literal_time_Duration_from(secs, nanos);
}

time_Duration time_Duration_fromSecs(u64 secs) {
    return literal_time_Duration_fromSecs(secs);
}

time_Duration time_Duration_fromMillis(u64 millis) {
    return literal_time_Duration_fromMillis(millis);
}

time_Duration time_Duration_fromMicros(u64 micros) {
    return literal_time_Duration_fromMicros(micros);
}

time_Duration time_Duration_fromNanos(u64 nanos) {
    return literal_time_Duration_fromNanos(nanos);
}

u64 time_Duration_asSecs(time_Duration self) {
    return self.secs_;
}

u32 time_Duration_subsecMillis(time_Duration self) {
    return self.nanos_ / time_nanos_per_milli;
}

u32 time_Duration_subsecMicros(time_Duration self) {
    return self.nanos_ / time_nanos_per_micro;
}

u32 time_Duration_subsecNanos(time_Duration self) {
    return self.nanos_;
}

time_Duration time_Duration_fromSecs_f64(f64 secs) {
    return time_Duration_from(
        as$(u64, secs),
        as$(u32, (secs - as$(f64, as$(u64, secs))) * as$(f64, time_nanos_per_sec))
    );
}

f64 time_Duration_asSecs_f64(time_Duration self) {
    return as$(f64, self.secs_) + as$(f64, self.nanos_) / as$(f64, time_nanos_per_sec);
}

time_Duration op_fnAdd(time_Duration) {
    u64 total_nanos = as$(u64, self.nanos_) + other.nanos_;
    return (time_Duration){
        self.secs_ + other.secs_ + (total_nanos >= time_nanos_per_sec),
        as$(u32,
            time_nanos_per_sec <= total_nanos
                ? total_nanos - time_nanos_per_sec
                : total_nanos
        )
    };
}

time_Duration op_fnSub(time_Duration) {
    return (time_Duration){
        self.secs_ - other.secs_ - (self.nanos_ < other.nanos_),
        self.nanos_ < other.nanos_
            ? self.nanos_ + time_nanos_per_sec - other.nanos_
            : self.nanos_ - other.nanos_
    };
}

time_Duration op_fnMulBy(time_Duration, u64) {
    u64 total_nanos = self.nanos_ * other;
    return (time_Duration){
        self.secs_ * other + total_nanos / time_nanos_per_sec,
        as$(u32, total_nanos % time_nanos_per_sec)
    };
}

time_Duration op_fnDivBy(time_Duration, u64) {
    claim_assert_fmt(other != 0, "Division by zero");
    // This logic only divides the nanos_ field by other and divides the secs_ field by other separately.
    // That loses the fractional portion contributed by the secs_.
    // For example, if self = { secs_ = 1, nanos_ = 500_000_000 } (i.e. 1.5 s) and you divide by 2,
    // you’d want 0.75 s. Instead, the above code returns 0.25 s,
    // because it is ignoring the 1 second’s worth of nanoseconds during the division.

    // Combine both secs_ and nanos_ into total nanoseconds before dividing
    // Watch out for potential overflow if secs_ is very large,
    // but for typical usage this is acceptable.
    u64 total_nanos = as$(u64, self.secs_) * as$(u64, time_nanos_per_sec) + as$(u64, self.nanos_);
    total_nanos /= other;
    return (time_Duration){
        .secs_  = total_nanos / time_nanos_per_sec,
        .nanos_ = as$(u32, total_nanos % time_nanos_per_sec)
    };
}

bool time_Duration_isZero(time_Duration self) {
    return self.secs_ == 0 && self.nanos_ == 0;
}

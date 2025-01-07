#include "dh/time/Duration.h"
#include "dh/core/prim/flt.h"
#include "dh/debug/assert.h"


time_Duration time_Duration_from(u64 secs_, u32 nanos_) {
    return literal_time_Duration_from(secs_, nanos_);
}

time_Duration time_Duration_fromSecs(u64 secs_) {
    return literal_time_Duration_fromSecs(secs_);
}

time_Duration time_Duration_fromMillis(u64 millis) {
    return literal_time_Duration_fromMillis(millis);
}

time_Duration time_Duration_fromMicros(u64 micros) {
    return literal_time_Duration_fromMicros(micros);
}

time_Duration time_Duration_fromNanos(u64 nanos_) {
    return literal_time_Duration_fromNanos(nanos_);
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
        (u64)secs,
        (u32)((secs - (f64)((u64)secs)) * (f64)time_nanos_per_sec)
    );
}

f64 time_Duration_asSecs_f64(time_Duration self) {
    return (f64)self.secs_ + (f64)self.nanos_ / (f64)time_nanos_per_sec;
}

time_Duration op_fnAdd(time_Duration) {
    u64 total_nanos = (u64)self.nanos_ + other.nanos_;
    return make(
        time_Duration,
        self.secs_ + other.secs_ + (total_nanos >= time_nanos_per_sec),
        (u32)(total_nanos >= time_nanos_per_sec
                  ? total_nanos - time_nanos_per_sec
                  : total_nanos)
    );
}

time_Duration op_fnSub(time_Duration) {
    return make(
        time_Duration,
        self.secs_ - other.secs_ - (self.nanos_ < other.nanos_),
        self.nanos_ < other.nanos_
            ? self.nanos_ + time_nanos_per_sec - other.nanos_
            : self.nanos_ - other.nanos_
    );
}

time_Duration op_fnMulBy(time_Duration, u64) {
    u64 total_nanos = self.nanos_ * other;
    return make(
        time_Duration,
        self.secs_ * other + total_nanos / time_nanos_per_sec,
        (u32)(total_nanos % time_nanos_per_sec)
    );
}

time_Duration op_fnDivBy(time_Duration, u64) {
    claim_assert_fmt(other != 0, "Division by zero");
    u64 total_nanos = self.nanos_ / other;
    return make(
        time_Duration,
        self.secs_ / other + total_nanos / time_nanos_per_sec,
        (u32)(total_nanos % time_nanos_per_sec)
    );
}

bool time_Duration_isZero(time_Duration self) {
    return self.secs_ == 0 && self.nanos_ == 0;
}

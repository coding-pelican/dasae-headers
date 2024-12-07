#if defined(_WIN32) || defined(_WIN64)

#include "dh/time/cfg.h"
#include "dh/time/common.h"
#include "dh/time/SysTime.h"
#include "dh/time/Instant.h"
#include "dh/time/Duration.h"

/*========== Operations =================================================*/

time_Instant time_Instant_now(void) {
    return (time_Instant){
        .time_ = time_SysTime_now()
    };
}

time_Duration time_Instant_elapsed(time_Instant self) {
    return time_SysTime_elapsed(self.time_);
}

time_Duration time_Instant_durationSince(time_Instant self, time_Instant earlier) {
    return time_SysTime_durationSince(self.time_, earlier.time_);
}

/*========== Validation ================================================*/

bool time_Instant_isValid(time_Instant self) {
    return 0 < self.time_.QuadPart;
}

/*========== Conversion ================================================*/

u64 time_Instant_toNanos(time_Instant self) {
    // Convert performance counter value to nanoseconds
    time_Duration elapsed = time_SysTime_durationSince(self.time_, time_SysTime_unix_epoch);
    return elapsed.secs_ * time_nanos_per_sec + elapsed.nanos_;
}

/*========== Safe Arithmetic Operations ================================*/

Opt_time_Instant time_Instant_addDurationChecked(time_Instant self, time_Duration other) {
    Opt_time_SysTime result = time_SysTime_addDurationChecked(self.time_, other);
    if (Opt_isNone(result)) {
        return Opt_none(Opt_time_Instant);
    }
    return Opt_some(Opt_time_Instant, Opt_unwrap(Opt_time_SysTime, result));
}

Opt_time_Instant time_Instant_subDurationChecked(time_Instant self, time_Duration other) {
    Opt_time_SysTime result = time_SysTime_subDurationChecked(self.time_, other);
    if (Opt_isNone(result)) {
        return Opt_none(Opt_time_Instant);
    }
    return Opt_some(Opt_time_Instant, Opt_unwrap(Opt_time_SysTime, result));
}

/*========== Unsafe Arithmetic Operations ==============================*/

time_Instant op_fnAddBy(time_Instant, time_Duration) {
    return Opt_unwrap(Opt_time_Instant, time_Instant_addDurationChecked(self, other));
}

time_Instant op_fnSubBy(time_Instant, time_Duration) {
    return Opt_unwrap(Opt_time_Instant, time_Instant_subDurationChecked(self, other));
}

/*========== Comparison Operations ====================================*/

cmp_Ord time_Instant_cmp(time_Instant self, time_Instant other) {
    return time_SysTime_cmp(self.time_, other.time_);
}

#endif /* defined(_WIN32) || defined(_WIN64) */

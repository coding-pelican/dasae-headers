#include "dh/time.h"

#if bti_plat_windows && (bti_plat_32bit || bti_plat_64bit)

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

Opt$time_Instant time_Instant_addDurationChecked(time_Instant self, time_Duration other) {
    reserveReturn(Opt$time_Instant);
    let result = time_SysTime_addDurationChecked(self.time_, other);
    if_some(result, opt) {
        return_some((time_Instant){ opt });
    }
    return_none();
}

Opt$time_Instant time_Instant_subDurationChecked(time_Instant self, time_Duration other) {
    reserveReturn(Opt$time_Instant);
    let result = time_SysTime_subDurationChecked(self.time_, other);
    if_some(result, opt) {
        return_some((time_Instant){ opt });
    }
    return_none();
}

/*========== Unsafe Arithmetic Operations ==============================*/

time_Instant op_fnAddBy(time_Instant, time_Duration) {
    return unwrap(time_Instant_addDurationChecked(self, other));
}

time_Instant op_fnSubBy(time_Instant, time_Duration) {
    return unwrap(time_Instant_subDurationChecked(self, other));
}

/*========== Comparison Operations ====================================*/

cmp_fnCmp(time_Instant) {
    return time_SysTime_cmp(self.time_, other.time_);
}

#endif /* bti_plat_windows && (bti_plat_32BIT || bti_plat_64BIT) */

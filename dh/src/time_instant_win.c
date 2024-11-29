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

// bool time_Instant ops_try_add_other(time_Duration, time_Instant) {
//     return time_SysTime_try_add_time_Duration(self.time_, other, &out->time_);
// }

// bool time_Instant ops_try_sub_other(time_Duration, time_Instant) {
//     return time_SysTime_try_sub_time_Duration(self.time_, other, &out->time_);
// }

/*========== Unsafe Arithmetic Operations ==============================*/

time_Instant ops_fnAddBy(time_Instant, time_Duration) {
    return (time_Instant){
        .time_ = ops_addBy(time_SysTime, time_Duration)(self.time_, other)
    };
}

time_Instant ops_fnSubBy(time_Instant, time_Duration) {
    return (time_Instant){
        .time_ = ops_addBy(time_SysTime, time_Duration)(self.time_, other)
    };
}

/*========== Comparison Operations ====================================*/

cmp_Ord time_Instant_cmp(time_Instant self, time_Instant other) {
    return time_SysTime_cmp(self.time_, other.time_);
}

#endif /* defined(_WIN32) || defined(_WIN64) */

#include "dh/time/Instant.h"
#include "dh/time/SysTime.h"
#include "dh/time/Duration.h"

/*========== Operations =====================================================*/

time_Instant time_Instant_now(void) {
    return (time_Instant){ .point = time_SysTime_now() };
}

time_Duration time_Instant_elapsed(time_Instant self) {
    return time_Instant_durationSince(time_Instant_now(), self);
}

time_Duration time_Instant_durationSince(time_Instant later, time_Instant earlier) {
    return unwrap_(time_SysTime_durationSinceChkd(later.point, earlier.point));
}

O$time_Duration time_Instant_durationSinceChkd(time_Instant later, time_Instant earlier) {
    return time_SysTime_durationSinceChkd(later.point, earlier.point);
}

/*========== Arithmetic Operations ==========================================*/

time_Instant op_fnAddBy(time_Instant, time_Duration) {
    return unwrap_(time_Instant_addChkdDuration(self, other));
}

time_Instant op_fnAddAsgBy(time_Instant, time_Duration) {
    return *self = op_addBy(time_Instant, time_Duration)(*self, other);
}

time_Instant op_fnSubBy(time_Instant, time_Duration) {
    return unwrap_(time_Instant_subChkdDuration(self, other));
}

time_Instant op_fnSubAsgBy(time_Instant, time_Duration) {
    return *self = op_subBy(time_Instant, time_Duration)(*self, other);
}

fn_((time_Instant_addChkdDuration(time_Instant lhs, time_Duration rhs))(O$time_Instant) $scope) {
    if_some((time_SysTime_addChkdDuration(lhs.point, rhs))(opt)) {
        return_some({ .point = opt });
    }
    return_none();
} $unscoped_(fn);

fn_((time_Instant_subChkdDuration(time_Instant lhs, time_Duration rhs))(O$time_Instant) $scope) {
    if_some((time_SysTime_subChkdDuration(lhs.point, rhs))(opt)) {
        return_some({ .point = opt });
    }
    return_none();
} $unscoped_(fn);

/*========== Time Conversion to/from Unix Epoch =============================*/

time_Instant time_Instant_fromUnixEpoch(u64 secs) {
    return (time_Instant){ .point = time_SysTime_fromUnixEpoch(secs) };
}

u64 time_Instant_toUnixEpoch(time_Instant self) {
    return time_SysTime_toUnixEpoch(self.point);
}

/*========== Comparison Operations ==========================================*/

cmp_fnCmp(time_Instant) {
    return time_SysTime_cmp(self.point, other.point);
}

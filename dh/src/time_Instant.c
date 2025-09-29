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
    return unwrap(time_SysTime_chkdDurationSince(later.point, earlier.point));
}

Opt$time_Duration time_Instant_chkdDurationSince(time_Instant later, time_Instant earlier) {
    return time_SysTime_chkdDurationSince(later.point, earlier.point);
}

/*========== Arithmetic Operations ==========================================*/

time_Instant op_fnAddBy(time_Instant, time_Duration) {
    return unwrap(time_Instant_chkdAddDuration(self, other));
}

time_Instant op_fnAddAsgBy(time_Instant, time_Duration) {
    return *self = op_addBy(time_Instant, time_Duration)(*self, other);
}

time_Instant op_fnSubBy(time_Instant, time_Duration) {
    return unwrap(time_Instant_chkdSubDuration(self, other));
}

time_Instant op_fnSubAsgBy(time_Instant, time_Duration) {
    return *self = op_subBy(time_Instant, time_Duration)(*self, other);
}

fn_(time_Instant_chkdAddDuration(time_Instant lhs, time_Duration rhs), Opt$time_Instant $scope) {
    if_some(time_SysTime_chkdAddDuration(lhs.point, rhs), opt) {
        return_some({ .point = opt });
    }
    return_none();
} $unscoped;

fn_(time_Instant_chkdSubDuration(time_Instant lhs, time_Duration rhs), Opt$time_Instant $scope) {
    if_some(time_SysTime_chkdSubDuration(lhs.point, rhs), opt) {
        return_some({ .point = opt });
    }
    return_none();
} $unscoped;

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

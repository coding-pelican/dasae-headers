#include "dh/time/Instant.h"

#include "dh/time/SysTime.h"
#include "dh/time/Duration.h"

#if bti_plat_windows && (bti_plat_32bit || bti_plat_64bit)

/*========== Operations =====================================================*/

time_Instant time_Instant_now(void) {
    return (time_Instant){ .point = time_SysTime_now() };
}

time_Duration time_Instant_elapsed(time_Instant self) {
    return time_Instant_durationSince(time_Instant_now(), self);
}

time_Duration time_Instant_durationSince(time_Instant self, time_Instant earlier) {
    return unwrap(time_Instant_durationSinceChecked(self, earlier));
}

Opt$time_Duration time_Instant_durationSinceChecked(time_Instant self, time_Instant earlier) {
    return time_SysTime_durationSinceChecked(self.point, earlier.point);
}

/*========== Arithmetic Operations ==========================================*/

time_Instant op_fnAddBy(time_Instant, time_Duration) {
    return unwrap(time_Instant_addDurationChecked(self, other));
}

time_Instant op_fnSubBy(time_Instant, time_Duration) {
    return unwrap(time_Instant_subDurationChecked(self, other));
}

Opt$time_Instant time_Instant_addDurationChecked(time_Instant lhs, time_Duration rhs) {
    reserveReturn(Opt$time_Instant);
    if_some(time_SysTime_addDurationChecked(lhs.point, rhs), opt) {
        return_some({ .point = opt });
    }
    return_none();
}

Opt$time_Instant time_Instant_subDurationChecked(time_Instant lhs, time_Duration rhs) {
    reserveReturn(Opt$time_Instant);
    if_some(time_SysTime_subDurationChecked(lhs.point, rhs), opt) {
        return_some({ .point = opt });
    }
    return_none();
}

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

#endif /* bti_plat_windows && (bti_plat_32BIT || bti_plat_64BIT) */

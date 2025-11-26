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

op_fn_addWith$(((time_Instant, time_Duration)(lhs, rhs))(time_Instant)) {
    return unwrap_(time_Instant_addChkdDuration(lhs, rhs));
}

op_fn_addAsgWith$(((time_Instant, time_Duration)(lhs, rhs))(time_Instant*)) {
    return *lhs = unwrap_(time_Instant_addChkdDuration(*lhs, rhs)), lhs;
}

op_fn_subWith$(((time_Instant, time_Duration)(lhs, rhs))(time_Instant)) {
    return unwrap_(time_Instant_subChkdDuration(lhs, rhs));
}

op_fn_subAsgWith$(((time_Instant, time_Duration)(lhs, rhs))(time_Instant*)) {
    return *lhs = unwrap_(time_Instant_subChkdDuration(*lhs, rhs)), lhs;
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

cmp_fn_ord$((time_Instant)(lhs, rhs)) {
    return time_SysTime_ord(lhs.point, rhs.point);
}
cmp_fn_eq_default$((time_Instant)(lhs, rhs));
cmp_fn_ne_default$((time_Instant)(lhs, rhs));
cmp_fn_lt_default$((time_Instant)(lhs, rhs));
cmp_fn_gt_default$((time_Instant)(lhs, rhs));
cmp_fn_le_default$((time_Instant)(lhs, rhs));
cmp_fn_ge_default$((time_Instant)(lhs, rhs));
cmp_fn_ordCtx$((time_Instant)(lhs, rhs, ctx)) {
    let_ignore = ctx;
    return cmp_ord$(time_Instant)(lhs, rhs);
}
cmp_fn_eqCtx_default$((time_Instant)(lhs, rhs, ctx));
cmp_fn_neCtx_default$((time_Instant)(lhs, rhs, ctx));
cmp_fn_ltCtx_default$((time_Instant)(lhs, rhs, ctx));
cmp_fn_gtCtx_default$((time_Instant)(lhs, rhs, ctx));
cmp_fn_leCtx_default$((time_Instant)(lhs, rhs, ctx));
cmp_fn_geCtx_default$((time_Instant)(lhs, rhs, ctx));

cmp_fn_eql$((time_Instant)(lhs, rhs)) {
    return cmp_ord$(time_Instant)(lhs, rhs) == cmp_Ord_eq;
}
cmp_fn_neq_default$((time_Instant)(lhs, rhs));
cmp_fn_eqlCtx$((time_Instant)(lhs, rhs, ctx)) {
    let_ignore = ctx;
    return cmp_eql$(time_Instant)(lhs, rhs);
}
cmp_fn_neqCtx_default$((time_Instant)(lhs, rhs, ctx));

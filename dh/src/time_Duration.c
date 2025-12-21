#include "dh/time/Duration.h"
#include "dh/time/common.h"

/* --- Constructors --- */

fn_((time_Duration_from(u64 secs, u32 nanos))(time_Duration)) {
    return time_Duration_from_static(secs, nanos);
};

fn_((time_Duration_fromSecs(u64 secs))(time_Duration)) {
    return time_Duration_fromSecs_static(secs);
};

fn_((time_Duration_fromSecs$f64(f64 secs))(time_Duration)) {
    return time_Duration_fromSecs$f64_static(secs);
};

fn_((time_Duration_fromSecs$f32(f32 secs))(time_Duration)) {
    return time_Duration_fromSecs$f32_static(secs);
};

fn_((time_Duration_fromMillis(u64 millis))(time_Duration)) {
    return time_Duration_fromMillis_static(millis);
};

fn_((time_Duration_fromMicros(u64 micros))(time_Duration)) {
    return time_Duration_fromMicros_static(micros);
};

fn_((time_Duration_fromNanos(u64 nanos))(time_Duration)) {
    return time_Duration_fromNanos_static(nanos);
};

/* --- Accessors --- */

fn_((time_Duration_asSecs(time_Duration self))(u64)) {
    return self.secs;
};

fn_((time_Duration_asSecs$f64(time_Duration self))(f64)) {
    return as$(f64)(self.secs) + as$(f64)(self.nanos) / as$(f64)(time_nanos_per_sec);
};

fn_((time_Duration_asSecs$f32(time_Duration self))(f32)) {
    return as$(f32)(self.secs) + as$(f32)(self.nanos) / as$(f32)(time_nanos_per_sec);
};

fn_((time_Duration_subsecMillis(time_Duration self))(u32)) {
    return self.nanos / time_nanos_per_milli;
};

fn_((time_Duration_subsecMicros(time_Duration self))(u32)) {
    return self.nanos / time_nanos_per_micro;
};

fn_((time_Duration_subsecNanos(time_Duration self))(u32)) {
    return self.nanos;
};

/* --- Validation --- */

fn_((time_Duration_isZero(time_Duration self))(bool)) {
    return time_Duration_eq(self, time_Duration_zero);
};

/* --- Arithmetic --- */

op_fn_add$(((time_Duration)(lhs, rhs))(time_Duration)) {
    return unwrap_(time_Duration_addChkd(lhs, rhs));
};

op_fn_addAsg$(((time_Duration)(lhs, rhs))(time_Duration*)) {
    return *lhs = unwrap_(time_Duration_addChkd(*lhs, rhs)), lhs;
};

op_fn_sub$(((time_Duration)(lhs, rhs))(time_Duration)) {
    return unwrap_(time_Duration_subChkd(lhs, rhs));
};

op_fn_subAsg$(((time_Duration)(lhs, rhs))(time_Duration*)) {
    return *lhs = unwrap_(time_Duration_subChkd(*lhs, rhs)), lhs;
};

op_fn_mulWith$(((time_Duration, u32)(lhs, rhs))(time_Duration)) {
    return unwrap_(time_Duration_mulChkd$u32(lhs, rhs));
};

op_fn_mulAsgWith$(((time_Duration, u32)(lhs, rhs))(time_Duration*)) {
    return *lhs = unwrap_(time_Duration_mulChkd$u32(*lhs, rhs)), lhs;
};

op_fn_divWith$(((time_Duration, u32)(lhs, rhs))(time_Duration)) {
    return unwrap_(time_Duration_divChkd$u32(lhs, rhs));
};

op_fn_divAsgWith$(((time_Duration, u32)(lhs, rhs))(time_Duration*)) {
    return *lhs = unwrap_(time_Duration_divChkd$u32(*lhs, rhs)), lhs;
};

fn_((time_Duration_addChkd(time_Duration lhs, time_Duration rhs))(O$time_Duration) $scope) {
    let total_nanos = (lhs.secs * time_nanos_per_sec + lhs.nanos)
                    + (rhs.secs * time_nanos_per_sec + rhs.nanos);
    // Check for overflow in nanoseconds
    if (total_nanos < lhs.nanos || total_nanos < rhs.nanos
        || (0 < lhs.secs && (u64_limit - lhs.secs) < rhs.secs)) {
        return_none();
    }
    let secs = total_nanos / time_nanos_per_sec;
    let nanos = as$(u32)(total_nanos % time_nanos_per_sec);
    return_some(time_Duration_from(secs, nanos));
} $unscoped_(fn);

fn_((time_Duration_subChkd(time_Duration lhs, time_Duration rhs))(O$time_Duration) $scope) {
    let lhs_total_nanos = lhs.secs * time_nanos_per_sec + lhs.nanos;
    let rhs_total_nanos = rhs.secs * time_nanos_per_sec + rhs.nanos;
    // Check for underflow
    if (lhs_total_nanos < rhs_total_nanos) {
        return_none();
    }
    let diff_nanos = lhs_total_nanos - rhs_total_nanos;
    let secs = diff_nanos / time_nanos_per_sec;
    let nanos = as$(u32)(diff_nanos % time_nanos_per_sec);
    return_some(time_Duration_from(secs, nanos));
} $unscoped_(fn);


fn_((time_Duration_mulChkd$u32(time_Duration lhs, u32 rhs))(O$time_Duration) $scope) {
    let total_nanos = as$(u64)(lhs.nanos) * as$(u64)(rhs);
    let extra_secs = total_nanos / as$(u64)(time_nanos_per_sec);
    let nanos = as$(u32)(total_nanos % as$(u64)(time_nanos_per_sec));
    if_some((u64_mulChkd(lhs.secs, as$(u64)(rhs)))(scaled)) {
        if_some((u64_addChkd(scaled, extra_secs))(secs)) {
            claim_assert(nanos < time_nanos_per_sec);
            return_some(time_Duration_from(secs, nanos));
        }
    }
    return_none();
} $unscoped_(fn);

fn_((time_Duration_divChkd$u32(time_Duration lhs, u32 rhs))(O$time_Duration) $scope) {
    if (rhs == 0) { return_none(); }
    let secs = lhs.secs / (as$(u64)(rhs));
    let extra_secs = lhs.secs % (as$(u64)(rhs));
    var nanos = lhs.nanos / (as$(u32)(rhs));
    let extra_nanos = lhs.nanos % (as$(u32)(rhs));
    nanos += nanos + as$(u64)((extra_secs * as$(u64)(time_nanos_per_sec) + as$(u64)(extra_nanos)) / as$(u64)(rhs));
    claim_assert(nanos < time_nanos_per_sec);
    return_some(time_Duration_from(secs, nanos));
} $unscoped_(fn);

fn_((time_Duration_addSat(time_Duration lhs, time_Duration rhs))(time_Duration)) {
    return orelse_((time_Duration_addChkd(lhs, rhs))(time_Duration_from(u64_limit_max, u32_limit_max)));
};

fn_((time_Duration_subSat(time_Duration lhs, time_Duration rhs))(time_Duration)) {
    return orelse_((time_Duration_subChkd(lhs, rhs))(time_Duration_zero));
};

fn_((time_Duration_mulSat$u32(time_Duration lhs, u32 rhs))(time_Duration)) {
    return orelse_((time_Duration_mulChkd$u32(lhs, rhs))(time_Duration_from(u64_limit_max, u32_limit_max)));
};

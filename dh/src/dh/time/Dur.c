#include "dh/time/Dur.h"

/* --- Constructors --- */

fn_((time_Dur_from(u64 secs, u32 nanos))(time_Dur)) {
    return time_Dur_from_static(secs, nanos);
};

fn_((time_Dur_fromSecs(u64 secs))(time_Dur)) {
    return time_Dur_fromSecs_static(secs);
};

fn_((time_Dur_fromSecs$f64(f64 secs))(time_Dur)) {
    return time_Dur_fromSecs$f64_static(secs);
};

fn_((time_Dur_fromSecs$f32(f32 secs))(time_Dur)) {
    return time_Dur_fromSecs$f32_static(secs);
};

fn_((time_Dur_fromMillis(u64 millis))(time_Dur)) {
    return time_Dur_fromMillis_static(millis);
};

fn_((time_Dur_fromMicros(u64 micros))(time_Dur)) {
    return time_Dur_fromMicros_static(micros);
};

fn_((time_Dur_fromNanos(u64 nanos))(time_Dur)) {
    return time_Dur_fromNanos_static(nanos);
};

/* --- Accessors --- */

fn_((time_Dur_asSecs(time_Dur self))(u64)) {
    return self.secs;
};

fn_((time_Dur_asSecs$f64(time_Dur self))(f64)) {
    return as$(f64)(self.secs) + as$(f64)(self.nanos) / as$(f64)(time_nanos_per_sec);
};

fn_((time_Dur_asSecs$f32(time_Dur self))(f32)) {
    return as$(f32)(self.secs) + as$(f32)(self.nanos) / as$(f32)(time_nanos_per_sec);
};

fn_((time_Dur_subsecMillis(time_Dur self))(u32)) {
    return self.nanos / time_nanos_per_milli;
};

fn_((time_Dur_subsecMicros(time_Dur self))(u32)) {
    return self.nanos / time_nanos_per_micro;
};

fn_((time_Dur_subsecNanos(time_Dur self))(u32)) {
    return self.nanos;
};

/* --- Validation --- */

fn_((time_Dur_isZero(time_Dur self))(bool)) {
    return time_Dur_eq(self, time_Dur_zero);
};

/* --- Arithmetic --- */

op_fn_add$(((time_Dur)(lhs, rhs))(time_Dur)) {
    return unwrap_(time_Dur_addChkd(lhs, rhs));
};

op_fn_addAsg$(((time_Dur)(lhs, rhs))(time_Dur*)) {
    return *lhs = unwrap_(time_Dur_addChkd(*lhs, rhs)), lhs;
};

op_fn_sub$(((time_Dur)(lhs, rhs))(time_Dur)) {
    return unwrap_(time_Dur_subChkd(lhs, rhs));
};

op_fn_subAsg$(((time_Dur)(lhs, rhs))(time_Dur*)) {
    return *lhs = unwrap_(time_Dur_subChkd(*lhs, rhs)), lhs;
};

op_fn_mulWith$(((time_Dur, u32)(lhs, rhs))(time_Dur)) {
    return unwrap_(time_Dur_mulChkd$u32(lhs, rhs));
};

op_fn_mulAsgWith$(((time_Dur, u32)(lhs, rhs))(time_Dur*)) {
    return *lhs = unwrap_(time_Dur_mulChkd$u32(*lhs, rhs)), lhs;
};

op_fn_divWith$(((time_Dur, u32)(lhs, rhs))(time_Dur)) {
    return unwrap_(time_Dur_divChkd$u32(lhs, rhs));
};

op_fn_divAsgWith$(((time_Dur, u32)(lhs, rhs))(time_Dur*)) {
    return *lhs = unwrap_(time_Dur_divChkd$u32(*lhs, rhs)), lhs;
};

fn_((time_Dur_addChkd(time_Dur lhs, time_Dur rhs))(O$time_Dur) $scope) {
    let total_nanos = (lhs.secs * time_nanos_per_sec + lhs.nanos)
                    + (rhs.secs * time_nanos_per_sec + rhs.nanos);
    // Check for overflow in nanoseconds
    if (total_nanos < lhs.nanos || total_nanos < rhs.nanos
        || (0 < lhs.secs && (u64_limit - lhs.secs) < rhs.secs)) {
        return_none();
    }
    let secs = total_nanos / time_nanos_per_sec;
    let nanos = as$(u32)(total_nanos % time_nanos_per_sec);
    return_some(time_Dur_from(secs, nanos));
} $unscoped(fn);

fn_((time_Dur_subChkd(time_Dur lhs, time_Dur rhs))(O$time_Dur) $scope) {
    let lhs_total_nanos = lhs.secs * time_nanos_per_sec + lhs.nanos;
    let rhs_total_nanos = rhs.secs * time_nanos_per_sec + rhs.nanos;
    // Check for underflow
    if (lhs_total_nanos < rhs_total_nanos) {
        return_none();
    }
    let diff_nanos = lhs_total_nanos - rhs_total_nanos;
    let secs = diff_nanos / time_nanos_per_sec;
    let nanos = as$(u32)(diff_nanos % time_nanos_per_sec);
    return_some(time_Dur_from(secs, nanos));
} $unscoped(fn);


fn_((time_Dur_mulChkd$u32(time_Dur lhs, u32 rhs))(O$time_Dur) $scope) {
    let total_nanos = as$(u64)(lhs.nanos) * as$(u64)(rhs);
    let extra_secs = total_nanos / as$(u64)(time_nanos_per_sec);
    let nanos = as$(u32)(total_nanos % as$(u64)(time_nanos_per_sec));
    if_some((u64_mulChkd(lhs.secs, as$(u64)(rhs)))(scaled)) {
        if_some((u64_addChkd(scaled, extra_secs))(secs)) {
            claim_assert(nanos < time_nanos_per_sec);
            return_some(time_Dur_from(secs, nanos));
        }
    }
    return_none();
} $unscoped(fn);

fn_((time_Dur_divChkd$u32(time_Dur lhs, u32 rhs))(O$time_Dur) $scope) {
    if (rhs == 0) { return_none(); }
    let rhs_u64 = as$(u64)(rhs);
    let secs = lhs.secs / rhs_u64;
    let rem_secs = lhs.secs % rhs_u64;
    let nanos = as$(u32)((rem_secs * as$(u64)(time_nanos_per_sec) + as$(u64)(lhs.nanos)) / rhs_u64);
    claim_assert(nanos < time_nanos_per_sec);
    return_some(time_Dur_from(secs, nanos));
} $unscoped(fn);

fn_((time_Dur_addSat(time_Dur lhs, time_Dur rhs))(time_Dur)) {
    return orelse_((time_Dur_addChkd(lhs, rhs))(time_Dur_from(u64_limit_max, u32_limit_max)));
};

fn_((time_Dur_subSat(time_Dur lhs, time_Dur rhs))(time_Dur)) {
    return orelse_((time_Dur_subChkd(lhs, rhs))(time_Dur_zero));
};

fn_((time_Dur_mulSat$u32(time_Dur lhs, u32 rhs))(time_Dur)) {
    return orelse_((time_Dur_mulChkd$u32(lhs, rhs))(time_Dur_from(u64_limit_max, u32_limit_max)));
};

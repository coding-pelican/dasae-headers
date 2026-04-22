#include "Dur.h"

/*========== External Definitions ===========================================*/

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
    let nanos_sum = as$(u64)(lhs.nanos) + as$(u64)(rhs.nanos);
    let carry = nanos_sum / as$(u64)(time_nanos_per_sec);
    let nanos = as$(u32)(nanos_sum % as$(u64)(time_nanos_per_sec));
    if_some((u64_addChkd(lhs.secs, rhs.secs))(secs_without_carry)) {
        if_some((u64_addChkd(secs_without_carry, carry))(secs)) {
            return_some(time_Dur_from(secs, nanos));
        }
    }
    return_none();
} $unscoped(fn);

fn_((time_Dur_subChkd(time_Dur lhs, time_Dur rhs))(O$time_Dur) $scope) {
    if (cmp_lt$(time_Dur)(lhs, rhs)) {
        return_none();
    }
    var secs = lhs.secs - rhs.secs;
    var nanos = u32_limit_min;
    if (lhs.nanos < rhs.nanos) {
        secs -= 1;
        nanos = as$(u32)(time_nanos_per_sec + lhs.nanos - rhs.nanos);
    } else {
        nanos = lhs.nanos - rhs.nanos;
    }
    return_some(time_Dur_from(secs, nanos));
} $unscoped(fn);

fn_((time_Dur_mulChkd$u32(time_Dur lhs, u32 rhs))(O$time_Dur) $scope) {
    let total_nanos = as$(u64)(lhs.nanos) * as$(u64)(rhs);
    let extra_secs = total_nanos / as$(u64)(time_nanos_per_sec);
    let nanos = as$(u32)(total_nanos % as$(u64)(time_nanos_per_sec));
    if_some((u64_mulChkd(lhs.secs, as$(u64)(rhs)))(scaled)) {
        if_some((u64_addChkd(scaled, extra_secs))(secs)) {
            return_some(time_Dur_from(secs, nanos));
        }
    }
    return_none();
} $unscoped(fn);

fn_((time_Dur_divChkd$u32(time_Dur lhs, u32 rhs))(O$time_Dur) $scope) {
    if (rhs == 0) { return_none(); }
    let divisor = as$(u64)(rhs);
    let secs = lhs.secs / divisor;
    let rem_secs = lhs.secs % divisor;
    let nanos_total = rem_secs * as$(u64)(time_nanos_per_sec) + as$(u64)(lhs.nanos);
    let nanos = as$(u32)(nanos_total / divisor);
    return_some(time_Dur_from(secs, nanos));
} $unscoped(fn);

fn_((time_Dur_addSat(time_Dur lhs, time_Dur rhs))(time_Dur)) {
    return orelse_((time_Dur_addChkd(lhs, rhs))(time_Dur_from(u64_limit_max, time_nanos_per_sec - 1)));
};

fn_((time_Dur_subSat(time_Dur lhs, time_Dur rhs))(time_Dur)) {
    return orelse_((time_Dur_subChkd(lhs, rhs))(time_Dur_zero));
};

fn_((time_Dur_mulSat$u32(time_Dur lhs, u32 rhs))(time_Dur)) {
    return orelse_((time_Dur_mulChkd$u32(lhs, rhs))(time_Dur_from(u64_limit_max, time_nanos_per_sec - 1)));
};

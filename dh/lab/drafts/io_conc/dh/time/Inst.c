#include "Inst.h"
#include "Self.h"

/*========== External Definitions ===========================================*/

fn_((time_Inst_from(u64 secs, u32 nanos))(time_Inst)) {
    return (time_Inst){
        .secs = secs + nanos / time_nanos_per_sec,
        .nanos = nanos % time_nanos_per_sec,
    };
};

fn_((time_Inst_isZero(time_Inst self))(bool)) {
    return isZero(self.secs) && isZero(self.nanos);
};

fn_((time_Inst_ticks(time_Inst self))(u64)) {
    return self.secs * as$(u64)(time_nanos_per_sec) + self.nanos;
};

op_fn_addWith$(((time_Inst, time_Dur)(lhs, rhs))(time_Inst)) {
    return unwrap_(time_Inst_addChkdDur(lhs, rhs));
};

op_fn_addAsgWith$(((time_Inst, time_Dur)(lhs, rhs))(time_Inst*)) {
    return *lhs = unwrap_(time_Inst_addChkdDur(*lhs, rhs)), lhs;
};

op_fn_subWith$(((time_Inst, time_Dur)(lhs, rhs))(time_Inst)) {
    return unwrap_(time_Inst_subChkdDur(lhs, rhs));
};

op_fn_subAsgWith$(((time_Inst, time_Dur)(lhs, rhs))(time_Inst*)) {
    return *lhs = unwrap_(time_Inst_subChkdDur(*lhs, rhs)), lhs;
};

fn_((time_Inst_addChkdDur(time_Inst lhs, time_Dur rhs))(O$time_Inst) $scope) {
    let nanos_sum = as$(u64)(lhs.nanos) + as$(u64)(rhs.nanos);
    let carry = nanos_sum / as$(u64)(time_nanos_per_sec);
    let nanos = as$(u32)(nanos_sum % as$(u64)(time_nanos_per_sec));
    if_some((u64_addChkd(lhs.secs, rhs.secs))(secs_without_carry)) {
        if_some((u64_addChkd(secs_without_carry, carry))(secs)) {
            return_some((time_Inst){ .secs = secs, .nanos = nanos });
        }
    }
    return_none();
} $unscoped(fn);

fn_((time_Inst_subChkdDur(time_Inst lhs, time_Dur rhs))(O$time_Inst) $scope) {
    if (cmp_lt$(time_Inst)(lhs, (time_Inst){ .secs = rhs.secs, .nanos = rhs.nanos })) {
        return_none();
    }
    let borrow = lhs.nanos < rhs.nanos ? 1ull : 0ull;
    let secs = lhs.secs - rhs.secs - borrow;
    let nanos = lhs.nanos < rhs.nanos
                  ? lhs.nanos + time_nanos_per_sec - rhs.nanos
                  : lhs.nanos - rhs.nanos;
    return_some((time_Inst){ .secs = secs, .nanos = nanos });
} $unscoped(fn);

fn_((time_Inst_durSince(time_Inst later, time_Inst earlier))(time_Dur)) {
    return unwrap_(time_Inst_durSinceChkd(later, earlier));
};

fn_((time_Inst_durSinceChkd(time_Inst later, time_Inst earlier))(O$time_Dur) $scope) {
    let diff = orelse_((time_Inst_subChkdDur(later, (time_Dur){ .secs = earlier.secs, .nanos = earlier.nanos }))(
        return_none()
    ));
    return_some({ .secs = diff.secs, .nanos = diff.nanos });
} $unscoped(fn);

cmp_fn_ord$((time_Inst)(lhs, rhs)) {
    if (lhs.secs < rhs.secs) return cmp_Ord_lt;
    if (lhs.secs > rhs.secs) return cmp_Ord_gt;
    if (lhs.nanos < rhs.nanos) return cmp_Ord_lt;
    if (lhs.nanos > rhs.nanos) return cmp_Ord_gt;
    return cmp_Ord_eq;
};
cmp_fn_eq_default$((time_Inst)(lhs, rhs));
cmp_fn_ne_default$((time_Inst)(lhs, rhs));
cmp_fn_lt_default$((time_Inst)(lhs, rhs));
cmp_fn_gt_default$((time_Inst)(lhs, rhs));
cmp_fn_le_default$((time_Inst)(lhs, rhs));
cmp_fn_ge_default$((time_Inst)(lhs, rhs));
cmp_fn_ordCtx$((time_Inst)(lhs, rhs, ctx)) {
    let_ignore = ctx;
    return cmp_ord$(time_Inst)(lhs, rhs);
};
cmp_fn_eqCtx_default$((time_Inst)(lhs, rhs, ctx));
cmp_fn_neCtx_default$((time_Inst)(lhs, rhs, ctx));
cmp_fn_ltCtx_default$((time_Inst)(lhs, rhs, ctx));
cmp_fn_gtCtx_default$((time_Inst)(lhs, rhs, ctx));
cmp_fn_leCtx_default$((time_Inst)(lhs, rhs, ctx));
cmp_fn_geCtx_default$((time_Inst)(lhs, rhs, ctx));
cmp_fn_eql$((time_Inst)(lhs, rhs)) {
    return cmp_ord$(time_Inst)(lhs, rhs) == cmp_Ord_eq;
};
cmp_fn_neq_default$((time_Inst)(lhs, rhs));
cmp_fn_eqlCtx$((time_Inst)(lhs, rhs, ctx)) {
    let_ignore = ctx;
    return cmp_eql$(time_Inst)(lhs, rhs);
};
cmp_fn_neqCtx_default$((time_Inst)(lhs, rhs, ctx));

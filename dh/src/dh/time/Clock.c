#include "dh/time/Clock.h"

fn_((time_Clock_now(time_Clock clock))(time_Clock_Inst) $scope) {
    match_(clock) {
    pattern_((time_Clock_real)(time)) return_(union_of((time_Clock_real)(time_Real_now(time)))) $end(pattern);
    pattern_((time_Clock_awake)(time)) return_(union_of((time_Clock_awake)(time_Awake_now(time)))) $end(pattern);
    pattern_((time_Clock_boot)(time)) return_(union_of((time_Clock_boot)(time_Boot_now(time)))) $end(pattern);
    pattern_((time_Clock_proc)(time)) return_(union_of((time_Clock_proc)(time_Proc_now(time)))) $end(pattern);
    pattern_((time_Clock_thrd)(time)) return_(union_of((time_Clock_thrd)(time_Thrd_now(time)))) $end(pattern);
    } $end(match);
} $unscoped(fn);
fn_((time_Clock_sleep(time_Clock self, time_Dur dur))(Sched_Cancelable$void) $scope) {
    match_(self) {
    pattern_((time_Clock_real)($ignore)) return_err(E_cause$Sched_Canceled()) $end(pattern);
    pattern_((time_Clock_awake)(time)) return_(time_Awake_sleep(time, dur)) $end(pattern);
    pattern_((time_Clock_boot)(time)) return_(time_Boot_sleep(time, dur)) $end(pattern);
    pattern_((time_Clock_proc)($ignore)) return_err(E_cause$Sched_Canceled()) $end(pattern);
    pattern_((time_Clock_thrd)($ignore)) return_err(E_cause$Sched_Canceled()) $end(pattern);
    } $end(match);
} $unscoped(fn);
fn_((time_Clock_sleepSecs(time_Clock self, u64 secs))(Sched_Cancelable$void) $scope) {
    match_(self) {
    pattern_((time_Clock_real)($ignore)) return_err(E_cause$Sched_Canceled()) $end(pattern);
    pattern_((time_Clock_awake)(time)) return_(time_Awake_sleepSecs(time, secs)) $end(pattern);
    pattern_((time_Clock_boot)(time)) return_(time_Boot_sleepSecs(time, secs)) $end(pattern);
    pattern_((time_Clock_proc)($ignore)) return_err(E_cause$Sched_Canceled()) $end(pattern);
    pattern_((time_Clock_thrd)($ignore)) return_err(E_cause$Sched_Canceled()) $end(pattern);
    } $end(match);
} $unscoped(fn);
fn_((time_Clock_sleepMillis(time_Clock self, u64 millis))(Sched_Cancelable$void) $scope) {
    match_(self) {
    pattern_((time_Clock_real)($ignore)) return_err(E_cause$Sched_Canceled()) $end(pattern);
    pattern_((time_Clock_awake)(time)) return_(time_Awake_sleepMillis(time, millis)) $end(pattern);
    pattern_((time_Clock_boot)(time)) return_(time_Boot_sleepMillis(time, millis)) $end(pattern);
    pattern_((time_Clock_proc)($ignore)) return_err(E_cause$Sched_Canceled()) $end(pattern);
    pattern_((time_Clock_thrd)($ignore)) return_err(E_cause$Sched_Canceled()) $end(pattern);
    } $end(match);
} $unscoped(fn);
fn_((time_Clock_sleepMicros(time_Clock self, u64 micros))(Sched_Cancelable$void) $scope) {
    match_(self) {
    pattern_((time_Clock_real)($ignore)) return_err(E_cause$Sched_Canceled()) $end(pattern);
    pattern_((time_Clock_awake)(time)) return_(time_Awake_sleepMicros(time, micros)) $end(pattern);
    pattern_((time_Clock_boot)(time)) return_(time_Boot_sleepMicros(time, micros)) $end(pattern);
    pattern_((time_Clock_proc)($ignore)) return_err(E_cause$Sched_Canceled()) $end(pattern);
    pattern_((time_Clock_thrd)($ignore)) return_err(E_cause$Sched_Canceled()) $end(pattern);
    } $end(match);
} $unscoped(fn);
fn_((time_Clock_sleepNanos(time_Clock self, u32 nanos))(Sched_Cancelable$void) $scope) {
    match_(self) {
    pattern_((time_Clock_real)($ignore)) return_err(E_cause$Sched_Canceled()) $end(pattern);
    pattern_((time_Clock_awake)(time)) return_(time_Awake_sleepNanos(time, nanos)) $end(pattern);
    pattern_((time_Clock_boot)(time)) return_(time_Boot_sleepNanos(time, nanos)) $end(pattern);
    pattern_((time_Clock_proc)($ignore)) return_err(E_cause$Sched_Canceled()) $end(pattern);
    pattern_((time_Clock_thrd)($ignore)) return_err(E_cause$Sched_Canceled()) $end(pattern);
    } $end(match);
} $unscoped(fn);

fn_((time_Clock_Inst_elapsed(time_Clock_Inst self, time_Clock time))(time_Dur) $scope) {
    match_(self) {
    pattern_((time_Clock_real)(inst)) return_(
        time_Real_Inst_elapsed(inst, union_to((time)(time_Clock_real)))
    ) $end(pattern);
    pattern_((time_Clock_awake)(inst)) return_(
        time_Awake_Inst_elapsed(inst, union_to((time)(time_Clock_awake)))
    ) $end(pattern);
    pattern_((time_Clock_boot)(inst)) return_(
        time_Boot_Inst_elapsed(inst, union_to((time)(time_Clock_boot)))
    ) $end(pattern);
    pattern_((time_Clock_proc)(inst)) return_(
        time_Proc_Inst_elapsed(inst, union_to((time)(time_Clock_proc)))
    ) $end(pattern);
    pattern_((time_Clock_thrd)(inst)) return_(
        time_Thrd_Inst_elapsed(inst, union_to((time)(time_Clock_thrd)))
    ) $end(pattern);
    } $end(match);
} $unscoped(fn);
fn_((time_Clock_Inst_durSince(time_Clock_Inst later, time_Clock_Inst earlier))(time_Dur) $scope) {
    match_(later) {
    pattern_((time_Clock_real)(later)) return_(
        time_Real_Inst_durSince(later, union_to((earlier)(time_Clock_real)))
    ) $end(pattern);
    pattern_((time_Clock_awake)(later)) return_(
        time_Awake_Inst_durSince(later, union_to((earlier)(time_Clock_awake)))
    ) $end(pattern);
    pattern_((time_Clock_boot)(later)) return_(
        time_Boot_Inst_durSince(later, union_to((earlier)(time_Clock_boot)))
    ) $end(pattern);
    pattern_((time_Clock_proc)(later)) return_(
        time_Proc_Inst_durSince(later, union_to((earlier)(time_Clock_proc)))
    ) $end(pattern);
    pattern_((time_Clock_thrd)(later)) return_(
        time_Thrd_Inst_durSince(later, union_to((earlier)(time_Clock_thrd)))
    ) $end(pattern);
    } $end(match);
} $unscoped(fn);
fn_((time_Clock_Inst_durSinceChkd(time_Clock_Inst later, time_Clock_Inst earlier))(O$time_Dur) $scope) {
    match_(later) {
    pattern_((time_Clock_real)(later)) return_(
        time_Real_Inst_durSinceChkd(later, union_to((earlier)(time_Clock_real)))
    ) $end(pattern);
    pattern_((time_Clock_awake)(later)) return_(
        time_Awake_Inst_durSinceChkd(later, union_to((earlier)(time_Clock_awake)))
    ) $end(pattern);
    pattern_((time_Clock_boot)(later)) return_(
        time_Boot_Inst_durSinceChkd(later, union_to((earlier)(time_Clock_boot)))
    ) $end(pattern);
    pattern_((time_Clock_proc)(later)) return_(
        time_Proc_Inst_durSinceChkd(later, union_to((earlier)(time_Clock_proc)))
    ) $end(pattern);
    pattern_((time_Clock_thrd)(later)) return_(
        time_Thrd_Inst_durSinceChkd(later, union_to((earlier)(time_Clock_thrd)))
    ) $end(pattern);
    } $end(match);
} $unscoped(fn);

op_fn_addWith$(((time_Clock_Inst, time_Dur)(lhs, rhs))(time_Clock_Inst)) {
    return unwrap_(time_Clock_Inst_addChkdDur(lhs, rhs));
};
op_fn_addAsgWith$(((time_Clock_Inst, time_Dur)(lhs, rhs))(time_Clock_Inst*)) {
    return *lhs = unwrap_(time_Clock_Inst_addChkdDur(*lhs, rhs)), lhs;
};
op_fn_subWith$(((time_Clock_Inst, time_Dur)(lhs, rhs))(time_Clock_Inst)) {
    return unwrap_(time_Clock_Inst_subChkdDur(lhs, rhs));
};
op_fn_subAsgWith$(((time_Clock_Inst, time_Dur)(lhs, rhs))(time_Clock_Inst*)) {
    return *lhs = unwrap_(time_Clock_Inst_subChkdDur(*lhs, rhs)), lhs;
};
fn_((time_Clock_Inst_addChkdDur(time_Clock_Inst lhs, time_Dur rhs))(O$time_Clock_Inst) $scope) {
    match_(lhs) {
    pattern_((time_Clock_real)(lhs)) return_some(
        union_of((time_Clock_real)(orelse_((time_Real_Inst_addChkdDur(lhs, rhs))(return_none()))))
    ) $end(pattern);
    pattern_((time_Clock_awake)(lhs)) return_some(
        union_of((time_Clock_awake)(orelse_((time_Awake_Inst_addChkdDur(lhs, rhs))(return_none()))))
    ) $end(pattern);
    pattern_((time_Clock_boot)(lhs)) return_some(
        union_of((time_Clock_boot)(orelse_((time_Boot_Inst_addChkdDur(lhs, rhs))(return_none()))))
    ) $end(pattern);
    pattern_((time_Clock_proc)(lhs)) return_some(
        union_of((time_Clock_proc)(orelse_((time_Proc_Inst_addChkdDur(lhs, rhs))(return_none()))))
    ) $end(pattern);
    pattern_((time_Clock_thrd)(lhs)) return_some(
        union_of((time_Clock_thrd)(orelse_((time_Thrd_Inst_addChkdDur(lhs, rhs))(return_none()))))
    ) $end(pattern);
    } $end(match);
} $unscoped(fn);
fn_((time_Clock_Inst_subChkdDur(time_Clock_Inst lhs, time_Dur rhs))(O$time_Clock_Inst) $scope) {
    match_(lhs) {
    pattern_((time_Clock_real)(lhs)) return_some(
        union_of((time_Clock_real)(orelse_((time_Real_Inst_subChkdDur(lhs, rhs))(return_none()))))
    ) $end(pattern);
    pattern_((time_Clock_awake)(lhs)) return_some(
        union_of((time_Clock_awake)(orelse_((time_Awake_Inst_subChkdDur(lhs, rhs))(return_none()))))
    ) $end(pattern);
    pattern_((time_Clock_boot)(lhs)) return_some(
        union_of((time_Clock_boot)(orelse_((time_Boot_Inst_subChkdDur(lhs, rhs))(return_none()))))
    ) $end(pattern);
    pattern_((time_Clock_proc)(lhs)) return_some(
        union_of((time_Clock_proc)(orelse_((time_Proc_Inst_subChkdDur(lhs, rhs))(return_none()))))
    ) $end(pattern);
    pattern_((time_Clock_thrd)(lhs)) return_some(
        union_of((time_Clock_thrd)(orelse_((time_Thrd_Inst_subChkdDur(lhs, rhs))(return_none()))))
    ) $end(pattern);
    } $end(match);
} $unscoped(fn);

cmp_fn_ord$((time_Clock_Inst)(lhs, rhs)) {
    match_(lhs) {
    pattern_((time_Clock_real)(lhs)) return cmp_ord$(time_Real_Inst)(
        lhs, union_to((rhs)(time_Clock_real))
    ) $end(pattern);
    pattern_((time_Clock_awake)(lhs)) return cmp_ord$(time_Awake_Inst)(
        lhs, union_to((rhs)(time_Clock_awake))
    ) $end(pattern);
    pattern_((time_Clock_boot)(lhs)) return cmp_ord$(time_Boot_Inst)(
        lhs, union_to((rhs)(time_Clock_boot))
    ) $end(pattern);
    pattern_((time_Clock_proc)(lhs)) return cmp_ord$(time_Proc_Inst)(
        lhs, union_to((rhs)(time_Clock_proc))
    ) $end(pattern);
    pattern_((time_Clock_thrd)(lhs)) return cmp_ord$(time_Thrd_Inst)(
        lhs, union_to((rhs)(time_Clock_thrd))
    ) $end(pattern);
    } $end(match);
};
cmp_fn_eq_default$((time_Clock_Inst)(lhs, rhs));
cmp_fn_ne_default$((time_Clock_Inst)(lhs, rhs));
cmp_fn_lt_default$((time_Clock_Inst)(lhs, rhs));
cmp_fn_gt_default$((time_Clock_Inst)(lhs, rhs));
cmp_fn_le_default$((time_Clock_Inst)(lhs, rhs));
cmp_fn_ge_default$((time_Clock_Inst)(lhs, rhs));
cmp_fn_ordCtx$((time_Clock_Inst)(lhs, rhs, ctx)) {
    match_(lhs) {
    pattern_((time_Clock_real)(lhs)) return cmp_ordCtx$(time_Real_Inst)(
        lhs, union_to((rhs)(time_Clock_real)), ctx
    ) $end(pattern);
    pattern_((time_Clock_awake)(lhs)) return cmp_ordCtx$(time_Awake_Inst)(
        lhs, union_to((rhs)(time_Clock_awake)), ctx
    ) $end(pattern);
    pattern_((time_Clock_boot)(lhs)) return cmp_ordCtx$(time_Boot_Inst)(
        lhs, union_to((rhs)(time_Clock_boot)), ctx
    ) $end(pattern);
    pattern_((time_Clock_proc)(lhs)) return cmp_ordCtx$(time_Proc_Inst)(
        lhs, union_to((rhs)(time_Clock_proc)), ctx
    ) $end(pattern);
    pattern_((time_Clock_thrd)(lhs)) return cmp_ordCtx$(time_Thrd_Inst)(
        lhs, union_to((rhs)(time_Clock_thrd)), ctx
    ) $end(pattern);
    } $end(match);
};
cmp_fn_eqCtx_default$((time_Clock_Inst)(lhs, rhs, ctx));
cmp_fn_neCtx_default$((time_Clock_Inst)(lhs, rhs, ctx));
cmp_fn_ltCtx_default$((time_Clock_Inst)(lhs, rhs, ctx));
cmp_fn_gtCtx_default$((time_Clock_Inst)(lhs, rhs, ctx));
cmp_fn_leCtx_default$((time_Clock_Inst)(lhs, rhs, ctx));
cmp_fn_geCtx_default$((time_Clock_Inst)(lhs, rhs, ctx));
cmp_fn_eql$((time_Clock_Inst)(lhs, rhs)) {
    match_(lhs) {
    pattern_((time_Clock_real)(lhs)) return cmp_eql$(time_Real_Inst)(
        lhs, union_to((rhs)(time_Clock_real))
    ) $end(pattern);
    pattern_((time_Clock_awake)(lhs)) return cmp_eql$(time_Awake_Inst)(
        lhs, union_to((rhs)(time_Clock_awake))
    ) $end(pattern);
    pattern_((time_Clock_boot)(lhs)) return cmp_eql$(time_Boot_Inst)(
        lhs, union_to((rhs)(time_Clock_boot))
    ) $end(pattern);
    pattern_((time_Clock_proc)(lhs)) return cmp_eql$(time_Proc_Inst)(
        lhs, union_to((rhs)(time_Clock_proc))
    ) $end(pattern);
    pattern_((time_Clock_thrd)(lhs)) return cmp_eql$(time_Thrd_Inst)(
        lhs, union_to((rhs)(time_Clock_thrd))
    ) $end(pattern);
    } $end(match);
};
cmp_fn_neq_default$((time_Clock_Inst)(lhs, rhs));
cmp_fn_eqlCtx$((time_Clock_Inst)(lhs, rhs, ctx)) {
    match_(lhs) {
    pattern_((time_Clock_real)(lhs)) return cmp_eqlCtx$(time_Real_Inst)(
        lhs, union_to((rhs)(time_Clock_real)), ctx
    ) $end(pattern);
    pattern_((time_Clock_awake)(lhs)) return cmp_eqlCtx$(time_Awake_Inst)(
        lhs, union_to((rhs)(time_Clock_awake)), ctx
    ) $end(pattern);
    pattern_((time_Clock_boot)(lhs)) return cmp_eqlCtx$(time_Boot_Inst)(
        lhs, union_to((rhs)(time_Clock_boot)), ctx
    ) $end(pattern);
    pattern_((time_Clock_proc)(lhs)) return cmp_eqlCtx$(time_Proc_Inst)(
        lhs, union_to((rhs)(time_Clock_proc)), ctx
    ) $end(pattern);
    pattern_((time_Clock_thrd)(lhs)) return cmp_eqlCtx$(time_Thrd_Inst)(
        lhs, union_to((rhs)(time_Clock_thrd)), ctx
    ) $end(pattern);
    } $end(match);
};
cmp_fn_neqCtx_default$((time_Clock_Inst)(lhs, rhs, ctx));

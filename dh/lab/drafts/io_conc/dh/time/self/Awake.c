#include "Awake.h"
#include "internal.h"
#include "../../exec/Coop.h"

/*========== Internal Declarations ==========================================*/

$attr($maybe_unused)
$static fn_((time_Awake_direct__unsupported_now(P$raw ctx))(time_Awake_Inst));
pp_if_(plat_is_windows)(pp_then_(
    $static fn_((time_Awake_direct__windows_now(P$raw ctx))(time_Awake_Inst));
));
pp_if_(plat_based_unix)(pp_then_(
    $static fn_((time_Awake_direct__unix__clockId(void))(clockid_t));
    $static fn_((time_Awake_direct__unix_now(P$raw ctx))(time_Awake_Inst));
));
$attr($must_check)
$static fn_((time_Awake_direct__sleep(P$raw ctx, time_Dur dur))(Sched_Cancelable$void));
$static fn_((time_Awake_evented__now(P$raw ctx))(time_Awake_Inst));
$attr($must_check)
$static fn_((time_Awake_evented__sleep(P$raw ctx, time_Dur dur))(Sched_Cancelable$void));

$static let time_Awake_direct__now = pp_if_(plat_is_windows)(
    pp_then_(time_Awake_direct__windows_now),
    pp_else_(pp_if_(plat_based_unix)(
        pp_then_(time_Awake_direct__unix_now),
        pp_else_(time_Awake_direct__unsupported_now)
    )));

T_use$((exec_Timer)(ArrPQue_enque));

/*========== External Definitions ===========================================*/

let_(time_Awake_VTbl_noop, time_Awake_VTbl) = {
    .nowFn = time_Awake_VTbl_noNow,
    .sleepFn = time_Awake_VTbl_failingSleep,
};

let_(time_Awake_VTbl_failing, time_Awake_VTbl) = {
    .nowFn = time_Awake_VTbl_unreachableNow,
    .sleepFn = time_Awake_VTbl_failingSleep,
};

$static var_(time_Awake_noop_ctx, Void) = cleared();
let_(time_Awake_noop, time_Awake) = {
    .ctx = &time_Awake_noop_ctx,
    .vtbl = &time_Awake_VTbl_noop,
};

$static var_(time_Awake_failing_ctx, Void) = cleared();
let_(time_Awake_failing, time_Awake) = {
    .ctx = &time_Awake_failing_ctx,
    .vtbl = &time_Awake_VTbl_failing,
};

fn_((time_Awake_direct(void))(time_direct_E$time_Awake) $scope) {
    pp_if_(time_Awake_direct_supported)(
        pp_then_({
            $static var_(ctx, Void) $like_ref = cleared();
            $static let_(vtbl, time_Awake_VTbl) $like_ref = { {
                .nowFn = time_Awake_direct__now,
                .sleepFn = time_Awake_direct__sleep,
            } };
            return_ok(time_Awake_ensureValid((time_Awake){
                .ctx = &ctx,
                .vtbl = vtbl,
            }));
        }),
        pp_else_({
            return_err(time_direct_E_Unsupported());
        })
    );
} $unscoped(fn);

fn_((time_Awake_evented(exec_Coop* coop))(time_Awake)) {
    $static let_(vtbl, time_Awake_VTbl) $like_ref = { {
        .nowFn = time_Awake_evented__now,
        .sleepFn = time_Awake_evented__sleep,
    } };
    return time_Awake_ensureValid((time_Awake){
        .ctx = coop,
        .vtbl = vtbl,
    });
};

fn_((time_Awake_now(time_Awake self))(time_Awake_Inst)) {
    self = time_Awake_ensureValid(self);
    return self.vtbl->nowFn(self.ctx);
};

fn_((time_Awake_sleep(time_Awake self, time_Dur dur))(Sched_Cancelable$void)) {
    self = time_Awake_ensureValid(self);
    return self.vtbl->sleepFn(self.ctx, dur);
};

fn_((time_Awake_sleepSecs(time_Awake self, u64 secs))(Sched_Cancelable$void)) {
    return time_Awake_sleep(self, time_Dur_fromSecs(secs));
};

fn_((time_Awake_sleepMillis(time_Awake self, u64 millis))(Sched_Cancelable$void)) {
    return time_Awake_sleep(self, time_Dur_fromMillis(millis));
};

fn_((time_Awake_sleepMicros(time_Awake self, u64 micros))(Sched_Cancelable$void)) {
    return time_Awake_sleep(self, time_Dur_fromMicros(micros));
};

fn_((time_Awake_sleepNanos(time_Awake self, u32 nanos))(Sched_Cancelable$void)) {
    return time_Awake_sleep(self, time_Dur_fromNanos(nanos));
};

fn_((time_Awake_Inst_elapsed(time_Awake_Inst self, time_Awake time))(time_Dur)) {
    return time_Awake_Inst_durSince(time_Awake_now(time), self);
};

fn_((time_Awake_Inst_durSince(time_Awake_Inst later, time_Awake_Inst earlier))(time_Dur)) {
    return time_Inst_durSince(later.raw, earlier.raw);
};

fn_((time_Awake_Inst_durSinceChkd(time_Awake_Inst later, time_Awake_Inst earlier))(O$time_Dur)) {
    return time_Inst_durSinceChkd(later.raw, earlier.raw);
};

op_fn_addWith$(((time_Awake_Inst, time_Dur)(lhs, rhs))(time_Awake_Inst)) {
    return unwrap_(time_Awake_Inst_addChkdDur(lhs, rhs));
};

op_fn_addAsgWith$(((time_Awake_Inst, time_Dur)(lhs, rhs))(time_Awake_Inst*)) {
    return *lhs = unwrap_(time_Awake_Inst_addChkdDur(*lhs, rhs)), lhs;
};

op_fn_subWith$(((time_Awake_Inst, time_Dur)(lhs, rhs))(time_Awake_Inst)) {
    return unwrap_(time_Awake_Inst_subChkdDur(lhs, rhs));
};

op_fn_subAsgWith$(((time_Awake_Inst, time_Dur)(lhs, rhs))(time_Awake_Inst*)) {
    return *lhs = unwrap_(time_Awake_Inst_subChkdDur(*lhs, rhs)), lhs;
};

fn_((time_Awake_Inst_addChkdDur(time_Awake_Inst lhs, time_Dur rhs))(O$time_Awake_Inst) $scope) {
    let raw = orelse_((time_Inst_addChkdDur(lhs.raw, rhs))(return_none()));
    return_some((time_Awake_Inst){ .raw = raw });
} $unscoped(fn);

fn_((time_Awake_Inst_subChkdDur(time_Awake_Inst lhs, time_Dur rhs))(O$time_Awake_Inst) $scope) {
    let raw = orelse_((time_Inst_subChkdDur(lhs.raw, rhs))(return_none()));
    return_some((time_Awake_Inst){ .raw = raw });
} $unscoped(fn);

cmp_fn_ord$((time_Awake_Inst)(lhs, rhs)) {
    return cmp_ord$(time_Inst)(lhs.raw, rhs.raw);
};
cmp_fn_eq_default$((time_Awake_Inst)(lhs, rhs));
cmp_fn_ne_default$((time_Awake_Inst)(lhs, rhs));
cmp_fn_lt_default$((time_Awake_Inst)(lhs, rhs));
cmp_fn_gt_default$((time_Awake_Inst)(lhs, rhs));
cmp_fn_le_default$((time_Awake_Inst)(lhs, rhs));
cmp_fn_ge_default$((time_Awake_Inst)(lhs, rhs));
cmp_fn_ordCtx$((time_Awake_Inst)(lhs, rhs, ctx)) {
    return $ignore_void ctx, cmp_ord$(time_Awake_Inst)(lhs, rhs);
};
cmp_fn_eqCtx_default$((time_Awake_Inst)(lhs, rhs, ctx));
cmp_fn_neCtx_default$((time_Awake_Inst)(lhs, rhs, ctx));
cmp_fn_ltCtx_default$((time_Awake_Inst)(lhs, rhs, ctx));
cmp_fn_gtCtx_default$((time_Awake_Inst)(lhs, rhs, ctx));
cmp_fn_leCtx_default$((time_Awake_Inst)(lhs, rhs, ctx));
cmp_fn_geCtx_default$((time_Awake_Inst)(lhs, rhs, ctx));
cmp_fn_eql$((time_Awake_Inst)(lhs, rhs)) {
    return cmp_ord$(time_Awake_Inst)(lhs, rhs) == cmp_Ord_eq;
};
cmp_fn_neq_default$((time_Awake_Inst)(lhs, rhs));
cmp_fn_eqlCtx$((time_Awake_Inst)(lhs, rhs, ctx)) {
    return $ignore_void ctx, cmp_eql$(time_Awake_Inst)(lhs, rhs);
};
cmp_fn_neqCtx_default$((time_Awake_Inst)(lhs, rhs, ctx));

fn_((time_Awake_VTbl_noNow(P$raw ctx))(time_Awake_Inst)) {
    let_ignore = ctx;
    return (time_Awake_Inst){ .raw = time_Inst_from(0, 0) };
};

fn_((time_Awake_VTbl_unreachableNow(P$raw ctx))(time_Awake_Inst)) {
    let_ignore = ctx;
    claim_unreachable_msg("Awake time source is unavailable");
};

fn_((time_Awake_VTbl_failingSleep(P$raw ctx, time_Dur dur))(Sched_Cancelable$void) $scope) {
    let_ignore = ctx;
    let_ignore = dur;
    return_err(Sched_Cancelable_Canceled());
} $unscoped(fn);

/*========== Source Definitions =============================================*/

fn_((time_Awake_direct__unsupported_now(P$raw ctx))(time_Awake_Inst)) {
    let_ignore = ctx;
    claim_unreachable_msg("Awake direct time source is unavailable on this platform");
};

pp_if_(plat_is_windows)(pp_then_(
    fn_((time_Awake_direct__windows_now(P$raw ctx))(time_Awake_Inst)) {
        var_(counter, LARGE_INTEGER) = cleared();
        let_ignore = ctx;
        QueryPerformanceCounter(&counter);
        let freq = time__windows_qpcFreq();
        let ticks = as$(u64)(counter.QuadPart);
        let secs = ticks / freq;
        let nanos = as$(u32)(((ticks % freq) * as$(u64)(time_nanos_per_sec)) / freq);
        return (time_Awake_Inst){ .raw = time_Inst_from(secs, nanos) };
    };
));

#if plat_based_unix
fn_((time_Awake_direct__unix__clockId(void))(clockid_t)) {
#if defined(CLOCK_UPTIME_RAW) && plat_is_darwin
    return CLOCK_UPTIME_RAW;
#else
    return CLOCK_MONOTONIC;
#endif
};

fn_((time_Awake_direct__unix_now(P$raw ctx))(time_Awake_Inst)) {
    var_(now, struct timespec) = cleared();
    let_ignore = ctx;
    clock_gettime(time_Awake_direct__unix__clockId(), &now);
    return (time_Awake_Inst){ .raw = time__unix_fromTimespec(now) };
};
#endif /* plat_based_unix */

fn_((time_Awake_direct__sleep(P$raw ctx, time_Dur dur))(Sched_Cancelable$void)) {
    let_ignore = ctx;
    return time__direct_sleep(dur);
};

fn_((time_Awake_evented__now(P$raw ctx))(time_Awake_Inst)) {
    let self = as$(exec_Coop*)(ctx);
    return time_Awake_now(self->timed.clock);
};

fn_((time_Awake_evented__sleep(P$raw ctx, time_Dur dur))(Sched_Cancelable$void) $scope) {
    let self = as$(exec_Coop*)(ctx);
    let timed = &self->timed;
    let lane = &self->timed.lane;
    let deadline = orelse_((time_Awake_Inst_addChkdDur(time_Awake_now(self->timed.clock), dur))(
        return_ok({})
    ));

    if_some((lane->task_curr)(task)) {
        task->state = exec_Task_State_waiting;
        let_(timer, exec_Timer) = {
            .deadline = deadline.raw,
            .task = task,
        };
        let_ignore = catch_((ArrPQue_enque$exec_Timer(&timed->tasks_timer, lane->gpa, timer))($ignore, {
            task->state = exec_Task_State_canceled;
            return_err(Sched_Cancelable_Canceled());
        }));
        return_ok_void(exec_Lane_yield(lane));
    }

    return_ok_void(exec_LaneTimed_runUntil(timed, deadline));
} $unscoped(fn);

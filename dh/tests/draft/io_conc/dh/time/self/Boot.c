#include "Boot.h"
#include "internal.h"

/*========== Internal Declarations ==========================================*/

$attr($maybe_unused)
$static fn_((time_Boot_direct__unsupported_now(P$raw ctx))(time_Boot_Inst));
$attr($must_check)
$static fn_((time_Boot_direct__sleep(P$raw ctx, time_Dur dur))(Sched_Cancelable$void));
pp_if_(plat_is_windows)(pp_then_(
    $static fn_((time_Boot_direct__windows_now(P$raw ctx))(time_Boot_Inst));
));
pp_if_(plat_based_unix)(pp_then_(
    $static fn_((time_Boot_direct__unix__clockId(void))(clockid_t));
    $static fn_((time_Boot_direct__unix_now(P$raw ctx))(time_Boot_Inst));
));

$static let time_Boot_direct__now = pp_if_(plat_is_windows)(
    pp_then_(time_Boot_direct__windows_now),
    pp_else_(pp_if_(plat_based_unix)(
        pp_then_(time_Boot_direct__unix_now),
        pp_else_(time_Boot_direct__unsupported_now)
    )));

/*========== External Definitions ===========================================*/

let_(time_Boot_VTbl_noop, time_Boot_VTbl) = {
    .nowFn = time_Boot_VTbl_noNow,
    .sleepFn = time_Boot_VTbl_failingSleep,
};

let_(time_Boot_VTbl_failing, time_Boot_VTbl) = {
    .nowFn = time_Boot_VTbl_unreachableNow,
    .sleepFn = time_Boot_VTbl_failingSleep,
};

$static var_(time_Boot_noop_ctx, Void) = cleared();
let_(time_Boot_noop, time_Boot) = {
    .ctx = &time_Boot_noop_ctx,
    .vtbl = &time_Boot_VTbl_noop,
};

$static var_(time_Boot_failing_ctx, Void) = cleared();
let_(time_Boot_failing, time_Boot) = {
    .ctx = &time_Boot_failing_ctx,
    .vtbl = &time_Boot_VTbl_failing,
};

fn_((time_Boot_direct(void))(time_direct_E$time_Boot) $scope) {
    pp_if_(time_Boot_direct_supported)(
        pp_then_({
            $static var_(ctx, Void) $like_ref = cleared();
            $static let_(vtbl, time_Boot_VTbl) $like_ref = { {
                .nowFn = time_Boot_direct__now,
                .sleepFn = time_Boot_direct__sleep,
            } };
            return_ok(time_Boot_ensureValid((time_Boot){
                .ctx = &ctx,
                .vtbl = vtbl,
            }));
        }),
        pp_else_({
            return_err(time_direct_E_Unsupported());
        })
    );
} $unscoped(fn);

fn_((time_Boot_now(time_Boot self))(time_Boot_Inst)) {
    self = time_Boot_ensureValid(self);
    return self.vtbl->nowFn(self.ctx);
};

fn_((time_Boot_sleep(time_Boot self, time_Dur dur))(Sched_Cancelable$void)) {
    self = time_Boot_ensureValid(self);
    return self.vtbl->sleepFn(self.ctx, dur);
};

fn_((time_Boot_sleepSecs(time_Boot self, u64 secs))(Sched_Cancelable$void)) {
    return time_Boot_sleep(self, time_Dur_fromSecs(secs));
};

fn_((time_Boot_sleepMillis(time_Boot self, u64 millis))(Sched_Cancelable$void)) {
    return time_Boot_sleep(self, time_Dur_fromMillis(millis));
};

fn_((time_Boot_sleepMicros(time_Boot self, u64 micros))(Sched_Cancelable$void)) {
    return time_Boot_sleep(self, time_Dur_fromMicros(micros));
};

fn_((time_Boot_sleepNanos(time_Boot self, u32 nanos))(Sched_Cancelable$void)) {
    return time_Boot_sleep(self, time_Dur_fromNanos(nanos));
};

fn_((time_Boot_Inst_elapsed(time_Boot_Inst self, time_Boot time))(time_Dur)) {
    return time_Boot_Inst_durSince(time_Boot_now(time), self);
};

fn_((time_Boot_Inst_durSince(time_Boot_Inst later, time_Boot_Inst earlier))(time_Dur)) {
    return time_Inst_durSince(later.raw, earlier.raw);
};

fn_((time_Boot_Inst_durSinceChkd(time_Boot_Inst later, time_Boot_Inst earlier))(O$time_Dur)) {
    return time_Inst_durSinceChkd(later.raw, earlier.raw);
};

op_fn_addWith$(((time_Boot_Inst, time_Dur)(lhs, rhs))(time_Boot_Inst)) {
    return unwrap_(time_Boot_Inst_addChkdDur(lhs, rhs));
};

op_fn_addAsgWith$(((time_Boot_Inst, time_Dur)(lhs, rhs))(time_Boot_Inst*)) {
    return *lhs = unwrap_(time_Boot_Inst_addChkdDur(*lhs, rhs)), lhs;
};

op_fn_subWith$(((time_Boot_Inst, time_Dur)(lhs, rhs))(time_Boot_Inst)) {
    return unwrap_(time_Boot_Inst_subChkdDur(lhs, rhs));
};

op_fn_subAsgWith$(((time_Boot_Inst, time_Dur)(lhs, rhs))(time_Boot_Inst*)) {
    return *lhs = unwrap_(time_Boot_Inst_subChkdDur(*lhs, rhs)), lhs;
};

fn_((time_Boot_Inst_addChkdDur(time_Boot_Inst lhs, time_Dur rhs))(O$time_Boot_Inst) $scope) {
    let raw = orelse_((time_Inst_addChkdDur(lhs.raw, rhs))(return_none()));
    return_some((time_Boot_Inst){ .raw = raw });
} $unscoped(fn);

fn_((time_Boot_Inst_subChkdDur(time_Boot_Inst lhs, time_Dur rhs))(O$time_Boot_Inst) $scope) {
    let raw = orelse_((time_Inst_subChkdDur(lhs.raw, rhs))(return_none()));
    return_some((time_Boot_Inst){ .raw = raw });
} $unscoped(fn);

cmp_fn_ord$((time_Boot_Inst)(lhs, rhs)) {
    return cmp_ord$(time_Inst)(lhs.raw, rhs.raw);
};
cmp_fn_eq_default$((time_Boot_Inst)(lhs, rhs));
cmp_fn_ne_default$((time_Boot_Inst)(lhs, rhs));
cmp_fn_lt_default$((time_Boot_Inst)(lhs, rhs));
cmp_fn_gt_default$((time_Boot_Inst)(lhs, rhs));
cmp_fn_le_default$((time_Boot_Inst)(lhs, rhs));
cmp_fn_ge_default$((time_Boot_Inst)(lhs, rhs));
cmp_fn_ordCtx$((time_Boot_Inst)(lhs, rhs, ctx)) {
    return $ignore_void ctx, cmp_ord$(time_Boot_Inst)(lhs, rhs);
};
cmp_fn_eqCtx_default$((time_Boot_Inst)(lhs, rhs, ctx));
cmp_fn_neCtx_default$((time_Boot_Inst)(lhs, rhs, ctx));
cmp_fn_ltCtx_default$((time_Boot_Inst)(lhs, rhs, ctx));
cmp_fn_gtCtx_default$((time_Boot_Inst)(lhs, rhs, ctx));
cmp_fn_leCtx_default$((time_Boot_Inst)(lhs, rhs, ctx));
cmp_fn_geCtx_default$((time_Boot_Inst)(lhs, rhs, ctx));
cmp_fn_eql$((time_Boot_Inst)(lhs, rhs)) {
    return cmp_ord$(time_Boot_Inst)(lhs, rhs) == cmp_Ord_eq;
};
cmp_fn_neq_default$((time_Boot_Inst)(lhs, rhs));
cmp_fn_eqlCtx$((time_Boot_Inst)(lhs, rhs, ctx)) {
    return $ignore_void ctx, cmp_eql$(time_Boot_Inst)(lhs, rhs);
};
cmp_fn_neqCtx_default$((time_Boot_Inst)(lhs, rhs, ctx));

fn_((time_Boot_VTbl_noNow(P$raw ctx))(time_Boot_Inst)) {
    let_ignore = ctx;
    return (time_Boot_Inst){ .raw = time_Inst_from(0, 0) };
};

fn_((time_Boot_VTbl_unreachableNow(P$raw ctx))(time_Boot_Inst)) {
    let_ignore = ctx;
    claim_unreachable_msg("Boot time source is unavailable");
};

fn_((time_Boot_VTbl_failingSleep(P$raw ctx, time_Dur dur))(Sched_Cancelable$void) $scope) {
    let_ignore = ctx;
    let_ignore = dur;
    return_err(Sched_Cancelable_Canceled());
} $unscoped(fn);

/*========== Direct Source Definitions ======================================*/

fn_((time_Boot_direct__unsupported_now(P$raw ctx))(time_Boot_Inst)) {
    let_ignore = ctx;
    claim_unreachable_msg("Boot direct time source is unavailable on this platform");
};

pp_if_(plat_is_windows)(pp_then_(
    fn_((time_Boot_direct__windows_now(P$raw ctx))(time_Boot_Inst)) {
        let millis = GetTickCount64();
        let_ignore = ctx;
        return (time_Boot_Inst){
            .raw = time_Inst_from(
                millis / time_millis_per_sec,
                as$(u32)((millis % time_millis_per_sec) * time_nanos_per_milli)
            )
        };
    };
));

#if plat_based_unix
fn_((time_Boot_direct__unix__clockId(void))(clockid_t)) {
#if defined(CLOCK_BOOTTIME)
    return CLOCK_BOOTTIME;
#elif defined(CLOCK_MONOTONIC_RAW)
    return CLOCK_MONOTONIC_RAW;
#else
    return CLOCK_MONOTONIC;
#endif
};

fn_((time_Boot_direct__unix_now(P$raw ctx))(time_Boot_Inst)) {
    var_(now, struct timespec) = cleared();
    let_ignore = ctx;
    clock_gettime(time_Boot_direct__unix__clockId(), &now);
    return (time_Boot_Inst){ .raw = time__unix_fromTimespec(now) };
};
#endif /* plat_based_unix */

fn_((time_Boot_direct__sleep(P$raw ctx, time_Dur dur))(Sched_Cancelable$void)) {
    let_ignore = ctx;
    return time__direct_sleep(dur);
};

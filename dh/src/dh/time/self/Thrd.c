#include "dh/time/self/Thrd.h"
#include "private/share.h"

/*========== Internal Declarations ==========================================*/

$attr($maybe_unused)
$static fn_((time_thrd_direct__unsupported_now(P$raw ctx))(time_thrd_Inst));
pp_if_(plat_is_windows)(pp_then_(
    $static fn_((time_thrd_direct__windows_now(P$raw ctx))(time_thrd_Inst));
));
pp_if_(plat_based_unix)(pp_then_(
    $static fn_((time_thrd_direct__unix_now(P$raw ctx))(time_thrd_Inst));
));

$static let time_thrd_direct__now = pp_if_(plat_is_windows)(
    pp_then_(time_thrd_direct__windows_now),
    pp_else_(pp_if_(plat_based_unix)(
        pp_then_(time_thrd_direct__unix_now),
        pp_else_(time_thrd_direct__unsupported_now)
    ))
);

/*========== External Definitions ===========================================*/

let_(time_thrd_VTbl_noop, time_thrd_VTbl) = {
    .nowFn = time_thrd_VTbl_noNow,
};

let_(time_thrd_VTbl_failing, time_thrd_VTbl) = {
    .nowFn = time_thrd_VTbl_unreachableNow,
};

$static var_(time_thrd_noop_ctx, Void) = cleared();
let_(time_thrd_noop, time_Thrd) = {
    .ctx = &time_thrd_noop_ctx,
    .vtbl = &time_thrd_VTbl_noop,
};

$static var_(time_thrd_failing_ctx, Void) = cleared();
let_(time_thrd_failing, time_Thrd) = {
    .ctx = &time_thrd_failing_ctx,
    .vtbl = &time_thrd_VTbl_failing,
};

fn_((time_thrd_direct(void))(time_direct_E$time_Thrd) $scope) {
    pp_if_(time_thrd_direct_supported)(
        pp_then_({
            $static var_(ctx, Void) $like_ref = cleared();
            $static let_(vtbl, time_thrd_VTbl) $like_ref = { {
                .nowFn = time_thrd_direct__now,
            } };
            return_ok(time_thrd_ensureValid((time_Thrd){
                .ctx = &ctx,
                .vtbl = vtbl,
            }));
        }),
        pp_else_({
            return_err(E_cause$time_direct_Unsupported());
        })
    );
} $unscoped(fn);

fn_((time_thrd_now(time_Thrd self))(time_thrd_Inst)) {
    self = time_thrd_ensureValid(self);
    return self.vtbl->nowFn(self.ctx);
};

fn_((time_thrd_Inst_elapsed(time_thrd_Inst self, time_Thrd time))(time_Dur)) {
    return time_thrd_Inst_durSince(time_thrd_now(time), self);
};

fn_((time_thrd_Inst_durSince(time_thrd_Inst later, time_thrd_Inst earlier))(time_Dur)) {
    return time_Inst_durSince(later.raw, earlier.raw);
};

fn_((time_thrd_Inst_durSinceChkd(time_thrd_Inst later, time_thrd_Inst earlier))(O$time_Dur)) {
    return time_Inst_durSinceChkd(later.raw, earlier.raw);
};

op_fn_addWith$(((time_thrd_Inst, time_Dur)(lhs, rhs))(time_thrd_Inst)) {
    return unwrap_(time_thrd_Inst_addChkdDur(lhs, rhs));
};

op_fn_addAsgWith$(((time_thrd_Inst, time_Dur)(lhs, rhs))(time_thrd_Inst*)) {
    return *lhs = unwrap_(time_thrd_Inst_addChkdDur(*lhs, rhs)), lhs;
};

op_fn_subWith$(((time_thrd_Inst, time_Dur)(lhs, rhs))(time_thrd_Inst)) {
    return unwrap_(time_thrd_Inst_subChkdDur(lhs, rhs));
};

op_fn_subAsgWith$(((time_thrd_Inst, time_Dur)(lhs, rhs))(time_thrd_Inst*)) {
    return *lhs = unwrap_(time_thrd_Inst_subChkdDur(*lhs, rhs)), lhs;
};

fn_((time_thrd_Inst_addChkdDur(time_thrd_Inst lhs, time_Dur rhs))(O$time_thrd_Inst) $scope) {
    let raw = orelse_((time_Inst_addChkdDur(lhs.raw, rhs))(return_none()));
    return_some((time_thrd_Inst){ .raw = raw });
} $unscoped(fn);

fn_((time_thrd_Inst_subChkdDur(time_thrd_Inst lhs, time_Dur rhs))(O$time_thrd_Inst) $scope) {
    let raw = orelse_((time_Inst_subChkdDur(lhs.raw, rhs))(return_none()));
    return_some((time_thrd_Inst){ .raw = raw });
} $unscoped(fn);

cmp_fn_ord$((time_thrd_Inst)(lhs, rhs)) {
    return cmp_ord$(time_Inst)(lhs.raw, rhs.raw);
};
cmp_fn_eq_default$((time_thrd_Inst)(lhs, rhs));
cmp_fn_ne_default$((time_thrd_Inst)(lhs, rhs));
cmp_fn_lt_default$((time_thrd_Inst)(lhs, rhs));
cmp_fn_gt_default$((time_thrd_Inst)(lhs, rhs));
cmp_fn_le_default$((time_thrd_Inst)(lhs, rhs));
cmp_fn_ge_default$((time_thrd_Inst)(lhs, rhs));
cmp_fn_ordCtx$((time_thrd_Inst)(lhs, rhs, ctx)) {
    return $ignore_void ctx, cmp_ord$(time_thrd_Inst)(lhs, rhs);
};
cmp_fn_eqCtx_default$((time_thrd_Inst)(lhs, rhs, ctx));
cmp_fn_neCtx_default$((time_thrd_Inst)(lhs, rhs, ctx));
cmp_fn_ltCtx_default$((time_thrd_Inst)(lhs, rhs, ctx));
cmp_fn_gtCtx_default$((time_thrd_Inst)(lhs, rhs, ctx));
cmp_fn_leCtx_default$((time_thrd_Inst)(lhs, rhs, ctx));
cmp_fn_geCtx_default$((time_thrd_Inst)(lhs, rhs, ctx));
cmp_fn_eql$((time_thrd_Inst)(lhs, rhs)) {
    return cmp_ord$(time_thrd_Inst)(lhs, rhs) == cmp_Ord_eq;
};
cmp_fn_neq_default$((time_thrd_Inst)(lhs, rhs));
cmp_fn_eqlCtx$((time_thrd_Inst)(lhs, rhs, ctx)) {
    return $ignore_void ctx, cmp_eql$(time_thrd_Inst)(lhs, rhs);
};
cmp_fn_neqCtx_default$((time_thrd_Inst)(lhs, rhs, ctx));

fn_((time_thrd_VTbl_noNow(P$raw ctx))(time_thrd_Inst)) {
    let_ignore = ctx;
    return (time_thrd_Inst){ .raw = time_Inst_from(0, 0) };
};

fn_((time_thrd_VTbl_unreachableNow(P$raw ctx))(time_thrd_Inst)) {
    let_ignore = ctx;
    claim_unreachable_msg("Thread time source is unavailable");
};

/*========== Direct Source Definitions ======================================*/

fn_((time_thrd_direct__unsupported_now(P$raw ctx))(time_thrd_Inst)) {
    let_ignore = ctx;
    claim_unreachable_msg("Thread direct time source is unavailable on this platform");
};

pp_if_(plat_is_windows)(pp_then_(
fn_((time_thrd_direct__windows_now(P$raw ctx))(time_thrd_Inst)) {
    var_(create_time, FILETIME) = cleared();
    var_(exit_time, FILETIME) = cleared();
    var_(kernel_time, FILETIME) = cleared();
    var_(user_time, FILETIME) = cleared();
    let_ignore = ctx;
    if (!GetThreadTimes(GetCurrentThread(), &create_time, &exit_time, &kernel_time, &user_time)) {
        claim_unreachable_msg("Failed to query thread cpu time");
    }
    let kernel = time__windows_fromFileTime(kernel_time);
    let user = time__windows_fromFileTime(user_time);
    return (time_thrd_Inst){
        .raw = unwrap_(time_Inst_addChkdDur(
            kernel, (time_Dur){ .secs = user.secs, .nanos = user.nanos }
        )),
    };
};
));

#if plat_based_unix
fn_((time_thrd_direct__unix_now(P$raw ctx))(time_thrd_Inst)) {
    var_(now, struct timespec) = cleared();
    let_ignore = ctx;
#if defined(CLOCK_THREAD_CPUTIME_ID)
    clock_gettime(CLOCK_THREAD_CPUTIME_ID, &now);
    return (time_thrd_Inst){ .raw = time__unix_fromTimespec(now) };
#else
    claim_unreachable_msg("Thread cpu time is unavailable on this platform");
#endif
};
#endif /* plat_based_unix */

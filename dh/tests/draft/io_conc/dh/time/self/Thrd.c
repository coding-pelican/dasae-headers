#include "Thrd.h"
#include "internal.h"

/*========== Internal Declarations ==========================================*/

$attr($maybe_unused)
$static fn_((time_Thrd_direct__unsupported_now(P$raw ctx))(time_Thrd_Inst));
pp_if_(plat_is_windows)(pp_then_(
    $static fn_((time_Thrd_direct__windows_now(P$raw ctx))(time_Thrd_Inst));
));
pp_if_(plat_based_unix)(pp_then_(
    $static fn_((time_Thrd_direct__unix_now(P$raw ctx))(time_Thrd_Inst));
));

$static let time_Thrd_direct__now = pp_if_(plat_is_windows)(
    pp_then_(time_Thrd_direct__windows_now),
    pp_else_(pp_if_(plat_based_unix)(
        pp_then_(time_Thrd_direct__unix_now),
        pp_else_(time_Thrd_direct__unsupported_now)
    ))
);

/*========== External Definitions ===========================================*/

let_(time_Thrd_VTbl_noop, time_Thrd_VTbl) = {
    .nowFn = time_Thrd_VTbl_noNow,
};

let_(time_Thrd_VTbl_failing, time_Thrd_VTbl) = {
    .nowFn = time_Thrd_VTbl_unreachableNow,
};

fn_((time_Thrd_VTbl_noNow(P$raw ctx))(time_Thrd_Inst)) {
    let_ignore = ctx;
    return (time_Thrd_Inst){ .raw = time_Inst_from(0, 0) };
};

fn_((time_Thrd_VTbl_unreachableNow(P$raw ctx))(time_Thrd_Inst)) {
    let_ignore = ctx;
    claim_unreachable_msg("Thread time source is unavailable");
};

$static var_(time_Thrd_noop_ctx, Void) = cleared();
let_(time_Thrd_noop, time_Thrd) = {
    .ctx = &time_Thrd_noop_ctx,
    .vtbl = &time_Thrd_VTbl_noop,
};

$static var_(time_Thrd_failing_ctx, Void) = cleared();
let_(time_Thrd_failing, time_Thrd) = {
    .ctx = &time_Thrd_failing_ctx,
    .vtbl = &time_Thrd_VTbl_failing,
};

fn_((time_Thrd_direct(void))(time_E$time_Thrd) $scope) {
    pp_if_(time_Thrd_direct_supported)(
        pp_then_({
            $static var_(ctx, Void) $like_ref = cleared();
            $static let_(vtbl, time_Thrd_VTbl) $like_ref = { {
                .nowFn = time_Thrd_direct__now,
            } };
            return_ok(time_Thrd_ensureValid((time_Thrd){
                .ctx = &ctx,
                .vtbl = vtbl,
            }));
        }),
        pp_else_({
            return_err(time_E_Unsupported());
        })
    );
} $unscoped(fn);

fn_((time_Thrd_now(time_Thrd self))(time_Thrd_Inst)) {
    self = time_Thrd_ensureValid(self);
    return self.vtbl->nowFn(self.ctx);
};

fn_((time_Thrd_Inst_elapsed(time_Thrd_Inst self, time_Thrd time))(time_Dur)) {
    return time_Thrd_Inst_durSince(time_Thrd_now(time), self);
};

fn_((time_Thrd_Inst_durSince(time_Thrd_Inst later, time_Thrd_Inst earlier))(time_Dur)) {
    return time_Inst_durSince(later.raw, earlier.raw);
};

fn_((time_Thrd_Inst_durSinceChkd(time_Thrd_Inst later, time_Thrd_Inst earlier))(O$time_Dur)) {
    return time_Inst_durSinceChkd(later.raw, earlier.raw);
};

op_fn_addWith$(((time_Thrd_Inst, time_Dur)(lhs, rhs))(time_Thrd_Inst)) {
    return unwrap_(time_Thrd_Inst_addChkdDur(lhs, rhs));
};

op_fn_addAsgWith$(((time_Thrd_Inst, time_Dur)(lhs, rhs))(time_Thrd_Inst*)) {
    return *lhs = unwrap_(time_Thrd_Inst_addChkdDur(*lhs, rhs)), lhs;
};

op_fn_subWith$(((time_Thrd_Inst, time_Dur)(lhs, rhs))(time_Thrd_Inst)) {
    return unwrap_(time_Thrd_Inst_subChkdDur(lhs, rhs));
};

op_fn_subAsgWith$(((time_Thrd_Inst, time_Dur)(lhs, rhs))(time_Thrd_Inst*)) {
    return *lhs = unwrap_(time_Thrd_Inst_subChkdDur(*lhs, rhs)), lhs;
};

fn_((time_Thrd_Inst_addChkdDur(time_Thrd_Inst lhs, time_Dur rhs))(O$time_Thrd_Inst) $scope) {
    let raw = orelse_((time_Inst_addChkdDur(lhs.raw, rhs))(return_none()));
    return_some((time_Thrd_Inst){ .raw = raw });
} $unscoped(fn);

fn_((time_Thrd_Inst_subChkdDur(time_Thrd_Inst lhs, time_Dur rhs))(O$time_Thrd_Inst) $scope) {
    let raw = orelse_((time_Inst_subChkdDur(lhs.raw, rhs))(return_none()));
    return_some((time_Thrd_Inst){ .raw = raw });
} $unscoped(fn);

cmp_fn_ord$((time_Thrd_Inst)(lhs, rhs)) {
    return cmp_ord$(time_Inst)(lhs.raw, rhs.raw);
};
cmp_fn_eq_default$((time_Thrd_Inst)(lhs, rhs));
cmp_fn_ne_default$((time_Thrd_Inst)(lhs, rhs));
cmp_fn_lt_default$((time_Thrd_Inst)(lhs, rhs));
cmp_fn_gt_default$((time_Thrd_Inst)(lhs, rhs));
cmp_fn_le_default$((time_Thrd_Inst)(lhs, rhs));
cmp_fn_ge_default$((time_Thrd_Inst)(lhs, rhs));
cmp_fn_ordCtx$((time_Thrd_Inst)(lhs, rhs, ctx)) {
    return $ignore_void ctx, cmp_ord$(time_Thrd_Inst)(lhs, rhs);
};
cmp_fn_eqCtx_default$((time_Thrd_Inst)(lhs, rhs, ctx));
cmp_fn_neCtx_default$((time_Thrd_Inst)(lhs, rhs, ctx));
cmp_fn_ltCtx_default$((time_Thrd_Inst)(lhs, rhs, ctx));
cmp_fn_gtCtx_default$((time_Thrd_Inst)(lhs, rhs, ctx));
cmp_fn_leCtx_default$((time_Thrd_Inst)(lhs, rhs, ctx));
cmp_fn_geCtx_default$((time_Thrd_Inst)(lhs, rhs, ctx));
cmp_fn_eql$((time_Thrd_Inst)(lhs, rhs)) {
    return cmp_ord$(time_Thrd_Inst)(lhs, rhs) == cmp_Ord_eq;
};
cmp_fn_neq_default$((time_Thrd_Inst)(lhs, rhs));
cmp_fn_eqlCtx$((time_Thrd_Inst)(lhs, rhs, ctx)) {
    return $ignore_void ctx, cmp_eql$(time_Thrd_Inst)(lhs, rhs);
};
cmp_fn_neqCtx_default$((time_Thrd_Inst)(lhs, rhs, ctx));

/*========== Direct Source Definitions ======================================*/

fn_((time_Thrd_direct__unsupported_now(P$raw ctx))(time_Thrd_Inst)) {
    let_ignore = ctx;
    claim_unreachable_msg("Thread direct time source is unavailable on this platform");
};

pp_if_(plat_is_windows)(pp_then_(
fn_((time_Thrd_direct__windows_now(P$raw ctx))(time_Thrd_Inst)) {
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
    return (time_Thrd_Inst){
        .raw = unwrap_(time_Inst_addChkdDur(
            kernel, (time_Dur){ .secs = user.secs, .nanos = user.nanos }
        )),
    };
};
));

#if plat_based_unix
fn_((time_Thrd_direct__unix_now(P$raw ctx))(time_Thrd_Inst)) {
    var_(now, struct timespec) = cleared();
    let_ignore = ctx;
#if defined(CLOCK_THREAD_CPUTIME_ID)
    clock_gettime(CLOCK_THREAD_CPUTIME_ID, &now);
    return (time_Thrd_Inst){ .raw = time__unix_fromTimespec(now) };
#else
    claim_unreachable_msg("Thread cpu time is unavailable on this platform");
#endif
};
#endif /* plat_based_unix */

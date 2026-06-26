#include "dh/time/self/Real.h"
#include "private/share.h"

/*========== Internal Declarations ==========================================*/

$attr($maybe_unused)
$static fn_((time_Real_direct__unsupported_now(P$raw ctx))(time_Real_Inst));
$attr($maybe_unused $must_check)
$static fn_((time_Real_direct__unsupported_resolution(P$raw ctx))(time_ResolutionE$time_Resolution));
pp_if_(plat_is_windows)(pp_then_(
    $static fn_((time_Real_direct__windows_now(P$raw ctx))(time_Real_Inst));
    $attr($must_check)
    $static fn_((time_Real_direct__windows_resolution(P$raw ctx))(time_ResolutionE$time_Resolution));
));
pp_if_(plat_based_unix)(pp_then_(
    $static fn_((time_Real_direct__unix_now(P$raw ctx))(time_Real_Inst));
    $attr($must_check)
    $static fn_((time_Real_direct__unix_resolution(P$raw ctx))(time_ResolutionE$time_Resolution));
));

$static let time_Real_direct__now = pp_if_(plat_is_windows)(
    pp_then_(time_Real_direct__windows_now),
    pp_else_(pp_if_(plat_based_unix)(
        pp_then_(time_Real_direct__unix_now),
        pp_else_(time_Real_direct__unsupported_now)
    )));
$static let time_Real_direct__resolution = pp_if_(plat_is_windows)(
    pp_then_(time_Real_direct__windows_resolution),
    pp_else_(pp_if_(plat_based_unix)(
        pp_then_(time_Real_direct__unix_resolution),
        pp_else_(time_Real_direct__unsupported_resolution)
    )));

/*========== External Definitions ===========================================*/

let_(time_Real_VTbl_noop, time_Real_VTbl) = {
    .nowFn = time_Real_VTbl_noNow,
    .resolutionFn = time_Real_VTbl_failingResolution,
};

let_(time_Real_VTbl_failing, time_Real_VTbl) = {
    .nowFn = time_Real_VTbl_unreachableNow,
    .resolutionFn = time_Real_VTbl_failingResolution,
};

$static var_(time_Real_noop_ctx, Void) = cleared();
let_(time_Real_noop, time_Real) = {
    .ctx = &time_Real_noop_ctx,
    .vtbl = &time_Real_VTbl_noop,
};

$static var_(time_Real_failing_ctx, Void) = cleared();
let_(time_Real_failing, time_Real) = {
    .ctx = &time_Real_failing_ctx,
    .vtbl = &time_Real_VTbl_failing,
};

fn_((time_Real_direct(void))(time_direct_E$time_Real) $scope) {
    pp_if_(time_Real_direct_supported)(
        pp_then_({
            $static var_(ctx, Void) $like_ref = cleared();
            $static let_(vtbl, time_Real_VTbl) $like_ref = { {
                .nowFn = time_Real_direct__now,
                .resolutionFn = time_Real_direct__resolution,
            } };
            return_ok(time_Real_ensureValid((time_Real){
                .ctx = &ctx,
                .vtbl = vtbl,
            }));
        }),
        pp_else_({
            return_err(E_cause$time_direct_Unsupported());
        })
    );
} $unscoped(fn);

fn_((time_Real_now(time_Real self))(time_Real_Inst)) {
    self = time_Real_ensureValid(self);
    return self.vtbl->nowFn(self.ctx);
};

fn_((time_Real_resolution(time_Real self))(time_ResolutionE$time_Resolution)) {
    self = time_Real_ensureValid(self);
    return self.vtbl->resolutionFn(self.ctx);
};

fn_((time_Real_Inst_elapsed(time_Real_Inst self, time_Real time))(time_Dur)) {
    return time_Real_Inst_durSince(time_Real_now(time), self);
};

fn_((time_Real_Inst_durSince(time_Real_Inst later, time_Real_Inst earlier))(time_Dur)) {
    return time_Inst_durSince(later.raw, earlier.raw);
};

fn_((time_Real_Inst_durSinceChkd(time_Real_Inst later, time_Real_Inst earlier))(O$time_Dur)) {
    return time_Inst_durSinceChkd(later.raw, earlier.raw);
};

op_fn_addWith$(((time_Real_Inst, time_Dur)(lhs, rhs))(time_Real_Inst)) {
    return unwrap_(time_Real_Inst_addChkdDur(lhs, rhs));
};

op_fn_addAsgWith$(((time_Real_Inst, time_Dur)(lhs, rhs))(time_Real_Inst*)) {
    return *lhs = unwrap_(time_Real_Inst_addChkdDur(*lhs, rhs)), lhs;
};

op_fn_subWith$(((time_Real_Inst, time_Dur)(lhs, rhs))(time_Real_Inst)) {
    return unwrap_(time_Real_Inst_subChkdDur(lhs, rhs));
};

op_fn_subAsgWith$(((time_Real_Inst, time_Dur)(lhs, rhs))(time_Real_Inst*)) {
    return *lhs = unwrap_(time_Real_Inst_subChkdDur(*lhs, rhs)), lhs;
};

fn_((time_Real_Inst_addChkdDur(time_Real_Inst lhs, time_Dur rhs))(O$time_Real_Inst) $scope) {
    let raw = orelse_((time_Inst_addChkdDur(lhs.raw, rhs))(return_none()));
    return_some((time_Real_Inst){ .raw = raw });
} $unscoped(fn);

fn_((time_Real_Inst_subChkdDur(time_Real_Inst lhs, time_Dur rhs))(O$time_Real_Inst) $scope) {
    let raw = orelse_((time_Inst_subChkdDur(lhs.raw, rhs))(return_none()));
    return_some((time_Real_Inst){ .raw = raw });
} $unscoped(fn);

cmp_fn_ord$((time_Real_Inst)(lhs, rhs)) {
    return cmp_ord$(time_Inst)(lhs.raw, rhs.raw);
};
cmp_fn_eq_default$((time_Real_Inst)(lhs, rhs));
cmp_fn_ne_default$((time_Real_Inst)(lhs, rhs));
cmp_fn_lt_default$((time_Real_Inst)(lhs, rhs));
cmp_fn_gt_default$((time_Real_Inst)(lhs, rhs));
cmp_fn_le_default$((time_Real_Inst)(lhs, rhs));
cmp_fn_ge_default$((time_Real_Inst)(lhs, rhs));
cmp_fn_ordCtx$((time_Real_Inst)(lhs, rhs, ctx)) {
    return $ignore_void ctx, cmp_ord$(time_Real_Inst)(lhs, rhs);
};
cmp_fn_eqCtx_default$((time_Real_Inst)(lhs, rhs, ctx));
cmp_fn_neCtx_default$((time_Real_Inst)(lhs, rhs, ctx));
cmp_fn_ltCtx_default$((time_Real_Inst)(lhs, rhs, ctx));
cmp_fn_gtCtx_default$((time_Real_Inst)(lhs, rhs, ctx));
cmp_fn_leCtx_default$((time_Real_Inst)(lhs, rhs, ctx));
cmp_fn_geCtx_default$((time_Real_Inst)(lhs, rhs, ctx));
cmp_fn_eql$((time_Real_Inst)(lhs, rhs)) {
    return cmp_ord$(time_Real_Inst)(lhs, rhs) == cmp_Ord_eq;
};
cmp_fn_neq_default$((time_Real_Inst)(lhs, rhs));
cmp_fn_eqlCtx$((time_Real_Inst)(lhs, rhs, ctx)) {
    return $ignore_void ctx, cmp_eql$(time_Real_Inst)(lhs, rhs);
};
cmp_fn_neqCtx_default$((time_Real_Inst)(lhs, rhs, ctx));

fn_((time_Real_Inst_fromUnixEpoch(u64 secs))(time_Real_Inst)) {
    return (time_Real_Inst){ .raw = time_Inst_from(secs, 0) };
};

fn_((time_Real_Inst_toUnixEpoch(time_Real_Inst self))(u64)) {
    return self.raw.secs;
};

fn_((time_Real_Inst_durSinceUnixEpoch(time_Real_Inst self))(time_Dur)) {
    return (time_Dur){
        .secs = self.raw.secs,
        .nanos = self.raw.nanos,
    };
};

fn_((time_Real_VTbl_noNow(P$raw ctx))(time_Real_Inst)) {
    let_ignore = ctx;
    return (time_Real_Inst){ .raw = time_Inst_from(0, 0) };
};

fn_((time_Real_VTbl_unreachableNow(P$raw ctx))(time_Real_Inst)) {
    let_ignore = ctx;
    claim_unreachable_msg("Real time source is unavailable");
};

fn_((time_Real_VTbl_failingResolution(P$raw ctx))(time_ResolutionE$time_Resolution) $scope) {
    let_ignore = ctx;
    return_err(E_cause$time_direct_Unsupported());
} $unscoped(fn);

/*========== Direct Source Definitions ======================================*/

fn_((time_Real_direct__unsupported_now(P$raw ctx))(time_Real_Inst)) {
    let_ignore = ctx;
    claim_unreachable_msg("Real direct time source is unavailable on this platform");
};

fn_((time_Real_direct__unsupported_resolution(P$raw ctx))(time_ResolutionE$time_Resolution) $scope) {
    let_ignore = ctx;
    return_err(E_cause$time_direct_Unsupported());
} $unscoped(fn);

pp_if_(plat_is_windows)(pp_then_(
    fn_((time_Real_direct__windows_now(P$raw ctx))(time_Real_Inst)) {
        var_(now, FILETIME) = cleared();
        let_ignore = ctx;
        GetSystemTimeAsFileTime(&now);
        return l$((time_Real_Inst){ .raw = time__windows_fromUnixFileTime(now) });
    };

    fn_((time_Real_direct__windows_resolution(P$raw ctx))(time_ResolutionE$time_Resolution) $scope) {
        let_ignore = ctx;
        return_ok(time_Dur_fromNanos(100));
    } $unscoped(fn);
));

pp_if_(plat_based_unix)(pp_then_(
    fn_((time_Real_direct__unix_now(P$raw ctx))(time_Real_Inst)) {
        var_(now, sys_posix_timespec) = cleared();
        let_ignore = ctx;
        let_ignore = sys_posix_clock_gettime(sys_posix_CLOCK_REALTIME, &now);
        return l$((time_Real_Inst){ .raw = time__posix_fromTimespec(now) });
    };

    fn_((time_Real_direct__unix_resolution(P$raw ctx))(time_ResolutionE$time_Resolution) $scope) {
        var_(res, sys_posix_timespec) = cleared();
        let_ignore = ctx;
        let_ignore = sys_posix_clock_getres(sys_posix_CLOCK_REALTIME, &res);
        return_ok({ .secs = as$(u64)(res.tv_sec), .nanos = as$(u32)(res.tv_nsec) });
    } $unscoped(fn);
));

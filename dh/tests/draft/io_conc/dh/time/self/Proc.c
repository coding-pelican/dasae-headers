#include "Proc.h"
#include "dh/builtin/comp.h"
#include "internal.h"

/*========== Internal Declarations ==========================================*/

$attr($maybe_unused)
$static fn_((time_Proc_direct__unsupported_now(P$raw ctx))(time_Proc_Inst));
pp_if_(plat_is_windows)(pp_then_(
    $static fn_((time_Proc_direct__windows_now(P$raw ctx))(time_Proc_Inst));
));
pp_if_(plat_based_unix)(pp_then_(
    $static fn_((time_Proc_direct__unix_now(P$raw ctx))(time_Proc_Inst));
));

$static let time_Proc_direct__now = pp_if_(plat_is_windows)(
    pp_then_(time_Proc_direct__windows_now),
    pp_else_(pp_if_(plat_based_unix)(
        pp_then_(time_Proc_direct__unix_now),
        pp_else_(time_Proc_direct__unsupported_now)
    )));

/*========== External Definitions ===========================================*/

let_(time_Proc_VTbl_noop, time_Proc_VTbl) = {
    .nowFn = time_Proc_VTbl_noNow,
};

let_(time_Proc_VTbl_failing, time_Proc_VTbl) = {
    .nowFn = time_Proc_VTbl_unreachableNow,
};

fn_((time_Proc_VTbl_noNow(P$raw ctx))(time_Proc_Inst)) {
    let_ignore = ctx;
    return (time_Proc_Inst){ .raw = time_Inst_from(0, 0) };
};

fn_((time_Proc_VTbl_unreachableNow(P$raw ctx))(time_Proc_Inst)) {
    let_ignore = ctx;
    claim_unreachable_msg("Process time source is unavailable");
};

$static var_(time_Proc_noop_ctx, Void) = cleared();
let_(time_Proc_noop, time_Proc) = {
    .ctx = &time_Proc_noop_ctx,
    .vtbl = &time_Proc_VTbl_noop,
};

$static var_(time_Proc_failing_ctx, Void) = cleared();
let_(time_Proc_failing, time_Proc) = {
    .ctx = &time_Proc_failing_ctx,
    .vtbl = &time_Proc_VTbl_failing,
};

fn_((time_Proc_direct(void))(time_E$time_Proc) $scope) {
    pp_if_(time_Proc_direct_supported)(
        pp_then_({
            $static var_(ctx, Void) $like_ref = cleared();
            $static let_(vtbl, time_Proc_VTbl) $like_ref = { {
                .nowFn = time_Proc_direct__now,
            } };
            return_ok(time_Proc_ensureValid((time_Proc){
                .ctx = &ctx,
                .vtbl = vtbl,
            }));
        }),
        pp_else_({
            return_err(time_E_Unsupported());
        })
    );
} $unscoped(fn);

fn_((time_Proc_now(time_Proc self))(time_Proc_Inst)) {
    self = time_Proc_ensureValid(self);
    return self.vtbl->nowFn(self.ctx);
};

fn_((time_Proc_Inst_elapsed(time_Proc_Inst self, time_Proc time))(time_Dur)) {
    return time_Proc_Inst_durSince(time_Proc_now(time), self);
};

fn_((time_Proc_Inst_durSince(time_Proc_Inst later, time_Proc_Inst earlier))(time_Dur)) {
    return time_Inst_durSince(later.raw, earlier.raw);
};

fn_((time_Proc_Inst_durSinceChkd(time_Proc_Inst later, time_Proc_Inst earlier))(O$time_Dur)) {
    return time_Inst_durSinceChkd(later.raw, earlier.raw);
};

op_fn_addWith$(((time_Proc_Inst, time_Dur)(lhs, rhs))(time_Proc_Inst)) {
    return unwrap_(time_Proc_Inst_addChkdDur(lhs, rhs));
};

op_fn_addAsgWith$(((time_Proc_Inst, time_Dur)(lhs, rhs))(time_Proc_Inst*)) {
    return *lhs = unwrap_(time_Proc_Inst_addChkdDur(*lhs, rhs)), lhs;
};

op_fn_subWith$(((time_Proc_Inst, time_Dur)(lhs, rhs))(time_Proc_Inst)) {
    return unwrap_(time_Proc_Inst_subChkdDur(lhs, rhs));
};

op_fn_subAsgWith$(((time_Proc_Inst, time_Dur)(lhs, rhs))(time_Proc_Inst*)) {
    return *lhs = unwrap_(time_Proc_Inst_subChkdDur(*lhs, rhs)), lhs;
};

fn_((time_Proc_Inst_addChkdDur(time_Proc_Inst lhs, time_Dur rhs))(O$time_Proc_Inst) $scope) {
    let raw = orelse_((time_Inst_addChkdDur(lhs.raw, rhs))(return_none()));
    return_some((time_Proc_Inst){ .raw = raw });
} $unscoped(fn);

fn_((time_Proc_Inst_subChkdDur(time_Proc_Inst lhs, time_Dur rhs))(O$time_Proc_Inst) $scope) {
    let raw = orelse_((time_Inst_subChkdDur(lhs.raw, rhs))(return_none()));
    return_some((time_Proc_Inst){ .raw = raw });
} $unscoped(fn);

cmp_fn_ord$((time_Proc_Inst)(lhs, rhs)) {
    return cmp_ord$(time_Inst)(lhs.raw, rhs.raw);
};
cmp_fn_eq_default$((time_Proc_Inst)(lhs, rhs));
cmp_fn_ne_default$((time_Proc_Inst)(lhs, rhs));
cmp_fn_lt_default$((time_Proc_Inst)(lhs, rhs));
cmp_fn_gt_default$((time_Proc_Inst)(lhs, rhs));
cmp_fn_le_default$((time_Proc_Inst)(lhs, rhs));
cmp_fn_ge_default$((time_Proc_Inst)(lhs, rhs));
cmp_fn_ordCtx$((time_Proc_Inst)(lhs, rhs, ctx)) {
    return $ignore_void ctx, cmp_ord$(time_Proc_Inst)(lhs, rhs);
};
cmp_fn_eqCtx_default$((time_Proc_Inst)(lhs, rhs, ctx));
cmp_fn_neCtx_default$((time_Proc_Inst)(lhs, rhs, ctx));
cmp_fn_ltCtx_default$((time_Proc_Inst)(lhs, rhs, ctx));
cmp_fn_gtCtx_default$((time_Proc_Inst)(lhs, rhs, ctx));
cmp_fn_leCtx_default$((time_Proc_Inst)(lhs, rhs, ctx));
cmp_fn_geCtx_default$((time_Proc_Inst)(lhs, rhs, ctx));
cmp_fn_eql$((time_Proc_Inst)(lhs, rhs)) {
    return cmp_ord$(time_Proc_Inst)(lhs, rhs) == cmp_Ord_eq;
};
cmp_fn_neq_default$((time_Proc_Inst)(lhs, rhs));
cmp_fn_eqlCtx$((time_Proc_Inst)(lhs, rhs, ctx)) {
    return $ignore_void ctx, cmp_eql$(time_Proc_Inst)(lhs, rhs);
};
cmp_fn_neqCtx_default$((time_Proc_Inst)(lhs, rhs, ctx));

/*========== Direct Source Definitions ======================================*/

fn_((time_Proc_direct__unsupported_now(P$raw ctx))(time_Proc_Inst)) {
    let_ignore = ctx;
    claim_unreachable_msg("Process direct time source is unavailable on this platform");
};

pp_if_(plat_is_windows)(pp_then_(
    fn_((time_Proc_direct__windows_now(P$raw ctx))(time_Proc_Inst)) {
        var_(create_time, FILETIME) = cleared();
        var_(exit_time, FILETIME) = cleared();
        var_(kernel_time, FILETIME) = cleared();
        var_(user_time, FILETIME) = cleared();
        let_ignore = ctx;
        if (!GetProcessTimes(GetCurrentProcess(), &create_time, &exit_time, &kernel_time, &user_time)) {
            claim_unreachable_msg("Failed to query process cpu time");
        }
        let kernel = time__windows_fromFileTime(kernel_time);
        let user = time__windows_fromFileTime(user_time);
        return (time_Proc_Inst){
            .raw = unwrap_(time_Inst_addChkdDur(
                kernel, (time_Dur){ .secs = user.secs, .nanos = user.nanos }
            )),
        };
    };
));

#if plat_based_unix
fn_((time_Proc_direct__unix_now(P$raw ctx))(time_Proc_Inst)) {
    var_(now, struct timespec) = cleared();
    let_ignore = ctx;
#if defined(CLOCK_PROCESS_CPUTIME_ID)
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &now);
    return (time_Proc_Inst){ .raw = time__unix_fromTimespec(now) };
#else
    claim_unreachable_msg("Process cpu time is unavailable on this platform");
#endif
};
#endif /* plat_based_unix */

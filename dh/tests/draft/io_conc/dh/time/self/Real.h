#ifndef time_self_Real__included
#define time_self_Real__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "../Inst.h"

/*========== Macros and Declarations ========================================*/

#define time_Real_direct_supported __bool__time_Real_direct_supported
#if plat_is_windows || plat_based_unix
#define __bool__time_Real_direct_supported pp_true
#else
#define __bool__time_Real_direct_supported pp_false
#endif

T_alias$((time_Real_VTbl)(struct time_Real_VTbl {
    fn_(((*nowFn)(P$raw ctx))(time_Real_Inst));
}));
$extern let_(time_Real_VTbl_noop, time_Real_VTbl);
$extern let_(time_Real_VTbl_failing, time_Real_VTbl);

$extern fn_((time_Real_VTbl_noNow(P$raw ctx))(time_Real_Inst));
$extern fn_((time_Real_VTbl_unreachableNow(P$raw ctx))(time_Real_Inst));

struct time_Real {
    var_(ctx, P$raw);
    var_(vtbl, P_const$$(time_Real_VTbl));
};
T_impl_E$($set(time_E)(time_Real));
$extern let_(time_Real_noop, time_Real);
$extern let_(time_Real_failing, time_Real);

$attr($inline_always)
$static fn_((time_Real_isValid(time_Real self))(bool));
$attr($inline_always)
$static fn_((time_Real_assertValid(P$raw ctx, P_const$$(time_Real_VTbl) vtbl))(void));
$attr($inline_always)
$static fn_((time_Real_ensureValid(time_Real self))(time_Real));

$extern fn_((time_Real_now(time_Real self))(time_Real_Inst));
$attr($must_check)
$extern fn_((time_Real_direct(void))(time_E$time_Real));

struct time_Real_Inst {
    var_(raw, time_Inst);
};
T_impl_O$(time_Real_Inst);
$extern fn_((time_Real_Inst_elapsed(time_Real_Inst self, time_Real time))(time_Dur));
$extern fn_((time_Real_Inst_durSince(time_Real_Inst later, time_Real_Inst earlier))(time_Dur));
$extern fn_((time_Real_Inst_durSinceChkd(time_Real_Inst later, time_Real_Inst earlier))(O$time_Dur));

$extern op_fn_addWith$(((time_Real_Inst, time_Dur)(lhs, rhs))(time_Real_Inst));
$extern op_fn_addAsgWith$(((time_Real_Inst, time_Dur)(lhs, rhs))(time_Real_Inst*));
$extern op_fn_subWith$(((time_Real_Inst, time_Dur)(lhs, rhs))(time_Real_Inst));
$extern op_fn_subAsgWith$(((time_Real_Inst, time_Dur)(lhs, rhs))(time_Real_Inst*));
$extern fn_((time_Real_Inst_addChkdDur(time_Real_Inst lhs, time_Dur rhs))(O$time_Real_Inst));
$extern fn_((time_Real_Inst_subChkdDur(time_Real_Inst lhs, time_Dur rhs))(O$time_Real_Inst));

$extern cmp_fn_ord$((time_Real_Inst)(lhs, rhs));
$extern cmp_fn_eq$((time_Real_Inst)(lhs, rhs));
$extern cmp_fn_ne$((time_Real_Inst)(lhs, rhs));
$extern cmp_fn_lt$((time_Real_Inst)(lhs, rhs));
$extern cmp_fn_gt$((time_Real_Inst)(lhs, rhs));
$extern cmp_fn_le$((time_Real_Inst)(lhs, rhs));
$extern cmp_fn_ge$((time_Real_Inst)(lhs, rhs));
$extern cmp_fn_ordCtx$((time_Real_Inst)(lhs, rhs, ctx));
$extern cmp_fn_eqCtx$((time_Real_Inst)(lhs, rhs, ctx));
$extern cmp_fn_neCtx$((time_Real_Inst)(lhs, rhs, ctx));
$extern cmp_fn_ltCtx$((time_Real_Inst)(lhs, rhs, ctx));
$extern cmp_fn_gtCtx$((time_Real_Inst)(lhs, rhs, ctx));
$extern cmp_fn_leCtx$((time_Real_Inst)(lhs, rhs, ctx));
$extern cmp_fn_geCtx$((time_Real_Inst)(lhs, rhs, ctx));
$extern cmp_fn_eql$((time_Real_Inst)(lhs, rhs));
$extern cmp_fn_neq$((time_Real_Inst)(lhs, rhs));
$extern cmp_fn_eqlCtx$((time_Real_Inst)(lhs, rhs, ctx));
$extern cmp_fn_neqCtx$((time_Real_Inst)(lhs, rhs, ctx));

$extern fn_((time_Real_Inst_fromUnixEpoch(u64 secs))(time_Real_Inst));
$extern fn_((time_Real_Inst_toUnixEpoch(time_Real_Inst self))(u64));
$extern fn_((time_Real_Inst_durSinceUnixEpoch(time_Real_Inst self))(time_Dur));

/*========== Macros and Definitions =========================================*/

fn_((time_Real_isValid(time_Real self))(bool)) {
    return isNonnull(self.ctx)
        && isNonnull(self.vtbl)
        && isNonnull(self.vtbl->nowFn);
};
fn_((time_Real_assertValid(P$raw ctx, P_const$$(time_Real_VTbl) vtbl))(void)) {
    claim_assert_nonnull(ctx);
    claim_assert_nonnull(vtbl);
    claim_assert_nonnull(vtbl->nowFn);
};
fn_((time_Real_ensureValid(time_Real self))(time_Real)) {
    return time_Real_assertValid(self.ctx, self.vtbl), self;
};

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* time_self_Real__included */

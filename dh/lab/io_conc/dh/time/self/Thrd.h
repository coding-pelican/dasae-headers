#ifndef time_self_thrd__included
#define time_self_thrd__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "../Inst.h"

/*========== Macros and Declarations ========================================*/

#define time_thrd_direct_supported __bool__time_thrd_direct_supported
#if plat_is_windows || plat_based_unix
#define __bool__time_thrd_direct_supported pp_true
#else
#define __bool__time_thrd_direct_supported pp_false
#endif

T_alias$((time_thrd_VTbl)(struct time_thrd_VTbl));
$extern let_(time_thrd_VTbl_noop, time_thrd_VTbl);
$extern let_(time_thrd_VTbl_failing, time_thrd_VTbl);

struct time_Thrd {
    var_(ctx, P$raw);
    var_(vtbl, P_const$$(time_thrd_VTbl));
};
T_impl_E$($set(time_direct_E)(time_Thrd));
$extern let_(time_thrd_noop, time_Thrd);
$extern let_(time_thrd_failing, time_Thrd);

$attr($inline_always)
$static fn_((time_thrd_isValid(time_Thrd self))(bool));
$attr($inline_always)
$static fn_((time_thrd_assertValid(P$raw ctx, P_const$$(time_thrd_VTbl) vtbl))(void));
$attr($inline_always)
$static fn_((time_thrd_ensureValid(time_Thrd self))(time_Thrd));

$extern fn_((time_thrd_now(time_Thrd self))(time_thrd_Inst));
$attr($must_check)
$extern fn_((time_thrd_direct(void))(time_direct_E$time_Thrd));

struct time_thrd_Inst {
    var_(raw, time_Inst);
};
T_impl_O$(time_thrd_Inst);
$extern fn_((time_thrd_Inst_elapsed(time_thrd_Inst self, time_Thrd time))(time_Dur));
$extern fn_((time_thrd_Inst_durSince(time_thrd_Inst later, time_thrd_Inst earlier))(time_Dur));
$extern fn_((time_thrd_Inst_durSinceChkd(time_thrd_Inst later, time_thrd_Inst earlier))(O$time_Dur));

$extern op_fn_addWith$(((time_thrd_Inst, time_Dur)(lhs, rhs))(time_thrd_Inst));
$extern op_fn_addAsgWith$(((time_thrd_Inst, time_Dur)(lhs, rhs))(time_thrd_Inst*));
$extern op_fn_subWith$(((time_thrd_Inst, time_Dur)(lhs, rhs))(time_thrd_Inst));
$extern op_fn_subAsgWith$(((time_thrd_Inst, time_Dur)(lhs, rhs))(time_thrd_Inst*));
$extern fn_((time_thrd_Inst_addChkdDur(time_thrd_Inst lhs, time_Dur rhs))(O$time_thrd_Inst));
$extern fn_((time_thrd_Inst_subChkdDur(time_thrd_Inst lhs, time_Dur rhs))(O$time_thrd_Inst));

$extern cmp_fn_ord$((time_thrd_Inst)(lhs, rhs));
$extern cmp_fn_eq$((time_thrd_Inst)(lhs, rhs));
$extern cmp_fn_ne$((time_thrd_Inst)(lhs, rhs));
$extern cmp_fn_lt$((time_thrd_Inst)(lhs, rhs));
$extern cmp_fn_gt$((time_thrd_Inst)(lhs, rhs));
$extern cmp_fn_le$((time_thrd_Inst)(lhs, rhs));
$extern cmp_fn_ge$((time_thrd_Inst)(lhs, rhs));
$extern cmp_fn_ordCtx$((time_thrd_Inst)(lhs, rhs, ctx));
$extern cmp_fn_eqCtx$((time_thrd_Inst)(lhs, rhs, ctx));
$extern cmp_fn_neCtx$((time_thrd_Inst)(lhs, rhs, ctx));
$extern cmp_fn_ltCtx$((time_thrd_Inst)(lhs, rhs, ctx));
$extern cmp_fn_gtCtx$((time_thrd_Inst)(lhs, rhs, ctx));
$extern cmp_fn_leCtx$((time_thrd_Inst)(lhs, rhs, ctx));
$extern cmp_fn_geCtx$((time_thrd_Inst)(lhs, rhs, ctx));
$extern cmp_fn_eql$((time_thrd_Inst)(lhs, rhs));
$extern cmp_fn_neq$((time_thrd_Inst)(lhs, rhs));
$extern cmp_fn_eqlCtx$((time_thrd_Inst)(lhs, rhs, ctx));
$extern cmp_fn_neqCtx$((time_thrd_Inst)(lhs, rhs, ctx));

struct time_thrd_VTbl {
    fn_(((*nowFn)(P$raw ctx))(time_thrd_Inst));
};
$extern fn_((time_thrd_VTbl_noNow(P$raw ctx))(time_thrd_Inst));
$extern fn_((time_thrd_VTbl_unreachableNow(P$raw ctx))(time_thrd_Inst));

/*========== Macros and Definitions =========================================*/

fn_((time_thrd_isValid(time_Thrd self))(bool)) {
    return isNonnull(self.ctx)
        && isNonnull(self.vtbl)
        && isNonnull(self.vtbl->nowFn);
};
fn_((time_thrd_assertValid(P$raw ctx, P_const$$(time_thrd_VTbl) vtbl))(void)) {
    claim_assert_nonnull(ctx);
    claim_assert_nonnull(vtbl);
    claim_assert_nonnull(vtbl->nowFn);
};
fn_((time_thrd_ensureValid(time_Thrd self))(time_Thrd)) {
    return time_thrd_assertValid(self.ctx, self.vtbl), self;
};

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* time_self_thrd__included */

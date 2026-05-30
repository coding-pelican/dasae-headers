#ifndef time_self_Proc__included
#define time_self_Proc__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "../Inst.h"

/*========== Macros and Declarations ========================================*/

#define time_Proc_direct_supported __bool__time_Proc_direct_supported
#if plat_is_windows || plat_based_unix
#define __bool__time_Proc_direct_supported pp_true
#else
#define __bool__time_Proc_direct_supported pp_false
#endif

T_alias$((time_Proc_VTbl)(struct time_Proc_VTbl));
$extern let_(time_Proc_VTbl_noop, time_Proc_VTbl);
$extern let_(time_Proc_VTbl_failing, time_Proc_VTbl);

struct time_Proc {
    var_(ctx, P$raw);
    var_(vtbl, P_const$$(time_Proc_VTbl));
};
T_impl_E$($set(time_direct_E)(time_Proc));
$extern let_(time_Proc_noop, time_Proc);
$extern let_(time_Proc_failing, time_Proc);

$attr($inline_always)
$static fn_((time_Proc_isValid(time_Proc self))(bool));
$attr($inline_always)
$static fn_((time_Proc_assertValid(P$raw ctx, P_const$$(time_Proc_VTbl) vtbl))(void));
$attr($inline_always)
$static fn_((time_Proc_ensureValid(time_Proc self))(time_Proc));

$extern fn_((time_Proc_now(time_Proc self))(time_Proc_Inst));
$attr($must_check)
$extern fn_((time_Proc_direct(void))(time_direct_E$time_Proc));

struct time_Proc_Inst {
    var_(raw, time_Inst);
};
T_impl_O$(time_Proc_Inst);
$extern fn_((time_Proc_Inst_elapsed(time_Proc_Inst self, time_Proc time))(time_Dur));
$extern fn_((time_Proc_Inst_durSince(time_Proc_Inst later, time_Proc_Inst earlier))(time_Dur));
$extern fn_((time_Proc_Inst_durSinceChkd(time_Proc_Inst later, time_Proc_Inst earlier))(O$time_Dur));

$extern op_fn_addWith$(((time_Proc_Inst, time_Dur)(lhs, rhs))(time_Proc_Inst));
$extern op_fn_addAsgWith$(((time_Proc_Inst, time_Dur)(lhs, rhs))(time_Proc_Inst*));
$extern op_fn_subWith$(((time_Proc_Inst, time_Dur)(lhs, rhs))(time_Proc_Inst));
$extern op_fn_subAsgWith$(((time_Proc_Inst, time_Dur)(lhs, rhs))(time_Proc_Inst*));
$extern fn_((time_Proc_Inst_addChkdDur(time_Proc_Inst lhs, time_Dur rhs))(O$time_Proc_Inst));
$extern fn_((time_Proc_Inst_subChkdDur(time_Proc_Inst lhs, time_Dur rhs))(O$time_Proc_Inst));

$extern cmp_fn_ord$((time_Proc_Inst)(lhs, rhs));
$extern cmp_fn_eq$((time_Proc_Inst)(lhs, rhs));
$extern cmp_fn_ne$((time_Proc_Inst)(lhs, rhs));
$extern cmp_fn_lt$((time_Proc_Inst)(lhs, rhs));
$extern cmp_fn_gt$((time_Proc_Inst)(lhs, rhs));
$extern cmp_fn_le$((time_Proc_Inst)(lhs, rhs));
$extern cmp_fn_ge$((time_Proc_Inst)(lhs, rhs));
$extern cmp_fn_ordCtx$((time_Proc_Inst)(lhs, rhs, ctx));
$extern cmp_fn_eqCtx$((time_Proc_Inst)(lhs, rhs, ctx));
$extern cmp_fn_neCtx$((time_Proc_Inst)(lhs, rhs, ctx));
$extern cmp_fn_ltCtx$((time_Proc_Inst)(lhs, rhs, ctx));
$extern cmp_fn_gtCtx$((time_Proc_Inst)(lhs, rhs, ctx));
$extern cmp_fn_leCtx$((time_Proc_Inst)(lhs, rhs, ctx));
$extern cmp_fn_geCtx$((time_Proc_Inst)(lhs, rhs, ctx));
$extern cmp_fn_eql$((time_Proc_Inst)(lhs, rhs));
$extern cmp_fn_neq$((time_Proc_Inst)(lhs, rhs));
$extern cmp_fn_eqlCtx$((time_Proc_Inst)(lhs, rhs, ctx));
$extern cmp_fn_neqCtx$((time_Proc_Inst)(lhs, rhs, ctx));

struct time_Proc_VTbl {
    fn_(((*nowFn)(P$raw ctx))(time_Proc_Inst));
};
$extern fn_((time_Proc_VTbl_noNow(P$raw ctx))(time_Proc_Inst));
$extern fn_((time_Proc_VTbl_unreachableNow(P$raw ctx))(time_Proc_Inst));

/*========== Macros and Definitions =========================================*/

fn_((time_Proc_isValid(time_Proc self))(bool)) {
    return isNonnull(self.ctx)
        && isNonnull(self.vtbl)
        && isNonnull(self.vtbl->nowFn);
};
fn_((time_Proc_assertValid(P$raw ctx, P_const$$(time_Proc_VTbl) vtbl))(void)) {
    claim_assert_nonnull(ctx);
    claim_assert_nonnull(vtbl);
    claim_assert_nonnull(vtbl->nowFn);
};
fn_((time_Proc_ensureValid(time_Proc self))(time_Proc)) {
    return time_Proc_assertValid(self.ctx, self.vtbl), self;
};

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* time_self_Proc__included */

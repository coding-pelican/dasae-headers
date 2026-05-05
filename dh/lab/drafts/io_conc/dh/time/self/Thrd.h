#ifndef time_self_Thrd__included
#define time_self_Thrd__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "../Inst.h"

/*========== Macros and Declarations ========================================*/

#define time_Thrd_direct_supported __bool__time_Thrd_direct_supported
#if plat_is_windows || plat_based_unix
#define __bool__time_Thrd_direct_supported pp_true
#else
#define __bool__time_Thrd_direct_supported pp_false
#endif

T_alias$((time_Thrd_VTbl)(struct time_Thrd_VTbl));
$extern let_(time_Thrd_VTbl_noop, time_Thrd_VTbl);
$extern let_(time_Thrd_VTbl_failing, time_Thrd_VTbl);

struct time_Thrd {
    var_(ctx, P$raw);
    var_(vtbl, P_const$$(time_Thrd_VTbl));
};
T_impl_E$($set(time_direct_E)(time_Thrd));
$extern let_(time_Thrd_noop, time_Thrd);
$extern let_(time_Thrd_failing, time_Thrd);

$attr($inline_always)
$static fn_((time_Thrd_isValid(time_Thrd self))(bool));
$attr($inline_always)
$static fn_((time_Thrd_assertValid(P$raw ctx, P_const$$(time_Thrd_VTbl) vtbl))(void));
$attr($inline_always)
$static fn_((time_Thrd_ensureValid(time_Thrd self))(time_Thrd));

$extern fn_((time_Thrd_now(time_Thrd self))(time_Thrd_Inst));
$attr($must_check)
$extern fn_((time_Thrd_direct(void))(time_direct_E$time_Thrd));

struct time_Thrd_Inst {
    var_(raw, time_Inst);
};
T_impl_O$(time_Thrd_Inst);
$extern fn_((time_Thrd_Inst_elapsed(time_Thrd_Inst self, time_Thrd time))(time_Dur));
$extern fn_((time_Thrd_Inst_durSince(time_Thrd_Inst later, time_Thrd_Inst earlier))(time_Dur));
$extern fn_((time_Thrd_Inst_durSinceChkd(time_Thrd_Inst later, time_Thrd_Inst earlier))(O$time_Dur));

$extern op_fn_addWith$(((time_Thrd_Inst, time_Dur)(lhs, rhs))(time_Thrd_Inst));
$extern op_fn_addAsgWith$(((time_Thrd_Inst, time_Dur)(lhs, rhs))(time_Thrd_Inst*));
$extern op_fn_subWith$(((time_Thrd_Inst, time_Dur)(lhs, rhs))(time_Thrd_Inst));
$extern op_fn_subAsgWith$(((time_Thrd_Inst, time_Dur)(lhs, rhs))(time_Thrd_Inst*));
$extern fn_((time_Thrd_Inst_addChkdDur(time_Thrd_Inst lhs, time_Dur rhs))(O$time_Thrd_Inst));
$extern fn_((time_Thrd_Inst_subChkdDur(time_Thrd_Inst lhs, time_Dur rhs))(O$time_Thrd_Inst));

$extern cmp_fn_ord$((time_Thrd_Inst)(lhs, rhs));
$extern cmp_fn_eq$((time_Thrd_Inst)(lhs, rhs));
$extern cmp_fn_ne$((time_Thrd_Inst)(lhs, rhs));
$extern cmp_fn_lt$((time_Thrd_Inst)(lhs, rhs));
$extern cmp_fn_gt$((time_Thrd_Inst)(lhs, rhs));
$extern cmp_fn_le$((time_Thrd_Inst)(lhs, rhs));
$extern cmp_fn_ge$((time_Thrd_Inst)(lhs, rhs));
$extern cmp_fn_ordCtx$((time_Thrd_Inst)(lhs, rhs, ctx));
$extern cmp_fn_eqCtx$((time_Thrd_Inst)(lhs, rhs, ctx));
$extern cmp_fn_neCtx$((time_Thrd_Inst)(lhs, rhs, ctx));
$extern cmp_fn_ltCtx$((time_Thrd_Inst)(lhs, rhs, ctx));
$extern cmp_fn_gtCtx$((time_Thrd_Inst)(lhs, rhs, ctx));
$extern cmp_fn_leCtx$((time_Thrd_Inst)(lhs, rhs, ctx));
$extern cmp_fn_geCtx$((time_Thrd_Inst)(lhs, rhs, ctx));
$extern cmp_fn_eql$((time_Thrd_Inst)(lhs, rhs));
$extern cmp_fn_neq$((time_Thrd_Inst)(lhs, rhs));
$extern cmp_fn_eqlCtx$((time_Thrd_Inst)(lhs, rhs, ctx));
$extern cmp_fn_neqCtx$((time_Thrd_Inst)(lhs, rhs, ctx));

struct time_Thrd_VTbl {
    fn_(((*nowFn)(P$raw ctx))(time_Thrd_Inst));
};
$extern fn_((time_Thrd_VTbl_noNow(P$raw ctx))(time_Thrd_Inst));
$extern fn_((time_Thrd_VTbl_unreachableNow(P$raw ctx))(time_Thrd_Inst));

/*========== Macros and Definitions =========================================*/

fn_((time_Thrd_isValid(time_Thrd self))(bool)) {
    return isNonnull(self.ctx)
        && isNonnull(self.vtbl)
        && isNonnull(self.vtbl->nowFn);
};
fn_((time_Thrd_assertValid(P$raw ctx, P_const$$(time_Thrd_VTbl) vtbl))(void)) {
    claim_assert_nonnull(ctx);
    claim_assert_nonnull(vtbl);
    claim_assert_nonnull(vtbl->nowFn);
};
fn_((time_Thrd_ensureValid(time_Thrd self))(time_Thrd)) {
    return time_Thrd_assertValid(self.ctx, self.vtbl), self;
};

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* time_self_Thrd__included */

#ifndef time_self_Awake__included
#define time_self_Awake__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "../Inst.h"
#include "../../Sched.h"
#include "../../exec/base.h"

/*========== Macros and Declarations ========================================*/

#define time_Awake_direct_supported __bool__time_Awake_direct_supported
#if plat_is_windows || plat_based_unix
#define __bool__time_Awake_direct_supported pp_true
#else
#define __bool__time_Awake_direct_supported pp_false
#endif

T_alias$((time_Awake_VTbl)(struct time_Awake_VTbl));
$extern let_(time_Awake_VTbl_noop, time_Awake_VTbl);
$extern let_(time_Awake_VTbl_failing, time_Awake_VTbl);

struct time_Awake {
    var_(ctx, P$raw);
    var_(vtbl, P_const$$(time_Awake_VTbl));
};
T_impl_E$($set(time_direct_E)(time_Awake));
$extern let_(time_Awake_noop, time_Awake);
$extern let_(time_Awake_failing, time_Awake);

$attr($inline_always)
$static fn_((time_Awake_isValid(time_Awake self))(bool));
$attr($inline_always)
$static fn_((time_Awake_assertValid(P$raw ctx, P_const$$(time_Awake_VTbl) vtbl))(void));
$attr($inline_always)
$static fn_((time_Awake_ensureValid(time_Awake self))(time_Awake));

$extern fn_((time_Awake_now(time_Awake self))(time_Awake_Inst));
$attr($must_check)
$extern fn_((time_Awake_sleep(time_Awake self, time_Dur dur))(Sched_Cancelable$void));
$attr($must_check)
$extern fn_((time_Awake_sleepSecs(time_Awake self, u64 secs))(Sched_Cancelable$void));
$attr($must_check)
$extern fn_((time_Awake_sleepMillis(time_Awake self, u64 millis))(Sched_Cancelable$void));
$attr($must_check)
$extern fn_((time_Awake_sleepMicros(time_Awake self, u64 micros))(Sched_Cancelable$void));
$attr($must_check)
$extern fn_((time_Awake_sleepNanos(time_Awake self, u32 nanos))(Sched_Cancelable$void));

$attr($must_check)
$extern fn_((time_Awake_direct(void))(time_direct_E$time_Awake));
$extern fn_((time_Awake_evented(exec_Coop* coop))(time_Awake));

struct time_Awake_Inst {
    var_(raw, time_Inst);
};
T_impl_O$(time_Awake_Inst);
$extern fn_((time_Awake_Inst_elapsed(time_Awake_Inst self, time_Awake time))(time_Dur));
$extern fn_((time_Awake_Inst_durSince(time_Awake_Inst later, time_Awake_Inst earlier))(time_Dur));
$extern fn_((time_Awake_Inst_durSinceChkd(time_Awake_Inst later, time_Awake_Inst earlier))(O$time_Dur));

$extern op_fn_addWith$(((time_Awake_Inst, time_Dur)(lhs, rhs))(time_Awake_Inst));
$extern op_fn_addAsgWith$(((time_Awake_Inst, time_Dur)(lhs, rhs))(time_Awake_Inst*));
$extern op_fn_subWith$(((time_Awake_Inst, time_Dur)(lhs, rhs))(time_Awake_Inst));
$extern op_fn_subAsgWith$(((time_Awake_Inst, time_Dur)(lhs, rhs))(time_Awake_Inst*));
$extern fn_((time_Awake_Inst_addChkdDur(time_Awake_Inst lhs, time_Dur rhs))(O$time_Awake_Inst));
$extern fn_((time_Awake_Inst_subChkdDur(time_Awake_Inst lhs, time_Dur rhs))(O$time_Awake_Inst));

$extern cmp_fn_ord$((time_Awake_Inst)(lhs, rhs));
$extern cmp_fn_eq$((time_Awake_Inst)(lhs, rhs));
$extern cmp_fn_ne$((time_Awake_Inst)(lhs, rhs));
$extern cmp_fn_lt$((time_Awake_Inst)(lhs, rhs));
$extern cmp_fn_gt$((time_Awake_Inst)(lhs, rhs));
$extern cmp_fn_le$((time_Awake_Inst)(lhs, rhs));
$extern cmp_fn_ge$((time_Awake_Inst)(lhs, rhs));
$extern cmp_fn_ordCtx$((time_Awake_Inst)(lhs, rhs, ctx));
$extern cmp_fn_eqCtx$((time_Awake_Inst)(lhs, rhs, ctx));
$extern cmp_fn_neCtx$((time_Awake_Inst)(lhs, rhs, ctx));
$extern cmp_fn_ltCtx$((time_Awake_Inst)(lhs, rhs, ctx));
$extern cmp_fn_gtCtx$((time_Awake_Inst)(lhs, rhs, ctx));
$extern cmp_fn_leCtx$((time_Awake_Inst)(lhs, rhs, ctx));
$extern cmp_fn_geCtx$((time_Awake_Inst)(lhs, rhs, ctx));
$extern cmp_fn_eql$((time_Awake_Inst)(lhs, rhs));
$extern cmp_fn_neq$((time_Awake_Inst)(lhs, rhs));
$extern cmp_fn_eqlCtx$((time_Awake_Inst)(lhs, rhs, ctx));
$extern cmp_fn_neqCtx$((time_Awake_Inst)(lhs, rhs, ctx));

struct time_Awake_VTbl {
    fn_(((*nowFn)(P$raw ctx))(time_Awake_Inst));
    $attr($must_check)
    fn_(((*sleepFn)(P$raw ctx, time_Dur dur))(Sched_Cancelable$void));
};
$extern fn_((time_Awake_VTbl_noNow(P$raw ctx))(time_Awake_Inst));
$extern fn_((time_Awake_VTbl_unreachableNow(P$raw ctx))(time_Awake_Inst));
$attr($must_check)
$extern fn_((time_Awake_VTbl_failingSleep(P$raw ctx, time_Dur dur))(Sched_Cancelable$void));

/*========== Macros and Definitions =========================================*/

fn_((time_Awake_isValid(time_Awake self))(bool)) {
    return isNonnull(self.ctx)
        && isNonnull(self.vtbl)
        && isNonnull(self.vtbl->nowFn)
        && isNonnull(self.vtbl->sleepFn);
};
fn_((time_Awake_assertValid(P$raw ctx, P_const$$(time_Awake_VTbl) vtbl))(void)) {
    claim_assert_nonnull(ctx);
    claim_assert_nonnull(vtbl);
    claim_assert_nonnull(vtbl->nowFn);
    claim_assert_nonnull(vtbl->sleepFn);
};
fn_((time_Awake_ensureValid(time_Awake self))(time_Awake)) {
    return time_Awake_assertValid(self.ctx, self.vtbl), self;
};

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* time_self_Awake__included */

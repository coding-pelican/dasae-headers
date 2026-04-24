#ifndef time_self_Boot__included
#define time_self_Boot__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "../Inst.h"
#include "../../Sched.h"

/*========== Macros and Declarations ========================================*/

#define time_Boot_direct_supported __bool__time_Boot_direct_supported
#if plat_is_windows || plat_based_unix
#define __bool__time_Boot_direct_supported pp_true
#else
#define __bool__time_Boot_direct_supported pp_false
#endif

T_alias$((time_Boot_VTbl)(struct time_Boot_VTbl {
    fn_(((*nowFn)(P$raw ctx))(time_Boot_Inst));
    $attr($must_check)
    fn_(((*sleepFn)(P$raw ctx, time_Dur dur))(Sched_Cancelable$void));
}));
$extern let_(time_Boot_VTbl_noop, time_Boot_VTbl);
$extern let_(time_Boot_VTbl_failing, time_Boot_VTbl);

$extern fn_((time_Boot_VTbl_noNow(P$raw ctx))(time_Boot_Inst));
$extern fn_((time_Boot_VTbl_unreachableNow(P$raw ctx))(time_Boot_Inst));
$attr($must_check)
$extern fn_((time_Boot_VTbl_failingSleep(P$raw ctx, time_Dur dur))(Sched_Cancelable$void));

struct time_Boot {
    var_(ctx, P$raw);
    var_(vtbl, P_const$$(time_Boot_VTbl));
};
T_impl_E$($set(time_E)(time_Boot));
$extern let_(time_Boot_noop, time_Boot);
$extern let_(time_Boot_failing, time_Boot);

$attr($inline_always)
$static fn_((time_Boot_isValid(time_Boot self))(bool));
$attr($inline_always)
$static fn_((time_Boot_assertValid(P$raw ctx, P_const$$(time_Boot_VTbl) vtbl))(void));
$attr($inline_always)
$static fn_((time_Boot_ensureValid(time_Boot self))(time_Boot));

$extern fn_((time_Boot_now(time_Boot self))(time_Boot_Inst));
$attr($must_check)
$extern fn_((time_Boot_sleep(time_Boot self, time_Dur dur))(Sched_Cancelable$void));
$attr($must_check)
$extern fn_((time_Boot_sleepSecs(time_Boot self, u64 secs))(Sched_Cancelable$void));
$attr($must_check)
$extern fn_((time_Boot_sleepMillis(time_Boot self, u64 millis))(Sched_Cancelable$void));
$attr($must_check)
$extern fn_((time_Boot_sleepMicros(time_Boot self, u64 micros))(Sched_Cancelable$void));
$attr($must_check)
$extern fn_((time_Boot_sleepNanos(time_Boot self, u32 nanos))(Sched_Cancelable$void));

$attr($must_check)
$extern fn_((time_Boot_direct(void))(time_E$time_Boot));

struct time_Boot_Inst {
    var_(raw, time_Inst);
};
T_impl_O$(time_Boot_Inst);
$extern fn_((time_Boot_Inst_elapsed(time_Boot_Inst self, time_Boot time))(time_Dur));
$extern fn_((time_Boot_Inst_durSince(time_Boot_Inst later, time_Boot_Inst earlier))(time_Dur));
$extern fn_((time_Boot_Inst_durSinceChkd(time_Boot_Inst later, time_Boot_Inst earlier))(O$time_Dur));

$extern op_fn_addWith$(((time_Boot_Inst, time_Dur)(lhs, rhs))(time_Boot_Inst));
$extern op_fn_addAsgWith$(((time_Boot_Inst, time_Dur)(lhs, rhs))(time_Boot_Inst*));
$extern op_fn_subWith$(((time_Boot_Inst, time_Dur)(lhs, rhs))(time_Boot_Inst));
$extern op_fn_subAsgWith$(((time_Boot_Inst, time_Dur)(lhs, rhs))(time_Boot_Inst*));
$extern fn_((time_Boot_Inst_addChkdDur(time_Boot_Inst lhs, time_Dur rhs))(O$time_Boot_Inst));
$extern fn_((time_Boot_Inst_subChkdDur(time_Boot_Inst lhs, time_Dur rhs))(O$time_Boot_Inst));

$extern cmp_fn_ord$((time_Boot_Inst)(lhs, rhs));
$extern cmp_fn_eq$((time_Boot_Inst)(lhs, rhs));
$extern cmp_fn_ne$((time_Boot_Inst)(lhs, rhs));
$extern cmp_fn_lt$((time_Boot_Inst)(lhs, rhs));
$extern cmp_fn_gt$((time_Boot_Inst)(lhs, rhs));
$extern cmp_fn_le$((time_Boot_Inst)(lhs, rhs));
$extern cmp_fn_ge$((time_Boot_Inst)(lhs, rhs));
$extern cmp_fn_ordCtx$((time_Boot_Inst)(lhs, rhs, ctx));
$extern cmp_fn_eqCtx$((time_Boot_Inst)(lhs, rhs, ctx));
$extern cmp_fn_neCtx$((time_Boot_Inst)(lhs, rhs, ctx));
$extern cmp_fn_ltCtx$((time_Boot_Inst)(lhs, rhs, ctx));
$extern cmp_fn_gtCtx$((time_Boot_Inst)(lhs, rhs, ctx));
$extern cmp_fn_leCtx$((time_Boot_Inst)(lhs, rhs, ctx));
$extern cmp_fn_geCtx$((time_Boot_Inst)(lhs, rhs, ctx));
$extern cmp_fn_eql$((time_Boot_Inst)(lhs, rhs));
$extern cmp_fn_neq$((time_Boot_Inst)(lhs, rhs));
$extern cmp_fn_eqlCtx$((time_Boot_Inst)(lhs, rhs, ctx));
$extern cmp_fn_neqCtx$((time_Boot_Inst)(lhs, rhs, ctx));

/*========== Macros and Definitions =========================================*/

fn_((time_Boot_isValid(time_Boot self))(bool)) {
    return isNonnull(self.ctx)
        && isNonnull(self.vtbl)
        && isNonnull(self.vtbl->nowFn)
        && isNonnull(self.vtbl->sleepFn);
};
fn_((time_Boot_assertValid(P$raw ctx, P_const$$(time_Boot_VTbl) vtbl))(void)) {
    claim_assert_nonnull(ctx);
    claim_assert_nonnull(vtbl);
    claim_assert_nonnull(vtbl->nowFn);
    claim_assert_nonnull(vtbl->sleepFn);
};
fn_((time_Boot_ensureValid(time_Boot self))(time_Boot)) {
    return time_Boot_assertValid(self.ctx, self.vtbl), self;
};

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* time_self_Boot__included */

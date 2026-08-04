#pragma once
#ifndef time_Clock__included
#define time_Clock__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "self.h"

/*========== Macros and Declarations ========================================*/

/// Tagged clock source handle for real, awake, boot, process, or thread time.
variant_((time_Clock $fits($packed))(
    (time_Clock_real, time_Real),
    (time_Clock_awake, time_Awake),
    (time_Clock_boot, time_Boot),
    (time_Clock_proc, time_Proc),
    (time_Clock_thrd, time_Thrd)
));
T_impl_O$(time_Clock);
/// Read the current clock instant.
$extern fn_((time_Clock_now(time_Clock clock))(time_Clock_Inst));
/// Return the clock resolution.
$attr($must_check)
$extern fn_((time_Clock_resoln(time_Clock clock))(time_ResolnE$time_Resoln));
/// Sleep on the clock for a duration.
$attr($must_check)
$extern fn_((time_Clock_sleep(time_Clock self, time_Dur dur))(Sched_Cancelable$void));
/// Sleep on the clock for whole seconds.
$attr($must_check)
$extern fn_((time_Clock_sleepSecs(time_Clock self, u64 secs))(Sched_Cancelable$void));
/// Sleep on the clock for whole milliseconds.
$attr($must_check)
$extern fn_((time_Clock_sleepMillis(time_Clock self, u64 millis))(Sched_Cancelable$void));
/// Sleep on the clock for whole microseconds.
$attr($must_check)
$extern fn_((time_Clock_sleepMicros(time_Clock self, u64 micros))(Sched_Cancelable$void));
/// Sleep on the clock for whole nanoseconds.
$attr($must_check)
$extern fn_((time_Clock_sleepNanos(time_Clock self, u32 nanos))(Sched_Cancelable$void));

/// Tagged instant value produced by one of the supported clock sources.
variant_((time_Clock_Inst $maps(FieldType$(time_Clock, tag)))(
    (time_Clock_real, time_Real_Inst),
    (time_Clock_awake, time_Awake_Inst),
    (time_Clock_boot, time_Boot_Inst),
    (time_Clock_proc, time_Proc_Inst),
    (time_Clock_thrd, time_Thrd_Inst)
));
T_impl_O$(time_Clock_Inst);
/// Return elapsed duration from an instant to the clock's current instant.
$extern fn_((time_Clock_Inst_elapsed(time_Clock_Inst self, time_Clock time))(time_Dur));
/// Return duration between two instants.
$extern fn_((time_Clock_Inst_durSince(time_Clock_Inst later, time_Clock_Inst earlier))(time_Dur));
/// Return duration between two instants, or none if ordered backwards.
$extern fn_((time_Clock_Inst_durSinceChkd(time_Clock_Inst later, time_Clock_Inst earlier))(O$time_Dur));

$extern op_fn_addWith$(((time_Clock_Inst, time_Dur)(lhs, rhs))(time_Clock_Inst));
$static op_fn_addWith$(addDur, ((time_Clock_Inst, time_Dur)(lhs, rhs))(time_Clock_Inst));
$extern op_fn_addAsgWith$(((time_Clock_Inst, time_Dur)(lhs, rhs))(time_Clock_Inst*));
$static op_fn_addAsgWith$(addAsgDur, ((time_Clock_Inst, time_Dur)(lhs, rhs))(time_Clock_Inst*));
$extern op_fn_subWith$(((time_Clock_Inst, time_Dur)(lhs, rhs))(time_Clock_Inst));
$static op_fn_subWith$(subDur, ((time_Clock_Inst, time_Dur)(lhs, rhs))(time_Clock_Inst));
$extern op_fn_subAsgWith$(((time_Clock_Inst, time_Dur)(lhs, rhs))(time_Clock_Inst*));
$static op_fn_subAsgWith$(subAsgDur, ((time_Clock_Inst, time_Dur)(lhs, rhs))(time_Clock_Inst*));
/// Add a duration to an instant, returning none on overflow.
$extern fn_((time_Clock_Inst_addChkdDur(time_Clock_Inst lhs, time_Dur rhs))(O$time_Clock_Inst));
/// Subtract a duration from an instant, returning none on underflow.
$extern fn_((time_Clock_Inst_subChkdDur(time_Clock_Inst lhs, time_Dur rhs))(O$time_Clock_Inst));

$extern cmp_fn_ord$((time_Clock_Inst)(lhs, rhs));
$extern cmp_fn_eq$((time_Clock_Inst)(lhs, rhs));
$extern cmp_fn_ne$((time_Clock_Inst)(lhs, rhs));
$extern cmp_fn_lt$((time_Clock_Inst)(lhs, rhs));
$extern cmp_fn_gt$((time_Clock_Inst)(lhs, rhs));
$extern cmp_fn_le$((time_Clock_Inst)(lhs, rhs));
$extern cmp_fn_ge$((time_Clock_Inst)(lhs, rhs));
$extern cmp_fn_ordCtx$((time_Clock_Inst)(lhs, rhs, ctx));
$extern cmp_fn_eqCtx$((time_Clock_Inst)(lhs, rhs, ctx));
$extern cmp_fn_neCtx$((time_Clock_Inst)(lhs, rhs, ctx));
$extern cmp_fn_ltCtx$((time_Clock_Inst)(lhs, rhs, ctx));
$extern cmp_fn_gtCtx$((time_Clock_Inst)(lhs, rhs, ctx));
$extern cmp_fn_leCtx$((time_Clock_Inst)(lhs, rhs, ctx));
$extern cmp_fn_geCtx$((time_Clock_Inst)(lhs, rhs, ctx));
$extern cmp_fn_eql$((time_Clock_Inst)(lhs, rhs));
$extern cmp_fn_neq$((time_Clock_Inst)(lhs, rhs));
$extern cmp_fn_eqlCtx$((time_Clock_Inst)(lhs, rhs, ctx));
$extern cmp_fn_neqCtx$((time_Clock_Inst)(lhs, rhs, ctx));

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* time_Clock__included */

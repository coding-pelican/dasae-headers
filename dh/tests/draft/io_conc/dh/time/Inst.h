#ifndef time_Inst__included
#define time_Inst__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "Dur.h"

/*========== Macros and Declarations ========================================*/

struct time_Inst {
    u64 secs;
    u32 nanos;
};
T_impl_O$(time_Inst);
$extern fn_((time_Inst_from(u64 secs, u32 nanos))(time_Inst));
$extern fn_((time_Inst_isZero(time_Inst self))(bool));
$extern fn_((time_Inst_ticks(time_Inst self))(u64));

$extern op_fn_addWith$(((time_Inst, time_Dur)(lhs, rhs))(time_Inst));
$static op_fn_addWith$(addDur, ((time_Inst, time_Dur)(lhs, rhs))(time_Inst));
$extern op_fn_addAsgWith$(((time_Inst, time_Dur)(lhs, rhs))(time_Inst*));
$static op_fn_addAsgWith$(addAsgDur, ((time_Inst, time_Dur)(lhs, rhs))(time_Inst*));
$extern op_fn_subWith$(((time_Inst, time_Dur)(lhs, rhs))(time_Inst));
$static op_fn_subWith$(subDur, ((time_Inst, time_Dur)(lhs, rhs))(time_Inst));
$extern op_fn_subAsgWith$(((time_Inst, time_Dur)(lhs, rhs))(time_Inst*));
$static op_fn_subAsgWith$(subAsgDur, ((time_Inst, time_Dur)(lhs, rhs))(time_Inst*));
$extern fn_((time_Inst_addChkdDur(time_Inst lhs, time_Dur rhs))(O$time_Inst));
$extern fn_((time_Inst_subChkdDur(time_Inst lhs, time_Dur rhs))(O$time_Inst));
$extern fn_((time_Inst_durSince(time_Inst later, time_Inst earlier))(time_Dur));
$extern fn_((time_Inst_durSinceChkd(time_Inst later, time_Inst earlier))(O$time_Dur));

$extern cmp_fn_ord$((time_Inst)(lhs, rhs));
$extern cmp_fn_eq$((time_Inst)(lhs, rhs));
$extern cmp_fn_ne$((time_Inst)(lhs, rhs));
$extern cmp_fn_lt$((time_Inst)(lhs, rhs));
$extern cmp_fn_gt$((time_Inst)(lhs, rhs));
$extern cmp_fn_le$((time_Inst)(lhs, rhs));
$extern cmp_fn_ge$((time_Inst)(lhs, rhs));
$extern cmp_fn_ordCtx$((time_Inst)(lhs, rhs, ctx));
$extern cmp_fn_eqCtx$((time_Inst)(lhs, rhs, ctx));
$extern cmp_fn_neCtx$((time_Inst)(lhs, rhs, ctx));
$extern cmp_fn_ltCtx$((time_Inst)(lhs, rhs, ctx));
$extern cmp_fn_gtCtx$((time_Inst)(lhs, rhs, ctx));
$extern cmp_fn_leCtx$((time_Inst)(lhs, rhs, ctx));
$extern cmp_fn_geCtx$((time_Inst)(lhs, rhs, ctx));
$extern cmp_fn_eql$((time_Inst)(lhs, rhs));
$extern cmp_fn_neq$((time_Inst)(lhs, rhs));
$extern cmp_fn_eqlCtx$((time_Inst)(lhs, rhs, ctx));
$extern cmp_fn_neqCtx$((time_Inst)(lhs, rhs, ctx));

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* time_Inst__included */

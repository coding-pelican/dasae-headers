/**
 * @copyright Copyright (c) 2026 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    Invoc/Args.h
 * @ingroup dasae-headers(dh)/start
 * @prefix  start_Invoc_Args
 *
 * @brief   Caller-owned startup argument-source adapter
 */
#pragma once
#ifndef start_Invoc_Args__included
#define start_Invoc_Args__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "../../proc/Args.h"

/*========== Macros and Declarations ========================================*/

T_alias$((start_Invoc_Args_VecZ)(struct start_Invoc_Args_VecZ {
    var_(count, usize);
    var_(items, P_const$P_const$u8);
}));
T_alias$((start_Invoc_Args_WTF16)(struct start_Invoc_Args_WTF16 {
    var_(cmd_line, S_const$u16);
}));
T_alias$((start_Invoc_Args)(variant_((start_Invoc_Args $fits($packed))(
    (start_Invoc_Args_vec_z, start_Invoc_Args_VecZ),
    (start_Invoc_Args_wtf16, start_Invoc_Args_WTF16)
))));
T_use_prl$(start_Invoc_Args);
$attr($inline_always)
$static fn_((start_Invoc_Args_initVecZ(usize count, P_const$P_const$u8 items))(start_Invoc_Args));
$attr($inline_always)
$static fn_((start_Invoc_Args_initWTF16(S_const$u16 cmd_line))(start_Invoc_Args));
$extern fn_((start_Invoc_Args_self(start_Invoc_Args* self))(proc_Args));

/*========== Macros and Definitions =========================================*/

#if in_analysis_active_only || in_comptime
fn_((start_Invoc_Args_initVecZ(usize count, P_const$P_const$u8 items))(start_Invoc_Args)) {
    claim_assert_nonnull(items);
    return union_of$((start_Invoc_Args)(start_Invoc_Args_vec_z){
        .count = count,
        .items = items,
    });
};
fn_((start_Invoc_Args_initWTF16(S_const$u16 cmd_line))(start_Invoc_Args)) {
    claim_assert_nonnullS(cmd_line);
    return union_of$((start_Invoc_Args)(start_Invoc_Args_wtf16){
        .cmd_line = cmd_line,
    });
};
#endif /* in_analysis_active_only || in_comptime */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* start_Invoc_Args__included */

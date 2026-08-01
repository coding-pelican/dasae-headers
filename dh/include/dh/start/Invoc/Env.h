/**
 * @copyright Copyright (c) 2026 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    Invoc/Env.h
 * @ingroup dasae-headers(dh)/start
 * @prefix  start_Invoc_Env
 *
 * @brief   Caller-owned startup environment-source adapter
 */
#pragma once
#ifndef start_Invoc_Env__included
#define start_Invoc_Env__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "../../proc/Env.h"

/*========== Macros and Declarations ========================================*/

T_alias$((start_Invoc_Env_VecZ)(struct start_Invoc_Env_VecZ {
    var_(count, usize);
    var_(items, P_const$P_const$u8);
}));
T_alias$((start_Invoc_Env_WTF16)(struct start_Invoc_Env_WTF16 {
    var_(block, S_const$u16);
}));
T_alias$((start_Invoc_Env)(variant_((start_Invoc_Env $fits($packed))(
    (start_Invoc_Env_empty, Void),
    (start_Invoc_Env_vec_z, start_Invoc_Env_VecZ),
    (start_Invoc_Env_wtf16, start_Invoc_Env_WTF16)
))));
T_use_prl$(start_Invoc_Env);
$attr($inline_always)
$static fn_((start_Invoc_Env_initEmpty(void))(start_Invoc_Env));
$attr($inline_always)
$static fn_((start_Invoc_Env_initVecZ(usize count, P_const$P_const$u8 items))(start_Invoc_Env));
$attr($inline_always)
$static fn_((start_Invoc_Env_initWTF16(S_const$u16 block))(start_Invoc_Env));
$extern fn_((start_Invoc_Env_self(start_Invoc_Env* self))(proc_Env));

/*========== Macros and Definitions =========================================*/

#if in_analysis_active_only || in_comptime
fn_((start_Invoc_Env_initEmpty(void))(start_Invoc_Env)) {
    return union_of$((start_Invoc_Env)(start_Invoc_Env_empty){});
};
fn_((start_Invoc_Env_initVecZ(usize count, P_const$P_const$u8 items))(start_Invoc_Env)) {
    claim_assert_nonnull(items);
    return union_of$((start_Invoc_Env)(start_Invoc_Env_vec_z){
        .count = count,
        .items = items,
    });
};
fn_((start_Invoc_Env_initWTF16(S_const$u16 block))(start_Invoc_Env)) {
    claim_assert_nonnullS(block);
    return union_of$((start_Invoc_Env)(start_Invoc_Env_wtf16){
        .block = block,
    });
};
#endif /* in_analysis_active_only || in_comptime */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* start_Invoc_Env__included */

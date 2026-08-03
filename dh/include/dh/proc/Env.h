/**
 * @copyright Copyright (c) 2026 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    Env.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2026-07-30 (date of creation)
 * @updated 2026-07-31 (date of last update)
 * @ingroup dasae-headers(dh)/proc
 * @prefix  proc_Env
 *
 * @brief   Injectable process environment source and iterator
 */
#pragma once
#ifndef proc_Env__included
#define proc_Env__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "Args.h"

/*========== Macros and Declarations ========================================*/

errset_((proc_Env_E)() $union_errset_(proc_ResrcLimitReachedE));
T_use_E$($set(proc_Env_E)(O$S_const$u8));

T_alias$((proc_Env_VTbl)(struct proc_Env_VTbl));
$extern let_(proc_Env_VTbl_empty, proc_Env_VTbl);

struct proc_Env {
    var_(ctx, P$raw);
    var_(vtbl, P_const$$(proc_Env_VTbl));
};
T_use_prl$(proc_Env);
$attr($inline_always)
$static fn_((proc_Env_isValid(proc_Env self))(bool));
$attr($inline_always)
$static fn_((proc_Env_assertValid(P$raw ctx, P_const$$(proc_Env_VTbl) vtbl))(void));
$attr($inline_always)
$static fn_((proc_Env_ensureValid(proc_Env self))(proc_Env));

$extern let_(proc_Env_empty, proc_Env);
$attr($must_check)
$extern fn_((proc_Env_by(proc_Env self, S_const$u8 name, S$u8 scratch))(proc_Env_E$O$S_const$u8));

T_alias$((proc_Env_Iter)(struct proc_Env_Iter {
    var_(src, proc_Env);
    var_(idx, usize);
    var_(offset, usize);
}));
T_use_prl$(proc_Env_Iter);
$extern fn_((proc_Env_iter(proc_Env self))(proc_Env_Iter));
$attr($must_check)
$extern fn_((proc_Env_Iter_next(proc_Env_Iter* self, S$u8 scratch))(proc_Env_E$O$S_const$u8));

/**
 * Iterator providers must leave `idx` and `offset` unchanged when returning
 * `proc_ResrcLimitReached`, so callers can retry with a larger scratch slice.
 */
struct proc_Env_VTbl {
    $attr($must_check)
    fn_(((*nextFn)(P$raw ctx, usize* idx, usize* offset, S$u8 scratch))(proc_Env_E$O$S_const$u8));
};
$attr($must_check)
$extern fn_((proc_Env_VTbl_emptyNext(P$raw ctx, usize* idx, usize* offset, S$u8 scratch))(proc_Env_E$O$S_const$u8));

/*========== Macros and Definitions =========================================*/

#if in_analysis_active_only || in_comptime
fn_((proc_Env_isValid(proc_Env self))(bool)) {
    return isNonnull(self.ctx)
        && isNonnull(self.vtbl)
        && isNonnull(self.vtbl->nextFn);
};
fn_((proc_Env_assertValid(P$raw ctx, P_const$$(proc_Env_VTbl) vtbl))(void)) {
    claim_assert_nonnull(ctx);
    claim_assert_nonnull(vtbl);
    claim_assert_nonnull(vtbl->nextFn);
};
fn_((proc_Env_ensureValid(proc_Env self))(proc_Env)) {
    return proc_Env_assertValid(self.ctx, self.vtbl), self;
};
#endif /* in_analysis_active_only || in_comptime */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* proc_Env__included */

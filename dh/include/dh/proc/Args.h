/**
 * @copyright Copyright (c) 2026 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    Args.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2026-07-30 (date of creation)
 * @updated 2026-07-31 (date of last update)
 * @ingroup dasae-headers(dh)/proc
 * @prefix  proc_Args
 *
 * @brief   Injectable process argument source and iterator
 */
#pragma once
#ifndef proc_Args__included
#define proc_Args__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "base.h"

/*========== Macros and Declarations ========================================*/

errset_((proc_Args_E)() $union_errset_(proc_ResourceLimitReached_E));
T_useBy$(($spec(E, $set(proc_Args_E)))(O$S_const$u8, bool));

T_alias$((proc_Args_VTbl)(struct proc_Args_VTbl));
T_alias$((proc_Args)(struct proc_Args {
    var_(ctx, P$raw);
    var_(vtbl, P_const$$(proc_Args_VTbl));
}));
T_use_prl$(proc_Args);
$attr($inline_always)
$static fn_((proc_Args_isValid(proc_Args self))(bool));
$attr($inline_always)
$static fn_((proc_Args_assertValid(P$raw ctx, P_const$$(proc_Args_VTbl) vtbl))(void));
$attr($inline_always)
$static fn_((proc_Args_ensureValid(proc_Args self))(proc_Args));

$extern let_(proc_Args_empty, proc_Args);

T_alias$((proc_Args_Iter)(struct proc_Args_Iter {
    var_(src, proc_Args);
    var_(idx, usize);
    var_(offset, usize);
}));
T_use_prl$(proc_Args_Iter);
$attr($must_check)
$extern fn_((proc_Args_iter(proc_Args self))(proc_Args_Iter));
$attr($must_check)
$extern fn_((proc_Args_Iter_next(proc_Args_Iter* self, S$u8 scratch))(proc_Args_E$O$S_const$u8));
$attr($must_check)
$extern fn_((proc_Args_Iter_skip(proc_Args_Iter* self))(proc_Args_E$bool));

/**
 * Iterator providers must leave `idx` and `offset` unchanged when returning
 * `proc_ResourceLimitReached`, so callers can retry with a larger scratch slice.
 */
struct proc_Args_VTbl {
    $attr($must_check)
    fn_(((*nextFn)(P$raw ctx, usize* idx, usize* offset, S$u8 scratch))(proc_Args_E$O$S_const$u8));
    $attr($must_check)
    fn_(((*skipFn)(P$raw ctx, usize* idx, usize* offset))(proc_Args_E$bool));
};

/*========== Macros and Definitions =========================================*/

#if in_analysis_active_only || in_comptime
fn_((proc_Args_isValid(proc_Args self))(bool)) {
    return isNonnull(self.ctx)
        && isNonnull(self.vtbl)
        && isNonnull(self.vtbl->nextFn)
        && isNonnull(self.vtbl->skipFn);
};
fn_((proc_Args_assertValid(P$raw ctx, P_const$$(proc_Args_VTbl) vtbl))(void)) {
    claim_assert_nonnull(ctx);
    claim_assert_nonnull(vtbl);
    claim_assert_nonnull(vtbl->nextFn);
    claim_assert_nonnull(vtbl->skipFn);
};
fn_((proc_Args_ensureValid(proc_Args self))(proc_Args)) {
    return proc_Args_assertValid(self.ctx, self.vtbl), self;
};
#endif /* in_analysis_active_only || in_comptime */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* proc_Args__included */

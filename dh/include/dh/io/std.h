/**
 * @copyright Copyright (c) 2026 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    std.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2026-07-31 (date of creation)
 * @updated 2026-07-31 (date of last update)
 * @ingroup dasae-headers(dh)/io
 * @prefix  io_std
 *
 * @brief   Injectable standard input, output, and error capability
 */
#pragma once
#ifndef io_std__included
#define io_std__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "Locked.h"

/*========== Macros and Declarations ========================================*/

errset_((io_std_direct_E)(io_std_direct_Unsupported));

T_alias$((io_std_Self_VTbl)(struct io_std_Self_VTbl));
T_alias$((io_std_Self)(struct io_std_Self {
    var_(ctx, P$raw);
    var_(vtbl, P_const$$(io_std_Self_VTbl));
}));
T_use_prl$(io_std_Self);
T_use_E$($set(io_std_direct_E)(io_std_Self));
$attr($inline_always $must_check)
$static fn_((io_std_isValid(io_std_Self self))(bool));
$attr($inline_always)
$static fn_((io_std_assertValid(P$raw ctx, P_const$$(io_std_Self_VTbl) vtbl))(void));
$attr($inline_always $must_check)
$static fn_((io_std_ensureValid(io_std_Self self))(io_std_Self));

$extern let_(io_std_noop, io_std_Self);
$extern let_(io_std_failing, io_std_Self);
$attr($must_check)
$extern fn_((io_std_direct(void))(io_std_direct_E$io_std_Self));

$attr($must_check)
$extern fn_((io_std_in(io_std_Self self))(io_Reader));
$attr($must_check)
$extern fn_((io_std_tryLockIn(io_std_Self self))(O$io_Locked_Reader));
$attr($must_check)
$extern fn_((io_std_lockIn(io_std_Self self))(io_Locked_Reader));

$attr($must_check)
$extern fn_((io_std_out(io_std_Self self))(io_Writer));
$attr($must_check)
$extern fn_((io_std_tryLockOut(io_std_Self self))(O$io_Locked_Writer));
$attr($must_check)
$extern fn_((io_std_lockOut(io_std_Self self))(io_Locked_Writer));

$attr($must_check)
$extern fn_((io_std_err(io_std_Self self))(io_Writer));
$attr($must_check)
$extern fn_((io_std_tryLockErr(io_std_Self self))(O$io_Locked_Writer));
$attr($must_check)
$extern fn_((io_std_lockErr(io_std_Self self))(io_Locked_Writer));

struct io_std_Self_VTbl {
    fn_(((*inFn)(P$raw ctx))(io_Reader));
    fn_(((*tryLockInFn)(P$raw ctx))(bool));
    fn_(((*lockInFn)(P$raw ctx))(void));
    fn_(((*unlockInFn)(P$raw ctx))(void));

    fn_(((*outFn)(P$raw ctx))(io_Writer));
    fn_(((*tryLockOutFn)(P$raw ctx))(bool));
    fn_(((*lockOutFn)(P$raw ctx))(void));
    fn_(((*unlockOutFn)(P$raw ctx))(void));

    fn_(((*errFn)(P$raw ctx))(io_Writer));
    fn_(((*tryLockErrFn)(P$raw ctx))(bool));
    fn_(((*lockErrFn)(P$raw ctx))(void));
    fn_(((*unlockErrFn)(P$raw ctx))(void));
};

$extern fn_((io_std_VTbl_noIn(P$raw ctx))(io_Reader));
$extern fn_((io_std_VTbl_failingIn(P$raw ctx))(io_Reader));
$extern fn_((io_std_VTbl_noTryLockIn(P$raw ctx))(bool));
$extern fn_((io_std_VTbl_unreachableTryLockIn(P$raw ctx))(bool));
$extern fn_((io_std_VTbl_noLockIn(P$raw ctx))(void));
$extern fn_((io_std_VTbl_unreachableLockIn(P$raw ctx))(void));
$extern fn_((io_std_VTbl_noUnlockIn(P$raw ctx))(void));
$extern fn_((io_std_VTbl_unreachableUnlockIn(P$raw ctx))(void));

$extern fn_((io_std_VTbl_noOut(P$raw ctx))(io_Writer));
$extern fn_((io_std_VTbl_failingOut(P$raw ctx))(io_Writer));
$extern fn_((io_std_VTbl_noTryLockOut(P$raw ctx))(bool));
$extern fn_((io_std_VTbl_unreachableTryLockOut(P$raw ctx))(bool));
$extern fn_((io_std_VTbl_noLockOut(P$raw ctx))(void));
$extern fn_((io_std_VTbl_unreachableLockOut(P$raw ctx))(void));
$extern fn_((io_std_VTbl_noUnlockOut(P$raw ctx))(void));
$extern fn_((io_std_VTbl_unreachableUnlockOut(P$raw ctx))(void));

$extern fn_((io_std_VTbl_noErr(P$raw ctx))(io_Writer));
$extern fn_((io_std_VTbl_failingErr(P$raw ctx))(io_Writer));
$extern fn_((io_std_VTbl_noTryLockErr(P$raw ctx))(bool));
$extern fn_((io_std_VTbl_unreachableTryLockErr(P$raw ctx))(bool));
$extern fn_((io_std_VTbl_noLockErr(P$raw ctx))(void));
$extern fn_((io_std_VTbl_unreachableLockErr(P$raw ctx))(void));
$extern fn_((io_std_VTbl_noUnlockErr(P$raw ctx))(void));
$extern fn_((io_std_VTbl_unreachableUnlockErr(P$raw ctx))(void));

/*========== Macros and Definitions =========================================*/

#if in_analysis_active_only || in_comptime
fn_((io_std_isValid(io_std_Self self))(bool)) {
    return isNonnull(self.ctx)
        && isNonnull(self.vtbl)
        && isNonnull(self.vtbl->inFn)
        && isNonnull(self.vtbl->tryLockInFn)
        && isNonnull(self.vtbl->lockInFn)
        && isNonnull(self.vtbl->unlockInFn)
        && isNonnull(self.vtbl->outFn)
        && isNonnull(self.vtbl->tryLockOutFn)
        && isNonnull(self.vtbl->lockOutFn)
        && isNonnull(self.vtbl->unlockOutFn)
        && isNonnull(self.vtbl->errFn)
        && isNonnull(self.vtbl->tryLockErrFn)
        && isNonnull(self.vtbl->lockErrFn)
        && isNonnull(self.vtbl->unlockErrFn);
};
fn_((io_std_assertValid(P$raw ctx, P_const$$(io_std_Self_VTbl) vtbl))(void)) {
    claim_assert_nonnull(ctx);
    claim_assert_nonnull(vtbl);
    claim_assert_nonnull(vtbl->inFn);
    claim_assert_nonnull(vtbl->tryLockInFn);
    claim_assert_nonnull(vtbl->lockInFn);
    claim_assert_nonnull(vtbl->unlockInFn);
    claim_assert_nonnull(vtbl->outFn);
    claim_assert_nonnull(vtbl->tryLockOutFn);
    claim_assert_nonnull(vtbl->lockOutFn);
    claim_assert_nonnull(vtbl->unlockOutFn);
    claim_assert_nonnull(vtbl->errFn);
    claim_assert_nonnull(vtbl->tryLockErrFn);
    claim_assert_nonnull(vtbl->lockErrFn);
    claim_assert_nonnull(vtbl->unlockErrFn);
};
fn_((io_std_ensureValid(io_std_Self self))(io_std_Self)) {
    return io_std_assertValid(self.ctx, self.vtbl), self;
};
#endif /* in_analysis_active_only || in_comptime */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* io_std__included */

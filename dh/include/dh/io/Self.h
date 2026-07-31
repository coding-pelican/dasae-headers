/**
 * @copyright Copyright (c) 2025-2026 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    Self.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2025-08-09 (date of creation)
 * @updated 2026-07-31 (date of last update)
 * @ingroup dal-project/da/io
 * @prefix  io_Self
 */
#pragma once
#ifndef io_Self__included
#define io_Self__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "common.h"
#include "../thrd/Mtx.h"

/*========== Macros and Declarations ========================================*/

T_alias$((io_Self_VTbl)(struct io_Self_VTbl));
/// Copyable capability for coordinated I/O operations.
T_alias$((io_Self)(struct io_Self {
    var_(ctx, P$raw);
    var_(vtbl, P_const$$(io_Self_VTbl));
}));
T_use_prl$(io_Self);
$extern let_(io_Self_noop, io_Self);
$extern let_(io_Self_failing, io_Self);

$attr($inline_always)
$static fn_((io_isValid(io_Self self))(bool));
$attr($inline_always)
$static fn_((io_assertValid(P$raw ctx, P_const$$(io_Self_VTbl) vtbl))(void));
$attr($inline_always)
$static fn_((io_ensureValid(io_Self self))(io_Self));

$extern fn_((io_lockStdIn(io_Self self))(void));
$extern fn_((io_tryLockStdIn(io_Self self))(bool));
$extern fn_((io_unlockStdIn(io_Self self))(void));

$extern fn_((io_lockStdOut(io_Self self))(void));
$extern fn_((io_tryLockStdOut(io_Self self))(bool));
$extern fn_((io_unlockStdOut(io_Self self))(void));

$extern fn_((io_lockStdErr(io_Self self))(void));
$extern fn_((io_tryLockStdErr(io_Self self))(bool));
$extern fn_((io_unlockStdErr(io_Self self))(void));

struct io_Self_VTbl {
    fn_(((*lockStdInFn)(P$raw ctx))(void));
    fn_(((*tryLockStdInFn)(P$raw ctx))(bool));
    fn_(((*unlockStdInFn)(P$raw ctx))(void));
    fn_(((*lockStdOutFn)(P$raw ctx))(void));
    fn_(((*tryLockStdOutFn)(P$raw ctx))(bool));
    fn_(((*unlockStdOutFn)(P$raw ctx))(void));
    fn_(((*lockStdErrFn)(P$raw ctx))(void));
    fn_(((*tryLockStdErrFn)(P$raw ctx))(bool));
    fn_(((*unlockStdErrFn)(P$raw ctx))(void));
};

/// Caller-owned native I/O coordination provider.
T_alias$((io_Direct)(struct io_Direct {
    var_(std_in_mtx, thrd_Mtx_Recur);
    var_(std_out_mtx, thrd_Mtx_Recur);
    var_(std_err_mtx, thrd_Mtx_Recur);
}));
$extern fn_((io_Direct_init(void))(io_Direct));
$extern fn_((io_Direct_fini(io_Direct* self))(void));
$extern fn_((io_Direct_self(io_Direct* self))(io_Self));

/*========== Macros and Definitions =========================================*/

#if in_analysis_active_only || in_comptime
fn_((io_isValid(io_Self self))(bool)) {
    return isNonnull(self.ctx)
        && isNonnull(self.vtbl)
        && isNonnull(self.vtbl->lockStdInFn)
        && isNonnull(self.vtbl->tryLockStdInFn)
        && isNonnull(self.vtbl->unlockStdInFn)
        && isNonnull(self.vtbl->lockStdOutFn)
        && isNonnull(self.vtbl->tryLockStdOutFn)
        && isNonnull(self.vtbl->unlockStdOutFn)
        && isNonnull(self.vtbl->lockStdErrFn)
        && isNonnull(self.vtbl->tryLockStdErrFn)
        && isNonnull(self.vtbl->unlockStdErrFn);
};
fn_((io_assertValid(P$raw ctx, P_const$$(io_Self_VTbl) vtbl))(void)) {
    claim_assert_nonnull(ctx);
    claim_assert_nonnull(vtbl);
    claim_assert_nonnull(vtbl->lockStdInFn);
    claim_assert_nonnull(vtbl->tryLockStdInFn);
    claim_assert_nonnull(vtbl->unlockStdInFn);
    claim_assert_nonnull(vtbl->lockStdOutFn);
    claim_assert_nonnull(vtbl->tryLockStdOutFn);
    claim_assert_nonnull(vtbl->unlockStdOutFn);
    claim_assert_nonnull(vtbl->lockStdErrFn);
    claim_assert_nonnull(vtbl->tryLockStdErrFn);
    claim_assert_nonnull(vtbl->unlockStdErrFn);
};
fn_((io_ensureValid(io_Self self))(io_Self)) {
    return io_assertValid(self.ctx, self.vtbl), self;
};
#endif /* in_analysis_active_only || in_comptime */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* io_Self__included */

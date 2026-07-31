#include "dh/io/Self.h"

#if plat_is_windows
#include "dh/sys/api/windows/console.h"
#include "dh/sys/api/windows/nls.h"
#endif /* plat_is_windows */
#if io_using_libc
#include <locale.h>
#endif /* io_using_libc */

$static fn_((io_Self__noop(P$raw ctx))(void)) {
    claim_assert_nonnull(ctx);
};
$static fn_((io_Self__noopTry(P$raw ctx))(bool)) {
    claim_assert_nonnull(ctx);
    return true;
};
$static fn_((io_Self__failing(P$raw ctx))(void)) {
    claim_assert_nonnull(ctx);
    claim_unreachable;
};
$static fn_((io_Self__failingTry(P$raw ctx))(bool)) {
    claim_assert_nonnull(ctx);
    return false;
};

$static let_(io_Self__noop_vtbl, io_Self_VTbl) = {
    .lockStdInFn = io_Self__noop,
    .tryLockStdInFn = io_Self__noopTry,
    .unlockStdInFn = io_Self__noop,
    .lockStdOutFn = io_Self__noop,
    .tryLockStdOutFn = io_Self__noopTry,
    .unlockStdOutFn = io_Self__noop,
    .lockStdErrFn = io_Self__noop,
    .tryLockStdErrFn = io_Self__noopTry,
    .unlockStdErrFn = io_Self__noop,
};
$static let_(io_Self__failing_vtbl, io_Self_VTbl) = {
    .lockStdInFn = io_Self__failing,
    .tryLockStdInFn = io_Self__failingTry,
    .unlockStdInFn = io_Self__failing,
    .lockStdOutFn = io_Self__failing,
    .tryLockStdOutFn = io_Self__failingTry,
    .unlockStdOutFn = io_Self__failing,
    .lockStdErrFn = io_Self__failing,
    .tryLockStdErrFn = io_Self__failingTry,
    .unlockStdErrFn = io_Self__failing,
};

$static var_(io_Self__noop_ctx, Void) = cleared();
let_(io_Self_noop, io_Self) = {
    .ctx = &io_Self__noop_ctx,
    .vtbl = &io_Self__noop_vtbl,
};
$static var_(io_Self__failing_ctx, Void) = cleared();
let_(io_Self_failing, io_Self) = {
    .ctx = &io_Self__failing_ctx,
    .vtbl = &io_Self__failing_vtbl,
};

$static fn_((io_Direct__ctx(P$raw ctx))(io_Direct*)) {
    return ptrCast$((io_Direct*)(ensureNonnull(ctx)));
};
$static fn_((io_Direct__lockStdIn(P$raw ctx))(void)) {
    thrd_Mtx_Recur_lockProtcd(&io_Direct__ctx(ctx)->std_in_mtx);
};
$static fn_((io_Direct__tryLockStdIn(P$raw ctx))(bool)) {
    return thrd_Mtx_Recur_tryLock(&io_Direct__ctx(ctx)->std_in_mtx);
};
$static fn_((io_Direct__unlockStdIn(P$raw ctx))(void)) {
    thrd_Mtx_Recur_unlock(&io_Direct__ctx(ctx)->std_in_mtx);
};
$static fn_((io_Direct__lockStdOut(P$raw ctx))(void)) {
    thrd_Mtx_Recur_lockProtcd(&io_Direct__ctx(ctx)->std_out_mtx);
};
$static fn_((io_Direct__tryLockStdOut(P$raw ctx))(bool)) {
    return thrd_Mtx_Recur_tryLock(&io_Direct__ctx(ctx)->std_out_mtx);
};
$static fn_((io_Direct__unlockStdOut(P$raw ctx))(void)) {
    thrd_Mtx_Recur_unlock(&io_Direct__ctx(ctx)->std_out_mtx);
};
$static fn_((io_Direct__lockStdErr(P$raw ctx))(void)) {
    thrd_Mtx_Recur_lockProtcd(&io_Direct__ctx(ctx)->std_err_mtx);
};
$static fn_((io_Direct__tryLockStdErr(P$raw ctx))(bool)) {
    return thrd_Mtx_Recur_tryLock(&io_Direct__ctx(ctx)->std_err_mtx);
};
$static fn_((io_Direct__unlockStdErr(P$raw ctx))(void)) {
    thrd_Mtx_Recur_unlock(&io_Direct__ctx(ctx)->std_err_mtx);
};

$static let_(io_Direct__vtbl, io_Self_VTbl) = {
    .lockStdInFn = io_Direct__lockStdIn,
    .tryLockStdInFn = io_Direct__tryLockStdIn,
    .unlockStdInFn = io_Direct__unlockStdIn,
    .lockStdOutFn = io_Direct__lockStdOut,
    .tryLockStdOutFn = io_Direct__tryLockStdOut,
    .unlockStdOutFn = io_Direct__unlockStdOut,
    .lockStdErrFn = io_Direct__lockStdErr,
    .tryLockStdErrFn = io_Direct__tryLockStdErr,
    .unlockStdErrFn = io_Direct__unlockStdErr,
};

$attr($on_load)
$static fn_((io__init(void))(void)) {
#if io_pre_ensured_utf8_env_enabled
#if plat_is_windows
    // [Console]::OutputEncoding = [System.Text.Encoding]::UTF8
    // chcp 65001
    let_ignore = SetConsoleCP(CP_UTF8);
    let_ignore = SetConsoleOutputCP(CP_UTF8);
#endif /* plat_is_windows */
#if io_using_libc
    /* NOLINTNEXTLINE(concurrency-mt-unsafe) */
    let_ignore = setlocale(LC_ALL, ".UTF-8"); /* Code page 65001 */
#endif /* io_using_libc */
#endif /* io_pre_ensured_utf8_env_enabled */
};

fn_((io_lockStdIn(io_Self self))(void)) {
    self = io_ensureValid(self);
    self.vtbl->lockStdInFn(self.ctx);
};
fn_((io_tryLockStdIn(io_Self self))(bool)) {
    self = io_ensureValid(self);
    return self.vtbl->tryLockStdInFn(self.ctx);
};
fn_((io_unlockStdIn(io_Self self))(void)) {
    self = io_ensureValid(self);
    self.vtbl->unlockStdInFn(self.ctx);
};
fn_((io_lockStdOut(io_Self self))(void)) {
    self = io_ensureValid(self);
    self.vtbl->lockStdOutFn(self.ctx);
};
fn_((io_tryLockStdOut(io_Self self))(bool)) {
    self = io_ensureValid(self);
    return self.vtbl->tryLockStdOutFn(self.ctx);
};
fn_((io_unlockStdOut(io_Self self))(void)) {
    self = io_ensureValid(self);
    self.vtbl->unlockStdOutFn(self.ctx);
};
fn_((io_lockStdErr(io_Self self))(void)) {
    self = io_ensureValid(self);
    self.vtbl->lockStdErrFn(self.ctx);
};
fn_((io_tryLockStdErr(io_Self self))(bool)) {
    self = io_ensureValid(self);
    return self.vtbl->tryLockStdErrFn(self.ctx);
};
fn_((io_unlockStdErr(io_Self self))(void)) {
    self = io_ensureValid(self);
    self.vtbl->unlockStdErrFn(self.ctx);
};

fn_((io_Direct_init(void))(io_Direct)) {
    return (io_Direct){
        .std_in_mtx = thrd_Mtx_Recur_init(),
        .std_out_mtx = thrd_Mtx_Recur_init(),
        .std_err_mtx = thrd_Mtx_Recur_init(),
    };
};
fn_((io_Direct_fini(io_Direct* self))(void)) {
    claim_assert_nonnull(self);
    thrd_Mtx_Recur_fini(&self->std_err_mtx);
    thrd_Mtx_Recur_fini(&self->std_out_mtx);
    thrd_Mtx_Recur_fini(&self->std_in_mtx);
};
fn_((io_Direct_self(io_Direct* self))(io_Self)) {
    claim_assert_nonnull(self);
    return io_ensureValid((io_Self){
        .ctx = self,
        .vtbl = &io_Direct__vtbl,
    });
};

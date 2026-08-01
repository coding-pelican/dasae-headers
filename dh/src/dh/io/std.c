#include "dh/io/std.h"
#include "dh/fs/File/self.h"
#include "dh/thrd/Mtx.h"

#if plat_is_windows
#include "dh/sys/api/windows/console.h"
#elif plat_is_posix
#include "dh/sys/posix.h"
#endif

/*========== External Definitions ===========================================*/

$static var_(io_std__ctx_noop, Void) $undefined_static;
$static let_(io_std__vtbl_noop, io_std_Self_VTbl) = {
    .inFn = io_std_VTbl_noIn,
    .tryLockInFn = io_std_VTbl_noTryLockIn,
    .lockInFn = io_std_VTbl_noLockIn,
    .unlockInFn = io_std_VTbl_noUnlockIn,
    .outFn = io_std_VTbl_noOut,
    .tryLockOutFn = io_std_VTbl_noTryLockOut,
    .lockOutFn = io_std_VTbl_noLockOut,
    .unlockOutFn = io_std_VTbl_noUnlockOut,
    .errFn = io_std_VTbl_noErr,
    .tryLockErrFn = io_std_VTbl_noTryLockErr,
    .lockErrFn = io_std_VTbl_noLockErr,
    .unlockErrFn = io_std_VTbl_noUnlockErr,
};
let_(io_std_noop, io_std_Self) = {
    .ctx = &io_std__ctx_noop,
    .vtbl = &io_std__vtbl_noop,
};

$static var_(io_std__ctx_failing, Void) $undefined_static;
$static let_(io_std__vtbl_failing, io_std_Self_VTbl) = {
    .inFn = io_std_VTbl_failingIn,
    .tryLockInFn = io_std_VTbl_noTryLockIn,
    .lockInFn = io_std_VTbl_noLockIn,
    .unlockInFn = io_std_VTbl_noUnlockIn,
    .outFn = io_std_VTbl_failingOut,
    .tryLockOutFn = io_std_VTbl_noTryLockOut,
    .lockOutFn = io_std_VTbl_noLockOut,
    .unlockOutFn = io_std_VTbl_noUnlockOut,
    .errFn = io_std_VTbl_failingErr,
    .tryLockErrFn = io_std_VTbl_noTryLockErr,
    .lockErrFn = io_std_VTbl_noLockErr,
    .unlockErrFn = io_std_VTbl_noUnlockErr,
};
let_(io_std_failing, io_std_Self) = {
    .ctx = &io_std__ctx_failing,
    .vtbl = &io_std__vtbl_failing,
};

$static fn_((io_std_direct__in(P$raw ctx))(io_Reader));
$static fn_((io_std_direct__tryLockIn(P$raw ctx))(bool));
$static fn_((io_std_direct__lockIn(P$raw ctx))(void));
$static fn_((io_std_direct__unlockIn(P$raw ctx))(void));
$static fn_((io_std_direct__out(P$raw ctx))(io_Writer));
$static fn_((io_std_direct__tryLockOut(P$raw ctx))(bool));
$static fn_((io_std_direct__lockOut(P$raw ctx))(void));
$static fn_((io_std_direct__unlockOut(P$raw ctx))(void));
$static fn_((io_std_direct__err(P$raw ctx))(io_Writer));
$static fn_((io_std_direct__tryLockErr(P$raw ctx))(bool));
$static fn_((io_std_direct__lockErr(P$raw ctx))(void));
$static fn_((io_std_direct__unlockErr(P$raw ctx))(void));
T_alias$((io_std_direct__Ctx)(struct io_std_direct__Ctx {
    var_(in_mtx, thrd_Mtx_Recur);
    var_(out_mtx, thrd_Mtx_Recur);
    var_(err_mtx, thrd_Mtx_Recur);
}));
fn_((io_std_direct(void))(io_std_direct_E$io_std_Self) $scope) {
    pp_if_(pp_or(plat_is_windows, plat_is_posix))(
        pp_then_({
            $static var_(ctx, io_std_direct__Ctx) = {
                .in_mtx = thrd_Mtx_Recur_init_static(),
                .out_mtx = thrd_Mtx_Recur_init_static(),
                .err_mtx = thrd_Mtx_Recur_init_static(),
            };
            $static let_(vtbl, io_std_Self_VTbl) = {
                .inFn = io_std_direct__in,
                .tryLockInFn = io_std_direct__tryLockIn,
                .lockInFn = io_std_direct__lockIn,
                .unlockInFn = io_std_direct__unlockIn,
                .outFn = io_std_direct__out,
                .tryLockOutFn = io_std_direct__tryLockOut,
                .lockOutFn = io_std_direct__lockOut,
                .unlockOutFn = io_std_direct__unlockOut,
                .errFn = io_std_direct__err,
                .tryLockErrFn = io_std_direct__tryLockErr,
                .lockErrFn = io_std_direct__lockErr,
                .unlockErrFn = io_std_direct__unlockErr,
            };
            return_ok(io_std_ensureValid((io_std_Self){
                .ctx = &ctx,
                .vtbl = &vtbl,
            }));
        }),
        pp_else_({
            return_err(E_cause$io_std_direct_Unsupported());
        })
    );
} $unscoped(fn);

fn_((io_std_in(io_std_Self self))(io_Reader)) {
    self = io_std_ensureValid(self);
    return io_Reader_ensureValid(self.vtbl->inFn(self.ctx));
};
fn_((io_std_tryLockIn(io_std_Self self))(O$io_Locked_Reader)) {
    self = io_std_ensureValid(self);
    if (!self.vtbl->tryLockInFn(self.ctx)) {
        return none$((O$io_Locked_Reader));
    }
    return some$((O$io_Locked_Reader)(io_Locked_Reader_from(
        self.vtbl->inFn(self.ctx),
        l$((io_Locked){
            .ctx = self.ctx,
            .unlockFn = self.vtbl->unlockInFn,
        })
    )));
};
fn_((io_std_lockIn(io_std_Self self))(io_Locked_Reader)) {
    self = io_std_ensureValid(self);
    self.vtbl->lockInFn(self.ctx);
    return io_Locked_Reader_from(
        self.vtbl->inFn(self.ctx),
        l$((io_Locked){
            .ctx = self.ctx,
            .unlockFn = self.vtbl->unlockInFn,
        })
    );
};

fn_((io_std_out(io_std_Self self))(io_Writer)) {
    self = io_std_ensureValid(self);
    return io_Writer_ensureValid(self.vtbl->outFn(self.ctx));
};
fn_((io_std_tryLockOut(io_std_Self self))(O$io_Locked_Writer)) {
    self = io_std_ensureValid(self);
    if (!self.vtbl->tryLockOutFn(self.ctx)) {
        return none$((O$io_Locked_Writer));
    }
    return some$((O$io_Locked_Writer)(io_Locked_Writer_from(
        self.vtbl->outFn(self.ctx),
        l$((io_Locked){
            .ctx = self.ctx,
            .unlockFn = self.vtbl->unlockOutFn,
        })
    )));
};
fn_((io_std_lockOut(io_std_Self self))(io_Locked_Writer)) {
    self = io_std_ensureValid(self);
    self.vtbl->lockOutFn(self.ctx);
    return io_Locked_Writer_from(
        self.vtbl->outFn(self.ctx),
        l$((io_Locked){
            .ctx = self.ctx,
            .unlockFn = self.vtbl->unlockOutFn,
        })
    );
};

fn_((io_std_err(io_std_Self self))(io_Writer)) {
    self = io_std_ensureValid(self);
    return io_Writer_ensureValid(self.vtbl->errFn(self.ctx));
};
fn_((io_std_tryLockErr(io_std_Self self))(O$io_Locked_Writer)) {
    self = io_std_ensureValid(self);
    if (!self.vtbl->tryLockErrFn(self.ctx)) {
        return none$((O$io_Locked_Writer));
    }
    return some$((O$io_Locked_Writer)(io_Locked_Writer_from(
        self.vtbl->errFn(self.ctx),
        l$((io_Locked){
            .ctx = self.ctx,
            .unlockFn = self.vtbl->unlockErrFn,
        })
    )));
};
fn_((io_std_lockErr(io_std_Self self))(io_Locked_Writer)) {
    self = io_std_ensureValid(self);
    self.vtbl->lockErrFn(self.ctx);
    return io_Locked_Writer_from(
        self.vtbl->errFn(self.ctx),
        l$((io_Locked){
            .ctx = self.ctx,
            .unlockFn = self.vtbl->unlockErrFn,
        })
    );
};

fn_((io_std_VTbl_noIn(P$raw ctx))(io_Reader)) {
    let_ignore = ctx;
    return io_Reader_noop;
};
fn_((io_std_VTbl_failingIn(P$raw ctx))(io_Reader)) {
    let_ignore = ctx;
    return io_Reader_failing;
};
fn_((io_std_VTbl_noTryLockIn(P$raw ctx))(bool)) {
    let_ignore = ctx;
    return true;
};
fn_((io_std_VTbl_unreachableTryLockIn(P$raw ctx))(bool)) {
    let_ignore = ctx;
    claim_unreachable;
};
fn_((io_std_VTbl_noLockIn(P$raw ctx))(void)) {
    let_ignore = ctx;
};
fn_((io_std_VTbl_unreachableLockIn(P$raw ctx))(void)) {
    let_ignore = ctx;
    claim_unreachable;
};
fn_((io_std_VTbl_noUnlockIn(P$raw ctx))(void)) {
    let_ignore = ctx;
};
fn_((io_std_VTbl_unreachableUnlockIn(P$raw ctx))(void)) {
    let_ignore = ctx;
    claim_unreachable;
};

fn_((io_std_VTbl_noOut(P$raw ctx))(io_Writer)) {
    let_ignore = ctx;
    return io_Writer_noop;
};
fn_((io_std_VTbl_failingOut(P$raw ctx))(io_Writer)) {
    let_ignore = ctx;
    return io_Writer_failing;
};
fn_((io_std_VTbl_noTryLockOut(P$raw ctx))(bool)) {
    let_ignore = ctx;
    return true;
};
fn_((io_std_VTbl_unreachableTryLockOut(P$raw ctx))(bool)) {
    let_ignore = ctx;
    claim_unreachable;
};
fn_((io_std_VTbl_noLockOut(P$raw ctx))(void)) {
    let_ignore = ctx;
};
fn_((io_std_VTbl_unreachableLockOut(P$raw ctx))(void)) {
    let_ignore = ctx;
    claim_unreachable;
};
fn_((io_std_VTbl_noUnlockOut(P$raw ctx))(void)) {
    let_ignore = ctx;
};
fn_((io_std_VTbl_unreachableUnlockOut(P$raw ctx))(void)) {
    let_ignore = ctx;
    claim_unreachable;
};

fn_((io_std_VTbl_noErr(P$raw ctx))(io_Writer)) {
    let_ignore = ctx;
    return io_Writer_noop;
};
fn_((io_std_VTbl_failingErr(P$raw ctx))(io_Writer)) {
    let_ignore = ctx;
    return io_Writer_failing;
};
fn_((io_std_VTbl_noTryLockErr(P$raw ctx))(bool)) {
    let_ignore = ctx;
    return true;
};
fn_((io_std_VTbl_unreachableTryLockErr(P$raw ctx))(bool)) {
    let_ignore = ctx;
    claim_unreachable;
};
fn_((io_std_VTbl_noLockErr(P$raw ctx))(void)) {
    let_ignore = ctx;
};
fn_((io_std_VTbl_unreachableLockErr(P$raw ctx))(void)) {
    let_ignore = ctx;
    claim_unreachable;
};
fn_((io_std_VTbl_noUnlockErr(P$raw ctx))(void)) {
    let_ignore = ctx;
};
fn_((io_std_VTbl_unreachableUnlockErr(P$raw ctx))(void)) {
    let_ignore = ctx;
    claim_unreachable;
};

/*========== Internal Definitions ===========================================*/

fn_((io_std_direct__in(P$raw ctx))(io_Reader)) {
    claim_assert_nonnull(ctx);
    let file = fs_File_Handle_promote(
        pp_if_(plat_is_windows)(
            pp_then_(GetStdHandle(STD_INPUT_HANDLE)),
            pp_else_(sys_posix_STDIN_FILENO)),
        fs_File_Flags_default
    );
    return fs_File_reader(file);
};
fn_((io_std_direct__tryLockIn(P$raw ctx))(bool)) {
    claim_assert_nonnull(ctx);
    return thrd_Mtx_Recur_tryLock(&((io_std_direct__Ctx*)ctx)->in_mtx);
};
fn_((io_std_direct__lockIn(P$raw ctx))(void)) {
    claim_assert_nonnull(ctx);
    thrd_Mtx_Recur_lockProtcd(&((io_std_direct__Ctx*)ctx)->in_mtx);
};
fn_((io_std_direct__unlockIn(P$raw ctx))(void)) {
    claim_assert_nonnull(ctx);
    thrd_Mtx_Recur_unlock(&((io_std_direct__Ctx*)ctx)->in_mtx);
};

fn_((io_std_direct__out(P$raw ctx))(io_Writer)) {
    claim_assert_nonnull(ctx);
    let file = fs_File_Handle_promote(
        pp_if_(plat_is_windows)(
            pp_then_(GetStdHandle(STD_OUTPUT_HANDLE)),
            pp_else_(sys_posix_STDOUT_FILENO)),
        fs_File_Flags_default
    );
    return fs_File_writer(file);
};
fn_((io_std_direct__tryLockOut(P$raw ctx))(bool)) {
    claim_assert_nonnull(ctx);
    return thrd_Mtx_Recur_tryLock(&((io_std_direct__Ctx*)ctx)->out_mtx);
};
fn_((io_std_direct__lockOut(P$raw ctx))(void)) {
    claim_assert_nonnull(ctx);
    thrd_Mtx_Recur_lockProtcd(&((io_std_direct__Ctx*)ctx)->out_mtx);
};
fn_((io_std_direct__unlockOut(P$raw ctx))(void)) {
    claim_assert_nonnull(ctx);
    thrd_Mtx_Recur_unlock(&((io_std_direct__Ctx*)ctx)->out_mtx);
};

fn_((io_std_direct__err(P$raw ctx))(io_Writer)) {
    claim_assert_nonnull(ctx);
    let file = fs_File_Handle_promote(
        pp_if_(plat_is_windows)(
            pp_then_(GetStdHandle(STD_ERROR_HANDLE)),
            pp_else_(sys_posix_STDERR_FILENO)),
        fs_File_Flags_default
    );
    return fs_File_writer(file);
};
fn_((io_std_direct__tryLockErr(P$raw ctx))(bool)) {
    claim_assert_nonnull(ctx);
    return thrd_Mtx_Recur_tryLock(&((io_std_direct__Ctx*)ctx)->err_mtx);
};
fn_((io_std_direct__lockErr(P$raw ctx))(void)) {
    claim_assert_nonnull(ctx);
    thrd_Mtx_Recur_lockProtcd(&((io_std_direct__Ctx*)ctx)->err_mtx);
};
fn_((io_std_direct__unlockErr(P$raw ctx))(void)) {
    claim_assert_nonnull(ctx);
    thrd_Mtx_Recur_unlock(&((io_std_direct__Ctx*)ctx)->err_mtx);
};

#include "dh/io/std.h"
#include "dh/fs/File/std.h"
#include "dh/thrd/Mtx.h"

#if io_pre_ensured_utf8_env_enabled && plat_is_windows
#include "dh/sys/api/windows/console.h"
#include "dh/sys/api/windows/nls.h"
#endif /* io_pre_ensured_utf8_env_enabled && plat_is_windows */
#if io_pre_ensured_utf8_env_enabled && io_using_libc
#include <locale.h>
#endif /* io_pre_ensured_utf8_env_enabled && io_using_libc */

/*========== Internal Declarations ==========================================*/

$static fn_((io_std_direct__ensureUTF8Env(void))(void));
T_alias$((io_std_direct__Ctx)(struct io_std_direct__Ctx {
    var_(in_file, fs_File);
    var_(in_mtx, thrd_Mtx_Recur);
    var_(out_file, fs_File);
    var_(out_mtx, thrd_Mtx_Recur);
    var_(err_file, fs_File);
    var_(err_mtx, thrd_Mtx_Recur);
}));
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

/*========== External Definitions ===========================================*/

$static var_(io_std__ctx_noop, Void) $undefined_static;
let_(io_std_VTbl_noop, io_std_Self_VTbl) = {
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
    .vtbl = &io_std_VTbl_noop,
};

$static var_(io_std__ctx_failing, Void) $undefined_static;
let_(io_std_VTbl_failing, io_std_Self_VTbl) = {
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
    .vtbl = &io_std_VTbl_failing,
};

/* TODO: Using `thrd_LazyLock` */
fn_((io_std_direct(void))(io_std_direct_E$io_std_Self) $scope) {
    pp_if_(pp_or(plat_is_windows, plat_is_posix))(
        pp_then_({
            $static var_(ctx, O$$(io_std_direct__Ctx)) = none();
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
            let ctx_initialized = orelse_((O_ref(&ctx))(local_({
                io_std_direct__ensureUTF8Env();
                let fs_File_std = catch_((fs_File_std_direct())(err, switch (E_tag$fs_File_std_direct_E(err)) {
                    case_((E_Tag$fs_File_std_direct_Unsupported)) return_err(
                        E_cause$io_std_direct_Unsupported()
                    ) $end(case);
                    case_((E_Tag$Any)) claim_unreachable $end(case);
                }));
                asg_l((&ctx)(some({
                    .in_file = fs_File_std_in(fs_File_std),
                    .in_mtx = thrd_Mtx_Recur_init(),
                    .out_file = fs_File_std_out(fs_File_std),
                    .out_mtx = thrd_Mtx_Recur_init(),
                    .err_file = fs_File_std_err(fs_File_std),
                    .err_mtx = thrd_Mtx_Recur_init(),
                })));
                local_return_(unwrap_(O_ref(&ctx)));
            })));
            return_ok(io_std_ensureValid((io_std_Self){
                .ctx = ctx_initialized,
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
fn_((io_std_tryLockIn(io_std_Self self))(O$io_Locked_Reader) $scope) {
    self = io_std_ensureValid(self);
    if (!self.vtbl->tryLockInFn(self.ctx)) {
        return_none();
    }
    var_(lock, io_Locked) = {
        .ctx = self.ctx,
        .unlockFn = self.vtbl->unlockInFn,
    };
    return_some(io_Locked_Reader_adopt(
        self.vtbl->inFn(self.ctx),
        &lock
    ));
} $unscoped(fn);
fn_((io_std_lockIn(io_std_Self self))(io_Locked_Reader)) {
    self = io_std_ensureValid(self);
    self.vtbl->lockInFn(self.ctx);
    var_(lock, io_Locked) = {
        .ctx = self.ctx,
        .unlockFn = self.vtbl->unlockInFn,
    };
    return io_Locked_Reader_adopt(
        self.vtbl->inFn(self.ctx),
        &lock
    );
};

fn_((io_std_out(io_std_Self self))(io_Writer)) {
    self = io_std_ensureValid(self);
    return io_Writer_ensureValid(self.vtbl->outFn(self.ctx));
};
fn_((io_std_tryLockOut(io_std_Self self))(O$io_Locked_Writer) $scope) {
    self = io_std_ensureValid(self);
    if (!self.vtbl->tryLockOutFn(self.ctx)) {
        return_none();
    }
    var_(lock, io_Locked) = {
        .ctx = self.ctx,
        .unlockFn = self.vtbl->unlockOutFn,
    };
    return_some(io_Locked_Writer_adopt(
        self.vtbl->outFn(self.ctx),
        &lock
    ));
} $unscoped(fn);
fn_((io_std_lockOut(io_std_Self self))(io_Locked_Writer)) {
    self = io_std_ensureValid(self);
    self.vtbl->lockOutFn(self.ctx);
    var_(lock, io_Locked) = {
        .ctx = self.ctx,
        .unlockFn = self.vtbl->unlockOutFn,
    };
    return io_Locked_Writer_adopt(
        self.vtbl->outFn(self.ctx),
        &lock
    );
};

fn_((io_std_err(io_std_Self self))(io_Writer)) {
    self = io_std_ensureValid(self);
    return io_Writer_ensureValid(self.vtbl->errFn(self.ctx));
};
fn_((io_std_tryLockErr(io_std_Self self))(O$io_Locked_Writer) $scope) {
    self = io_std_ensureValid(self);
    if (!self.vtbl->tryLockErrFn(self.ctx)) {
        return_none();
    }
    var_(lock, io_Locked) = {
        .ctx = self.ctx,
        .unlockFn = self.vtbl->unlockErrFn,
    };
    return_some(io_Locked_Writer_adopt(
        self.vtbl->errFn(self.ctx),
        &lock
    ));
} $unscoped(fn);
fn_((io_std_lockErr(io_std_Self self))(io_Locked_Writer)) {
    self = io_std_ensureValid(self);
    self.vtbl->lockErrFn(self.ctx);
    var_(lock, io_Locked) = {
        .ctx = self.ctx,
        .unlockFn = self.vtbl->unlockErrFn,
    };
    return io_Locked_Writer_adopt(
        self.vtbl->errFn(self.ctx),
        &lock
    );
};

fn_((io_std_VTbl_noIn(P$raw ctx))(io_Reader)) {
    let_ignore = ensureNonnull(ctx);
    return io_Reader_noop;
};
fn_((io_std_VTbl_failingIn(P$raw ctx))(io_Reader)) {
    let_ignore = ensureNonnull(ctx);
    return io_Reader_failing;
};
fn_((io_std_VTbl_noTryLockIn(P$raw ctx))(bool)) {
    let_ignore = ensureNonnull(ctx);
    return true;
};
fn_((io_std_VTbl_unreachableTryLockIn(P$raw ctx))(bool)) {
    let_ignore = ensureNonnull(ctx);
    claim_unreachable;
};
fn_((io_std_VTbl_noLockIn(P$raw ctx))(void)) {
    let_ignore = ensureNonnull(ctx);
};
fn_((io_std_VTbl_unreachableLockIn(P$raw ctx))(void)) {
    let_ignore = ensureNonnull(ctx);
    claim_unreachable;
};
fn_((io_std_VTbl_noUnlockIn(P$raw ctx))(void)) {
    let_ignore = ensureNonnull(ctx);
};
fn_((io_std_VTbl_unreachableUnlockIn(P$raw ctx))(void)) {
    let_ignore = ensureNonnull(ctx);
    claim_unreachable;
};

fn_((io_std_VTbl_noOut(P$raw ctx))(io_Writer)) {
    let_ignore = ensureNonnull(ctx);
    return io_Writer_noop;
};
fn_((io_std_VTbl_failingOut(P$raw ctx))(io_Writer)) {
    let_ignore = ensureNonnull(ctx);
    return io_Writer_failing;
};
fn_((io_std_VTbl_noTryLockOut(P$raw ctx))(bool)) {
    let_ignore = ensureNonnull(ctx);
    return true;
};
fn_((io_std_VTbl_unreachableTryLockOut(P$raw ctx))(bool)) {
    let_ignore = ensureNonnull(ctx);
    claim_unreachable;
};
fn_((io_std_VTbl_noLockOut(P$raw ctx))(void)) {
    let_ignore = ensureNonnull(ctx);
};
fn_((io_std_VTbl_unreachableLockOut(P$raw ctx))(void)) {
    let_ignore = ensureNonnull(ctx);
    claim_unreachable;
};
fn_((io_std_VTbl_noUnlockOut(P$raw ctx))(void)) {
    let_ignore = ensureNonnull(ctx);
};
fn_((io_std_VTbl_unreachableUnlockOut(P$raw ctx))(void)) {
    let_ignore = ensureNonnull(ctx);
    claim_unreachable;
};

fn_((io_std_VTbl_noErr(P$raw ctx))(io_Writer)) {
    let_ignore = ensureNonnull(ctx);
    return io_Writer_noop;
};
fn_((io_std_VTbl_failingErr(P$raw ctx))(io_Writer)) {
    let_ignore = ensureNonnull(ctx);
    return io_Writer_failing;
};
fn_((io_std_VTbl_noTryLockErr(P$raw ctx))(bool)) {
    let_ignore = ensureNonnull(ctx);
    return true;
};
fn_((io_std_VTbl_unreachableTryLockErr(P$raw ctx))(bool)) {
    let_ignore = ensureNonnull(ctx);
    claim_unreachable;
};
fn_((io_std_VTbl_noLockErr(P$raw ctx))(void)) {
    let_ignore = ensureNonnull(ctx);
};
fn_((io_std_VTbl_unreachableLockErr(P$raw ctx))(void)) {
    let_ignore = ensureNonnull(ctx);
    claim_unreachable;
};
fn_((io_std_VTbl_noUnlockErr(P$raw ctx))(void)) {
    let_ignore = ensureNonnull(ctx);
};
fn_((io_std_VTbl_unreachableUnlockErr(P$raw ctx))(void)) {
    let_ignore = ensureNonnull(ctx);
    claim_unreachable;
};

/*========== Internal Definitions ===========================================*/

fn_((io_std_direct__ensureUTF8Env(void))(void)) {
#if io_pre_ensured_utf8_env_enabled
#if plat_is_windows
    let_ignore = SetConsoleCP(CP_UTF8);
    let_ignore = SetConsoleOutputCP(CP_UTF8);
#endif /* plat_is_windows */
#if io_using_libc
    /* NOLINTNEXTLINE(concurrency-mt-unsafe) */
    let_ignore = setlocale(LC_ALL, ".UTF-8");
#endif /* io_using_libc */
#endif /* io_pre_ensured_utf8_env_enabled */
};
fn_((io_std_direct__in(P$raw ctx))(io_Reader)) {
    let self = ptrAlignCast$((io_std_direct__Ctx*)(ensureNonnull(ctx)));
    return fs_File_reader(self->in_file);
};
fn_((io_std_direct__tryLockIn(P$raw ctx))(bool)) {
    let self = ptrAlignCast$((io_std_direct__Ctx*)(ensureNonnull(ctx)));
    return thrd_Mtx_Recur_tryLock(&self->in_mtx);
};
fn_((io_std_direct__lockIn(P$raw ctx))(void)) {
    let self = ptrAlignCast$((io_std_direct__Ctx*)(ensureNonnull(ctx)));
    thrd_Mtx_Recur_lockProtcd(&self->in_mtx);
};
fn_((io_std_direct__unlockIn(P$raw ctx))(void)) {
    let self = ptrAlignCast$((io_std_direct__Ctx*)(ensureNonnull(ctx)));
    thrd_Mtx_Recur_unlock(&self->in_mtx);
};

fn_((io_std_direct__out(P$raw ctx))(io_Writer)) {
    let self = ptrAlignCast$((io_std_direct__Ctx*)(ensureNonnull(ctx)));
    return fs_File_writer(self->out_file);
};
fn_((io_std_direct__tryLockOut(P$raw ctx))(bool)) {
    let self = ptrAlignCast$((io_std_direct__Ctx*)(ensureNonnull(ctx)));
    return thrd_Mtx_Recur_tryLock(&self->out_mtx);
};
fn_((io_std_direct__lockOut(P$raw ctx))(void)) {
    let self = ptrAlignCast$((io_std_direct__Ctx*)(ensureNonnull(ctx)));
    thrd_Mtx_Recur_lockProtcd(&self->out_mtx);
};
fn_((io_std_direct__unlockOut(P$raw ctx))(void)) {
    let self = ptrAlignCast$((io_std_direct__Ctx*)(ensureNonnull(ctx)));
    thrd_Mtx_Recur_unlock(&self->out_mtx);
};

fn_((io_std_direct__err(P$raw ctx))(io_Writer)) {
    let self = ptrAlignCast$((io_std_direct__Ctx*)(ensureNonnull(ctx)));
    return fs_File_writer(self->err_file);
};
fn_((io_std_direct__tryLockErr(P$raw ctx))(bool)) {
    let self = ptrAlignCast$((io_std_direct__Ctx*)(ensureNonnull(ctx)));
    return thrd_Mtx_Recur_tryLock(&self->err_mtx);
};
fn_((io_std_direct__lockErr(P$raw ctx))(void)) {
    let self = ptrAlignCast$((io_std_direct__Ctx*)(ensureNonnull(ctx)));
    thrd_Mtx_Recur_lockProtcd(&self->err_mtx);
};
fn_((io_std_direct__unlockErr(P$raw ctx))(void)) {
    let self = ptrAlignCast$((io_std_direct__Ctx*)(ensureNonnull(ctx)));
    thrd_Mtx_Recur_unlock(&self->err_mtx);
};

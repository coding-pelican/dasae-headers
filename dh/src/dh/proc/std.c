#include "dh/proc/std.h"
#include "dh/sys/api/windows/proc.h"
#include "dh/sys/posix.h"

$static fn_((proc_std_Direct__ctx(P$raw ctx))(proc_std_Direct*)) {
    return ptrCast$((proc_std_Direct*)(ensureNonnull(ctx)));
};
$static fn_((proc_std_Direct__in(P$raw ctx))(fs_File)) {
    return proc_std_Direct__ctx(ctx)->in;
};
$static fn_((proc_std_Direct__out(P$raw ctx))(fs_File)) {
    return proc_std_Direct__ctx(ctx)->out;
};
$static fn_((proc_std_Direct__err(P$raw ctx))(fs_File)) {
    return proc_std_Direct__ctx(ctx)->err;
};
$static let_(proc_std_Direct__vtbl, proc_std_Self_VTbl) = {
    .inFn = proc_std_Direct__in,
    .outFn = proc_std_Direct__out,
    .errFn = proc_std_Direct__err,
};

fn_((proc_std_in(proc_std_Self self))(fs_File)) {
    self = proc_std_ensureValid(self);
    return self.vtbl->inFn(self.ctx);
};
fn_((proc_std_out(proc_std_Self self))(fs_File)) {
    self = proc_std_ensureValid(self);
    return self.vtbl->outFn(self.ctx);
};
fn_((proc_std_err(proc_std_Self self))(fs_File)) {
    self = proc_std_ensureValid(self);
    return self.vtbl->errFn(self.ctx);
};
fn_((proc_std_Direct_init(fs_File in, fs_File out, fs_File err))(proc_std_Direct)) {
    return (proc_std_Direct){
        .in = in,
        .out = out,
        .err = err,
    };
};
fn_((proc_std_nativeFiles(void))(proc_std_Files)) {
    return (proc_std_Files){
        .in = fs_File_Handle_promote(
            pp_if_(plat_is_windows)(
                pp_then_(GetStdHandle(STD_INPUT_HANDLE)),
                pp_else_(sys_posix_STDIN_FILENO)),
            fs_File_Flags_default
        ),
        .out = fs_File_Handle_promote(
            pp_if_(plat_is_windows)(
                pp_then_(GetStdHandle(STD_OUTPUT_HANDLE)),
                pp_else_(sys_posix_STDOUT_FILENO)),
            fs_File_Flags_default
        ),
        .err = fs_File_Handle_promote(
            pp_if_(plat_is_windows)(
                pp_then_(GetStdHandle(STD_ERROR_HANDLE)),
                pp_else_(sys_posix_STDERR_FILENO)),
            fs_File_Flags_default
        ),
    };
};
fn_((proc_std_Direct_initNative(void))(proc_std_Direct)) {
    let files = proc_std_nativeFiles();
    return proc_std_Direct_init(files.in, files.out, files.err);
};
fn_((proc_std_Direct_self(proc_std_Direct* self))(proc_std_Self)) {
    claim_assert_nonnull(self);
    return proc_std_ensureValid((proc_std_Self){
        .ctx = self,
        .vtbl = &proc_std_Direct__vtbl,
    });
};

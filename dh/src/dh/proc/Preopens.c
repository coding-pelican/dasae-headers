#include "dh/proc/Preopens.h"
#include "dh/mem/common.h"
#include "dh/fs/File/self.h"

/*========== Internal Declarations ==========================================*/

$attr($maybe_unused)
$static fn_((proc_Preopens__unsupported_by(P$raw ctx, S_const$u8 name))(O$proc_Preopens_Resrc));
pp_if_(plat_is_windows)(pp_then_(
    $static fn_((proc_Preopens__windows_by(P$raw ctx, S_const$u8 name))(O$proc_Preopens_Resrc));
));
pp_if_(plat_is_posix)(pp_then_(
    $static fn_((proc_Preopens__posix_by(P$raw ctx, S_const$u8 name))(O$proc_Preopens_Resrc));
));

$static let proc_Preopens_direct__by = pp_if_(plat_is_windows)(
    pp_then_(proc_Preopens__windows_by),
    pp_else_(pp_if_(plat_is_posix)(
        pp_then_(proc_Preopens__posix_by),
        pp_else_(proc_Preopens__unsupported_by)
    )));

/*========== External Definitions ===========================================*/

$static var_(proc_Preopens__ctx_empty, Void) $undefined_static;
let_(proc_Preopens_VTbl_empty, proc_Preopens_VTbl) = {
    .byFn = proc_Preopens_VTbl_emptyBy,
};
let_(proc_Preopens_empty, proc_Preopens) = {
    .ctx = &proc_Preopens__ctx_empty,
    .vtbl = &proc_Preopens_VTbl_empty,
};
fn_((proc_Preopens_direct(void))(proc_Preopens_direct_E$proc_Preopens) $scope) {
    pp_if_(pp_or(plat_is_windows, plat_is_posix))(
        pp_then_({
            $static var_(ctx, Void) $undefined_static;
            $static let_(vtbl, proc_Preopens_VTbl) = {
                .byFn = proc_Preopens_direct__by,
            };
            return_ok(proc_Preopens_ensureValid((proc_Preopens){
                .ctx = &ctx,
                .vtbl = &vtbl,
            }));
        }),
        pp_else_({
            return_err(E_cause$proc_Preopens_direct_Unsupported());
        })
    );
} $unscoped(fn);

fn_((proc_Preopens_by(proc_Preopens self, S_const$u8 name))(O$proc_Preopens_Resrc)) {
    self = proc_Preopens_ensureValid(self);
    return self.vtbl->byFn(self.ctx, name);
};

fn_((proc_Preopens_VTbl_emptyBy(P$raw ctx, S_const$u8 name))(O$proc_Preopens_Resrc) $scope) {
    let_ignore = ensureNonnull(ctx);
    let_ignore = ensureNonnullS(name);
    return_none();
} $unscoped(fn);

/*========== Internal Definitions ===========================================*/

/*--- Unsupported ---*/

fn_((proc_Preopens__unsupported_by(P$raw ctx, S_const$u8 name))(O$proc_Preopens_Resrc)) {
    return proc_Preopens_VTbl_emptyBy(ctx, name);
};

/*--- Windows ---*/

#if plat_is_windows
fn_((proc_Preopens__windows_by(P$raw ctx, S_const$u8 name))(O$proc_Preopens_Resrc) $scope) {
    let_ignore = ensureNonnull(ctx);
    claim_assert_nonnullS(name);
    if (mem_eqlBytes(name, u8_l("stdin"))) return_some(union_of((proc_Preopens_Resrc_file)(fs_File_Handle_promote(
        GetStdHandle(STD_INPUT_HANDLE), fs_File_Flags_default
    ))));
    if (mem_eqlBytes(name, u8_l("stdout"))) return_some(union_of((proc_Preopens_Resrc_file)(fs_File_Handle_promote(
        GetStdHandle(STD_OUTPUT_HANDLE), fs_File_Flags_default
    ))));
    if (mem_eqlBytes(name, u8_l("stderr"))) return_some(union_of((proc_Preopens_Resrc_file)(fs_File_Handle_promote(
        GetStdHandle(STD_ERROR_HANDLE), fs_File_Flags_default
    ))));
    return_none();
} $unscoped(fn);
#endif /* plat_is_windows */

/*--- POSIX ---*/

#if plat_is_posix
fn_((proc_Preopens__posix_by(P$raw ctx, S_const$u8 name))(O$proc_Preopens_Resrc) $scope) {
    let_ignore = ensureNonnull(ctx);
    claim_assert_nonnullS(name);
    if (mem_eqlBytes(name, u8_l("stdin"))) return_some(union_of((proc_Preopens_Resrc_file)(fs_File_Handle_promote(
        sys_posix_STDIN_FILENO, fs_File_Flags_default
    ))));
    if (mem_eqlBytes(name, u8_l("stdout"))) return_some(union_of((proc_Preopens_Resrc_file)(fs_File_Handle_promote(
        sys_posix_STDOUT_FILENO, fs_File_Flags_default
    ))));
    if (mem_eqlBytes(name, u8_l("stderr"))) return_some(union_of((proc_Preopens_Resrc_file)(fs_File_Handle_promote(
        sys_posix_STDERR_FILENO, fs_File_Flags_default
    ))));
    return_none();
} $unscoped(fn);
#endif /* plat_is_posix */

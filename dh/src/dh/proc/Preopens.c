#include "dh/proc/Preopens.h"
#include "dh/mem/common.h"

#if plat_is_windows
#include "dh/sys/api/windows/proc.h"
#endif /* plat_is_windows */
#if plat_is_posix
#include "dh/sys/posix.h"
#endif /* plat_is_posix */

/*========== Internal Declarations ==========================================*/

$static fn_((proc_Preopens__emptyBy(
    P$raw ctx,
    S_const$u8 name
))(O$proc_Preopens_Resrc));
$attr($maybe_unused)
$static fn_((proc_Preopens__unsupportedBy(
    P$raw ctx,
    S_const$u8 name
))(O$proc_Preopens_Resrc));

pp_if_(plat_is_windows)(pp_then_(
    $static fn_((proc_Preopens__windowsFile(
        fs_File_Handle handle
    ))(O$proc_Preopens_Resrc));
    $static fn_((proc_Preopens__windowsBy(
        P$raw ctx,
        S_const$u8 name
    ))(O$proc_Preopens_Resrc));
));

pp_if_(plat_is_posix)(pp_then_(
    $static fn_((proc_Preopens__posixFile(
        fs_File_Handle handle
    ))(O$proc_Preopens_Resrc));
    $static fn_((proc_Preopens__posixBy(
        P$raw ctx,
        S_const$u8 name
    ))(O$proc_Preopens_Resrc));
));

$static let proc_Preopens__directBy = pp_if_(plat_is_windows)(
    pp_then_(proc_Preopens__windowsBy),
    pp_else_(pp_if_(plat_is_posix)(
        pp_then_(proc_Preopens__posixBy),
        pp_else_(proc_Preopens__unsupportedBy)
    )));

$static let_(proc_Preopens__empty_vtbl, proc_Preopens_VTbl) = {
    .byFn = proc_Preopens__emptyBy,
};
$static let_(proc_Preopens__direct_vtbl, proc_Preopens_VTbl) = {
    .byFn = proc_Preopens__directBy,
};

/*========== External Definitions ===========================================*/

$static var_(proc_Preopens__empty_ctx, Void) $undefined_static;
let_(proc_Preopens_empty, proc_Preopens) = {
    .ctx = &proc_Preopens__empty_ctx,
    .vtbl = &proc_Preopens__empty_vtbl,
};

fn_((proc_Preopens_direct(void))(
    proc_Preopens_direct_E$proc_Preopens
) $scope) {
    pp_if_(pp_or(plat_is_windows, plat_is_posix))(
        pp_then_(
            $static var_(ctx, Void) $undefined_static;
            return_ok(proc_Preopens_ensureValid((proc_Preopens){
                .ctx = &ctx,
                .vtbl = &proc_Preopens__direct_vtbl,
            }))
        ),
        pp_else_(
            return_err(E_cause$proc_Preopens_direct_Unsupported())
        ));
} $unscoped(fn);

fn_((proc_Preopens_by(
    proc_Preopens self,
    S_const$u8 name
))(O$proc_Preopens_Resrc)) {
    self = proc_Preopens_ensureValid(self);
    return self.vtbl->byFn(self.ctx, name);
};

/*========== Internal Definitions ===========================================*/

/*--- Common ---*/

fn_((proc_Preopens__emptyBy(
    P$raw ctx,
    S_const$u8 name
))(O$proc_Preopens_Resrc)) {
    claim_assert_nonnull(ctx);
    let_ignore = name;
    return none$((O$proc_Preopens_Resrc));
};

/*--- Unsupported ---*/

fn_((proc_Preopens__unsupportedBy(
    P$raw ctx,
    S_const$u8 name
))(O$proc_Preopens_Resrc)) {
    claim_assert_nonnull(ctx);
    let_ignore = name;
    return none$((O$proc_Preopens_Resrc));
};

/*--- Windows ---*/

#if plat_is_windows
fn_((proc_Preopens__windowsFile(
    fs_File_Handle handle
))(O$proc_Preopens_Resrc)) {
    if (handle == null || handle == INVALID_HANDLE_VALUE) {
        return none$((O$proc_Preopens_Resrc));
    }
    return some$((O$proc_Preopens_Resrc)(union_of$((
        proc_Preopens_Resrc
    )(proc_Preopens_Resrc_file)(fs_File_Handle_promote(
        handle,
        fs_File_Flags_default
    )))));
};

fn_((proc_Preopens__windowsBy(
    P$raw ctx,
    S_const$u8 name
))(O$proc_Preopens_Resrc)) {
    claim_assert_nonnull(ctx);
    if (mem_eqlBytes(name, u8_l("stdin"))) {
        return proc_Preopens__windowsFile(GetStdHandle(STD_INPUT_HANDLE));
    }
    if (mem_eqlBytes(name, u8_l("stdout"))) {
        return proc_Preopens__windowsFile(GetStdHandle(STD_OUTPUT_HANDLE));
    }
    if (mem_eqlBytes(name, u8_l("stderr"))) {
        return proc_Preopens__windowsFile(GetStdHandle(STD_ERROR_HANDLE));
    }
    return none$((O$proc_Preopens_Resrc));
};
#endif /* plat_is_windows */

/*--- POSIX ---*/

#if plat_is_posix
fn_((proc_Preopens__posixFile(
    fs_File_Handle handle
))(O$proc_Preopens_Resrc)) {
    return some$((O$proc_Preopens_Resrc)(union_of$((
        proc_Preopens_Resrc
    )(proc_Preopens_Resrc_file)(fs_File_Handle_promote(
        handle,
        fs_File_Flags_default
    )))));
};

fn_((proc_Preopens__posixBy(
    P$raw ctx,
    S_const$u8 name
))(O$proc_Preopens_Resrc)) {
    claim_assert_nonnull(ctx);
    if (mem_eqlBytes(name, u8_l("stdin"))) {
        return proc_Preopens__posixFile(sys_posix_STDIN_FILENO);
    }
    if (mem_eqlBytes(name, u8_l("stdout"))) {
        return proc_Preopens__posixFile(sys_posix_STDOUT_FILENO);
    }
    if (mem_eqlBytes(name, u8_l("stderr"))) {
        return proc_Preopens__posixFile(sys_posix_STDERR_FILENO);
    }
    return none$((O$proc_Preopens_Resrc));
};
#endif /* plat_is_posix */

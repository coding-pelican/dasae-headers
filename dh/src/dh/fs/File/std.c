#include "dh/fs/File/std.h"

/*========== Internal Declarations ==========================================*/

$attr($maybe_unused)
$static fn_((fs_File_std_direct__unsupported_in(P$raw ctx))(fs_File));
$attr($maybe_unused)
$static fn_((fs_File_std_direct__unsupported_out(P$raw ctx))(fs_File));
$attr($maybe_unused)
$static fn_((fs_File_std_direct__unsupported_err(P$raw ctx))(fs_File));

pp_if_(pp_or(plat_is_windows, plat_is_posix))(pp_then_(
    $static fn_((fs_File_std_direct__supported_in(P$raw ctx))(fs_File));
    $static fn_((fs_File_std_direct__supported_out(P$raw ctx))(fs_File));
    $static fn_((fs_File_std_direct__supported_err(P$raw ctx))(fs_File));
));

$static let fs_File_std_direct__in = pp_if_(pp_or(plat_is_windows, plat_is_posix))(
    pp_then_(fs_File_std_direct__supported_in),
    pp_else_(fs_File_std_direct__unsupported_in));
$static let fs_File_std_direct__out = pp_if_(pp_or(plat_is_windows, plat_is_posix))(
    pp_then_(fs_File_std_direct__supported_out),
    pp_else_(fs_File_std_direct__unsupported_out));
$static let fs_File_std_direct__err = pp_if_(pp_or(plat_is_windows, plat_is_posix))(
    pp_then_(fs_File_std_direct__supported_err),
    pp_else_(fs_File_std_direct__unsupported_err));

/*========== External Definitions ===========================================*/

$static var_(fs_File_std__ctx_failing, Void) $undefined_static;
let_(fs_File_std_VTbl_failing, fs_File_std_Self_VTbl) = {
    .inFn = fs_File_std_VTbl_unreachableIn,
    .outFn = fs_File_std_VTbl_unreachableOut,
    .errFn = fs_File_std_VTbl_unreachableErr,
};
let_(fs_File_std_failing, fs_File_std_Self) = {
    .ctx = &fs_File_std__ctx_failing,
    .vtbl = &fs_File_std_VTbl_failing,
};
fn_((fs_File_std_direct(void))(fs_File_std_direct_E$fs_File_std_Self $scope)) {
    pp_if_(pp_or(plat_is_windows, plat_is_posix))(
        pp_then_({
            $static var_(ctx, Void) $undefined_static;
            $static let_(vtbl, fs_File_std_Self_VTbl) = {
                .inFn = fs_File_std_direct__in,
                .outFn = fs_File_std_direct__out,
                .errFn = fs_File_std_direct__err,
            };
            return_ok(fs_File_std_ensureValid((fs_File_std_Self){
                .ctx = &ctx,
                .vtbl = &vtbl,
            }));
        }),
        pp_else_({
            return_err(E_cause$fs_File_std_direct_Unsupported());
        })
    );
} $unscoped(fn);

fn_((fs_File_std_in(fs_File_std_Self self))(fs_File)) {
    self = fs_File_std_ensureValid(self);
    return self.vtbl->inFn(self.ctx);
};
fn_((fs_File_std_out(fs_File_std_Self self))(fs_File)) {
    self = fs_File_std_ensureValid(self);
    return self.vtbl->outFn(self.ctx);
};
fn_((fs_File_std_err(fs_File_std_Self self))(fs_File)) {
    self = fs_File_std_ensureValid(self);
    return self.vtbl->errFn(self.ctx);
};

fn_((fs_File_std_VTbl_unreachableIn(P$raw ctx))(fs_File)) {
    let_ignore = ensureNonnull(ctx);
    claim_unreachable;
};
fn_((fs_File_std_VTbl_unreachableOut(P$raw ctx))(fs_File)) {
    let_ignore = ensureNonnull(ctx);
    claim_unreachable;
};
fn_((fs_File_std_VTbl_unreachableErr(P$raw ctx))(fs_File)) {
    let_ignore = ensureNonnull(ctx);
    claim_unreachable;
};

/*========== Internal Definitions ===========================================*/

fn_((fs_File_std_direct__unsupported_in(P$raw ctx))(fs_File)) {
    let_ignore = ensureNonnull(ctx);
    claim_unreachable_msg(nameOf(fs_File_std_in) " is not supported on this platform: " plat_name);
};
fn_((fs_File_std_direct__unsupported_out(P$raw ctx))(fs_File)) {
    let_ignore = ensureNonnull(ctx);
    claim_unreachable_msg(nameOf(fs_File_std_out) " is not supported on this platform: " plat_name);
};
fn_((fs_File_std_direct__unsupported_err(P$raw ctx))(fs_File)) {
    let_ignore = ensureNonnull(ctx);
    claim_unreachable_msg(nameOf(fs_File_std_err) " is not supported on this platform: " plat_name);
};

pp_if_(pp_or(plat_is_windows, plat_is_posix))(pp_then_(
    fn_((fs_File_std_direct__supported_in(P$raw ctx))(fs_File)) {
        let_ignore = ensureNonnull(ctx);
        return fs_File_Handle_promote(
            pp_if_(plat_is_windows)(
                pp_then_(GetStdHandle(STD_INPUT_HANDLE)),
                pp_else_(sys_posix_STDIN_FILENO)
            ),
            fs_File_Flags_default
        );
    };
    fn_((fs_File_std_direct__supported_out(P$raw ctx))(fs_File)) {
        let_ignore = ensureNonnull(ctx);
        return fs_File_Handle_promote(
            pp_if_(plat_is_windows)(
                pp_then_(GetStdHandle(STD_OUTPUT_HANDLE)),
                pp_else_(sys_posix_STDOUT_FILENO)
            ),
            fs_File_Flags_default
        );
    };
    fn_((fs_File_std_direct__supported_err(P$raw ctx))(fs_File)) {
        let_ignore = ensureNonnull(ctx);
        return fs_File_Handle_promote(
            pp_if_(plat_is_windows)(
                pp_then_(GetStdHandle(STD_ERROR_HANDLE)),
                pp_else_(sys_posix_STDERR_FILENO)
            ),
            fs_File_Flags_default
        );
    };
));

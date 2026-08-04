#include "dh-main.h"
#include "dh/fs/File/std.h"
#include "dh/sys/posix.h"

T_alias$((test_fs_File_std_Ctx)(struct test_fs_File_std_Ctx {
    var_(in_calls, usize);
    var_(out_calls, usize);
    var_(err_calls, usize);
}));

$static fn_((test_fs_File_std__in(P$raw ctx))(fs_File)) {
    let self = ptrCast$((test_fs_File_std_Ctx*)(ensureNonnull(ctx)));
    ++self->in_calls;
    return l0$((fs_File));
};

$static fn_((test_fs_File_std__out(P$raw ctx))(fs_File)) {
    let self = ptrCast$((test_fs_File_std_Ctx*)(ensureNonnull(ctx)));
    ++self->out_calls;
    return l0$((fs_File));
};

$static fn_((test_fs_File_std__err(P$raw ctx))(fs_File)) {
    let self = ptrCast$((test_fs_File_std_Ctx*)(ensureNonnull(ctx)));
    ++self->err_calls;
    return l0$((fs_File));
};

TEST_fn_("fs/File/std: injected interface dispatches every standard file" $scope) {
    var_(ctx, test_fs_File_std_Ctx) = cleared();
    let_(vtbl, fs_File_std_Self_VTbl) = {
        .inFn = test_fs_File_std__in,
        .outFn = test_fs_File_std__out,
        .errFn = test_fs_File_std__err,
    };
    let std = fs_File_std_ensureValid((fs_File_std_Self){
        .ctx = &ctx,
        .vtbl = &vtbl,
    });

    let_ignore = fs_File_std_in(std);
    let_ignore = fs_File_std_out(std);
    let_ignore = fs_File_std_err(std);
    try_(TEST_expect(ctx.in_calls == 1));
    try_(TEST_expect(ctx.out_calls == 1));
    try_(TEST_expect(ctx.err_calls == 1));
} $unscoped(TEST_fn);

TEST_fn_("fs/File/std: failing standard streams retain a complete interface" $scope) {
    try_(TEST_expect(fs_File_std_isValid(fs_File_std_failing)));
} $unscoped(TEST_fn);

TEST_fn_("fs/File/std: direct capability preserves native standard handles" $scope) {
    let std = catch_((fs_File_std_direct())(err, {
        try_(TEST_expect(E_tag(err.as_any) == E_Tag$fs_File_std_direct_Unsupported));
        return_ok_void();
    }));
    try_(TEST_expect(
        fs_File_handle(fs_File_std_in(std))
        == pp_if_(plat_is_windows)(
            pp_then_(GetStdHandle(STD_INPUT_HANDLE)),
            pp_else_(sys_posix_STDIN_FILENO))
    ));
    try_(TEST_expect(
        fs_File_handle(fs_File_std_out(std))
        == pp_if_(plat_is_windows)(
            pp_then_(GetStdHandle(STD_OUTPUT_HANDLE)),
            pp_else_(sys_posix_STDOUT_FILENO))
    ));
    try_(TEST_expect(
        fs_File_handle(fs_File_std_err(std))
        == pp_if_(plat_is_windows)(
            pp_then_(GetStdHandle(STD_ERROR_HANDLE)),
            pp_else_(sys_posix_STDERR_FILENO))
    ));
} $unscoped(TEST_fn);

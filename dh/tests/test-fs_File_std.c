#include "dh-main.h"
#include "dh/fs/File/std.h"
#include "dh/sys/posix.h"

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

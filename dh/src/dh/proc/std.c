#include "dh/proc/std.h"
#include "dh/sys/api/windows/proc.h"
#include "dh/sys/posix.h"

fn_((proc_std_in(void))(fs_File)) {
    return fs_File_Handle_promote(
        pp_if_(plat_is_windows)(
            pp_then_(GetStdHandle(STD_INPUT_HANDLE)),
            pp_else_(sys_posix_STDIN_FILENO)),
        fs_File_Flags_default
    );
};
fn_((proc_std_out(void))(fs_File)) {
    return fs_File_Handle_promote(
        pp_if_(plat_is_windows)(
            pp_then_(GetStdHandle(STD_OUTPUT_HANDLE)),
            pp_else_(sys_posix_STDOUT_FILENO)),
        fs_File_Flags_default
    );
};
fn_((proc_std_err(void))(fs_File)) {
    return fs_File_Handle_promote(
        pp_if_(plat_is_windows)(
            pp_then_(GetStdHandle(STD_ERROR_HANDLE)),
            pp_else_(sys_posix_STDERR_FILENO)),
        fs_File_Flags_default
    );
};

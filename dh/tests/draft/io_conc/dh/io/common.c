#include "common.h"
#include "../fs/common.h"

#include "dh/posix.h"
#if plat_is_windows
#include "dh/os/windows/proc.h"
#endif /* plat_is_windows */

$attr($inline_always)
$static fn_((io__getStdInHandle(void))(posix_fd_t)) {
    return pp_if_(plat_is_windows)(
        pp_then_(GetStdHandle(STD_INPUT_HANDLE)),
        pp_else_(posix_STDIN_FILENO));
};

$attr($inline_always)
$static fn_((io__getStdOutHandle(void))(posix_fd_t)) {
    return pp_if_(plat_is_windows)(
        pp_then_(GetStdHandle(STD_OUTPUT_HANDLE)),
        pp_else_(posix_STDOUT_FILENO));
};

$attr($inline_always)
$static fn_((io__getStdErrHandle(void))(posix_fd_t)) {
    return pp_if_(plat_is_windows)(
        pp_then_(GetStdHandle(STD_ERROR_HANDLE)),
        pp_else_(posix_STDERR_FILENO));
};

fn_((io_getStdIn(void))(fs_File)) {
    return fs_File_Handle_promote(io__getStdInHandle(), fs_File_Flags_default);
};

fn_((io_getStdOut(void))(fs_File)) {
    return fs_File_Handle_promote(io__getStdOutHandle(), fs_File_Flags_default);
};

fn_((io_getStdErr(void))(fs_File)) {
    return fs_File_Handle_promote(io__getStdErrHandle(), fs_File_Flags_default);
};

#include "dh/io/common.h"
#include "dh/fs/common.h"

#include "dh/posix.h"
#if plat_is_windows
#include "dh/os/windows.h"
#endif /* plat_is_windows */

$inline_always
$static fn_((io_getStdInHandle(void))(posix_fd_t)) {
    return pp_if_(plat_is_windows)(
        pp_then_(GetStdHandle(STD_INPUT_HANDLE)),
        pp_else_(posix_STDIN_FILENO)
    );
}

$inline_always
$static fn_((io_getStdOutHandle(void))(posix_fd_t)) {
    return pp_if_(plat_is_windows)(
        pp_then_(GetStdHandle(STD_OUTPUT_HANDLE)),
        pp_else_(posix_STDOUT_FILENO)
    );
}

$inline_always
$static fn_((io_getStdErrHandle(void))(posix_fd_t)) {
    return pp_if_(plat_is_windows)(
        pp_then_(GetStdHandle(STD_ERROR_HANDLE)),
        pp_else_(posix_STDERR_FILENO)
    );
}

fn_((io_getStdIn(void))(fs_File)) {
    return (fs_File){ .handle = io_getStdInHandle() };
}

fn_((io_getStdOut(void))(fs_File)) {
    return (fs_File){ .handle = io_getStdOutHandle() };
}

fn_((io_getStdErr(void))(fs_File)) {
    return (fs_File){ .handle = io_getStdErrHandle() };
}

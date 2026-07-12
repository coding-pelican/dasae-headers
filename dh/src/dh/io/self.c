#include "dh/io/self.h"
#include "dh/fs/File/self.h"
#include "dh/thrd/Mtx.h"

#include "dh/sys/posix.h"
#if plat_is_windows
#include "dh/sys/api/windows/proc.h"
#include "dh/sys/api/windows/console.h"
#endif /* plat_is_windows */
#if io_stream_using_libc
#include <locale.h>
#endif /* io_stream_using_libc */

#if io_locked_std_enabled
$static var_(io__s_in_mtx, thrd_Mtx_Recur) = cleared();
$static var_(io__s_out_mtx, thrd_Mtx_Recur) = cleared();
$static var_(io__s_err_mtx, thrd_Mtx_Recur) = cleared();
#endif /* io_locked_std_enabled */

$attr($on_load)
$static fn_((io__init(void))(void)) {
#if io_locked_std_enabled
    io__s_in_mtx = thrd_Mtx_Recur_init();
    io__s_out_mtx = thrd_Mtx_Recur_init();
    io__s_err_mtx = thrd_Mtx_Recur_init();
#endif /* io_locked_std_enabled */
#if io_pre_ensured_utf8_env_enabled
#if plat_is_windows
    // [Console]::OutputEncoding = [System.Text.Encoding]::UTF8
    // chcp 65001
    let_ignore = SetConsoleCP(CP_UTF8);
    let_ignore = SetConsoleOutputCP(CP_UTF8);
#endif /* plat_is_windows */
#if io_stream_using_libc
    /* NOLINTNEXTLINE(concurrency-mt-unsafe) */
    let_ignore = setlocale(LC_ALL, ".UTF-8"); /* Code page 65001 */
#endif /* io_stream_using_libc */
#endif /* io_pre_ensured_utf8_env_enabled */
};

$attr($on_exit)
$static fn_((io__fini(void))(void)) {
#if io_locked_std_enabled
    thrd_Mtx_Recur_fini(&io__s_err_mtx);
    thrd_Mtx_Recur_fini(&io__s_out_mtx);
    thrd_Mtx_Recur_fini(&io__s_in_mtx);
#endif /* io_locked_std_enabled */
};

fn_((io_handleStdIn(void))(fs_File)) {
    return fs_File_Handle_promote(
        pp_if_(plat_is_windows)(
            pp_then_(GetStdHandle(STD_INPUT_HANDLE)),
            pp_else_(sys_posix_STDIN_FILENO)),
        fs_File_Flags_default
    );
};

fn_((io_lockStdIn(void))(void)) {
#if io_locked_std_enabled
    thrd_Mtx_Recur_lockProtcd(&io__s_in_mtx);
#endif /* io_locked_std_enabled */
};

fn_((io_tryLockStdIn(void))(bool)) {
#if io_locked_std_enabled
    return thrd_Mtx_Recur_tryLock(&io__s_in_mtx);
#endif /* io_locked_std_enabled */
};

fn_((io_unlockStdIn(void))(void)) {
#if io_locked_std_enabled
    thrd_Mtx_Recur_unlock(&io__s_in_mtx);
#endif /* io_locked_std_enabled */
};

fn_((io_handleStdOut(void))(fs_File)) {
    return fs_File_Handle_promote(
        pp_if_(plat_is_windows)(
            pp_then_(GetStdHandle(STD_OUTPUT_HANDLE)),
            pp_else_(sys_posix_STDOUT_FILENO)),
        fs_File_Flags_default
    );
};

fn_((io_lockStdOut(void))(void)) {
#if io_locked_std_enabled
    thrd_Mtx_Recur_lockProtcd(&io__s_out_mtx);
#endif /* io_locked_std_enabled */
};

fn_((io_tryLockStdOut(void))(bool)) {
#if io_locked_std_enabled
    return thrd_Mtx_Recur_tryLock(&io__s_out_mtx);
#endif /* io_locked_std_enabled */
};

fn_((io_unlockStdOut(void))(void)) {
#if io_locked_std_enabled
    thrd_Mtx_Recur_unlock(&io__s_out_mtx);
#endif /* io_locked_std_enabled */
};

fn_((io_handleStdErr(void))(fs_File)) {
    return fs_File_Handle_promote(
        pp_if_(plat_is_windows)(
            pp_then_(GetStdHandle(STD_ERROR_HANDLE)),
            pp_else_(sys_posix_STDERR_FILENO)),
        fs_File_Flags_default
    );
};

fn_((io_lockStdErr(void))(void)) {
#if io_locked_std_enabled
    thrd_Mtx_Recur_lockProtcd(&io__s_err_mtx);
#endif /* io_locked_std_enabled */
};

fn_((io_tryLockStdErr(void))(bool)) {
#if io_locked_std_enabled
    return thrd_Mtx_Recur_tryLock(&io__s_err_mtx);
#endif /* io_locked_std_enabled */
};

fn_((io_unlockStdErr(void))(void)) {
#if io_locked_std_enabled
    thrd_Mtx_Recur_unlock(&io__s_err_mtx);
#endif /* io_locked_std_enabled */
};

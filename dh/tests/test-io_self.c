/**
 * @copyright Copyright (c) 2025 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    test-io_common.c
 * @author  Gyeongtae Kim(dev-dasae)
 * @date    2025-06-22 (date of creation)
 * @updated 2025-06-22 (date of last update)
 * @version v0.1-alpha
 * @ingroup dasae-headers(dh)/tests
 * @prefix  test
 *
 * @brief   Unit tests for common IO functions.
 */

/*========== Includes =======================================================*/

#include "dh-main.h"
#include "dh/io/Self.h"
#include "dh/fs/File.h"
#include "dh/proc/std.h"

#if plat_is_windows
#include "dh/sys/api/windows.h"
#else
#include "dh/sys/posix.h"
#endif

/*========== Tests ==========================================================*/

TEST_fn_("proc/std: Check `proc_std_in` is valid handle" $guard) {
    var direct = proc_std_Direct_initNative();
    let std_in = proc_std_in(proc_std_Direct_self(&direct));
    let handle = pp_if_(plat_is_windows)(
        pp_then_(GetStdHandle(STD_INPUT_HANDLE)),
        pp_else_(sys_posix_STDIN_FILENO));
    try_(TEST_expect(std_in.handle == handle));
} $unguarded(TEST_fn);

TEST_fn_("proc/std: Check `proc_std_out` is valid handle" $guard) {
    var direct = proc_std_Direct_initNative();
    let std_out = proc_std_out(proc_std_Direct_self(&direct));
    let handle = pp_if_(plat_is_windows)(
        pp_then_(GetStdHandle(STD_OUTPUT_HANDLE)),
        pp_else_(sys_posix_STDOUT_FILENO));
    try_(TEST_expect(std_out.handle == handle));
} $unguarded(TEST_fn);

TEST_fn_("proc/std: Check `proc_std_err` is valid handle" $guard) {
    var direct = proc_std_Direct_initNative();
    let std_err = proc_std_err(proc_std_Direct_self(&direct));
    let handle = pp_if_(plat_is_windows)(
        pp_then_(GetStdHandle(STD_ERROR_HANDLE)),
        pp_else_(sys_posix_STDERR_FILENO));
    try_(TEST_expect(std_err.handle == handle));
} $unguarded(TEST_fn);

TEST_fn_("io: explicit provider coordinates standard output" $guard) {
    var direct = io_Direct_init();
    defer_(io_Direct_fini(&direct));
    let io = io_Direct_self(&direct);

    io_lockStdOut(io);
    defer_(io_unlockStdOut(io));
    try_(TEST_expect(io_tryLockStdOut(io)));
    io_unlockStdOut(io);
} $unguarded(TEST_fn);

TEST_fn_("io: noop provider accepts standard I/O coordination" $guard) {
    io_lockStdIn(io_Self_noop);
    io_unlockStdIn(io_Self_noop);
    io_lockStdOut(io_Self_noop);
    io_unlockStdOut(io_Self_noop);
    io_lockStdErr(io_Self_noop);
    io_unlockStdErr(io_Self_noop);
    try_(TEST_expect(io_tryLockStdIn(io_Self_noop)));
    try_(TEST_expect(io_tryLockStdOut(io_Self_noop)));
    try_(TEST_expect(io_tryLockStdErr(io_Self_noop)));
} $unguarded(TEST_fn);

TEST_fn_("io: failing provider rejects non-blocking coordination" $guard) {
    try_(TEST_expect(!io_tryLockStdIn(io_Self_failing)));
    try_(TEST_expect(!io_tryLockStdOut(io_Self_failing)));
    try_(TEST_expect(!io_tryLockStdErr(io_Self_failing)));
} $unguarded(TEST_fn);

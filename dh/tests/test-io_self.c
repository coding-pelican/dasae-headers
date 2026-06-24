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
#include "dh/io/self.h"
#include "dh/fs/File.h"

#if plat_is_windows
#include "dh/sys/api/windows.h"
#else
#include "dh/sys/posix.h"
#endif

/*========== Tests ==========================================================*/

TEST_fn_("io/self: Check `io_handleStdIn` is valid handle" $scope) {
    let std_in = io_handleStdIn();
    let handle = pp_if_(plat_is_windows)(
        pp_then_(GetStdHandle(STD_INPUT_HANDLE)),
        pp_else_(sys_posix_STDIN_FILENO));
    try_(TEST_expect(std_in.handle == handle));
} $unscoped(TEST_fn);

TEST_fn_("io/self: Check `io_handleStdOut` is valid handle" $scope) {
    let std_out = io_handleStdOut();
    let handle = pp_if_(plat_is_windows)(
        pp_then_(GetStdHandle(STD_OUTPUT_HANDLE)),
        pp_else_(sys_posix_STDOUT_FILENO));
    try_(TEST_expect(std_out.handle == handle));
} $unscoped(TEST_fn);

TEST_fn_("io/self: Check `io_handleStdErr` is valid handle" $scope) {
    let std_err = io_handleStdErr();
    let handle = pp_if_(plat_is_windows)(
        pp_then_(GetStdHandle(STD_ERROR_HANDLE)),
        pp_else_(sys_posix_STDERR_FILENO));
    try_(TEST_expect(std_err.handle == handle));
} $unscoped(TEST_fn);

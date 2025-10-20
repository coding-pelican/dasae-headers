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

#include "dh/main.h"
#include "dh/io/common.h"
#include "dh/fs/File.h"

#if bti_plat_windows
#include "dh/os/windows.h"
#else
#include "dh/posix.h"
#endif

/*========== Tests ==========================================================*/

TEST_fn_("io_common: Check `io_getStdIn` is valid handle" $scope) {
    let std_in = io_getStdIn();
    let handle = pp_if_(bti_plat_windows)(
        pp_than_(GetStdHandle(STD_INPUT_HANDLE)),
        pp_else_(posix_STDIN_FILENO)
    );
    try_(TEST_expect(std_in.handle == handle));
} $unscoped_(TEST_fn);

TEST_fn_("io_common: Check `io_getStdOut` is valid handle" $scope) {
    let std_out = io_getStdOut();
    let handle  = pp_if_(bti_plat_windows)(
        pp_than_(GetStdHandle(STD_OUTPUT_HANDLE)),
        pp_else_(posix_STDOUT_FILENO)
    );
    try_(TEST_expect(std_out.handle == handle));
} $unscoped_(TEST_fn);

TEST_fn_("io_common: Check `io_getStdErr` is valid handle" $scope) {
    let std_err = io_getStdErr();
    let handle  = pp_if_(bti_plat_windows)(
        pp_than_(GetStdHandle(STD_ERROR_HANDLE)),
        pp_else_(posix_STDERR_FILENO)
    );
    try_(TEST_expect(std_err.handle == handle));
} $unscoped_(TEST_fn);

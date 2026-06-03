/**
 * @copyright Copyright (c) 2024-2026 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    dh-TEST-main.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2026-05-05 (date of creation)
 * @updated 2026-05-27 (date of last update)
 * @ingroup dasae-headers(dh)
 * @prefix  TEST
 *
 * @brief   Aggregate test runner entry surface
 * @details Emits the root test `main` only when `TEST_main_enabled` is defined.
 */
#ifndef dh_TEST_main__included
#define dh_TEST_main__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "dh/prl.h"
#include "dh/TEST.h"
#include "dh/start.h"

/*========== Macros and Declarations ========================================*/

/*---------- Configuration Flags --------------------------------------------*/

#if !defined(TEST_main_enabled)
#define TEST_main_enabled __comp_bool__TEST_main_enabled
#endif /* !defined(TEST_main_enabled) */
#define TEST_main__enabled_default __comp_flag__TEST_main__enabled_default

/*========== Macros and Definitions =========================================*/

/*---------- Configuration Flags --------------------------------------------*/

#define __comp_bool__TEST_main_enabled TEST_main__enabled_default
#define __comp_flag__TEST_main__enabled_default pp_false

/*---------- Root main ------------------------------------------------------*/

#if TEST_main_enabled
$static fn_((TEST__runMain(void))(start_ExitCode)) {
    debug_StackTrace_setupCrashHandler();
    TEST_Framework_run();
    return 0;
};

#ifndef main_root_included
#define main_root_included 1

#if comp_start_files_linked
fn_((main(void))(int)) {
    return TEST__runMain();
};

#else /* !comp_start_files_linked */
$attr($no_return $maybe_unused)
$static fn_((TEST__callMainAndExit(P$raw raw_ctx))(void)) {
    let_ignore = raw_ctx;
    start_callInitArray();
    let code = TEST__runMain();
    start_callFiniArray();
    start_exit(code);
};

start_emitEntry(TEST__callMainAndExit);
#endif /* comp_start_files_linked || !comp_start_files_linked */

#endif /* main_root_included */
#endif /* TEST_main_enabled */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* dh_TEST_main__included */

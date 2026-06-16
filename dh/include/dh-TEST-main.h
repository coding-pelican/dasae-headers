/**
 * @copyright Copyright (c) 2024-2026 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    dh-TEST-main.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2026-05-05 (date of creation)
 * @updated 2026-06-16 (date of last update)
 * @ingroup dasae-headers(dh)
 * @prefix  (none)
 *
 * @brief   Aggregate test runner entry surface
 * @details Emits the root test `main` only when `TEST_main_enabled` is defined.
 */
#pragma once
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
#define __comp_flag__TEST_main__enabled_default TEST_enabled

/*---------- Root main ------------------------------------------------------*/

#if on_comptime
#if TEST_main_enabled
$attr($inline)
$static fn_((TEST_main__runTESTMain(void))(start_ExitCode)) {
    debug_StackTrace_setupCrashHandler();
    TEST_Framework_run();
    return 0;
};

#ifndef main_root_included
#define main_root_included 1

#if comp_start_files_linked
fn_((main(void))(int)) {
    return TEST_main__runTESTMain();
};

#else /* !comp_start_files_linked */
$attr($maybe_unused $no_return)
$static fn_((TEST_main__callTESTMainAndExit(P$raw raw_ctx))(void)) {
    let_ignore = raw_ctx;
    start_callInitArray();
    let code = TEST_main__runTESTMain();
    start_callFiniArray();
    start_exit(code);
};

start_emitEntry(TEST_main__callTESTMainAndExit);
#endif /* comp_start_files_linked || !comp_start_files_linked */

#endif /* main_root_included */
#endif /* TEST_main_enabled */
#endif /* on_comptime */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* dh_TEST_main__included */

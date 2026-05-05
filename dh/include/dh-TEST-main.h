/**
 * @copyright Copyright (c) 2024-2026 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    dh-TEST-main.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2026-05-05 (date of creation)
 * @updated 2026-05-05 (date of last update)
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

/*========== Macros =========================================================*/

#if !defined(TEST_main_enabled)
#define TEST_main_enabled 0
#endif /* !defined(TEST_main_enabled) */

/*========== Root main ======================================================*/

#if TEST_main_enabled
fn_((main(void))(int)) {
    return TEST_Framework_run(), 0;
};
#endif /* TEST_main_enabled */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* dh_TEST_main__included */

/**
 * @copyright Copyright (c) 2025-2026 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    self.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2025-08-09 (date of creation)
 * @updated 2026-06-24 (date of last update)
 * @ingroup dal-project/da/io
 * @prefix  io
 */
#pragma once
#ifndef io_self__included
#define io_self__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "common.h"

/*========== Macros and Declarations ========================================*/

#define io_using_libc __comp_bool__io_using_libc

$extern fn_((io_lockStdIn(void))(void));
$extern fn_((io_tryLockStdIn(void))(bool));
$extern fn_((io_unlockStdIn(void))(void));

$extern fn_((io_lockStdOut(void))(void));
$extern fn_((io_tryLockStdOut(void))(bool));
$extern fn_((io_unlockStdOut(void))(void));

$extern fn_((io_lockStdErr(void))(void));
$extern fn_((io_tryLockStdErr(void))(bool));
$extern fn_((io_unlockStdErr(void))(void));

/*========== Macros and Definitions =========================================*/

#define __comp_bool__io_using_libc env_libc_linked

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* io_self__included */

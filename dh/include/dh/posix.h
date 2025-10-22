/**
 * @copyright Copyright (c) 2025 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    posix.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2025-06-22 (date of creation)
 * @updated 2025-06-22 (date of last update)
 * @version v0.1-alpha
 * @ingroup dasae-headers(dh)
 * @prefix  posix
 *
 * @brief   POSIX constants
 * @details Provides POSIX constants for standard file descriptors.
 */

#ifndef POSIX_INCLUDED
#define POSIX_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "dh/core.h"

#if bti_plat_windows
#include "dh/os/windows.h"
#endif /* bti_plat_windows */

/*========== Macros and Declarations ========================================*/

typedef pp_if_(bti_plat_windows)(
    pp_than_(HANDLE),
    pp_else_(i32)
) posix_fd_t;
typedef i32 posix_mode_t;

#define posix_STDIN_FILENO  comp_const__posix_STDIN_FILENO
#define posix_STDOUT_FILENO comp_const__posix_STDOUT_FILENO
#define posix_STDERR_FILENO comp_const__posix_STDERR_FILENO

/*========== Macros and Definitions =========================================*/

#define comp_const__posix_STDIN_FILENO  as$((posix_fd_t)(0))
#define comp_const__posix_STDOUT_FILENO as$((posix_fd_t)(1))
#define comp_const__posix_STDERR_FILENO as$((posix_fd_t)(2))

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* POSIX_INCLUDED */

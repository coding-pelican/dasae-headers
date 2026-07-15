/**
 * @copyright Copyright (c) 2026 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    common.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2026-06-26 (date of creation)
 * @ingroup dasae-headers(dh)/sys/libc/darwin
 * @prefix  sys_libc_darwin
 */
#pragma once
#ifndef sys_libc_darwin_common__included
#define sys_libc_darwin_common__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "cfg.h"
#include "dh/prl.h"

/*========== Macros and Declarations ========================================*/

#if in_analysis || plat_is_darwin
typedef i32 sys_libc_darwin_fd_t;
typedef u16 sys_libc_darwin_mode_t;
typedef i32 sys_libc_darwin_signal_t;
typedef usize sys_libc_darwin_ioctl_req_t;
#endif /* in_analysis || plat_is_darwin */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* sys_libc_darwin_common__included */

/**
 * @copyright Copyright (c) 2026 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    mem.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2026-06-26 (date of creation)
 * @ingroup dasae-headers(dh)/sys/libc/darwin
 * @prefix  sys_libc_darwin
 */
#pragma once
#ifndef sys_libc_darwin_mem__included
#define sys_libc_darwin_mem__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "cfg.h"
#include "dh/prl.h"

/*========== Macros and Declarations ========================================*/

#if in_analysis || plat_is_darwin
#define sys_libc_darwin_PROT_NONE 0
#define sys_libc_darwin_PROT_READ 0x01
#define sys_libc_darwin_PROT_WRITE 0x02
#define sys_libc_darwin_MAP_PRIVATE 0x0002
#define sys_libc_darwin_MAP_ANONYMOUS 0x1000
#define sys_libc_darwin_MAP_FAILED as$(P$raw)(-1)

$extern fn_((sys_libc_darwin_mmap(P$raw addr, usize len, i32 prot, i32 flags, i32 fd, isize offset))(P$raw));
$extern fn_((sys_libc_darwin_munmap(P$raw addr, usize len))(i32));
$extern fn_((sys_libc_darwin_mprotect(P$raw addr, usize len, i32 prot))(i32));
#endif /* in_analysis || plat_is_darwin */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* sys_libc_darwin_mem__included */

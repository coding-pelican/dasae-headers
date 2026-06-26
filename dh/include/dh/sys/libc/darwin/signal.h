/**
 * @copyright Copyright (c) 2026 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    signal.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2026-06-26 (date of creation)
 * @ingroup dasae-headers(dh)/sys/libc/darwin
 * @prefix  sys_libc_darwin
 */
#pragma once
#ifndef sys_libc_darwin_signal__included
#define sys_libc_darwin_signal__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "cfg.h"
#include "dh/prl.h"

#if plat_is_darwin
#include <signal.h>
#endif /* plat_is_darwin */

/*========== Macros and Declarations ========================================*/

#if plat_is_darwin
typedef struct sigaction sys_libc_darwin_sigaction;
typedef sigset_t sys_libc_darwin_sigset;
typedef siginfo_t sys_libc_darwin_siginfo;
typedef sig_atomic_t sys_libc_darwin_sig_atomic_t;
#else
typedef Void sys_libc_darwin_sigaction;
typedef Void sys_libc_darwin_sigset;
typedef Void sys_libc_darwin_siginfo;
typedef i32 sys_libc_darwin_sig_atomic_t;
#endif /* plat_is_darwin */

typedef enum sys_libc_darwin_SIG {
    sys_libc_darwin_SIGILL = 4,
    sys_libc_darwin_SIGTRAP = 5,
    sys_libc_darwin_SIGABRT = 6,
    sys_libc_darwin_SIGBUS = 10,
    sys_libc_darwin_SIGFPE = 8,
    sys_libc_darwin_SIGSEGV = 11,
    sys_libc_darwin_SIGWINCH = 28,
} sys_libc_darwin_SIG;

typedef enum sys_libc_darwin_SA {
    sys_libc_darwin_SA_SIGINFO = 0x00000040,
} sys_libc_darwin_SA;

#if plat_is_darwin
#define sys_libc_darwin_SIG_DFL SIG_DFL
#else
#define sys_libc_darwin_SIG_DFL as$(void (*)(i32))(0)
#endif /* plat_is_darwin */

$extern fn_((sys_libc_darwin_sigemptyset(sys_libc_darwin_sigset* set))(i32));
$extern fn_((sys_libc_darwin_siginfo_addr(const sys_libc_darwin_siginfo* info))(void*));
$extern fn_((sys_libc_darwin_sigaction_set(i32 signal, const sys_libc_darwin_sigaction* act, sys_libc_darwin_sigaction* old_act))(i32));
$extern fn_((sys_libc_darwin_raise(i32 signal))(i32));

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* sys_libc_darwin_signal__included */

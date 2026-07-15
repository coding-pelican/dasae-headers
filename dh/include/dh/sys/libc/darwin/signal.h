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
#include "common.h"
#include "dh/prl.h"

/*========== Macros and Declarations ========================================*/

#if in_analysis || plat_is_darwin
typedef i32 sys_libc_darwin_sig_atomic_t;
typedef u32 sys_libc_darwin_sigset;

typedef union sys_libc_darwin_sigval {
    var_(sival_int, int);
    var_(sival_ptr, P$raw);
} sys_libc_darwin_sigval;

typedef struct sys_libc_darwin_siginfo {
    var_(si_signo, int);
    var_(si_errno, int);
    var_(si_code, int);
    var_(si_pid, i32);
    var_(si_uid, u32);
    var_(si_status, int);
    var_(si_addr, P$raw);
    var_(si_value, sys_libc_darwin_sigval);
    var_(si_band, isize);
    var_(__pad, A$$(7, usize));
} sys_libc_darwin_siginfo;

typedef void (*sys_libc_darwin_sighandler_fn)(sys_libc_darwin_signal_t);
typedef void (*sys_libc_darwin_sigaction_fn)(sys_libc_darwin_signal_t, sys_libc_darwin_siginfo*, void*);

typedef struct sys_libc_darwin_sigaction {
    union {
        sys_libc_darwin_sighandler_fn sa_handler;
        sys_libc_darwin_sigaction_fn sa_sigaction;
    };
    var_(sa_mask, sys_libc_darwin_sigset);
    var_(sa_flags, int);
} sys_libc_darwin_sigaction;

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

#define sys_libc_darwin_SIG_DFL as$(sys_libc_darwin_sighandler_fn)(0)

$extern fn_((sys_libc_darwin_sigemptyset(sys_libc_darwin_sigset* set))(i32));
$extern fn_((sys_libc_darwin_siginfo_addr(const sys_libc_darwin_siginfo* info))(void*));
$extern fn_((sys_libc_darwin_sigaction_set(sys_libc_darwin_signal_t signal, const sys_libc_darwin_sigaction* act, sys_libc_darwin_sigaction* old_act))(i32));
$extern fn_((sys_libc_darwin_raise(sys_libc_darwin_signal_t signal))(i32));
#endif /* in_analysis || plat_is_darwin */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* sys_libc_darwin_signal__included */

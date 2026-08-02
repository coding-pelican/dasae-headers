#pragma once
#ifndef sys_posix__included
#define sys_posix__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "call.h"
#include "libc.h"
#include "api/windows.h"

/*========== Macros and Declarations ========================================*/

typedef pp_if_(plat_is_windows)(
    pp_then_(HANDLE),
    pp_else_(pp_if_(plat_is_linux)(
        pp_then_(sys_call_linux_fd_t),
        pp_else_(pp_if_(plat_is_darwin)(
            pp_then_(sys_libc_darwin_fd_t),
            pp_else_(Void)
        ))
    ))) sys_posix_fd_t;

typedef pp_if_(plat_is_windows)(
    pp_then_(i32),
    pp_else_(pp_if_(plat_is_linux)(
        pp_then_(sys_call_linux_mode_t),
        pp_else_(pp_if_(plat_is_darwin)(
            pp_then_(sys_libc_darwin_mode_t),
            pp_else_(Void)
        ))
    ))) sys_posix_mode_t;

#define sys_posix_STDIN_FILENO comp_const__sys_posix_STDIN_FILENO
#define sys_posix_STDOUT_FILENO comp_const__sys_posix_STDOUT_FILENO
#define sys_posix_STDERR_FILENO comp_const__sys_posix_STDERR_FILENO

typedef pp_if_(plat_is_linux)(
    pp_then_(sys_call_linux_timespec),
    pp_else_(pp_if_(plat_is_darwin)(
        pp_then_(sys_libc_darwin_timespec),
        pp_else_(Void)
    ))) sys_posix_timespec;

typedef pp_if_(plat_is_linux)(
    pp_then_(sys_call_linux_timeval),
    pp_else_(pp_if_(plat_is_darwin)(
        pp_then_(sys_libc_darwin_timeval),
        pp_else_(Void)
    ))) sys_posix_timeval;

#define sys_posix_CLOCK_REALTIME comp_const__sys_posix_CLOCK_REALTIME
#define sys_posix_CLOCK_MONOTONIC comp_const__sys_posix_CLOCK_MONOTONIC
#define sys_posix_CLOCK_BOOT comp_const__sys_posix_CLOCK_BOOT
#define sys_posix_CLOCK_AWAKE comp_const__sys_posix_CLOCK_AWAKE
#define sys_posix_CLOCK_PROCESS_CPUTIME_ID comp_const__sys_posix_CLOCK_PROCESS_CPUTIME_ID
#define sys_posix_CLOCK_THREAD_CPUTIME_ID comp_const__sys_posix_CLOCK_THREAD_CPUTIME_ID

typedef pp_if_(plat_is_linux)(
    pp_then_(sys_call_linux_CLOCK),
    pp_else_(pp_if_(plat_is_darwin)(
        pp_then_(sys_libc_darwin_clockid_t),
        pp_else_(Void)
    ))) sys_posix_clockid_t;

typedef pp_if_(plat_is_linux)(
    pp_then_(sys_call_linux_sigaction),
    pp_else_(pp_if_(plat_is_darwin)(
        pp_then_(sys_libc_darwin_sigaction),
        pp_else_(Void)
    ))) sys_posix_sigaction;

typedef pp_if_(plat_is_linux)(
    pp_then_(sys_call_linux_sigset),
    pp_else_(pp_if_(plat_is_darwin)(
        pp_then_(sys_libc_darwin_sigset),
        pp_else_(Void)
    ))) sys_posix_sigset;

typedef pp_if_(plat_is_linux)(
    pp_then_(sys_call_linux_siginfo),
    pp_else_(pp_if_(plat_is_darwin)(
        pp_then_(sys_libc_darwin_siginfo),
        pp_else_(Void)
    ))) sys_posix_siginfo;

typedef pp_if_(plat_is_linux)(
    pp_then_(sys_call_linux_signal_t),
    pp_else_(pp_if_(plat_is_darwin)(
        pp_then_(sys_libc_darwin_signal_t),
        pp_else_(i32)
    ))) sys_posix_signal_t;

typedef pp_if_(plat_is_linux)(
    pp_then_(sys_call_linux_sig_atomic_t),
    pp_else_(pp_if_(plat_is_darwin)(
        pp_then_(sys_libc_darwin_sig_atomic_t),
        pp_else_(i32)
    ))) sys_posix_sig_atomic_t;

typedef pp_if_(plat_is_linux)(
    pp_then_(sys_call_linux_tcflag_t),
    pp_else_(pp_if_(plat_is_darwin)(
        pp_then_(sys_libc_darwin_tcflag_t),
        pp_else_(u32)
    ))) sys_posix_tcflag_t;

typedef pp_if_(plat_is_linux)(
    pp_then_(sys_call_linux_cc_t),
    pp_else_(pp_if_(plat_is_darwin)(
        pp_then_(sys_libc_darwin_cc_t),
        pp_else_(u8)
    ))) sys_posix_cc_t;

typedef pp_if_(plat_is_linux)(
    pp_then_(sys_call_linux_termios),
    pp_else_(pp_if_(plat_is_darwin)(
        pp_then_(sys_libc_darwin_termios),
        pp_else_(Void)
    ))) sys_posix_termios;

typedef pp_if_(plat_is_linux)(
    pp_then_(sys_call_linux_winsize),
    pp_else_(pp_if_(plat_is_darwin)(
        pp_then_(sys_libc_darwin_winsize),
        pp_else_(Void)
    ))) sys_posix_winsize;

typedef pp_if_(plat_is_linux)(
    pp_then_(sys_call_linux_ioctl_req_t),
    pp_else_(pp_if_(plat_is_darwin)(
        pp_then_(sys_libc_darwin_ioctl_req_t),
        pp_else_(Void)
    ))) sys_posix_ioctl_req_t;

#define sys_posix_SIGILL comp_const__sys_posix_SIGILL
#define sys_posix_SIGTRAP comp_const__sys_posix_SIGTRAP
#define sys_posix_SIGABRT comp_const__sys_posix_SIGABRT
#define sys_posix_SIGBUS comp_const__sys_posix_SIGBUS
#define sys_posix_SIGFPE comp_const__sys_posix_SIGFPE
#define sys_posix_SIGSEGV comp_const__sys_posix_SIGSEGV
#define sys_posix_SIGWINCH comp_const__sys_posix_SIGWINCH

#define sys_posix_SA_SIGINFO comp_const__sys_posix_SA_SIGINFO
#define sys_posix_SIG_DFL comp_const__sys_posix_SIG_DFL

#define sys_posix_VTIME comp_const__sys_posix_VTIME
#define sys_posix_VMIN comp_const__sys_posix_VMIN
#define sys_posix_BRKINT comp_const__sys_posix_BRKINT
#define sys_posix_INPCK comp_const__sys_posix_INPCK
#define sys_posix_ISTRIP comp_const__sys_posix_ISTRIP
#define sys_posix_ICRNL comp_const__sys_posix_ICRNL
#define sys_posix_IXON comp_const__sys_posix_IXON
#define sys_posix_IXOFF comp_const__sys_posix_IXOFF
#define sys_posix_OPOST comp_const__sys_posix_OPOST
#define sys_posix_ISIG comp_const__sys_posix_ISIG
#define sys_posix_ICANON comp_const__sys_posix_ICANON
#define sys_posix_ECHO comp_const__sys_posix_ECHO
#define sys_posix_IEXTEN comp_const__sys_posix_IEXTEN

$attr($inline_always)
$static fn_((sys_posix_clock_gettime(sys_posix_clockid_t clock_id, sys_posix_timespec* ts))(i32));
$attr($inline_always)
$static fn_((sys_posix_clock_getres(sys_posix_clockid_t clock_id, sys_posix_timespec* ts))(i32));
$attr($inline_always)
$static fn_((sys_posix_nanosleep(const sys_posix_timespec* req, sys_posix_timespec* rem))(i32));
$attr($inline_always)
$static fn_((sys_posix_gettimeofday(sys_posix_timeval* tv))(i32));
$attr($inline_always)
$static fn_((sys_posix_sigemptyset(sys_posix_sigset* set))(i32));
$attr($inline_always)
$static fn_((sys_posix_siginfo_addr(const sys_posix_siginfo* info))(void*));
$attr($inline_always)
$static fn_((sys_posix_sigaction_set(sys_posix_signal_t signal, O$P$raw act, O$P$raw old_act))(i32));
$attr($inline_always)
$static fn_((sys_posix_raise(sys_posix_signal_t signal))(i32));
$attr($inline_always)
$static fn_((sys_posix_tcgetattr(sys_posix_fd_t fd, sys_posix_termios* termios))(i32));
$attr($inline_always)
$static fn_((sys_posix_tcsetattr(sys_posix_fd_t fd, const sys_posix_termios* termios))(i32));
$attr($inline_always)
$static fn_((sys_posix_ioctl(sys_posix_fd_t fd, sys_posix_ioctl_req_t request, O$P$raw arg))(i32));
$attr($inline_always)
$static fn_((sys_posix_tiocgwinsz(sys_posix_fd_t fd, sys_posix_winsize* size))(i32));
$attr($inline_always)
$static fn_((sys_posix_fionread(sys_posix_fd_t fd, int* count))(i32));

/*========== Macros and Definitions =========================================*/

#define comp_const__sys_posix_STDIN_FILENO (as$(sys_posix_fd_t)( \
    pp_if_(pp_not(pp_or(plat_is_windows, plat_is_posix)))(pp_then_(Void_))(0) \
))
#define comp_const__sys_posix_STDOUT_FILENO (as$(sys_posix_fd_t)( \
    pp_if_(pp_not(pp_or(plat_is_windows, plat_is_posix)))(pp_then_(Void_))(1) \
))
#define comp_const__sys_posix_STDERR_FILENO (as$(sys_posix_fd_t)( \
    pp_if_(pp_not(pp_or(plat_is_windows, plat_is_posix)))(pp_then_(Void_))(2) \
))

#define comp_const__sys_posix_CLOCK_REALTIME pp_if_(plat_is_linux)( \
    pp_then_(sys_call_linux_CLOCK_REALTIME), \
    pp_else_(pp_if_(plat_is_darwin)( \
        pp_then_(sys_libc_darwin_CLOCK_REALTIME), \
        pp_else_(Void_()) \
    )) \
)
#define comp_const__sys_posix_CLOCK_MONOTONIC pp_if_(plat_is_linux)( \
    pp_then_(sys_call_linux_CLOCK_MONOTONIC), \
    pp_else_(pp_if_(plat_is_darwin)( \
        pp_then_(sys_libc_darwin_CLOCK_MONOTONIC), \
        pp_else_(Void_()) \
    )) \
)
#define comp_const__sys_posix_CLOCK_BOOT pp_if_(plat_is_linux)( \
    pp_then_(sys_call_linux_CLOCK_BOOTTIME), \
    pp_else_(pp_if_(plat_is_darwin)( \
        pp_then_(sys_libc_darwin_CLOCK_MONOTONIC_RAW), \
        pp_else_(Void_()) \
    )) \
)
#define comp_const__sys_posix_CLOCK_AWAKE pp_if_(plat_is_linux)( \
    pp_then_(sys_call_linux_CLOCK_MONOTONIC), \
    pp_else_(pp_if_(plat_is_darwin)( \
        pp_then_(sys_libc_darwin_CLOCK_UPTIME_RAW), \
        pp_else_(Void_()) \
    )) \
)
#define comp_const__sys_posix_CLOCK_PROCESS_CPUTIME_ID pp_if_(plat_is_linux)( \
    pp_then_(sys_call_linux_CLOCK_PROCESS_CPUTIME_ID), \
    pp_else_(pp_if_(plat_is_darwin)( \
        pp_then_(sys_libc_darwin_CLOCK_PROCESS_CPUTIME_ID), \
        pp_else_(Void_()) \
    )) \
)
#define comp_const__sys_posix_CLOCK_THREAD_CPUTIME_ID pp_if_(plat_is_linux)( \
    pp_then_(sys_call_linux_CLOCK_THREAD_CPUTIME_ID), \
    pp_else_(pp_if_(plat_is_darwin)( \
        pp_then_(sys_libc_darwin_CLOCK_THREAD_CPUTIME_ID), \
        pp_else_(Void_()) \
    )) \
)

#define comp_const__sys_posix_SIGILL pp_if_(plat_is_linux)(pp_then_(sys_call_linux_SIGILL), pp_else_(pp_if_(plat_is_darwin)(pp_then_(sys_libc_darwin_SIGILL), pp_else_(0))))
#define comp_const__sys_posix_SIGTRAP pp_if_(plat_is_linux)(pp_then_(sys_call_linux_SIGTRAP), pp_else_(pp_if_(plat_is_darwin)(pp_then_(sys_libc_darwin_SIGTRAP), pp_else_(0))))
#define comp_const__sys_posix_SIGABRT pp_if_(plat_is_linux)(pp_then_(sys_call_linux_SIGABRT), pp_else_(pp_if_(plat_is_darwin)(pp_then_(sys_libc_darwin_SIGABRT), pp_else_(0))))
#define comp_const__sys_posix_SIGBUS pp_if_(plat_is_linux)(pp_then_(sys_call_linux_SIGBUS), pp_else_(pp_if_(plat_is_darwin)(pp_then_(sys_libc_darwin_SIGBUS), pp_else_(0))))
#define comp_const__sys_posix_SIGFPE pp_if_(plat_is_linux)(pp_then_(sys_call_linux_SIGFPE), pp_else_(pp_if_(plat_is_darwin)(pp_then_(sys_libc_darwin_SIGFPE), pp_else_(0))))
#define comp_const__sys_posix_SIGSEGV pp_if_(plat_is_linux)(pp_then_(sys_call_linux_SIGSEGV), pp_else_(pp_if_(plat_is_darwin)(pp_then_(sys_libc_darwin_SIGSEGV), pp_else_(0))))
#define comp_const__sys_posix_SIGWINCH pp_if_(plat_is_linux)(pp_then_(sys_call_linux_SIGWINCH), pp_else_(pp_if_(plat_is_darwin)(pp_then_(sys_libc_darwin_SIGWINCH), pp_else_(0))))

#define comp_const__sys_posix_SA_SIGINFO pp_if_(plat_is_linux)(pp_then_(sys_call_linux_SA_SIGINFO), pp_else_(pp_if_(plat_is_darwin)(pp_then_(sys_libc_darwin_SA_SIGINFO), pp_else_(0))))
#define comp_const__sys_posix_SIG_DFL pp_if_(plat_is_linux)(pp_then_(sys_call_linux_SIG_DFL), pp_else_(pp_if_(plat_is_darwin)(pp_then_(sys_libc_darwin_SIG_DFL), pp_else_(null))))

#define comp_const__sys_posix_VTIME pp_if_(plat_is_linux)(pp_then_(sys_call_linux_VTIME), pp_else_(pp_if_(plat_is_darwin)(pp_then_(sys_libc_darwin_VTIME), pp_else_(0))))
#define comp_const__sys_posix_VMIN pp_if_(plat_is_linux)(pp_then_(sys_call_linux_VMIN), pp_else_(pp_if_(plat_is_darwin)(pp_then_(sys_libc_darwin_VMIN), pp_else_(0))))
#define comp_const__sys_posix_BRKINT pp_if_(plat_is_linux)(pp_then_(sys_call_linux_BRKINT), pp_else_(pp_if_(plat_is_darwin)(pp_then_(sys_libc_darwin_BRKINT), pp_else_(0))))
#define comp_const__sys_posix_INPCK pp_if_(plat_is_linux)(pp_then_(sys_call_linux_INPCK), pp_else_(pp_if_(plat_is_darwin)(pp_then_(sys_libc_darwin_INPCK), pp_else_(0))))
#define comp_const__sys_posix_ISTRIP pp_if_(plat_is_linux)(pp_then_(sys_call_linux_ISTRIP), pp_else_(pp_if_(plat_is_darwin)(pp_then_(sys_libc_darwin_ISTRIP), pp_else_(0))))
#define comp_const__sys_posix_ICRNL pp_if_(plat_is_linux)(pp_then_(sys_call_linux_ICRNL), pp_else_(pp_if_(plat_is_darwin)(pp_then_(sys_libc_darwin_ICRNL), pp_else_(0))))
#define comp_const__sys_posix_IXON pp_if_(plat_is_linux)(pp_then_(sys_call_linux_IXON), pp_else_(pp_if_(plat_is_darwin)(pp_then_(sys_libc_darwin_IXON), pp_else_(0))))
#define comp_const__sys_posix_IXOFF pp_if_(plat_is_linux)(pp_then_(sys_call_linux_IXOFF), pp_else_(pp_if_(plat_is_darwin)(pp_then_(sys_libc_darwin_IXOFF), pp_else_(0))))
#define comp_const__sys_posix_OPOST pp_if_(plat_is_linux)(pp_then_(sys_call_linux_OPOST), pp_else_(pp_if_(plat_is_darwin)(pp_then_(sys_libc_darwin_OPOST), pp_else_(0))))
#define comp_const__sys_posix_ISIG pp_if_(plat_is_linux)(pp_then_(sys_call_linux_ISIG), pp_else_(pp_if_(plat_is_darwin)(pp_then_(sys_libc_darwin_ISIG), pp_else_(0))))
#define comp_const__sys_posix_ICANON pp_if_(plat_is_linux)(pp_then_(sys_call_linux_ICANON), pp_else_(pp_if_(plat_is_darwin)(pp_then_(sys_libc_darwin_ICANON), pp_else_(0))))
#define comp_const__sys_posix_ECHO pp_if_(plat_is_linux)(pp_then_(sys_call_linux_ECHO), pp_else_(pp_if_(plat_is_darwin)(pp_then_(sys_libc_darwin_ECHO), pp_else_(0))))
#define comp_const__sys_posix_IEXTEN pp_if_(plat_is_linux)(pp_then_(sys_call_linux_IEXTEN), pp_else_(pp_if_(plat_is_darwin)(pp_then_(sys_libc_darwin_IEXTEN), pp_else_(0))))

#if in_analysis_active_only || in_comptime
fn_((sys_posix_clock_gettime(sys_posix_clockid_t clock_id, sys_posix_timespec* ts))(i32)) {
    pp_switch_((plat_type)(
        pp_case_((plat_type_linux)(
            return as$(i32)(sys_call_linux_clock_gettime(as$(sys_call_linux_clockid_t)(clock_id), ts))
        )),
        pp_case_((plat_type_darwin)(
            return sys_libc_darwin_clock_gettime(clock_id, ts)
        )),
        pp_default_(()({
            let_ignore = clock_id;
            let_ignore = ts;
            claim_unreachable_msg(nameOf(sys_posix_clock_gettime) "is not supported on this platform");
        }))
    ));
};
fn_((sys_posix_clock_getres(sys_posix_clockid_t clock_id, sys_posix_timespec* ts))(i32)) {
    pp_switch_((plat_type)(
        pp_case_((plat_type_linux)(
            return as$(i32)(sys_call_linux_clock_getres(as$(sys_call_linux_clockid_t)(clock_id), ts))
        )),
        pp_case_((plat_type_darwin)(
            return sys_libc_darwin_clock_getres(clock_id, ts)
        )),
        pp_default_(()({
            let_ignore = clock_id;
            let_ignore = ts;
            claim_unreachable_msg(nameOf(sys_posix_clock_getres) "is not supported on this platform");
        }))
    ));
};
fn_((sys_posix_nanosleep(const sys_posix_timespec* req, sys_posix_timespec* rem))(i32)) {
    pp_switch_((plat_type)(
        pp_case_((plat_type_linux)(
            return as$(i32)(sys_call_linux_nanosleep(req, rem))
        )),
        pp_case_((plat_type_darwin)(
            return sys_libc_darwin_nanosleep(req, rem)
        )),
        pp_default_(()({
            let_ignore = req;
            let_ignore = rem;
            claim_unreachable_msg(nameOf(sys_posix_nanosleep) "is not supported on this platform");
        }))
    ));
};
fn_((sys_posix_gettimeofday(sys_posix_timeval* tv))(i32)) {
    pp_switch_((plat_type)(
        pp_case_((plat_type_linux)(
            return as$(i32)(sys_call_linux_gettimeofday(tv))
        )),
        pp_case_((plat_type_darwin)(
            return sys_libc_darwin_gettimeofday(tv)
        )),
        pp_default_(()({
            let_ignore = tv;
            claim_unreachable_msg(nameOf(sys_posix_gettimeofday) "is not supported on this platform");
        }))
    ));
};
fn_((sys_posix_sigemptyset(sys_posix_sigset* set))(i32)) {
    pp_switch_((plat_type)(
        pp_case_((plat_type_linux)({
            sys_call_linux_sigemptyset(set);
            return 0;
        })),
        pp_case_((plat_type_darwin)(
            return sys_libc_darwin_sigemptyset(set)
        )),
        pp_default_(()({
            let_ignore = set;
            claim_unreachable_msg(nameOf(sys_posix_sigemptyset) "is not supported on this platform");
        }))
    ));
};
fn_((sys_posix_siginfo_addr(const sys_posix_siginfo* info))(void*)) {
    pp_switch_((plat_type)(
        pp_case_((plat_type_linux)(
            return sys_call_linux_siginfo_addr(info)
        )),
        pp_case_((plat_type_darwin)(
            return sys_libc_darwin_siginfo_addr(info)
        )),
        pp_default_(()({
            let_ignore = info;
            claim_unreachable_msg(nameOf(sys_posix_siginfo_addr) "is not supported on this platform");
        }))
    ));
};
fn_((sys_posix_sigaction_set(sys_posix_signal_t signal, O$P$raw act, O$P$raw old_act))(i32)) {
    pp_switch_((plat_type)(
        pp_case_((plat_type_linux)({
            var_(linux_act, sys_call_linux_sigaction) = cleared();
            var act_ptr = as$(sys_call_linux_sigaction*)(orelse_((act)(null)));
            let old_act_ptr = as$(sys_call_linux_sigaction*)(orelse_((old_act)(null)));
            if (act_ptr != null) {
                linux_act = *act_ptr;
                linux_act.sa_flags |= sys_call_linux_SA_RESTORER;
                linux_act.sa_restorer = sys_call_linux_rt_sigreturn_trampoline;
                act_ptr = &linux_act;
            }
            return as$(i32)(sys_call_linux_rt_sigaction(signal, act_ptr, old_act_ptr));
        })),
        pp_case_((plat_type_darwin)(
            return sys_libc_darwin_sigaction_set(
                signal,
                as$(sys_libc_darwin_sigaction*)(orelse_((act)(null))),
                as$(sys_libc_darwin_sigaction*)(orelse_((old_act)(null)))
            )
        )),
        pp_default_(()({
            let_ignore = signal;
            let_ignore = act;
            let_ignore = old_act;
            claim_unreachable_msg(nameOf(sys_posix_sigaction_set) "is not supported on this platform");
        }))
    ));
};
fn_((sys_posix_raise(sys_posix_signal_t signal))(i32)) {
    pp_switch_((plat_type)(
        pp_case_((plat_type_linux)({
            let pid = sys_call_linux_getpid();
            if (sys_call_linux_syscall_isErr(pid)) return as$(i32)(pid);
            return as$(i32)(sys_call_linux_kill(pid, signal));
        })),
        pp_case_((plat_type_darwin)(
            return sys_libc_darwin_raise(signal)
        )),
        pp_default_(()({
            let_ignore = signal;
            claim_unreachable_msg(nameOf(sys_posix_raise) "is not supported on this platform");
        }))
    ));
};
fn_((sys_posix_tcgetattr(sys_posix_fd_t fd, sys_posix_termios* termios))(i32)) {
    pp_switch_((plat_type)(
        pp_case_((plat_type_linux)(
            return as$(i32)(sys_call_linux_ioctl(as$(sys_call_linux_fd_t)(fd), sys_call_linux_TCGETS, termios))
        )),
        pp_case_((plat_type_darwin)(
            return sys_libc_darwin_tcgetattr(fd, termios)
        )),
        pp_default_(()({
            let_ignore = fd;
            let_ignore = termios;
            claim_unreachable_msg(nameOf(sys_posix_tcgetattr) "is not supported on this platform");
        }))
    ));
};
fn_((sys_posix_tcsetattr(sys_posix_fd_t fd, const sys_posix_termios* termios))(i32)) {
    pp_switch_((plat_type)(
        pp_case_((plat_type_linux)(
            return as$(i32)(sys_call_linux_ioctl(
                as$(sys_call_linux_fd_t)(fd),
                sys_call_linux_TCSETS,
                ptrQualCast$((sys_posix_termios*)(termios))
            ))
        )),
        pp_case_((plat_type_darwin)(
            return sys_libc_darwin_tcsetattr(fd, termios)
        )),
        pp_default_(()({
            let_ignore = fd;
            let_ignore = termios;
            claim_unreachable_msg(nameOf(sys_posix_tcsetattr) "is not supported on this platform");
        }))
    ));
};
fn_((sys_posix_ioctl(sys_posix_fd_t fd, sys_posix_ioctl_req_t request, O$P$raw arg))(i32)) {
    pp_switch_((plat_type)(
        pp_case_((plat_type_linux)(
            return as$(i32)(sys_call_linux_ioctl(
                as$(sys_call_linux_fd_t)(fd),
                as$(sys_call_linux_ioctl_req_t)(request),
                orelse_((arg)(null))
            ))
        )),
        pp_case_((plat_type_darwin)(
            let_ignore = fd;
            let_ignore = request;
            let_ignore = arg;
            claim_unreachable_msg(nameOf(sys_posix_ioctl) "does not expose raw Darwin ioctl requests")
        )),
        pp_default_(()({
            let_ignore = fd;
            let_ignore = request;
            let_ignore = arg;
            claim_unreachable_msg(nameOf(sys_posix_ioctl) "is not supported on this platform");
        }))
    ));
};
fn_((sys_posix_tiocgwinsz(sys_posix_fd_t fd, sys_posix_winsize* size))(i32)) {
    pp_switch_((plat_type)(
        pp_case_((plat_type_linux)(
            return as$(i32)(sys_call_linux_ioctl(
                as$(sys_call_linux_fd_t)(fd),
                sys_call_linux_TIOCGWINSZ,
                size
            ))
        )),
        pp_case_((plat_type_darwin)(
            return sys_libc_darwin_tiocgwinsz(fd, size)
        )),
        pp_default_(()({
            let_ignore = fd;
            let_ignore = size;
            claim_unreachable_msg(nameOf(sys_posix_tiocgwinsz) "is not supported on this platform");
        }))
    ));
};
fn_((sys_posix_fionread(sys_posix_fd_t fd, int* count))(i32)) {
    pp_switch_((plat_type)(
        pp_case_((plat_type_linux)(
            return as$(i32)(sys_call_linux_ioctl(
                as$(sys_call_linux_fd_t)(fd),
                sys_call_linux_FIONREAD,
                count
            ))
        )),
        pp_case_((plat_type_darwin)(
            return sys_libc_darwin_fionread(fd, count)
        )),
        pp_default_(()({
            let_ignore = fd;
            let_ignore = count;
            claim_unreachable_msg(nameOf(sys_posix_fionread) "is not supported on this platform");
        }))
    ));
};
#endif /* in_analysis_active_only || in_comptime */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* sys_posix__included */

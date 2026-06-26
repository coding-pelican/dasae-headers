#include "dh/sys/libc/darwin/signal.h"

#if plat_is_darwin
$extern fn_((sigemptyset(sys_libc_darwin_sigset* set))(i32));
$extern fn_((sigaction(i32 signal, const sys_libc_darwin_sigaction* act, sys_libc_darwin_sigaction* old_act))(i32));
$extern fn_((raise(i32 signal))(i32));
#endif /* plat_is_darwin */

fn_((sys_libc_darwin_sigemptyset(sys_libc_darwin_sigset* set))(i32)) {
#if plat_is_darwin
    return sigemptyset(set);
#else
    let_ignore = set;
    claim_unreachable_msg(nameOf(sys_libc_darwin_sigemptyset) "is not supported on this platform");
#endif /* plat_is_darwin */
};

fn_((sys_libc_darwin_siginfo_addr(const sys_libc_darwin_siginfo* info))(void*)) {
#if plat_is_darwin
    return info->si_addr;
#else
    let_ignore = info;
    claim_unreachable_msg(nameOf(sys_libc_darwin_siginfo_addr) "is not supported on this platform");
#endif /* plat_is_darwin */
};

fn_((sys_libc_darwin_sigaction_set(
    i32 signal,
    const sys_libc_darwin_sigaction* act,
    sys_libc_darwin_sigaction* old_act
))(i32)) {
#if plat_is_darwin
    return sigaction(signal, act, old_act);
#else
    let_ignore = signal;
    let_ignore = act;
    let_ignore = old_act;
    claim_unreachable_msg(nameOf(sys_libc_darwin_sigaction_set) "is not supported on this platform");
#endif /* plat_is_darwin */
};

fn_((sys_libc_darwin_raise(i32 signal))(i32)) {
#if plat_is_darwin
    return raise(signal);
#else
    let_ignore = signal;
    claim_unreachable_msg(nameOf(sys_libc_darwin_raise) "is not supported on this platform");
#endif /* plat_is_darwin */
};

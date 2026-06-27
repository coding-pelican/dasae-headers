#include "dh/sys/libc/darwin/signal.h"

#if plat_is_darwin
#include <signal.h>

claim_assert_static(sizeOf$(sys_libc_darwin_sigset) == sizeof(sigset_t));
claim_assert_static(alignOf$(sys_libc_darwin_sigset) == __alignof__(sigset_t));
claim_assert_static(sizeOf$(sys_libc_darwin_sig_atomic_t) == sizeof(sig_atomic_t));
claim_assert_static(alignOf$(sys_libc_darwin_sig_atomic_t) == __alignof__(sig_atomic_t));
claim_assert_static(sizeOf$(sys_libc_darwin_siginfo) == sizeof(siginfo_t));
claim_assert_static(alignOf$(sys_libc_darwin_siginfo) == __alignof__(siginfo_t));
claim_assert_static(offsetTo(sys_libc_darwin_siginfo, si_addr) == __builtin_offsetof(siginfo_t, si_addr));
claim_assert_static(sizeOf$(sys_libc_darwin_sigaction) == sizeof(struct sigaction));
claim_assert_static(alignOf$(sys_libc_darwin_sigaction) == __alignof__(struct sigaction));
claim_assert_static(offsetTo(sys_libc_darwin_sigaction, sa_mask) == __builtin_offsetof(struct sigaction, sa_mask));
claim_assert_static(offsetTo(sys_libc_darwin_sigaction, sa_flags) == __builtin_offsetof(struct sigaction, sa_flags));
#endif /* plat_is_darwin */

#if on_analysis || plat_is_darwin
fn_((sys_libc_darwin_sigemptyset(sys_libc_darwin_sigset* set))(i32)) {
#if plat_is_darwin
    return sigemptyset(as$(sigset_t*)(set));
#else
    let_ignore = set;
    claim_unreachable_msg(nameOf(sys_libc_darwin_sigemptyset) "is not supported on this platform");
#endif /* plat_is_darwin */
};

fn_((sys_libc_darwin_siginfo_addr(const sys_libc_darwin_siginfo* info))(void*)) {
#if plat_is_darwin
    return as$(const siginfo_t*)(info)->si_addr;
#else
    let_ignore = info;
    claim_unreachable_msg(nameOf(sys_libc_darwin_siginfo_addr) "is not supported on this platform");
#endif /* plat_is_darwin */
};

fn_((sys_libc_darwin_sigaction_set(
    sys_libc_darwin_signal_t signal,
    const sys_libc_darwin_sigaction* act,
    sys_libc_darwin_sigaction* old_act
))(i32)) {
#if plat_is_darwin
    return sigaction(signal, as$(const struct sigaction*)(act), as$(struct sigaction*)(old_act));
#else
    let_ignore = signal;
    let_ignore = act;
    let_ignore = old_act;
    claim_unreachable_msg(nameOf(sys_libc_darwin_sigaction_set) "is not supported on this platform");
#endif /* plat_is_darwin */
};

fn_((sys_libc_darwin_raise(sys_libc_darwin_signal_t signal))(i32)) {
#if plat_is_darwin
    return raise(signal);
#else
    let_ignore = signal;
    claim_unreachable_msg(nameOf(sys_libc_darwin_raise) "is not supported on this platform");
#endif /* plat_is_darwin */
};
#endif /* on_analysis || plat_is_darwin */

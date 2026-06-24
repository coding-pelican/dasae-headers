#include "dh/sys/libc/darwin/sync.h"

#if plat_is_darwin
$extern fn_((os_unfair_lock_lock(sys_libc_darwin_unfair_lock_t lock))(void));
$extern fn_((os_unfair_lock_trylock(sys_libc_darwin_unfair_lock_t lock))(bool));
$extern fn_((os_unfair_lock_unlock(sys_libc_darwin_unfair_lock_t lock))(void));
$extern fn_((os_unfair_lock_assert_owner(sys_libc_darwin_unfair_lock_t lock))(void));
$extern fn_((os_unfair_lock_assert_not_owner(sys_libc_darwin_unfair_lock_t lock))(void));

$extern fn_((__ulock_wait(sys_libc_darwin_UL operation, P$raw addr, u64 value, u32 timeout_us))(i32));
#if sys_libc_darwin_has_ulock_wait2
$extern fn_((__ulock_wait2(sys_libc_darwin_UL operation, P$raw addr, u64 value, u64 timeout_ns, u64 value2))(i32));
#endif /* sys_libc_darwin_has_ulock_wait2 */
$extern fn_((__ulock_wake(sys_libc_darwin_UL operation, P$raw addr, u64 wake_value))(i32));
#endif /* plat_is_darwin */

fn_((sys_libc_darwin_unfair_lock_lock(sys_libc_darwin_unfair_lock_t lock))(void)) {
#if plat_is_darwin
    os_unfair_lock_lock(lock);
#else
    let_ignore = lock;
    claim_unreachable_msg(nameOf(sys_libc_darwin_unfair_lock_lock) "is not supported on this platform");
#endif /* plat_is_darwin */
};

fn_((sys_libc_darwin_unfair_lock_trylock(sys_libc_darwin_unfair_lock_t lock))(bool)) {
#if plat_is_darwin
    return os_unfair_lock_trylock(lock);
#else
    let_ignore = lock;
    claim_unreachable_msg(nameOf(sys_libc_darwin_unfair_lock_trylock) "is not supported on this platform");
#endif /* plat_is_darwin */
};

fn_((sys_libc_darwin_unfair_lock_unlock(sys_libc_darwin_unfair_lock_t lock))(void)) {
#if plat_is_darwin
    os_unfair_lock_unlock(lock);
#else
    let_ignore = lock;
    claim_unreachable_msg(nameOf(sys_libc_darwin_unfair_lock_unlock) "is not supported on this platform");
#endif /* plat_is_darwin */
};

fn_((sys_libc_darwin_unfair_lock_assert_owner(sys_libc_darwin_unfair_lock_t lock))(void)) {
#if plat_is_darwin
    os_unfair_lock_assert_owner(lock);
#else
    let_ignore = lock;
    claim_unreachable_msg(nameOf(sys_libc_darwin_unfair_lock_assert_owner) "is not supported on this platform");
#endif /* plat_is_darwin */
};

fn_((sys_libc_darwin_unfair_lock_assert_not_owner(sys_libc_darwin_unfair_lock_t lock))(void)) {
#if plat_is_darwin
    os_unfair_lock_assert_not_owner(lock);
#else
    let_ignore = lock;
    claim_unreachable_msg(nameOf(sys_libc_darwin_unfair_lock_assert_not_owner) "is not supported on this platform");
#endif /* plat_is_darwin */
};

fn_((sys_libc_darwin_ulock_wait(sys_libc_darwin_UL operation, P$raw addr, u64 value, u32 timeout_us))(i32)) {
#if plat_is_darwin
    return __ulock_wait(operation, addr, value, timeout_us);
#else
    let_ignore = operation;
    let_ignore = addr;
    let_ignore = value;
    let_ignore = timeout_us;
    claim_unreachable_msg(nameOf(sys_libc_darwin_ulock_wait) "is not supported on this platform");
#endif /* plat_is_darwin */
};

fn_((sys_libc_darwin_ulock_wait2(sys_libc_darwin_UL operation, P$raw addr, u64 value, u64 timeout_ns, u64 value2))(i32)) {
#if plat_is_darwin && sys_libc_darwin_has_ulock_wait2
    return __ulock_wait2(operation, addr, value, timeout_ns, value2);
#else
    let_ignore = operation;
    let_ignore = addr;
    let_ignore = value;
    let_ignore = timeout_ns;
    let_ignore = value2;
    claim_unreachable_msg(nameOf(sys_libc_darwin_ulock_wait2) "is not supported on this platform");
#endif /* plat_is_darwin && sys_libc_darwin_has_ulock_wait2 */
};

fn_((sys_libc_darwin_ulock_wake(sys_libc_darwin_UL operation, P$raw addr, u64 wake_value))(i32)) {
#if plat_is_darwin
    return __ulock_wake(operation, addr, wake_value);
#else
    let_ignore = operation;
    let_ignore = addr;
    let_ignore = wake_value;
    claim_unreachable_msg(nameOf(sys_libc_darwin_ulock_wake) "is not supported on this platform");
#endif /* plat_is_darwin */
};

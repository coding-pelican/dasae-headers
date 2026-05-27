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

fn_((sys_libc_darwin_unfair_lock_lock(sys_libc_darwin_unfair_lock_t lock))(void)) {
    os_unfair_lock_lock(lock);
};

fn_((sys_libc_darwin_unfair_lock_trylock(sys_libc_darwin_unfair_lock_t lock))(bool)) {
    return os_unfair_lock_trylock(lock);
};

fn_((sys_libc_darwin_unfair_lock_unlock(sys_libc_darwin_unfair_lock_t lock))(void)) {
    os_unfair_lock_unlock(lock);
};

fn_((sys_libc_darwin_unfair_lock_assert_owner(sys_libc_darwin_unfair_lock_t lock))(void)) {
    os_unfair_lock_assert_owner(lock);
};

fn_((sys_libc_darwin_unfair_lock_assert_not_owner(sys_libc_darwin_unfair_lock_t lock))(void)) {
    os_unfair_lock_assert_not_owner(lock);
};

fn_((sys_libc_darwin_ulock_wait(sys_libc_darwin_UL operation, P$raw addr, u64 value, u32 timeout_us))(i32)) {
    return __ulock_wait(operation, addr, value, timeout_us);
};

#if sys_libc_darwin_has_ulock_wait2
fn_((sys_libc_darwin_ulock_wait2(sys_libc_darwin_UL operation, P$raw addr, u64 value, u64 timeout_ns, u64 value2))(i32)) {
    return __ulock_wait2(operation, addr, value, timeout_ns, value2);
};
#endif /* sys_libc_darwin_has_ulock_wait2 */

fn_((sys_libc_darwin_ulock_wake(sys_libc_darwin_UL operation, P$raw addr, u64 wake_value))(i32)) {
    return __ulock_wake(operation, addr, wake_value);
};
#endif /* plat_is_darwin */

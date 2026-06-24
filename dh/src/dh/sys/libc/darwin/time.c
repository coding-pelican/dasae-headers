#include "dh/sys/libc/darwin/time.h"

#if plat_is_darwin
$extern fn_((clock_gettime(sys_libc_darwin_clockid_t clock_id, sys_libc_darwin_timespec* ts))(i32));
#endif /* plat_is_darwin */

fn_((sys_libc_darwin_clock_gettime(sys_libc_darwin_clockid_t clock_id, sys_libc_darwin_timespec* ts))(i32)) {
#if plat_is_darwin
    return clock_gettime(clock_id, ts);
#else
    let_ignore = clock_id;
    let_ignore = ts;
    claim_unreachable_msg(nameOf(sys_libc_darwin_clock_gettime) "is not supported on this platform");
#endif /* plat_is_darwin */
};

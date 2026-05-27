#include "dh/sys/libc/darwin/time.h"

#if plat_is_darwin
$extern fn_((clock_gettime(sys_libc_darwin_clockid_t clock_id, sys_libc_darwin_timespec* ts))(i32));

fn_((sys_libc_darwin_clock_gettime(sys_libc_darwin_clockid_t clock_id, sys_libc_darwin_timespec* ts))(i32)) {
    return clock_gettime(clock_id, ts);
};
#endif /* plat_is_darwin */

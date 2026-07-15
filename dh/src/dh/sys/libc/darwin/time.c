#include "dh/sys/libc/darwin/time.h"

#if plat_is_darwin
$extern fn_((clock_gettime(sys_libc_darwin_clockid_t clock_id, sys_libc_darwin_timespec* ts))(i32));
$extern fn_((clock_getres(sys_libc_darwin_clockid_t clock_id, sys_libc_darwin_timespec* ts))(i32));
$extern fn_((nanosleep(const sys_libc_darwin_timespec* req, sys_libc_darwin_timespec* rem))(i32));
$extern fn_((gettimeofday(sys_libc_darwin_timeval* tv, void* tz))(i32));
#endif /* plat_is_darwin */

#if in_analysis || plat_is_darwin
fn_((sys_libc_darwin_clock_gettime(sys_libc_darwin_clockid_t clock_id, sys_libc_darwin_timespec* ts))(i32)) {
#if plat_is_darwin
    return clock_gettime(clock_id, ts);
#else
    let_ignore = clock_id;
    let_ignore = ts;
    claim_unreachable_msg(nameOf(sys_libc_darwin_clock_gettime) "is not supported on this platform");
#endif /* plat_is_darwin */
};

fn_((sys_libc_darwin_clock_getres(sys_libc_darwin_clockid_t clock_id, sys_libc_darwin_timespec* ts))(i32)) {
#if plat_is_darwin
    return clock_getres(clock_id, ts);
#else
    let_ignore = clock_id;
    let_ignore = ts;
    claim_unreachable_msg(nameOf(sys_libc_darwin_clock_getres) "is not supported on this platform");
#endif /* plat_is_darwin */
};

fn_((sys_libc_darwin_nanosleep(const sys_libc_darwin_timespec* req, sys_libc_darwin_timespec* rem))(i32)) {
#if plat_is_darwin
    return nanosleep(req, rem);
#else
    let_ignore = req;
    let_ignore = rem;
    claim_unreachable_msg(nameOf(sys_libc_darwin_nanosleep) "is not supported on this platform");
#endif /* plat_is_darwin */
};

fn_((sys_libc_darwin_gettimeofday(sys_libc_darwin_timeval* tv))(i32)) {
#if plat_is_darwin
    return gettimeofday(tv, null);
#else
    let_ignore = tv;
    claim_unreachable_msg(nameOf(sys_libc_darwin_gettimeofday) "is not supported on this platform");
#endif /* plat_is_darwin */
};
#endif /* in_analysis || plat_is_darwin */

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
    pp_else_(i32)) sys_posix_fd_t;
typedef i32 sys_posix_mode_t;

#define sys_posix_STDIN_FILENO comp_const__sys_posix_STDIN_FILENO
#define sys_posix_STDOUT_FILENO comp_const__sys_posix_STDOUT_FILENO
#define sys_posix_STDERR_FILENO comp_const__sys_posix_STDERR_FILENO

typedef pp_if_(plat_is_linux)(
    pp_then_(sys_call_linux_timespec),
    pp_else_(pp_if_(plat_is_darwin)(
        pp_then_(sys_libc_darwin_timespec),
        pp_else_(Void)
    ))) sys_posix_timespec;

#define sys_posix_CLOCK_REALTIME comp_const__sys_posix_CLOCK_REALTIME
#define sys_posix_CLOCK_MONOTONIC comp_const__sys_posix_CLOCK_MONOTONIC

typedef pp_if_(plat_is_linux)(
    pp_then_(sys_call_linux_CLOCK),
    pp_else_(pp_if_(plat_is_darwin)(
        pp_then_(sys_libc_darwin_clockid_t),
        pp_else_(Void)
    ))) sys_posix_clockid_t;

$attr($inline_always)
$static fn_((sys_posix_clock_gettime(sys_posix_clockid_t clock_id, sys_posix_timespec* ts))(i32));
$attr($inline_always)
$static fn_((sys_posix_nanosleep(const sys_posix_timespec* req, sys_posix_timespec* rem))(i32));

/*========== Macros and Definitions =========================================*/

#define comp_const__sys_posix_STDIN_FILENO (as$(sys_posix_fd_t)(0))
#define comp_const__sys_posix_STDOUT_FILENO (as$(sys_posix_fd_t)(1))
#define comp_const__sys_posix_STDERR_FILENO (as$(sys_posix_fd_t)(2))

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

#if on_analysis_active_only || on_comptime
fn_((sys_posix_clock_gettime(sys_posix_clockid_t clock_id, sys_posix_timespec* ts))(i32)) {
    pp_switch_((plat_type)(
        pp_case_((plat_type_linux)(
            return as$(i32)(sys_call_linux_clock_gettime(as$(sys_call_linux_word)(clock_id), ts))
        )),
        pp_case_((plat_type_darwin)(
            return sys_libc_darwin_clock_gettime(clock_id, ts)
        )),
        pp_default_({
            let_ignore = clock_id;
            let_ignore = ts;
            claim_unreachable_msg(nameOf(sys_posix_clock_gettime) "is not supported on this platform");
        })
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
        pp_default_({
            let_ignore = req;
            let_ignore = rem;
            claim_unreachable_msg(nameOf(sys_posix_nanosleep) "is not supported on this platform");
        })
    ));
};
#endif /* on_analysis_active_only || on_comptime */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* sys_posix__included */

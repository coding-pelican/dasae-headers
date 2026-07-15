/**
 * @copyright Copyright (c) 2026 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    time.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2026-05-27 (date of creation)
 * @ingroup dasae-headers(dh)/sys/libc/darwin
 * @prefix  sys_libc_darwin
 */
#pragma once
#ifndef sys_libc_darwin_time__included
#define sys_libc_darwin_time__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "cfg.h"
#include "dh/prl.h"

/*========== Macros and Declarations ========================================*/

#if in_analysis || plat_is_darwin
typedef isize sys_libc_darwin_time_t;
typedef isize sys_libc_darwin_c_long;

typedef struct sys_libc_darwin_timespec {
    sys_libc_darwin_time_t tv_sec;
    sys_libc_darwin_c_long tv_nsec;
} sys_libc_darwin_timespec;

typedef struct sys_libc_darwin_timeval {
    sys_libc_darwin_time_t tv_sec;
    sys_libc_darwin_c_long tv_usec;
} sys_libc_darwin_timeval;

typedef enum sys_libc_darwin_CLOCK {
    sys_libc_darwin_CLOCK_REALTIME = 0,
    sys_libc_darwin_CLOCK_MONOTONIC_RAW = 4,
    sys_libc_darwin_CLOCK_MONOTONIC_RAW_APPROX = 5,
    sys_libc_darwin_CLOCK_MONOTONIC = 6,
    sys_libc_darwin_CLOCK_UPTIME_RAW = 8,
    sys_libc_darwin_CLOCK_PROCESS_CPUTIME_ID = 12,
    sys_libc_darwin_CLOCK_THREAD_CPUTIME_ID = 16,
} sys_libc_darwin_CLOCK;
typedef sys_libc_darwin_CLOCK sys_libc_darwin_clockid_t;

$extern fn_((sys_libc_darwin_clock_gettime(sys_libc_darwin_clockid_t clock_id, sys_libc_darwin_timespec* ts))(i32));
$extern fn_((sys_libc_darwin_clock_getres(sys_libc_darwin_clockid_t clock_id, sys_libc_darwin_timespec* ts))(i32));
$extern fn_((sys_libc_darwin_nanosleep(const sys_libc_darwin_timespec* req, sys_libc_darwin_timespec* rem))(i32));
$extern fn_((sys_libc_darwin_gettimeofday(sys_libc_darwin_timeval* tv))(i32));
#endif /* in_analysis || plat_is_darwin */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* sys_libc_darwin_time__included */

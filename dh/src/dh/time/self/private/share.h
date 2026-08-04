#pragma once
#ifndef time_private_share__included
#define time_private_share__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "dh/time/common.h"
#include "dh/time/Dur.h"
#include "dh/time/Inst.h"
#include "dh/Sched.h"

#if plat_is_windows
#include "dh/sys/api/windows/sync.h"
#include "dh/sys/api/windows/proc.h"
#include "dh/sys/api/windows/thrd.h"
#endif /* plat_is_windows */
#if plat_is_posix
#include "dh/sys/posix.h"
#endif /* plat_is_posix */

/*========== Macros and Declarations ========================================*/

#if plat_is_windows
#define time__windows_intervals_per_sec (u64_(10, 000, 000ull))
#define time__windows_intervals_to_unix_epoch (u64_(116, 444, 736, 000, 000, 000ull))
$extern fn_((time__windows_fromUnixFileTime(FILETIME ft))(time_Inst));
$extern fn_((time__windows_fromFileTime(FILETIME ft))(time_Inst));
$extern fn_((time__windows_qpcFreq(void))(u64));
$extern fn_((time__windows_qpcNow(void))(time_Inst));
$extern fn_((time__windows_qpcResoln(void))(time_Resoln));
#endif /* plat_is_windows */

#if plat_is_posix
$extern fn_((time__posix_fromTimespec(sys_posix_timespec ts))(time_Inst));
#endif /* plat_is_posix */

$attr($must_check)
$extern fn_((time__direct_sleep(time_Dur dur))(Sched_Cancelable$void));

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* time_private_share__included */

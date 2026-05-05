#ifndef time_internal__included
#define time_internal__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "../common.h"
#include "../Dur.h"
#include "../Inst.h"
#include "../../Sched.h"

#if plat_is_windows
#include "dh/os/windows/sync.h"
#include "dh/os/windows/proc.h"
#include "dh/os/windows/thrd.h"
#endif /* plat_is_windows */
#if plat_based_unix
#include <time.h>
#endif /* plat_based_unix */

/*========== Macros and Declarations ========================================*/

#if plat_is_windows
#define time__windows_intervals_per_sec (n$(u64)(10, 000, 000ull))
#define time__windows_intervals_to_unix_epoch (n$(u64)(116, 444, 736, 000, 000, 000ull))
$extern fn_((time__windows_fromUnixFileTime(FILETIME ft))(time_Inst));
$extern fn_((time__windows_fromFileTime(FILETIME ft))(time_Inst));
$extern fn_((time__windows_qpcFreq(void))(u64));
#endif /* plat_is_windows */
#if plat_based_unix
$extern fn_((time__unix_fromTimespec(struct timespec ts))(time_Inst));
#endif /* plat_based_unix */
$extern fn_((time__direct_sleep(time_Dur dur))(Sched_Cancelable$void));

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* time_internal__included */

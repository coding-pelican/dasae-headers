#ifndef time_self__included
#define time_self__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "common.h"
#include "../Sched/base.h"

/*========== Macros and Declarations ========================================*/

errset_((time_sleep_E)() $union_errset_(time_E, Sched_Cancelable));

/* --- Sleep --- */
$attr($must_check)
$extern fn_((time_sleep(time_Dur duration))(time_sleep_E$void));
$attr($must_check)
$extern fn_((time_sleepSecs(u64 secs))(time_sleep_E$void));
$attr($must_check)
$extern fn_((time_sleepMillis(u64 millis))(time_sleep_E$void));
$attr($must_check)
$extern fn_((time_sleepMicros(u64 micros))(time_sleep_E$void));
$attr($must_check)
$extern fn_((time_sleepNanos(u32 nanos))(time_sleep_E$void));

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* time_self__included */

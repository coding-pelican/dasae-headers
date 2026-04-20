#ifndef time_Self__included
#define time_Self__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "common.h"
#include "../Sched.h"

/*========== Macros and Declarations ========================================*/

T_alias$((time_Self)(struct time_Self{}));
$extern fn_((time_seq(void))(time_Self));
$extern fn_((time_para(void))(time_Self));
$attr($must_check)
$extern fn_((time_sleep(time_Self self, time_Dur dur))(Sched_Cancelable$void));
$attr($must_check)
$extern fn_((time_sleepSecs(time_Self self, u64 secs))(Sched_Cancelable$void));
$attr($must_check)
$extern fn_((time_sleepMillis(time_Self self, u64 millis))(Sched_Cancelable$void));
$attr($must_check)
$extern fn_((time_sleepMicros(time_Self self, u64 micros))(Sched_Cancelable$void));
$attr($must_check)
$extern fn_((time_sleepNanos(time_Self self, u32 nanos))(Sched_Cancelable$void));

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* time_Self__included */

#ifndef time_Self__included
#define time_Self__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "common.h"
#include "../Sched.h"

/*========== Macros and Declarations ========================================*/

T_alias$((time_Self_Vtbl)(struct time_Self_Vtbl {
    fn_(((*nowClockFn)(P$raw ctx))(time_Clock));
    fn_(((*nowInstFn)(P$raw ctx))(time_Inst));
    fn_(((*freqInstFn)(P$raw ctx))(time_Inst));
    fn_(((*freqInstInvFn)(P$raw ctx))(f64));
    fn_(((*offsetInstFn)(P$raw ctx))(time_Inst));
    $attr($must_check)
    fn_(((*sleepFn)(P$raw ctx, time_Dur dur))(Sched_Cancelable$void));
}));
T_alias$((time_Self)(struct time_Self {
    var_(ctx, P$raw);
    var_(vtbl, P_const$$(time_Self_Vtbl));
}));
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

$extern fn_((time_direct(void))(time_Self));
$extern fn_((time_evented(exec_Coop* coop))(time_Self));

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* time_Self__included */

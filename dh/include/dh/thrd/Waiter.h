#pragma once
#ifndef thrd_Waiter__included
#define thrd_Waiter__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "wait.h"
#include "Mtx.h"
#include "Cond.h"

/*========== Macros and Declarations ========================================*/

typedef struct thrd_Waiter {
    var_(lock, thrd_Mtx);
    var_(cond, thrd_Cond);
    var_(woken, bool);
} thrd_Waiter;
$extern fn_((thrd_Waiter_init(void))(thrd_Waiter));
$extern fn_((thrd_Waiter_fini(thrd_Waiter* self))(void));

$extern fn_((thrd_Waiter_link(thrd_Waiter* self, usize case_idx))(thrd_wait_Link));
$attr($must_check)
$extern fn_((thrd_Waiter_wait(thrd_Waiter* self, thrd_wait_Src cancel_src))(Sched_Cancelable$void));
$attr($must_check)
$extern fn_((thrd_Waiter_waitFor(thrd_Waiter* self, thrd_wait_Src cancel_src, time_Dur dur))(Sched_TimedE$void));
$extern fn_((thrd_Waiter_waitProtcd(thrd_Waiter* self))(void));
$extern fn_((thrd_Waiter_wake(P$raw ctx))(void));

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* thrd_Waiter__included */

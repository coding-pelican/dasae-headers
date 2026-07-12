#pragma once
#ifndef thrd_chan__included
#define thrd_chan__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "Mtx.h"
#include "Cond.h"

/*========== Macros and Declarations ========================================*/

errset_((thrd_chan_ClosedE)(thrd_chan_Closed));
T_use_E$($set(thrd_chan_ClosedE)(u_V$raw));
errset_((thrd_chan_CapE)(thrd_chan_Empty, thrd_chan_Full));
T_use_E$($set(thrd_chan_CapE)(u_V$raw));
errset_((thrd_chan_E)() $union_errset_(thrd_chan_ClosedE, thrd_chan_CapE));
T_use_E$($set(thrd_chan_E)(u_V$raw));

errset_((thrd_chan_WaitE)() $union_errset_(thrd_chan_ClosedE, Sched_Cancelable));
T_use_E$($set(thrd_chan_WaitE)(u_V$raw));
errset_((thrd_chan_TimedE)() $union_errset_(thrd_chan_ClosedE, Sched_TimedE));
T_use_E$($set(thrd_chan_TimedE)(u_V$raw));

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* thrd_chan__included */

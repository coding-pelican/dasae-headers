#pragma once
#ifndef Sched_base__included
#define Sched_base__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "../Future/base.h"

/*========== Macros and Declarations ========================================*/

T_alias$((Sched)(struct Sched));

errset_((Sched_ConcE)(Sched_ConcUnavailable));
T_use_E$($set(Sched_ConcE)(P$raw));
T_use_E$($set(Sched_ConcE)(V$raw));
T_use_E$($set(Sched_ConcE)(u_V$raw));

T_use_E$($set(Sched_ConcE)(P$FutureAny));
T_use_E$($set(Sched_ConcE)(P$Future$raw));
T_use_E$($set(Sched_ConcE)(V$Future$raw));
T_use_E$($set(Sched_ConcE)(u_V$Future$raw));

errset_((Sched_Cancelable)(Sched_Canceled));
T_use_E$($set(Sched_Cancelable)(P$raw));
T_use_E$($set(Sched_Cancelable)(V$raw));
T_use_E$($set(Sched_Cancelable)(u_V$raw));

T_use_E$($set(Sched_Cancelable)(P$FutureAny));
T_use_E$($set(Sched_Cancelable)(P$Future$raw));
T_use_E$($set(Sched_Cancelable)(V$Future$raw));
T_use_E$($set(Sched_Cancelable)(u_V$Future$raw));

T_alias$((Sched_CancelProtcn)(enum_((Sched_CancelProtcn $fits($packed))(
    Sched_CancelProtcn_unblocked = 0,
    Sched_CancelProtcn_blocked
))));
claim_assert_static(eqlType$(Sched_CancelProtcn, u8));

errset_((Sched_TimeoutE)(Sched_Timeout));
T_use_E$($set(Sched_TimeoutE)(P$raw));
T_use_E$($set(Sched_TimeoutE)(V$raw));
T_use_E$($set(Sched_TimeoutE)(u_V$raw));

errset_((Sched_TimedE)() $union_errset_(Sched_Cancelable, Sched_TimeoutE));
T_use_E$($set(Sched_TimedE)(P$raw));
T_use_E$($set(Sched_TimedE)(V$raw));
T_use_E$($set(Sched_TimedE)(u_V$raw));

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* Sched_base__included */

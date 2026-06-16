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
T_use_E$($set(Sched_ConcE)(P$FutureAny));
T_use_E$($set(Sched_ConcE)(P$Future$raw));
T_use_E$($set(Sched_ConcE)(V$Future$raw));

errset_((Sched_Cancelable)(Sched_Canceled));
T_use_E$($set(Sched_Cancelable)(P$FutureAny));
T_use_E$($set(Sched_Cancelable)(P$Future$raw));
T_use_E$($set(Sched_Cancelable)(V$Future$raw));

T_alias$((Sched_CancelProtn)(enum_((Sched_CancelProtn $fits($packed))(
    Sched_CancelProtn_unblocked = 0,
    Sched_CancelProtn_blocked,
))));
claim_assert_static(eqlType$(Sched_CancelProtn, u8));

errset_((Sched_TimeoutE)(Sched_Timeout));

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* Sched_base__included */

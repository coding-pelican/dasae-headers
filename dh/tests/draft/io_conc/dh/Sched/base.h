#ifndef Sched_base__included
#define Sched_base__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "../prl.h"
#include "../Future/base.h"

/*========== Macros and Declarations ========================================*/

T_alias$((Sched)(struct Sched));

errset_((Sched_ConcE)(Unavailable));
T_use_E$($set(Sched_ConcE)(P$FutureAny));
T_use_E$($set(Sched_ConcE)(P$Future$raw));
T_use_E$($set(Sched_ConcE)(V$Future$raw));

errset_((Sched_Cancelable)(Canceled));
T_use_E$($set(Sched_Cancelable)(P$FutureAny));
T_use_E$($set(Sched_Cancelable)(P$Future$raw));
T_use_E$($set(Sched_Cancelable)(V$Future$raw));

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* Sched_base__included */

#ifndef exec_common__included
#define exec_common__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "base.h"

/*========== Macros and Declarations ========================================*/

/* --- Fiber context switch --- */

$attr($no_return)
$extern fn_((exec_callFiber(exec_Fiber_Starter* starter, const co_Fiber* first_switch))(void));
$extern fn_((exec_switchToFiber(co_Fiber_Context* sched_context, exec_Fiber* fiber))(void));
$extern fn_((exec_switchFromFiber(co_Fiber_Context* fiber_context, co_Fiber_Context* sched_context))(void));

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* exec_common__included */

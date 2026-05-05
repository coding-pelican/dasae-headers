#ifndef exec_Task__included
#define exec_Task__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "../prl/Closure.h"
#include "../Future/base.h"
#include "Fiber.h"
#include "dh/ArrList.h"
#include "dh/ArrQue.h"

/*========== Macros and Declarations ========================================*/

T_alias$((exec_Task_Kind)(enum_((exec_Task_Kind $fits($packed))(
    exec_Task_Kind_stackless = 0,
    exec_Task_Kind_fiber,
))));
claim_assert_static(eqlType$(exec_Task_Kind, u8));
T_alias$((exec_Task_State)(enum_((exec_Task_State $fits($packed))(
    exec_Task_State_none = 0,
    exec_Task_State_deferred,
    exec_Task_State_ready,
    exec_Task_State_running,
    exec_Task_State_waiting,
    exec_Task_State_done,
    exec_Task_State_canceled,
))));
claim_assert_static(eqlType$(exec_Task_State, u8));
T_alias$((exec_Task)(union exec_Task {
    struct {
        var_(state, exec_Task_State);
        var_(result, u_P$raw);
        var_(inner, P$$(Closure$raw));
        var_(fiber, O$P$exec_Fiber);
    };
    var_(as_any, FutureAny) $flexible;
}));
T_use_prl$(exec_Task);
T_use_ArrList$(P$exec_Task);
T_use_ArrQue$(P$exec_Task);
$extern fn_((exec_Task_isDone(P_const$exec_Task task))(bool));
$extern fn_((exec_Task_copyToResult(P_const$exec_Task task, u_P$raw result))(void));
$extern fn_((exec_Task_cancel(exec_Task* task))(void));

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* exec_Task__included */

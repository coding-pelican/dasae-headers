#ifndef exec_Seq__included
#define exec_Seq__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "common.h"
#include "dh/ArrList.h"
#include "dh/ArrQue.h"

/*========== Macros and Declarations ========================================*/

T_alias$((exec_Seq_Task_State)(enum_((exec_Seq_Task_State $fits($packed))(
    exec_Seq_Task_State_none = 0,
    exec_Seq_Task_State_deferred,
    exec_Seq_Task_State_ready,
    exec_Seq_Task_State_running,
    exec_Seq_Task_State_done,
    exec_Seq_Task_State_canceled,
))));
T_alias$((exec_Seq_Task)(struct exec_Seq_Task {
    var_(state, exec_Seq_Task_State);
    var_(result, u_P$raw);
    var_(inner, P$$(Closure$raw));
    var_(fiber, O$P$exec_Fiber);
}));
T_use_prl$(exec_Seq_Task);
T_use_ArrList$(P$exec_Seq_Task);
T_use_ArrQue$(P$exec_Seq_Task);

T_alias$((exec_Seq)(struct exec_Seq {
    var_(gpa, mem_Alctr);
    var_(fiber_ctx, Co_Fiber_Context);
    var_(task_curr, O$P$exec_Seq_Task);
    var_(tasks, ArrList$P$exec_Seq_Task);
    var_(tasks_ready, ArrQue$P$exec_Seq_Task);
}));
$extern fn_((exec_Seq_init(mem_Alctr gpa))(exec_Seq));
$extern fn_((exec_Seq_fini(exec_Seq* self))(void));

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* exec_Seq__included */

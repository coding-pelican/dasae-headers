#ifndef exec_Coop__included
#define exec_Coop__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "common.h"
#include "dh/ArrList.h"
#include "dh/ArrPQue.h"
#include "dh/ArrQue.h"
#include "../time/Inst.h"
#include "../time/Clock.h"

/*========== Macros and Declarations ========================================*/

T_alias$((exec_Coop_Task_State)(enum_((exec_Coop_Task_State $fits($packed))(
    exec_Coop_Task_State_none = 0,
    exec_Coop_Task_State_deferred,
    exec_Coop_Task_State_ready,
    exec_Coop_Task_State_running,
    exec_Coop_Task_State_waiting,
    exec_Coop_Task_State_done,
    exec_Coop_Task_State_canceled,
))));
claim_assert_static(eqlType$(exec_Coop_Task_State, u8));
T_alias$((exec_Coop_Task)(struct exec_Coop_Task {
    var_(state, exec_Coop_Task_State);
    var_(result, u_P$raw);
    var_(inner, P$$(Closure$raw));
    var_(fiber, O$P$exec_Fiber);
}));
T_use_prl$(exec_Coop_Task);
T_use_ArrList$(P$exec_Coop_Task);
T_use_ArrQue$(P$exec_Coop_Task);
T_alias$((exec_Coop_Timer)(struct exec_Coop_Timer {
    var_(deadline, time_Inst);
    var_(task, P$exec_Coop_Task);
}));
T_use_prl$(exec_Coop_Timer);
T_use_ArrPQue$(exec_Coop_Timer);

T_alias$((exec_Coop)(struct exec_Coop {
    var_(gpa, mem_Alctr);
    var_(clock, time_Self);
    var_(fiber_ctx, Co_Fiber_Context);
    var_(task_curr, O$P$exec_Coop_Task);
    // TODO(heap): replace per-task allocation with heap_Pool once heap_Arena
    // free-list storage and heap_Pool are available.
    var_(tasks, ArrList$P$exec_Coop_Task);
    var_(tasks_ready, ArrQue$P$exec_Coop_Task);
    var_(tasks_timer, ArrPQue$exec_Coop_Timer);
}));
$extern fn_((exec_Coop_init(mem_Alctr gpa, time_Self clock))(exec_Coop));
$extern fn_((exec_Coop_fini(exec_Coop* self))(void));

$extern fn_((exec_Coop_task(exec_Coop* self))(O$P$exec_Coop_Task));
$extern fn_((exec_Coop_yield(exec_Coop* self))(void));
$extern fn_((exec_Coop_run(exec_Coop* self))(void));
$extern fn_((exec_Coop_runUntil(exec_Coop* self, time_Inst deadline))(void));

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* exec_Coop__included */

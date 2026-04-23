#ifndef exec_Lane__included
#define exec_Lane__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "common.h"

/*========== Macros and Declarations ========================================*/

T_alias$((exec_Lane)(struct exec_Lane {
    var_(gpa, mem_Alctr);
    var_(fiber_ctx, Co_Fiber_Context);
    var_(task_curr, O$P$exec_Task);
    var_(tasks, ArrList$P$exec_Task);
    var_(tasks_ready, ArrQue$P$exec_Task);
}));
$extern fn_((exec_Lane_init(mem_Alctr gpa))(exec_Lane));
$extern fn_((exec_Lane_fini(exec_Lane* self))(void));

$extern fn_((exec_Lane_task(exec_Lane* self))(O$P$exec_Task));

$extern fn_((exec_Lane_createReadyTask(exec_Lane* self, u_P$raw result, P$$(Closure$raw) inner))(O$P$exec_Task));
$extern fn_((exec_Lane_readyTask(exec_Lane* self, exec_Task* task))(bool));
$extern fn_((exec_Lane_asyncTask(exec_Lane* self, u_P$raw result, P$$(Closure$raw) inner))(O$P$exec_Task));
$extern fn_((exec_Lane_spawnTask(exec_Lane* self, u_P$raw result, P$$(Closure$raw) inner))(O$P$exec_Task));

$extern fn_((exec_Lane_runTask(exec_Lane* self, exec_Task* task))(void));
$extern fn_((exec_Lane_runOneReady(exec_Lane* self))(bool));
$extern fn_((exec_Lane_yield(exec_Lane* self))(void));

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* exec_Lane__included */

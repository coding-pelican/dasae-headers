#pragma once
#ifndef exec_Lane__included
#define exec_Lane__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "base.h"

/*========== Macros and Declarations ========================================*/

/* --- Types --- */

T_alias$((exec_Lane)(struct exec_Lane {
    var_(gpa, mem_Alctr);
    var_(fiber_context, co_Fiber_Context);
    var_(task_curr, O$P$exec_Task);
    var_(tasks, ArrList$P$exec_Task);
    var_(tasks_ready, ArrQue$P$exec_Task);
}));

/* --- Lifecycle --- */

$extern fn_((exec_Lane_init(mem_Alctr gpa))(exec_Lane));
$extern fn_((exec_Lane_fini(exec_Lane* self))(void));

/* --- Introspection --- */

$extern fn_((exec_Lane_task(exec_Lane* self))(O$P$exec_Task));

/* --- Task creation --- */

$extern fn_((exec_Lane_createReadyTask(exec_Lane* self, u_P$raw result, P$$(Clsr$raw) inner))(O$P$exec_Task));
$extern fn_((exec_Lane_readyTask(exec_Lane* self, exec_Task* task))(bool));
$extern fn_((exec_Lane_asyncTask(exec_Lane* self, u_P$raw result, P$$(Clsr$raw) inner))(O$P$exec_Task));
$extern fn_((exec_Lane_spawnTask(exec_Lane* self, u_P$raw result, P$$(Clsr$raw) inner))(O$P$exec_Task));

/* --- Run loop --- */

$extern fn_((exec_Lane_runTask(exec_Lane* self, exec_Task* task))(void));
$extern fn_((exec_Lane_runOneReady(exec_Lane* self))(bool));
$extern fn_((exec_Lane_yield(exec_Lane* self))(void));

/* --- Cooperative cancel --- */

/// Requeue `task` when a cooperative cancel was requested while it was `waiting`.
$extern fn_((exec_Lane_deliverCancel(exec_Lane* self, exec_Task* task))(void));

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* exec_Lane__included */

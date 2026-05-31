#ifndef exec_Coop__included
#define exec_Coop__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "LaneTimed.h"
#include "common.h"

/*========== Macros and Declarations ========================================*/

T_alias$((exec_Coop)(struct exec_Coop {
    var_(timed, exec_LaneTimed);
}));

/* --- Lifecycle --- */

/// Initialize a cooperative executor with an explicit awake clock.
$extern fn_((exec_Coop_init(mem_Alctr gpa, time_Awake clock))(exec_Coop));
/// Finalize a cooperative executor and release queued timer storage.
$extern fn_((exec_Coop_fini(exec_Coop* self))(void));

/* --- Introspection --- */

/// Return the currently running task, if the executor is inside a task.
$extern fn_((exec_Coop_task(exec_Coop* self))(O$P$exec_Task));

/* --- Run loop --- */

/// Run until no ready or timed tasks remain.
$extern fn_((exec_Coop_run(exec_Coop* self))(void));
/// Run until the given awake-clock deadline or until no work remains.
$extern fn_((exec_Coop_runUntil(exec_Coop* self, time_Awake_Inst deadline))(void));
/// Yield from the current cooperative task back to the executor lane.
$extern fn_((exec_Coop_yield(exec_Coop* self))(void));

/* --- Await --- */

/// Drive `task` until it reaches `exec_Task_State_done`.
$extern fn_((exec_Coop_awaitUntilDone(exec_Coop* self, exec_Task* task))(void));

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* exec_Coop__included */

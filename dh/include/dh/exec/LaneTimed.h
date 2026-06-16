#pragma once
#ifndef exec_LaneTimed__included
#define exec_LaneTimed__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "Lane.h"
#include "Timer.h"
#include "../time/self/Awake.h"

/*========== Macros and Declarations ========================================*/

/* --- Types --- */

T_alias$((exec_LaneTimed)(struct exec_LaneTimed {
    var_(lane, exec_Lane);
    var_(clock, time_Awake);
    /// Non-evented clock for executor pumps (`exec_Coop_awaitUntilDone`); not a cancel point.
    var_(clock_pump, time_Awake);
    var_(tasks_timer, ArrPQue$exec_Timer);
}));

/* --- Lifecycle --- */

$extern fn_((exec_LaneTimed_init(mem_Alctr gpa, time_Awake clock))(exec_LaneTimed));
$extern fn_((exec_LaneTimed_fini(exec_LaneTimed* self))(void));

/* --- Timer queue --- */

/// Return the earliest queued timer deadline, if any.
$extern fn_((exec_LaneTimed_nextTimerDeadline(exec_LaneTimed* self))(O$time_Awake_Inst));
/// Return remaining duration until a deadline, saturating elapsed deadlines to zero.
$extern fn_((exec_LaneTimed_remaining(exec_LaneTimed* self, time_Awake_Inst deadline))(time_Dur));
/// Move all due timer tasks back to the ready queue.
$extern fn_((exec_LaneTimed_wakeDueTimers(exec_LaneTimed* self))(void));

/* --- Run loop --- */

/// Run ready and timed tasks until no timed work remains.
$extern fn_((exec_LaneTimed_run(exec_LaneTimed* self))(void));
/// Run ready and timed tasks until a deadline.
$extern fn_((exec_LaneTimed_runUntil(exec_LaneTimed* self, time_Awake_Inst deadline))(void));

/* --- Cooperative cancel --- */

/// Remove timers for `task` and requeue it when parked in `waiting`.
$extern fn_((exec_LaneTimed_deliverCancel(exec_LaneTimed* self, exec_Task* task))(void));

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* exec_LaneTimed__included */

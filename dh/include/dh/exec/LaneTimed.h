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

struct exec_LaneTimed {
    var_(lane, exec_Lane);
    var_(clock, time_Awake);
    var_(tasks_timer, ArrPQue$exec_Timer);
};
/// Initialize a timed lane with the awake clock used for deadlines and sleeps.
$extern fn_((exec_LaneTimed_init(mem_Alctr gpa, time_Awake clock))(exec_LaneTimed));
/// Finalize the timed lane and release queued timer storage.
$extern fn_((exec_LaneTimed_fini(exec_LaneTimed* self))(void));

/// Return the earliest queued timer deadline, if any.
$extern fn_((exec_LaneTimed_nextTimerDeadline(exec_LaneTimed* self))(O$time_Awake_Inst));
/// Return remaining duration until a deadline, saturating elapsed deadlines to zero.
$extern fn_((exec_LaneTimed_remaining(exec_LaneTimed* self, time_Awake_Inst deadline))(time_Dur));
/// Move all due timer tasks back to the ready queue.
$extern fn_((exec_LaneTimed_wakeDueTimers(exec_LaneTimed* self))(void));
/// Run ready and timed tasks until a deadline.
$extern fn_((exec_LaneTimed_runUntil(exec_LaneTimed* self, time_Awake_Inst deadline))(void));
/// Run ready and timed tasks until no timed work remains.
$extern fn_((exec_LaneTimed_run(exec_LaneTimed* self))(void));

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* exec_LaneTimed__included */

#ifndef exec_LaneTimed__included
#define exec_LaneTimed__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "Lane.h"
#include "Timer.h"
#include "../time/self.h"

/*========== Macros and Declarations ========================================*/

struct exec_LaneTimed {
    var_(lane, exec_Lane);
    var_(clock, time_Awake);
    var_(tasks_timer, ArrPQue$exec_Timer);
};
$extern fn_((exec_LaneTimed_init(mem_Alctr gpa, time_Awake clock))(exec_LaneTimed));
$extern fn_((exec_LaneTimed_fini(exec_LaneTimed* self))(void));

$extern fn_((exec_LaneTimed_nextTimerDeadline(exec_LaneTimed* self))(O$time_Awake_Inst));
$extern fn_((exec_LaneTimed_remaining(exec_LaneTimed* self, time_Awake_Inst deadline))(time_Dur));
$extern fn_((exec_LaneTimed_wakeDueTimers(exec_LaneTimed* self))(void));
$extern fn_((exec_LaneTimed_runUntil(exec_LaneTimed* self, time_Awake_Inst deadline))(void));
$extern fn_((exec_LaneTimed_run(exec_LaneTimed* self))(void));

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* exec_LaneTimed__included */

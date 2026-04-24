#ifndef exec_Coop__included
#define exec_Coop__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "LaneTimed.h"

/*========== Macros and Declarations ========================================*/

T_alias$((exec_Coop)(struct exec_Coop {
    var_(timed, exec_LaneTimed);
}));
$extern fn_((exec_Coop_init(mem_Alctr gpa, time_Awake clock))(exec_Coop));
$extern fn_((exec_Coop_fini(exec_Coop* self))(void));

$extern fn_((exec_Coop_task(exec_Coop* self))(O$P$exec_Task));
$extern fn_((exec_Coop_yield(exec_Coop* self))(void));
$extern fn_((exec_Coop_run(exec_Coop* self))(void));
$extern fn_((exec_Coop_runUntil(exec_Coop* self, time_Awake_Inst deadline))(void));

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* exec_Coop__included */

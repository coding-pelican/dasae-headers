#ifndef exec_Preem__included
#define exec_Preem__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "common.h"
#include "../Thrd.h"

/*========== Macros and Declarations ========================================*/

T_alias$((exec_Preem_Task)(union exec_Preem_Task));
T_use_P$(exec_Preem_Task);
T_use_E$($set(Sched_ConcE)(P$exec_Preem_Task));
T_use_O$(P$exec_Preem_Task);
$extern fn_((exec_Preem_work(exec_Preem_Task* task))(Void));
fn_use_Closure_((exec_Preem_work)(exec_Preem_Task*)(Void));
union exec_Preem_Task {
    T_embed$(struct {
        var_(next, O$P$exec_Preem_Task);
        var_(owner, exec_Preem*);
        var_(thrd, Thrd);
        var_(result, u_P$raw);
        var_(inner, P$$(Closure$raw));
        var_(state, exec_Task_State);
        var_(runner, Closure_(exec_Preem_work));
    });
    var_(as_any, FutureAny) $flexible;
};

T_alias$((exec_Preem)(struct exec_Preem {
    var_(spawn_cfg, Thrd_SpawnCfg);
    var_(tasks_mtx, Thrd_Mtx);
    var_(tasks, O$P$exec_Preem_Task);
}));
$extern fn_((exec_Preem_init(mem_Alctr gpa))(exec_Preem));
$extern fn_((exec_Preem_fini(exec_Preem* self))(void));

$attr($must_check)
$extern fn_((exec_Preem_createTask(exec_Preem* self, u_P$raw result, P$$(Closure$raw) inner))(Sched_ConcE$P$exec_Preem_Task));
$extern fn_((exec_Preem_destroyTask(exec_Preem* self, exec_Preem_Task* task))(void));
$extern fn_((exec_Preem_linkTask(exec_Preem* self, exec_Preem_Task* task))(void));
$extern fn_((exec_Preem_unlinkTask(exec_Preem* self, exec_Preem_Task* task))(void));

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* exec_Preem__included */

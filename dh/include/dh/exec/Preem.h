#pragma once
#ifndef exec_Preem__included
#define exec_Preem__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "common.h"
#include "../atom.h"
#include "../thrd/Self.h"
#include "../thrd/Mtx.h"

/*========== Macros and Declarations ========================================*/

/* --- Types --- */

T_alias$((exec_Preem_Task)(union exec_Preem_Task));
T_use_P$(exec_Preem_Task);
T_use_E$($set(Sched_ConcE)(P$exec_Preem_Task));
T_use_O$(P$exec_Preem_Task);

$extern fn_((exec_Preem_work(exec_Preem_Task* task))(Void));
fn_use_Clsr_((exec_Preem_work)(exec_Preem_Task*)(Void));

union exec_Preem_Task {
    T_embed$(struct {
        var_(next, O$P$exec_Preem_Task);
        var_(owner, exec_Preem*);
        var_(thrd, thrd_Self);
        var_(result, u_P$raw);
        var_(inner, P$$(Clsr$raw));
        var_(state, atom_V$u8);
        var_(runner, Clsr_(exec_Preem_work));
    });
    var_(as_any, FutureAny) $flexible;
};

T_alias$((exec_Preem)(struct exec_Preem {
    var_(spawn_cfg, thrd_SpawnCfg);
    var_(tasks_mtx, thrd_Mtx);
    var_(tasks, O$P$exec_Preem_Task);
}));

/* --- Lifecycle --- */

$extern fn_((exec_Preem_init(mem_Alctr gpa))(exec_Preem));
$extern fn_((exec_Preem_fini(exec_Preem* self))(void));

/* --- Task registry --- */

$attr($must_check)
$extern fn_((exec_Preem_createTask(exec_Preem* self, u_P$raw result, P$$(Clsr$raw) inner))(Sched_ConcE$P$exec_Preem_Task));
$extern fn_((exec_Preem_destroyTask(exec_Preem* self, exec_Preem_Task* task))(void));
$extern fn_((exec_Preem_linkTask(exec_Preem* self, exec_Preem_Task* task))(void));
$extern fn_((exec_Preem_unlinkTask(exec_Preem* self, exec_Preem_Task* task))(void));
$extern fn_((exec_Preem_Task_state(exec_Preem_Task* self))(exec_Task_State));
$extern fn_((exec_Preem_Task_requestCancel(exec_Preem_Task* self))(exec_Task_State));

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* exec_Preem__included */

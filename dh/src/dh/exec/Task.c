#include "dh/exec/Task.h"

fn_((exec_Task_isDone(P_const$exec_Task task))(bool)) {
    claim_assert_nonnull(task);
    return task->state == exec_Task_State_done;
};

fn_((exec_Task_copyToResult(P_const$exec_Task task, u_P$raw result))(void)) {
    claim_assert_nonnull(task), claim_assert_nonnull(result.raw);
    claim_assert(exec_Task_isDone(task));
    u_memcpy(result, task->result.as_const);
};

fn_((exec_Task_hasCancelRequest(P_const$exec_Task task))(bool)) {
    claim_assert_nonnull(task);
    return task->cancel == exec_Task_Cancel_requested;
};

fn_((exec_Task_requestCancel(exec_Task* task))(void)) {
    claim_assert_nonnull(task);
    if (exec_Task_isDone(task)) return;
    if (task->cancel != exec_Task_Cancel_none) return;
    task->cancel = exec_Task_Cancel_requested;
};

fn_((exec_Task_recancel(exec_Task* task))(void)) {
    claim_assert_nonnull(task);
    claim_assert(task->cancel == exec_Task_Cancel_acknowledged);
    task->cancel = exec_Task_Cancel_requested;
};

fn_((exec_Task_swapCancelProtcn(exec_Task* task, Sched_CancelProtcn new_protection))(Sched_CancelProtcn)) {
    claim_assert_nonnull(task);
    let old = task->cancel_protection;
    task->cancel_protection = new_protection;
    return old;
};

fn_((exec_Task_idle(exec_Task* task))(Sched_Cancelable$void) $scope) {
    claim_assert_nonnull(task);
    if (task->cancel_protection == Sched_CancelProtcn_blocked) return_ok({});
    if (task->cancel == exec_Task_Cancel_requested) {
        task->cancel = exec_Task_Cancel_acknowledged;
        return_err(E_cause$Sched_Canceled());
    }
    return_ok({});
} $unscoped(fn);

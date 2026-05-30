#include "Task.h"

fn_((exec_Task_isDone(P_const$exec_Task task))(bool)) {
    claim_assert_nonnull(task);
    return task->state == exec_Task_State_done;
};

fn_((exec_Task_copyToResult(P_const$exec_Task task, u_P$raw result))(void)) {
    claim_assert_nonnull(task), claim_assert_nonnull(result.raw);
    claim_assert(exec_Task_isDone(task));
    u_memcpy(result, task->result.as_const);
};

fn_((exec_Task_cancel(exec_Task* task))(void)) {
    claim_assert_nonnull(task);
    if (exec_Task_isDone(task)) return;
    task->state = exec_Task_State_canceled;
};

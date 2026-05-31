#include "dh/exec/Seq.h"
#include "dh/exec/Task.h"

fn_((exec_Seq_init(mem_Alctr gpa))(exec_Seq)) {
    return (exec_Seq){
        .lane = exec_Lane_init(gpa),
    };
};

fn_((exec_Seq_fini(exec_Seq* self))(void)) {
    claim_assert_nonnull(self);
    exec_Lane_fini(&self->lane);
    asg_l((self)(cleared()));
};

fn_((exec_Seq_task(exec_Seq* self))(O$P$exec_Task)) {
    claim_assert_nonnull(self);
    return exec_Lane_task(&self->lane);
};

fn_((exec_Seq_awaitUntilDone(exec_Seq* self, exec_Task* task))(void)) {
    claim_assert_nonnull(self), claim_assert_nonnull(task);
    while (!exec_Task_isDone(task)) {
        if (task->state == exec_Task_State_deferred) {
            $continue_(exec_Lane_runTask(&self->lane, task));
        }
        if (exec_Lane_runOneReady(&self->lane)) continue;
        if (task->state == exec_Task_State_waiting) {
            claim_unreachable_msg("`exec_Seq` cannot drive waiting tasks; use `exec_Coop` for evented suspension");
        }
        exec_Lane_runTask(&self->lane, task);
    }
};

#include "dh/exec/Coop.h"
#include "dh/exec/Task.h"
#include "dh/time/self/Awake.h"

/*========== Internal Declarations ==========================================*/

$static fn_((exec_Coop__pumpUntil(exec_Coop* self, O$time_Awake_Inst deadline))(bool));
$static fn_((exec_Coop__pumpCompletion(exec_Coop* self, time_Dur timeout))(bool));

/*========== External Definitions ===========================================*/

fn_((exec_Coop_init(mem_Alctr gpa, time_Awake clock))(exec_Coop)) {
    return (exec_Coop){
        .timed = exec_LaneTimed_init(gpa, clock),
    };
};

fn_((exec_Coop_fini(exec_Coop* self))(void)) {
    claim_assert_nonnull(self);
    exec_LaneTimed_fini(&self->timed);
    asg_l((self)(cleared()));
};

fn_((exec_Coop_task(exec_Coop* self))(O$P$exec_Task)) {
    claim_assert_nonnull(self);
    return exec_Lane_task(&self->timed.lane);
};

fn_((exec_Coop_run(exec_Coop* self))(void)) {
    claim_assert_nonnull(self);
    while (true) {
        exec_LaneTimed_wakeDueTimers(&self->timed);
        if (exec_Lane_runOneReady(&self->timed.lane)) continue;
        if (exec_Coop__pumpUntil(self, none$((O$time_Awake_Inst)))) continue;
        break;
    }
};

fn_((exec_Coop_runUntil(exec_Coop* self, time_Awake_Inst deadline))(void)) {
    claim_assert_nonnull(self);
    while (cmp_lt$(time_Awake_Inst)(time_Awake_now(self->timed.clock), deadline)) {
        exec_LaneTimed_wakeDueTimers(&self->timed);
        if (exec_Lane_runOneReady(&self->timed.lane)) continue;
        if (exec_Coop__pumpUntil(self, some$((O$time_Awake_Inst)(deadline)))) continue;
        break;
    }
    exec_LaneTimed_wakeDueTimers(&self->timed);
};

fn_((exec_Coop_yield(exec_Coop* self))(void)) {
    claim_assert_nonnull(self);
    exec_Lane_yield(&self->timed.lane);
};

fn_((exec_Coop_awaitUntilDone(exec_Coop* self, exec_Task* task))(void)) {
    claim_assert_nonnull(self), claim_assert_nonnull(task);
    while (!exec_Task_isDone(task)) {
        exec_LaneTimed_wakeDueTimers(&self->timed);
        if (task->state == exec_Task_State_deferred) {
            $continue_(exec_Lane_runTask(&self->timed.lane, task));
        }
        if (exec_Lane_runOneReady(&self->timed.lane)) continue;
        let deadline = orelse_((exec_LaneTimed_nextTimerDeadline(&self->timed))(
            if (exec_Task_isDone(task)) break;
            else return;
        ));
        let remaining = exec_LaneTimed_remaining(&self->timed, deadline);
        if (time_Dur_isZero(remaining)) continue;
        catch_((time_Awake_sleep(self->timed.clock_pump, remaining))($ignore, $do_nothing));
    }
};

/*========== Internal Definitions ===========================================*/

fn_((exec_Coop__pumpUntil(exec_Coop* self, O$time_Awake_Inst deadline))(bool)) {
    claim_assert_nonnull(self);
    let next_deadline = orelse_((exec_LaneTimed_nextTimerDeadline(&self->timed))(
        if_none((deadline)) return false;
        else_some(run_deadline) return exec_Coop__pumpCompletion(
            self, exec_LaneTimed_remaining(&self->timed, run_deadline)
        );
    ));
    var timeout = exec_LaneTimed_remaining(&self->timed, next_deadline);
    if (time_Dur_isZero(timeout)) return true;
    if_some((deadline)(run_deadline)) {
        let remaining = exec_LaneTimed_remaining(&self->timed, run_deadline);
        if (time_Dur_isZero(remaining)) return false;
        if (cmp_lt$(time_Dur)(remaining, timeout)) timeout = remaining;
    }
    return exec_Coop__pumpCompletion(self, timeout);
};

fn_((exec_Coop__pumpCompletion(exec_Coop* self, time_Dur timeout))(bool)) {
    claim_assert_nonnull(self);
    if (time_Dur_isZero(timeout)) return false;
    catch_((time_Awake_sleep(self->timed.clock_pump, timeout))($ignore, $do_nothing));
    return true;
};

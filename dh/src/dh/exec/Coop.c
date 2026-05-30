#include "dh/exec/Coop.h"
#include "dh/time/self/Awake.h"

$static fn_((exec_Coop__pumpCompletion(exec_Coop* self, time_Dur timeout))(bool));
$static fn_((exec_Coop__pumpUntil(exec_Coop* self, O$time_Awake_Inst deadline))(bool));

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

fn_((exec_Coop_yield(exec_Coop* self))(void)) {
    claim_assert_nonnull(self);
    exec_Lane_yield(&self->timed.lane);
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

fn_((exec_Coop__pumpCompletion(exec_Coop* self, time_Dur timeout))(bool)) {
    claim_assert_nonnull(self);
    if (time_Dur_isZero(timeout)) return false;
    catch_((time_Awake_sleep(self->timed.clock, timeout))($ignore, $do_nothing));
    return true;
};

fn_((exec_Coop__pumpUntil(exec_Coop* self, O$time_Awake_Inst deadline))(bool)) {
    claim_assert_nonnull(self);
    var timeout = time_Dur_fromSecs(4294968);
    if_some((exec_LaneTimed_nextTimerDeadline(&self->timed))(next_deadline)) {
        timeout = exec_LaneTimed_remaining(&self->timed, next_deadline);
        if (time_Dur_isZero(timeout)) return false;
    }
    if_some((deadline)(run_deadline)) {
        let remaining = exec_LaneTimed_remaining(&self->timed, run_deadline);
        if (time_Dur_isZero(remaining)) return false;
        if (cmp_lt$(time_Dur)(remaining, timeout)) timeout = remaining;
    }
    return exec_Coop__pumpCompletion(self, timeout);
};

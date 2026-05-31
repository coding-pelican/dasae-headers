#include "dh/exec/LaneTimed.h"
#include "dh/exec/Task.h"
#include "dh/time/self/Awake.h"

/*========== Internal Declarations ==========================================*/

$static fn_((exec_LaneTimed__timerCtx(void))(P_const$ArrPQue_Ctx));
$static fn_((exec_LaneTimed__now(exec_LaneTimed* self))(time_Awake_Inst));

/*========== External Definitions ===========================================*/

T_use$((exec_Timer)(ArrPQue_empty, ArrPQue_len, ArrPQue_at, ArrPQue_removeAt));
fn_((exec_LaneTimed_init(mem_Alctr gpa, time_Awake clock))(exec_LaneTimed)) {
    let clock_pump = catch_((time_Awake_direct())($ignore, clock));
    return (exec_LaneTimed){
        .lane = exec_Lane_init(gpa),
        .clock = clock,
        .clock_pump = clock_pump,
        .tasks_timer = ArrPQue_empty$exec_Timer(exec_LaneTimed__timerCtx()),
    };
};

T_use$((exec_Timer)(ArrPQue_fini));
fn_((exec_LaneTimed_fini(exec_LaneTimed* self))(void)) {
    claim_assert_nonnull(self);
    ArrPQue_fini$exec_Timer(&self->tasks_timer, self->lane.gpa);
    exec_Lane_fini(&self->lane);
    asg_l((self)(cleared()));
};

T_use$((exec_Timer)(ArrPQue_peek));
fn_((exec_LaneTimed_nextTimerDeadline(exec_LaneTimed* self))(O$time_Awake_Inst) $scope) {
    claim_assert_nonnull(self);
    let next = orelse_((ArrPQue_peek$exec_Timer(self->tasks_timer))(return_none()));
    return_some({ .raw = next->deadline });
} $unscoped(fn);

fn_((exec_LaneTimed_remaining(exec_LaneTimed* self, time_Awake_Inst deadline))(time_Dur)) {
    claim_assert_nonnull(self);
    let now = exec_LaneTimed__now(self);
    return orelse_((time_Awake_Inst_durSinceChkd(deadline, now))(time_Dur_zero));
};

T_use$((exec_Timer)(ArrPQue_isEmpty, ArrPQue_deque));
fn_((exec_LaneTimed_wakeDueTimers(exec_LaneTimed* self))(void)) {
    claim_assert_nonnull(self);
    let now = exec_LaneTimed__now(self);
    while (!ArrPQue_isEmpty$exec_Timer(self->tasks_timer)) {
        let next = orelse_((ArrPQue_peek$exec_Timer(self->tasks_timer))(break));
        if (cmp_gt$(time_Inst)(next->deadline, now.raw)) break;
        let timer = orelse_((ArrPQue_deque$exec_Timer(&self->tasks_timer))(break));
        if (timer.task->state == exec_Task_State_waiting) {
            let_ignore = exec_Lane_readyTask(&self->lane, timer.task);
        }
    }
};

fn_((exec_LaneTimed_run(exec_LaneTimed* self))(void)) {
    claim_assert_nonnull(self);
    while (true) {
        exec_LaneTimed_wakeDueTimers(self);
        if (exec_Lane_runOneReady(&self->lane)) continue;
        let deadline = orelse_((exec_LaneTimed_nextTimerDeadline(self))(break));
        let remaining = exec_LaneTimed_remaining(self, deadline);
        if (time_Dur_isZero(remaining)) continue;
        catch_((time_Awake_sleep(self->clock_pump, remaining))($ignore, $do_nothing));
    }
};

fn_((exec_LaneTimed_runUntil(exec_LaneTimed* self, time_Awake_Inst deadline))(void)) {
    claim_assert_nonnull(self);
    while (cmp_lt$(time_Awake_Inst)(exec_LaneTimed__now(self), deadline)) {
        exec_LaneTimed_wakeDueTimers(self);
        if (exec_Lane_runOneReady(&self->lane)) continue;
        var wait_deadline = deadline;
        if_some((exec_LaneTimed_nextTimerDeadline(self))(next_deadline)) {
            if (cmp_lt$(time_Awake_Inst)(next_deadline, wait_deadline)) {
                wait_deadline = next_deadline;
            }
        }
        let remaining = exec_LaneTimed_remaining(self, wait_deadline);
        if (time_Dur_isZero(remaining)) break;
        catch_((time_Awake_sleep(self->clock_pump, remaining))($ignore, $do_nothing));
    }
    exec_LaneTimed_wakeDueTimers(self);
};

$static fn_((exec_LaneTimed__removeTimersForTask(exec_LaneTimed* self, exec_Task* task))(void));
fn_((exec_LaneTimed_deliverCancel(exec_LaneTimed* self, exec_Task* task))(void)) {
    claim_assert_nonnull(self), claim_assert_nonnull(task);
    exec_LaneTimed__removeTimersForTask(self, task);
    if (task->state == exec_Task_State_waiting) {
        let_ignore = exec_Lane_readyTask(&self->lane, task);
    }
};
fn_((exec_LaneTimed__removeTimersForTask(exec_LaneTimed* self, exec_Task* task))(void)) {
    claim_assert_nonnull(self), claim_assert_nonnull(task);
    let len = ArrPQue_len$exec_Timer(self->tasks_timer);
    var_(i, usize) = len;
    while (i > 0) {
        i -= 1;
        let entry = ArrPQue_at$exec_Timer(self->tasks_timer, i);
        if (entry->task != task) continue;
        let_ignore = ArrPQue_removeAt$exec_Timer(&self->tasks_timer, i);
    }
};

/*========== Internal Definitions ===========================================*/

$static cmp_fn_ord$((exec_Timer)(lhs, rhs)) {
    return cmp_ord$(time_Inst)(lhs.deadline, rhs.deadline);
};
$static cmp_fn_ordCtx$((exec_Timer)(lhs, rhs, ctx)) {
    return $ignore_void ctx, cmp_ord$(exec_Timer)(lhs, rhs);
};
$static cmp_fn_u_ordCtx_default$((exec_Timer)(lhs, rhs, ctx));
fn_((exec_LaneTimed__timerCtx(void))(P_const$ArrPQue_Ctx)) {
    $static let_(inner, Void) = cleared();
    $static let_(ctx, ArrPQue_Ctx) = {
        .inner = u_anyP(&inner),
        .ordFn = cmp_u_ordCtx$(exec_Timer),
    };
    return &ctx;
};

fn_((exec_LaneTimed__now(exec_LaneTimed* self))(time_Awake_Inst)) {
    return time_Awake_now(self->clock);
};

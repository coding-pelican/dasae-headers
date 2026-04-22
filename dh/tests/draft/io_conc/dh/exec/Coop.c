#include "Coop.h"
#include "common.h"
#include "dh/prl/types/Opt.h"

T_use_E$($set(mem_E)(P$P$exec_Coop_Task));
T_use_E$($set(mem_E)(P$exec_Coop_Task));
T_use$((exec_Coop_Task)(mem_Alctr_create, mem_Alctr_destroy));
T_use$((P$exec_Coop_Task)(
    ArrQue_empty,
    ArrQue_fini,
    ArrQue_enque,
    ArrQue_deque,
    ArrList_empty,
    ArrList_fini,
    ArrList_addBack
));
T_use$((exec_Coop_Timer)(
    ArrPQue_empty,
    ArrPQue_fini,
    ArrPQue_isEmpty,
    ArrPQue_peek,
    ArrPQue_deque
));

$static fn_((exec_Coop__workFiber(P$raw owner, P$raw any_task))(void)) {
    let self = ptrAlignCast$((exec_Coop*)(ensureNonnull(owner)));
    let task = ptrAlignCast$((exec_Coop_Task*)(ensureNonnull(any_task)));
    exec_invokeToCompletion(task->inner, task->result);
    if (task->state != exec_Coop_Task_State_canceled) task->state = exec_Coop_Task_State_done;
    let fiber = orelse_((task->fiber)(claim_unreachable));
    exec_switchFromFiber(&fiber->ctx, &self->fiber_ctx);
    claim_unreachable;
};

$static cmp_fn_ord$((exec_Coop_Timer)(lhs, rhs)) {
    return cmp_ord$(time_Inst)(lhs.deadline, rhs.deadline);
};
$static cmp_fn_ordCtx$((exec_Coop_Timer)(lhs, rhs, ctx)) {
    return $ignore_void ctx, cmp_ord$(exec_Coop_Timer)(lhs, rhs);
};
$static cmp_fn_u_ordCtx_default$((exec_Coop_Timer)(lhs, rhs, ctx));
$static fn_((exec_Coop__timerCtx(void))(P_const$ArrPQue_Ctx)) {
    $static let_(inner, Void) $like_ref = cleared();
    $static let_(ctx, ArrPQue_Ctx) $like_ref = { {
        .inner = u_anyP(inner),
        .ordFn = cmp_u_ordCtx$(exec_Coop_Timer),
    } };
    return ctx;
};

$static fn_((exec_Coop__now(exec_Coop* self))(time_Inst)) {
    return self->clock.vtbl->nowInstFn(self->clock.ctx);
};

$static fn_((exec_Coop__remaining(exec_Coop* self, time_Inst deadline))(time_Dur)) {
    let now = exec_Coop__now(self);
    return orelse_((time_Inst_durSinceChkd(deadline, now))(time_Dur_zero));
};

$static fn_((exec_Coop__nextTimerDeadline(exec_Coop* self))(O$time_Inst) $scope) {
    let next = orelse_((ArrPQue_peek$exec_Coop_Timer(self->tasks_timer))(return_none()));
    return_some(next->deadline);
} $unscoped(fn);

$static fn_((exec_Coop__createTask(
    exec_Coop* self,
    exec_Coop_Task_State state,
    u_P$raw result,
    P$$(Closure$raw) inner
))(E$P$exec_Coop_Task) $guard) {
    let task = try_(mem_Alctr_create$exec_Coop_Task($trace self->gpa));
    errdefer_($ignore, mem_Alctr_destroy$exec_Coop_Task($trace self->gpa, task));
    return_ok(asg_l((task)({
        .state = state,
        .result = result,
        .inner = inner,
        .fiber = none(),
    })));
} $unguarded(fn);

$static fn_((exec_Coop__destroyTask(exec_Coop* self, exec_Coop_Task* task))(void)) {
    claim_assert_nonnull(self), claim_assert_nonnull(task);
    if_some((task->fiber)(fiber)) exec_destroyFiber(self->gpa, fiber);
    mem_Alctr_destroy($trace self->gpa, u_anyP(task));
};

$static fn_((exec_Coop__enqueReady(exec_Coop* self, exec_Coop_Task* task))(bool)) {
    task->state = exec_Coop_Task_State_ready;
    if (isOk(ArrQue_enque$P$exec_Coop_Task(&self->tasks_ready, self->gpa, task))) return true;
    task->state = exec_Coop_Task_State_canceled;
    return false;
};

$attr($must_check)
$static fn_((exec_Coop__storeTask(
    exec_Coop* self,
    exec_Coop_Task_State state,
    u_P$raw result,
    P$$(Closure$raw) inner
))(E$P$exec_Coop_Task) $guard) {
    claim_assert_nonnull(self), claim_assert_nonnull(result.raw), claim_assert_nonnull(inner);
    let task = try_(exec_Coop__createTask(self, state, result, inner));
    errdefer_($ignore, exec_Coop__destroyTask(self, task));
    if (exec_kind(inner) == exec_Task_Kind_fiber) {
        asg_l((&task->fiber)(some(try_(exec_createFiber(self->gpa, self, task, exec_Coop__workFiber)))));
    }
    return_ok(*try_(ArrList_addBack$P$exec_Coop_Task(&self->tasks, self->gpa)) = task);
} $unguarded(fn);

$static fn_((exec_Coop__createReadyTask(
    exec_Coop* self,
    u_P$raw result,
    P$$(Closure$raw) inner
))(O$P$exec_Coop_Task) $scope) {
    let task = catch_((exec_Coop__storeTask(self, exec_Coop_Task_State_none, result, inner))($ignore, return_none()));
    if (!exec_Coop__enqueReady(self, task)) {
        task->state = exec_Coop_Task_State_canceled;
        return_none();
    }
    return_some(task);
} $unscoped(fn);

$static fn_((exec_Coop__runTask(exec_Coop* self, exec_Coop_Task* task))(void)) {
    if (task->state == exec_Coop_Task_State_canceled || task->state == exec_Coop_Task_State_done) return;
    if (exec_kind(task->inner) == exec_Task_Kind_fiber) {
        task->state = exec_Coop_Task_State_running;
        let prev = self->task_curr;
        asg_l((&self->task_curr)(some(task)));
        let fiber = orelse_((task->fiber)(claim_unreachable));
        exec_switchToFiber(&self->fiber_ctx, fiber);
        asg_l((&self->task_curr)(prev));
        return;
    }
    let prev = self->task_curr;
    asg_l((&self->task_curr)(some(task)));
    task->state = exec_Coop_Task_State_running;
    let done = exec_invokeStep(task->inner, task->result);
    if (task->state == exec_Coop_Task_State_running) {
        if (done) {
            task->state = exec_Coop_Task_State_done;
        } else {
            let_ignore = exec_Coop__enqueReady(self, task);
        }
    }
    asg_l((&self->task_curr)(prev));
};

$static fn_((exec_Coop__runOneReady(exec_Coop* self))(bool)) {
    let task = orelse_((ArrQue_deque$P$exec_Coop_Task(&self->tasks_ready))(return false));
    exec_Coop__runTask(self, task);
    return true;
};

$static fn_((exec_Coop__wakeDueTimers(exec_Coop* self))(void)) {
    let now = exec_Coop__now(self);
    while (!ArrPQue_isEmpty$exec_Coop_Timer(self->tasks_timer)) {
        let next = orelse_((ArrPQue_peek$exec_Coop_Timer(self->tasks_timer))(break));
        if (cmp_gt$(time_Inst)(next->deadline, now)) break;
        let timer = orelse_((ArrPQue_deque$exec_Coop_Timer(&self->tasks_timer))(break));
        if (timer.task->state == exec_Coop_Task_State_waiting) {
            let_ignore = exec_Coop__enqueReady(self, timer.task);
        }
    }
};

fn_((exec_Coop_init(mem_Alctr gpa, time_Self clock))(exec_Coop)) {
    return (exec_Coop){
        .gpa = mem_Alctr_ensureValid(gpa),
        .clock = clock,
        .fiber_ctx = {},
        .task_curr = none(),
        .tasks = ArrList_empty$P$exec_Coop_Task(),
        .tasks_ready = ArrQue_empty$P$exec_Coop_Task(),
        .tasks_timer = ArrPQue_empty$exec_Coop_Timer(exec_Coop__timerCtx()),
    };
};

fn_((exec_Coop_fini(exec_Coop* self))(void)) {
    for_(($s(self->tasks.items))(task)) exec_Coop__destroyTask(self, *task) $end(for);
    ArrPQue_fini$exec_Coop_Timer(&self->tasks_timer, self->gpa);
    ArrQue_fini$P$exec_Coop_Task(&self->tasks_ready, self->gpa);
    ArrList_fini$P$exec_Coop_Task(&self->tasks, self->gpa);
    asg_l((self)(cleared()));
};

$static fn_((exec_Coop__Sched_async(P$raw ctx, u_P$raw result, P$$(Closure$raw) inner))(O$P$Future$raw) $scope) {
    let self = as$(exec_Coop*)(ctx);
    let task = orelse_((exec_Coop__createReadyTask(self, result, inner))({
        return_none(exec_invokeToCompletion(inner, result));
    }));
    return_some(as$(P$Future$raw)(task));
} $unscoped(fn);

$static fn_((exec_Coop__Sched_spawn(P$raw ctx, u_P$raw result, P$$(Closure$raw) inner))(Sched_ConcE$P$Future$raw) $scope) {
    let self = as$(exec_Coop*)(ctx);
    let task = orelse_((exec_Coop__createReadyTask(self, result, inner))(
        return_err(Sched_ConcE_Unavailable())
    ));
    return_ok(as$(P$Future$raw)(task));
} $unscoped(fn);

$static fn_((exec_Coop__Sched_await(P$raw ctx, P$Future$raw any_future, u_P$raw result))(void)) {
    let self = as$(exec_Coop*)(ctx);
    let task = as$(exec_Coop_Task*)(any_future);
    while (task->state != exec_Coop_Task_State_done && task->state != exec_Coop_Task_State_canceled) {
        exec_Coop__wakeDueTimers(self);
        if (task->state == exec_Coop_Task_State_deferred) {
            exec_Coop__runTask(self, task);
            continue;
        }
        if (exec_Coop__runOneReady(self)) continue;
        let deadline = orelse_((exec_Coop__nextTimerDeadline(self))(break));
        let remaining = exec_Coop__remaining(self, deadline);
        if (time_Dur_isZero(remaining)) continue;
        let_ignore = self->clock.vtbl->sleepFn(self->clock.ctx, remaining);
    }
    if (task->state == exec_Coop_Task_State_done) u_memcpy(result, task->result.as_const);
};

$static fn_((exec_Coop__Sched_cancel(P$raw ctx, P$Future$raw any_future, u_P$raw result))(void)) {
    let_ignore = ctx;
    let task = as$(exec_Coop_Task*)(any_future);
    if (task->state != exec_Coop_Task_State_done) task->state = exec_Coop_Task_State_canceled;
    if (task->state == exec_Coop_Task_State_done) u_memcpy(result, task->result.as_const);
};

fn_((Sched_coop(exec_Coop* loop))(Sched)) {
    static const Sched_VTbl vtbl $like_ref = { {
        .asyncFn = exec_Coop__Sched_async,
        .spawnFn = exec_Coop__Sched_spawn,
        .awaitFn = exec_Coop__Sched_await,
        .cancelFn = exec_Coop__Sched_cancel,
    } };
    return (Sched){
        .ctx = loop,
        .vtbl = vtbl,
    };
};

fn_((exec_Coop_task(exec_Coop* self))(O$P$exec_Coop_Task)) {
    return self->task_curr;
};

fn_((exec_Coop_yield(exec_Coop* self))(void)) {
    let task = orelse_((self->task_curr)(return));
    if (exec_kind(task->inner) != exec_Task_Kind_fiber) return;
    let fiber = orelse_((task->fiber)(claim_unreachable));
    exec_switchFromFiber(&fiber->ctx, &self->fiber_ctx);
};

fn_((exec_Coop_run(exec_Coop* self))(void)) {
    while (true) {
        exec_Coop__wakeDueTimers(self);
        if (exec_Coop__runOneReady(self)) continue;
        let deadline = orelse_((exec_Coop__nextTimerDeadline(self))(break));
        let remaining = exec_Coop__remaining(self, deadline);
        if (time_Dur_isZero(remaining)) continue;
        let_ignore = self->clock.vtbl->sleepFn(self->clock.ctx, remaining);
    }
};

fn_((exec_Coop_runUntil(exec_Coop* self, time_Inst deadline))(void)) {
    while (cmp_lt$(time_Inst)(exec_Coop__now(self), deadline)) {
        exec_Coop__wakeDueTimers(self);
        if (exec_Coop__runOneReady(self)) continue;
        var wait_deadline = deadline;
        if_some((exec_Coop__nextTimerDeadline(self))(next_deadline)) {
            if (cmp_lt$(time_Inst)(next_deadline, wait_deadline)) {
                wait_deadline = next_deadline;
            }
        }
        let remaining = exec_Coop__remaining(self, wait_deadline);
        if (time_Dur_isZero(remaining)) break;
        let_ignore = self->clock.vtbl->sleepFn(self->clock.ctx, remaining);
    }
    exec_Coop__wakeDueTimers(self);
};

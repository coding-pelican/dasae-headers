#include "Seq.h"
#include "common.h"
#include "../Sched.h"

T_use_E$($set(mem_E)(P$exec_Seq_Task));
T_use_E$($set(mem_E)(P$P$exec_Seq_Task));
T_use$((exec_Seq_Task)(mem_Alctr_create, mem_Alctr_destroy));
T_use$((P$exec_Seq_Task)(
    ArrQue_empty,
    ArrQue_fini,
    ArrQue_enque,
    ArrQue_deque,
    ArrList_empty,
    ArrList_fini,
    ArrList_addBack
));

$static fn_((exec_Seq__workFiber(P$raw owner, P$raw any_task))(void)) {
    let self = ptrAlignCast$((exec_Seq*)(ensureNonnull(owner)));
    let task = ptrAlignCast$((exec_Seq_Task*)(ensureNonnull(any_task)));
    exec_invokeToCompletion(task->inner, task->result);
    if (task->state != exec_Seq_Task_State_canceled) task->state = exec_Seq_Task_State_done;
    let fiber = orelse_((task->fiber)(claim_unreachable));
    exec_switchFromFiber(&fiber->ctx, &self->fiber_ctx);
    claim_unreachable;
};

$attr($must_check)
$static fn_((exec_Seq__createTask(
    exec_Seq* self,
    exec_Seq_Task_State state,
    u_P$raw result,
    P$$(Closure$raw) inner
))(E$P$exec_Seq_Task) $guard) {
    claim_assert_nonnull(self), claim_assert_nonnull(result.raw), claim_assert_nonnull(inner);
    let task = try_(mem_Alctr_create$exec_Seq_Task($trace self->gpa));
    errdefer_($ignore, mem_Alctr_destroy$exec_Seq_Task($trace self->gpa, task));
    return_ok(asg_l((task)({
        .state = state,
        .result = result,
        .inner = inner,
        .fiber = none(),
    })));
} $unguarded(fn);
$static fn_((exec_Seq__destroyTask(exec_Seq* self, exec_Seq_Task* task))(void)) {
    claim_assert_nonnull(self), claim_assert_nonnull(task);
    if_some((task->fiber)(fiber)) exec_destroyFiber(self->gpa, fiber);
    mem_Alctr_destroy($trace self->gpa, u_anyP(task));
};

$static fn_((exec_Seq__enqueReady(exec_Seq* self, exec_Seq_Task* task))(bool)) {
    claim_assert_nonnull(self), claim_assert_nonnull(task);
    task->state = exec_Seq_Task_State_ready;
    if (isOk(ArrQue_enque$P$exec_Seq_Task(&self->tasks_ready, self->gpa, task))) return true;
    task->state = exec_Seq_Task_State_canceled;
    return false;
};

$attr($must_check)
$static fn_((exec_Seq__storeTask(
    exec_Seq* self,
    exec_Seq_Task_State state,
    u_P$raw result,
    P$$(Closure$raw) inner
))(E$P$exec_Seq_Task) $guard) {
    claim_assert_nonnull(self), claim_assert_nonnull(result.raw), claim_assert_nonnull(inner);
    let task = try_(exec_Seq__createTask(self, state, result, inner));
    errdefer_($ignore, exec_Seq__destroyTask(self, task));
    if (exec_kind(inner) == exec_Task_Kind_fiber) {
        asg_l((&task->fiber)(some(try_(exec_createFiber(self->gpa, self, task, exec_Seq__workFiber)))));
    }
    return_ok(*try_(ArrList_addBack$P$exec_Seq_Task(&self->tasks, self->gpa)) = task);
} $unguarded(fn);

$static fn_((exec_Seq__createReadyTask(
    exec_Seq* self,
    u_P$raw result,
    P$$(Closure$raw) inner
))(O$P$exec_Seq_Task) $scope) {
    claim_assert_nonnull(self), claim_assert_nonnull(result.raw), claim_assert_nonnull(inner);
    let task = catch_((exec_Seq__storeTask(self, exec_Seq_Task_State_none, result, inner))($ignore, return_none()));
    if (!exec_Seq__enqueReady(self, task)) return_none();
    return_some(task);
} $unscoped(fn);

$static fn_((exec_Seq__runTask(exec_Seq* self, exec_Seq_Task* task))(void)) {
    claim_assert_nonnull(self), claim_assert_nonnull(task);
    if (task->state == exec_Seq_Task_State_canceled || task->state == exec_Seq_Task_State_done) return;
    if (exec_kind(task->inner) == exec_Task_Kind_fiber) {
        task->state = exec_Seq_Task_State_running;
        let prev = self->task_curr;
        asg_l((&self->task_curr)(some(task)));
        let fiber = orelse_((task->fiber)(claim_unreachable));
        exec_switchToFiber(&self->fiber_ctx, fiber);
        asg_l((&self->task_curr)(prev));
        return;
    }
    task->state = exec_Seq_Task_State_running;
    let done = exec_invokeStep(task->inner, task->result);
    if (task->state == exec_Seq_Task_State_running) {
        if (done) {
            task->state = exec_Seq_Task_State_done;
        } else {
            let_ignore = exec_Seq__enqueReady(self, task);
        }
    }
};

$static fn_((exec_Seq__runOneReady(exec_Seq* self))(bool)) {
    claim_assert_nonnull(self);
    let task = orelse_((ArrQue_deque$P$exec_Seq_Task(&self->tasks_ready))(return false));
    exec_Seq__runTask(self, task);
    return true;
};

fn_((exec_Seq_init(mem_Alctr gpa))(exec_Seq)) {
    return (exec_Seq){
        .gpa = mem_Alctr_ensureValid(gpa),
        .fiber_ctx = {},
        .task_curr = none(),
        .tasks = ArrList_empty$P$exec_Seq_Task(),
        .tasks_ready = ArrQue_empty$P$exec_Seq_Task(),
    };
};

fn_((exec_Seq_fini(exec_Seq* self))(void)) {
    claim_assert_nonnull(self);
    for_(($s(self->tasks.items))(task)) exec_Seq__destroyTask(self, *task) $end(for);
    ArrQue_fini$P$exec_Seq_Task(&self->tasks_ready, self->gpa);
    ArrList_fini$P$exec_Seq_Task(&self->tasks, self->gpa);
    asg_l((self)(cleared()));
};

$static fn_((exec_Seq__Sched_async(
    P$raw ctx,
    u_P$raw result,
    P$$(Closure$raw) inner
))(O$P$Future$raw) $scope) {
    let self = ptrAlignCast$((exec_Seq*)(ensureNonnull(ctx)));
    let task = orelse_((exec_Seq__createReadyTask(self, result, inner))({
        return_none(exec_invokeToCompletion(inner, result));
    }));
    return_some(ptrCast$((P$Future$raw)(task)));
} $unscoped(fn);

$static fn_((exec_Seq__Sched_spawn(
    P$raw ctx,
    u_P$raw result,
    P$$(Closure$raw) inner
))(Sched_ConcE$P$Future$raw) $scope) {
    let self = ptrAlignCast$((exec_Seq*)(ensureNonnull(ctx)));
    let task = orelse_((exec_Seq__createReadyTask(self, result, inner))(
        return_err(Sched_ConcE_Unavailable())
    ));
    return_ok(ptrCast$((P$Future$raw)(task)));
} $unscoped(fn);

$static fn_((exec_Seq__Sched_await(P$raw ctx, P$Future$raw any_future, u_P$raw result))(void)) {
    let self = ptrAlignCast$((exec_Seq*)(ensureNonnull(ctx)));
    let task = ptrAlignCast$((exec_Seq_Task*)(ensureNonnull(any_future)));
    while (task->state != exec_Seq_Task_State_done && task->state != exec_Seq_Task_State_canceled) {
        if (task->state == exec_Seq_Task_State_deferred) {
            exec_Seq__runTask(self, task);
            continue;
        }
        if (!exec_Seq__runOneReady(self)) exec_Seq__runTask(self, task);
    }
    if (task->state == exec_Seq_Task_State_done) u_memcpy(result, task->result.as_const);
};

$static fn_((exec_Seq__Sched_cancel(P$raw ctx, P$Future$raw any_future, u_P$raw result))(void)) {
    let_ignore = ptrAlignCast$((exec_Seq*)(ensureNonnull(ctx)));
    let task = ptrAlignCast$((exec_Seq_Task*)(ensureNonnull(any_future)));
    if (task->state != exec_Seq_Task_State_done) task->state = exec_Seq_Task_State_canceled;
    if (task->state == exec_Seq_Task_State_done) u_memcpy(result, task->result.as_const);
};

fn_((Sched_seq(exec_Seq* self))(Sched)) {
    $static let_(vtbl, Sched_VTbl) $like_ref = { {
        .asyncFn = exec_Seq__Sched_async,
        .spawnFn = exec_Seq__Sched_spawn,
        .awaitFn = exec_Seq__Sched_await,
        .cancelFn = exec_Seq__Sched_cancel,
    } };
    return (Sched){
        .ctx = self,
        .vtbl = vtbl,
    };
};

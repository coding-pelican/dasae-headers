#include "Lane.h"

$static fn_((exec_Lane__workFiber(P$raw owner, P$raw any_task))(void));
$static fn_((exec_Lane__createTask(
    exec_Lane* self,
    exec_Task_State state,
    u_P$raw result,
    P$$(Closure$raw) inner
))(E$P$exec_Task));
$static fn_((exec_Lane__destroyTask(exec_Lane* self, exec_Task* task))(void));

fn_((exec_Lane__workFiber(P$raw owner, P$raw any_task))(void)) {
    let self = ptrAlignCast$((exec_Lane*)(ensureNonnull(owner)));
    let task = ptrAlignCast$((exec_Task*)(ensureNonnull(any_task)));
    u_memcpy(task->result, Closure_invokeToComplete(task->inner, task->result.type));
    if (task->state != exec_Task_State_canceled) task->state = exec_Task_State_done;
    let fiber = orelse_((task->fiber)(claim_unreachable));
    exec_switchFromFiber(&fiber->context, &self->fiber_context);
    claim_unreachable;
};

T_use_E$($set(mem_E)(P$exec_Task));
T_use$((exec_Task)(mem_Alctr_create, mem_Alctr_destroy));
fn_((exec_Lane__createTask(
    exec_Lane* self,
    exec_Task_State state,
    u_P$raw result,
    P$$(Closure$raw) inner
))(E$P$exec_Task) $guard) {
    claim_assert_nonnull(self), claim_assert_nonnull(result.raw), claim_assert_nonnull(inner);
    let task = try_(mem_Alctr_create$exec_Task($trace self->gpa));
    errdefer_($ignore, mem_Alctr_destroy$exec_Task($trace self->gpa, task));
    return_ok(asg_l((task)({
        .state = state,
        .result = result,
        .inner = inner,
        .fiber = none(),
    })));
} $unguarded(fn);

fn_((exec_Lane__destroyTask(exec_Lane* self, exec_Task* task))(void)) {
    claim_assert_nonnull(self), claim_assert_nonnull(task);
    if_some((task->fiber)(fiber)) exec_Fiber_fini(fiber, self->gpa);
    mem_Alctr_destroy($trace self->gpa, u_anyP(task));
};

T_use$((P$exec_Task)(ArrList_empty, ArrQue_empty));
fn_((exec_Lane_init(mem_Alctr gpa))(exec_Lane)) {
    return (exec_Lane){
        .gpa = mem_Alctr_ensureValid(gpa),
        .fiber_context = {},
        .task_curr = none(),
        .tasks = ArrList_empty$P$exec_Task(),
        .tasks_ready = ArrQue_empty$P$exec_Task(),
    };
};

T_use$((P$exec_Task)(ArrList_fini, ArrQue_fini));
fn_((exec_Lane_fini(exec_Lane* self))(void)) {
    claim_assert_nonnull(self);
    for_(($s(self->tasks.items))(task)) exec_Lane__destroyTask(self, *task) $end(for);
    ArrQue_fini$P$exec_Task(&self->tasks_ready, self->gpa);
    ArrList_fini$P$exec_Task(&self->tasks, self->gpa);
    asg_l((self)(cleared()));
};

fn_((exec_Lane_task(exec_Lane* self))(O$P$exec_Task)) {
    claim_assert_nonnull(self);
    return self->task_curr;
};

T_use$((P$exec_Task)(ArrQue_enque));
fn_((exec_Lane_readyTask(exec_Lane* self, exec_Task* task))(bool)) {
    claim_assert_nonnull(self), claim_assert_nonnull(task);
    task->state = exec_Task_State_ready;
    if (isOk(ArrQue_enque$P$exec_Task(&self->tasks_ready, self->gpa, task))) return true;
    task->state = exec_Task_State_canceled;
    return false;
};

T_use_E$($set(mem_E)(P$P$exec_Task));
T_use$((P$exec_Task)(ArrList_addBack));
fn_((exec_Lane_createReadyTask(
    exec_Lane* self,
    u_P$raw result,
    P$$(Closure$raw) inner
))(O$P$exec_Task) $scope) {
    claim_assert_nonnull(self), claim_assert_nonnull(result.raw), claim_assert_nonnull(inner);
    let task = catch_((exec_Lane__createTask(self, exec_Task_State_none, result, inner))(
        $ignore, return_none()
    ));
    if (exec_kind(inner) == exec_Task_Kind_fiber) {
        let fiber = catch_((exec_Fiber_init(self->gpa, self, task, exec_Lane__workFiber))(
            $ignore, {
                exec_Lane__destroyTask(self, task);
                return_none();
            }
        ));
        asg_l((&task->fiber)(some(fiber)));
    }
    let slot = catch_((ArrList_addBack$P$exec_Task(&self->tasks, self->gpa))(
        $ignore, {
            exec_Lane__destroyTask(self, task);
            return_none();
        }
    ));
    *slot = task;
    if (!exec_Lane_readyTask(self, task)) return_none();
    return_some(task);
} $unscoped(fn);

fn_((exec_Lane_asyncTask(exec_Lane* self, u_P$raw result, P$$(Closure$raw) inner))(O$P$exec_Task) $scope) {
    claim_assert_nonnull(self), claim_assert_nonnull(result.raw), claim_assert_nonnull(inner);
    let task = orelse_((exec_Lane_createReadyTask(self, result, inner))({
        u_memcpy(result, Closure_invokeToComplete(inner, result.type));
        return_none();
    }));
    return_some(task);
} $unscoped(fn);

fn_((exec_Lane_spawnTask(exec_Lane* self, u_P$raw result, P$$(Closure$raw) inner))(O$P$exec_Task) $scope) {
    claim_assert_nonnull(self), claim_assert_nonnull(result.raw), claim_assert_nonnull(inner);
    let task = orelse_((exec_Lane_createReadyTask(self, result, inner))(return_none()));
    return_some(task);
} $unscoped(fn);

fn_((exec_Lane_runTask(exec_Lane* self, exec_Task* task))(void)) {
    claim_assert_nonnull(self), claim_assert_nonnull(task);
    if (task->state == exec_Task_State_canceled
        || task->state == exec_Task_State_done
        || task->state == exec_Task_State_waiting) return;
    if (exec_kind(task->inner) == exec_Task_Kind_fiber) {
        task->state = exec_Task_State_running;
        let prev = self->task_curr;
        asg_l((&self->task_curr)(some(task)));
        let fiber = orelse_((task->fiber)(claim_unreachable));
        exec_switchToFiber(&self->fiber_context, fiber);
        asg_l((&self->task_curr)(prev));
        return;
    }
    let prev = self->task_curr;
    asg_l((&self->task_curr)(some(task)));
    task->state = exec_Task_State_running;
    let ret = Closure_invokeToStep(task->inner, task->result.type);
    let done = isSome(ret);
    if (done) u_memcpy(task->result, unwrap_(ret));
    if (task->state == exec_Task_State_running) {
        if (done) {
            task->state = exec_Task_State_done;
        } else {
            let_ignore = exec_Lane_readyTask(self, task);
        }
    }
    asg_l((&self->task_curr)(prev));
};

T_use$((P$exec_Task)(ArrQue_deque));
fn_((exec_Lane_runOneReady(exec_Lane* self))(bool)) {
    claim_assert_nonnull(self);
    let task = orelse_((ArrQue_deque$P$exec_Task(&self->tasks_ready))(return false));
    exec_Lane_runTask(self, task);
    return true;
};

fn_((exec_Lane_yield(exec_Lane* self))(void)) {
    claim_assert_nonnull(self);
    let task = orelse_((self->task_curr)(return));
    if (exec_kind(task->inner) != exec_Task_Kind_fiber) return;
    let fiber = orelse_((task->fiber)(claim_unreachable));
    exec_switchFromFiber(&fiber->context, &self->fiber_context);
};

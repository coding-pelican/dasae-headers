#include "Preem.h"

fn_((exec_Preem_work(exec_Preem_Task* task))(Void)) {
    claim_assert_nonnull(task), claim_assert_nonnull(task->owner);
    claim_assert_nonnull(task->result.raw), claim_assert_nonnull(task->inner);
    task->state = exec_Task_State_running;
    u_memcpy(task->result, Closure_invokeToComplete(task->inner, task->result.type));
    if (task->state != exec_Task_State_canceled) task->state = exec_Task_State_done;
    return (Void){};
};

fn_((exec_Preem_init(mem_Alctr gpa))(exec_Preem)) {
    gpa = mem_Alctr_ensureValid(gpa);
    return (exec_Preem){
        .spawn_cfg = with_((Thrd_SpawnCfg_default)((.gpa)(some(gpa)))),
        .tasks_mtx = Thrd_Mtx_init(),
        .tasks = none(),
    };
};

fn_((exec_Preem_fini(exec_Preem* self))(void)) {
    claim_assert_nonnull(self);
    claim_assert(isNone(self->tasks));
    Thrd_Mtx_fini(&self->tasks_mtx);
    asg_l((self)(cleared()));
};

fn_((exec_Preem_createTask(exec_Preem* self, u_P$raw result, P$$(Closure$raw) inner))(Sched_ConcE$P$exec_Preem_Task) $guard) {
    claim_assert_nonnull(self), claim_assert_nonnull(result.raw), claim_assert_nonnull(inner);
    let gpa = unwrap_(self->spawn_cfg.gpa);
    let task = u_castP$((exec_Preem_Task*)(catch_(
        (mem_Alctr_create($trace gpa, typeInfo$(exec_Preem_Task)))(
            $ignore, return_err(Sched_ConcE_Unavailable())
        )
    )));
    errdefer_($ignore, mem_Alctr_destroy($trace gpa, u_anyP(task)));
    asg_l((task)({
        .next = none(),
        .owner = self,
        .thrd = cleared(),
        .result = result,
        .inner = inner,
        .state = exec_Task_State_ready,
        .runner = closure_(exec_Preem_work)(task),
    }));
    let thrd = catch_((Thrd_spawn(self->spawn_cfg, task->runner.as_raw, typeInfo$(Void)))(
        $ignore, return_err(Sched_ConcE_Unavailable())
    ));
    task->thrd = thrd;
    exec_Preem_linkTask(self, task);
    return_ok(task);
} $unguarded(fn);

fn_((exec_Preem_destroyTask(exec_Preem* self, exec_Preem_Task* task))(void)) {
    claim_assert_nonnull(self), claim_assert_nonnull(task);
    let gpa = unwrap_(self->spawn_cfg.gpa);
    mem_Alctr_destroy($trace gpa, u_anyP(task));
};

fn_((exec_Preem_linkTask(exec_Preem* self, exec_Preem_Task* task))(void) $guard) {
    claim_assert_nonnull(self), claim_assert_nonnull(task);
    Thrd_Mtx_lock(&self->tasks_mtx);
    defer_(Thrd_Mtx_unlock(&self->tasks_mtx));
    task->next = self->tasks;
    asg_l((&self->tasks)(some(task)));
} $unguarded(fn);

fn_((exec_Preem_unlinkTask(exec_Preem* self, exec_Preem_Task* task))(void) $guard) {
    claim_assert_nonnull(self), claim_assert_nonnull(task);
    Thrd_Mtx_lock(&self->tasks_mtx);
    defer_(Thrd_Mtx_unlock(&self->tasks_mtx));
    var_(prev, O$P$exec_Preem_Task) = none();
    var_(curr, O$P$exec_Preem_Task) = self->tasks;
    while_some(curr, node) {
        if (node == task) {
            if_some((prev)(prev_node)) {
                prev_node->next = node->next;
            } else {
                self->tasks = node->next;
            }
            break;
        }
        prev = curr;
        curr = node->next;
    }
} $unguarded(fn);

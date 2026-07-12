#include "dh/exec/Preem.h"

/*========== Internal Declarations ==========================================*/

$static fn_((exec_Preem_Task__slabBytes(TypeInfo result_ty))(usize));
$static fn_((exec_Preem_Task__resultMut(exec_Preem_Task* self, TypeInfo type))(u_P$raw));
$static fn_((exec_Preem_Task__freeSlab(exec_Preem_Task* self, mem_Alctr gpa))(void));

/*========== External Definitions ===========================================*/

fn_((exec_Preem_work(exec_Preem_Task* task))(Void)) {
    claim_assert_nonnull(task), claim_assert_nonnull(task->owner);
    claim_assert_nonnull(task->result.raw), claim_assert_nonnull(task->inner);
    task->state = exec_Task_State_running;
    u_memcpy(task->result, clsr_invokeToComplete(task->inner, task->result.type));
    if (task->state != exec_Task_State_canceled) task->state = exec_Task_State_done;
    return (Void){};
};

fn_((exec_Preem_init(mem_Alctr gpa))(exec_Preem)) {
    gpa = mem_Alctr_ensureValid(gpa);
    return (exec_Preem){
        .spawn_cfg = thrd_SpawnCfg_default(gpa),
        .tasks_mtx = thrd_Mtx_init(),
        .tasks = none(),
    };
};

fn_((exec_Preem_fini(exec_Preem* self))(void)) {
    claim_assert_nonnull(self);
    claim_assert(isNone(self->tasks));
    thrd_Mtx_fini(&self->tasks_mtx);
    asg_l((self)(cleared()));
};

fn_((exec_Preem_createTask(exec_Preem* self, u_P$raw result, P$$(Clsr$raw) inner))(Sched_ConcE$P$exec_Preem_Task) $guard) {
    claim_assert_nonnull(self), claim_assert_nonnull(result.raw), claim_assert_nonnull(inner);
    let gpa = self->spawn_cfg.gpa;
    let bytes = exec_Preem_Task__slabBytes(result.type);
    let mem = orelse_((mem_Alctr_rawAlloc($trace gpa, bytes, alignOfLog2$(exec_Preem_Task)))(
        return_err(E_cause$Sched_ConcUnavailable())
    ));
    let task = ptrAlignCast$((exec_Preem_Task*)(mem));
    errdefer_($ignore, exec_Preem_Task__freeSlab(task, gpa));
    mem_set0Bytes(P_prefix$((S$u8)(mem)(bytes)));
    asg_l((task)({
        .next = none(),
        .owner = self,
        .thrd = cleared(),
        .result = exec_Preem_Task__resultMut(task, result.type),
        .inner = inner,
        .state = exec_Task_State_ready,
        .runner = clsr_((exec_Preem_work)(task)),
    }));
    let thrd = catch_((thrd_spawn(self->spawn_cfg, task->runner.as_raw, typeInfo$(Void)))(
        $ignore, return_err(E_cause$Sched_ConcUnavailable())
    ));
    task->thrd = thrd;
    exec_Preem_linkTask(self, task);
    return_ok(task);
} $unguarded(fn);

fn_((exec_Preem_destroyTask(exec_Preem* self, exec_Preem_Task* task))(void)) {
    claim_assert_nonnull(self), claim_assert_nonnull(task);
    let gpa = self->spawn_cfg.gpa;
    exec_Preem_Task__freeSlab(task, gpa);
};

fn_((exec_Preem_linkTask(exec_Preem* self, exec_Preem_Task* task))(void) $guard) {
    claim_assert_nonnull(self), claim_assert_nonnull(task);
    thrd_Mtx_lockProtcd(&self->tasks_mtx);
    defer_(thrd_Mtx_unlock(&self->tasks_mtx));
    task->next = self->tasks;
    asg_l((&self->tasks)(some(task)));
} $unguarded(fn);

fn_((exec_Preem_unlinkTask(exec_Preem* self, exec_Preem_Task* task))(void) $guard) {
    claim_assert_nonnull(self), claim_assert_nonnull(task);
    thrd_Mtx_lockProtcd(&self->tasks_mtx);
    defer_(thrd_Mtx_unlock(&self->tasks_mtx));
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

/*========== Internal Definitions ===========================================*/

fn_((exec_Preem_Task__slabBytes(TypeInfo result_ty))(usize)) {
    return mem_alignFwd(sizeOf$(exec_Preem_Task), mem_log2ToAlign(result_ty.log2_align)) + result_ty.size;
};
fn_((exec_Preem_Task__resultMut(exec_Preem_Task* self, TypeInfo type))(u_P$raw)) {
    claim_assert_nonnull(self);
    return (u_P$raw){
        .raw = intToPtr$((u8*)(ptrToInt(self) + mem_alignFwd(sizeOf$(exec_Preem_Task), mem_log2ToAlign(type.log2_align)))),
        .type = type,
    };
};
fn_((exec_Preem_Task__freeSlab(exec_Preem_Task* self, mem_Alctr gpa))(void)) {
    claim_assert_nonnull(self);
    let bytes = exec_Preem_Task__slabBytes(self->result.type);
    mem_Alctr_rawFree($trace gpa, P_prefix$((S$u8)(as$(u8*)(self))(bytes)), alignOfLog2$(exec_Preem_Task));
};

#include "Preem.h"
#include "common.h"

typedef struct exec_Preem_Task {
    var_(fn, Thrd_WorkFn);
    var_(handle, Thrd_Handle);
    var_(thrd, Thrd);
    var_(result, u_P$raw);
    var_(inner, P$$(Closure$raw));
} exec_Preem_Task;
T_use_prl$(exec_Preem_Task);
T_use_E$($set(mem_E)(P$exec_Preem_Task));
T_use$((exec_Preem_Task)(mem_Alctr_create, mem_Alctr_destroy));

$static fn_((exec_Preem__thrdEntry(Thrd_FnCtx* fn_ctx))(Thrd_FnCtx*)) {
    let task = as$(exec_Preem_Task*)(fn_ctx);
    exec_invokeToCompletion(task->inner, task->result);
    return as$(Thrd_FnCtx*)(task);
};

$static fn_((exec_Preem__createTask(
    exec_Preem* self,
    u_P$raw result,
    P$$(Closure$raw) inner
))(E$P$exec_Preem_Task) $guard) {
    let task = try_(mem_Alctr_create$exec_Preem_Task($trace self->gpa));
    errdefer_($ignore, mem_Alctr_destroy$exec_Preem_Task($trace self->gpa, task));
    return_ok(asg_l((task)({
        .fn = exec_Preem__thrdEntry,
        .handle = cleared(),
        .thrd = cleared(),
        .result = result,
        .inner = inner,
    })));
} $unguarded(fn);

$static fn_((exec_Preem__destroyTask(exec_Preem* self, exec_Preem_Task* task))(void)) {
    claim_assert_nonnull(self);
    claim_assert_nonnull(task);
    mem_Alctr_destroy($trace self->gpa, u_anyP(task));
};

$static fn_((exec_Preem__trySpawn(
    exec_Preem* self,
    u_P$raw result,
    P$$(Closure$raw) inner
))(E$P$exec_Preem_Task) $guard) {
    let task = try_(exec_Preem__createTask(self, result, inner));
    errdefer_($ignore, exec_Preem__destroyTask(self, task));
    task->thrd = try_(Thrd_spawn(self->spawn_cfg, as$(Thrd_FnCtx*)(task)));
    return_ok(task);
} $unguarded(fn);

fn_((exec_Preem_init(mem_Alctr gpa))(exec_Preem)) {
    return (exec_Preem){
        .gpa = mem_Alctr_ensureValid(gpa),
        .spawn_cfg = Thrd_SpawnCfg_default,
    };
};

$static fn_((exec_Preem__Sched_async(
    P$raw ctx,
    u_P$raw result,
    P$$(Closure$raw) inner
))(O$P$Future$raw) $scope) {
    let self = as$(exec_Preem*)(ctx);
    let task = catch_((exec_Preem__trySpawn(self, result, inner))($ignore, {
        exec_invokeToCompletion(inner, result);
        return_none();
    }));
    return_some(as$(P$Future$raw)(task));
} $unscoped(fn);

$static fn_((exec_Preem__Sched_spawn(
    P$raw ctx,
    u_P$raw result,
    P$$(Closure$raw) inner
))(Sched_ConcE$P$Future$raw) $scope) {
    let self = as$(exec_Preem*)(ctx);
    let task = catch_((exec_Preem__trySpawn(self, result, inner))($ignore, {
        return_err(Sched_ConcE_Unavailable());
    }));
    return_ok(as$(P$Future$raw)(task));
} $unscoped(fn);

$static fn_((exec_Preem__Sched_await(P$raw ctx, P$Future$raw any_future, u_P$raw result))(void)) {
    let self = as$(exec_Preem*)(ctx);
    let task = as$(exec_Preem_Task*)(any_future);
    let joined = as$(exec_Preem_Task*)(Thrd_join(task->thrd));
    claim_assert(joined == task);
    u_memcpy(result, task->result.as_const);
    exec_Preem__destroyTask(self, task);
};

$static fn_((exec_Preem__Sched_cancel(P$raw ctx, P$Future$raw any_future, u_P$raw result))(void)) {
    exec_Preem__Sched_await(ctx, any_future, result);
};

fn_((Sched_preem(exec_Preem* self))(Sched)) {
    static const Sched_VTbl vtbl $like_ref = { {
        .asyncFn = exec_Preem__Sched_async,
        .spawnFn = exec_Preem__Sched_spawn,
        .awaitFn = exec_Preem__Sched_await,
        .cancelFn = exec_Preem__Sched_cancel,
    } };
    return (Sched){
        .ctx = self,
        .vtbl = vtbl,
    };
};

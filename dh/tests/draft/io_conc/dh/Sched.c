#include "Sched.h"
#include "exec/Seq.h"
#include "exec/Coop.h"
#include "exec/Preem.h"
#include "exec/Para.h"

/*========== Internal Declarations ==========================================*/

$attr($inline_always)
$static fn_((Sched__async(exec_Lane* ctx, u_P$raw result, P$$(Closure$raw) inner))(O$P$Future$raw));
$attr($inline_always $must_check)
$static fn_((Sched__spawn(exec_Lane* ctx, u_P$raw result, P$$(Closure$raw) inner))(Sched_ConcE$P$Future$raw));
$attr($inline_always)
$static fn_((Sched__cancel(exec_Lane* ctx, exec_Task* any_future, u_P$raw result))(void));

$static fn_((Sched_seq__async(P$raw ctx, u_P$raw result, P$$(Closure$raw) inner))(O$P$Future$raw));
$attr($must_check)
$static fn_((Sched_seq__spawn(P$raw ctx, u_P$raw result, P$$(Closure$raw) inner))(Sched_ConcE$P$Future$raw));
$static fn_((Sched_seq__await(P$raw ctx, P$Future$raw any_future, u_P$raw result))(void));
$static fn_((Sched_seq__cancel(P$raw ctx, P$Future$raw any_future, u_P$raw result))(void));

$static fn_((Sched_coop__async(P$raw ctx, u_P$raw result, P$$(Closure$raw) inner))(O$P$Future$raw));
$attr($must_check)
$static fn_((Sched_coop__spawn(P$raw ctx, u_P$raw result, P$$(Closure$raw) inner))(Sched_ConcE$P$Future$raw));
$static fn_((Sched_coop__await(P$raw ctx, P$Future$raw any_future, u_P$raw result))(void));
$static fn_((Sched_coop__cancel(P$raw ctx, P$Future$raw any_future, u_P$raw result))(void));

/*========== External Definitions ===========================================*/

fn_((Sched_VTbl_noAsync(P$raw ctx, u_P$raw result, P$$(Closure$raw) inner))(O$P$Future$raw) $scope) {
    let_ignore = ctx;
    let_ignore = result;
    let_ignore = inner;
    return_none();
} $unscoped(fn);

fn_((Sched_VTbl_noSpawn(P$raw ctx, u_P$raw result, P$$(Closure$raw) inner))(Sched_ConcE$P$Future$raw) $scope) {
    let_ignore = ctx;
    let_ignore = result;
    let_ignore = inner;
    return_err(Sched_ConcE_Unavailable());
} $unscoped(fn);

fn_((Sched_VTbl_noAwait(P$raw ctx, P$Future$raw any_future, u_P$raw result))(void)) {
    let_ignore = ctx;
    let_ignore = any_future;
    let_ignore = result;
};

fn_((Sched_VTbl_noCancel(P$raw ctx, P$Future$raw any_future, u_P$raw result))(void)) {
    let_ignore = ctx;
    let_ignore = any_future;
    let_ignore = result;
};

fn_((Sched_async(Sched self, Closure$raw* closure, TypeInfo ret_ty, V$Future$raw ret_mem))(V$Future$raw)) {
    claim_assert_nonnull(closure);
    claim_assert_nonnull(ret_mem);
    debug_assert_eqBy(ret_mem->type, ret_ty, TypeInfo_eql);
    let result = Future_resultMut(ret_mem, ret_ty);
    ret_mem->any_future = self.vtbl->asyncFn(self.ctx, result, closure);
    return ret_mem;
};

fn_((Sched_spawn(Sched self, Closure$raw* closure, TypeInfo ret_ty, V$Future$raw ret_mem))(Sched_ConcE$V$Future$raw) $scope) {
    claim_assert_nonnull(closure);
    claim_assert_nonnull(ret_mem);
    debug_assert_eqBy(ret_mem->type, ret_ty, TypeInfo_eql);
    let result = Future_resultMut(ret_mem, ret_ty);
    asg_l((&ret_mem->any_future)(some(try_(self.vtbl->spawnFn(self.ctx, result, closure)))));
    return_ok(ret_mem);
} $unscoped(fn);

fn_((Sched_seq(exec_Seq* self))(Sched)) {
    $static let_(vtbl, Sched_VTbl) $like_ref = { {
        .asyncFn = Sched_seq__async,
        .spawnFn = Sched_seq__spawn,
        .awaitFn = Sched_seq__await,
        .cancelFn = Sched_seq__cancel,
    } };
    return (Sched){
        .ctx = self,
        .vtbl = vtbl,
    };
};

fn_((Sched_coop(exec_Coop* loop))(Sched)) {
    static const Sched_VTbl vtbl $like_ref = { {
        .asyncFn = Sched_coop__async,
        .spawnFn = Sched_coop__spawn,
        .awaitFn = Sched_coop__await,
        .cancelFn = Sched_coop__cancel,
    } };
    return (Sched){
        .ctx = loop,
        .vtbl = vtbl,
    };
};

fn_((Sched_preem(exec_Preem* preem))(Sched)) {
    /* TODO: Support */
    static const Sched_VTbl vtbl $like_ref = { {
        .asyncFn = Sched_VTbl_noAsync,
        .spawnFn = Sched_VTbl_noSpawn,
        .awaitFn = Sched_VTbl_noAwait,
        .cancelFn = Sched_VTbl_noCancel,
    } };
    return (Sched){
        .ctx = preem,
        .vtbl = vtbl,
    };
};

fn_((Sched_para(exec_Para* para))(Sched)) {
    /* TODO: Support */
    static const Sched_VTbl vtbl $like_ref = { {
        .asyncFn = Sched_VTbl_noAsync,
        .spawnFn = Sched_VTbl_noSpawn,
        .awaitFn = Sched_VTbl_noAwait,
        .cancelFn = Sched_VTbl_noCancel,
    } };
    return (Sched){
        .ctx = para,
        .vtbl = vtbl,
    };
};

/*========== Internal Definitions ===========================================*/

fn_((Sched__async(exec_Lane* ctx, u_P$raw result, P$$(Closure$raw) inner))(O$P$Future$raw) $scope) {
    claim_assert_nonnull(ctx), claim_assert_nonnull(result.raw), claim_assert_nonnull(inner);
    let task = orelse_((exec_Lane_asyncTask(ctx, result, inner))(
        return_none()
    ));
    return_some(ptrCast$((P$Future$raw)(task)));
} $unscoped(fn);

fn_((Sched__spawn(exec_Lane* ctx, u_P$raw result, P$$(Closure$raw) inner))(Sched_ConcE$P$Future$raw) $scope) {
    claim_assert_nonnull(ctx), claim_assert_nonnull(result.raw), claim_assert_nonnull(inner);
    let task = orelse_((exec_Lane_spawnTask(ctx, result, inner))(
        return_err(Sched_ConcE_Unavailable())
    ));
    return_ok(ptrCast$((P$Future$raw)(task)));
} $unscoped(fn);

fn_((Sched__cancel(exec_Lane* ctx, exec_Task* any_future, u_P$raw result))(void)) {
    claim_assert_nonnull(ctx), claim_assert_nonnull(any_future), claim_assert_nonnull(result.raw);
    if (exec_Task_isDone(any_future)) {
        exec_Task_copyToResult(any_future, result);
        return;
    }
    exec_Task_cancel(any_future);
};

fn_((Sched_seq__async(P$raw ctx, u_P$raw result, P$$(Closure$raw) inner))(O$P$Future$raw) $scope) {
    let self = ptrAlignCast$((exec_Seq*)(ensureNonnull(ctx)));
    return Sched__async(&self->lane, result, inner);
} $unscoped(fn);

fn_((Sched_seq__spawn(P$raw ctx, u_P$raw result, P$$(Closure$raw) inner))(Sched_ConcE$P$Future$raw) $scope) {
    let self = ptrAlignCast$((exec_Seq*)(ensureNonnull(ctx)));
    return Sched__spawn(&self->lane, result, inner);
} $unscoped(fn);

fn_((Sched_seq__await(P$raw ctx, P$Future$raw any_future, u_P$raw result))(void)) {
    claim_assert_nonnull(result.raw);
    let self = ptrAlignCast$((exec_Seq*)(ensureNonnull(ctx)));
    let task = ptrAlignCast$((exec_Task*)(ensureNonnull(any_future)));
    exec_Seq_awaitUntilDone(self, task);
    if (exec_Task_isDone(task)) exec_Task_copyToResult(task, result);
};

fn_((Sched_seq__cancel(P$raw ctx, P$Future$raw any_future, u_P$raw result))(void)) {
    let self = ptrAlignCast$((exec_Seq*)(ensureNonnull(ctx)));
    let task = ptrAlignCast$((exec_Task*)(ensureNonnull(any_future)));
    Sched__cancel(&self->lane, task, result);
};

fn_((Sched_coop__async(P$raw ctx, u_P$raw result, P$$(Closure$raw) inner))(O$P$Future$raw) $scope) {
    let self = ptrAlignCast$((exec_Coop*)(ensureNonnull(ctx)));
    return Sched__async(&self->timed.lane, result, inner);
} $unscoped(fn);

fn_((Sched_coop__spawn(P$raw ctx, u_P$raw result, P$$(Closure$raw) inner))(Sched_ConcE$P$Future$raw) $scope) {
    let self = ptrAlignCast$((exec_Coop*)(ensureNonnull(ctx)));
    return Sched__spawn(&self->timed.lane, result, inner);
} $unscoped(fn);

fn_((Sched_coop__await(P$raw ctx, P$Future$raw any_future, u_P$raw result))(void)) {
    claim_assert_nonnull(result.raw);
    let self = ptrAlignCast$((exec_Coop*)(ensureNonnull(ctx)));
    let task = ptrAlignCast$((exec_Task*)(ensureNonnull(any_future)));
    while (task->state != exec_Task_State_done && task->state != exec_Task_State_canceled) {
        exec_LaneTimed_wakeDueTimers(&self->timed);
        if (task->state == exec_Task_State_deferred) {
            exec_Lane_runTask(&self->timed.lane, task);
            continue;
        }
        if (exec_Lane_runOneReady(&self->timed.lane)) continue;
        let deadline = orelse_((exec_LaneTimed_nextTimerDeadline(&self->timed))(break));
        let remaining = exec_LaneTimed_remaining(&self->timed, deadline);
        if (time_Dur_isZero(remaining)) continue;
        let_ignore = self->timed.clock.vtbl->sleepFn(self->timed.clock.ctx, remaining);
    }
    if (exec_Task_isDone(task)) exec_Task_copyToResult(task, result);
};

fn_((Sched_coop__cancel(P$raw ctx, P$Future$raw any_future, u_P$raw result))(void)) {
    let self = ptrAlignCast$((exec_Coop*)(ensureNonnull(ctx)));
    let task = ptrAlignCast$((exec_Task*)(ensureNonnull(any_future)));
    Sched__cancel(&self->timed.lane, task, result);
};

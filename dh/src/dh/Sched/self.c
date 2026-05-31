#include "dh/Sched/self.h"
#include "dh/Future/self.h"
#include "dh/exec/Seq.h"
#include "dh/exec/Coop.h"
#include "dh/exec/Preem.h"
#include "dh/exec/Para.h"
#include "dh/exec/LaneTimed.h"
#include "dh/exec/Task.h"

/*========== Internal Declarations ==========================================*/

$attr($inline_always)
$static fn_((Sched__async(exec_Lane* ctx, u_P$raw result, P$$(Clsr$raw) inner))(O$P$FutureAny));
$attr($inline_always $must_check)
$static fn_((Sched__spawn(exec_Lane* ctx, u_P$raw result, P$$(Clsr$raw) inner))(Sched_ConcE$P$FutureAny));
$attr($inline_always)
$static fn_((Sched__cancelLane(exec_Lane* lane, exec_Task* any_future, u_P$raw result))(void));
$attr($inline_always)
$static fn_((Sched__cancelTimed(exec_Lane* lane, exec_LaneTimed* timed, exec_Task* any_future, u_P$raw result))(void));
$attr($inline_always)
$static fn_((Sched__recancelLane(exec_Lane* lane))(void));
$attr($inline_always)
$static fn_((Sched__swapCancelProtnLane(exec_Lane* lane, Sched_CancelProtn new_protection))(Sched_CancelProtn));
$attr($inline_always $must_check)
$static fn_((Sched__idleLane(exec_Lane* lane))(Sched_Cancelable$void));

$static fn_((Sched_seq__async(P$raw ctx, u_P$raw result, P$$(Clsr$raw) inner))(O$P$FutureAny));
$attr($must_check)
$static fn_((Sched_seq__spawn(P$raw ctx, u_P$raw result, P$$(Clsr$raw) inner))(Sched_ConcE$P$FutureAny));
$static fn_((Sched_seq__await(P$raw ctx, P$FutureAny any_future, u_P$raw result))(void));
$static fn_((Sched_seq__cancel(P$raw ctx, P$FutureAny any_future, u_P$raw result))(void));
$static fn_((Sched_seq__recancel(P$raw ctx))(void));
$static fn_((Sched_seq__swapCancelProtn(P$raw ctx, Sched_CancelProtn new_protection))(Sched_CancelProtn));
$attr($must_check)
$static fn_((Sched_seq__idle(P$raw ctx))(Sched_Cancelable$void));

$static fn_((Sched_coop__async(P$raw ctx, u_P$raw result, P$$(Clsr$raw) inner))(O$P$FutureAny));
$attr($must_check)
$static fn_((Sched_coop__spawn(P$raw ctx, u_P$raw result, P$$(Clsr$raw) inner))(Sched_ConcE$P$FutureAny));
$static fn_((Sched_coop__await(P$raw ctx, P$FutureAny any_future, u_P$raw result))(void));
$static fn_((Sched_coop__cancel(P$raw ctx, P$FutureAny any_future, u_P$raw result))(void));
$static fn_((Sched_coop__recancel(P$raw ctx))(void));
$static fn_((Sched_coop__swapCancelProtn(P$raw ctx, Sched_CancelProtn new_protection))(Sched_CancelProtn));
$attr($must_check)
$static fn_((Sched_coop__idle(P$raw ctx))(Sched_Cancelable$void));

$static fn_((Sched_preem__async(P$raw ctx, u_P$raw result, P$$(Clsr$raw) inner))(O$P$FutureAny));
$attr($must_check)
$static fn_((Sched_preem__spawn(P$raw ctx, u_P$raw result, P$$(Clsr$raw) inner))(Sched_ConcE$P$FutureAny));
$static fn_((Sched_preem__await(P$raw ctx, P$FutureAny any_future, u_P$raw result))(void));
$static fn_((Sched_preem__cancel(P$raw ctx, P$FutureAny any_future, u_P$raw result))(void));
$static fn_((Sched_preem__recancel(P$raw ctx))(void));
$static fn_((Sched_preem__swapCancelProtn(P$raw ctx, Sched_CancelProtn new_protection))(Sched_CancelProtn));
$attr($must_check)
$static fn_((Sched_preem__idle(P$raw ctx))(Sched_Cancelable$void));

/*========== External Definitions ===========================================*/

let_(Sched_VTbl_noop, Sched_VTbl) = {
    .asyncFn = Sched_VTbl_noAsync,
    .spawnFn = Sched_VTbl_failingSpawn,
    .awaitFn = Sched_VTbl_noAwait,
    .cancelFn = Sched_VTbl_noCancel,
    .recancelFn = Sched_VTbl_noRecancel,
    .swapCancelProtnFn = Sched_VTbl_noSwapCancelProtn,
    .idleFn = Sched_VTbl_failingIdle,
};

let_(Sched_VTbl_failing, Sched_VTbl) = {
    .asyncFn = Sched_VTbl_noAsync,
    .spawnFn = Sched_VTbl_failingSpawn,
    .awaitFn = Sched_VTbl_unreachableAwait,
    .cancelFn = Sched_VTbl_unreachableCancel,
    .recancelFn = Sched_VTbl_unreachableRecancel,
    .swapCancelProtnFn = Sched_VTbl_unreachableSwapCancelProtn,
    .idleFn = Sched_VTbl_failingIdle,
};

$static var_(Sched_noop__ctx, Void) = cleared();
let_(Sched_noop, Sched) = {
    .ctx = &Sched_noop__ctx,
    .vtbl = &Sched_VTbl_noop,
};

$static var_(Sched_failing__ctx, Void) = cleared();
let_(Sched_failing, Sched) = {
    .ctx = &Sched_failing__ctx,
    .vtbl = &Sched_VTbl_failing,
};

fn_((Sched_async(Sched self, Clsr$raw* clsr, TypeInfo ret_ty, V$Future$raw ret_mem))(V$Future$raw)) {
    claim_assert_nonnull(clsr);
    claim_assert_nonnull(ret_mem);
    self = Sched_ensureValid(self);
    ret_mem->type = $typing(ret_ty);
    let result = Future_resultMut(ret_mem, ret_ty);
    ret_mem->any_future = self.vtbl->asyncFn(self.ctx, result, clsr);
    return ret_mem;
};

fn_((Sched_spawn(Sched self, Clsr$raw* clsr, TypeInfo ret_ty, V$Future$raw ret_mem))(Sched_ConcE$V$Future$raw) $scope) {
    claim_assert_nonnull(clsr);
    claim_assert_nonnull(ret_mem);
    self = Sched_ensureValid(self);
    ret_mem->type = $typing(ret_ty);
    let result = Future_resultMut(ret_mem, ret_ty);
    asg_l((&ret_mem->any_future)(some(try_(self.vtbl->spawnFn(self.ctx, result, clsr)))));
    return_ok(ret_mem);
} $unscoped(fn);

fn_((Sched_recancel(Sched self))(void)) {
    self = Sched_ensureValid(self);
    self.vtbl->recancelFn(self.ctx);
};

fn_((Sched_swapCancelProtn(Sched self, Sched_CancelProtn new_protect))(Sched_CancelProtn)) {
    self = Sched_ensureValid(self);
    return self.vtbl->swapCancelProtnFn(self.ctx, new_protect);
};

fn_((Sched_idle(Sched self))(Sched_Cancelable$void)) {
    self = Sched_ensureValid(self);
    return self.vtbl->idleFn(self.ctx);
};

fn_((Sched_seq(exec_Seq* self))(Sched)) {
    $static let_(vtbl, Sched_VTbl) $like_ref = { {
        .asyncFn = Sched_seq__async,
        .spawnFn = Sched_seq__spawn,
        .awaitFn = Sched_seq__await,
        .cancelFn = Sched_seq__cancel,
        .recancelFn = Sched_seq__recancel,
        .swapCancelProtnFn = Sched_seq__swapCancelProtn,
        .idleFn = Sched_seq__idle,
    } };
    return Sched_ensureValid((Sched){
        .ctx = self,
        .vtbl = vtbl,
    });
};

fn_((Sched_coop(exec_Coop* loop))(Sched)) {
    $static let_(vtbl, Sched_VTbl) $like_ref = { {
        .asyncFn = Sched_coop__async,
        .spawnFn = Sched_coop__spawn,
        .awaitFn = Sched_coop__await,
        .cancelFn = Sched_coop__cancel,
        .recancelFn = Sched_coop__recancel,
        .swapCancelProtnFn = Sched_coop__swapCancelProtn,
        .idleFn = Sched_coop__idle,
    } };
    return Sched_ensureValid((Sched){
        .ctx = loop,
        .vtbl = vtbl,
    });
};

fn_((Sched_preem(exec_Preem* preem))(Sched)) {
    $static let_(vtbl, Sched_VTbl) $like_ref = { {
        .asyncFn = Sched_preem__async,
        .spawnFn = Sched_preem__spawn,
        .awaitFn = Sched_preem__await,
        .cancelFn = Sched_preem__cancel,
        .recancelFn = Sched_preem__recancel,
        .idleFn = Sched_preem__idle,
        .swapCancelProtnFn = Sched_preem__swapCancelProtn,
    } };
    return Sched_ensureValid((Sched){
        .ctx = preem,
        .vtbl = vtbl,
    });
};

fn_((Sched_para(exec_Para* para))(Sched)) {
    $static let_(vtbl, Sched_VTbl) $like_ref = { {
        .asyncFn = Sched_VTbl_noAsync,
        .spawnFn = Sched_VTbl_failingSpawn,
        .awaitFn = Sched_VTbl_unreachableAwait,
        .cancelFn = Sched_VTbl_unreachableCancel,
        .recancelFn = Sched_VTbl_unreachableRecancel,
        .idleFn = Sched_VTbl_failingIdle,
        .swapCancelProtnFn = Sched_VTbl_unreachableSwapCancelProtn,
    } };
    return Sched_ensureValid((Sched){
        .ctx = para,
        .vtbl = vtbl,
    });
};

fn_((Sched_VTbl_noAsync(P$raw ctx, u_P$raw result, P$$(Clsr$raw) inner))(O$P$FutureAny) $scope) {
    let_ignore = ctx;
    let_ignore = result;
    let_ignore = inner;
    return_none();
} $unscoped(fn);

fn_((Sched_VTbl_failingSpawn(P$raw ctx, u_P$raw result, P$$(Clsr$raw) inner))(Sched_ConcE$P$FutureAny) $scope) {
    let_ignore = ctx;
    let_ignore = result;
    let_ignore = inner;
    return_err(E_cause$Sched_ConcUnavailable());
} $unscoped(fn);

fn_((Sched_VTbl_noAwait(P$raw ctx, P$FutureAny any_future, u_P$raw result))(void)) {
    let_ignore = ctx;
    let_ignore = any_future;
    let_ignore = result;
};

fn_((Sched_VTbl_unreachableAwait(P$raw ctx, P$FutureAny any_future, u_P$raw result))(void)) {
    let_ignore = ctx;
    let_ignore = any_future;
    let_ignore = result;
    claim_unreachable;
};

fn_((Sched_VTbl_noCancel(P$raw ctx, P$FutureAny any_future, u_P$raw result))(void)) {
    let_ignore = ctx;
    let_ignore = any_future;
    let_ignore = result;
};

fn_((Sched_VTbl_unreachableCancel(P$raw ctx, P$FutureAny any_future, u_P$raw result))(void)) {
    let_ignore = ctx;
    let_ignore = any_future;
    let_ignore = result;
    claim_unreachable;
};

fn_((Sched_VTbl_noRecancel(P$raw ctx))(void)) {
    let_ignore = ctx;
};

fn_((Sched_VTbl_unreachableRecancel(P$raw ctx))(void)) {
    let_ignore = ctx;
    claim_unreachable;
};

fn_((Sched_VTbl_noSwapCancelProtn(P$raw ctx, Sched_CancelProtn new_protection))(Sched_CancelProtn)) {
    let_ignore = ctx;
    let_ignore = new_protection;
    return Sched_CancelProtn_unblocked;
};

fn_((Sched_VTbl_unreachableSwapCancelProtn(P$raw ctx, Sched_CancelProtn new_protection))(Sched_CancelProtn)) {
    let_ignore = ctx;
    let_ignore = new_protection;
    claim_unreachable;
};

fn_((Sched_VTbl_failingIdle(P$raw ctx))(Sched_Cancelable$void) $scope) {
    let_ignore = ctx;
    return_err(E_cause$Sched_Canceled());
} $unscoped(fn);

/*========== Internal Definitions ===========================================*/

fn_((Sched__async(exec_Lane* ctx, u_P$raw result, P$$(Clsr$raw) inner))(O$P$FutureAny) $scope) {
    claim_assert_nonnull(ctx), claim_assert_nonnull(result.raw), claim_assert_nonnull(inner);
    if_some((exec_Lane_asyncTask(ctx, result, inner))(task)) {
        return_some(task->as_any);
    }
    return_none();
} $unscoped(fn);

fn_((Sched__spawn(exec_Lane* ctx, u_P$raw result, P$$(Clsr$raw) inner))(Sched_ConcE$P$FutureAny) $scope) {
    claim_assert_nonnull(ctx), claim_assert_nonnull(result.raw), claim_assert_nonnull(inner);
    let task = orelse_((exec_Lane_spawnTask(ctx, result, inner))(
        return_err(E_cause$Sched_ConcUnavailable())
    ));
    return_ok(task->as_any);
} $unscoped(fn);

fn_((Sched__cancelLane(exec_Lane* lane, exec_Task* any_future, u_P$raw result))(void)) {
    claim_assert_nonnull(lane), claim_assert_nonnull(any_future), claim_assert_nonnull(result.raw);
    if (exec_Task_isDone(any_future)) {
        return exec_Task_copyToResult(any_future, result);
    }
    exec_Task_requestCancel(any_future);
    return exec_Lane_deliverCancel(lane, any_future);
};

fn_((Sched__cancelTimed(exec_Lane* lane, exec_LaneTimed* timed, exec_Task* any_future, u_P$raw result))(void)) {
    claim_assert_nonnull(lane), claim_assert_nonnull(timed);
    claim_assert_nonnull(any_future), claim_assert_nonnull(result.raw);
    if (exec_Task_isDone(any_future)) {
        return exec_Task_copyToResult(any_future, result);
    }
    exec_Task_requestCancel(any_future);
    return exec_LaneTimed_deliverCancel(timed, any_future);
};

fn_((Sched__recancelLane(exec_Lane* lane))(void)) {
    let task = orelse_((ensureNonnull(lane)->task_curr)(return));
    return exec_Task_recancel(task);
};

fn_((Sched__swapCancelProtnLane(exec_Lane* lane, Sched_CancelProtn new_protection))(Sched_CancelProtn)) {
    if_some((ensureNonnull(lane)->task_curr)(task)) {
        return exec_Task_swapCancelProtn(task, new_protection);
    }
    return Sched_CancelProtn_unblocked;
};

fn_((Sched__idleLane(exec_Lane* lane))(Sched_Cancelable$void) $scope) {
    if_some((ensureNonnull(lane)->task_curr)(task)) {
        return exec_Task_idle(task);
    }
    return_ok({});
} $unscoped(fn);

fn_((Sched_seq__async(P$raw ctx, u_P$raw result, P$$(Clsr$raw) inner))(O$P$FutureAny)) {
    let self = ptrAlignCast$((exec_Seq*)(ensureNonnull(ctx)));
    return Sched__async(&self->lane, result, inner);
};

fn_((Sched_seq__spawn(P$raw ctx, u_P$raw result, P$$(Clsr$raw) inner))(Sched_ConcE$P$FutureAny) $scope) {
    let self = ptrAlignCast$((exec_Seq*)(ensureNonnull(ctx)));
    return Sched__spawn(&self->lane, result, inner);
} $unscoped(fn);

fn_((Sched_seq__await(P$raw ctx, P$FutureAny any_future, u_P$raw result))(void)) {
    claim_assert_nonnull(result.raw);
    let self = ptrAlignCast$((exec_Seq*)(ensureNonnull(ctx)));
    let task = ptrAlignCast$((exec_Task*)(ensureNonnull(any_future)));
    exec_Seq_awaitUntilDone(self, task);
    if (!exec_Task_isDone(task)) return;
    return exec_Task_copyToResult(task, result);
};

fn_((Sched_seq__cancel(P$raw ctx, P$FutureAny any_future, u_P$raw result))(void)) {
    claim_assert_nonnull(result.raw);
    let self = ptrAlignCast$((exec_Seq*)(ensureNonnull(ctx)));
    let task = ptrAlignCast$((exec_Task*)(ensureNonnull(any_future)));
    Sched__cancelLane(&self->lane, task, result);
    exec_Seq_awaitUntilDone(self, task);
    if (!exec_Task_isDone(task)) return;
    return exec_Task_copyToResult(task, result);
};

fn_((Sched_seq__recancel(P$raw ctx))(void)) {
    let self = ptrAlignCast$((exec_Seq*)(ensureNonnull(ctx)));
    return Sched__recancelLane(&self->lane);
};

fn_((Sched_seq__swapCancelProtn(P$raw ctx, Sched_CancelProtn new_protection))(Sched_CancelProtn)) {
    let self = ptrAlignCast$((exec_Seq*)(ensureNonnull(ctx)));
    return Sched__swapCancelProtnLane(&self->lane, new_protection);
};

fn_((Sched_seq__idle(P$raw ctx))(Sched_Cancelable$void)) {
    let self = ptrAlignCast$((exec_Seq*)(ensureNonnull(ctx)));
    return Sched__idleLane(&self->lane);
};

fn_((Sched_coop__async(P$raw ctx, u_P$raw result, P$$(Clsr$raw) inner))(O$P$FutureAny)) {
    let self = ptrAlignCast$((exec_Coop*)(ensureNonnull(ctx)));
    return Sched__async(&self->timed.lane, result, inner);
};

fn_((Sched_coop__spawn(P$raw ctx, u_P$raw result, P$$(Clsr$raw) inner))(Sched_ConcE$P$FutureAny)) {
    let self = ptrAlignCast$((exec_Coop*)(ensureNonnull(ctx)));
    return Sched__spawn(&self->timed.lane, result, inner);
};

fn_((Sched_coop__await(P$raw ctx, P$FutureAny any_future, u_P$raw result))(void)) {
    claim_assert_nonnull(result.raw);
    let self = ptrAlignCast$((exec_Coop*)(ensureNonnull(ctx)));
    let task = ptrAlignCast$((exec_Task*)(ensureNonnull(any_future)));
    exec_Coop_awaitUntilDone(self, task);
    return exec_Task_copyToResult(task, result);
};

fn_((Sched_coop__cancel(P$raw ctx, P$FutureAny any_future, u_P$raw result))(void)) {
    claim_assert_nonnull(result.raw);
    let self = ptrAlignCast$((exec_Coop*)(ensureNonnull(ctx)));
    let task = ptrAlignCast$((exec_Task*)(ensureNonnull(any_future)));
    Sched__cancelTimed(&self->timed.lane, &self->timed, task, result);
    exec_Coop_awaitUntilDone(self, task);
    if (!exec_Task_isDone(task)) return;
    return exec_Task_copyToResult(task, result);
};

fn_((Sched_coop__recancel(P$raw ctx))(void)) {
    let self = ptrAlignCast$((exec_Coop*)(ensureNonnull(ctx)));
    return Sched__recancelLane(&self->timed.lane);
};

fn_((Sched_coop__swapCancelProtn(P$raw ctx, Sched_CancelProtn new_protection))(Sched_CancelProtn)) {
    let self = ptrAlignCast$((exec_Coop*)(ensureNonnull(ctx)));
    return Sched__swapCancelProtnLane(&self->timed.lane, new_protection);
};

fn_((Sched_coop__idle(P$raw ctx))(Sched_Cancelable$void)) {
    let self = ptrAlignCast$((exec_Coop*)(ensureNonnull(ctx)));
    return Sched__idleLane(&self->timed.lane);
};

fn_((Sched_preem__async(P$raw ctx, u_P$raw result, P$$(Clsr$raw) inner))(O$P$FutureAny) $scope) {
    claim_assert_nonnull(result.raw), claim_assert_nonnull(inner);
    let self = ptrAlignCast$((exec_Preem*)(ensureNonnull(ctx)));
    let task = catch_((exec_Preem_createTask(self, result, inner))($ignore, {
        u_memcpy(result, clsr_invokeToComplete(inner, result.type));
        return_none();
    }));
    return_some(task->as_any);
} $unscoped(fn);

fn_((Sched_preem__spawn(P$raw ctx, u_P$raw result, P$$(Clsr$raw) inner))(Sched_ConcE$P$FutureAny) $scope) {
    claim_assert_nonnull(result.raw), claim_assert_nonnull(inner);
    let self = ptrAlignCast$((exec_Preem*)(ensureNonnull(ctx)));
    let task = try_(exec_Preem_createTask(self, result, inner));
    return_ok(task->as_any);
} $unscoped(fn);

fn_((Sched_preem__await(P$raw ctx, P$FutureAny any_future, u_P$raw result))(void)) {
    claim_assert_nonnull(result.raw);
    let self = ptrAlignCast$((exec_Preem*)(ensureNonnull(ctx)));
    let task = ptrAlignCast$((exec_Preem_Task*)(ensureNonnull(any_future)));
    let_ignore = thrd_join(task->thrd);
    if (task->state == exec_Task_State_done) u_memcpy(result, task->result.as_const);
    exec_Preem_unlinkTask(self, task);
    exec_Preem_destroyTask(self, task);
};

fn_((Sched_preem__cancel(P$raw ctx, P$FutureAny any_future, u_P$raw result))(void)) {
    claim_assert_nonnull(result.raw);
    let self = ptrAlignCast$((exec_Preem*)(ensureNonnull(ctx)));
    let task = ptrAlignCast$((exec_Preem_Task*)(ensureNonnull(any_future)));
    if (task->state == exec_Task_State_done) {
        u_memcpy(result, task->result.as_const);
    } else {
        task->state = exec_Task_State_canceled;
        let_ignore = thrd_join(task->thrd);
    }
    exec_Preem_unlinkTask(self, task);
    exec_Preem_destroyTask(self, task);
};

fn_((Sched_preem__recancel(P$raw ctx))(void)) {
    let_ignore = ctx;
    claim_unreachable_msg("`exec_Preem` has no cooperative cancel points");
};

fn_((Sched_preem__swapCancelProtn(
    P$raw ctx,
    Sched_CancelProtn new_protection
))(Sched_CancelProtn)) {
    let_ignore = ctx;
    let_ignore = new_protection;
    return Sched_CancelProtn_unblocked;
};

fn_((Sched_preem__idle(P$raw ctx))(Sched_Cancelable$void) $scope) {
    let_ignore = ctx;
    return_ok({});
} $unscoped(fn);

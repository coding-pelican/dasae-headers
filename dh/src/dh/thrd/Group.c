#include "dh/thrd/Group.h"

$attr($must_check)
$static fn_((thrd_Group__spawnInst(thrd_Group* self, mem_Alctr gpa, Clsr$Void* clsr))(thrd_spawn_E$thrd_Self));

T_alias$((thrd_Group__Inst)(struct thrd_Group__Inst {
    var_(clsr, Clsr$Void*);
}));
$static fn_((thrd_Group__entryInst(thrd_Group* mgr, thrd_Group__Inst inst))(Void));
fn_use_Clsr_((thrd_Group__entryInst)(thrd_Group*, thrd_Group__Inst)(Void));
T_use$((Void)(clsr_invokeToComplete));

$static fn_((thrd_Group__pushThread(thrd_Group* self, thrd_Group_Node* node))(void));
$static fn_((thrd_Group__joinThreads(thrd_Group* self))(void));

fn_((thrd_Group_init(void))(thrd_Group)) {
    return (thrd_Group){
        .latch = thrd_Latch_initPending(0),
        .lock = thrd_Mtx_init(),
        .threads = none$((O$P$thrd_Group_Node)),
    };
};
fn_((thrd_Group_fini(thrd_Group* self))(void)) {
    thrd_Group__joinThreads(self);
    thrd_Mtx_fini(&self->lock);
    thrd_Latch_fini(&self->latch);
};

fn_((thrd_Group_start(thrd_Group* self))(void)) {
    thrd_Latch_start(&self->latch);
};
fn_((thrd_Group_startN(thrd_Group* self, usize n))(void)) {
    thrd_Latch_startN(&self->latch, n);
};
fn_((thrd_Group_spawn(
    thrd_Group* self, mem_Alctr gpa, Clsr$Void* clsr
))(thrd_spawn_E$void) $scope) {
    thrd_Group_start(self);
    let node = u_castP$((thrd_Group_Node*)(catch_((mem_Alctr_create($trace gpa, typeInfo$(thrd_Group_Node)))(err, {
        thrd_Group_finish(self);
        return_err(err);
    }))));
    *node = (thrd_Group_Node){
        .next = none$((O$P$thrd_Group_Node)),
        .gpa = mem_Alctr_ensureValid(gpa),
        .self = cleared(),
    };
    let instance = catch_((thrd_Group__spawnInst(self, gpa, clsr))(err, {
        mem_Alctr_destroy($trace node->gpa, u_anyP(node));
        thrd_Group_finish(self);
        return_err(err);
    }));
    thrd_Group_adopt(self, node, instance);
    return_ok({});
} $unscoped(fn);
fn_((thrd_Group__spawnInst(thrd_Group* self, mem_Alctr gpa, Clsr$Void* clsr))(thrd_spawn_E$thrd_Self) $guard) {
    let thrd_clsr = u_castP$((Clsr_(thrd_Group__entryInst)*)(try_((
        mem_Alctr_create($trace gpa, typeInfo$(Clsr_(thrd_Group__entryInst)))
    ))));
    errdefer_($ignore, mem_Alctr_destroy($trace gpa, u_anyP(thrd_clsr)));
    *thrd_clsr = clsr_((thrd_Group__entryInst)(
        self,
        l$((thrd_Group__Inst){
            .clsr = clsr,
        })
    ));
    let thrd = try_(thrd_spawnOwned(
        thrd_SpawnCfg_default(gpa),
        thrd_clsr->as_raw,
        typeInfo$(Void),
        u_anyP(thrd_clsr)
    ));
    return_ok(thrd);
} $unguarded(fn);
fn_((thrd_Group__entryInst(thrd_Group* mgr, thrd_Group__Inst inst))(Void) $guard) {
    defer_(thrd_Group_finish(mgr));
    clsr_invokeToComplete$Void(inst.clsr);
    return_void();
} $unguarded(fn);
fn_((thrd_Group__pushThread(thrd_Group* self, thrd_Group_Node* node))(void)) {
    thrd_Mtx_lockProtcd(&self->lock);
    node->next = self->threads;
    self->threads = some$((O$P$thrd_Group_Node)(node));
    thrd_Mtx_unlock(&self->lock);
};
fn_((thrd_Group__joinThreads(thrd_Group* self))(void)) {
    thrd_Mtx_lockProtcd(&self->lock);
    var threads = self->threads;
    self->threads = none$((O$P$thrd_Group_Node));
    thrd_Mtx_unlock(&self->lock);
    while_some((threads), node) {
        threads = node->next;
        thrd_join(node->self);
        mem_Alctr_destroy($trace node->gpa, u_anyP(node));
    }
};
fn_((thrd_Group_adopt(
    thrd_Group* self, thrd_Group_Node* node, thrd_Self thrd
))(void)) {
    node->self = thrd;
    thrd_Group__pushThread(self, node);
};
fn_((thrd_Group_finish(thrd_Group* self))(void)) {
    thrd_Latch_finish(&self->latch);
};

fn_((thrd_Group_isDone(thrd_Group* self))(bool)) {
    return thrd_Latch_isDone(&self->latch);
};
fn_((thrd_Group_value(thrd_Group* self))(usize)) {
    return thrd_Latch_value(&self->latch);
};

fn_((thrd_Group_tryWait(thrd_Group* self))(bool)) {
    if (!thrd_Latch_tryWait(&self->latch)) return false;
    return thrd_Group__joinThreads(self), true;
};
fn_((thrd_Group_wait(
    thrd_Group* self, thrd_Wakeable cancel_src
))(Sched_Cancelable$void) $guard) {
    try_(thrd_Latch_wait(&self->latch, cancel_src));
    thrd_Group__joinThreads(self);
    return_ok({});
} $unguarded(fn);
fn_((thrd_Group_waitFor(
    thrd_Group* self, thrd_Wakeable cancel_src, time_Dur timeout
))(Sched_TimedE$void) $guard) {
    try_(thrd_Latch_waitFor(&self->latch, cancel_src, timeout));
    thrd_Group__joinThreads(self);
    return_ok({});
} $unguarded(fn);
fn_((thrd_Group_waitProtcd(thrd_Group* self))(void)) {
    thrd_Latch_waitProtcd(&self->latch);
    thrd_Group__joinThreads(self);
};

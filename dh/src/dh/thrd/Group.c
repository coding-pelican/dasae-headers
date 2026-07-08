#include "dh/thrd/Group.h"

fn_((thrd_Group_init(void))(thrd_Group)) {
    return thrd_Group_init_static();
};

fn_((thrd_Group_fini(thrd_Group* self))(void)) {
    thrd_CancelTok_Src_fini(&self->cancel);
    thrd_Latch_fini(&self->latch);
};

fn_((thrd_Group_start(thrd_Group* self))(void)) {
    thrd_Latch_start(&self->latch);
};

fn_((thrd_Group_startN(thrd_Group* self, usize n))(void)) {
    thrd_Latch_startN(&self->latch, n);
};

fn_((thrd_Group_finish(thrd_Group* self))(void)) {
    thrd_Latch_finish(&self->latch);
};

fn_((thrd_Group_wait(thrd_Group* self))(void)) {
    thrd_Latch_wait(&self->latch);
};

fn_((thrd_Group_timedWait(thrd_Group* self, time_Dur timeout))(thrd_ftx_E$void)) {
    return thrd_Latch_timedWait(&self->latch, timeout);
};

fn_((thrd_Group_isDone(thrd_Group* self))(bool)) {
    return thrd_Latch_isDone(&self->latch);
};

fn_((thrd_Group_value(thrd_Group* self))(usize)) {
    return thrd_Latch_value(&self->latch);
};

fn_((thrd_Group_cancelTok(thrd_Group* self))(thrd_CancelTok)) {
    return thrd_CancelTok_Src_tok(&self->cancel);
};

fn_((thrd_Group_cancel(thrd_Group* self))(void)) {
    thrd_CancelTok_Src_cancel(&self->cancel);
};

fn_((thrd_Group_isCanceled(thrd_Group* self))(bool)) {
    return thrd_CancelTok_Src_isCanceled(&self->cancel);
};

$attr($must_check)
$static fn_((thrd_Group__spawnInst(thrd_Group* self, mem_Alctr gpa, Clsr$Void* clsr))(thrd_spawn_E$thrd_Self));
$extern fn_((thrd__spawnOwned(
    thrd_SpawnCfg cfg,
    Clsr$raw* clsr,
    TypeInfo ret_type,
    u_P$raw owned_clsr
))(thrd_spawn_E$thrd_Self));
T_alias$((thrd_Group__Inst)(struct thrd_Group__Inst {
    var_(clsr, Clsr$Void*);
}));

$static fn_((thrd_Group__entryInst(thrd_Group* mgr, thrd_Group__Inst inst))(Void));
fn_use_Clsr_((thrd_Group__entryInst)(thrd_Group*, thrd_Group__Inst)(Void));

T_use$((Void)(clsr_invokeToComplete));
fn_((thrd_Group_spawn(thrd_Group* self, mem_Alctr gpa, Clsr$Void* clsr))(void) $scope) {
    thrd_Group_start(self);
    let instance = catch_((thrd_Group__spawnInst(self, gpa, clsr))(
        $ignore, $ignore_void clsr_invokeToComplete$Void(clsr)
    ));
    thrd_detach(instance);
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
    let thrd = try_(thrd__spawnOwned(
        l$((thrd_SpawnCfg){
            .gpa = some$((O$mem_Alctr)(gpa)),
            .stack_size = thrd_SpawnCfg_default_stack_size,
        }),
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

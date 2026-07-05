#include "dh/thrd/WaitGroup.h"

#define thrd_WaitGroup__is_waiting (as$(usize)(1) << 0)
#define thrd_WaitGroup__one_pending (as$(usize)(1) << 1)

fn_((thrd_WaitGroup_init(void))(thrd_WaitGroup)) {
    return thrd_WaitGroup_init_static();
};

fn_((thrd_WaitGroup_fini(thrd_WaitGroup* self))(void)) {
    thrd_ResetEvent_fini(&self->event);
    atom_V_store(&self->state, 0, atom_MemOrd_monotonic);
};

fn_((thrd_WaitGroup_start(thrd_WaitGroup* self))(void)) {
    thrd_WaitGroup_startOn(&self->state);
};

fn_((thrd_WaitGroup_startOn(atom_V$usize* state))(void)) {
    let prev_state = atom_V_pri_fetchAdd(state, thrd_WaitGroup__one_pending, atom_MemOrd_monotonic);
    claim_assert((prev_state / thrd_WaitGroup__one_pending) < (usize_limit_max / thrd_WaitGroup__one_pending));
};

fn_((thrd_WaitGroup_startN(thrd_WaitGroup* self, usize n))(void)) {
    thrd_WaitGroup_startNOn(&self->state, n);
};

fn_((thrd_WaitGroup_startNOn(atom_V$usize* state, usize n))(void)) {
    let prev_state = atom_V_pri_fetchAdd(state, thrd_WaitGroup__one_pending * n, atom_MemOrd_monotonic);
    claim_assert((prev_state / thrd_WaitGroup__one_pending) < (usize_limit_max / thrd_WaitGroup__one_pending));
};

fn_((thrd_WaitGroup_finish(thrd_WaitGroup* self))(void)) {
    thrd_WaitGroup_finishOn(&self->state, &self->event);
};

fn_((thrd_WaitGroup_finishOn(atom_V$usize* state, thrd_ResetEvent* event))(void)) {
    let prev_state = atom_V_pri_fetchSub(state, thrd_WaitGroup__one_pending, atom_MemOrd_acq_rel);
    claim_assert((prev_state / thrd_WaitGroup__one_pending) > 0);
    if (prev_state == (thrd_WaitGroup__one_pending | thrd_WaitGroup__is_waiting)) {
        thrd_ResetEvent_set(event);
    }
};

fn_((thrd_WaitGroup_wait(thrd_WaitGroup* self))(void)) {
    thrd_WaitGroup_waitOn(&self->state, &self->event);
};

fn_((thrd_WaitGroup_waitOn(atom_V$usize* state, thrd_ResetEvent* event))(void)) {
    let prev_state = atom_V_pri_fetchAdd(state, thrd_WaitGroup__is_waiting, atom_MemOrd_acquire);
    claim_assert((prev_state & thrd_WaitGroup__is_waiting) == 0);
    if ((prev_state / thrd_WaitGroup__one_pending) > 0) {
        thrd_ResetEvent_wait(event);
    }
};

fn_((thrd_WaitGroup_reset(thrd_WaitGroup* self))(void)) {
    thrd_WaitGroup_resetOn(&self->state, &self->event);
};

fn_((thrd_WaitGroup_resetOn(atom_V$usize* state, thrd_ResetEvent* event))(void)) {
    atom_V_store(state, 0, atom_MemOrd_monotonic);
    thrd_ResetEvent_reset(event);
};

fn_((thrd_WaitGroup_isDone(thrd_WaitGroup* self))(bool)) {
    return thrd_WaitGroup_isDoneOn(&self->state);
};

fn_((thrd_WaitGroup_isDoneOn(atom_V$usize* state))(bool)) {
    let prev_state = atom_V_load(state, atom_MemOrd_acquire);
    claim_assert((prev_state & thrd_WaitGroup__is_waiting) == 0);
    return (prev_state / thrd_WaitGroup__one_pending) == 0;
};

fn_((thrd_WaitGroup_value(thrd_WaitGroup* self))(usize)) {
    return thrd_WaitGroup_valueOn(&self->state);
};

fn_((thrd_WaitGroup_valueOn(atom_V$usize* state))(usize)) {
    return atom_V_load(state, atom_MemOrd_monotonic) / thrd_WaitGroup__one_pending;
};

$attr($must_check)
$static fn_((thrd_WaitGroup__spawnInst(thrd_WaitGroup* self, mem_Alctr gpa, Clsr$Void* clsr))(thrd_spawn_E$thrd_Self));
$extern fn_((thrd__spawnOwned(
    thrd_SpawnCfg cfg,
    Clsr$raw* clsr,
    TypeInfo ret_type,
    u_P$raw owned_clsr
))(thrd_spawn_E$thrd_Self));
T_alias$((thrd_WaitGroup__Inst)(struct thrd_WaitGroup__Inst {
    var_(clsr, Clsr$Void*);
}));

$static fn_((thrd_WaitGroup__entryInst(thrd_WaitGroup* mgr, thrd_WaitGroup__Inst inst))(Void));
fn_use_Clsr_((thrd_WaitGroup__entryInst)(thrd_WaitGroup*, thrd_WaitGroup__Inst)(Void));

T_use$((Void)(clsr_invokeToComplete));
fn_((thrd_WaitGroup_spawn(thrd_WaitGroup* self, mem_Alctr gpa, Clsr$Void* clsr))(void) $scope) {
    thrd_WaitGroup_start(self);
    let instance = catch_((thrd_WaitGroup__spawnInst(self, gpa, clsr))(
        $ignore, $ignore_void clsr_invokeToComplete$Void(clsr)
    ));
    thrd_detach(instance);
} $unscoped(fn);

fn_((thrd_WaitGroup__spawnInst(thrd_WaitGroup* self, mem_Alctr gpa, Clsr$Void* clsr))(thrd_spawn_E$thrd_Self) $guard) {
    let thrd_clsr = u_castP$((Clsr_(thrd_WaitGroup__entryInst)*)(try_((
        mem_Alctr_create($trace gpa, typeInfo$(Clsr_(thrd_WaitGroup__entryInst)))
    ))));
    errdefer_($ignore, mem_Alctr_destroy($trace gpa, u_anyP(thrd_clsr)));
    var_(inst, thrd_WaitGroup__Inst) = {
        .clsr = clsr,
    };
    *thrd_clsr = clsr_((thrd_WaitGroup__entryInst)(self, inst));
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

fn_((thrd_WaitGroup__entryInst(thrd_WaitGroup* mgr, thrd_WaitGroup__Inst inst))(Void) $guard) {
    defer_(thrd_WaitGroup_finish(mgr));
    clsr_invokeToComplete$Void(inst.clsr);
    return_void();
} $unguarded(fn);

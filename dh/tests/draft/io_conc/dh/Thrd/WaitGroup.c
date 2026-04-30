#include "WaitGroup.h"

#define Thrd_WaitGroup__is_waiting (as$(usize)(1) << 0)
#define Thrd_WaitGroup__one_pending (as$(usize)(1) << 1)

fn_((Thrd_WaitGroup_init(void))(Thrd_WaitGroup)) {
    return (Thrd_WaitGroup){
        .state = atom_V_init(0),
        .event = Thrd_ResetEvent_init(),
    };
};

fn_((Thrd_WaitGroup_fini(Thrd_WaitGroup* self))(void)) {
    Thrd_ResetEvent_fini(&self->event);
    atom_V_store(&self->state, 0, atom_MemOrd_monotonic);
};

fn_((Thrd_WaitGroup_start(Thrd_WaitGroup* self))(void)) {
    Thrd_WaitGroup_startOn(&self->state);
};

fn_((Thrd_WaitGroup_startOn(atom_V$usize* state))(void)) {
    let prev_state = atom_V_fetchAdd(state, Thrd_WaitGroup__one_pending, atom_MemOrd_monotonic);
    claim_assert((prev_state / Thrd_WaitGroup__one_pending) < (usize_limit_max / Thrd_WaitGroup__one_pending));
};

fn_((Thrd_WaitGroup_startN(Thrd_WaitGroup* self, usize n))(void)) {
    Thrd_WaitGroup_startNOn(&self->state, n);
};

fn_((Thrd_WaitGroup_startNOn(atom_V$usize* state, usize n))(void)) {
    let prev_state = atom_V_fetchAdd(state, Thrd_WaitGroup__one_pending * n, atom_MemOrd_monotonic);
    claim_assert((prev_state / Thrd_WaitGroup__one_pending) < (usize_limit_max / Thrd_WaitGroup__one_pending));
};

fn_((Thrd_WaitGroup_finish(Thrd_WaitGroup* self))(void)) {
    Thrd_WaitGroup_finishOn(&self->state, &self->event);
};

fn_((Thrd_WaitGroup_finishOn(atom_V$usize* state, Thrd_ResetEvent* event))(void)) {
    let prev_state = atom_V_fetchSub(state, Thrd_WaitGroup__one_pending, atom_MemOrd_acq_rel);
    claim_assert((prev_state / Thrd_WaitGroup__one_pending) > 0);
    if (prev_state == (Thrd_WaitGroup__one_pending | Thrd_WaitGroup__is_waiting)) {
        Thrd_ResetEvent_set(event);
    }
};

fn_((Thrd_WaitGroup_wait(Thrd_WaitGroup* self))(void)) {
    Thrd_WaitGroup_waitOn(&self->state, &self->event);
};

fn_((Thrd_WaitGroup_waitOn(atom_V$usize* state, Thrd_ResetEvent* event))(void)) {
    let prev_state = atom_V_fetchAdd(state, Thrd_WaitGroup__is_waiting, atom_MemOrd_acquire);
    claim_assert((prev_state & Thrd_WaitGroup__is_waiting) == 0);
    if ((prev_state / Thrd_WaitGroup__one_pending) > 0) {
        Thrd_ResetEvent_wait(event);
    }
};

fn_((Thrd_WaitGroup_reset(Thrd_WaitGroup* self))(void)) {
    Thrd_WaitGroup_resetOn(&self->state, &self->event);
};

fn_((Thrd_WaitGroup_resetOn(atom_V$usize* state, Thrd_ResetEvent* event))(void)) {
    atom_V_store(state, 0, atom_MemOrd_monotonic);
    Thrd_ResetEvent_reset(event);
};

fn_((Thrd_WaitGroup_isDone(Thrd_WaitGroup* self))(bool)) {
    return Thrd_WaitGroup_isDoneOn(&self->state);
};

fn_((Thrd_WaitGroup_isDoneOn(atom_V$usize* state))(bool)) {
    let prev_state = atom_V_load(state, atom_MemOrd_acquire);
    claim_assert((prev_state & Thrd_WaitGroup__is_waiting) == 0);
    return (prev_state / Thrd_WaitGroup__one_pending) == 0;
};

fn_((Thrd_WaitGroup_value(Thrd_WaitGroup* self))(usize)) {
    return Thrd_WaitGroup_valueOn(&self->state);
};

fn_((Thrd_WaitGroup_valueOn(atom_V$usize* state))(usize)) {
    return atom_V_load(state, atom_MemOrd_monotonic) / Thrd_WaitGroup__one_pending;
};

$attr($must_check)
$static fn_((Thrd_WaitGroup__spawnInst(Thrd_WaitGroup* self, mem_Alctr gpa, Closure$Void* closure))(E$Thrd));
T_alias$((Thrd_WaitGroup__Inst)(struct Thrd_WaitGroup__Inst {
    var_(closure, Closure$Void*);
}));

$static fn_((Thrd_WaitGroup__entryInst(Thrd_WaitGroup* mgr, mem_Alctr gpa, Closure$raw* self_closure, Thrd_WaitGroup__Inst inst))(Void));
fn_use_Closure_((Thrd_WaitGroup__entryInst)(Thrd_WaitGroup*, mem_Alctr, Closure$raw*, Thrd_WaitGroup__Inst)(Void));

T_use$((Void)(Closure_invokeToComplete));
fn_((Thrd_WaitGroup_spawn(Thrd_WaitGroup* self, mem_Alctr gpa, Closure$Void* closure))(void) $scope) {
    Thrd_WaitGroup_start(self);
    let instance = catch_((Thrd_WaitGroup__spawnInst(self, gpa, closure))(
        $ignore, $ignore_void Closure_invokeToComplete$Void(closure)
    ));
    Thrd_detach(instance);
} $unscoped(fn);

fn_((Thrd_WaitGroup__spawnInst(Thrd_WaitGroup* self, mem_Alctr gpa, Closure$Void* closure))(E$Thrd) $guard) {
    let thrd_closure = u_castP$((Closure_(Thrd_WaitGroup__entryInst)*)(try_((
        mem_Alctr_create($trace gpa, typeInfo$(Closure_(Thrd_WaitGroup__entryInst)))
    ))));
    errdefer_($ignore, mem_Alctr_destroy($trace gpa, u_anyP(thrd_closure)));
    *thrd_closure = closure_(Thrd_WaitGroup__entryInst)(
        self, gpa, thrd_closure->as_raw,
        l$((Thrd_WaitGroup__Inst){
            .closure = closure,
        })
    );
    let thrd = try_(Thrd_spawn(
        (Thrd_SpawnCfg){
            .gpa = some$((O$mem_Alctr)(gpa)),
            .stack_size = Thrd_SpawnCfg_default_stack_size,
        },
        thrd_closure->as_raw,
        typeInfo$(Void)
    ));
    return_ok(thrd);
} $unguarded(fn);

fn_((Thrd_WaitGroup__entryInst(Thrd_WaitGroup* mgr, mem_Alctr gpa, Closure$raw* self_closure, Thrd_WaitGroup__Inst inst))(Void) $guard) {
    defer_(Thrd_WaitGroup_finish(mgr));
    defer_(mem_Alctr_destroy($trace gpa, u_anyP(self_closure)));
    Closure_invokeToComplete$Void(inst.closure);
    return_void();
} $unguarded(fn);

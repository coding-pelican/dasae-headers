#include "dh/Thrd/WaitGroup.h"

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
    let state = atom_V_fetchAdd(&self->state, Thrd_WaitGroup__one_pending, atom_MemOrd_monotonic);
    debug_assert((state / Thrd_WaitGroup__one_pending) < (usize_limit_max / Thrd_WaitGroup__one_pending));
    let_ignore = state;
};

fn_((Thrd_WaitGroup_startN(Thrd_WaitGroup* self, usize n))(void)) {
    let state = atom_V_fetchAdd(&self->state, Thrd_WaitGroup__one_pending * n, atom_MemOrd_monotonic);
    debug_assert((state / Thrd_WaitGroup__one_pending) < (usize_limit_max / Thrd_WaitGroup__one_pending));
    let_ignore = state;
};

fn_((Thrd_WaitGroup_finish(Thrd_WaitGroup* self))(void)) {
    let state = atom_V_fetchSub(&self->state, Thrd_WaitGroup__one_pending, atom_MemOrd_acq_rel);
    debug_assert((state / Thrd_WaitGroup__one_pending) > 0);
    if (state == (Thrd_WaitGroup__one_pending | Thrd_WaitGroup__is_waiting)) {
        Thrd_ResetEvent_set(&self->event);
    }
};

fn_((Thrd_WaitGroup_wait(Thrd_WaitGroup* self))(void)) {
    let state = atom_V_fetchAdd(&self->state, Thrd_WaitGroup__is_waiting, atom_MemOrd_acquire);
    debug_assert((state & Thrd_WaitGroup__is_waiting) == 0);
    if ((state / Thrd_WaitGroup__one_pending) > 0) {
        Thrd_ResetEvent_wait(&self->event);
    }
};

fn_((Thrd_WaitGroup_reset(Thrd_WaitGroup* self))(void)) {
    atom_V_store(&self->state, 0, atom_MemOrd_monotonic);
    Thrd_ResetEvent_reset(&self->event);
};

fn_((Thrd_WaitGroup_isDone(Thrd_WaitGroup* self))(bool)) {
    let state = atom_V_load(&self->state, atom_MemOrd_acquire);
    debug_assert((state & Thrd_WaitGroup__is_waiting) == 0);
    return (state / Thrd_WaitGroup__one_pending) == 0;
};

$attr($must_check)
$static fn_((Thrd_WaitGroup__spawnInst(Thrd_WaitGroup* self, mem_Allocator gpa, Thrd_FnCtx* fn_ctx))(E$Thrd));
$attr($inline_always)
$static fn_((Thrd_WaitGroup__runInst(Thrd_FnCtx* fn_ctx))(void));
fn_((Thrd_WaitGroup_spawn(Thrd_WaitGroup* self, mem_Allocator gpa, Thrd_FnCtx* fn_ctx))(void) $scope) {
    Thrd_WaitGroup_start(self);
    let instance = catch_((Thrd_WaitGroup__spawnInst(self, gpa, fn_ctx))(
        $ignore, Thrd_WaitGroup__runInst(fn_ctx)
    ));
    Thrd_detach(instance);
} $unscoped_(fn);

$static Thrd_fn_(Thrd_WaitGroup__entryInst, ({ Thrd_WaitGroup* mgr; mem_Allocator gpa; Thrd_FnCtx* fn_ctx; }, Void));
fn_((Thrd_WaitGroup__spawnInst(Thrd_WaitGroup* self, mem_Allocator gpa, Thrd_FnCtx* fn_ctx))(E$Thrd) $guard) {
    let thrd_ctx = u_castP$((Thrd_FnCtx$(Thrd_WaitGroup__entryInst)*)(try_((mem_Allocator_create(gpa, typeInfo$(InnerType))))));
    errdefer_($ignore, mem_Allocator_destroy(gpa, u_anyP(thrd_ctx)));
    *thrd_ctx = Thrd_FnCtx_from$((Thrd_WaitGroup__entryInst)(self, gpa, fn_ctx));
    let thrd = try_(Thrd_spawn(Thrd_SpawnCfg_default, thrd_ctx->as_raw));
    return_ok(thrd);
} $unguarded_(fn);

Thrd_fn_(Thrd_WaitGroup__entryInst, ($ignore, args)$guard) {
    let mgr = args->mgr;
    defer_(Thrd_WaitGroup_finish(mgr));
    let gpa = args->gpa;
    let fn_ctx = args->fn_ctx;
    defer_(mem_Allocator_destroy(gpa, u_anyP(fn_ctx)));
    return_void(Thrd_WaitGroup__runInst(fn_ctx));
} $unguarded_(Thrd_fn);

$attr($inline_always)
$static fn_((Thrd_WaitGroup__runInst(Thrd_FnCtx* fn_ctx))(void)) {
    let_ignore = fn_ctx->fn(fn_ctx);
};

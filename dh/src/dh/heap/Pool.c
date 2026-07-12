#include "dh/heap/Pool.h"
#include "dh/mem/common.h"

/*========== Internal Declarations ==========================================*/

$attr($inline_always)
$static fn_((heap_Pool__slot_size(TypeInfo type))(usize));
$attr($inline_always)
$static fn_((heap_Pool__slot_align(TypeInfo type))(mem_Align));

$attr($inline_always)
$static fn_((heap_Pool__pushFree(heap_Pool* self, TypeInfo type, P$ListSgl_Link link))(void));
$static fn_((heap_Pool__allocNew(heap_Pool* self, TypeInfo type, mem_Alctr child_alctr))(mem_E$u_P$raw));

/*========== External Definitions ===========================================*/

fn_((heap_Pool_empty(TypeInfo type, heap_Pool_Opts opts))(heap_Pool)) {
    return (heap_Pool){
        .arena_state = heap_Arena_State_default(),
        .free_list = ListSgl_empty(type),
        .type = $typing(type),
        .opts = opts,
    };
};
fn_((heap_Pool_init(TypeInfo type, mem_Alctr child_alctr, usize cap, heap_Pool_Opts opts))(mem_E$heap_Pool) $scope) {
    var pool = heap_Pool_empty(type, opts);
    try_(heap_Pool_addCap(&pool, type, child_alctr, cap));
    return_ok(pool);
} $unscoped(fn);
fn_((heap_Pool_fini(heap_Pool* self, mem_Alctr child_alctr))(void)) {
    claim_assert_nonnull(self);
    var arena = heap_Arena_State_promote(self->arena_state, child_alctr);
    heap_Arena_fini(&arena);
    asg_l((self)(cleared()));
};
fn_((heap_Pool_addCap(heap_Pool* self, TypeInfo type, mem_Alctr child_alctr, usize additional))(mem_E$void) $scope) {
    claim_assert_nonnull(self);
    debug_assert_eqBy($typed(self->type), type, TypeInfo_eql);
    for_(($rt(additional))($ignore)) {
        let ptr = try_(heap_Pool__allocNew(self, type, child_alctr));
        heap_Pool__pushFree(self, type, ptrAlignCast$((P$ListSgl_Link)(ptr.raw)));
    } $end(for);
    return_ok({});
} $unscoped(fn);
fn_((heap_Pool_reset(heap_Pool* self, TypeInfo type, mem_Alctr child_alctr, heap_Pool_ResetMode mode))(bool)) {
    claim_assert_nonnull(self);
    debug_assert_eqBy($typed(self->type), type, TypeInfo_eql);
    var arena = heap_Arena_State_promote(self->arena_state, child_alctr);
    let reset_successful = heap_Arena_reset(&arena, mode);
    self->arena_state = arena.state;
    self->free_list = ListSgl_empty(type);
    return reset_successful;
};

fn_((heap_Pool_create(heap_Pool* self, TypeInfo type, mem_Alctr child_alctr))(mem_E$u_P$raw) $scope) {
    claim_assert_nonnull(self);
    debug_assert_eqBy($typed(self->type), type, TypeInfo_eql);
    let ptr = expr_(P$raw $scope)(if_some((ListSgl_shift(&self->free_list))(link)) {
        $break_(as$(P$raw)(link));
    } else if (self->opts.growable) {
        $break_(try_(heap_Pool__allocNew(self, type, child_alctr)).raw);
    } else {
        return_err(E_cause$OutOfMemory());
    }) $unscoped(expr);
    mem_set0Bytes(P_prefix$((S$u8)(ptr)(type.size)));
    return_ok({
        .raw = ptr,
        .type = type,
    });
} $unscoped(fn);
fn_((heap_Pool_destroy(heap_Pool* self, u_P$raw ptr))(void)) {
    claim_assert_nonnull(self);
    debug_assert_eqBy($typed(self->type), ptr.type, TypeInfo_eql);
    mem_set0Bytes(P_prefix$((S$u8)(ptr.raw)(ptr.type.size)));
    heap_Pool__pushFree(self, ptr.type, ptrAlignCast$((P$ListSgl_Link)(ptr.raw)));
};

/*========== Internal Definitions ===========================================*/

fn_((heap_Pool__slot_size(TypeInfo type))(usize)) {
    return pri_max(TypeInfo_size(type), sizeOf$(ListSgl_Link));
};
fn_((heap_Pool__slot_align(TypeInfo type))(mem_Align)) {
    return pri_max(TypeInfo_log2_align(type), alignOfLog2$(ListSgl_Link));
};

fn_((heap_Pool__pushFree(heap_Pool* self, TypeInfo type, P$ListSgl_Link link))(void)) {
    *link = ListSgl_Link_empty(type);
    ListSgl_prepend(&self->free_list, link);
};
fn_((heap_Pool__allocNew(heap_Pool* self, TypeInfo type, mem_Alctr child_alctr))(mem_E$u_P$raw) $scope) {
    var arena = heap_Arena_State_promote(self->arena_state, child_alctr);
    let arena_alctr = heap_Arena_alctr(&arena);
    let ptr = orelse_((mem_Alctr_rawAlloc(
        $trace arena_alctr,
        heap_Pool__slot_size(type),
        heap_Pool__slot_align(type)
    ))(
        self->arena_state = arena.state,
        return_err(E_cause$OutOfMemory())
    ));
    self->arena_state = arena.state;
    return_ok({
        .raw = ptr,
        .type = type,
    });
} $unscoped(fn);

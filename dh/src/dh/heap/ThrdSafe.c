#include "dh/heap/ThrdSafe.h"

/*========== Internal Declarations ==========================================*/

$static fn_((heap_ThrdSafe__alloc(P$raw ctx, usize len, mem_Align align))(O$P$u8));
$static fn_((heap_ThrdSafe__resize(P$raw ctx, S$u8 buf, mem_Align buf_align, usize new_len))(bool));
$static fn_((heap_ThrdSafe__remap(P$raw ctx, S$u8 buf, mem_Align buf_align, usize new_len))(O$P$u8));
$static fn_((heap_ThrdSafe__free(P$raw ctx, S$u8 buf, mem_Align buf_align))(void));

/*========== External Definitions ===========================================*/

fn_((heap_ThrdSafe_alctr(heap_ThrdSafe* self))(mem_Alctr)) {
    // VTable for Thread-safe allocator
    $static let_(vtbl, mem_Alctr_VTbl) = {
        .allocFn = heap_ThrdSafe__alloc,
        .resizeFn = heap_ThrdSafe__resize,
        .remapFn = heap_ThrdSafe__remap,
        .freeFn = heap_ThrdSafe__free,
    };
    return mem_Alctr_ensureValid((mem_Alctr){
        .ctx = self,
        .vtbl = &vtbl,
    });
};
fn_((heap_ThrdSafe_init(mem_Alctr child_alctr))(heap_ThrdSafe)) {
    return (heap_ThrdSafe){
        .child_alctr = mem_Alctr_ensureValid(child_alctr),
        .mtx = thrd_Mtx_init(),
    };
}
fn_((heap_ThrdSafe_fini(heap_ThrdSafe* self))(void)) {
    thrd_Mtx_fini(&self->mtx);
    asg_l((self)(cleared()));
};

/*========== Internal Definitions ===========================================*/

fn_((heap_ThrdSafe__alloc(P$raw ctx, usize len, mem_Align align))(O$P$u8) $guard) {
    claim_assert_nonnull(ctx);
    let self = as$(heap_ThrdSafe*)(ctx);
    thrd_Mtx_lockProtcd(&self->mtx);
    defer_(thrd_Mtx_unlock(&self->mtx));
    return_(mem_Alctr_rawAlloc($trace self->child_alctr, len, align));
} $unguarded(fn);

fn_((heap_ThrdSafe__resize(P$raw ctx, S$u8 buf, mem_Align buf_align, usize new_len))(bool) $guard) {
    claim_assert_nonnull(ctx);
    let self = as$(heap_ThrdSafe*)(ctx);
    thrd_Mtx_lockProtcd(&self->mtx);
    defer_(thrd_Mtx_unlock(&self->mtx));
    return_(mem_Alctr_rawResize($trace self->child_alctr, buf, buf_align, new_len));
} $unguarded(fn);

fn_((heap_ThrdSafe__remap(P$raw ctx, S$u8 buf, mem_Align buf_align, usize new_len))(O$P$u8) $guard) {
    claim_assert_nonnull(ctx);
    let self = as$(heap_ThrdSafe*)(ctx);
    thrd_Mtx_lockProtcd(&self->mtx);
    defer_(thrd_Mtx_unlock(&self->mtx));
    return_(mem_Alctr_rawRemap($trace self->child_alctr, buf, buf_align, new_len));
} $unguarded(fn);

fn_((heap_ThrdSafe__free(P$raw ctx, S$u8 buf, mem_Align buf_align))(void) $guard) {
    claim_assert_nonnull(ctx);
    let self = as$(heap_ThrdSafe*)(ctx);
    thrd_Mtx_lockProtcd(&self->mtx);
    defer_(thrd_Mtx_unlock(&self->mtx));
    return_void(mem_Alctr_rawFree($trace self->child_alctr, buf, buf_align));
} $unguarded(fn);

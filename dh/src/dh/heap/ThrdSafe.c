#include "dh/heap/ThrdSafe.h"

/*========== Internal Declarations ==========================================*/

$static fn_((heap_ThrdSafe__alloc(P$raw ctx, usize len, mem_Align align))(O$P$u8));
$static fn_((heap_ThrdSafe__resize(P$raw ctx, S$u8 buf, mem_Align buf_align, usize new_len))(bool));
$static fn_((heap_ThrdSafe__remap(P$raw ctx, S$u8 buf, mem_Align buf_align, usize new_len))(O$P$u8));
$static fn_((heap_ThrdSafe__free(P$raw ctx, S$u8 buf, mem_Align buf_align))(void));

/*========== External Definitions ===========================================*/

fn_((heap_ThrdSafe_alctr(heap_ThrdSafe* self))(mem_Alctr)) {
    // VTable for Thread-safe allocator
    $static const mem_Alctr_VTbl vtbl $like_ref = { {
        .alloc = heap_ThrdSafe__alloc,
        .resize = heap_ThrdSafe__resize,
        .remap = heap_ThrdSafe__remap,
        .free = heap_ThrdSafe__free,
    } };
    return mem_Alctr_ensureValid((mem_Alctr){
        .ctx = self,
        .vtbl = vtbl,
    });
};

/*========== Internal Definitions ===========================================*/

fn_((heap_ThrdSafe__alloc(P$raw ctx, usize len, mem_Align align))(O$P$u8) $guard) {
    claim_assert_nonnull(ctx);
    let self = as$(heap_ThrdSafe*)(ctx);
    Thrd_Mtx_lock(&self->mtx);
    defer_(Thrd_Mtx_unlock(&self->mtx));
    return_(mem_Alctr_rawAlloc($trace self->child_alctr, len, align));
} $unguarded(fn);

fn_((heap_ThrdSafe__resize(P$raw ctx, S$u8 buf, mem_Align buf_align, usize new_len))(bool) $guard) {
    claim_assert_nonnull(ctx);
    let self = as$(heap_ThrdSafe*)(ctx);
    Thrd_Mtx_lock(&self->mtx);
    defer_(Thrd_Mtx_unlock(&self->mtx));
    return_(mem_Alctr_rawResize($trace self->child_alctr, buf, buf_align, new_len));
} $unguarded(fn);

fn_((heap_ThrdSafe__remap(P$raw ctx, S$u8 buf, mem_Align buf_align, usize new_len))(O$P$u8) $guard) {
    claim_assert_nonnull(ctx);
    let self = as$(heap_ThrdSafe*)(ctx);
    Thrd_Mtx_lock(&self->mtx);
    defer_(Thrd_Mtx_unlock(&self->mtx));
    return_(mem_Alctr_rawRemap($trace self->child_alctr, buf, buf_align, new_len));
} $unguarded(fn);

fn_((heap_ThrdSafe__free(P$raw ctx, S$u8 buf, mem_Align buf_align))(void) $guard) {
    claim_assert_nonnull(ctx);
    let self = as$(heap_ThrdSafe*)(ctx);
    Thrd_Mtx_lock(&self->mtx);
    defer_(Thrd_Mtx_unlock(&self->mtx));
    return_void(mem_Alctr_rawFree($trace self->child_alctr, buf, buf_align));
} $unguarded(fn);

#include "dh/heap/ThrdSafe.h"

/*========== Internal Declarations ==========================================*/

$static fn_((heap_ThrdSafe__alloc(P$raw ctx, usize len, mem_Align align))(O$P$u8));
$static fn_((heap_ThrdSafe__resize(P$raw ctx, S$u8 buf, mem_Align buf_align, usize new_len))(bool));
$static fn_((heap_ThrdSafe__remap(P$raw ctx, S$u8 buf, mem_Align buf_align, usize new_len))(O$P$u8));
$static fn_((heap_ThrdSafe__free(P$raw ctx, S$u8 buf, mem_Align buf_align))(void));

/*========== External Definitions ===========================================*/

fn_((heap_ThrdSafe_allocator(heap_ThrdSafe* self))(mem_Allocator)) {
    // VTable for Thread-safe allocator
    $static const mem_Allocator_VT vt $like_ref = { {
        .alloc = heap_ThrdSafe__alloc,
        .resize = heap_ThrdSafe__resize,
        .remap = heap_ThrdSafe__remap,
        .free = heap_ThrdSafe__free,
    } };
    return mem_Allocator_ensureValid((mem_Allocator){
        .ctx = self,
        .vt = vt,
    });
};

/*========== Internal Definitions ===========================================*/

fn_((heap_ThrdSafe__alloc(P$raw ctx, usize len, mem_Align align))(O$P$u8) $guard) {
    claim_assert_nonnull(ctx);
    let self = as$(heap_ThrdSafe*)(ctx);
    Thrd_Mtx_lock(&self->mtx);
    defer_(Thrd_Mtx_unlock(&self->mtx));
    return_(mem_Allocator_rawAlloc(self->child_allocator, len, align));
} $unguarded_(fn);

fn_((heap_ThrdSafe__resize(P$raw ctx, S$u8 buf, mem_Align buf_align, usize new_len))(bool) $guard) {
    claim_assert_nonnull(ctx);
    let self = as$(heap_ThrdSafe*)(ctx);
    Thrd_Mtx_lock(&self->mtx);
    defer_(Thrd_Mtx_unlock(&self->mtx));
    return_(mem_Allocator_rawResize(self->child_allocator, buf, buf_align, new_len));
} $unguarded_(fn);

fn_((heap_ThrdSafe__remap(P$raw ctx, S$u8 buf, mem_Align buf_align, usize new_len))(O$P$u8) $guard) {
    claim_assert_nonnull(ctx);
    let self = as$(heap_ThrdSafe*)(ctx);
    Thrd_Mtx_lock(&self->mtx);
    defer_(Thrd_Mtx_unlock(&self->mtx));
    return_(mem_Allocator_rawRemap(self->child_allocator, buf, buf_align, new_len));
} $unguarded_(fn);

fn_((heap_ThrdSafe__free(P$raw ctx, S$u8 buf, mem_Align buf_align))(void) $guard) {
    claim_assert_nonnull(ctx);
    let self = as$(heap_ThrdSafe*)(ctx);
    Thrd_Mtx_lock(&self->mtx);
    defer_(Thrd_Mtx_unlock(&self->mtx));
    return_void(mem_Allocator_rawFree(self->child_allocator, buf, buf_align));
} $unguarded_(fn);

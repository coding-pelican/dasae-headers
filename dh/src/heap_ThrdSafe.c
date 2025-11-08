#include "dh/heap/ThrdSafe.h"

// Forward declarations for allocator vtable functions
static fn_((heap_ThrdSafe_alloc(P$raw ctx, usize len, u8 align))(O$P$u8));
static fn_((heap_ThrdSafe_resize(P$raw ctx, S$u8 buf, u8 buf_align, usize new_len))(bool));
static fn_((heap_ThrdSafe_remap(P$raw ctx, S$u8 buf, u8 buf_align, usize new_len))(O$P$u8));
static fn_((heap_ThrdSafe_free(P$raw ctx, S$u8 buf, u8 buf_align))(void));

fn_((heap_ThrdSafe_allocator(heap_ThrdSafe* self))(mem_Allocator)) {
    claim_assert_nonnull(self);
    // VTable for Thread-safe allocator
    static const mem_Allocator_VT vt[1] = { {
        .alloc = heap_ThrdSafe_alloc,
        .resize = heap_ThrdSafe_resize,
        .remap = heap_ThrdSafe_remap,
        .free = heap_ThrdSafe_free,
    } };
    return (mem_Allocator){
        .ctx = self,
        .vt = vt
    };
}

fn_((heap_ThrdSafe_alloc(P$raw ctx, usize len, u8 align))(O$P$u8) $guard) {
    claim_assert_nonnull(ctx);
    let self = as$((heap_ThrdSafe*)(ctx));
    Thrd_Mtx_lock(&self->mutex);
    defer_(Thrd_Mtx_unlock(&self->mutex));
    return_(mem_Allocator_rawAlloc(self->child_allocator, len, align));
} $unguarded_(fn);

fn_((heap_ThrdSafe_resize(P$raw ctx, S$u8 buf, u8 buf_align, usize new_len))(bool) $guard) {
    claim_assert_nonnull(ctx);
    let self = as$((heap_ThrdSafe*)(ctx));
    Thrd_Mtx_lock(&self->mutex);
    defer_(Thrd_Mtx_unlock(&self->mutex));
    return_(mem_Allocator_rawResize(self->child_allocator, buf, buf_align, new_len));
} $unguarded_(fn);

fn_((heap_ThrdSafe_remap(P$raw ctx, S$u8 buf, u8 buf_align, usize new_len))(O$P$u8) $guard) {
    claim_assert_nonnull(ctx);
    let self = as$((heap_ThrdSafe*)(ctx));
    Thrd_Mtx_lock(&self->mutex);
    defer_(Thrd_Mtx_unlock(&self->mutex));
    return_(mem_Allocator_rawRemap(self->child_allocator, buf, buf_align, new_len));
} $unguarded_(fn);

fn_((heap_ThrdSafe_free(P$raw ctx, S$u8 buf, u8 buf_align))(void) $guard) {
    claim_assert_nonnull(ctx);
    let self = as$((heap_ThrdSafe*)(ctx));
    Thrd_Mtx_lock(&self->mutex);
    defer_(Thrd_Mtx_unlock(&self->mutex));
    return_void(mem_Allocator_rawFree(self->child_allocator, buf, buf_align));
} $unguarded_(fn);

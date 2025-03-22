#include "dh/heap/Fixed.h"
#include "dh/mem/common.h"
#include <stdatomic.h>

// Forward declarations for allocator vtable functions
static fn_(heap_Fixed_alloc(anyptr ctx, usize len, u32 align), Opt$Ptr$u8);
static fn_(heap_Fixed_resize(anyptr ctx, Sli$u8 buf, u32 buf_align, usize new_size), bool);
static fn_(heap_Fixed_remap(anyptr ctx, Sli$u8 buf, u32 buf_align, usize new_size), Opt$Ptr$u8);
static fn_(heap_Fixed_free(anyptr ctx, Sli$u8 buf, u32 buf_align), void);

// Thread-safe variants
static fn_(heap_Fixed_thrdSafeAlloc(anyptr ctx, usize len, u32 align), Opt$Ptr$u8);

// Utility functions
static_inline fn_(heap_Fixed_sliContainsPtr(Sli_const$u8 container, Ptr_const$u8 ptr), bool);
static_inline fn_(heap_Fixed_sliContainsSli(Sli_const$u8 container, Sli_const$u8 sli), bool);

fn_(heap_Fixed_allocator(heap_Fixed* self), mem_Allocator) {
    /* VTable for FixedBuf allocator */
    static const mem_Allocator_VT vt[1] = { {
        .alloc  = heap_Fixed_alloc,
        .resize = heap_Fixed_resize,
        .remap  = heap_Fixed_remap,
        .free   = heap_Fixed_free,
    } };
    return (mem_Allocator){
        .ptr = self,
        .vt  = vt
    };
}

fn_(heap_Fixed_thrdSafeAllocator(heap_Fixed* self), mem_Allocator) {
    /* Thread-safe VTable for FixedBuf allocator */
    static const mem_Allocator_VT vt[1] = { {
        .alloc  = heap_Fixed_thrdSafeAlloc,
        .resize = mem_Allocator_VT_noResize,
        .remap  = mem_Allocator_VT_noRemap,
        .free   = mem_Allocator_VT_noFree,
    } };
    return (mem_Allocator){
        .ptr = self,
        .vt  = vt
    };
}

fn_(heap_Fixed_init(heap_Fixed* self, Sli$u8 buf), void) {
    debug_assert_nonnull(self);
    debug_assert_nonnull(buf.ptr);

    *self = (heap_Fixed){
        .buffer    = buf,
        .end_index = 0,
    };
}

fn_(heap_Fixed_reset(heap_Fixed* self), void) {
    debug_assert_nonnull(self);
    self->end_index = 0;
}

fn_(heap_Fixed_ownsPtr(const heap_Fixed* self, Ptr_const$u8 ptr), bool) {
    debug_assert_nonnull(self);
    return heap_Fixed_sliContainsPtr(self->buffer.as_const, ptr);
}

fn_(heap_Fixed_ownsSli(const heap_Fixed* self, Sli_const$u8 sli), bool) {
    debug_assert_nonnull(self);
    return heap_Fixed_sliContainsSli(self->buffer.as_const, sli);
}

fn_(heap_Fixed_isLastAllocation(const heap_Fixed* self, Sli_const$u8 buf), bool) {
    debug_assert_nonnull(self);
    // Check if this is the last allocation
    // This has false negatives when the last allocation had an alignment adjustment
    let buf_end = rawptrToInt(buf.ptr) + buf.len;
    let mem_end = rawptrToInt(self->buffer.ptr) + self->end_index;
    return buf_end == mem_end;
}

/*========== Allocator Interface Implementation =============================*/

static fn_ext_scope(heap_Fixed_alloc(anyptr ctx, usize len, u32 align), Opt$Ptr$u8) {
    debug_assert_nonnull(ctx);
    debug_assert_fmt(mem_isValidAlign(align), "Alignment must be a power of 2");

    let self = as$(heap_Fixed*, ctx);

    // Calculate aligned offset
    let ptr_addr       = rawptrToInt(self->buffer.ptr) + self->end_index;
    let aligned_addr   = mem_alignForward(ptr_addr, align);
    let adjust_off     = aligned_addr - ptr_addr;
    let adjusted_index = self->end_index + adjust_off;
    let new_end_index  = adjusted_index + len;

    // Check if we have enough space
    if (self->buffer.len < new_end_index) { return_none(); }

    // Update allocation position
    self->end_index = new_end_index;
    return_some(intToRawptr$(u8*, aligned_addr));
} ext_unscoped;

static fn_(heap_Fixed_resize(anyptr ctx, Sli$u8 buf, u32 buf_align, usize new_size), bool) {
    debug_assert_nonnull(ctx);
    debug_assert_fmt(mem_isValidAlign(buf_align), "Alignment must be a power of 2");

    let self = as$(heap_Fixed*, ctx);
    unused(buf_align);

    // Verify buffer ownership
    debug_assert_fmt(
        heap_Fixed_ownsSli(self, buf.as_const),
        "Buffer not owned by this allocator"
    );

    // If it's not the last allocation, we can only shrink
    if (!heap_Fixed_isLastAllocation(self, buf.as_const)) {
        return new_size <= buf.len;
    }

    // If it's the last allocation, we can resize
    if (new_size <= buf.len) {
        // Shrink
        let reduction = buf.len - new_size;
        self->end_index -= reduction;
        return true;
    }
    // Expand
    let addition = new_size - buf.len;
    if (self->buffer.len < self->end_index + addition) {
        return false;
    }
    self->end_index += addition;
    return true;
}

static fn_ext_scope(heap_Fixed_remap(anyptr ctx, Sli$u8 buf, u32 buf_align, usize new_size), Opt$Ptr$u8) {
    debug_assert_nonnull(ctx);
    debug_assert_fmt(mem_isValidAlign(buf_align), "Alignment must be a power of 2");

    if (heap_Fixed_resize(ctx, buf, buf_align, new_size)) {
        return_some(buf.ptr);
    }
    return_none();
} ext_unscoped;

static fn_(heap_Fixed_free(anyptr ctx, Sli$u8 buf, u32 buf_align), void) {
    debug_assert_nonnull(ctx);
    debug_assert_fmt(mem_isValidAlign(buf_align), "Alignment must be a power of 2");

    let self = as$(heap_Fixed*, ctx);
    unused(buf_align);

    // Verify buffer ownership
    debug_assert_fmt(
        heap_Fixed_ownsSli(self, buf.as_const),
        "Buffer not owned by this allocator"
    );

    // We can only truly free the last allocation
    if (heap_Fixed_isLastAllocation(self, buf.as_const)) {
        self->end_index -= buf.len;
    }
    // Otherwise, we do nothing (memory is still considered allocated)
}

/*========== Thread-Safe Implementation =====================================*/

static fn_ext_scope(heap_Fixed_thrdSafeAlloc(anyptr ctx, usize len, u32 align), Opt$Ptr$u8) {
    debug_assert_nonnull(ctx);
    debug_assert_fmt(mem_isValidAlign(align), "Alignment must be a power of 2");

    let self = as$(heap_Fixed*, ctx);

    // Use atomic operations for thread safety
    usize end_index = atomic_load_explicit(
        as$(_Atomic(usize)*, &self->end_index),
        memory_order_seq_cst
    );

    while (true) {
        // Calculate aligned offset
        let ptr_addr       = rawptrToInt(self->buffer.ptr) + end_index;
        let aligned_addr   = mem_alignForward(ptr_addr, align);
        let adjust_off     = aligned_addr - ptr_addr;
        let adjusted_index = end_index + adjust_off;
        let new_end_index  = adjusted_index + len;

        // Check if we have enough space
        if (self->buffer.len < new_end_index) {
            return_none();
        }

        // Try to atomically update the end_index
        usize expected = end_index;
        if (atomic_compare_exchange_strong_explicit(
                as$(_Atomic(usize)*, &self->end_index),
                &expected,
                new_end_index,
                memory_order_seq_cst,
                memory_order_seq_cst
            )) {
            // Success, return the allocated pointer
            return_some(intToRawptr$(u8*, aligned_addr));
        }

        // Another thread modified end_index, try again with the new value
        end_index = expected;
    }
} ext_unscoped;

/*========== Utility Functions ==============================================*/

static_inline fn_(heap_Fixed_sliContainsPtr(Sli_const$u8 container, Ptr_const$u8 ptr), bool) {
    let container_start = rawptrToInt(container.ptr);
    let container_end   = container_start + container.len;
    let ptr_addr        = rawptrToInt(ptr);

    return container_start <= ptr_addr && ptr_addr < container_end;
}

static_inline fn_(heap_Fixed_sliContainsSli(Sli_const$u8 container, Sli_const$u8 sli), bool) {
    let container_start = rawptrToInt(container.ptr);
    let container_end   = container_start + container.len;
    let slice_start     = rawptrToInt(sli.ptr);
    let slice_end       = slice_start + sli.len;

    return container_start <= slice_start && slice_end <= container_end;
}

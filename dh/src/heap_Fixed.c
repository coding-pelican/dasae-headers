#include "dh/heap/Fixed.h"
#include "dh/mem/common.h"

// Forward declarations for allocator vtable functions
$static fn_((heap_Fixed_alloc(P$raw ctx, usize len, u8 align))(O$P$u8));
$static fn_((heap_Fixed_resize(P$raw ctx, S$u8 buf, u8 buf_align, usize new_len))(bool));
$static fn_((heap_Fixed_remap(P$raw ctx, S$u8 buf, u8 buf_align, usize new_len))(O$P$u8));
$static fn_((heap_Fixed_free(P$raw ctx, S$u8 buf, u8 buf_align))(void));

// Thread-safe variants
$static fn_((heap_Fixed_thrdSafeAlloc(P$raw ctx, usize len, u8 align))(O$P$u8));

// Utility functions
$inline $static fn_((heap_Fixed_sliContainsPtr(S_const$u8 container, P_const$u8 ptr))(bool));
$inline $static fn_((heap_Fixed_sliContainsSli(S_const$u8 container, S_const$u8 sli))(bool));

fn_((heap_Fixed_allocator(heap_Fixed* self))(mem_Allocator)) {
    claim_assert_nonnull(self);
    // VTable for Fixed buffer allocator
    static const mem_Allocator_VT vt[1] = { {
        .alloc = heap_Fixed_alloc,
        .resize = heap_Fixed_resize,
        .remap = heap_Fixed_remap,
        .free = heap_Fixed_free,
    } };
    return (mem_Allocator){
        .ctx = self,
        .vt = vt
    };
}

fn_((heap_Fixed_thrdSafeAllocator(heap_Fixed* self))(mem_Allocator)) {
    /* Thread-safe VTable for FixedBuf allocator */
    static const mem_Allocator_VT vt[1] = { {
        .alloc = heap_Fixed_thrdSafeAlloc,
        .resize = mem_Allocator_VT_noResize,
        .remap = mem_Allocator_VT_noRemap,
        .free = mem_Allocator_VT_noFree,
    } };
    return (mem_Allocator){
        .ctx = self,
        .vt = vt
    };
}

fn_((heap_Fixed_init(S$u8 buf))(heap_Fixed)) {
    claim_assert_nonnull(buf.ptr);
    return (heap_Fixed){
        .buffer = buf,
        .end_index = 0,
    };
}

fn_((heap_Fixed_reset(heap_Fixed* self))(void)) {
    claim_assert_nonnull(self);
    self->end_index = 0;
}

fn_((heap_Fixed_ownsPtr(const heap_Fixed* self, P_const$u8 ptr))(bool)) {
    claim_assert_nonnull(self);
    return heap_Fixed_sliContainsPtr(self->buffer.as_const, ptr);
}

fn_((heap_Fixed_ownsSli(const heap_Fixed* self, S_const$u8 sli))(bool)) {
    claim_assert_nonnull(self);
    return heap_Fixed_sliContainsSli(self->buffer.as_const, sli);
}

fn_((heap_Fixed_isLastAllocation(const heap_Fixed* self, S_const$u8 buf))(bool)) {
    claim_assert_nonnull(self);
    // Check if this is the last allocation
    // This has false negatives when the last allocation had an alignment adjustment
    let buf_end = ptrToInt(buf.ptr) + buf.len;
    let mem_end = ptrToInt(self->buffer.ptr) + self->end_index;
    return buf_end == mem_end;
}

/*========== Allocator Interface Implementation =============================*/

$static fn_((heap_Fixed_alloc(P$raw ctx, usize len, u8 align))(O$P$u8) $scope) {
    claim_assert_nonnull(ctx);
    let self = as$(heap_Fixed*)(ctx);
    let ptr_align = 1ull << align;

    // Calculate aligned offset
    let ptr_addr = ptrToInt(self->buffer.ptr) + self->end_index;
    let aligned_addr = mem_alignForward(ptr_addr, ptr_align);
    let adjust_off = aligned_addr - ptr_addr;
    let adjusted_index = self->end_index + adjust_off;
    let new_end_index = adjusted_index + len;

    // Check if we have enough space
    if (self->buffer.len < new_end_index) { return_none(); }

    // Update allocation position
    self->end_index = new_end_index;
    return_some(intToPtr$(u8*, aligned_addr));
} $unscoped_(fn);

$static fn_((heap_Fixed_resize(P$raw ctx, S$u8 buf, u8 buf_align, usize new_len))(bool)) {
    claim_assert_nonnull(ctx);
    let self = as$(heap_Fixed*)(ctx);
    let_ignore = buf_align;

    // Verify buffer ownership
    debug_assert_fmt(
        heap_Fixed_ownsSli(self, buf.as_const),
        "Buffer not owned by this allocator"
    );

    // If it's not the last allocation, we can only shrink
    if (!heap_Fixed_isLastAllocation(self, buf.as_const)) {
        return new_len <= buf.len;
    }

    // If it's the last allocation, we can resize
    if (new_len <= buf.len) {
        // Shrink
        let reduction = buf.len - new_len;
        self->end_index -= reduction;
        return true;
    }
    // Expand
    let addition = new_len - buf.len;
    if (self->buffer.len < self->end_index + addition) {
        return false;
    }
    self->end_index += addition;
    return true;
}

$static fn_((heap_Fixed_remap(P$raw ctx, S$u8 buf, u8 buf_align, usize new_len))(O$P$u8) $scope) {
    claim_assert_nonnull(ctx);
    if (heap_Fixed_resize(ctx, buf, buf_align, new_len)) {
        return_some(buf.ptr);
    }
    return_none();
} $unscoped_(fn);

$static fn_((heap_Fixed_free(P$raw ctx, S$u8 buf, u8 buf_align))(void)) {
    claim_assert_nonnull(ctx);
    let self = as$(heap_Fixed*)(ctx);
    let_ignore = buf_align;

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

$static fn_((heap_Fixed_thrdSafeAlloc(P$raw ctx, usize len, u8 align))(O$P$u8) $scope) {
    claim_assert_nonnull(ctx);
    let self = as$(heap_Fixed*)(ctx);
    let ptr_align = 1ull << align;

    // Use atomic operations for thread safety
    usize end_index = atom_load(&self->end_index, atom_MemOrd_seq_cst);
    while (true) {
        // Calculate aligned offset
        let ptr_addr = ptrToInt(self->buffer.ptr) + end_index;
        let aligned_addr = mem_alignForward(ptr_addr, ptr_align);
        let adjust_off = aligned_addr - ptr_addr;
        let adjusted_index = end_index + adjust_off;
        let new_end_index = adjusted_index + len;
        if (self->buffer.len < new_end_index) { return_none(); }
        end_index = orelse_((atom_cmpXchgStrong(
            &self->end_index, end_index, new_end_index,
            atom_MemOrd_seq_cst, atom_MemOrd_seq_cst
        ))(return_some(intToPtr$(u8*, aligned_addr))));
    }
} $unscoped_(fn);

/*========== Utility Functions ==============================================*/

$inline $static fn_((heap_Fixed_sliContainsPtr(S_const$u8 container, P_const$u8 ptr))(bool)) {
    let container_start = ptrToInt(container.ptr);
    let container_end = container_start + container.len;
    let ptr_addr = ptrToInt(ptr);

    return container_start <= ptr_addr && ptr_addr < container_end;
}

$inline $static fn_((heap_Fixed_sliContainsSli(S_const$u8 container, S_const$u8 sli))(bool)) {
    let container_start = ptrToInt(container.ptr);
    let container_end = container_start + container.len;
    let slice_start = ptrToInt(sli.ptr);
    let slice_end = slice_start + sli.len;

    return container_start <= slice_start && slice_end <= container_end;
}

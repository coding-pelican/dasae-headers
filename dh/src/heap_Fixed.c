#include "dh/heap/Fixed.h"
#include "dh/mem/common.h"

/*========== Internal Declarations ==========================================*/

$static fn_((heap_Fixed__alloc(P$raw ctx, usize len, mem_Align align))(O$P$u8));
$static fn_((heap_Fixed__resize(P$raw ctx, S$u8 buf, mem_Align buf_align, usize new_len))(bool));
$static fn_((heap_Fixed__remap(P$raw ctx, S$u8 buf, mem_Align buf_align, usize new_len))(O$P$u8));
$static fn_((heap_Fixed__free(P$raw ctx, S$u8 buf, mem_Align buf_align))(void));
$static fn_((heap_Fixed__thrdSafeAlloc(P$raw ctx, usize len, mem_Align align))(O$P$u8));
$attr($inline_always)
$static fn_((heap_Fixed__sliContainsPtr(S_const$u8 container, P_const$u8 ptr))(bool));
$attr($inline_always)
$static fn_((heap_Fixed__sliContainsSli(S_const$u8 container, S_const$u8 sli))(bool));

/*========== External Definitions ===========================================*/

fn_((heap_Fixed_allocator(heap_Fixed* self))(mem_Allocator)) {
    // VTable for Fixed buf allocator
    $static const mem_Allocator_VT vt $like_ref = { {
        .alloc = heap_Fixed__alloc,
        .resize = heap_Fixed__resize,
        .remap = heap_Fixed__remap,
        .free = heap_Fixed__free,
    } };
    return mem_Allocator_ensureValid((mem_Allocator){
        .ctx = self,
        .vt = vt,
    });
};

fn_((heap_Fixed_thrdSafeAllocator(heap_Fixed* self))(mem_Allocator)) {
    /* Thread-safe VTable for FixedBuf allocator */
    $static const mem_Allocator_VT vt $like_ref = { {
        .alloc = heap_Fixed__thrdSafeAlloc,
        .resize = mem_Allocator_VT_noResize,
        .remap = mem_Allocator_VT_noRemap,
        .free = mem_Allocator_VT_noFree,
    } };
    return mem_Allocator_ensureValid((mem_Allocator){
        .ctx = self,
        .vt = vt,
    });
};

fn_((heap_Fixed_from(S$u8 buf))(heap_Fixed)) {
    claim_assert_nonnullS(buf);
    return (heap_Fixed){
        .buf = buf,
        .end_idx = 0,
    };
};

fn_((heap_Fixed_reset(heap_Fixed* self))(void)) {
    claim_assert_nonnull(self);
    self->end_idx = 0;
};

fn_((heap_Fixed_ownsPtr(const heap_Fixed* self, P_const$u8 ptr))(bool)) {
    claim_assert_nonnull(self);
    return heap_Fixed__sliContainsPtr(self->buf.as_const, ptr);
};

fn_((heap_Fixed_ownsSli(const heap_Fixed* self, S_const$u8 sli))(bool)) {
    claim_assert_nonnull(self);
    return heap_Fixed__sliContainsSli(self->buf.as_const, sli);
};

fn_((heap_Fixed_isLastAllocation(const heap_Fixed* self, S_const$u8 buf))(bool)) {
    claim_assert_nonnull(self);
    // Check if this is the last allocation
    // This has false negatives when the last allocation had an alignment adjustment
    let buf_end = ptrToInt(buf.ptr) + buf.len;
    let mem_end = ptrToInt(self->buf.ptr) + self->end_idx;
    return buf_end == mem_end;
};

/*========== Internal Definitions ===========================================*/

fn_((heap_Fixed__alloc(P$raw ctx, usize len, mem_Align align))(O$P$u8) $scope) {
    claim_assert_nonnull(ctx);
    let self = as$(heap_Fixed*)(ctx);
    let ptr_align = mem_log2ToAlign(align);

    // Calculate aligned offset
    let ptr_addr = ptrToInt(self->buf.ptr) + self->end_idx;
    let aligned_addr = mem_alignFwd(ptr_addr, ptr_align);
    let adjust_off = aligned_addr - ptr_addr;
    let adjusted_index = self->end_idx + adjust_off;
    let new_end_index = adjusted_index + len;

    // Check if we have enough space
    if (self->buf.len < new_end_index) { return_none(); }

    // Update allocation position
    self->end_idx = new_end_index;
    return_some(intToPtr$((u8*)(aligned_addr)));
} $unscoped_(fn);

fn_((heap_Fixed__resize(P$raw ctx, S$u8 buf, mem_Align buf_align, usize new_len))(bool)) {
    claim_assert_nonnull(ctx);
    let self = as$(heap_Fixed*)(ctx);
    let_ignore = buf_align;

    // Verify buf ownership
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
        self->end_idx -= reduction;
        return true;
    }
    // Expand
    let addition = new_len - buf.len;
    if (self->buf.len < self->end_idx + addition) {
        return false;
    }
    self->end_idx += addition;
    return true;
};

fn_((heap_Fixed__remap(P$raw ctx, S$u8 buf, mem_Align buf_align, usize new_len))(O$P$u8) $scope) {
    claim_assert_nonnull(ctx);
    if (heap_Fixed__resize(ctx, buf, buf_align, new_len)) {
        return_some(buf.ptr);
    }
    return_none();
} $unscoped_(fn);

fn_((heap_Fixed__free(P$raw ctx, S$u8 buf, mem_Align buf_align))(void)) {
    claim_assert_nonnull(ctx);
    let self = as$(heap_Fixed*)(ctx);
    let_ignore = buf_align;

    // Verify buf ownership
    debug_assert_fmt(
        heap_Fixed_ownsSli(self, buf.as_const),
        "Buffer not owned by this allocator"
    );

    // We can only truly free the last allocation
    if (heap_Fixed_isLastAllocation(self, buf.as_const)) {
        self->end_idx -= buf.len;
    }
    // Otherwise, we do nothing (memory is still considered allocated)
};

fn_((heap_Fixed__thrdSafeAlloc(P$raw ctx, usize len, mem_Align align))(O$P$u8) $scope) {
    claim_assert_nonnull(ctx);
    let self = as$(heap_Fixed*)(ctx);
    let ptr_align = mem_log2ToAlign(align);

    // Use atomic operations for thread safety
    usize end_idx = atom_load(&self->end_idx, atom_MemOrd_seq_cst);
    while (true) {
        // Calculate aligned offset
        let ptr_addr = ptrToInt(self->buf.ptr) + end_idx;
        let aligned_addr = mem_alignFwd(ptr_addr, ptr_align);
        let adjust_off = aligned_addr - ptr_addr;
        let adjusted_index = end_idx + adjust_off;
        let new_end_index = adjusted_index + len;
        if (self->buf.len < new_end_index) { return_none(); }
        end_idx = orelse_((atom_cmpXchgStrong(
            &self->end_idx, end_idx, new_end_index,
            atom_MemOrd_seq_cst, atom_MemOrd_seq_cst
        ))(return_some(intToPtr$((u8*)(aligned_addr)))));
    }
} $unscoped_(fn);

fn_((heap_Fixed__sliContainsPtr(S_const$u8 container, P_const$u8 ptr))(bool)) {
    let container_start = ptrToInt(container.ptr);
    let container_end = container_start + container.len;
    let ptr_addr = ptrToInt(ptr);
    return container_start <= ptr_addr && ptr_addr < container_end;
};

fn_((heap_Fixed__sliContainsSli(S_const$u8 container, S_const$u8 sli))(bool)) {
    let container_start = ptrToInt(container.ptr);
    let container_end = container_start + container.len;
    let slice_start = ptrToInt(sli.ptr);
    let slice_end = slice_start + sli.len;
    return container_start <= slice_start && slice_end <= container_end;
};

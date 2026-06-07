#include "dh/heap/Fixed.h"
#include "dh/mem/common.h"

/*========== Internal Declarations ==========================================*/

$static fn_((heap_Fixed__alloc(P$raw ctx, usize len, mem_Align align))(O$P$u8));
$static fn_((heap_Fixed__resize(P$raw ctx, S$u8 buf, mem_Align buf_align, usize new_len))(bool));
$static fn_((heap_Fixed__remap(P$raw ctx, S$u8 buf, mem_Align buf_align, usize new_len))(O$P$u8));
$static fn_((heap_Fixed__free(P$raw ctx, S$u8 buf, mem_Align buf_align))(void));

$static fn_((heap_Fixed__thrdSafeAlloc(P$raw ctx, usize len, mem_Align align))(O$P$u8));
$static fn_((heap_Fixed__thrdSafeResize(P$raw ctx, S$u8 buf, mem_Align buf_align, usize new_len))(bool));
$static fn_((heap_Fixed__thrdSafeRemap(P$raw ctx, S$u8 buf, mem_Align buf_align, usize new_len))(O$P$u8));
$static fn_((heap_Fixed__thrdSafeFree(P$raw ctx, S$u8 buf, mem_Align buf_align))(void));

$attr($inline_always)
$static fn_((heap_Fixed__sliContainsPtr(S_const$u8 container, P_const$u8 ptr))(bool));
$attr($inline_always)
$static fn_((heap_Fixed__sliContainsSli(S_const$u8 container, S_const$u8 sli))(bool));

/*========== External Definitions ===========================================*/

fn_((heap_Fixed_alctr(heap_Fixed* self))(mem_Alctr)) {
    $static let_(vtbl, mem_Alctr_VTbl) = {
        .allocFn = heap_Fixed__alloc,
        .resizeFn = heap_Fixed__resize,
        .remapFn = heap_Fixed__remap,
        .freeFn = heap_Fixed__free,
    };
    return mem_Alctr_ensureValid((mem_Alctr){
        .ctx = self,
        .vtbl = &vtbl,
    });
};

fn_((heap_Fixed_thrdSafeAlctr(heap_Fixed* self))(mem_Alctr)) {
    $static let_(vtbl, mem_Alctr_VTbl) = {
        .allocFn = heap_Fixed__thrdSafeAlloc,
        .resizeFn = heap_Fixed__thrdSafeResize,
        .remapFn = heap_Fixed__thrdSafeRemap,
        .freeFn = heap_Fixed__thrdSafeFree,
    };
    return mem_Alctr_ensureValid((mem_Alctr){
        .ctx = self,
        .vtbl = &vtbl,
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
    return heap_Fixed__sliContainsPtr(ensureNonnull(self)->buf.as_const, ptr);
};

fn_((heap_Fixed_ownsSli(const heap_Fixed* self, S_const$u8 sli))(bool)) {
    return heap_Fixed__sliContainsSli(ensureNonnull(self)->buf.as_const, sli);
};

fn_((heap_Fixed_isLastAllocation(const heap_Fixed* self, S_const$u8 buf))(bool)) {
    claim_assert_nonnull(self), claim_assert_nonnullS(buf);
    // Check if this is the last allocation
    // This has false negatives when the last allocation had an alignment adjustment
    let buf_end = ptrToInt(buf.ptr) + buf.len;
    let mem_end = ptrToInt(self->buf.ptr) + self->end_idx;
    return buf_end == mem_end;
};

/*========== Internal Definitions ===========================================*/

fn_((heap_Fixed__alloc(P$raw ctx, usize len, mem_Align align))(O$P$u8) $scope) {
    let self = ptrAlignCast$((heap_Fixed*)(ensureNonnull(ctx)));
    let ptr_align = mem_log2ToAlign(align);

    // Calculate aligned offset
    let ptr_addr = ptrToInt(self->buf.ptr) + self->end_idx;
    let aligned_addr = mem_alignFwd(ptr_addr, ptr_align);
    let adjust_off = aligned_addr - ptr_addr;
    let adjusted_index = self->end_idx + adjust_off;
    let new_end_index = adjusted_index + len;

    // Check if we have enough space
    if (self->buf.len < new_end_index) return_none();

    // Update allocation position
    self->end_idx = new_end_index;
    return_some(intToPtr$((u8*)(aligned_addr)));
} $unscoped(fn);

fn_((heap_Fixed__resize(P$raw ctx, S$u8 buf, mem_Align buf_align, usize new_len))(bool)) {
    let self = ptrAlignCast$((heap_Fixed*)(ensureNonnull(ctx)));
    buf = ensureNonnullS(buf);
    let_ignore = buf_align;
    claim_assert_fmt(heap_Fixed_ownsSli(self, buf.as_const), "Buffer not owned by this allocator");

    // If it's not the last allocation, we can only shrink
    if (!heap_Fixed_isLastAllocation(self, buf.as_const)) return new_len <= buf.len;

    // If it's the last allocation, we can resize
    if (new_len <= buf.len) {
        // Shrink
        let reduction = buf.len - new_len;
        self->end_idx -= reduction;
        return true;
    }

    // Expand
    let addition = new_len - buf.len;
    if (self->buf.len < self->end_idx + addition) return false;
    self->end_idx += addition;
    return true;
};

fn_((heap_Fixed__remap(P$raw ctx, S$u8 buf, mem_Align buf_align, usize new_len))(O$P$u8) $scope) {
    let self = ptrAlignCast$((heap_Fixed*)(ensureNonnull(ctx)));
    buf = ensureNonnullS(buf);
    if (heap_Fixed__resize(self, buf, buf_align, new_len)) return_some(buf.ptr);
    return_none();
} $unscoped(fn);

fn_((heap_Fixed__free(P$raw ctx, S$u8 buf, mem_Align buf_align))(void)) {
    let self = ptrAlignCast$((heap_Fixed*)(ensureNonnull(ctx)));
    buf = ensureNonnullS(buf);
    let_ignore = buf_align;
    claim_assert_fmt(heap_Fixed_ownsSli(self, buf.as_const), "Buffer not owned by this allocator");

    // We can only truly free the last allocation
    if (heap_Fixed_isLastAllocation(self, buf.as_const)) {
        self->end_idx -= buf.len;
    }
    // Otherwise, we do nothing (memory is still considered allocated)
};

fn_((heap_Fixed__thrdSafeAlloc(P$raw ctx, usize len, mem_Align align))(O$P$u8) $scope) {
    let self = ptrAlignCast$((heap_Fixed*)(ensureNonnull(ctx)));
    let ptr_align = mem_log2ToAlign(align);

    // Use atomic operations for thread safety
    var_(end_idx, usize) = atom_load(&self->end_idx, atom_MemOrd_monotonic);
    while (true) {
        // Calculate aligned offset
        let ptr_addr = ptrToInt(self->buf.ptr) + end_idx;
        let aligned_addr = mem_alignFwd(ptr_addr, ptr_align);
        let adjust_off = aligned_addr - ptr_addr;
        let adjusted_index = end_idx + adjust_off;
        let new_end_index = adjusted_index + len;
        if (self->buf.len < new_end_index) return_none();
        end_idx = orelse_((atom_cmpXchgStrong(
            &self->end_idx, end_idx, new_end_index,
            atom_MemOrd_acquire, atom_MemOrd_monotonic
        ))(return_some(intToPtr$((u8*)(aligned_addr)))));
    }
} $unscoped(fn);

fn_((heap_Fixed__thrdSafeResize(P$raw ctx, S$u8 buf, mem_Align buf_align, usize new_len))(bool)) {
    let self = ptrAlignCast$((heap_Fixed*)(ensureNonnull(ctx)));
    buf = ensureNonnullS(buf);
    let_ignore = buf_align;
    claim_assert_fmt(heap_Fixed_ownsSli(self, buf.as_const), "Buffer not owned by this allocator");

    let cur_end_idx = atom_load(&self->end_idx, atom_MemOrd_monotonic);
    if (ptrToInt(self->buf.ptr) + cur_end_idx != ptrToInt(buf.ptr) + buf.len) {
        return new_len <= buf.len;
    }

    if (new_len <= buf.len) {
        let new_end_idx = cur_end_idx - (buf.len - new_len);
        claim_assert(ptrToInt(self->buf.ptr) + new_end_idx == ptrToInt(buf.ptr) + new_len);
        let_ignore = atom_cmpXchgStrong(
            &self->end_idx, cur_end_idx, new_end_idx,
            atom_MemOrd_release, atom_MemOrd_monotonic
        );
        return true;
    }

    let addition = new_len - buf.len;
    if (self->buf.len - cur_end_idx < addition) return false;
    let new_end_idx = cur_end_idx + addition;
    claim_assert(ptrToInt(self->buf.ptr) + new_end_idx == ptrToInt(buf.ptr) + new_len);
    return isNone(atom_cmpXchgStrong(
        &self->end_idx, cur_end_idx, new_end_idx,
        atom_MemOrd_acquire, atom_MemOrd_monotonic
    ));
};

fn_((heap_Fixed__thrdSafeRemap(P$raw ctx, S$u8 buf, mem_Align buf_align, usize new_len))(O$P$u8) $scope) {
    let self = ptrAlignCast$((heap_Fixed*)(ensureNonnull(ctx)));
    if (heap_Fixed__thrdSafeResize(self, buf, buf_align, new_len)) return_some(buf.ptr);
    return_none();
} $unscoped(fn);

fn_((heap_Fixed__thrdSafeFree(P$raw ctx, S$u8 buf, mem_Align buf_align))(void)) {
    let self = ptrAlignCast$((heap_Fixed*)(ensureNonnull(ctx)));
    buf = ensureNonnullS(buf);
    let_ignore = buf_align;
    claim_assert_fmt(heap_Fixed_ownsSli(self, buf.as_const), "Buffer not owned by this allocator");

    let cur_end_idx = atom_load(&self->end_idx, atom_MemOrd_monotonic);
    if (ptrToInt(self->buf.ptr) + cur_end_idx != ptrToInt(buf.ptr) + buf.len) return;

    let new_end_idx = cur_end_idx - buf.len;
    claim_assert(ptrToInt(self->buf.ptr) + new_end_idx == ptrToInt(buf.ptr));
    let_ignore = atom_cmpXchgStrong(
        &self->end_idx, cur_end_idx, new_end_idx,
        atom_MemOrd_release, atom_MemOrd_monotonic
    );
};

fn_((heap_Fixed__sliContainsPtr(S_const$u8 container, P_const$u8 ptr))(bool)) {
    claim_assert_nonnullS(container), claim_assert_nonnull(ptr);
    let container_start = ptrToInt(container.ptr);
    let container_end = container_start + container.len;
    let ptr_addr = ptrToInt(ptr);
    return container_start <= ptr_addr && ptr_addr < container_end;
};

fn_((heap_Fixed__sliContainsSli(S_const$u8 container, S_const$u8 sli))(bool)) {
    claim_assert_nonnullS(container), claim_assert_nonnullS(sli);
    let container_start = ptrToInt(container.ptr);
    let container_end = container_start + container.len;
    let slice_start = ptrToInt(sli.ptr);
    let slice_end = slice_start + sli.len;
    return container_start <= slice_start && slice_end <= container_end;
};

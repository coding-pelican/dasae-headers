#include "dh/heap/Sbrk.h"

/*========== Internal Declarations ==========================================*/

// Common implementation
$attr($inline_always)
$static fn_((heap_Sbrk__alloc(P$raw ctx, usize len, mem_Align align))(O$P$u8));
$attr($inline_always)
$static fn_((heap_Sbrk__resize(P$raw ctx, S$u8 buf, mem_Align buf_align, usize new_len))(bool));
$attr($inline_always)
$static fn_((heap_Sbrk__remap(P$raw ctx, S$u8 buf, mem_Align buf_align, usize new_len))(O$P$u8));
$attr($inline_always)
$static fn_((heap_Sbrk__free(P$raw ctx, S$u8 buf, mem_Align buf_align))(void));

$attr($inline_always)
$static fn_((heap_Sbrk__sizeClassIdx(usize len, mem_Align align))(usize));
$attr($inline_always)
$static fn_((heap_Sbrk__slotSize(usize class))(usize));
$attr($inline_always)
$static fn_((heap_Sbrk__bigPagesNeeded(heap_Sbrk self, usize byte_count))(usize));
$attr($inline_always)
$static fn_((heap_Sbrk__allocBigPages(heap_Sbrk* self, usize n))(usize));

// Validation helpers
$attr($inline_always)
$static fn_((heap_Sbrk__isValidAllocation(heap_Sbrk self, usize len, mem_Align align))(bool));
$attr($inline_always)
$static fn_((heap_Sbrk__actualLen(usize len, mem_Align align))(usize));

// Small variant functions
$static fn_((heap_Sbrk__alloc_small(P$raw ctx, usize len, mem_Align align))(O$P$u8));
$static fn_((heap_Sbrk__resize_small(P$raw ctx, S$u8 buf, mem_Align buf_align, usize new_len))(bool));
$static fn_((heap_Sbrk__remap_small(P$raw ctx, S$u8 buf, mem_Align buf_align, usize new_len))(O$P$u8));
$static fn_((heap_Sbrk__free_small(P$raw ctx, S$u8 buf, mem_Align buf_align))(void));

// Medium variant functions
$static fn_((heap_Sbrk__alloc_medium(P$raw ctx, usize len, mem_Align align))(O$P$u8));
$static fn_((heap_Sbrk__resize_medium(P$raw ctx, S$u8 buf, mem_Align buf_align, usize new_len))(bool));
$static fn_((heap_Sbrk__remap_medium(P$raw ctx, S$u8 buf, mem_Align buf_align, usize new_len))(O$P$u8));
$static fn_((heap_Sbrk__free_medium(P$raw ctx, S$u8 buf, mem_Align buf_align))(void));

// Large variant functions
$static fn_((heap_Sbrk__alloc_large(P$raw ctx, usize len, mem_Align align))(O$P$u8));
$static fn_((heap_Sbrk__resize_large(P$raw ctx, S$u8 buf, mem_Align buf_align, usize new_len))(bool));
$static fn_((heap_Sbrk__remap_large(P$raw ctx, S$u8 buf, mem_Align buf_align, usize new_len))(O$P$u8));
$static fn_((heap_Sbrk__free_large(P$raw ctx, S$u8 buf, mem_Align buf_align))(void));

/*========== External Definitions ===========================================*/

let_(heap_Sbrk_vt_local_small, mem_Allocator_VT) = {
    .alloc = heap_Sbrk__alloc_small,
    .resize = heap_Sbrk__resize_small,
    .remap = heap_Sbrk__remap_small,
    .free = heap_Sbrk__free_small,
};

let_(heap_Sbrk_vt_local_medium, mem_Allocator_VT) = {
    .alloc = heap_Sbrk__alloc_medium,
    .resize = heap_Sbrk__resize_medium,
    .remap = heap_Sbrk__remap_medium,
    .free = heap_Sbrk__free_medium,
};

let_(heap_Sbrk_vt_local_large, mem_Allocator_VT) = {
    .alloc = heap_Sbrk__alloc_large,
    .resize = heap_Sbrk__resize_large,
    .remap = heap_Sbrk__remap_large,
    .free = heap_Sbrk__free_large,
};

fn_((heap_Sbrk_allocator(heap_Sbrk* self))(mem_Allocator)) {
    claim_assert_nonnull(self);
    let vt = expr_(const mem_Allocator_VT* $scope)(match_(self->local_ref) {
        case_((heap_Sbrk_LocalRef_small))  $break_(&heap_Sbrk_vt_local_small) $end(case);
        case_((heap_Sbrk_LocalRef_medium)) $break_(&heap_Sbrk_vt_local_medium) $end(case);
        case_((heap_Sbrk_LocalRef_large)) $break_(&heap_Sbrk_vt_local_large) $end(case);
    } $end(match)) $unscoped_(expr);
    return mem_Allocator_ensureValid((mem_Allocator){
        .ctx = self,
        .vt = vt,
    });
};

fn_((heap_Sbrk_from(heap_Sbrk_LocalRef local_ref, P_const$heap_Sbrk_Ctx ctx))(heap_Sbrk)) {
    claim_assert_nonnull(ctx);
    claim_assert_nonnull(ctx->sbrkFn);
    return (heap_Sbrk){
        .local_ref = local_ref,
        .ctx = ctx,
    };
};

/*========== Internal Definitions - Helper Functions ========================*/

// Calculate size class index from length and alignment
fn_((heap_Sbrk__sizeClassIdx(usize len, mem_Align align))(usize)) {
    let actual_len = heap_Sbrk__actualLen(len, align);
    // Precondition: actual_len must be > 0 and power-of-2-able
    // Caller MUST validate with heap_Sbrk__isValidAllocation first
    claim_assert(actual_len <= (as$(usize)(1) << (sizeOf$(usize) * 8 - 1)));
    let slot_size = uint_exp2$((usize)(actual_len));
    // Precondition: slot_size must be non-zero for uint_log2
    let log_val = uint_log2(slot_size);
    // Precondition: log_val must be >= heap_Sbrk__min_class
    claim_assert(log_val >= heap_Sbrk__min_class);
    return log_val - heap_Sbrk__min_class;
};

// Calculate slot size from size class index
fn_((heap_Sbrk__slotSize(usize class))(usize)) {
    // Precondition: class must not cause shift overflow
    claim_assert(class < (sizeOf$(usize) * 8 - heap_Sbrk__min_class));
    return as$(usize)(1) << (class + heap_Sbrk__min_class);
};

fn_((heap_Sbrk__bigPagesNeeded(heap_Sbrk self, usize byte_count))(usize)) {
    let bigpage_size = heap_Sbrk__bigpage_size(self);
    // Precondition: byte_count + metadata must not overflow
    claim_assert(byte_count <= (usize_limit_max - sizeOf$(usize)));
    return (byte_count + (bigpage_size + (sizeOf$(usize) - 1))) / bigpage_size;
};

fn_((heap_Sbrk__allocBigPages(heap_Sbrk* self, usize n))(usize)) {
    // Precondition: n must be power-of-2-able
    claim_assert(n <= (as$(usize)(1) << (sizeOf$(usize) * 8 - 1)));
    let pow2_pages = uint_exp2$((usize)(n));
    let slot_size_bytes = pow2_pages * heap_Sbrk__bigpage_size(*self);
    // Precondition: pow2_pages must be non-zero for uint_log2
    let class = uint_log2(pow2_pages);
    let top_free_ptr = *S_at((heap_Sbrk_LocalRef_big_frees(self->local_ref))[class]);
    if (top_free_ptr != 0) {
        let node = intToPtr$((usize*)(top_free_ptr + (slot_size_bytes - sizeOf$(usize))));
        *S_at((heap_Sbrk_LocalRef_big_frees(self->local_ref))[class]) = *node;
        return top_free_ptr;
    }
    return self->ctx->sbrkFn(self->ctx->inner, pow2_pages * heap_Sbrk__pages_per_bigpage(*self) * heap_page_size);
};

// Calculate actual allocation length (with metadata + alignment)
fn_((heap_Sbrk__actualLen(usize len, mem_Align align))(usize)) {
    let buf_align = as$(usize)(1) << align;
    return int_max(len + sizeOf$(usize), buf_align);
};

// Validate if allocation request is within bounds
fn_((heap_Sbrk__isValidAllocation(heap_Sbrk self, usize len, mem_Align align))(bool)) {
    let actual_len = heap_Sbrk__actualLen(len, align);
    // Check for overflow in actual_len calculation
    if (actual_len < len) { return false; }
    // Check if actual_len exceeds maximum allocatable size
    let max_pool = heap_Sbrk__max_pool_size(self);
    if (max_pool < actual_len) { return false; }
    // Check if alignment is reasonable (prevent shift overflow)
    if ((sizeOf$(usize) * 8) <= align) { return false; }
    return true;
};

/*========== Internal Definitions - Core Operations =========================*/

fn_((heap_Sbrk__alloc(P$raw ctx, usize len, mem_Align align))(O$P$u8) $scope) {
    let self = ptrAlignCast$((heap_Sbrk*)(ctx));
    // VALIDATION: Check if allocation is valid BEFORE calling uint_log2/uint_exp2
    if (!heap_Sbrk__isValidAllocation(*self, len, align)) { return_none(); }
    // NOW SAFE: Use helper function instead of inline calculation
    let class = heap_Sbrk__sizeClassIdx(len, align);
    if ($branch_likely(class < heap_Sbrk__size_class_count(*self))) {
        let slot_size = heap_Sbrk__slotSize(class);
        let addr = expr_(usize $scope)({
            let top_free_ptr = *S_at((heap_Sbrk_LocalRef_frees(self->local_ref))[class]);
            if (top_free_ptr != 0) {
                let node = intToPtr$((usize*)(top_free_ptr + (slot_size - sizeOf$(usize))));
                *S_at((heap_Sbrk_LocalRef_frees(self->local_ref))[class]) = *node;
                $break_(top_free_ptr);
            }

            let next_addr = *S_at((heap_Sbrk_LocalRef_next_addrs(self->local_ref))[class]);
            if ((next_addr % heap_page_size) == 0) {
                let addr = heap_Sbrk__allocBigPages(self, 1);
                if (addr == 0) { $break_(0); }
                *S_at((heap_Sbrk_LocalRef_next_addrs(self->local_ref))[class]) = addr + slot_size;
                $break_(addr);
            } else {
                *S_at((heap_Sbrk_LocalRef_next_addrs(self->local_ref))[class]) = next_addr + slot_size;
                $break_(next_addr);
            }
        }) $unscoped_(expr);

        if (addr == 0) { return_none(); }
        return_some(intToPtr$((u8*)(addr)));
    }

    // Large allocation path
    let actual_len = heap_Sbrk__actualLen(len, align);
    let bigpages_needed = heap_Sbrk__bigPagesNeeded(*self, actual_len);
    let addr = heap_Sbrk__allocBigPages(self, bigpages_needed);
    if (addr == 0) { return_none(); }
    return_some(intToPtr$((u8*)(addr)));
} $unscoped_(fn);

fn_((heap_Sbrk__resize(P$raw ctx, S$u8 buf, mem_Align buf_align, usize new_len))(bool)) {
    let self = ptrAlignCast$((heap_Sbrk*)(ctx));
    // VALIDATION: Check both old and new sizes
    if (!heap_Sbrk__isValidAllocation(*self, buf.len, buf_align)) { return false; }
    let old_class = heap_Sbrk__sizeClassIdx(buf.len, buf_align);
    if (!heap_Sbrk__isValidAllocation(*self, new_len, buf_align)) { return false; }
    let new_class = heap_Sbrk__sizeClassIdx(new_len, buf_align);
    return expr_(bool $scope)(if (old_class < heap_Sbrk__size_class_count(*self)) {
        // Small allocation - compare slot sizes
        $break_(old_class == new_class);
    } else {
        // Large allocation - compare big page counts
        let old_actual_len = heap_Sbrk__actualLen(buf.len, buf_align);
        let new_actual_len = heap_Sbrk__actualLen(new_len, buf_align);
        let old_bigpages_needed = heap_Sbrk__bigPagesNeeded(*self, old_actual_len);
        let new_bigpages_needed = heap_Sbrk__bigPagesNeeded(*self, new_actual_len);
        // Both must be valid for uint_exp2
        if (old_bigpages_needed == 0 || new_bigpages_needed == 0) { $break_(false); }
        let old_big_slot_pages = uint_exp2$((usize)(old_bigpages_needed));
        let new_big_slot_pages = uint_exp2$((usize)(new_bigpages_needed));
        $break_(old_big_slot_pages == new_big_slot_pages);
    }) $unscoped_(expr);
};

fn_((heap_Sbrk__remap(P$raw ctx, S$u8 buf, mem_Align buf_align, usize new_len))(O$P$u8) $scope) {
    return_(expr_(O$P$u8 $scope)(
        heap_Sbrk__resize(ctx, buf, buf_align, new_len)
            ? $break_(some(buf.ptr))
            : $break_(none())
    ) $unscoped_(expr));
} $unscoped_(fn);

fn_((heap_Sbrk__free(P$raw ctx, S$u8 buf, mem_Align buf_align))(void)) {
    let self = ptrAlignCast$((heap_Sbrk*)(ctx));
    // VALIDATION: Ensure buffer is valid before freeing
    if (!heap_Sbrk__isValidAllocation(*self, buf.len, buf_align)) { return; /* Invalid free */ }
    let class = heap_Sbrk__sizeClassIdx(buf.len, buf_align);
    let addr = ptrToInt(buf.ptr);
    if (class < heap_Sbrk__size_class_count(*self)) {
        // Small allocation - use slot size helper
        let slot_size = heap_Sbrk__slotSize(class);
        let node = intToPtr$((usize*)(addr + (slot_size - sizeOf$(usize))));
        *node = *S_at((heap_Sbrk_LocalRef_frees(self->local_ref))[class]);
        *S_at((heap_Sbrk_LocalRef_frees(self->local_ref))[class]) = addr;
    } else {
        // Large allocation
        let actual_len = heap_Sbrk__actualLen(buf.len, buf_align);
        let bigpages_needed = heap_Sbrk__bigPagesNeeded(*self, actual_len);
        // Must be valid for uint_exp2/uint_log2
        if (bigpages_needed == 0) { return; }

        let pow2_pages = uint_exp2$((usize)(bigpages_needed));
        let big_slot_size_bytes = pow2_pages * heap_Sbrk__bigpage_size(*self);
        let node = intToPtr$((usize*)(addr + (big_slot_size_bytes - sizeOf$(usize))));

        let big_class = uint_log2(pow2_pages);
        *node = *S_at((heap_Sbrk_LocalRef_big_frees(self->local_ref))[big_class]);
        *S_at((heap_Sbrk_LocalRef_big_frees(self->local_ref))[big_class]) = addr;
    }
};

/*========== Internal Definitions - Small/Medium/Large Variants =============*/

// Small variant
fn_((heap_Sbrk__alloc_small(P$raw ctx, usize len, mem_Align align))(O$P$u8)) {
    let self = ptrAlignCast$((heap_Sbrk*)(ctx));
    claim_assert(self->local_ref.tag == heap_Sbrk_LocalRef_small);
    return heap_Sbrk__alloc(ctx, len, align);
};

fn_((heap_Sbrk__resize_small(P$raw ctx, S$u8 buf, mem_Align buf_align, usize new_len))(bool)) {
    let self = ptrAlignCast$((heap_Sbrk*)(ctx));
    claim_assert(self->local_ref.tag == heap_Sbrk_LocalRef_small);
    return heap_Sbrk__resize(ctx, buf, buf_align, new_len);
};

fn_((heap_Sbrk__remap_small(P$raw ctx, S$u8 buf, mem_Align buf_align, usize new_len))(O$P$u8)) {
    let self = ptrAlignCast$((heap_Sbrk*)(ctx));
    claim_assert(self->local_ref.tag == heap_Sbrk_LocalRef_small);
    return heap_Sbrk__remap(ctx, buf, buf_align, new_len);
};

fn_((heap_Sbrk__free_small(P$raw ctx, S$u8 buf, mem_Align buf_align))(void)) {
    let self = ptrAlignCast$((heap_Sbrk*)(ctx));
    claim_assert(self->local_ref.tag == heap_Sbrk_LocalRef_small);
    return heap_Sbrk__free(ctx, buf, buf_align);
};

// Medium variant
fn_((heap_Sbrk__alloc_medium(P$raw ctx, usize len, mem_Align align))(O$P$u8)) {
    let self = ptrAlignCast$((heap_Sbrk*)(ctx));
    claim_assert(self->local_ref.tag == heap_Sbrk_LocalRef_medium);
    return heap_Sbrk__alloc(ctx, len, align);
};

fn_((heap_Sbrk__resize_medium(P$raw ctx, S$u8 buf, mem_Align buf_align, usize new_len))(bool)) {
    let self = ptrAlignCast$((heap_Sbrk*)(ctx));
    claim_assert(self->local_ref.tag == heap_Sbrk_LocalRef_medium);
    return heap_Sbrk__resize(ctx, buf, buf_align, new_len);
};

fn_((heap_Sbrk__remap_medium(P$raw ctx, S$u8 buf, mem_Align buf_align, usize new_len))(O$P$u8)) {
    let self = ptrAlignCast$((heap_Sbrk*)(ctx));
    claim_assert(self->local_ref.tag == heap_Sbrk_LocalRef_medium);
    return heap_Sbrk__remap(ctx, buf, buf_align, new_len);
};

fn_((heap_Sbrk__free_medium(P$raw ctx, S$u8 buf, mem_Align buf_align))(void)) {
    let self = ptrAlignCast$((heap_Sbrk*)(ctx));
    claim_assert(self->local_ref.tag == heap_Sbrk_LocalRef_medium);
    return heap_Sbrk__free(ctx, buf, buf_align);
};

// Large variant
fn_((heap_Sbrk__alloc_large(P$raw ctx, usize len, mem_Align align))(O$P$u8)) {
    let self = ptrAlignCast$((heap_Sbrk*)(ctx));
    claim_assert(self->local_ref.tag == heap_Sbrk_LocalRef_large);
    return heap_Sbrk__alloc(ctx, len, align);
};

fn_((heap_Sbrk__resize_large(P$raw ctx, S$u8 buf, mem_Align buf_align, usize new_len))(bool)) {
    let self = ptrAlignCast$((heap_Sbrk*)(ctx));
    claim_assert(self->local_ref.tag == heap_Sbrk_LocalRef_large);
    return heap_Sbrk__resize(ctx, buf, buf_align, new_len);
};

fn_((heap_Sbrk__remap_large(P$raw ctx, S$u8 buf, mem_Align buf_align, usize new_len))(O$P$u8)) {
    let self = ptrAlignCast$((heap_Sbrk*)(ctx));
    claim_assert(self->local_ref.tag == heap_Sbrk_LocalRef_large);
    return heap_Sbrk__remap(ctx, buf, buf_align, new_len);
};

fn_((heap_Sbrk__free_large(P$raw ctx, S$u8 buf, mem_Align buf_align))(void)) {
    let self = ptrAlignCast$((heap_Sbrk*)(ctx));
    claim_assert(self->local_ref.tag == heap_Sbrk_LocalRef_large);
    return heap_Sbrk__free(ctx, buf, buf_align);
};

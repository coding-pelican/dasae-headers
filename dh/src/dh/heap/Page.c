#include "dh/heap/Page.h"
#include "dh/heap/vmap.h"
#include "dh/mem/common.h"

/*========== Internal Declarations ==========================================*/

$attr($inline_always)
$static fn_((heap_Page__geom(void))(heap_Geom));
$attr($inline_always)
$static fn_((heap_Page__guaranteedAlign(void))(usize));
$attr($inline_always)
$static fn_((heap_Page__alignedLen(usize len))(usize));
$attr($inline_always)
$static fn_((heap_Page__assertContract(mem_Align align))(void));
$attr($inline_always)
$static fn_((heap_Page__hintAlign(void))(usize));

$attr($inline_always)
$static fn_((heap_Page__noHintAddr(void))(usize));
$attr($inline_always)
$static fn_((heap_Page__normalizeHintAddr(usize hint_addr))(usize));
$attr($inline_always)
$static fn_((heap_Page__hintAddrAsPtr(usize hint_addr))(P$raw));

$static fn_((heap_Page__loadHintAddr(heap_Page* self))(usize));
$static fn_((heap_Page__storeHintAddr(heap_Page* self, usize old_hint_addr, u8* map, usize aligned_len))(void));
$static fn_((heap_Page__updateHintForRelease(heap_Page* self, u8* released_ptr, usize released_len))(void));
#if !plat_is_windows
$static fn_((heap_Page__updateHintForRemap(heap_Page* self, u8* old_ptr, usize old_len, u8* new_ptr, usize new_len))(void));
$static fn_((heap_Page__shrinkPosix(heap_Page* self, S$u8 buf, usize new_aligned_len))(bool));
#endif

$static fn_((heap_Page__alloc(P$raw ctx, usize len, mem_Align align))(O$P$u8));
$static fn_((heap_Page__resize(P$raw ctx, S$u8 buf, mem_Align buf_align, usize new_len))(bool));
$static fn_((heap_Page__remap(P$raw ctx, S$u8 buf, mem_Align buf_align, usize new_len))(O$P$u8));
$static fn_((heap_Page__free(P$raw ctx, S$u8 buf, mem_Align buf_align))(void));

/*========== External Definitions ===========================================*/

fn_((heap_Page_alctr(heap_Page* self))(mem_Alctr)) {
    // VTable for Page allocator
    $static let_(vtbl, mem_Alctr_VTbl) = {
        .alloc = heap_Page__alloc,
        .resize = heap_Page__resize,
        .remap = heap_Page__remap,
        .free = heap_Page__free,
    };
    return mem_Alctr_ensureValid((mem_Alctr){
        .ctx = self,
        .vtbl = &vtbl,
    });
};

/*========== Internal Definitions ===========================================*/

fn_((heap_Page__geom(void))(heap_Geom)) {
    return heap_vmap_geom();
};

fn_((heap_Page__guaranteedAlign(void))(usize)) {
    return heap_Page__geom().map_align;
};

fn_((heap_Page__alignedLen(usize len))(usize)) {
    return heap_Geom_alignPageWith(heap_Page__geom(), len);
};

fn_((heap_Page__hintAlign(void))(usize)) {
    return heap_Page__geom().map_align;
};

fn_((heap_Page__noHintAddr(void))(usize)) {
    return ptrToInt(null);
};

fn_((heap_Page__assertContract(mem_Align align))(void)) {
    let ptr_align = mem_log2ToAlign(align);
    let max_align = heap_Page__guaranteedAlign();
    claim_assert_fmt(
        ptr_align <= max_align,
        "Page allocator can only guarantee platform page mapping alignment (requested: {:uz}, max: {:uz})",
        ptr_align, max_align
    );
};

fn_((heap_Page__normalizeHintAddr(usize hint_addr))(usize)) {
    if (hint_addr == heap_Page__noHintAddr()) return heap_Page__noHintAddr();
    return mem_alignFwd(hint_addr, heap_Page__hintAlign());
};

fn_((heap_Page__hintAddrAsPtr(usize hint_addr))(P$raw)) {
    return hint_addr == heap_Page__noHintAddr() ? null : intToPtr$((P$raw)(hint_addr));
};

fn_((heap_Page__loadHintAddr(heap_Page* self))(usize)) {
    return heap_Page__normalizeHintAddr(atom_load(&self->next_addr_hint_, atom_MemOrd_seq_cst));
};

fn_((heap_Page__storeHintAddr(heap_Page* self, usize old_hint_addr, u8* map, usize aligned_len))(void)) {
    let new_hint_addr = heap_Page__normalizeHintAddr(ptrToInt(map + aligned_len));
    let_ignore = atom_cmpXchgStrong(
        &self->next_addr_hint_,
        old_hint_addr,
        new_hint_addr,
        atom_MemOrd_seq_cst,
        atom_MemOrd_seq_cst
    );
};

fn_((heap_Page__updateHintForRelease(heap_Page* self, u8* released_ptr, usize released_len))(void)) {
    let start = ptrToInt(released_ptr);
    let end = start + released_len;
    while (true) {
        let old_hint_addr = heap_Page__loadHintAddr(self);
        if (old_hint_addr == heap_Page__noHintAddr()) return;
        if (old_hint_addr < start || end < old_hint_addr) return;
        let new_hint_addr = heap_Page__normalizeHintAddr(ptrToInt(released_ptr));
        let_ignore = orelse_((atom_cmpXchgStrong(
            &self->next_addr_hint_,
            old_hint_addr,
            new_hint_addr,
            atom_MemOrd_seq_cst,
            atom_MemOrd_seq_cst
        ))(return));
    }
};
#if !plat_is_windows
fn_((heap_Page__updateHintForRemap(heap_Page* self, u8* old_ptr, usize old_len, u8* new_ptr, usize new_len))(void)) {
    let old_start = ptrToInt(old_ptr);
    let old_end = old_start + old_len;
    while (true) {
        let old_hint_addr = heap_Page__loadHintAddr(self);
        if (old_hint_addr == heap_Page__noHintAddr()) return;
        if (old_hint_addr < old_start || old_end < old_hint_addr) return;
        let new_hint_addr = heap_Page__normalizeHintAddr(ptrToInt(new_ptr + new_len));
        let_ignore = orelse_((atom_cmpXchgStrong(
            &self->next_addr_hint_,
            old_hint_addr,
            new_hint_addr,
            atom_MemOrd_seq_cst,
            atom_MemOrd_seq_cst
        ))(return));
    }
};

fn_((heap_Page__shrinkPosix(heap_Page* self, S$u8 buf, usize new_aligned_len))(bool)) {
    let buf_aligned_len = heap_Page__alignedLen(buf.len);
    if (buf_aligned_len <= new_aligned_len) return true;
    let tail_len = buf_aligned_len - new_aligned_len;
    if (tail_len == 0) return true;
    let tail_ptr = as$(u8*)(buf.ptr + new_aligned_len);
    if (!heap_vmap_release(tail_ptr, tail_len)) return false;
    heap_Page__updateHintForRelease(self, tail_ptr, tail_len);
    return true;
};
#endif

fn_((heap_Page__alloc(P$raw ctx, usize len, mem_Align align))(O$P$u8) $scope) {
    let self = ptrAlignCast$((heap_Page*)(ctx));
    let ptr_align = mem_log2ToAlign(align);
    let geom = heap_Page__geom();
    heap_Page__assertContract(align);

    // Check for overflow when aligning to page size
    if (usize_limit - (geom.page_size - 1) < len) { return_none(); }

#if plat_is_windows
    let aligned_len = heap_Page__alignedLen(len);
    let hint_addr = heap_Page__loadHintAddr(self);
    let hint = heap_Page__hintAddrAsPtr(hint_addr);
    var addr = orelse_((heap_vmap_map(hint, len))(null));
    if (addr == null && hint_addr != heap_Page__noHintAddr()) {
        addr = orelse_((heap_vmap_map(null, len))(null));
    }
    if (addr != null && mem_isAligned(ptrToInt(addr), ptr_align)) {
        heap_Page__storeHintAddr(self, hint_addr, addr, aligned_len);
        return_some(addr);
    }
    if (addr != null) {
        let_ignore = heap_vmap_release(addr, len);
    }

    // Fallback: map a temporary region, derive a target address from it,
    // release that region, then immediately map the desired subset there.
    // Another thread may have won the race to map the target range, in which
    // case a retry is needed.
    if (usize_limit - aligned_len < ptr_align - geom.page_size) { return_none(); }
    let overalloc_len = aligned_len + (ptr_align - geom.page_size);

    $static let retry_limit = 4;
    for (var retry_count = 0; retry_count < retry_limit; ++retry_count) {
        let_ignore = retry_count;
        var overalloc_addr = orelse_((heap_vmap_map(hint, overalloc_len))(null));
        if (overalloc_addr == null && hint_addr != heap_Page__noHintAddr()) {
            overalloc_addr = orelse_((heap_vmap_map(null, overalloc_len))(null));
        }
        if (overalloc_addr == null) {
            return_none();
        }

        let aligned_addr = mem_alignFwd(ptrToInt(overalloc_addr), ptr_align);
        let_ignore = heap_vmap_release(overalloc_addr, overalloc_len);

        let addr = orelse_((heap_vmap_map(intToPtr$((P$raw)(aligned_addr)), aligned_len))(null));
        if (addr != null) {
            heap_Page__storeHintAddr(self, hint_addr, addr, aligned_len);
            return_some(addr);
        }
        // If VirtualAlloc fails, it might be due to address collision, retry.
        // In a real-world scenario, consider adding a retry limit.
    }
    return_none();
#else /* posix */
    let aligned_len = heap_Page__alignedLen(len);
    let hint_addr = heap_Page__loadHintAddr(self);
    let hint = heap_Page__hintAddrAsPtr(hint_addr);
    let map = orelse_((heap_vmap_map(hint, aligned_len))(null));
    if (map == null) return_none();
    claim_assert_fmt(mem_isAligned(ptrToInt(map), geom.page_size));
    claim_assert_fmt(mem_isAligned(ptrToInt(map), ptr_align), "mmap returned misaligned address");

    heap_Page__storeHintAddr(self, hint_addr, map, aligned_len);
    return_some(map);
#endif /* posix */
} $unscoped(fn);

fn_((heap_Page__resize(P$raw ctx, S$u8 buf, mem_Align buf_align, usize new_len))(bool)) {
    let self = ptrAlignCast$((heap_Page*)(ctx));
    let ptr_align = mem_log2ToAlign(buf_align);
    heap_Page__assertContract(buf_align);
    claim_assert_fmt(mem_isAligned(ptrToInt(buf.ptr), ptr_align), "Buffer address does not match the specified alignment");
    let_ignore = ptr_align;

    let new_size_aligned = heap_Page__alignedLen(new_len);
    let buf_aligned_len = heap_Page__alignedLen(buf.len);

    if (new_size_aligned == buf_aligned_len && new_len <= buf.len) {
        return true; // No resize needed
    }

#if plat_is_windows
    let_ignore = self;
    if (new_len <= buf.len) {
        return true;
    }

    // Windows PageAlctr in zig doesn't support resize larger.
    // Returning false to indicate resize failure.
    return false;

#else /* posix */

    if (new_size_aligned < buf_aligned_len) {
        return heap_Page__shrinkPosix(self, buf, new_size_aligned);
    }

    if (isSome(heap_vmap_remap(buf.ptr, buf.len, new_len))) {
        heap_Page__updateHintForRemap(self, buf.ptr, buf_aligned_len, buf.ptr, new_size_aligned);
        return true; // Assume success for now, further hint update needed if address changes.
    }

    // mremap is not available or failed, larger resize is not supported in this simple page allocator.
    return false;
#endif /* posix */
};

fn_((heap_Page__remap(P$raw ctx, S$u8 buf, mem_Align buf_align, usize new_len))(O$P$u8) $scope) {
    let self = ptrAlignCast$((heap_Page*)(ctx));
    let ptr_align = mem_log2ToAlign(buf_align);
    heap_Page__assertContract(buf_align);
    debug_assert_fmt(mem_isAligned( ptrToInt(buf.ptr), ptr_align), "Buffer address does not match the specified alignment");
    let_ignore = ptr_align;

    let new_size_aligned = heap_Page__alignedLen(new_len);
    let buf_aligned_len = heap_Page__alignedLen(buf.len);

    if (new_size_aligned == buf_aligned_len && new_len <= buf.len) {
        return_some(buf.ptr); // No resize needed
    }

#if plat_is_windows
    let_ignore = self;
    // Windows PageAlctr in zig doesn't support resize larger.
    return_none(); // Indicate remap failure, as resize up is not supported.

#else /* posix */

    if (new_size_aligned < buf_aligned_len) {
        if (heap_Page__shrinkPosix(self, buf, new_size_aligned)) return_some(buf.ptr);
        return_none();
    }

    if_some((heap_vmap_remap(buf.ptr, buf.len, new_len))(new_ptr)) {
        heap_Page__updateHintForRemap(self, buf.ptr, buf_aligned_len, new_ptr, new_size_aligned);
        return_some(new_ptr); // Assume success for now, further hint update needed if address changes.
    }

    // mremap is not available or failed, larger resize is not supported in this simple page allocator.
    return_none();
#endif /* posix */
} $unscoped(fn);

fn_((heap_Page__free(P$raw ctx, S$u8 buf, mem_Align buf_align))(void)) {
    let self = ptrAlignCast$((heap_Page*)(ctx));
    let ptr_align = mem_log2ToAlign(buf_align);
    heap_Page__assertContract(buf_align);
    claim_assert_fmt(mem_isAligned( ptrToInt(buf.ptr), ptr_align), "Buffer address does not match the specified alignment");
    let_ignore = ptr_align;

    if (heap_vmap_release(buf.ptr, buf.len)) {
        heap_Page__updateHintForRelease(self, buf.ptr, heap_Page__alignedLen(buf.len));
    }
};

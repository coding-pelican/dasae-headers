#include "dh/heap/Page.h"
#include "dh/heap/vmap.h"
#include "dh/mem/common.h"

/*========== Internal Declarations ==========================================*/

$static fn_((heap_Page__alloc(P$raw ctx, usize len, mem_Align align))(O$P$u8));
$static fn_((heap_Page__resize(P$raw ctx, S$u8 buf, mem_Align buf_align, usize new_len))(bool));
$static fn_((heap_Page__remap(P$raw ctx, S$u8 buf, mem_Align buf_align, usize new_len))(O$P$u8));
$static fn_((heap_Page__free(P$raw ctx, S$u8 buf, mem_Align buf_align))(void));

/*========== External Definitions ===========================================*/

fn_((heap_Page_alctr(heap_Page* self))(mem_Alctr)) {
    // VTable for Page allocator
    $static const mem_Alctr_VTbl vtbl $like_ref = { {
        .alloc = heap_Page__alloc,
        .resize = heap_Page__resize,
        .remap = heap_Page__remap,
        .free = heap_Page__free,
    } };
    return mem_Alctr_ensureValid((mem_Alctr){
        .ctx = self,
        .vtbl = vtbl,
    });
};

/*========== Internal Definitions ===========================================*/

fn_((heap_Page__alloc(P$raw ctx, usize len, mem_Align align))(O$P$u8) $scope) {
    let_ignore = ctx;
    let ptr_align = mem_log2ToAlign(align);
    // Page allocator guarantees page alignment, which is typically larger than most requested alignments
    // Verify requested alignment is not stricter than page alignment
    debug_assert_fmt(
        ptr_align <= mem_page_size,
        "Page allocator can only guarantee page alignment (requested: {:uz}, page size: {:uz})",
        ptr_align, mem_page_size
    );

    // Check for overflow when aligning to page size
    if (usize_limit - (mem_page_size - 1) < len) { return_none(); }

#if plat_is_windows
    let addr = orelse_((heap_vmap_map(null, len))(
        null
    ));
    if (addr != null) {
        // Verify returned address meets alignment requirements
        debug_assert_fmt(mem_isAligned(ptrToInt(addr), ptr_align), "VirtualAlloc returned misaligned address");
        return_some(addr);
    }

    // Fallback: map a temporary region, derive a target address from it,
    // release that region, then immediately map the desired subset there.
    // Another thread may have won the race to map the target range, in which
    // case a retry is needed.

    let aligned_len = mem_alignFwd(len, mem_page_size);
    let overalloc_len = aligned_len;

    $static let retry_limit = 4;
    for (var retry_count = 0; retry_count < retry_limit; ++retry_count) {
        let overalloc_addr = orelse_((heap_vmap_map(null, overalloc_len))(null));
        if (overalloc_addr == null) {
            return_none();
        }

        let aligned_addr = mem_alignFwd(ptrToInt(overalloc_addr), ptr_align);
        let_ignore = heap_vmap_release(overalloc_addr, overalloc_len);

        let addr = orelse_((heap_vmap_map(
            intToPtr$((P$raw)(aligned_addr)), aligned_len))(null));
        if (addr != null) {
            return_some(addr);
        }
        // If VirtualAlloc fails, it might be due to address collision, retry.
        // In a real-world scenario, consider adding a retry limit.
    }
    return_none();
#else /* posix */
    let aligned_len = mem_alignFwd(len, mem_page_size);
    let hint = heap_Page_s_next_mmap_addr_hint;
    let map = orelse_((heap_vmap_map(hint, aligned_len))(null));
    if (map == null) { return_none(); }
    debug_assert_fmt(mem_isAligned(ptrToInt(map), mem_page_size));
    debug_assert_fmt(mem_isAligned(ptrToInt(map), ptr_align), "mmap returned misaligned address");

    let new_hint = as$(P$raw)(as$(u8*)((map + aligned_len)));
    // Here use atomic operations to update the hint
    atom_cmpXchgWeak(
        &heap_Page_s_next_mmap_addr_hint,
        hint,
        new_hint,
        atom_MemOrd_seq_cst,
        atom_MemOrd_seq_cst
    );
    return_some(map);
#endif /* posix */
} $unscoped(fn);

fn_((heap_Page__resize(P$raw ctx, S$u8 buf, mem_Align buf_align, usize new_len))(bool)) {
    let_ignore = ctx;
    let ptr_align = mem_log2ToAlign(buf_align);
    debug_assert_fmt(ptr_align <= mem_page_size, "Page allocator only guarantees page alignment");
    debug_assert_fmt(mem_isAligned(ptrToInt(buf.ptr), ptr_align), "Buffer address does not match the specified alignment");
    let_ignore = ptr_align;

    let new_size_aligned = mem_alignFwd(new_len, mem_page_size);
    let buf_aligned_len = mem_alignFwd(buf.len, mem_page_size);

    if (new_size_aligned == buf_aligned_len && new_len <= buf.len) {
        return true; // No resize needed
    }

#if plat_is_windows
    if (new_len <= buf.len) {
        return true;
    }

    // Windows PageAlctr in zig doesn't support resize larger.
    // Returning false to indicate resize failure.
    return false;

#else /* posix */

    if (new_size_aligned < buf_aligned_len) {
        return true;
    }

    if (isSome(heap_vmap_remap(buf.ptr, buf.len, new_len))) {
        // TODO: if heap_Page_s_next_mmap_addr_hint is within the remapped range, update it if moved.
        return true; // Assume success for now, further hint update needed if address changes.
    }

    // mremap is not available or failed, larger resize is not supported in this simple page allocator.
    return false;
#endif /* posix */
};

fn_((heap_Page__remap(P$raw ctx, S$u8 buf, mem_Align buf_align, usize new_len))(O$P$u8) $scope) {
    let_ignore = ctx;
    let ptr_align = mem_log2ToAlign(buf_align);
    debug_assert_fmt(ptr_align <= mem_page_size, "Page allocator only guarantees page alignment");
    debug_assert_fmt(mem_isAligned(ptrToInt(buf.ptr), ptr_align), "Buffer address does not match the specified alignment");
    let_ignore = ptr_align;

    let new_size_aligned = mem_alignFwd(new_len, mem_page_size);
    let buf_aligned_len = mem_alignFwd(buf.len, mem_page_size);

    if (new_size_aligned == buf_aligned_len && new_len <= buf.len) {
        return_some(buf.ptr); // No resize needed
    }

#if plat_is_windows
    // Windows PageAlctr in zig doesn't support resize larger.
    return_none(); // Indicate remap failure, as resize up is not supported.

#else /* posix */

    if (new_size_aligned < buf_aligned_len) {
        return_none(); // Indicate remap failure, shrinking via remap not supported
    }

    if_some((heap_vmap_remap(buf.ptr, buf.len, new_len))(new_ptr)) {
        // TODO: if heap_Page_s_next_mmap_addr_hint is within the remapped range, update it if moved.
        return_some(new_ptr); // Assume success for now, further hint update needed if address changes.
    }

    // mremap is not available or failed, larger resize is not supported in this simple page allocator.
    return_none();
#endif /* posix */
} $unscoped(fn);

fn_((heap_Page__free(P$raw ctx, S$u8 buf, mem_Align buf_align))(void)) {
    let_ignore = ctx;
    let ptr_align = mem_log2ToAlign(buf_align);
    debug_assert_fmt(ptr_align <= mem_page_size, "Page allocator only guarantees page alignment");
    debug_assert_fmt(mem_isAligned(ptrToInt(buf.ptr), ptr_align), "Buffer address does not match the specified alignment");
    let_ignore = ptr_align;

    let_ignore = heap_vmap_release(buf.ptr, buf.len);
};

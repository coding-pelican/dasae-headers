#include "dh/heap/Page.h"
#include "dh/mem.h"

#if bti_plat_windows
#include "dh/os/windows/mem.h"
#else /* posix */
#include <sys/mman.h>
#include <unistd.h>
#endif /* posix */

static Opt$Ptr$u8 heap_Page_alloc(anyptr ctx, usize len, usize ptr_align) must_check;
static bool       heap_Page_resize(anyptr ctx, Sli$u8 buf, usize buf_align, usize new_size) must_check;
static void       heap_Page_free(anyptr ctx, Sli$u8 buf, usize buf_align);

mem_Allocator heap_Page_allocator(heap_Page* self) {
    static const mem_AllocatorVT vt[1] = { {
        .alloc  = heap_Page_alloc,
        .resize = heap_Page_resize,
        .free   = heap_Page_free,
    } };
    return (mem_Allocator){
        .ptr = self,
        .vt  = vt,
    };
}

/*========== Allocator Interface Implementation =============================*/

static Opt$Ptr$u8 heap_Page_alloc(anyptr ctx, usize len, usize ptr_align) {
    reserveReturn(Opt$Ptr$u8);
    unused(ctx, ptr_align);
    // Ensure alignment is a power of 2
    debug_assert_fmt(mem_isValidAlign(ptr_align), "Alignment must be a power of 2");
    // Page allocator guarantees page alignment, which is typically larger than most requested alignments
    // Verify requested alignment is not stricter than page alignment
    debug_assert_fmt(ptr_align <= mem_page_size, "Page allocator can only guarantee page alignment (requested: %zu, page size: %zu)", ptr_align, mem_page_size);

    // Check for overflow when aligning to page size
    if (usize_limit - (mem_page_size - 1) < len) { return_none(); }

#if bti_plat_windows
    let addr = VirtualAlloc(
        null,
        len, // VirtualAlloc rounds to page size internally
        MEM_COMMIT | MEM_RESERVE,
        PAGE_READWRITE
    );
    if (addr == null) { return_none(); }
    // Verify returned address meets alignment requirements
    debug_assert_fmt(mem_isAligned(rawptrToInt(addr), ptr_align), "VirtualAlloc returned misaligned address");

    return_some(addr);
#else  /* posix */
    let aligned_len = mem_alignForward(len, mem_page_size);
    var hint        = heap_Page_s_next_mmap_addr_hint;

    let map = mmap(
        hint,
        aligned_len,
        PROT_READ | PROT_WRITE,
        MAP_PRIVATE | MAP_ANONYMOUS,
        -1,
        0
    );
    if (map == MAP_FAILED) { return_none(); }
    debug_assert_fmt(mem_isAligned(rawptrToInt(map), mem_page_size));
    debug_assert_fmt(mem_isAligned(rawptrToInt(map), ptr_align), "mmap returned misaligned address");

    let new_hint = as$(anyptr, as$(u8*, map) + aligned_len);
    // Here use atomic operations to update the hint
    __atomic_compare_exchange_n(
        &heap_Page_s_next_mmap_addr_hint,
        &hint,
        new_hint,
        false,
        __ATOMIC_SEQ_CST,
        __ATOMIC_SEQ_CST
    );
    return_some(map);
#endif /* posix */
}

static bool heap_Page_resize(anyptr ctx, Sli$u8 buf, usize buf_align, usize new_size) {
    unused(ctx, buf_align);
    // Ensure alignment is a power of 2
    debug_assert_fmt(mem_isValidAlign(buf_align), "Alignment must be a power of 2");
    debug_assert_fmt(buf_align <= mem_page_size, "Page allocator only guarantees page alignment");
    // Verify the buffer address actually has the claimed alignment
    debug_assert_fmt(mem_isAligned(rawptrToInt(buf.ptr), buf_align), "Buffer address does not match the specified alignment");

    const usize new_size_aligned = mem_alignForward(new_size, mem_page_size);

#if bti_plat_windows
    if (new_size <= buf.len) {
        const usize base_addr    = rawptrToInt(buf.ptr);
        const usize old_addr_end = base_addr + mem_alignForward(buf.len, mem_page_size);
        const usize new_addr_end = base_addr + new_size_aligned;

        if (old_addr_end > new_addr_end) {
            // For shrinking that is not releasing, only
            // decommit the pages not needed anymore.
            VirtualFree(
                intToRawptr$(LPVOID, new_addr_end),
                old_addr_end - new_addr_end,
                MEM_DECOMMIT
            );
        }
        return true;
    }

    const usize old_size_aligned = mem_alignForward(buf.len, mem_page_size);
    if (new_size_aligned <= old_size_aligned) { return true; }
    return false;
#else  /* posix */
    const usize buf_aligned_len = mem_alignForward(buf.len, mem_page_size);
    if (new_size_aligned == buf_aligned_len) { return true; }

    if (new_size_aligned < buf_aligned_len) {
        anyptr ptr = as$(u8*, buf.ptr) + new_size_aligned;
        // TODO: if heap_Page_s_next_mmap_addr_hint is within the unmapped range, update it
        munmap(ptr, buf_aligned_len - new_size_aligned);
        return true;
    }

    // TODO: call mremap
    // TODO: if heap_Page_s_next_mmap_addr_hint is within the remapped range, update it
    return false;
#endif /* posix */
}

static void heap_Page_free(anyptr ctx, Sli$u8 buf, usize buf_align) {
    unused(ctx, buf_align);
    // Ensure alignment is a power of 2
    debug_assert_fmt(mem_isValidAlign(buf_align), "Alignment must be a power of 2");
    debug_assert_fmt(buf_align <= mem_page_size, "Page allocator only guarantees page alignment");
    // Verify the buffer address actually has the claimed alignment
    debug_assert_fmt(mem_isAligned(rawptrToInt(buf.ptr), buf_align), "Buffer address does not match the specified alignment");

#if bti_plat_windows
    VirtualFree(buf.ptr, 0, MEM_RELEASE);
#else  /* posix */
    const usize buf_aligned_len = mem_alignForward(buf.len, mem_page_size);
    munmap(buf.ptr, buf_aligned_len);
#endif /* posix */
}

#include "dh/heap/Page.h"
#include "dh/mem/common.h"

#if plat_windows
#include "dh/os/windows/mem.h"
#else /* posix */
#include <sys/mman.h>
#include <unistd.h>
#endif                 /* posix */
#include <stdatomic.h> // Required for atomic operations

// Forward declarations for allocator vtable functions
static fn_((heap_Page_alloc(P$raw ctx, usize len, u32 align))(O$P$u8));
static fn_((heap_Page_resize(P$raw ctx, S$u8 buf, u32 buf_align, usize new_len))(bool));
static fn_((heap_Page_remap(P$raw ctx, S$u8 buf, u32 buf_align, usize new_len))(O$P$u8));
static fn_((heap_Page_free(P$raw ctx, S$u8 buf, u32 buf_align))(void));

fn_((heap_Page_allocator(heap_Page* self))(mem_Allocator)) {
    debug_assert_nonnull(self);
    // VTable for Page allocator
    static const mem_Allocator_VT vt[1] = { {
        .alloc = heap_Page_alloc,
        .resize = heap_Page_resize,
        .remap = heap_Page_remap,
        .free = heap_Page_free,
    } };
    return (mem_Allocator){
        .ctx = self,
        .vt = vt,
    };
}

/*========== Allocator Interface Implementation =============================*/

static fn_((heap_Page_alloc(P$raw ctx, usize len, u32 align))(O$P$u8) $scope) {
    debug_assert_fmt(mem_isValidAlign(align), "Alignment must be a power of 2");
    // Page allocator guarantees page alignment, which is typically larger than most requested alignments
    // Verify requested alignment is not stricter than page alignment
    debug_assert_fmt(align <= mem_page_size, "Page allocator can only guarantee page alignment (requested: %zu, page size: %zu)", align, mem_page_size);

    $unused(ctx, align);

    // Check for overflow when aligning to page size
    if (usize_limit - (mem_page_size - 1) < len) { return_none(); }

#if plat_windows
    // Windows allocation logic similar to zig's PageAllocator
    let addr = VirtualAlloc(
        null,
        len, // VirtualAlloc rounds to page size internally
        MEM_COMMIT | MEM_RESERVE,
        PAGE_READWRITE
    );
    if (addr != null) {
        // Verify returned address meets alignment requirements
        debug_assert_fmt(mem_isAligned(ptrToInt(addr), align), "VirtualAlloc returned misaligned address");
        return_some(addr);
    }

    // Fallback: reserve a range of memory large enough to find a
    // sufficiently aligned address, then free the entire range and
    // immediately allocate the desired subset. Another thread may have won
    // the race to map the target range, in which case a retry is needed.

    let overalloc_len = len + align - mem_page_size;
    let aligned_len = mem_alignForward(len, mem_page_size);

    static let retry_limit = 4;
    for (var retry_count = 0; retry_count < retry_limit; ++retry_count) {
        let reserved_addr = VirtualAlloc(
            null,
            overalloc_len,
            MEM_RESERVE,
            PAGE_NOACCESS
        );
        if (reserved_addr == null) {
            return_none();
        }

        let aligned_addr = mem_alignForward(ptrToInt(reserved_addr), align);
        VirtualFree(reserved_addr, 0, MEM_RELEASE);

        let addr = VirtualAlloc(
            intToPtr$(P$raw, aligned_addr),
            aligned_len,
            MEM_COMMIT | MEM_RESERVE,
            PAGE_READWRITE
        );
        if (addr != null) {
            return_some(addr);
        }
        // If VirtualAlloc fails, it might be due to address collision, retry.
        // In a real-world scenario, consider adding a retry limit.
    }
    return_none();
#else  /* posix */
    let aligned_len = mem_alignForward(len, mem_page_size);
    let hint = heap_Page_s_next_mmap_addr_hint;

    let map = mmap(
        hint,
        aligned_len,
        PROT_READ | PROT_WRITE,
        MAP_PRIVATE | MAP_ANONYMOUS,
        -1,
        0
    );
    if (map == MAP_FAILED) { return_none(); }
    debug_assert_fmt(mem_isAligned(ptrToInt(map), mem_page_size));
    debug_assert_fmt(mem_isAligned(ptrToInt(map), ptr_align), "mmap returned misaligned address");

    let new_hint = as$((P$raw)(as$((u8*)(map) + aligned_len)));
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
} $unscoped_(fn);

static fn_((heap_Page_resize(P$raw ctx, S$u8 buf, u32 buf_align, usize new_len))(bool)) {
    debug_assert_fmt(mem_isValidAlign(buf_align), "Alignment must be a power of 2");
    debug_assert_fmt(buf_align <= mem_page_size, "Page allocator only guarantees page alignment");
    // Verify the buffer address actually has the claimed alignment
    debug_assert_fmt(mem_isAligned(ptrToInt(buf.ptr), buf_align), "Buffer address does not match the specified alignment");

    $unused(ctx, buf_align);

    let new_size_aligned = mem_alignForward(new_len, mem_page_size);
    let buf_aligned_len = mem_alignForward(buf.len, mem_page_size);

    if (new_size_aligned == buf_aligned_len && new_len <= buf.len) {
        return true; // No resize needed
    }

#if plat_windows
    if (new_len <= buf.len) {
        let base_addr = ptrToInt(buf.ptr);
        let old_addr_end = base_addr + buf_aligned_len;
        let new_addr_end = base_addr + new_size_aligned;

        if (old_addr_end > new_addr_end) {
            // For shrinking that is not releasing, only
            // decommit the pages not needed anymore.
            VirtualFree(
                intToPtr$(LPVOID, new_addr_end),
                old_addr_end - new_addr_end,
                MEM_DECOMMIT
            );
        }
        return true;
    }

    // Windows PageAllocator in zig doesn't support resize larger.
    // Returning false to indicate resize failure.
    return false;

#else /* posix */

    if (new_size_aligned < buf_aligned_len) {
        let ptr = as$((u8*)(buf.ptr)) + new_size_aligned;
        munmap(ptr, buf_aligned_len - new_size_aligned);
        return true;
    }

#ifdef MAP_REMAP // Check if mremap is available
    if (MAP_REMAP != 0) { // MAP_REMAP is defined and not 0, mremap is likely available
        let new_ptr = mremap(buf.ptr, buf.len, new_len, MREMAP_MAYMOVE);
        if (new_ptr != MAP_FAILED) {
            // TODO: if heap_Page_s_next_mmap_addr_hint is within the remapped range, update it if moved.
            return true; // Assume success for now, further hint update needed if address changes.
        }
        // mremap failed, fall back to returning false.
        return false;
    }
#endif

    // mremap is not available or failed, larger resize is not supported in this simple page allocator.
    return false;
#endif /* posix */
}

static fn_((heap_Page_remap(P$raw ctx, S$u8 buf, u32 buf_align, usize new_len))(O$P$u8) $scope) {
    debug_assert_fmt(mem_isValidAlign(buf_align), "Alignment must be a power of 2");
    debug_assert_fmt(buf_align <= mem_page_size, "Page allocator only guarantees page alignment");
    // Verify the buffer address actually has the claimed alignment
    debug_assert_fmt(mem_isAligned(ptrToInt(buf.ptr), buf_align), "Buffer address does not match the specified alignment");

    $unused(ctx, buf_align);

    let new_size_aligned = mem_alignForward(new_len, mem_page_size);
    let buf_aligned_len = mem_alignForward(buf.len, mem_page_size);

    if (new_size_aligned == buf_aligned_len && new_len <= buf.len) {
        return_some(buf.ptr); // No resize needed
    }

#if plat_windows
    // Windows PageAllocator in zig doesn't support resize larger.
    return_none(); // Indicate remap failure, as resize up is not supported.

#else /* posix */

    if (new_size_aligned < buf_aligned_len) {
        return_none(); // Indicate remap failure, shrinking via remap not supported
    }

#ifdef MAP_REMAP // Check if mremap is available
    if (MAP_REMAP != 0) { // MAP_REMAP is defined and not 0, mremap is likely available
        let new_ptr = mremap(buf.ptr, buf.len, new_len, MREMAP_MAYMOVE);
        if (new_ptr != MAP_FAILED) {
            // TODO: if heap_Page_s_next_mmap_addr_hint is within the remapped range, update it if moved.
            return_some(new_ptr); // Assume success for now, further hint update needed if address changes.
        }
        // mremap failed, fall back to returning none.
        return_none();
    }
#endif

    // mremap is not available or failed, larger resize is not supported in this simple page allocator.
    return_none();
#endif /* posix */
} $unscoped_(fn);

static fn_((heap_Page_free(P$raw ctx, S$u8 buf, u32 buf_align))(void)) {
    debug_assert_fmt(mem_isValidAlign(buf_align), "Alignment must be a power of 2");
    debug_assert_fmt(buf_align <= mem_page_size, "Page allocator only guarantees page alignment");
    // Verify the buffer address actually has the claimed alignment
    debug_assert_fmt(mem_isAligned(ptrToInt(buf.ptr), buf_align), "Buffer address does not match the specified alignment");

    let_ignore = ctx;
    let_ignore = buf_align;

#if plat_windows
    VirtualFree(buf.ptr, 0, MEM_RELEASE);
#else  /* posix */
    let buf_aligned_len = mem_alignForward(buf.len, mem_page_size);
    munmap(buf.ptr, buf_aligned_len);
#endif /* posix */
}

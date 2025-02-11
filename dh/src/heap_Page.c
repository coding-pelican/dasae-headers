#include "dh/builtin/plat_cfg.h"
#if bti_plat_windows
#include <windows.h>
#include <memoryapi.h>
#else
#include <sys/mman.h>
#include <unistd.h>
#endif

#include "dh/heap/Page.h"
#include "dh/mem.h"

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
    unused(ctx);
    unused(ptr_align);
    // debug_assert_fmt(0 < len, "Allocation length must be greater than zero");

    // Check for overflow when aligning to page size
    if (usize_limit - (mem_page_size - 1) < len) {
        return_none();
    }

#if bti_plat_windows
    // Windows implementation
    let addr = VirtualAlloc(
        null,
        len, // VirtualAlloc rounds to page size internally
        MEM_COMMIT | MEM_RESERVE,
        PAGE_READWRITE
    );
    if (addr == null) {
        return_none();
    }
    return_some(addr);
#else
    // POSIX implementation
    const usize  aligned_len = mem_alignForward(len, mem_page_size);
    const anyptr hint        = heap_Page_s_next_mmap_addr_hint;

    let map = mmap(
        hint,
        aligned_len,
        PROT_READ | PROT_WRITE,
        MAP_PRIVATE | MAP_ANONYMOUS,
        -1,
        0
    );
    if (map == MAP_FAILED) {
        return_none();
    }

    debug_assert(mem_isAligned(intFromRawptr(map), mem_page_size));
    let new_hint = as$(anyptr, as$(u8*, map) + aligned_len);
    // Here we use atomic operations to update the hint
    __atomic_compare_exchange_n(
        &heap_Page_s_next_mmap_addr_hint,
        &hint,
        new_hint,
        false,
        __ATOMIC_SEQ_CST,
        __ATOMIC_SEQ_CST
    );
    return_some(map);
#endif
}

static bool heap_Page_resize(anyptr ctx, Sli$u8 buf, usize buf_align, usize new_size) {
    unused(ctx);
    unused(buf_align);

    const usize new_size_aligned = mem_alignForward(new_size, mem_page_size);

#if bti_plat_windows
    if (new_size <= buf.len) {
        const usize base_addr    = as$(usize, buf.ptr);
        const usize old_addr_end = base_addr + buf.len;
        const usize new_addr_end = mem_alignForward(base_addr + new_size, mem_page_size);

        if (old_addr_end > new_addr_end) {
            // For shrinking that is not releasing, we will only
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
    if (new_size_aligned <= old_size_aligned) {
        return true;
    }
    return false;
#else
    const usize buf_aligned_len = mem_alignForward(buf.len, mem_page_size);
    if (new_size_aligned == buf_aligned_len) {
        return true;
    }

    if (new_size_aligned < buf_aligned_len) {
        anyptr ptr = buf.ptr + new_size_aligned;
        // TODO: if heap_Page_s_next_mmap_addr_hint is within the unmapped range, update it
        munmap(ptr, buf_aligned_len - new_size_aligned);
        return true;
    }

    // TODO: call mremap
    // TODO: if heap_Page_s_next_mmap_addr_hint is within the remapped range, update it
    return false;
#endif
}

static void heap_Page_free(anyptr ctx, Sli$u8 buf, usize buf_align) {
    unused(ctx);
    unused(buf_align);

#if bti_plat_windows
    VirtualFree(buf.ptr, 0, MEM_RELEASE);
#else
    const usize buf_aligned_len = mem_alignForward(buf.len, mem_page_size);
    munmap(buf.ptr, buf_aligned_len);
#endif
}

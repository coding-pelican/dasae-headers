#include "dh/heap/Classic.h"


mem_Allocator heap_Classic_allocator(heap_Classic* self) {
    /* VTable implementations */
    extern must_check Optptr$u8  heap_Classic_alloc(anyptr ctx, usize len, usize ptr_align);
    extern must_check bool       heap_Classic_resize(anyptr ctx, Sli$u8 buf, usize buf_align, usize new_len);
    extern void                  heap_Classic_free(anyptr ctx, Sli$u8 buf, usize buf_align);
    /* VTable for Classic allocator */
    static const mem_AllocatorVT vt[1] = { {
        .alloc  = heap_Classic_alloc,
        .resize = heap_Classic_resize,
        .free   = heap_Classic_free,
    } };
    return (mem_Allocator){
        .ctx = self,
        .vt  = vt
    };
}

Err$void heap_Classic_init(mem_Allocator self) {
    reserveReturn(Err$void);
    debug_assert_nonnull(self.ctx);
    debug_assert_nonnull(self.vt);
    unused(self);
    return_void();
}

void heap_Classic_fini(mem_Allocator self) {
    debug_assert_nonnull(self.ctx);
    debug_assert_nonnull(self.vt);
    unused(self);
}

Optptr$u8 heap_Classic_alloc(anyptr ctx, usize len, usize ptr_align) {
    reserveReturn(Optptr$u8);
    unused(ctx);

    // Allocate aligned memory
    anyptr ptr = null;
#if defined(_MSC_VER) || defined(__MINGW32__) || defined(__MINGW64__)
    ptr = _aligned_malloc(len, ptr_align);
#elif defined(_POSIX_VERSION) && _POSIX_VERSION >= 200112L
    if (posix_memalign(&ptr, align, size) != 0) {
        ptr = null;
    }
#else
    // Manual alignment
    const usize header_size = sizeof(anyptr);
    anyptr      raw         = malloc(size + align + header_size);
    if (raw != null) {
        ptr                = (u8*)raw + header_size;
        ptr                = (anyptr)(((usize)ptr + align - 1) & ~(align - 1));
        ((anyptr*)ptr)[-1] = raw;
    }
#endif
    if (ptr == null) { return_none(); }
    return_some(ptr);
}

bool heap_Classic_resize(anyptr ctx, Sli$u8 buf, usize buf_align, usize new_len) {
    debug_assert_nonnull(buf.ptr);
    unused(ctx);
    unused(buf_align);

    // Special cases
    if (new_len == 0) { return true; }

    // Get current allocation size
    let ptr = buf.ptr;
    if (ptr == null) { return false; }

    // Check if new size fits in current allocation
#if heap_Classic_has_malloc_size
    const usize full_size = heap_Classic_mallocSize(addr);
    if (new_size <= full_size) { return true; }
#else
    // Without malloc_size, we can only shrink within the original allocation
    // Use PtrBase size which represents the original allocation size
    const usize original_size = buf.len;
    if (new_len <= original_size) { return true; }
#endif
    return false;
}

void heap_Classic_free(anyptr ctx, Sli$u8 buf, usize buf_align) {
    debug_assert_nonnull(buf.ptr);
    unused(ctx);
    unused(buf_align);

    var raw_ptr = (anyptr)buf.ptr;
    if (raw_ptr == null) { return; }

#if defined(_MSC_VER) || defined(__MINGW32__) || defined(__MINGW64__)
    _aligned_free(raw_ptr);
#elif defined(_POSIX_VERSION) && _POSIX_VERSION >= 200112L
    free(raw_ptr);
#else
    // Manual alignment cleanup
    free(((anyptr*)raw_ptr)[-1]);
#endif
}

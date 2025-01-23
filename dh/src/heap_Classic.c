#include "dh/heap/Classic.h"

mem_Allocator heap_Classic_allocator(heap_Classic* self) {
    /* VTable implementations */
    extern Opt$Ptr$u8            heap_Classic_alloc(anyptr ctx, usize len, usize ptr_align) must_check;
    extern bool                  heap_Classic_resize(anyptr ctx, Sli$u8 buf, usize buf_align, usize new_len) must_check;
    extern void                  heap_Classic_free(anyptr ctx, Sli$u8 buf, usize buf_align);
    /* VTable for Classic allocator */
    static const mem_AllocatorVT vt[1] = { {
        .alloc  = heap_Classic_alloc,
        .resize = heap_Classic_resize,
        .free   = heap_Classic_free,
    } };
    return (mem_Allocator){
        .ptr = self,
        .vt  = vt
    };
}

Err$void heap_Classic_init(mem_Allocator self) {
    reserveReturn(Err$void);
    debug_assert_nonnull(self.ptr);
    debug_assert_nonnull(self.vt);
    unused(self);
    return_void();
}

void heap_Classic_fini(mem_Allocator self) {
    debug_assert_nonnull(self.ptr);
    debug_assert_nonnull(self.vt);
    unused(self);
}

Opt$Ptr$u8 heap_Classic_alloc(anyptr ctx, usize len, usize ptr_align) {
    reserveReturn(Opt$Ptr$u8);
    unused(ctx);

    // Allocate aligned memory
    anyptr ptr = null;

#if defined(_MSC_VER) || defined(__MINGW32__) || defined(__MINGW64__)
    ptr = _aligned_malloc(len, ptr_align);
#elif defined(_POSIX_VERSION) && _POSIX_VERSION >= 200112L
    if (posix_memalign(&ptr, ptr_align, len) != 0) {
        ptr = null;
    }
#else
    // Manual alignment
    const usize header_size = sizeof(anyptr) + (ptr_align - 1);
    anyptr      raw         = malloc(len + header_size);
    if (raw != null) {
        ptr                = (u8*)raw + header_size;
        ptr                = (anyptr)(((usize)ptr + ptr_align - 1) & ~(ptr_align - 1));
        ((anyptr*)ptr)[-1] = raw;
    }
#endif

    if (ptr == null) { return_none(); }
    return_some(ptr);
}

bool heap_Classic_resize(anyptr ctx, Sli$u8 buf, usize buf_align, usize new_len) {
    unused(ctx);
    unused(buf_align);

    // Get current allocation size
    let ptr = buf.ptr;
    if (ptr == null) { return false; }

    // Special cases
    if (new_len == 0) { return true; }

    // Check if new size fits in current allocation
#if heap_Classic_has_malloc_size
    const usize full_size = heap_Classic_mallocSize(ptr);
    if (new_len <= full_size) { return true; }
#else
    // Without malloc_size, we can only shrink within the original allocation
    const usize original_size = buf.len;
    if (new_len <= original_size) { return true; }
#endif

    // Resizing to a larger size is not supported
    return false;
}

void heap_Classic_free(anyptr ctx, Sli$u8 buf, usize buf_align) {
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

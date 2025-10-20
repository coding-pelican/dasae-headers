#include "dh/heap/Classic.h"
#include "dh/mem/common.h"

#include <stdlib.h>

// Forward declarations for allocator vtable functions
static fn_((heap_Classic_alloc(anyptr ctx, usize len, u32 align))(Opt$Ptr$u8));
static fn_((heap_Classic_resize(anyptr ctx, Sli$u8 buf, u32 buf_align, usize new_len))(bool));
static fn_((heap_Classic_remap(anyptr ctx, Sli$u8 buf, u32 buf_align, usize new_len))(Opt$Ptr$u8));
static fn_((heap_Classic_free(anyptr ctx, Sli$u8 buf, u32 buf_align))(void));

fn_((heap_Classic_allocator(heap_Classic* self))(mem_Allocator)) {
    debug_assert_nonnull(self);
    // VTable for Classic allocator
    static const mem_Allocator_VT vt[1] = { {
        .alloc  = heap_Classic_alloc,
        .resize = heap_Classic_resize,
        .remap  = heap_Classic_remap,
        .free   = heap_Classic_free,
    } };
    return (mem_Allocator){
        .ptr = self,
        .vt  = vt
    };
}

/*========== Allocator Interface Implementation =============================*/

static fn_((heap_Classic_alloc(anyptr ctx, usize len, u32 align))(Opt$Ptr$u8) $scope) {
    debug_assert_fmt(mem_isValidAlign(align), "Alignment must be a power of 2");
    $unused(ctx);

    // Allocate aligned memory
#if defined(_MSC_VER) || defined(__MINGW32__) || defined(__MINGW64__)
    if_(let ptr = _aligned_malloc(len, align), ptr != null) { return_some(ptr); }
#elif defined(_POSIX_VERSION) && _POSIX_VERSION >= 200112L
    if_(var ptr = make$(anyptr), posix_memalign(&ptr, align, len) == 0) { return_some(ptr); }
#else  /* other platforms */
    // Manual alignment with proper header storage
    // Allocate extra space for the original pointer and alignment padding
    let header_size = sizeOf$(anyptr);
    let total_size  = len + header_size + align - 1;
    // Check for overflow
    if (total_size < len) { return_none(); } // Overflow occurred

    // Allocate raw memory
    var raw = malloc(total_size);
    if (raw != null) {
        // Calculate aligned address, leaving space for the header
        let raw_addr     = rawptrToInt(raw);
        let aligned_addr = mem_alignForward(raw_addr + header_size, align);

        // Store the original pointer just before the aligned address
        let ptr        = intToRawptr$(anyptr, aligned_addr);
        var header_ptr = intToRawptr$(anyptr*, aligned_addr - header_size);
        *header_ptr    = raw;
        return_some(ptr);
    }
#endif /* other platforms */

    // Failed to allocate memory
    return_none();
} $unscoped;

static fn_((heap_Classic_resize(anyptr ctx, Sli$u8 buf, u32 buf_align, usize new_len))(bool)) {
    debug_assert_fmt(mem_isValidAlign(buf_align), "Alignment must be a power of 2");
    // Verify the buffer address actually has the claimed alignment
    debug_assert_fmt(mem_isAligned(rawptrToInt(buf.ptr), buf_align), "Buffer address does not match the specified alignment");

    let_ignore = ctx;
    let_ignore = buf_align;

    // Get current allocation size
    let ptr = buf.ptr;
    if (ptr == null) { return false; }

    // Special cases
    if (new_len == 0) { return true; }

    // Check if new size fits in current allocation
#if heap_Classic_has_malloc_size
    const usize full_size = heap_Classic_mallocSize(ptr);
    if (new_len <= full_size) { return true; }
#else  /* !heap_Classic_has_malloc_size */
    // Without malloc_size, we can only shrink within the original allocation
    const usize original_size = buf.len;
    if (new_len <= original_size) { return true; }
#endif /* !heap_Classic_has_malloc_size */

    // Resizing to a larger size is not supported
    return false;
}

static fn_((heap_Classic_remap(anyptr ctx, Sli$u8 buf, u32 buf_align, usize new_len))(Opt$Ptr$u8) $scope) {
    debug_assert_fmt(mem_isValidAlign(buf_align), "Alignment must be a power of 2");
    debug_assert_fmt(mem_isAligned(rawptrToInt(buf.ptr), buf_align), "Buffer address does not match the specified alignment");

    $unused(ctx);

    // If the buffer is null, treat it as a malloc.
    if (buf.ptr == null) {
        return heap_Classic_alloc(ctx, new_len, buf_align);
    }

    // If the new length is zero, treat it as a free, and return NULL.
    if (new_len == 0) {
        heap_Classic_free(ctx, buf, buf_align);
        return_none();
    }
    // If the new size is smaller or equal to the usable size of the current block try to shrink and return the same pointer.
    if (heap_Classic_resize(ctx, buf, buf_align, new_len)) {
        return_some(buf.ptr);
    }

#if defined(_MSC_VER) || defined(__MINGW32__) || defined(__MINGW64__)
    // Use _aligned_realloc for MSVC, MinGW.
    if_(let new_ptr = _aligned_realloc(buf.ptr, new_len, buf_align), new_ptr != null) {
        return_some(new_ptr);
    }
#elif defined(_POSIX_VERSION) && _POSIX_VERSION >= 200112L
    // Use realloc for POSIX systems. It does not guarantee to maintain alignment.
    if_(let new_ptr = realloc(buf.ptr, new_len), new_ptr != null) {
        // we allocated a different block, make sure the alignment is correct.
        if (mem_isAligned(rawptrToInt(new_ptr), buf_align)) {
            return_some(new_ptr);
        } else {
            // Alignment is lost. The specification does not support this, free the new_ptr, and return none.
            free(new_ptr);
            return_none();
        }
    }
#else
    // Fallback implementation:  Allocate, copy, and free.
    //   This is necessary because realloc *does not* guarantee alignment.

    // 1. Allocate a new block with the requested size and alignment.
    Opt$Ptr$u8 new_buf = heap_Classic_alloc(ctx, new_len, buf_align);
    if (new_buf.ptr == null) {
        return_none(); // Allocation failed.
    }

    // 2. Copy the data from the old block to the new block.
    usize copy_len = (buf.len < new_len) ? buf.len : new_len; // Copy the smaller of the two sizes.
    mem_copy(new_buf.ptr, buf.ptr, copy_len);
    // 3. Free the old block.
    heap_Classic_free(ctx, buf, buf_align);

    // 4. Return the new block.
    return new_buf;
#endif
    return_none();
} $unscoped;

static fn_((heap_Classic_free(anyptr ctx, Sli$u8 buf, u32 buf_align))(void)) {
    debug_assert_fmt(mem_isValidAlign(buf_align), "Alignment must be a power of 2");
    // Verify the buffer address actually has the claimed alignment
    debug_assert_fmt(mem_isAligned(rawptrToInt(buf.ptr), buf_align), "Buffer address does not match the specified alignment");

    let_ignore = ctx;
    let_ignore = buf_align;

    var raw_ptr = as$(anyptr, buf.ptr);
    if (raw_ptr == null) { return; }

#if defined(_MSC_VER) || defined(__MINGW32__) || defined(__MINGW64__)
    _aligned_free(raw_ptr);
#elif defined(_POSIX_VERSION) && _POSIX_VERSION >= 200112L
    free(raw_ptr);
#else  /* other platforms */
    // Manual alignment cleanup - retrieve the original pointer
    var header_ptr   = intToRawptr$(anyptr*, rawptrToInt(raw_ptr) - sizeOf(anyptr));
    var original_ptr = *header_ptr;
    free(original_ptr);
#endif /* other platforms */
}

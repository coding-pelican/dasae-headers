#include "dh/heap/Classic.h"
#include "dh/mem/common.h"

static Opt$Ptr$u8 heap_Classic_alloc(anyptr ctx, usize len, usize ptr_align) must_check;
static bool       heap_Classic_resize(anyptr ctx, Sli$u8 buf, usize buf_align, usize new_len) must_check;
static void       heap_Classic_free(anyptr ctx, Sli$u8 buf, usize buf_align);

mem_Allocator heap_Classic_allocator(heap_Classic* self) {
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

/*========== Allocator Interface Implementation =============================*/

static Opt$Ptr$u8 heap_Classic_alloc(anyptr ctx, usize len, usize ptr_align) {
    reserveReturn(Opt$Ptr$u8);
    unused(ctx);
    // Ensure alignment is a power of 2
    debug_assert_fmt(mem_isValidAlign(ptr_align), "Alignment must be a power of 2");

    // Allocate aligned memory
#if defined(_MSC_VER) || defined(__MINGW32__) || defined(__MINGW64__)
    if_(let ptr = _aligned_malloc(len, ptr_align), ptr != null) { return_some(ptr); }
#elif defined(_POSIX_VERSION) && _POSIX_VERSION >= 200112L
    if_(var ptr = make$(anyptr), posix_memalign(&ptr, ptr_align, len) == 0) { return_some(ptr); }
#else  /* other platforms */
    // Manual alignment with proper header storage
    // Allocate extra space for the original pointer and alignment padding
    let header_size = sizeOf$(anyptr);
    let total_size  = len + header_size + ptr_align - 1;
    // Check for overflow
    if (total_size < len) { return_none(); } // Overflow occurred

    // Allocate raw memory
    var raw = malloc(total_size);
    if (raw != null) {
        // Calculate aligned address, leaving space for the header
        let raw_addr     = rawptrToInt(raw);
        let aligned_addr = mem_alignForward(raw_addr + header_size, ptr_align);

        // Store the original pointer just before the aligned address
        let ptr        = intToRawptr$(anyptr, aligned_addr);
        var header_ptr = intToRawptr$(anyptr*, aligned_addr - header_size);
        *header_ptr    = raw;
        return_some(ptr);
    }
#endif /* other platforms */

    // Failed to allocate memory
    return_none();
}

static bool heap_Classic_resize(anyptr ctx, Sli$u8 buf, usize buf_align, usize new_len) {
    unused(ctx, buf_align);
    // Ensure alignment is a power of 2
    debug_assert_fmt(mem_isValidAlign(buf_align), "Alignment must be a power of 2");
    // Verify the buffer address actually has the claimed alignment
    debug_assert_fmt(mem_isAligned(rawptrToInt(buf.ptr), buf_align), "Buffer address does not match the specified alignment");

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

static void heap_Classic_free(anyptr ctx, Sli$u8 buf, usize buf_align) {
    unused(ctx, buf_align);
    // Ensure alignment is a power of 2
    debug_assert_fmt(mem_isValidAlign(buf_align), "Alignment must be a power of 2");
    // Verify the buffer address actually has the claimed alignment
    debug_assert_fmt(mem_isAligned(rawptrToInt(buf.ptr), buf_align), "Buffer address does not match the specified alignment");

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

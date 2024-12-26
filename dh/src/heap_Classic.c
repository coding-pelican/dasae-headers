// #include "dh/core/Err.h"
// #include "dh/core/prim/struct.h"
// #include "dh/heap/Classic.h"

// extern mem_Allocator heap_Classic_allocator(void) {
//     static heap_Classic instance = cleared();
//     return (mem_Allocator){
//         .ctx = &instance,
//         .vt  = heap_Classic_vt
//     };
// }

// ResErr_Void heap_Classic_init(Ptr_mem_Allocator self) {
//     debug_assert_nonnull_fmt(self.addr, "Null allocator");
//     unused(self);
//     return ResErr_Void_ok(makeCleared(Void));
// }

// void heap_Classic_fini(Ptr_mem_Allocator self) {
//     debug_assert_nonnull_fmt(self.addr, "Null allocator");
//     unused(self);
// }

// ResErr_Ptr heap_Classic_alloc(Ptr ctx, TypeInfo type) {
//     debug_assert_nonnull(ctx.addr);
//     debug_assert_fmt(0 < type.size, "Type size must be greater than 0");
//     unused(ctx);

//     // Allocate aligned memory
//     anyptr ptr = null;

// #if defined(_MSC_VER) || defined(__MINGW32__) || defined(__MINGW64__)
//     ptr = _aligned_malloc(type.size, type.align);
// #elif defined(_POSIX_VERSION) && _POSIX_VERSION >= 200112L
//     if (posix_memalign(&ptr, align, size) != 0) {
//         ptr = null;
//     }
// #else
//     // Manual alignment
//     const usize header_size = sizeof(anyptr);
//     anyptr      raw         = malloc(size + align + header_size);
//     if (raw != null) {
//         ptr                = (u8*)raw + header_size;
//         ptr                = (anyptr)(((usize)ptr + align - 1) & ~(align - 1));
//         ((anyptr*)ptr)[-1] = raw;
//     }
// #endif

//     if (ptr == null) {
//         return ResErr_Ptr_err(Err_InsufficientMemory);
//     }
//     return ResErr_Ptr_ok(Ptr_from(type, ptr));
// }

// bool heap_Classic_resize(Ptr ctx, Ptr ptr, usize new_len) {
//     debug_assert_nonnull(ctx.addr);
//     debug_assert_nonnull(ptr.addr);
//     unused(ctx);

//     // Special cases
//     if (new_len == 0) { return true; }

//     // Get current allocation size
//     let addr = ptr.addr;
//     if (addr == null) { return false; }

//     // Check if new size fits in current allocation
// #if heap_Classic_has_malloc_size
//     const usize full_size = heap_Classic_mallocSize(addr);
//     if (new_size <= full_size) { return true; }
// #else
//     // Without malloc_size, we can only shrink within the original allocation
//     // Use PtrBase size which represents the original allocation size
//     const usize original_size = ptr.type.size;
//     if (new_len <= original_size) {
//         return true;
//     }
// #endif
//     return false;
// }

// void heap_Classic_free(Ptr ctx, Ptr ptr) {
//     debug_assert_nonnull(ctx.addr);
//     debug_assert_nonnull(ptr.addr);
//     unused(ctx);

//     var raw_ptr = ptr.addr;

// #if defined(_MSC_VER) || defined(__MINGW32__) || defined(__MINGW64__)
//     _aligned_free(raw_ptr);
// #elif defined(_POSIX_VERSION) && _POSIX_VERSION >= 200112L
//     free(raw_ptr);
// #else
//     // Manual alignment cleanup
//     free(((anyptr*)raw_ptr)[-1]);
// #endif
// }

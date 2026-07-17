#include "dh/heap/Classic.h"
#include "dh/mem/common.h"

/*========== Internal Declarations ==========================================*/

#if heap_Classic_enabled
$attr($inline_always)
$static fn_((heap_Classic__libcAlign(mem_Align align))(usize));
#define heap_Classic__has_malloc_size __comp_bool__heap_Classic__has_malloc_size
/// Get underlying malloc_size if available
$attr($maybe_unused $inline_always)
$static fn_((heap_Classic__mallocSize(P$raw target_ptr))(usize));

$static fn_((heap_Classic__alloc(P$raw ctx, usize len, mem_Align align))(O$P$u8));
$static fn_((heap_Classic__resize(P$raw ctx, S$u8 buf, mem_Align buf_align, usize new_len))(bool));
$static fn_((heap_Classic__remap(P$raw ctx, S$u8 buf, mem_Align buf_align, usize new_len))(O$P$u8));
$static fn_((heap_Classic__free(P$raw ctx, S$u8 buf, mem_Align buf_align))(void));
#endif /* heap_Classic_enabled */

/*========== External Definitions ===========================================*/

fn_((heap_Classic_alctr(heap_Classic* self))(heap_Classic_E$mem_Alctr) $scope) pp_if_(heap_Classic_enabled)(
    pp_then_({
        // VTable for Classic allocator
        $static let_(vtbl, mem_Alctr_VTbl) = {
            .allocFn = heap_Classic__alloc,
            .resizeFn = heap_Classic__resize,
            .remapFn = heap_Classic__remap,
            .freeFn = heap_Classic__free,
        };
        return_ok(mem_Alctr_ensureValid((mem_Alctr){
            .ctx = self,
            .vtbl = &vtbl,
        }));
    }),
    pp_else_({
        let_ignore = self;
        return_err(E_cause$heap_Classic_Unsupported());
    })
) $unscoped(fn);

/*========== Internal Definitions ===========================================*/

#if heap_Classic_enabled
#include <stdlib.h>

fn_((heap_Classic__libcAlign(mem_Align align))(usize)) {
    return pri_max(mem_log2ToAlign(align), sizeOf$(P$raw));
};

#if defined(__GLIBC__) || defined(__APPLE__)
#define __comp_bool__heap_Classic__has_malloc_size pp_true
#else /* other platforms */
#define __comp_bool__heap_Classic__has_malloc_size pp_false
#endif /* other platforms */

#if heap_Classic__has_malloc_size
#if defined(__GLIBC__)
#include <malloc.h>
#elif defined(__APPLE__)
#include <malloc/malloc.h>
#else /* other platforms */
#endif /* other platforms */
#endif /* heap_Classic__has_malloc_size */

fn_((heap_Classic__mallocSize(P$raw target_ptr))(usize)) {
#if heap_Classic__has_malloc_size
#if defined(__GLIBC__)
    return malloc_usable_size(target_ptr);
#elif defined(__APPLE__)
    return malloc_size(target_ptr);
#else /* other platforms */
#endif /* other platforms */
#endif /* heap_Classic__has_malloc_size */
    return $ignore_void target_ptr, 0;
};

$static fn_((heap_Classic__alloc(P$raw ctx, usize len, mem_Align align))(O$P$u8) $scope) {
    let_ignore = ctx;
    let ptr_align = heap_Classic__libcAlign(align);

    // Allocate aligned memory
#if defined(_MSC_VER) || defined(__MINGW32__) || defined(__MINGW64__)
    if_(let ptr = _aligned_malloc(len, ptr_align), ptr != null) return_some(ptr);
#elif plat_is_posix
    if_(var ptr = null$(P$raw), posix_memalign(&ptr, ptr_align, len) == 0) return_some(ptr);
#else /* other platforms */
    // Manual alignment with proper header storage
    // Allocate extra space for the original pointer and alignment padding
    let header_size = sizeOf$(P$raw);
    let total_size = orelse_((usize_addChkd(
        orelse_((usize_addChkd(len, header_size))(return_none())),
        ptr_align - 1
    ))(return_none()));

    // Allocate raw memory
    var raw = malloc(total_size);
    if (raw != null) {
        // Calculate aligned address, leaving space for the header
        let raw_addr = ptrToInt(raw);
        let aligned_addr = mem_alignFwd(raw_addr + header_size, ptr_align);

        // Store the original pointer just before the aligned address
        let ptr = intToPtr$((P$raw)(aligned_addr));
        var header_ptr = intToPtr$((P$raw*)(aligned_addr - header_size));
        *header_ptr = raw;
        return_some(ptr);
    }
#endif /* other platforms */

    return_none(); // Failed to allocate memory
} $unscoped(fn);

$static fn_((heap_Classic__resize(P$raw ctx, S$u8 buf, mem_Align buf_align, usize new_len))(bool)) {
    let_ignore = ctx;
    let_ignore = buf_align;
    if (buf.ptr == null) return false;
    if (new_len == 0) return true;

    let alloced_size = pp_if_(heap_Classic__has_malloc_size)(
        pp_then_(local_({
            let full_size = heap_Classic__mallocSize(buf.ptr);
            local_return_(full_size);
        })),
        pp_else_(local_({
            let orig_size = buf.len;
            local_return_(orig_size);
        })));
    if (new_len <= alloced_size) return true;

    return false;
};

$static fn_((heap_Classic__remap(P$raw ctx, S$u8 buf, mem_Align buf_align, usize new_len))(O$P$u8) $scope) {
    if (buf.ptr == null) {
        return heap_Classic__alloc(ctx, new_len, buf_align);
    }
    if (new_len == 0) {
        heap_Classic__free(ctx, buf, buf_align);
        return_none();
    }
    if (heap_Classic__resize(ctx, buf, buf_align, new_len)) {
        return_some(buf.ptr);
    }

#if defined(_MSC_VER) || defined(__MINGW32__) || defined(__MINGW64__)
    let ptr_align = heap_Classic__libcAlign(buf_align);
    if_(let new_ptr = _aligned_realloc(buf.ptr, new_len, ptr_align), new_ptr != null) {
        return_some(new_ptr);
    }
#endif /* defined(_MSC_VER) || defined(__MINGW32__) || defined(__MINGW64__) */
    return_none();
} $unscoped(fn);

$static fn_((heap_Classic__free(P$raw ctx, S$u8 buf, mem_Align buf_align))(void)) {
    let_ignore = ctx;
    let_ignore = buf_align;

    var raw_ptr = as$(P$raw)(buf.ptr);
    if (raw_ptr == null) { return; }
#if defined(_MSC_VER) || defined(__MINGW32__) || defined(__MINGW64__)
    _aligned_free(raw_ptr);
#elif plat_is_posix
    free(raw_ptr);
#else /* other platforms */
    // Manual alignment cleanup - retrieve the original pointer
    var header_ptr = intToPtr$((P$raw*)(ptrToInt(raw_ptr) - (sizeOf$(P$raw))));
    var original_ptr = *header_ptr;
    free(original_ptr);
#endif /* other platforms */
};
#endif /* heap_Classic_enabled */

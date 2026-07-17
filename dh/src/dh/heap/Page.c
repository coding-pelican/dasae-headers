#include "dh/heap/Page.h"
#include "dh/heap/VMap.h"
#include "dh/mem/common.h"

/*========== Internal Declarations ==========================================*/

$attr($inline_always)
$static fn_((heap_Page__vmap(heap_Page* self))(heap_VMap));
$attr($inline_always)
$static fn_((heap_Page__geom(heap_Page* self))(heap_Geom));
$attr($inline_always)
$static fn_((heap_Page__guaranteedAlign(heap_Page* self))(usize));
$attr($inline_always)
$static fn_((heap_Page__alignedLen(heap_Page* self, usize len))(usize));
$attr($inline_always)
$static fn_((heap_Page__assertContract(heap_Page* self, mem_Align align))(void));
$attr($inline_always)
$static fn_((heap_Page__hintAlign(heap_Page* self))(usize));

$attr($inline_always)
$static fn_((heap_Page__noHintAddr(void))(usize));
$attr($inline_always)
$static fn_((heap_Page__normalizeHintAddr(heap_Page* self, usize hint_addr))(usize));
$attr($inline_always)
$static fn_((heap_Page__hintAddrAsOpt(usize hint_addr))(O$P$raw));

$static fn_((heap_Page__loadHintAddr(heap_Page* self))(usize));
$static fn_((heap_Page__storeHintAddr(heap_Page* self, usize old_hint_addr, u8* map, usize aligned_len))(void));
$static fn_((heap_Page__updateHintForRelease(heap_Page* self, u8* released_ptr, usize released_len))(void));
$static fn_((heap_Page__thrdSafeLoadHintAddr(heap_Page* self))(usize));
$static fn_((heap_Page__thrdSafeStoreHintAddr(heap_Page* self, usize old_hint_addr, u8* map, usize aligned_len))(void));
$static fn_((heap_Page__thrdSafeUpdateHintForRelease(heap_Page* self, u8* released_ptr, usize released_len))(void));
#if !plat_is_windows
$static fn_((heap_Page__updateHintForRemap(heap_Page* self, u8* old_ptr, usize old_len, u8* new_ptr, usize new_len))(void));
$static fn_((heap_Page__shrinkPosix(heap_Page* self, S$u8 buf, usize new_aligned_len))(bool));
$static fn_((heap_Page__thrdSafeUpdateHintForRemap(heap_Page* self, u8* old_ptr, usize old_len, u8* new_ptr, usize new_len))(void));
$static fn_((heap_Page__thrdSafeShrinkPosix(heap_Page* self, S$u8 buf, usize new_aligned_len))(bool));
#endif

$static fn_((heap_Page__alloc(P$raw ctx, usize len, mem_Align align))(O$P$u8));
$static fn_((heap_Page__resize(P$raw ctx, S$u8 buf, mem_Align buf_align, usize new_len))(bool));
$static fn_((heap_Page__remap(P$raw ctx, S$u8 buf, mem_Align buf_align, usize new_len))(O$P$u8));
$static fn_((heap_Page__free(P$raw ctx, S$u8 buf, mem_Align buf_align))(void));

$static fn_((heap_Page__thrdSafeAlloc(P$raw ctx, usize len, mem_Align align))(O$P$u8));
$static fn_((heap_Page__thrdSafeResize(P$raw ctx, S$u8 buf, mem_Align buf_align, usize new_len))(bool));
$static fn_((heap_Page__thrdSafeRemap(P$raw ctx, S$u8 buf, mem_Align buf_align, usize new_len))(O$P$u8));
$static fn_((heap_Page__thrdSafeFree(P$raw ctx, S$u8 buf, mem_Align buf_align))(void));

/*========== External Definitions ===========================================*/

fn_((heap_Page_alctr(heap_Page* self))(mem_Alctr)) {
    $static let_(vtbl, mem_Alctr_VTbl) = {
        .allocFn = heap_Page__alloc,
        .resizeFn = heap_Page__resize,
        .remapFn = heap_Page__remap,
        .freeFn = heap_Page__free,
    };
    return mem_Alctr_ensureValid((mem_Alctr){
        .ctx = self,
        .vtbl = &vtbl,
    });
};

fn_((heap_Page_thrdSafeAlctr(heap_Page* self))(mem_Alctr)) {
    $static let_(vtbl, mem_Alctr_VTbl) = {
        .allocFn = heap_Page__thrdSafeAlloc,
        .resizeFn = heap_Page__thrdSafeResize,
        .remapFn = heap_Page__thrdSafeRemap,
        .freeFn = heap_Page__thrdSafeFree,
    };
    return mem_Alctr_ensureValid((mem_Alctr){
        .ctx = self,
        .vtbl = &vtbl,
    });
};

fn_((heap_Page_from(heap_VMap vmap))(heap_Page)) {
    return (heap_Page){
        .vmap = heap_VMap_ensureValid(vmap),
        ._next_addr_hint = heap_Page__noHintAddr(),
    };
};

/*========== Internal Definitions ===========================================*/

fn_((heap_Page__geom(heap_Page* self))(heap_Geom)) {
    return heap_VMap_geom(heap_Page__vmap(self));
};

fn_((heap_Page__vmap(heap_Page* self))(heap_VMap)) {
    claim_assert_nonnull(self);
    return heap_VMap_ensureValid(self->vmap);
};

fn_((heap_Page__guaranteedAlign(heap_Page* self))(usize)) {
    return heap_Page__geom(self).map_align;
};

fn_((heap_Page__alignedLen(heap_Page* self, usize len))(usize)) {
    return heap_Geom_alignPageWith(heap_Page__geom(self), len);
};

fn_((heap_Page__hintAlign(heap_Page* self))(usize)) {
    return heap_Page__geom(self).map_align;
};

fn_((heap_Page__noHintAddr(void))(usize)) {
    return ptrToInt(null);
};

fn_((heap_Page__assertContract(heap_Page* self, mem_Align align))(void)) {
    /* Page mappings are naturally aligned to heap_Page__guaranteedAlign().
     * Larger power-of-two alignments are supported by overmapping and trimming
     * the leading/trailing page ranges in heap_Page__alloc. */
    let_ignore = align;
    let_ignore = heap_Page__guaranteedAlign(self);
};

fn_((heap_Page__normalizeHintAddr(heap_Page* self, usize hint_addr))(usize)) {
    if (hint_addr == heap_Page__noHintAddr()) return heap_Page__noHintAddr();
    return mem_alignFwd(hint_addr, heap_Page__hintAlign(self));
};

fn_((heap_Page__hintAddrAsOpt(usize hint_addr))(O$P$raw)) {
    if (hint_addr == heap_Page__noHintAddr()) return none$((O$P$raw));
    return some$((O$P$raw)(intToPtr$((P$raw)(hint_addr))));
};

fn_((heap_Page__loadHintAddr(heap_Page* self))(usize)) {
    return heap_Page__normalizeHintAddr(self, self->_next_addr_hint);
};

fn_((heap_Page__storeHintAddr(heap_Page* self, usize old_hint_addr, u8* map, usize aligned_len))(void)) {
    let_ignore = old_hint_addr;
    let new_hint_addr = heap_Page__normalizeHintAddr(self, ptrToInt(map + aligned_len));
    self->_next_addr_hint = new_hint_addr;
};

fn_((heap_Page__updateHintForRelease(heap_Page* self, u8* released_ptr, usize released_len))(void)) {
    let start = ptrToInt(released_ptr);
    let end = start + released_len;
    let old_hint_addr = heap_Page__loadHintAddr(self);
    if (old_hint_addr == heap_Page__noHintAddr()) return;
    if (old_hint_addr < start || end < old_hint_addr) return;
    self->_next_addr_hint = heap_Page__normalizeHintAddr(self, ptrToInt(released_ptr));
};

fn_((heap_Page__thrdSafeLoadHintAddr(heap_Page* self))(usize)) {
    return heap_Page__normalizeHintAddr(self, atom_load(&self->_next_addr_hint, atom_MemOrd_seq_cst));
};

fn_((heap_Page__thrdSafeStoreHintAddr(heap_Page* self, usize old_hint_addr, u8* map, usize aligned_len))(void)) {
    let new_hint_addr = heap_Page__normalizeHintAddr(self, ptrToInt(map + aligned_len));
    let_ignore = atom_cmpXchgStrong(
        &self->_next_addr_hint,
        old_hint_addr,
        new_hint_addr,
        atom_MemOrd_seq_cst,
        atom_MemOrd_seq_cst
    );
};

fn_((heap_Page__thrdSafeUpdateHintForRelease(heap_Page* self, u8* released_ptr, usize released_len))(void)) {
    let start = ptrToInt(released_ptr);
    let end = start + released_len;
    while (true) {
        let old_hint_addr = heap_Page__thrdSafeLoadHintAddr(self);
        if (old_hint_addr == heap_Page__noHintAddr()) return;
        if (old_hint_addr < start || end < old_hint_addr) return;
        let new_hint_addr = heap_Page__normalizeHintAddr(self, ptrToInt(released_ptr));
        let_ignore = orelse_((atom_cmpXchgStrong(
            &self->_next_addr_hint,
            old_hint_addr,
            new_hint_addr,
            atom_MemOrd_seq_cst,
            atom_MemOrd_seq_cst
        ))(return));
    }
};

#if !plat_is_windows
fn_((heap_Page__updateHintForRemap(heap_Page* self, u8* old_ptr, usize old_len, u8* new_ptr, usize new_len))(void)) {
    let old_start = ptrToInt(old_ptr);
    let old_end = old_start + old_len;
    let old_hint_addr = heap_Page__loadHintAddr(self);
    if (old_hint_addr == heap_Page__noHintAddr()) return;
    if (old_hint_addr < old_start || old_end < old_hint_addr) return;
    self->_next_addr_hint = heap_Page__normalizeHintAddr(self, ptrToInt(new_ptr + new_len));
};

fn_((heap_Page__thrdSafeUpdateHintForRemap(heap_Page* self, u8* old_ptr, usize old_len, u8* new_ptr, usize new_len))(void)) {
    let old_start = ptrToInt(old_ptr);
    let old_end = old_start + old_len;
    while (true) {
        let old_hint_addr = heap_Page__thrdSafeLoadHintAddr(self);
        if (old_hint_addr == heap_Page__noHintAddr()) return;
        if (old_hint_addr < old_start || old_end < old_hint_addr) return;
        let new_hint_addr = heap_Page__normalizeHintAddr(self, ptrToInt(new_ptr + new_len));
        let_ignore = orelse_((atom_cmpXchgStrong(
            &self->_next_addr_hint,
            old_hint_addr,
            new_hint_addr,
            atom_MemOrd_seq_cst,
            atom_MemOrd_seq_cst
        ))(return));
    }
};

fn_((heap_Page__shrinkPosix(heap_Page* self, S$u8 buf, usize new_aligned_len))(bool)) {
    let buf_aligned_len = heap_Page__alignedLen(self, buf.len);
    if (buf_aligned_len <= new_aligned_len) return true;
    let tail_len = buf_aligned_len - new_aligned_len;
    if (tail_len == 0) return true;
    let tail_ptr = as$(u8*)(buf.ptr + new_aligned_len);
    if (!heap_VMap_release(heap_Page__vmap(self), tail_ptr, tail_len)) return false;
    heap_Page__updateHintForRelease(self, tail_ptr, tail_len);
    return true;
};

fn_((heap_Page__thrdSafeShrinkPosix(heap_Page* self, S$u8 buf, usize new_aligned_len))(bool)) {
    let buf_aligned_len = heap_Page__alignedLen(self, buf.len);
    if (buf_aligned_len <= new_aligned_len) return true;
    let tail_len = buf_aligned_len - new_aligned_len;
    if (tail_len == 0) return true;
    let tail_ptr = as$(u8*)(buf.ptr + new_aligned_len);
    if (!heap_VMap_release(heap_Page__vmap(self), tail_ptr, tail_len)) return false;
    heap_Page__thrdSafeUpdateHintForRelease(self, tail_ptr, tail_len);
    return true;
};
#endif

fn_((heap_Page__alloc(P$raw ctx, usize len, mem_Align align))(O$P$u8) $scope) {
    let self = ptrAlignCast$((heap_Page*)(ctx));
    let ptr_align = mem_log2ToAlign(align);
    let geom = heap_Page__geom(self);
    let vmap = heap_Page__vmap(self);
    heap_Page__assertContract(self, align);

    // Check for overflow when aligning to page size
    if (usize_limit - (geom.page_size - 1) < len) { return_none(); }

#if plat_is_windows
    let aligned_len = heap_Page__alignedLen(self, len);
    let hint_addr = heap_Page__loadHintAddr(self);
    let hint = heap_Page__hintAddrAsOpt(hint_addr);
    var addr = orelse_((heap_VMap_map(vmap, hint, len))(null));
    if (addr == null && hint_addr != heap_Page__noHintAddr()) {
        addr = orelse_((heap_VMap_map(vmap, none$((O$P$raw)), len))(null));
    }
    if (addr != null && mem_isAligned(ptrToInt(addr), ptr_align)) {
        heap_Page__storeHintAddr(self, hint_addr, addr, aligned_len);
        return_some(addr);
    }
    if (addr != null) {
        let_ignore = heap_VMap_release(vmap, addr, len);
    }

    // Fallback: map a temporary region, derive a target address from it,
    // release that region, then immediately map the desired subset there.
    // Another thread may have won the race to map the target range, in which
    // case a retry is needed.
    if (usize_limit - aligned_len < ptr_align - geom.page_size) { return_none(); }
    let overalloc_len = aligned_len + (ptr_align - geom.page_size);

    $static let retry_limit = 4;
    for (var retry_count = 0; retry_count < retry_limit; ++retry_count) {
        let_ignore = retry_count;
        var overalloc_addr = orelse_((heap_VMap_map(vmap, hint, overalloc_len))(null));
        if (overalloc_addr == null && hint_addr != heap_Page__noHintAddr()) {
            overalloc_addr = orelse_((heap_VMap_map(vmap, none$((O$P$raw)), overalloc_len))(null));
        }
        if (overalloc_addr == null) {
            return_none();
        }

        let aligned_addr = mem_alignFwd(ptrToInt(overalloc_addr), ptr_align);
        let_ignore = heap_VMap_release(vmap, overalloc_addr, overalloc_len);

        let addr = orelse_((heap_VMap_map(vmap, some$((O$P$raw)(intToPtr$((P$raw)(aligned_addr)))), aligned_len))(null));
        if (addr != null) {
            heap_Page__storeHintAddr(self, hint_addr, addr, aligned_len);
            return_some(addr);
        }
        // If VirtualAlloc fails, it might be due to address collision, retry.
        // In a real-world scenario, consider adding a retry limit.
    }
    return_none();
#else /* posix */
    let aligned_len = heap_Page__alignedLen(self, len);
    let hint_addr = heap_Page__loadHintAddr(self);
    let hint = heap_Page__hintAddrAsOpt(hint_addr);
    if (ptr_align <= geom.map_align) {
        let map = orelse_((heap_VMap_map(vmap, hint, aligned_len))(null));
        if (map == null) return_none();
        claim_assert(mem_isAligned(ptrToInt(map), geom.page_size));
        claim_assert_fmt(mem_isAligned(ptrToInt(map), ptr_align), "mmap returned misaligned address");

        heap_Page__storeHintAddr(self, hint_addr, map, aligned_len);
        return_some(map);
    }

    if (usize_limit - aligned_len < ptr_align - geom.map_align) return_none();
    let overalloc_len = aligned_len + (ptr_align - geom.map_align);
    let overalloc_map = orelse_((heap_VMap_map(vmap, hint, overalloc_len))(null));
    if (overalloc_map == null) return_none();

    let overalloc_addr = ptrToInt(overalloc_map);
    let aligned_addr = mem_alignFwd(overalloc_addr, ptr_align);
    let prefix_len = aligned_addr - overalloc_addr;
    let suffix_addr = aligned_addr + aligned_len;
    let overalloc_end = overalloc_addr + overalloc_len;
    let suffix_len = overalloc_end - suffix_addr;

    if (prefix_len != 0) { let_ignore = heap_VMap_release(vmap, overalloc_map, prefix_len); }
    if (suffix_len != 0) { let_ignore = heap_VMap_release(vmap, intToPtr$((P$raw)(suffix_addr)), suffix_len); }

    let map = intToPtr$((u8*)(aligned_addr));
    claim_assert(mem_isAligned(ptrToInt(map), geom.page_size));
    claim_assert(mem_isAligned(ptrToInt(map), ptr_align));
    heap_Page__storeHintAddr(self, hint_addr, map, aligned_len);
    return_some(map);
#endif /* posix */
} $unscoped(fn);

fn_((heap_Page__resize(P$raw ctx, S$u8 buf, mem_Align buf_align, usize new_len))(bool)) {
    let self = ptrAlignCast$((heap_Page*)(ctx));
    let ptr_align = mem_log2ToAlign(buf_align);
    heap_Page__assertContract(self, buf_align);
    claim_assert_fmt(mem_isAligned(ptrToInt(buf.ptr), ptr_align), "Buffer address does not match the specified alignment");
    let_ignore = ptr_align;

    let new_size_aligned = heap_Page__alignedLen(self, new_len);
    let buf_aligned_len = heap_Page__alignedLen(self, buf.len);

    if (new_size_aligned == buf_aligned_len) {
        return true; // Same mapped page span can satisfy the requested logical size.
    }

#if plat_is_windows
    let_ignore = self;
    if (new_len <= buf.len) {
        return true;
    }

    // Windows PageAlctr in zig doesn't support resize larger.
    // Returning false to indicate resize failure.
    return false;

#else /* posix */

    if (new_size_aligned < buf_aligned_len) {
        return heap_Page__shrinkPosix(self, buf, new_size_aligned);
    }

    // rawResize must preserve the original address.  Growing across a mapped-page
    // span can require a moving remap, so leave that to rawRemap/remapFn.
    return false;
#endif /* posix */
};

fn_((heap_Page__remap(P$raw ctx, S$u8 buf, mem_Align buf_align, usize new_len))(O$P$u8) $scope) {
    let self = ptrAlignCast$((heap_Page*)(ctx));
    let ptr_align = mem_log2ToAlign(buf_align);
    heap_Page__assertContract(self, buf_align);
    debug_assert_fmt(mem_isAligned( ptrToInt(buf.ptr), ptr_align), "Buffer address does not match the specified alignment");
    let_ignore = ptr_align;

    let new_size_aligned = heap_Page__alignedLen(self, new_len);
    let buf_aligned_len = heap_Page__alignedLen(self, buf.len);

    if (new_size_aligned == buf_aligned_len) {
        return_some(buf.ptr); // Same mapped page span can satisfy the requested logical size.
    }

#if plat_is_windows
    let_ignore = self;
    // Windows PageAlctr in zig doesn't support resize larger.
    return_none(); // Indicate remap failure, as resize up is not supported.

#else /* posix */

    if (new_size_aligned < buf_aligned_len) {
        if (heap_Page__shrinkPosix(self, buf, new_size_aligned)) return_some(buf.ptr);
        return_none();
    }

    if_some((heap_VMap_remap(heap_Page__vmap(self), buf.ptr, buf.len, new_len))(new_ptr)) {
        heap_Page__updateHintForRemap(self, buf.ptr, buf_aligned_len, new_ptr, new_size_aligned);
        return_some(new_ptr); // Assume success for now, further hint update needed if address changes.
    }

    // mremap is not available or failed, larger resize is not supported in this simple page allocator.
    return_none();
#endif /* posix */
} $unscoped(fn);

fn_((heap_Page__free(P$raw ctx, S$u8 buf, mem_Align buf_align))(void)) {
    let self = ptrAlignCast$((heap_Page*)(ctx));
    let ptr_align = mem_log2ToAlign(buf_align);
    heap_Page__assertContract(self, buf_align);
    claim_assert_fmt(mem_isAligned( ptrToInt(buf.ptr), ptr_align), "Buffer address does not match the specified alignment");
    let_ignore = ptr_align;

    if (heap_VMap_release(heap_Page__vmap(self), buf.ptr, buf.len)) {
        heap_Page__updateHintForRelease(self, buf.ptr, heap_Page__alignedLen(self, buf.len));
    }
};

fn_((heap_Page__thrdSafeAlloc(P$raw ctx, usize len, mem_Align align))(O$P$u8) $scope) {
    let self = ptrAlignCast$((heap_Page*)(ctx));
    let ptr_align = mem_log2ToAlign(align);
    let geom = heap_Page__geom(self);
    let vmap = heap_Page__vmap(self);
    heap_Page__assertContract(self, align);

    if (usize_limit - (geom.page_size - 1) < len) { return_none(); }

#if plat_is_windows
    let aligned_len = heap_Page__alignedLen(self, len);
    let hint_addr = heap_Page__thrdSafeLoadHintAddr(self);
    let hint = heap_Page__hintAddrAsOpt(hint_addr);
    var addr = orelse_((heap_VMap_map(vmap, hint, len))(null));
    if (addr == null && hint_addr != heap_Page__noHintAddr()) {
        addr = orelse_((heap_VMap_map(vmap, none$((O$P$raw)), len))(null));
    }
    if (addr != null && mem_isAligned(ptrToInt(addr), ptr_align)) {
        heap_Page__thrdSafeStoreHintAddr(self, hint_addr, addr, aligned_len);
        return_some(addr);
    }
    if (addr != null) {
        let_ignore = heap_VMap_release(vmap, addr, len);
    }

    if (usize_limit - aligned_len < ptr_align - geom.page_size) { return_none(); }
    let overalloc_len = aligned_len + (ptr_align - geom.page_size);

    $static let retry_limit = 4;
    for (var retry_count = 0; retry_count < retry_limit; ++retry_count) {
        let_ignore = retry_count;
        var overalloc_addr = orelse_((heap_VMap_map(vmap, hint, overalloc_len))(null));
        if (overalloc_addr == null && hint_addr != heap_Page__noHintAddr()) {
            overalloc_addr = orelse_((heap_VMap_map(vmap, none$((O$P$raw)), overalloc_len))(null));
        }
        if (overalloc_addr == null) {
            return_none();
        }

        let aligned_addr = mem_alignFwd(ptrToInt(overalloc_addr), ptr_align);
        let_ignore = heap_VMap_release(vmap, overalloc_addr, overalloc_len);

        let addr = orelse_((heap_VMap_map(vmap, some$((O$P$raw)(intToPtr$((P$raw)(aligned_addr)))), aligned_len))(null));
        if (addr != null) {
            heap_Page__thrdSafeStoreHintAddr(self, hint_addr, addr, aligned_len);
            return_some(addr);
        }
    }
    return_none();
#else /* posix */
    let aligned_len = heap_Page__alignedLen(self, len);
    let hint_addr = heap_Page__thrdSafeLoadHintAddr(self);
    let hint = heap_Page__hintAddrAsOpt(hint_addr);
    if (ptr_align <= geom.map_align) {
        let map = orelse_((heap_VMap_map(vmap, hint, aligned_len))(null));
        if (map == null) return_none();
        claim_assert(mem_isAligned(ptrToInt(map), geom.page_size));
        claim_assert_fmt(mem_isAligned(ptrToInt(map), ptr_align), "mmap returned misaligned address");

        heap_Page__thrdSafeStoreHintAddr(self, hint_addr, map, aligned_len);
        return_some(map);
    }

    if (usize_limit - aligned_len < ptr_align - geom.map_align) return_none();
    let overalloc_len = aligned_len + (ptr_align - geom.map_align);
    let overalloc_map = orelse_((heap_VMap_map(vmap, hint, overalloc_len))(null));
    if (overalloc_map == null) return_none();

    let overalloc_addr = ptrToInt(overalloc_map);
    let aligned_addr = mem_alignFwd(overalloc_addr, ptr_align);
    let prefix_len = aligned_addr - overalloc_addr;
    let suffix_addr = aligned_addr + aligned_len;
    let overalloc_end = overalloc_addr + overalloc_len;
    let suffix_len = overalloc_end - suffix_addr;

    if (prefix_len != 0) { let_ignore = heap_VMap_release(vmap, overalloc_map, prefix_len); }
    if (suffix_len != 0) { let_ignore = heap_VMap_release(vmap, intToPtr$((P$raw)(suffix_addr)), suffix_len); }

    let map = intToPtr$((u8*)(aligned_addr));
    claim_assert(mem_isAligned(ptrToInt(map), geom.page_size));
    claim_assert(mem_isAligned(ptrToInt(map), ptr_align));
    heap_Page__thrdSafeStoreHintAddr(self, hint_addr, map, aligned_len);
    return_some(map);
#endif /* posix */
} $unscoped(fn);

fn_((heap_Page__thrdSafeResize(P$raw ctx, S$u8 buf, mem_Align buf_align, usize new_len))(bool)) {
    let self = ptrAlignCast$((heap_Page*)(ctx));
    let ptr_align = mem_log2ToAlign(buf_align);
    heap_Page__assertContract(self, buf_align);
    claim_assert_fmt(mem_isAligned(ptrToInt(buf.ptr), ptr_align), "Buffer address does not match the specified alignment");
    let_ignore = ptr_align;

    let new_size_aligned = heap_Page__alignedLen(self, new_len);
    let buf_aligned_len = heap_Page__alignedLen(self, buf.len);

    if (new_size_aligned == buf_aligned_len) return true;

#if plat_is_windows
    let_ignore = self;
    return new_len <= buf.len;
#else /* posix */
    if (new_size_aligned < buf_aligned_len) {
        return heap_Page__thrdSafeShrinkPosix(self, buf, new_size_aligned);
    }
    return false;
#endif /* posix */
};

fn_((heap_Page__thrdSafeRemap(P$raw ctx, S$u8 buf, mem_Align buf_align, usize new_len))(O$P$u8) $scope) {
    let self = ptrAlignCast$((heap_Page*)(ctx));
    let ptr_align = mem_log2ToAlign(buf_align);
    heap_Page__assertContract(self, buf_align);
    debug_assert_fmt(mem_isAligned( ptrToInt(buf.ptr), ptr_align), "Buffer address does not match the specified alignment");
    let_ignore = ptr_align;

    let new_size_aligned = heap_Page__alignedLen(self, new_len);
    let buf_aligned_len = heap_Page__alignedLen(self, buf.len);

    if (new_size_aligned == buf_aligned_len) return_some(buf.ptr);

#if plat_is_windows
    let_ignore = self;
    return_none();
#else /* posix */
    if (new_size_aligned < buf_aligned_len) {
        if (heap_Page__thrdSafeShrinkPosix(self, buf, new_size_aligned)) return_some(buf.ptr);
        return_none();
    }

    if_some((heap_VMap_remap(heap_Page__vmap(self), buf.ptr, buf.len, new_len))(new_ptr)) {
        heap_Page__thrdSafeUpdateHintForRemap(self, buf.ptr, buf_aligned_len, new_ptr, new_size_aligned);
        return_some(new_ptr);
    }

    return_none();
#endif /* posix */
} $unscoped(fn);

fn_((heap_Page__thrdSafeFree(P$raw ctx, S$u8 buf, mem_Align buf_align))(void)) {
    let self = ptrAlignCast$((heap_Page*)(ctx));
    let ptr_align = mem_log2ToAlign(buf_align);
    heap_Page__assertContract(self, buf_align);
    claim_assert_fmt(mem_isAligned( ptrToInt(buf.ptr), ptr_align), "Buffer address does not match the specified alignment");
    let_ignore = ptr_align;

    if (heap_VMap_release(heap_Page__vmap(self), buf.ptr, buf.len)) {
        heap_Page__thrdSafeUpdateHintForRelease(self, buf.ptr, heap_Page__alignedLen(self, buf.len));
    }
};

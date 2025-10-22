#include "dh/mem/Allocator.h"
#include "dh/mem/common.h"
#include "dh/mem/Tracker.h"
#include "dh/debug/assert.h"
#include "dh/claim/assert.h"
#include "dh/claim/unreachable.h"
#include "dh/int.h"

/*========== Common VTable Functions ========================================*/

fn_((mem_Allocator_VT_noAlloc(anyptr ctx, usize len, u32 align))(Opt$Ptr$u8)) {
    let_ignore = ctx;
    let_ignore = len;
    let_ignore = align;
    return none$(Opt$Ptr$u8);
}

fn_((mem_Allocator_VT_noResize(anyptr ctx, Sli$u8 buf, u32 buf_align, usize new_len))(bool)) {
    let_ignore = ctx;
    let_ignore = buf;
    let_ignore = buf_align;
    let_ignore = new_len;
    return false;
}

fn_((mem_Allocator_VT_noRemap(anyptr ctx, Sli$u8 buf, u32 buf_align, usize new_len))(Opt$Ptr$u8)) {
    let_ignore = ctx;
    let_ignore = buf;
    let_ignore = buf_align;
    let_ignore = new_len;
    return none$(Opt$Ptr$u8);
}

fn_((mem_Allocator_VT_noFree(anyptr ctx, Sli$u8 buf, u32 buf_align))(void)) {
    let_ignore = ctx;
    let_ignore = buf;
    let_ignore = buf_align;
}

/*========== Raw Allocation Functions =======================================*/

fn_((
#if !COMP_TIME || (COMP_TIME && !debug_comp_enabled)
    mem_Allocator_rawAlloc(mem_Allocator self, usize len, u32 align)
#else /* COMP_TIME && (!COMP_TIME || debug_comp_enabled) */
    mem_Allocator_rawAlloc_debug(mem_Allocator self, usize len, u32 align, SrcLoc src_loc)
#endif /* COMP_TIME && (!COMP_TIME || debug_comp_enabled) */
)(Opt$Ptr$u8)) {
    debug_assert_nonnull(self.vt);
    debug_assert_nonnull(self.vt->alloc);
    debug_assert_fmt(mem_isValidAlign(align), "Alignment must be a power of 2: %u", align);

    // Special case for zero-sized allocations
    if (len == 0) {
        // For zero-sized allocations, return a non-null pointer at max address
        // aligned to the requested alignment
        let addr = intToRawptr$(u8*, usize_limit_max & ~(align - 1));
#if !COMP_TIME || (COMP_TIME && !debug_comp_enabled)
#else  /* COMP_TIME && (!COMP_TIME || debug_comp_enabled) */
        mem_Tracker_registerAlloc(addr, len, src_loc);
#endif /* COMP_TIME && (!COMP_TIME || debug_comp_enabled) */
        return some$(Opt$Ptr$u8, addr);
    }

    let result = self.vt->alloc(self.ptr, len, align);
#if !COMP_TIME || (COMP_TIME && !debug_comp_enabled)
#else  /* COMP_TIME && (!COMP_TIME || debug_comp_enabled) */
    if_some(result, addr) { mem_Tracker_registerAlloc(addr, len, src_loc); }
#endif /* COMP_TIME && (!COMP_TIME || debug_comp_enabled) */
    return result;
};

fn_((
#if !COMP_TIME || (COMP_TIME && !debug_comp_enabled)
    mem_Allocator_rawResize(mem_Allocator self, Sli$u8 buf, u32 buf_align, usize new_len)
#else /* COMP_TIME && (!COMP_TIME || debug_comp_enabled) */
    mem_Allocator_rawResize_debug(mem_Allocator self, Sli$u8 buf, u32 buf_align, usize new_len, SrcLoc src_loc)
#endif /* COMP_TIME && (!COMP_TIME || debug_comp_enabled) */
)(bool)) {
    debug_assert_nonnull(self.vt);
    debug_assert_nonnull(self.vt->resize);
    debug_assert_fmt(mem_isValidAlign(buf_align), "Alignment must be a power of 2: %u", buf_align);

    // Special case for zero-sized allocations
    if (new_len == 0) {
        mem_Allocator_rawFree(self, buf, buf_align);
#if !COMP_TIME || (COMP_TIME && !debug_comp_enabled)
#else  /* COMP_TIME && (!COMP_TIME || debug_comp_enabled) */
        mem_Tracker_registerRemap(buf.ptr, buf.ptr, new_len, src_loc);
#endif /* COMP_TIME && (!COMP_TIME || debug_comp_enabled) */
        return true;
    }

    // Special case for empty buffer
    if (buf.len == 0) {
        return false;
    }

    let result = self.vt->resize(self.ptr, buf, buf_align, new_len);
#if !COMP_TIME || (COMP_TIME && !debug_comp_enabled)
#else  /* COMP_TIME && (!COMP_TIME || debug_comp_enabled) */
    if (result) { mem_Tracker_registerRemap(buf.ptr, buf.ptr, new_len, src_loc); }
#endif /* COMP_TIME && (!COMP_TIME || debug_comp_enabled) */
    return result;
}

fn_((
#if !COMP_TIME || (COMP_TIME && !debug_comp_enabled)
    mem_Allocator_rawRemap(mem_Allocator self, Sli$u8 buf, u32 buf_align, usize new_len)
#else /* COMP_TIME && (!COMP_TIME || debug_comp_enabled) */
    mem_Allocator_rawRemap_debug(mem_Allocator self, Sli$u8 buf, u32 buf_align, usize new_len, SrcLoc src_loc)
#endif /* COMP_TIME && (!COMP_TIME || debug_comp_enabled) */
)(Opt$Ptr$u8)) {
    debug_assert_nonnull(self.vt);
    debug_assert_nonnull(self.vt->remap);
    debug_assert_fmt(mem_isValidAlign(buf_align), "Alignment must be a power of 2: %u", buf_align);

    // Special case for zero-sized allocations
    if (new_len == 0) {
        mem_Allocator_rawFree(self, buf, buf_align);
        let addr = intToRawptr$(u8*, usize_limit_max & ~(buf_align - 1));
#if !COMP_TIME || (COMP_TIME && !debug_comp_enabled)
#else  /* COMP_TIME && (!COMP_TIME || debug_comp_enabled) */
        mem_Tracker_registerRemap(buf.ptr, addr, new_len, src_loc);
#endif /* COMP_TIME && (!COMP_TIME || debug_comp_enabled) */
        return some$(Opt$Ptr$u8, addr);
    }

    // Special case for empty buffer
    if (buf.len == 0) {
        return none$(Opt$Ptr$u8);
    }

    let result = self.vt->remap(self.ptr, buf, buf_align, new_len);
#if !COMP_TIME || (COMP_TIME && !debug_comp_enabled)
#else  /* COMP_TIME && (!COMP_TIME || debug_comp_enabled) */
    if_some(result, addr) { mem_Tracker_registerRemap(buf.ptr, addr, new_len, src_loc); }
#endif /* COMP_TIME && (!COMP_TIME || debug_comp_enabled) */
    return result;
}

fn_((
#if !COMP_TIME || (COMP_TIME && !debug_comp_enabled)
    mem_Allocator_rawFree(mem_Allocator self, Sli$u8 buf, u32 buf_align)
#else /* COMP_TIME && (!COMP_TIME || debug_comp_enabled) */
    mem_Allocator_rawFree_debug(mem_Allocator self, Sli$u8 buf, u32 buf_align, SrcLoc src_loc)
#endif /* COMP_TIME && (!COMP_TIME || debug_comp_enabled) */
)(void)) {
    debug_assert_nonnull(self.vt);
    debug_assert_nonnull(self.vt->free);
    debug_assert_fmt(mem_isValidAlign(buf_align), "Alignment must be a power of 2: %u", buf_align);

    // Special case for zero-sized allocations
    if (buf.len == 0) { return; }

    // Set memory to undefined before freeing
    bti_memset(buf.ptr, 0xAA, buf.len);

#if !COMP_TIME || (COMP_TIME && !debug_comp_enabled)
#else  /* COMP_TIME && (!COMP_TIME || debug_comp_enabled) */
    mem_Tracker_registerFree(buf.ptr, src_loc);
#endif /* COMP_TIME && (!COMP_TIME || debug_comp_enabled) */
    self.vt->free(self.ptr, buf, buf_align);
}

/*========== High-level Allocator Functions =================================*/

#if !COMP_TIME || (COMP_TIME && !debug_comp_enabled)

fn_((mem_Allocator_create(mem_Allocator self, TypeInfo type))(mem_Err$meta_Ptr) $scope) {
    // Special case for zero-sized types
    if (type.size == 0) {
        return_ok({
            .type = type,
            .addr = intToRawptr$(anyptr, usize_limit_max & ~(type.align - 1)),
        });
    }

    let mem_opt = mem_Allocator_rawAlloc(self, type.size, type.align);
    if_none(mem_opt) {
        return_err(mem_Err_OutOfMemory());
    }

    // Initialize memory to undefined pattern
    bti_memset(mem_opt.value, 0xAA, type.size);

    return_ok({
        .type = type,
        .addr = mem_opt.value,
    });
} $unscoped_(fn);

fn_((mem_Allocator_destroy(mem_Allocator self, AnyType ptr))(void)) {
    let info = variant_extract(ptr, AnyType_ptr);

    // Special case for zero-sized types
    if (info.size == 0) {
        return;
    }

    // Convert to slice for freeing
    Sli$u8 mem = {
        .ptr = as$((u8*)(info.addr)),
        .len = info.size,
    };

    mem_Allocator_rawFree(self, mem, info.align);
}

fn_((mem_Allocator_alloc(mem_Allocator self, TypeInfo type, usize count))(mem_Err$meta_Sli) $scope) {
    // Special case for zero-sized types or zero count
    if (type.size == 0 || count == 0) {
        return_ok({
            .type = type,
            .addr = intToRawptr$(anyptr, usize_limit_max & ~(type.align - 1)),
            .len  = count,
        });
    }

    // Check for overflow in multiplication
    let byte_count = usize_mulChkd(type.size, count);
    if_none(byte_count) {
        return_err(mem_Err_OutOfMemory());
    }

    let mem_opt = mem_Allocator_rawAlloc(self, byte_count.value, type.align);
    if_none(mem_opt) {
        return_err(mem_Err_OutOfMemory());
    }

    // Initialize memory to undefined pattern
    bti_memset(mem_opt.value, 0xAA, byte_count.value);

    return_ok({
        .type = type,
        .addr = mem_opt.value,
        .len  = count,
    });
} $unscoped_(fn);

fn_((mem_Allocator_resize(mem_Allocator self, AnyType old_mem, usize new_len))(bool)) {
    let info = variant_extract(old_mem, AnyType_sli);

    // Special case for zero-sized types
    if (info.size == 0) {
        return true;
    }

    // Special case for zero new length
    if (new_len == 0) {
        mem_Allocator_free(self, old_mem);
        return true;
    }

    // Special case for empty old memory
    if (info.len == 0) {
        return false;
    }

    // Create byte slice from the old memory
    Sli$u8 old_bytes = {
        .ptr = info.addr,
        .len = info.size * info.len,
    };

    // Check for overflow in multiplication
    let new_byte_count = usize_mulChkd(info.size, new_len);
    if_none(new_byte_count) {
        return false;
    }

    return mem_Allocator_rawResize(self, old_bytes, info.align, new_byte_count.value);
}

fn_((mem_Allocator_remap(mem_Allocator self, AnyType old_mem, usize new_len))(Opt$meta_Sli) $scope) {
    let info = variant_extract(old_mem, AnyType_sli);

    // Special case for zero-sized types
    if (info.size == 0) {
        return_some({
            .type = info.type,
            .addr = intToRawptr$(anyptr, usize_limit_max & ~(info.align - 1)),
            .len  = new_len,
        });
    }

    // Special case for zero new length
    if (new_len == 0) {
        mem_Allocator_free(self, old_mem);

        return_some({
            .type = info.type,
            .addr = intToRawptr$(anyptr, usize_limit_max & ~(info.align - 1)),
            .len  = 0,
        });
    }

    // Special case for empty old memory
    if (info.len == 0) {
        return_none();
    }

    // Create byte slice from the old memory
    Sli$u8 old_bytes = {
        .ptr = as$((u8*)(info.addr)),
        .len = info.size * info.len,
    };

    // Check for overflow in multiplication
    let new_byte_count = usize_mulChkd(info.size, new_len);
    if_none(new_byte_count) {
        return_none();
    }

    let new_ptr = mem_Allocator_rawRemap(self, old_bytes, info.align, new_byte_count.value);
    if_none(new_ptr) {
        return_none();
    }
    return_some({
        .type = info.type,
        .addr = new_ptr.value,
        .len  = new_len,
    });
} $unscoped_(fn);

fn_((mem_Allocator_realloc(mem_Allocator self, AnyType old_mem, usize new_len))(mem_Err$meta_Sli) $scope) {
    let info = variant_extract(old_mem, AnyType_sli);

    // Special case for empty old memory
    if (info.len == 0) {
        // This is equivalent to a new allocation
        return_ok(try_(mem_Allocator_alloc(self, info.type, new_len)));
    }

    // Special case for zero new length
    if (new_len == 0) {
        mem_Allocator_free(self, old_mem);
        return_ok({
            .type = info.type,
            .addr = intToRawptr$(anyptr, usize_limit_max & ~(info.align - 1)),
            .len  = 0,
        });
    }

    // Special case for zero-sized types
    if (info.size == 0) {
        return_ok({
            .type = info.type,
            .addr = intToRawptr$(anyptr, usize_limit_max & ~(info.align - 1)),
            .len  = new_len,
        });
    }

    // Create byte slice from the old memory
    Sli$u8 old_bytes = {
        .ptr = as$((u8*)(info.addr)),
        .len = info.size * info.len,
    };

    // Check for overflow in multiplication
    let new_byte_count = usize_mulChkd(info.size, new_len);
    if_none(new_byte_count) {
        return_err(mem_Err_OutOfMemory());
    }

    // Try to remap first (which may be in-place resize or may relocate)
    let new_ptr = mem_Allocator_rawRemap(self, old_bytes, info.align, new_byte_count.value);
    if_some(new_ptr, p) {
        return_ok({
            .type = info.type,
            .addr = p,
            .len  = new_len,
        });
    }

    // Remap failed, need to allocate new memory and copy
    let new_mem = mem_Allocator_rawAlloc(self, new_byte_count.value, info.align);
    if_none(new_mem) {
        return_err(mem_Err_OutOfMemory());
    }

    // Copy the data from old memory to new memory (use smaller of the two sizes)
    let copy_len = prim_min(old_bytes.len, new_byte_count.value);
    mem_copy(new_mem.value, old_bytes.ptr, copy_len);

    // Zero out old memory before freeing
    bti_memset(old_bytes.ptr, 0xAA, old_bytes.len);
    mem_Allocator_rawFree(self, old_bytes, info.align);

    return_ok({
        .type = info.type,
        .addr = new_mem.value,
        .len  = new_len,
    });
} $unscoped_(fn);

fn_((mem_Allocator_free(mem_Allocator self, AnyType memory))(void)) {
    let info = variant_extract(memory, AnyType_sli);

    // Special case for zero-sized types or empty slices
    if (info.size == 0 || info.len == 0) {
        return;
    }

    // Create byte slice from memory and free it
    Sli$u8 bytes = {
        .ptr = as$((u8*)(info.addr)),
        .len = info.size * info.len,
    };

    mem_Allocator_rawFree(self, bytes, info.align);
}

/*========== Helper Functions ===============================================*/

fn_((mem_Allocator_dupe(mem_Allocator self, meta_Sli src))(mem_Err$meta_Sli) $scope) {
    // Allocate new memory with same element type and count
    let new_mem = try_(mem_Allocator_alloc(self, src.type, src.len));

    // Copy data from source to new memory
    let src_bytes = Sli_from$(Sli$u8, as$((u8*)(src.addr)), src.type.size * src.len);
    let dst_bytes = Sli_from$(Sli$u8, as$((u8*)(new_mem.addr)), src.type.size * src.len);
    mem_copy(dst_bytes.ptr, src_bytes.ptr, dst_bytes.len);

    return_ok(new_mem);
} $unscoped_(fn);

fn_((mem_Allocator_dupeZ(mem_Allocator self, meta_Sli src))(mem_Err$meta_Sli) $scope) {
    // Allocate new memory with same element type but one extra element for sentinel
    let new_mem = try_(mem_Allocator_alloc(self, src.type, src.len + 1));

    // Copy data from source to new memory
    let src_bytes = Sli_from$(Sli$u8, as$((u8*)(src.addr)), src.type.size * src.len);
    let dst_bytes = Sli_from$(Sli$u8, as$((u8*)(new_mem.addr)), src.type.size * src.len);
    mem_copy(dst_bytes.ptr, src_bytes.ptr, dst_bytes.len);
    mem_set(dst_bytes.ptr, 0, dst_bytes.len);

    // // Set sentinel value at end
    // if (src.type.size == 1) {
    //     // For byte-sized elements, directly set the sentinel
    //     as$((u8*)(new_mem.addr))[src.len] = 0;
    // } else {
    //     // For larger elements, we'd need to know the exact type to properly set the sentinel
    //     // This simple implementation assumes sentinel is just the first byte of the element
    //     bti_memset(as$((u8*)(new_mem.addr)) + (src.len * src.type.size), 0, 1);
    // }

    return_ok({
        .type = new_mem.type,
        .addr = new_mem.addr,
        .len = src.len,  // Note: we preserve original length, sentinel is separate
    });
} $unscoped_(fn);

#else /* COMP_TIME && (!COMP_TIME || debug_comp_enabled) */

/*========== Debug Versions of Functions ====================================*/

fn_((mem_Allocator_create_debug(mem_Allocator self, TypeInfo type, SrcLoc src_loc))(mem_Err$meta_Ptr) $scope) {
    // Special case for zero-sized types
    if (type.size == 0) {
        return_ok({
            .type = type,
            .addr = intToRawptr$(anyptr, usize_limit_max & ~(type.align - 1)),
        });
    }

    let mem_opt = mem_Allocator_rawAlloc_debug(self, type.size, type.align, src_loc);
    if_none(mem_opt) {
        return_err(mem_Err_OutOfMemory());
    }

    // Initialize memory to undefined pattern
    bti_memset(mem_opt.value, 0xAA, type.size);

    return_ok({
        .type = type,
        .addr = mem_opt.value,
    });
} $unscoped_(fn);

fn_((mem_Allocator_destroy_debug(mem_Allocator self, AnyType ptr, SrcLoc src_loc))(void)) {
    let info = variant_extract(ptr, AnyType_ptr);

    // Special case for zero-sized types
    if (info.size == 0) {
        return;
    }

    // Convert to slice for freeing
    Sli$u8 mem = {
        .ptr = as$((u8*)(info.addr)),
        .len = info.size,
    };

    mem_Allocator_rawFree_debug(self, mem, info.align, src_loc);
}

fn_((mem_Allocator_alloc_debug(mem_Allocator self, TypeInfo type, usize count, SrcLoc src_loc))(mem_Err$meta_Sli) $scope) {
    // Special case for zero-sized types or zero count
    if (type.size == 0 || count == 0) {
        return_ok({
            .type = type,
            .addr = intToRawptr$(anyptr, usize_limit_max & ~(type.align - 1)),
            .len  = count,
        });
    }

    // Check for overflow in multiplication
    let byte_count = usize_mulChkd(type.size, count);
    if_none(byte_count) {
        return_err(mem_Err_OutOfMemory());
    }

    let mem_opt = mem_Allocator_rawAlloc_debug(self, byte_count.value, type.align, src_loc);
    if_none(mem_opt) {
        return_err(mem_Err_OutOfMemory());
    }

    // Initialize memory to undefined pattern
    bti_memset(mem_opt.value, 0xAA, byte_count.value);

    return_ok({
        .type = type,
        .addr = mem_opt.value,
        .len  = count,
    });
} $unscoped_(fn);

fn_((mem_Allocator_resize_debug(mem_Allocator self, AnyType old_mem, usize new_len, SrcLoc src_loc))(bool)) {
    let info = variant_extract(old_mem, AnyType_sli);

    // Special case for zero-sized types
    if (info.size == 0) {
        return true;
    }

    // Special case for zero new length
    if (new_len == 0) {
        mem_Allocator_free_debug(self, old_mem, src_loc);
        return true;
    }
    // Special case for empty old memory
    if (info.len == 0) {
        return false;
    }

    // Create byte slice from the old memory
    Sli$u8 old_bytes = {
        .ptr = info.addr,
        .len = info.size * info.len,
    };

    // Check for overflow in multiplication
    let new_byte_count = usize_mulChkd(info.size, new_len);
    if_none(new_byte_count) {
        return false;
    }

    return mem_Allocator_rawResize_debug(self, old_bytes, info.align, new_byte_count.value, src_loc);
}

fn_((mem_Allocator_remap_debug(mem_Allocator self, AnyType old_mem, usize new_len, SrcLoc src_loc))(Opt$meta_Sli) $scope) {
    let info = variant_extract(old_mem, AnyType_sli);

    // Special case for zero-sized types
    if (info.size == 0) {
        return_some({
            .type = info.type,
            .addr = intToRawptr$(anyptr, usize_limit_max & ~(info.align - 1)),
            .len  = new_len,
        });
    }

    // Special case for zero new length
    if (new_len == 0) {
        mem_Allocator_free_debug(self, old_mem, src_loc);
        return_some({
            .type = info.type,
            .addr = intToRawptr$(anyptr, usize_limit_max & ~(info.align - 1)),
            .len  = 0,
        });
    }

    // Special case for empty old memory
    if (info.len == 0) {
        return_none();
    }

    // Create byte slice from the old memory
    Sli$u8 old_bytes = {
        .ptr = as$((u8*)(info.addr)),
        .len = info.size * info.len,
    };

    // Check for overflow in multiplication
    let new_byte_count = usize_mulChkd(info.size, new_len);
    if_none(new_byte_count) {
        return_none();
    }

    let new_ptr = mem_Allocator_rawRemap_debug(self, old_bytes, info.align, new_byte_count.value, src_loc);
    if_none(new_ptr) { return_none(); }

    return_some({
        .type = info.type,
        .addr = new_ptr.value,
        .len = new_len,
    });
} $unscoped_(fn);

fn_((mem_Allocator_realloc_debug(mem_Allocator self, AnyType old_mem, usize new_len, SrcLoc src_loc))(mem_Err$meta_Sli) $scope) {
    let info = variant_extract(old_mem, AnyType_sli);

    // Special case for empty old memory
    if (info.len == 0) {
        // This is equivalent to a new allocation
        return_ok(try_(mem_Allocator_alloc_debug(self, info.type, new_len, src_loc)));
    }

    // Special case for zero new length
    if (new_len == 0) {
        mem_Allocator_free_debug(self, old_mem, src_loc);
        return_ok({
            .type = info.type,
            .addr = intToRawptr$(anyptr, usize_limit_max & ~(info.align - 1)),
            .len  = 0,
        });
    }

    // Special case for zero-sized types
    if (info.size == 0) {
        return_ok({
            .type = info.type,
            .addr = intToRawptr$(anyptr, usize_limit_max & ~(info.align - 1)),
            .len  = new_len,
        });
    }

    // Create byte slice from the old memory
    Sli$u8 old_bytes = {
        .ptr = as$((u8*)(info.addr)),
        .len = info.size * info.len,
    };

    // Check for overflow in multiplication
    let new_byte_count = usize_mulChkd(info.size, new_len);
    if_none(new_byte_count) {
        return_err(mem_Err_OutOfMemory());
    }

    // Try to remap first (which may be in-place resize or may relocate)
    let new_ptr = mem_Allocator_rawRemap_debug(self, old_bytes, info.align, new_byte_count.value, src_loc);
    if_some(new_ptr, p) {
        return_ok({
            .type = info.type,
            .addr = p,
            .len  = new_len,
        });
    }

    // Remap failed, need to allocate new memory and copy
    let new_mem = mem_Allocator_rawAlloc_debug(self, new_byte_count.value, info.align, src_loc);
    if_none(new_mem) {
        return_err(mem_Err_OutOfMemory());
    }

    // Copy the data from old memory to new memory (use smaller of the two sizes)
    let copy_len = prim_min(old_bytes.len, new_byte_count.value);
    mem_copy(new_mem.value, old_bytes.ptr, copy_len);

    // Zero out old memory before freeing
    bti_memset(old_bytes.ptr, 0xAA, old_bytes.len);
    mem_Allocator_rawFree_debug(self, old_bytes, info.align, src_loc);

    return_ok({
        .type = info.type,
        .addr = new_mem.value,
        .len  = new_len,
    });
} $unscoped_(fn);

fn_((mem_Allocator_free_debug(mem_Allocator self, AnyType mem, SrcLoc src_loc))(void)) {
    let info = variant_extract(mem, AnyType_sli);

    // Special case for zero-sized types or empty slices
    if (info.size == 0 || info.len == 0) {
        return;
    }

    // Create byte slice from memory and free it
    Sli$u8 bytes = {
        .ptr = as$((u8*)(info.addr)),
        .len = info.size * info.len,
    };

    mem_Allocator_rawFree_debug(self, bytes, info.align, src_loc);
}

fn_((mem_Allocator_dupe_debug(mem_Allocator self, meta_Sli src, SrcLoc src_loc))(mem_Err$meta_Sli) $scope) {
    // Allocate new memory with same element type and count
    let new_mem = try_(mem_Allocator_alloc_debug(self, src.type, src.len, src_loc));

    // Copy data from source to new memory
    let src_bytes = Sli_from$(Sli$u8, as$((u8*)(src.addr)), src.type.size * src.len);
    let dst_bytes = Sli_from$(Sli$u8, as$((u8*)(new_mem.addr)), src.type.size * src.len);
    mem_copy(dst_bytes.ptr, src_bytes.ptr, dst_bytes.len);

    return_ok(new_mem);
} $unscoped_(fn);

fn_((mem_Allocator_dupeZ_debug(mem_Allocator self, meta_Sli src, SrcLoc src_loc))(mem_Err$meta_Sli) $scope) {
    // Allocate new memory with same element type but one extra element for sentinel
    let new_mem = try_(mem_Allocator_alloc_debug(self, src.type, src.len + 1, src_loc));

    // Copy data from source to new memory
    let src_bytes = Sli_from$(Sli$u8, as$((u8*)(src.addr)), src.type.size * src.len);
    let dst_bytes = Sli_from$(Sli$u8, as$((u8*)(new_mem.addr)), src.type.size * src.len);
    mem_copy(dst_bytes.ptr, src_bytes.ptr, dst_bytes.len);
    mem_set(dst_bytes.ptr, 0, dst_bytes.len);

    // // Set sentinel value at end
    // if (src.type.size == 1) {
    //     // For byte-sized elements, directly set the sentinel
    //     as$((u8*)(new_mem.addr))[src.len] = 0;
    // } else {
    //     // For larger elements, we'd need to know the exact type to properly set the sentinel
    //     // This simple implementation assumes sentinel is just the first byte of the element
    //       bti_memset(as$((u8*)(new_mem.addr)) + (src.len * src.type.size), 0, 1);
    // }

    return_ok({
        .type = new_mem.type,
        .addr = new_mem.addr,
        .len = src.len,  // Note: we preserve original length, sentinel is separate
    });
} $unscoped_(fn);

#endif /* COMP_TIME && (!COMP_TIME || debug_comp_enabled) */

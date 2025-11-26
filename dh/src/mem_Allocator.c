#include "dh/mem/Allocator.h"
#include "dh/mem/common.h"
#include "dh/mem/Tracker.h"

/*========== Common VTable Functions ========================================*/

fn_((mem_Allocator_VT_noAlloc(P$raw ctx, usize len, u8 align))(O$P$u8)) {
    let_ignore = ctx;
    let_ignore = len;
    let_ignore = align;
    return none$((O$P$u8));
}

fn_((mem_Allocator_VT_noResize(P$raw ctx, S$u8 buf, u8 buf_align, usize new_len))(bool)) {
    let_ignore = ctx;
    let_ignore = buf;
    let_ignore = buf_align;
    let_ignore = new_len;
    return false;
}

fn_((mem_Allocator_VT_noRemap(P$raw ctx, S$u8 buf, u8 buf_align, usize new_len))(O$P$u8)) {
    let_ignore = ctx;
    let_ignore = buf;
    let_ignore = buf_align;
    let_ignore = new_len;
    return none$((O$P$u8));
}

fn_((mem_Allocator_VT_noFree(P$raw ctx, S$u8 buf, u8 buf_align))(void)) {
    let_ignore = ctx;
    let_ignore = buf;
    let_ignore = buf_align;
}

/*========== Raw Allocation Functions =======================================*/

fn_((
#if !on_comptime || (on_comptime && !debug_comp_enabled)
    mem_Allocator_rawAlloc(mem_Allocator self, usize len, u8 align)
#else /* on_comptime && (!on_comptime || debug_comp_enabled) */
    mem_Allocator_rawAlloc_debug(mem_Allocator self, usize len, u8 align, SrcLoc src_loc)
#endif /* on_comptime && (!on_comptime || debug_comp_enabled) */
)(O$P$u8)) {
    debug_assert_nonnull(self.vt);
    debug_assert_nonnull(self.vt->alloc);
    // Special case for zero-sized allocations
    if (len == 0) {
        // For zero-sized allocations, return a non-null pointer at max address
        // aligned to the requested alignment
        let addr = intToPtr$(u8*, usize_limit_max & ~((1ull << align) - 1));
#if !on_comptime || (on_comptime && !debug_comp_enabled)
#else  /* on_comptime && (!on_comptime || debug_comp_enabled) */
        mem_Tracker_registerAlloc(addr, len, src_loc);
#endif /* on_comptime && (!on_comptime || debug_comp_enabled) */
        return some$((O$P$u8)(addr));
    }
    let result = self.vt->alloc(self.ctx, len, align);
#if !on_comptime || (on_comptime && !debug_comp_enabled)
#else  /* on_comptime && (!on_comptime || debug_comp_enabled) */
    if_some((result)(addr)) { mem_Tracker_registerAlloc(addr, len, src_loc); }
#endif /* on_comptime && (!on_comptime || debug_comp_enabled) */
    return result;
};

fn_((
#if !on_comptime || (on_comptime && !debug_comp_enabled)
    mem_Allocator_rawResize(mem_Allocator self, S$u8 buf, u8 buf_align, usize new_len)
#else /* on_comptime && (!on_comptime || debug_comp_enabled) */
    mem_Allocator_rawResize_debug(mem_Allocator self, S$u8 buf, u8 buf_align, usize new_len, SrcLoc src_loc)
#endif /* on_comptime && (!on_comptime || debug_comp_enabled) */
)(bool)) {
    debug_assert_nonnull(self.vt);
    debug_assert_nonnull(self.vt->resize);
    // Special case for zero-sized allocations
    if (new_len == 0) {
        mem_Allocator_rawFree(self, buf, buf_align);
#if !on_comptime || (on_comptime && !debug_comp_enabled)
#else  /* on_comptime && (!on_comptime || debug_comp_enabled) */
        mem_Tracker_registerRemap(buf.ptr, buf.ptr, new_len, src_loc);
#endif /* on_comptime && (!on_comptime || debug_comp_enabled) */
        return true;
    }
    // Special case for empty buffer
    if (buf.len == 0) {
        return false;
    }
    let result = self.vt->resize(self.ctx, buf, buf_align, new_len);
#if !on_comptime || (on_comptime && !debug_comp_enabled)
#else  /* on_comptime && (!on_comptime || debug_comp_enabled) */
    if (result) { mem_Tracker_registerRemap(buf.ptr, buf.ptr, new_len, src_loc); }
#endif /* on_comptime && (!on_comptime || debug_comp_enabled) */
    return result;
}

fn_((
#if !on_comptime || (on_comptime && !debug_comp_enabled)
    mem_Allocator_rawRemap(mem_Allocator self, S$u8 buf, u8 buf_align, usize new_len)
#else /* on_comptime && (!on_comptime || debug_comp_enabled) */
    mem_Allocator_rawRemap_debug(mem_Allocator self, S$u8 buf, u8 buf_align, usize new_len, SrcLoc src_loc)
#endif /* on_comptime && (!on_comptime || debug_comp_enabled) */
)(O$P$u8)) {
    debug_assert_nonnull(self.vt);
    debug_assert_nonnull(self.vt->remap);
    // Special case for zero-sized allocations
    if (new_len == 0) {
        mem_Allocator_rawFree(self, buf, buf_align);
        let addr = intToPtr$(u8*, usize_limit_max & ~((1ull << buf_align) - 1));
#if !on_comptime || (on_comptime && !debug_comp_enabled)
#else  /* on_comptime && (!on_comptime || debug_comp_enabled) */
        mem_Tracker_registerRemap(buf.ptr, addr, new_len, src_loc);
#endif /* on_comptime && (!on_comptime || debug_comp_enabled) */
        return some$((O$P$u8)(addr));
    }
    // Special case for empty buffer
    if (buf.len == 0) {
        return none$((O$P$u8));
    }
    let result = self.vt->remap(self.ctx, buf, buf_align, new_len);
#if !on_comptime || (on_comptime && !debug_comp_enabled)
#else  /* on_comptime && (!on_comptime || debug_comp_enabled) */
    if_some((result)(addr)) { mem_Tracker_registerRemap(buf.ptr, addr, new_len, src_loc); }
#endif /* on_comptime && (!on_comptime || debug_comp_enabled) */
    return result;
}

fn_((
#if !on_comptime || (on_comptime && !debug_comp_enabled)
    mem_Allocator_rawFree(mem_Allocator self, S$u8 buf, u8 buf_align)
#else /* on_comptime && (!on_comptime || debug_comp_enabled) */
    mem_Allocator_rawFree_debug(mem_Allocator self, S$u8 buf, u8 buf_align, SrcLoc src_loc)
#endif /* on_comptime && (!on_comptime || debug_comp_enabled) */
)(void)) {
    debug_assert_nonnull(self.vt);
    debug_assert_nonnull(self.vt->free);
    // Special case for zero-sized allocations
    if (buf.len == 0) { return; }
    // Set memory to undefined before freeing
    mem_setBytes0(buf);
#if !on_comptime || (on_comptime && !debug_comp_enabled)
#else  /* on_comptime && (!on_comptime || debug_comp_enabled) */
    mem_Tracker_registerFree(buf.ptr, src_loc);
#endif /* on_comptime && (!on_comptime || debug_comp_enabled) */
    self.vt->free(self.ctx, buf, buf_align);
}

/*========== High-level Allocator Functions =================================*/

#if !on_comptime || (on_comptime && !debug_comp_enabled)

fn_((mem_Allocator_create(mem_Allocator self, TypeInfo type))(mem_Err$u_P$raw) $scope) {
    // Special case for zero-sized types
    if (type.size == 0) {
        return_ok({
            .raw = intToPtr$(P$raw, usize_limit_max & ~((1ull << type.align) - 1)),
            .type = type,
        });
    }
    let mem = orelse_((mem_Allocator_rawAlloc(self, type.size, type.align))(
        return_err(mem_Err_OutOfMemory()) ));
    // Initialize memory to undefined pattern
    mem_setBytes0(init$S$((u8)(mem, type.size)));
    return_ok({
        .raw = mem,
        .type = type,
    });
} $unscoped_(fn);

fn_((mem_Allocator_destroy(mem_Allocator self, u_P$raw ptr))(void)) {
    // Special case for zero-sized types
    if (ptr.type.size == 0) { return; }
    // Convert to slice for freeing
    S$u8 mem = {
        .ptr = as$(u8*)(ptr.raw),
        .len = ptr.type.size,
    };
    mem_Allocator_rawFree(self, mem, ptr.type.align);
}

fn_((mem_Allocator_alloc(mem_Allocator self, TypeInfo type, usize count))(mem_Err$u_S$raw) $scope) {
    // Special case for zero-sized types or zero count
    if (type.size == 0 || count == 0) {
        return_ok({
            .ptr = intToPtr$(P$raw, usize_limit_max & ~((1ull << type.align) - 1)),
            .len = count,
            .type = type,
        });
    }
    // Check for overflow in multiplication
    let byte_count = orelse_((usize_mulChkd(type.size, count))(
        return_err(mem_Err_OutOfMemory()) ));
    let mem = orelse_((mem_Allocator_rawAlloc(self, byte_count, type.align))(
        return_err(mem_Err_OutOfMemory()) ));
    mem_setBytes0(init$S$((u8)(mem, byte_count)));
    return_ok({
        .ptr = mem,
        .len = count,
        .type = type,
    });
} $unscoped_(fn);

fn_((mem_Allocator_resize(mem_Allocator self, u_S$raw old_mem, usize new_len))(bool)) {
    // Special case for zero-sized types
    if (old_mem.type.size == 0) {
        return true;
    }
    // Special case for zero new length
    if (new_len == 0) {
        mem_Allocator_free(self, old_mem);
        return true;
    }
    // Special case for empty old memory
    if (old_mem.len == 0) {
        return false;
    }
    // Create byte slice from the old memory
    S$u8 old_bytes = {
        .ptr = old_mem.ptr,
        .len = old_mem.type.size * old_mem.len,
    };
    // Check for overflow in multiplication
    let new_byte_count = orelse_((usize_mulChkd(old_mem.type.size, new_len))(
        return false;
    ));
    return mem_Allocator_rawResize(self, old_bytes, old_mem.type.align, new_byte_count);
}

fn_((mem_Allocator_remap(mem_Allocator self, u_S$raw old_mem, usize new_len))(O$u_S$raw) $scope) {
    // Special case for zero-sized types
    if (old_mem.type.size == 0) {
        return_some({
            .ptr = intToPtr$(P$raw, usize_limit_max & ~((1ull << old_mem.type.align) - 1)),
            .len = new_len,
            .type = old_mem.type,
        });
    }
    // Special case for zero new length
    if (new_len == 0) {
        mem_Allocator_free(self, old_mem);
        return_some({
            .ptr = intToPtr$(P$raw, usize_limit_max & ~((1ull << old_mem.type.align) - 1)),
            .len = 0,
            .type = old_mem.type,
        });
    }
    // Special case for empty old memory
    if (old_mem.len == 0) {
        return_none();
    }
    // Create byte slice from the old memory
    S$u8 old_bytes = {
        .ptr = as$( u8*)(old_mem.ptr),
        .len = old_mem.type.size * old_mem.len,
    };
    // Check for overflow in multiplication
    let new_byte_count = orelse_((usize_mulChkd(old_mem.type.size, new_len))(
        return_none();));
    let new_ptr = orelse_((mem_Allocator_rawRemap(self, old_bytes, old_mem.type.align, new_byte_count))(
        return_none();));
    return_some({
        .ptr = new_ptr,
        .len = new_len,
        .type = old_mem.type,
    });
} $unscoped_(fn);

fn_((mem_Allocator_realloc(mem_Allocator self, u_S$raw old_mem, usize new_len))(mem_Err$u_S$raw) $scope) {
    // Special case for empty old memory
    if (old_mem.len == 0) {
        // This is equivalent to a new allocation
        return_ok(try_(mem_Allocator_alloc(self, old_mem.type, new_len)));
    }
    // Special case for zero new length
    if (new_len == 0) {
        mem_Allocator_free(self, old_mem);
        return_ok({
            .ptr = intToPtr$(P$raw, usize_limit_max & ~((1ull << old_mem.type.align) - 1)),
            .len = 0,
            .type = old_mem.type,
        });
    }
    // Special case for zero-sized types
    if (old_mem.type.size == 0) {
        return_ok({
            .ptr = intToPtr$(P$raw, usize_limit_max & ~(old_mem.type.align - 1)),
            .len = new_len,
            .type = old_mem.type,
        });
    }
    // Create byte slice from the old memory
    S$u8 old_bytes = {
        .ptr = as$( u8*)(old_mem.ptr),
        .len = old_mem.type.size * old_mem.len,
    };
    // Check for overflow in multiplication
    let new_byte_count = orelse_((usize_mulChkd(old_mem.type.size, new_len))(
        return_err(mem_Err_OutOfMemory());));
    // Try to remap first (which may be in-place resize or may relocate)
    let new_ptr = mem_Allocator_rawRemap(self, old_bytes, old_mem.type.align, new_byte_count);
    if_some((new_ptr)(ptr)) {
        return_ok({
            .ptr = ptr,
            .len = new_len,
            .type = old_mem.type,
        });
    }
    // Remap failed, need to allocate new memory and copy
    let new_mem = orelse_((mem_Allocator_rawAlloc(self, new_byte_count, old_mem.type.align))(
        return_err(mem_Err_OutOfMemory());));
    // Copy the data from old memory to new memory (use smaller of the two sizes)
    let copy_len = prim_min(old_bytes.len, new_byte_count);
    mem_copyBytes(init$S$((u8)(new_mem, copy_len)), old_bytes.as_const);
    // Zero out old memory before freeing
    mem_setBytes0(old_bytes);
    mem_Allocator_rawFree(self, old_bytes, old_mem.type.align);
    return_ok({
        .ptr = new_mem,
        .len = new_len,
        .type = old_mem.type,
    });
} $unscoped_(fn);

fn_((mem_Allocator_free(mem_Allocator self, u_S$raw mem))(void)) {
    // Special case for zero-sized types or empty slices
    if (mem.type.size == 0 || mem.len == 0) { return; }
    // Create byte slice from memory and free it
    S$u8 bytes = {
        .ptr = as$( u8*)(mem.ptr),
        .len = mem.type.size * mem.len,
    };
    mem_Allocator_rawFree(self, bytes, mem.type.align);
}

/*========== Helper Functions ===============================================*/

fn_((mem_Allocator_dupe(mem_Allocator self, u_S$raw src))(mem_Err$u_S$raw) $scope) {
    // Allocate new memory with same element type and count
    let new_mem = try_(mem_Allocator_alloc(self, src.type, src.len));
    // Copy data from source to new memory
    let src_bytes = init$S$((u8)(as$(u8*)(src.ptr), src.type.size * src.len));
    let dst_bytes = init$S$((u8)(as$(u8*)(new_mem.ptr), src.type.size * src.len));
    mem_copyBytes(dst_bytes, src_bytes.as_const);

    return_ok(new_mem);
} $unscoped_(fn);

fn_((mem_Allocator_dupeZ(mem_Allocator self, u_S$raw src))(mem_Err$u_S$raw) $scope) {
    // Allocate new memory with same element type but one extra element for sentinel
    let new_mem = try_(mem_Allocator_alloc(self, src.type, src.len + 1));
    // Copy data from source to new memory
    let src_bytes = init$S$((u8)(as$(u8*)(src.ptr), src.type.size * src.len));
    let dst_bytes = init$S$((u8)(as$(u8*)(new_mem.ptr), src.type.size * src.len));
    mem_copyBytes(dst_bytes, src_bytes.as_const);
    mem_setBytes0(dst_bytes);
    // // Set sentinel value at end
    // if (src.type.size == 1) {
    //     // For byte-sized elements, directly set the sentinel
    //     as$(u8*)(new_mem.addr)[src.len] = 0;
    // } else {
    //     // For larger elements, we'd need to know the exact type to properly set the sentinel
    //     // This simple implementation assumes sentinel is just the first byte of the element
    //     mem_set(as$(u8*)(new_mem.addr) + (src.len * src.type.size), 0, 1);
    // }
    return_ok({
        .ptr = new_mem.ptr,
        .len = src.len, // Note: we preserve original length, sentinel is separate
        .type = new_mem.type,
    });
} $unscoped_(fn);

#else /* on_comptime && (!on_comptime || debug_comp_enabled) */

/*========== Debug Versions of Functions ====================================*/

fn_((mem_Allocator_create_debug(mem_Allocator self, TypeInfo type, SrcLoc src_loc))(mem_Err$u_P$raw) $scope) {
    // Special case for zero-sized types
    if (type.size == 0) {
        return_ok({
            .raw = intToPtr$(P$raw, usize_limit_max & ~((1ull << type.align) - 1)),
            .type = type,
        });
    }
    let mem = orelse_((mem_Allocator_rawAlloc_debug(self, type.size, type.align, src_loc))(
        return_err(mem_Err_OutOfMemory());));
    // Initialize memory to undefined pattern
    mem_setBytes0(init$S$((u8)(mem, type.size)));
    return_ok({
        .raw = mem,
        .type = type,
    });
} $unscoped_(fn);

fn_((mem_Allocator_destroy_debug(mem_Allocator self, u_P$raw ptr, SrcLoc src_loc))(void)) {
    // Special case for zero-sized types
    if (ptr.type.size == 0) {
        return;
    }
    // Convert to slice for freeing
    S$u8 mem = {
        .ptr = as$(u8*)(ptr.raw),
        .len = ptr.type.size,
    };
    mem_Allocator_rawFree_debug(self, mem, ptr.type.align, src_loc);
}

fn_((mem_Allocator_alloc_debug(mem_Allocator self, TypeInfo type, usize count, SrcLoc src_loc))(mem_Err$u_S$raw) $scope) {
    // Special case for zero-sized types or zero count
    if (type.size == 0 || count == 0) {
        return_ok({
            .ptr = intToPtr$(P$raw, usize_limit_max & ~((1ull << type.align) - 1)),
            .len = count,
            .type = type,
        });
    }
    // Check for overflow in multiplication
    let byte_count = orelse_((usize_mulChkd(type.size, count))(
        return_err(mem_Err_OutOfMemory());));
    let mem = orelse_((mem_Allocator_rawAlloc_debug(self, byte_count, type.align, src_loc))(
        return_err(mem_Err_OutOfMemory());));
    // Initialize memory to undefined pattern
    mem_setBytes0(init$S$((u8)(mem, byte_count)));
    return_ok({
        .ptr = mem,
        .len = count,
        .type = type,
    });
} $unscoped_(fn);

fn_((mem_Allocator_resize_debug(mem_Allocator self, u_S$raw old_mem, usize new_len, SrcLoc src_loc))(bool)) {
    // Special case for zero-sized types
    if (old_mem.type.size == 0) {
        return true;
    }
    // Special case for zero new length
    if (new_len == 0) {
        mem_Allocator_free_debug(self, old_mem, src_loc);
        return true;
    }
    // Special case for empty old memory
    if (old_mem.len == 0) {
        return false;
    }
    // Create byte slice from the old memory
    S$u8 old_bytes = {
        .ptr = old_mem.ptr,
        .len = old_mem.type.size * old_mem.len,
    };
    // Check for overflow in multiplication
    let new_byte_count = orelse_((usize_mulChkd(old_mem.type.size, new_len))(
        return false;
    ));
    return mem_Allocator_rawResize_debug(self, old_bytes, old_mem.type.align, new_byte_count, src_loc);
}

fn_((mem_Allocator_remap_debug(mem_Allocator self, u_S$raw old_mem, usize new_len, SrcLoc src_loc))(O$u_S$raw) $scope) {
    // Special case for zero-sized types
    if (old_mem.type.size == 0) {
        return_some({
            .ptr = intToPtr$(P$raw, usize_limit_max & ~((1ull << old_mem.type.align) - 1)),
            .len = new_len,
            .type = old_mem.type,
        });
    }
    // Special case for zero new length
    if (new_len == 0) {
        mem_Allocator_free_debug(self, old_mem, src_loc);
        return_some({
            .ptr = intToPtr$(P$raw, usize_limit_max & ~((1ull << old_mem.type.align) - 1)),
            .len = 0,
            .type = old_mem.type,
        });
    }
    // Special case for empty old memory
    if (old_mem.len == 0) {
        return_none();
    }
    // Create byte slice from the old memory
    S$u8 old_bytes = {
        .ptr = as$(u8*)(old_mem.ptr),
        .len = old_mem.type.size * old_mem.len,
    };
    // Check for overflow in multiplication
    let new_byte_count = orelse_((usize_mulChkd(old_mem.type.size, new_len))(
        return_none();));
    let new_ptr = orelse_((mem_Allocator_rawRemap_debug(self, old_bytes, old_mem.type.align, new_byte_count, src_loc))(
        return_none();));
    return_some({
        .ptr = new_ptr,
        .len = new_len,
        .type = old_mem.type,
    });
} $unscoped_(fn);

fn_((mem_Allocator_realloc_debug(mem_Allocator self, u_S$raw old_mem, usize new_len, SrcLoc src_loc))(mem_Err$u_S$raw) $scope) {
    // Special case for empty old memory
    if (old_mem.len == 0) {
        // This is equivalent to a new allocation
        return_ok(try_(mem_Allocator_alloc_debug(self, old_mem.type, new_len, src_loc)));
    }
    // Special case for zero new length
    if (new_len == 0) {
        mem_Allocator_free_debug(self, old_mem, src_loc);
        return_ok({
            .ptr = intToPtr$(P$raw, usize_limit_max & ~((1ull << old_mem.type.align) - 1)),
            .len = 0,
            .type = old_mem.type,
        });
    }
    // Special case for zero-sized types
    if (old_mem.type.size == 0) {
        return_ok({
            .ptr = intToPtr$(P$raw, usize_limit_max & ~((1ull << old_mem.type.align) - 1)),
            .len = new_len,
            .type = old_mem.type,
        });
    }
    // Create byte slice from the old memory
    S$u8 old_bytes = {
        .ptr = as$(u8*)(old_mem.ptr),
        .len = old_mem.type.size * old_mem.len,
    };
    // Check for overflow in multiplication
    let new_byte_count = orelse_((usize_mulChkd(old_mem.type.size, new_len))(
        return_err(mem_Err_OutOfMemory());));
    // Try to remap first (which may be in-place resize or may relocate)
    let new_ptr = mem_Allocator_rawRemap(self, old_bytes, old_mem.type.align, new_byte_count);
    if_some((new_ptr)(ptr)) {
        return_ok({
            .ptr = ptr,
            .len = new_len,
            .type = old_mem.type,
        });
    }
    // Remap failed, need to allocate new memory and copy
    let new_mem = orelse_((mem_Allocator_rawAlloc_debug(self, new_byte_count, old_mem.type.align, src_loc))(
        return_err(mem_Err_OutOfMemory());));
    // Copy the data from old memory to new memory (use smaller of the two sizes)
    let copy_len = prim_min(old_bytes.len, new_byte_count);
    mem_copyBytes(init$S$((u8)(new_mem, copy_len)), old_bytes.as_const);
    // Zero out old memory before freeing
    mem_setBytes0(old_bytes);
    mem_Allocator_rawFree_debug(self, old_bytes, old_mem.type.align, src_loc);
    return_ok({
        .ptr = new_mem,
        .len = new_len,
        .type = old_mem.type,
    });
} $unscoped_(fn);

fn_((mem_Allocator_free_debug(mem_Allocator self, u_S$raw mem, SrcLoc src_loc))(void)) {
    // Special case for zero-sized types or empty slices
    if (mem.type.size == 0 || mem.len == 0) { return; }
    // Create byte slice from memory and free it
    S$u8 bytes = {
        .ptr = as$(u8*)(mem.ptr),
        .len = mem.type.size * mem.len,
    };
    mem_Allocator_rawFree_debug(self, bytes, mem.type.align, src_loc);
}

fn_((mem_Allocator_dupe_debug(mem_Allocator self, u_S$raw src, SrcLoc src_loc))(mem_Err$u_S$raw) $scope) {
    // Allocate new memory with same element type and count
    let new_mem = try_(mem_Allocator_alloc_debug(self, src.type, src.len, src_loc));
    // Copy data from source to new memory
    let src_bytes = init$S$((u8)(as$(u8*)(src.ptr), src.type.size * src.len));
    let dst_bytes = init$S$((u8)(as$(u8*)(new_mem.ptr), src.type.size * src.len));
    mem_copyBytes(dst_bytes, src_bytes.as_const);
    return_ok(new_mem);
} $unscoped_(fn);

fn_((mem_Allocator_dupeZ_debug(mem_Allocator self, u_S$raw src, SrcLoc src_loc))(mem_Err$u_S$raw) $scope) {
    // Allocate new memory with same element type but one extra element for sentinel
    let new_mem = try_(mem_Allocator_alloc_debug(self, src.type, src.len + 1, src_loc));
    // Copy data from source to new memory
    let src_bytes = init$S$((u8)(as$(u8*)(src.ptr), src.type.size * src.len));
    let dst_bytes = init$S$((u8)(as$(u8*)(new_mem.ptr), src.type.size * src.len));
    mem_copyBytes(dst_bytes, src_bytes.as_const);
    mem_setBytes0(dst_bytes);
    // // Set sentinel value at end
    // if (src.type.size == 1) {
    //     // For byte-sized elements, directly set the sentinel
    //     as$(u8*)(new_mem.addr)[src.len] = 0;
    // } else {
    //     // For larger elements, we'd need to know the exact type to properly set the sentinel
    //     // This simple implementation assumes sentinel is just the first byte of the element
    //       mem_set(as$(u8*)(new_mem.addr) + (src.len * src.type.size), 0, 1);
    // }
    return_ok({
        .type = new_mem.type,
        .ptr = new_mem.ptr,
        .len = src.len, // Note: we preserve original length, sentinel is separate
    });
} $unscoped_(fn);

#endif /* on_comptime && (!on_comptime || debug_comp_enabled) */

#include "dh/mem/Allocator.h"
#include "dh/mem/Tracker.h"

#if !COMP_TIME || (COMP_TIME && !debug_comp_enabled) || defined(MEM_NO_TRACE_ALLOC_AND_FREE)
Opt$Ptr$u8 mem_Allocator_rawAlloc(mem_Allocator self, usize len, usize ptr_align) {
    debug_assert_nonnull(self.ptr);
    debug_assert_nonnull(self.vt);
    return self.vt->alloc(self.ptr, len, ptr_align);
}
#else
Opt$Ptr$u8 mem_Allocator_rawAlloc_debug(
    mem_Allocator self,
    usize         len,
    usize         ptr_align,
    const char*   file,
    i32           line,
    const char*   func
) {
    debug_assert_nonnull(self.ptr);
    debug_assert_nonnull(self.vt);
    let result = self.vt->alloc(self.ptr, len, ptr_align);
    if (isSome(result)) {
        mem_Tracker_registerAlloc(unwrap(result), len, file, line, func);
    }
    return result;
}
#endif

#if !COMP_TIME || (COMP_TIME && !debug_comp_enabled) || defined(MEM_NO_TRACE_ALLOC_AND_FREE)
bool mem_Allocator_rawResize(mem_Allocator self, Sli$u8 buf, usize buf_align, usize new_len) {
    debug_assert_nonnull(self.ptr);
    debug_assert_nonnull(self.vt);
    return self.vt->resize(self.ptr, buf, buf_align, new_len);
}
#else
bool mem_Allocator_rawResize_debug(
    mem_Allocator self,
    Sli$u8        buf,
    usize         buf_align,
    usize         new_len,
    const char*   file,
    i32           line,
    const char*   func
) {
    debug_assert_nonnull(self.ptr);
    debug_assert_nonnull(self.vt);
    let result = self.vt->resize(self.ptr, buf, buf_align, new_len);
    if (result) {
        // Update tracking information for the resized allocation
        mem_Tracker_registerFree(buf.ptr, file, line, func);
        if (0 < new_len) {
            mem_Tracker_registerAlloc(buf.ptr, new_len, file, line, func);
        }
    }
    return result;
}
#endif

#if !COMP_TIME || (COMP_TIME && !debug_comp_enabled) || defined(MEM_NO_TRACE_ALLOC_AND_FREE)
void mem_Allocator_rawFree(mem_Allocator self, Sli$u8 buf, usize buf_align) {
    debug_assert_nonnull(self.ptr);
    debug_assert_nonnull(self.vt);
    if (buf.ptr == null) { return; /* Cannot free unallocated memory */ }
    if (buf.len == 0) { return; /* Cannot free zero-length buffer */ }
    self.vt->free(self.ptr, buf, buf_align);
}
#else
void mem_Allocator_rawFree_debug(
    mem_Allocator self,
    Sli$u8        buf,
    usize         buf_align,
    const char*   file,
    i32           line,
    const char*   func
) {
    debug_assert_nonnull(self.ptr);
    debug_assert_nonnull(self.vt);
    if (buf.ptr == null) { return; /* Cannot free unallocated memory */ }
    if (buf.len == 0) { return; /* Cannot free zero-length buffer */ }
    // Update tracking information for the freed allocation
    if (!mem_Tracker_registerFree(buf.ptr, file, line, func)) { return; /* Occurs double free */ };
    self.vt->free(self.ptr, buf, buf_align);
}
#endif

#if !COMP_TIME || (COMP_TIME && !debug_comp_enabled) || defined(MEM_NO_TRACE_ALLOC_AND_FREE)
mem_AllocErr$meta_Ptr mem_Allocator_create(mem_Allocator self, TypeInfo type) {
    reserveReturn(mem_AllocErr$meta_Ptr);
    let opt = mem_Allocator_rawAlloc(self, type.size, type.align);
    if_none(opt) {
        return_err(mem_AllocErr_OutOfMemory());
    }
    return_ok((meta_Ptr){ .addr = unwrap(opt), .type = type });
}
#else
mem_AllocErr$meta_Ptr mem_Allocator_create_debug(
    mem_Allocator self,
    TypeInfo      type,
    const char*   file,
    int           line,
    const char*   func
) {
    reserveReturn(mem_AllocErr$meta_Ptr);
    let opt = mem_Allocator_rawAlloc(self, type.size, type.align, file, line, func);
    if_none(opt) {
        return_err(mem_AllocErr_OutOfMemory());
    }
    return_ok((meta_Ptr){ .addr = unwrap(opt), .type = type });
}
#endif

#if !COMP_TIME || (COMP_TIME && !debug_comp_enabled) || defined(MEM_NO_TRACE_ALLOC_AND_FREE)
void mem_Allocator_destroy(mem_Allocator self, AnyType ptr) {
    if (ptr.ctx == null) { return; /* Cannot free unallocated memory */ }
    mem_Allocator_rawFree(
        self,
        make$(Sli$u8, .ptr = ptr.ctx, .len = ptr.type.size),
        ptr.type.align
    );
}
#else
void mem_Allocator_destroy_debug(
    mem_Allocator self,
    AnyType       ptr,
    const char*   file,
    int           line,
    const char*   func
) {
    if (ptr.ctx == null) { return; /* Cannot free unallocated memory */ }
    mem_Allocator_rawFree(
        self,
        make$(Sli$u8, .ptr = ptr.ctx, .len = ptr.type.size),
        ptr.type.align,
        file,
        line,
        func
    );
}
#endif

#if !COMP_TIME || (COMP_TIME && !debug_comp_enabled) || defined(MEM_NO_TRACE_ALLOC_AND_FREE)
mem_AllocErr$meta_Sli mem_Allocator_alloc(mem_Allocator self, TypeInfo type, usize count) {
    reserveReturn(mem_AllocErr$meta_Sli);
    let opt = mem_Allocator_rawAlloc(self, type.size * count, type.align);
    if_none(opt) {
        return_err(mem_AllocErr_OutOfMemory());
    }
    return_ok(make$(meta_Sli, .type = type, .addr = unwrap(opt), .len = count));
}
#else
mem_AllocErr$meta_Sli mem_Allocator_alloc_debug(
    mem_Allocator self,
    TypeInfo      type,
    usize         count,
    const char*   file,
    int           line,
    const char*   func
) {
    reserveReturn(mem_AllocErr$meta_Sli);
    let opt = mem_Allocator_rawAlloc(self, type.size * count, type.align, file, line, func);
    if_none(opt) {
        return_err(mem_AllocErr_OutOfMemory());
    }
    return_ok(make$(meta_Sli, .type = type, .addr = unwrap(opt), .len = count));
}
#endif

#if !COMP_TIME || (COMP_TIME && !debug_comp_enabled) || defined(MEM_NO_TRACE_ALLOC_AND_FREE)
bool mem_Allocator_resize(mem_Allocator self, AnyType old_mem, usize new_len) {
    if (old_mem.ctx == null) { return false; /* Cannot resize unallocated memory */ }

    let slice = (meta_Sli*)&old_mem;
    let type  = slice->type;

    if (new_len == 0) {
        mem_Allocator_free(self, old_mem);
        return true;
    }
    if (slice->len == 0) {
        return false; // Cannot resize empty slice
    }

    // Calculate new byte size - check for overflow
    if (usize_limit / type.size < new_len) {
        return false; // Would overflow
    }

    const usize new_byte_size = new_len * type.size;

    // Call raw resize with byte size
    return mem_Allocator_rawResize(
        self,
        make$(Sli$u8, .ptr = slice->addr, .len = slice->len * type.size),
        type.align,
        new_byte_size
    );
}
#else
bool mem_Allocator_resize_debug(
    mem_Allocator self,
    AnyType       old_mem,
    usize         new_len,
    const char*   file,
    int           line,
    const char*   func
) {
    if (old_mem.ctx == null) { return false; /* Cannot resize unallocated memory */ }

    let slice = (meta_Sli*)&old_mem;
    let type  = slice->type;

    if (new_len == 0) {
        mem_Allocator_free(self, old_mem, file, line, func);
        return true;
    }
    if (slice->len == 0) {
        return false; // Cannot resize empty slice
    }

    // Calculate new byte size - check for overflow
    if (usize_limit / type.size < new_len) {
        return false; // Would overflow
    }

    const usize new_byte_size = new_len * type.size;

    // Call raw resize with byte size
    return mem_Allocator_rawResize(
        self,
        make$(Sli$u8, .ptr = slice->addr, .len = slice->len * type.size),
        type.align,
        new_byte_size,
        file,
        line,
        func
    );
}
#endif

#if !COMP_TIME || (COMP_TIME && !debug_comp_enabled) || defined(MEM_NO_TRACE_ALLOC_AND_FREE)
Opt$meta_Sli mem_Allocator_realloc(mem_Allocator self, AnyType old_mem, usize new_len) {
    reserveReturn(Opt$meta_Sli);

    let slice = (meta_Sli*)&old_mem;
    let type  = slice->type;

    // Try resize in place first
    if (mem_Allocator_resize(self, old_mem, new_len)) {
        return_some(*slice);
    }

    // Allocate new buffer
    let new_slice = catch_from(mem_Allocator_alloc(self, type, new_len), err, {
        unused(err);
        return_none();
    });

    // Copy data and free old buffer
    let result   = new_slice;
    let copy_len = slice->len < new_len ? slice->len : new_len;

    memcpy(result.addr, slice->addr, copy_len * type.size);
    mem_Allocator_rawFree(
        self,
        make$(Sli$u8, .ptr = slice->addr, .len = slice->len * type.size),
        type.align
    );
    return_some(result);
}
#else
Opt$meta_Sli mem_Allocator_realloc_debug(
    mem_Allocator self,
    AnyType       old_mem,
    usize         new_len,
    const char*   file,
    int           line,
    const char*   func
) {
    reserveReturn(Opt$meta_Sli);

    let slice = (meta_Sli*)&old_mem;
    let type  = slice->type;

    // Try resize in place first
    if (mem_Allocator_resize(self, old_mem, new_len, file, line, func)) {
        return_some(*slice);
    }

    // Allocate new buffer
    let new_slice = catch_from(
        mem_Allocator_alloc(self, type, new_len, file, line, func),
        err,
        {
            unused(err);
            return_none();
        }
    );

    // Copy data and free old buffer
    let result   = new_slice;
    let copy_len = slice->len < new_len ? slice->len : new_len;

    memcpy(result.addr, slice->addr, copy_len * type.size);
    mem_Allocator_rawFree(
        self,
        make$(Sli$u8, .ptr = slice->addr, .len = slice->len * type.size),
        type.align,
        file,
        line,
        func
    );
    return_some(result);
}
#endif

#if !COMP_TIME || (COMP_TIME && !debug_comp_enabled) || defined(MEM_NO_TRACE_ALLOC_AND_FREE)
void mem_Allocator_free(mem_Allocator self, AnyType memory) {
    if (memory.ctx == null) { return; /* Cannot free unallocated memory */ }
    if (memory.len == 0) { return; }

    let slice = (meta_Sli*)&memory;
    mem_Allocator_rawFree(
        self,
        make$(Sli$u8, .ptr = slice->addr, .len = slice->len * slice->type.size),
        slice->type.align
    );
}
#else
void mem_Allocator_free_debug(
    mem_Allocator self,
    AnyType       memory,
    const char*   file,
    int           line,
    const char*   func
) {
    if (memory.ctx == null) { return; /* Cannot free unallocated memory */ }
    if (memory.len == 0) { return; }

    let slice = (meta_Sli*)&memory;
    mem_Allocator_rawFree(
        self,
        make$(Sli$u8, .ptr = slice->addr, .len = slice->len * slice->type.size),
        slice->type.align,
        file,
        line,
        func
    );
}
#endif

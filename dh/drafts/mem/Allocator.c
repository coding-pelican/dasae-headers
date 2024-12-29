#include "Allocator.h"

typedef struct Sli$u8 {
    u8*   ptr;
    usize len;
} Sli$u8;

/* Raw allocation */
Optptr$u8 mem_Allocator_rawAlloc(mem_Allocator self, usize len, usize ptr_align) {
    debug_assert_nonnull(self.vt);
    return self.vt->alloc(self.ctx, len, ptr_align);
}

/* Try to resize in-place */
bool mem_Allocator_rawResize(mem_Allocator self, Sli$u8 buf, usize buf_align, usize new_len) {
    debug_assert_nonnull(self.vt);
    debug_assert_nonnull(buf.ptr);
    return self.vt->resize(self.ctx, buf, buf_align, new_len);
}

/* Free memory */
void mem_Allocator_rawFree(mem_Allocator self, Sli$u8 buf, usize buf_align) {
    debug_assert_nonnull(self.vt);
    debug_assert_nonnull(buf.ptr);
    self.vt->free(self.ctx, buf, buf_align);
}

/* Single-item allocation */
Err$meta_Ptr mem_Allocator_create(mem_Allocator self, TypeInfo type) {
    reserveReturn(Err$meta_Ptr);
    let opt = mem_Allocator_rawAlloc(self, type.size, type.align);
    if (isNone(opt)) {
        return_err(mem_AllocErr_err(mem_AllocErrType_OutOfMemory));
    }
    return_ok((meta_Ptr){ .addr = unwrap(opt), .type = type });
}

/* Free single-item */
void mem_Allocator_destroy(mem_Allocator self, AnyType ptr) {
    debug_assert_nonnull(ptr.ctx);
    mem_Allocator_rawFree(
        self,
        (Sli$u8){ .ptr = ((meta_Ptr*)ptr.ctx)->addr, .len = ptr.type.size },
        ptr.type.align
    );
}

/* Sli allocation */
Err$meta_Sli mem_Allocator_alloc(mem_Allocator self, TypeInfo type, usize count) {
    reserveReturn(Err$meta_Sli);
    let opt = mem_Allocator_rawAlloc(self, type.size * count, type.align);
    if (isNone(opt)) {
        return_err(mem_AllocErr_err(mem_AllocErrType_OutOfMemory));
    }
    return_ok((meta_Sli){ .ptr = { .addr = unwrap(opt) }, .type = type, .len = count });
}

/* Try to resize slice in-place */
bool mem_Allocator_resize(mem_Allocator self, AnyType old_mem, usize new_len) {
    debug_assert_nonnull(old_mem.ctx);
    let slice = (meta_Sli*)old_mem.ctx;
    let type  = slice->type;
    if (new_len == 0) {
        mem_Allocator_free(self, old_mem);
        return true;
    }
    if (slice->len == 0) {
        return false;
    }
    /* Calculate new byte size - check for overflow */
    if (SIZE_MAX / type.size < new_len) { return false; /* Would overflow */ }
    const usize new_byte_size = new_len * type.size;
    /* Call raw resize with byte size */
    return mem_Allocator_rawResize(
        self,
        (Sli$u8){ .ptr = slice->addr, .len = slice->len * type.size },
        type.align,
        new_byte_size
    );
}

/* Reallocate slice with new size */
Opt$meta_Sli mem_Allocator_realloc(mem_Allocator self, AnyType old_mem, usize new_len) {
    reserveReturn(Opt$meta_Sli);
    debug_assert_nonnull(old_mem.ctx);
    let slice = (meta_Sli*)old_mem.ctx;
    let type  = slice->type;

    // Try resize in place first
    if (mem_Allocator_resize(self, old_mem, new_len)) {
        return_some(*slice);
    }

    // Allocate new buffer
    let new_slice = mem_Allocator_alloc(self, type, new_len);
    if (new_slice.is_err) {
        return_none();
    }

    // Copy data and free old buffer
    let result   = new_slice.ok;
    let copy_len = slice->len < new_len ? slice->len : new_len;

    memcpy(result.addr, slice->addr, copy_len * type.size);
    mem_Allocator_rawFree(
        self,
        (Sli$u8){ .ptr = slice->addr, .len = slice->len * type.size },
        type.align
    );
    return_some(result);
}

/* Free slice */
void mem_Allocator_free(mem_Allocator self, AnyType memory) {
    debug_assert_nonnull(memory.ctx);
    let slice = (meta_Sli*)memory.ctx;
    mem_Allocator_rawFree(
        self,
        (Sli$u8){ .ptr = slice->addr, .len = slice->len * slice->type.size },
        slice->type.align
    );
}

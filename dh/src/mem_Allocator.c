#include "dh/mem/Allocator.h"
#include "dh/debug/assert.h"

Optptr$u8 mem_Allocator_rawAlloc(mem_Allocator self, usize len, usize ptr_align) {
    return self.vt->alloc(self.ctx, len, ptr_align);
}

bool mem_Allocator_rawResize(mem_Allocator self, Sli$u8 buf, usize buf_align, usize new_len) {
    debug_assert_nonnull(buf.ptr);
    return self.vt->resize(self.ctx, buf, buf_align, new_len);
}

void mem_Allocator_rawFree(mem_Allocator self, Sli$u8 buf, usize buf_align) {
    debug_assert_nonnull(buf.ptr);
    self.vt->free(self.ctx, buf, buf_align);
}

impl_Err$(Ptr);
Err$Ptr mem_Allocator_create(mem_Allocator self, TypeInfo type) {
    reserveReturn(Err$Ptr);
    let opt = mem_Allocator_rawAlloc(self, type.size, type.align);
    if (isNone(opt)) {
        return_err(mem_AllocErr_err(mem_AllocErrType_OutOfMemory));
    }
    return_ok((Ptr){ .addr = unwrap(opt), .type = type });
}

void mem_Allocator_destroy(mem_Allocator self, Ptr ptr) {
    debug_assert_nonnull(ptr.addr);
    mem_Allocator_rawFree(self, (Sli$u8){ .ptr = ptr.addr, .len = ptr.type.size }, ptr.type.align);
}

impl_Err$(Sli);
Err$Sli mem_Allocator_alloc(mem_Allocator self, TypeInfo type, usize count) {
    reserveReturn(Err$Sli);
    let opt = mem_Allocator_rawAlloc(self, type.size * count, type.align);
    if (isNone(opt)) {
        return_err(mem_AllocErr_err(mem_AllocErrType_OutOfMemory));
    }
    Sli sli      = asSli(unwrap(opt), count);
    sli.ptr.type = type;
    return_ok(sli);
}

bool mem_Allocator_resize(mem_Allocator self, Ptr_Sli sli, usize new_len) {
    debug_assert_nonnull(sli.addr);
    debug_assert_nonnull(sli.addr->ptr);

    if (new_len == 0) {
        mem_Allocator_free(self, sli.ref->value);
        return true;
    }

    if (sli.addr->len == 0) {
        return false;
    }

    // Calculate new byte size - check for overflow
    const usize elem_size = sli.addr->type.size;
    if (new_len > SIZE_MAX / elem_size) {
        return false; // Would overflow
    }
    const usize new_byte_size = new_len * elem_size;

    // Call raw resize with byte size
    if (mem_Allocator_rawResize(self, Sli_u8_from(sli.addr->addr, 0, sli.addr->len * sli.addr->type.size), sli.addr->type.align, new_byte_size)) {
        sli.addr->len = new_len;
        return true;
    }
    return false;
}

Opt_Sli mem_Allocator_realloc(mem_Allocator self, Sli sli, usize new_len) {
    debug_assert_nonnull(sli.addr);

    // Try resize in place first
    if (mem_Allocator_resize(self, Ptr_Sli_from(&sli), new_len)) {
        return Opt_Sli_some(sli);
    }

    // Allocate new buffer
    let new_slice = mem_Allocator_alloc(self, sli.type, new_len);
    if (ResErr_Sli_isErr(new_slice)) {
        return Opt_Sli_none();
    }

    // Copy data and free old buffer
    let result   = ResErr_Sli_unwrap(new_slice);
    let copy_len = sli.len < new_len ? sli.len : new_len;

    memcpy(
        result.addr,
        sli.addr,
        copy_len * sli.type.size
    );

    mem_Allocator_rawFree(self, Sli_u8_from(sli.addr, 0, sli.len * sli.type.size), sli.type.align);
    return Opt_Sli_some(result);
}

void mem_Allocator_free(mem_Allocator self, Sli sli) {
    debug_assert_nonnull(sli.addr);
    mem_Allocator_rawFree(self, Sli_u8_from(sli.addr, 0, sli.len * sli.type.size), sli.type.align);
}

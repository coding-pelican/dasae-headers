#include "dh/mem/Alctr.h"
#include "dh/mem/AlcTrace.h"
#include "dh/mem/common.h"

fn_((mem_Alctr_VTbl_noAlloc(P$raw ctx, usize len, mem_Align align))(O$P$u8)) {
    let_ignore = ctx;
    let_ignore = len;
    let_ignore = align;
    return none$((O$P$u8));
};

fn_((mem_Alctr_VTbl_noResize(P$raw ctx, S$u8 buf, mem_Align buf_align, usize new_len))(bool)) {
    let_ignore = ctx;
    let_ignore = buf;
    let_ignore = buf_align;
    let_ignore = new_len;
    return false;
};

fn_((mem_Alctr_VTbl_noRemap(P$raw ctx, S$u8 buf, mem_Align buf_align, usize new_len))(O$P$u8)) {
    let_ignore = ctx;
    let_ignore = buf;
    let_ignore = buf_align;
    let_ignore = new_len;
    return none$((O$P$u8));
};

fn_((mem_Alctr_VTbl_noFree(P$raw ctx, S$u8 buf, mem_Align buf_align))(void)) {
    let_ignore = ctx;
    let_ignore = buf;
    let_ignore = buf_align;
};

fn_((mem_Alctr_rawAlloc($traced mem_Alctr self, usize len, mem_Align align))(O$P$u8) $scope) {
    self = mem_Alctr_ensureValid(self);
    if (len == 0) {
        let addr = intToPtr$((u8*)(usize_limit_max & ~(mem_log2ToAlign(align) - 1)));
        mem_AlcTrace_registerAlloc($tracing addr, len);
        return_some(addr);
    }
    let result = self.vtbl->alloc(self.ctx, len, align);
    if_some((result)(addr)) mem_AlcTrace_registerAlloc($tracing addr, len);
    return result;
} $unscoped(fn);

fn_((mem_Alctr_rawResize($traced mem_Alctr self, S$u8 buf, mem_Align buf_align, usize new_len))(bool)) {
    self = mem_Alctr_ensureValid(self);
    if (new_len == 0) {
        mem_Alctr_rawFree($trace self, buf, buf_align);
        mem_AlcTrace_registerRemap($tracing buf.ptr, buf.ptr, new_len);
        return true;
    }
    if (buf.len == 0) return false;
    let result = self.vtbl->resize(self.ctx, buf, buf_align, new_len);
    if (result) mem_AlcTrace_registerRemap($tracing buf.ptr, buf.ptr, new_len);
    return result;
};

fn_((mem_Alctr_rawRemap($traced mem_Alctr self, S$u8 buf, mem_Align buf_align, usize new_len))(O$P$u8) $scope) {
    self = mem_Alctr_ensureValid(self);
    if (new_len == 0) {
        mem_Alctr_rawFree($trace self, buf, buf_align);
        let addr = intToPtr$((u8*)(usize_limit_max & ~(mem_log2ToAlign(buf_align) - 1)));
        mem_AlcTrace_registerRemap($tracing buf.ptr, addr, new_len);
        return_some(addr);
    }
    if (buf.len == 0) return_none();
    let result = self.vtbl->remap(self.ctx, buf, buf_align, new_len);
    if_some((result)(addr)) mem_AlcTrace_registerRemap($tracing buf.ptr, addr, new_len);
    return result;
} $unscoped(fn);

fn_((mem_Alctr_rawFree($traced mem_Alctr self, S$u8 buf, mem_Align buf_align))(void)) {
    self = mem_Alctr_ensureValid(self);
    if (buf.len == 0) return;
    mem_set0Bytes(buf);
    mem_AlcTrace_registerFree($tracing buf.ptr);
    return self.vtbl->free(self.ctx, buf, buf_align);
};

fn_((mem_Alctr_create($traced mem_Alctr self, TypeInfo type))(mem_E$u_P$raw) $scope) {
    if (type.size == 0) return_ok({
        .raw = intToPtr$((P$raw)(usize_limit_max & ~(TypeInfo_align(type) - 1))),
        .type = type,
    });
    let mem = orelse_((mem_Alctr_rawAlloc($tracing self, type.size, type.log2_align))(
        return_err(mem_E_OutOfMemory())
    ));
    mem_set0Bytes(P_prefix$((S$u8)(mem)(type.size)));
    return_ok({
        .raw = mem,
        .type = type,
    });
} $unscoped(fn);

fn_((mem_Alctr_destroy($traced mem_Alctr self, u_P$raw ptr))(void)) {
    if (ptr.type.size == 0) return;
    let mem = P_prefix$((S$u8)(ptr.raw)(ptr.type.size));
    mem_Alctr_rawFree($tracing self, mem, ptr.type.log2_align);
};

fn_((mem_Alctr_clone($traced mem_Alctr self, u_P_const$raw ptr))(mem_E$u_P$raw) $scope) {
    let new_mem = try_(mem_Alctr_create($tracing self, ptr.type));
    return_ok(mem_copyP(new_mem, ptr));
} $unscoped(fn);

fn_((mem_Alctr_alloc($traced mem_Alctr self, TypeInfo type, usize count))(mem_E$u_S$raw) $scope) {
    if (type.size == 0 || count == 0) return_ok({
        .ptr = intToPtr$((P$raw)(usize_limit_max & ~(TypeInfo_align(type) - 1))),
        .len = count,
        .type = type,
    });
    let byte_count = orelse_((usize_mulChkd(type.size, count))(
        return_err(mem_E_OutOfMemory())
    ));
    let mem = orelse_((mem_Alctr_rawAlloc($tracing self, byte_count, type.log2_align))(
        return_err(mem_E_OutOfMemory())
    ));
    mem_set0Bytes(P_prefix$((S$u8)(mem)(byte_count)));
    return_ok({
        .ptr = mem,
        .len = count,
        .type = type,
    });
} $unscoped(fn);

fn_((mem_Alctr_resize($traced mem_Alctr self, u_S$raw old_mem, usize new_len))(bool)) {
    if (old_mem.type.size == 0) return true;
    if (new_len == 0) {
        mem_Alctr_free($tracing self, old_mem);
        return true;
    }
    if (old_mem.len == 0) return false;
    // Create byte slice from the old memory
    let old_bytes = P_prefix$((S$u8)(old_mem.ptr)(old_mem.type.size * old_mem.len));
    // Check for overflow in multiplication
    let new_byte_count = orelse_((usize_mulChkd(old_mem.type.size, new_len))(
        return false
    ));
    return mem_Alctr_rawResize($tracing self, old_bytes, old_mem.type.log2_align, new_byte_count);
};

fn_((mem_Alctr_remap($traced mem_Alctr self, u_S$raw old_mem, usize new_len))(O$u_S$raw) $scope) {
    if (old_mem.type.size == 0) return_some({
        .ptr = intToPtr$((P$raw)(usize_limit_max & ~(TypeInfo_align(old_mem.type) - 1))),
        .len = new_len,
        .type = old_mem.type,
    });
    if (new_len == 0) {
        mem_Alctr_free($tracing self, old_mem);
        return_some({
            .ptr = intToPtr$((P$raw)(usize_limit_max & ~(TypeInfo_align(old_mem.type) - 1))),
            .len = 0,
            .type = old_mem.type,
        });
    }
    if (old_mem.len == 0) return_none();
    // Create byte slice from the old memory
    let old_bytes = P_prefix$((S$u8)(old_mem.ptr)(old_mem.type.size * old_mem.len));
    // Check for overflow in multiplication
    let new_byte_count = orelse_((usize_mulChkd(old_mem.type.size, new_len))(
        return_none()
    ));
    let new_ptr = orelse_((mem_Alctr_rawRemap($tracing self, old_bytes, old_mem.type.log2_align, new_byte_count))(
        return_none()
    ));
    return_some({
        .ptr = new_ptr,
        .len = new_len,
        .type = old_mem.type,
    });
} $unscoped(fn);

fn_((mem_Alctr_realloc($traced mem_Alctr self, u_S$raw old_mem, usize new_len))(mem_E$u_S$raw) $scope) {
    // Special case for empty old memory
    if (old_mem.len == 0) {
        // This is equivalent to a new allocation
        return_ok(try_(mem_Alctr_alloc($tracing self, old_mem.type, new_len)));
    }
    // Special case for zero new length
    if (new_len == 0) {
        mem_Alctr_free($tracing self, old_mem);
        return_ok({
            .ptr = intToPtr$((P$raw)(usize_limit_max & ~(TypeInfo_align(old_mem.type) - 1))),
            .len = 0,
            .type = old_mem.type,
        });
    }
    // Special case for zero-sized types
    if (old_mem.type.size == 0) return_ok({
        .ptr = intToPtr$((P$raw)(usize_limit_max & ~(TypeInfo_align(old_mem.type) - 1))),
        .len = new_len,
        .type = old_mem.type,
    });
    // Create byte slice from the old memory
    let old_bytes = P_prefix$((S$u8)(old_mem.ptr)(old_mem.type.size * old_mem.len));
    // Check for overflow in multiplication
    let new_byte_count = orelse_((usize_mulChkd(old_mem.type.size, new_len))(
        return_err(mem_E_OutOfMemory())
    ));
    // Try to remap first (which may be in-place resize or may relocate)
    let new_ptr = mem_Alctr_rawRemap($tracing self, old_bytes, old_mem.type.log2_align, new_byte_count);
    if_some((new_ptr)(ptr)) return_ok({
        .ptr = ptr,
        .len = new_len,
        .type = old_mem.type,
    });
    // Remap failed, need to allocate new memory and copy
    let new_mem = orelse_((mem_Alctr_rawAlloc($tracing self, new_byte_count, old_mem.type.log2_align))(
        return_err(mem_E_OutOfMemory())
    ));
    // Copy the data from old memory to new memory (use smaller of the two sizes)
    let copy_len = pri_min(old_bytes.len, new_byte_count);
    let new_bytes = P_prefix$((S$u8)(new_mem)(copy_len));
    mem_copyBytes(new_bytes, old_bytes.as_const);
    // Zero out old memory before freeing
    mem_set0Bytes(old_bytes);
    mem_Alctr_rawFree($tracing self, old_bytes, old_mem.type.log2_align);
    return_ok({
        .ptr = new_mem,
        .len = new_len,
        .type = old_mem.type,
    });
} $unscoped(fn);

fn_((mem_Alctr_free($traced mem_Alctr self, u_S$raw mem))(void)) {
    // Special case for zero-sized types or empty slices
    if (mem.type.size == 0 || mem.len == 0) return;
    // Create byte slice from memory and free it
    let bytes = P_prefix$((S$u8)(mem.ptr)(mem.type.size * mem.len));
    mem_Alctr_rawFree($tracing self, bytes, mem.type.log2_align);
};

fn_((mem_Alctr_dupe($traced mem_Alctr self, u_S_const$raw src))(mem_E$u_S$raw) $scope) {
    let new_mem = try_(mem_Alctr_alloc($tracing self, src.type, src.len));
    return_ok(mem_copy(new_mem, src));
} $unscoped(fn);

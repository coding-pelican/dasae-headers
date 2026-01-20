#include "dh/prl.h"

/* TODO: Remove unnecessary stride checking in meta operations */
/* TODO: Add valid typeInfo, Ensure typeInfo -> mem_log2ToAlign(info.align) <= info.size, when info.size is not zero. */
/* TODO: Add P_meta, S_meta */

#define $traced __attr__$traced
#define $trace __attr__$trace
#define $tracing __val__$tracing

typedef struct debug_SrcLoc { /* clang-format off */
    var_(base, pp_if_(debug_comp_enabled)(
        pp_then_(SrcLoc),
        pp_else_(Void))
    );
} debug_SrcLoc;
#define debug_srcLoc() lit$((debug_SrcLoc){ \
    .base = pp_if_(debug_comp_enabled)( \
        pp_then_(srcLoc()), \
        pp_else_({}) \
    ), \
}) /* clang-format on */

#define __attr__$traced \
    $maybe_unused debug_SrcLoc __traced,
#define __attr__$trace \
    debug_srcLoc(),
#define __val__$tracing \
    __traced,

#include "dh/mem/cfg.h"
#include "dh/mem/common.h"

/*========== Macros and Declarations ========================================*/

/* --- Allocator Interface --- */

/// Allocator vtable
typedef struct mem_Allocator_VT {
    fn_(((*const alloc)(P$raw ctx, usize len, mem_Align align))(O$P$u8)) $must_check;
    fn_(((*const resize)(P$raw ctx, S$u8 buf, mem_Align buf_align, usize new_len))(bool)) $must_check;
    fn_(((*const remap)(P$raw ctx, S$u8 buf, mem_Align buf_align, usize new_len))(O$P$u8)) $must_check;
    fn_(((*const free)(P$raw ctx, S$u8 buf, mem_Align buf_align))(void));
} mem_Allocator_VT;

/// Allocator instance
typedef struct mem_Allocator {
    var_(ctx, P$raw); /**< Context pointer */
    var_(vt, P_const$$(mem_Allocator_VT)); /**< Virtual table */
} mem_Allocator;
T_use_O$(mem_Allocator);
T_use_E$($set(mem_Err)(u_P$raw));
T_use_E$($set(mem_Err)(u_S$raw));

$attr($inline_always)
$static fn_((mem_Allocator_isValid(mem_Allocator self))(bool));
$attr($inline_always)
$static fn_((mem_Allocator_assertValid(P$raw ctx, P_const$$(mem_Allocator_VT) vt))(void));
$attr($inline_always)
$static fn_((mem_Allocator_ensureValid(mem_Allocator self))(mem_Allocator));

/* --- Core Allocator Functions --- */

/// Raw allocation
$extern fn_((mem_Allocator_rawAlloc($traced mem_Allocator self, usize len, mem_Align align))(O$P$u8));
/// Try to resize in-place
$extern fn_((mem_Allocator_rawResize($traced mem_Allocator self, S$u8 buf, mem_Align buf_align, usize new_len))(bool));
/// Try to resize, allowing relocation
$extern fn_((mem_Allocator_rawRemap($traced mem_Allocator self, S$u8 buf, mem_Align buf_align, usize new_len))(O$P$u8));
/// Free mem
$extern fn_((mem_Allocator_rawFree($traced mem_Allocator self, S$u8 buf, mem_Align buf_align))(void));

/* --- High-level Allocator Functions --- */

/// Single-item allocation
$extern fn_((mem_Allocator_create($traced mem_Allocator self, TypeInfo type))(mem_Err$u_P$raw)) $must_check;
/// Free single-item
$extern fn_((mem_Allocator_destroy($traced mem_Allocator self, u_P$raw ptr))(void));
/// Slice allocation
$extern fn_((mem_Allocator_alloc($traced mem_Allocator self, TypeInfo type, usize count))(mem_Err$u_S$raw)) $must_check;
/// Try to resize slice in-place
$extern fn_((mem_Allocator_resize($traced mem_Allocator self, u_S$raw old_mem, usize new_len))(bool));
/// Try to resize slice, allowing relocation
$extern fn_((mem_Allocator_remap($traced mem_Allocator self, u_S$raw old_mem, usize new_len))(O$u_S$raw));
/// Reallocate slice with new size
$extern fn_((mem_Allocator_realloc($traced mem_Allocator self, u_S$raw old_mem, usize new_len))(mem_Err$u_S$raw)) $must_check;
/// Free slice
$extern fn_((mem_Allocator_free($traced mem_Allocator self, u_S$raw mem))(void));
/// Duplicate slice
$extern fn_((mem_Allocator_dupe($traced mem_Allocator self, u_S$raw src))(mem_Err$u_S$raw)) $must_check;

/*========== Macros and Definitions ========================================*/

fn_((mem_Allocator_isValid(mem_Allocator self))(bool)) {
    return self.ctx != null
        && self.vt != null
        && self.vt->alloc != null
        && self.vt->resize != null
        && self.vt->remap != null
        && self.vt->free != null;
};

fn_((mem_Allocator_assertValid(P$raw ctx, P_const$$(mem_Allocator_VT) vt))(void)) {
    claim_assert_nonnull(ctx);
    claim_assert_nonnull(vt);
    claim_assert_nonnull(vt->alloc);
    claim_assert_nonnull(vt->resize);
    claim_assert_nonnull(vt->remap);
    claim_assert_nonnull(vt->free);
};

fn_((mem_Allocator_ensureValid(mem_Allocator self))(mem_Allocator)) {
    return mem_Allocator_assertValid(self.ctx, self.vt), self;
};

/*========== Tracing Functions ==============================================*/

$extern fn_((mem_Tracker_registerAlloc($traced P$raw ptr, usize size))(void));
$extern fn_((mem_Tracker_registerRemap($traced P$raw old_ptr, P$raw new_ptr, usize new_size))(void));
$extern fn_((mem_Tracker_registerFree($traced P$raw ptr))(bool));

/*========== External Definitions ===========================================*/

fn_((mem_Allocator_rawAlloc($traced mem_Allocator self, usize len, mem_Align align))(O$P$u8) $scope) {
    self = mem_Allocator_ensureValid(self);
    if (len == 0) {
        let addr = intToPtr$((u8*)(usize_limit_max & ~(mem_log2ToAlign(align) - 1)));
        mem_Tracker_registerAlloc($tracing addr, len);
        return_some(addr);
    }
    let result = self.vt->alloc(self.ctx, len, align);
    if_some((result)(addr)) { mem_Tracker_registerAlloc($tracing addr, len); }
    return result;
} $unscoped_(fn);

fn_((mem_Allocator_rawResize($traced mem_Allocator self, S$u8 buf, mem_Align buf_align, usize new_len))(bool)) {
    self = mem_Allocator_ensureValid(self);
    if (new_len == 0) {
        mem_Allocator_rawFree($trace self, buf, buf_align);
        mem_Tracker_registerRemap($tracing buf.ptr, buf.ptr, new_len);
        return true;
    }
    if (buf.len == 0) {
        return false;
    }
    let result = self.vt->resize(self.ctx, buf, buf_align, new_len);
    if (result) { mem_Tracker_registerRemap($tracing buf.ptr, buf.ptr, new_len); }
    return result;
};

fn_((mem_Allocator_rawRemap($traced mem_Allocator self, S$u8 buf, mem_Align buf_align, usize new_len))(O$P$u8) $scope) {
    self = mem_Allocator_ensureValid(self);
    if (new_len == 0) {
        mem_Allocator_rawFree($trace self, buf, buf_align);
        let addr = intToPtr$((u8*)(usize_limit_max & ~(mem_log2ToAlign(buf_align) - 1)));
        mem_Tracker_registerRemap($tracing buf.ptr, addr, new_len);
        return_some(addr);
    }
    if (buf.len == 0) {
        return_none();
    }
    let result = self.vt->remap(self.ctx, buf, buf_align, new_len);
    if_some((result)(addr)) { mem_Tracker_registerRemap($tracing buf.ptr, addr, new_len); }
    return result;
} $unscoped_(fn);

fn_((mem_Allocator_rawFree($traced mem_Allocator self, S$u8 buf, mem_Align buf_align))(void)) {
    self = mem_Allocator_ensureValid(self);
    if (buf.len == 0) { return; }
    mem_setBytes0(buf);
    mem_Tracker_registerFree($tracing buf.ptr);
    return self.vt->free(self.ctx, buf, buf_align);
};

fn_((mem_Allocator_create($traced mem_Allocator self, TypeInfo type))(mem_Err$u_P$raw) $scope) {
    if (type.size == 0) {
        return_ok({
            .raw = intToPtr$((P$raw)(usize_limit_max & ~(mem_log2ToAlign(type.align) - 1))),
            .type = type,
        });
    }
    let mem = orelse_((mem_Allocator_rawAlloc($tracing self, type.size, type.align))(
        return_err(mem_Err_OutOfMemory())
    ));
    mem_setBytes0(P_prefix$((S$u8)(mem)(type.size)));
    return_ok({
        .raw = mem,
        .type = type,
    });
} $unscoped_(fn);

fn_((mem_Allocator_destroy($traced mem_Allocator self, u_P$raw ptr))(void)) {
    if (ptr.type.size == 0) {
        return;
    }
    let mem = P_prefix$((S$u8)(ptr.raw)(ptr.type.size));
    mem_Allocator_rawFree($tracing self, mem, ptr.type.align);
};

fn_((mem_Allocator_alloc($traced mem_Allocator self, TypeInfo type, usize count))(mem_Err$u_S$raw) $scope) {
    if (type.size == 0 || count == 0) {
        return_ok({
            .ptr = intToPtr$((P$raw)(usize_limit_max & ~(mem_log2ToAlign(type.align) - 1))),
            .len = count,
            .type = type,
        });
    }
    let byte_count = orelse_((usize_mulChkd(type.size, count))(
        return_err(mem_Err_OutOfMemory())
    ));
    let mem = orelse_((mem_Allocator_rawAlloc($tracing self, byte_count, type.align))(
        return_err(mem_Err_OutOfMemory())
    ));
    mem_setBytes0(P_prefix$((S$u8)(mem)(byte_count)));
    return_ok({
        .ptr = mem,
        .len = count,
        .type = type,
    });
} $unscoped_(fn);

fn_((mem_Allocator_resize($traced mem_Allocator self, u_S$raw old_mem, usize new_len))(bool)) {
    if (old_mem.type.size == 0) {
        return true;
    }
    if (new_len == 0) {
        mem_Allocator_free($tracing self, old_mem);
        return true;
    }
    if (old_mem.len == 0) {
        return false;
    }
    // Create byte slice from the old memory
    let old_bytes = P_prefix$((S$u8)(old_mem.ptr)(old_mem.type.size * old_mem.len));
    // Check for overflow in multiplication
    let new_byte_count = orelse_((usize_mulChkd(old_mem.type.size, new_len))(
        return false
    ));
    return mem_Allocator_rawResize($tracing self, old_bytes, old_mem.type.align, new_byte_count);
};

fn_((mem_Allocator_remap($traced mem_Allocator self, u_S$raw old_mem, usize new_len))(O$u_S$raw) $scope) {
    if (old_mem.type.size == 0) {
        return_some({
            .ptr = intToPtr$((P$raw)(usize_limit_max & ~(mem_log2ToAlign(old_mem.type.align) - 1))),
            .len = new_len,
            .type = old_mem.type,
        });
    }
    if (new_len == 0) {
        mem_Allocator_free($tracing self, old_mem);
        return_some({
            .ptr = intToPtr$((P$raw)(usize_limit_max & ~(mem_log2ToAlign(old_mem.type.align) - 1))),
            .len = 0,
            .type = old_mem.type,
        });
    }
    if (old_mem.len == 0) {
        return_none();
    }
    // Create byte slice from the old memory
    let old_bytes = P_prefix$((S$u8)(old_mem.ptr)(old_mem.type.size * old_mem.len));
    // Check for overflow in multiplication
    let new_byte_count = orelse_((usize_mulChkd(old_mem.type.size, new_len))(
        return_none()
    ));
    let new_ptr = orelse_((mem_Allocator_rawRemap($tracing self, old_bytes, old_mem.type.align, new_byte_count))(
        return_none()
    ));
    return_some({
        .ptr = new_ptr,
        .len = new_len,
        .type = old_mem.type,
    });
} $unscoped_(fn);

fn_((mem_Allocator_realloc($traced mem_Allocator self, u_S$raw old_mem, usize new_len))(mem_Err$u_S$raw) $scope) {
    // Special case for empty old memory
    if (old_mem.len == 0) {
        // This is equivalent to a new allocation
        return_ok(try_(mem_Allocator_alloc($tracing self, old_mem.type, new_len)));
    }
    // Special case for zero new length
    if (new_len == 0) {
        mem_Allocator_free($tracing self, old_mem);
        return_ok({
            .ptr = intToPtr$((P$raw)(usize_limit_max & ~(mem_log2ToAlign(old_mem.type.align) - 1))),
            .len = 0,
            .type = old_mem.type,
        });
    }
    // Special case for zero-sized types
    if (old_mem.type.size == 0) {
        return_ok({
            .ptr = intToPtr$((P$raw)(usize_limit_max & ~(mem_log2ToAlign(old_mem.type.align) - 1))),
            .len = new_len,
            .type = old_mem.type,
        });
    }
    // Create byte slice from the old memory
    let old_bytes = P_prefix$((S$u8)(old_mem.ptr)(old_mem.type.size * old_mem.len));
    // Check for overflow in multiplication
    let new_byte_count = orelse_((usize_mulChkd(old_mem.type.size, new_len))(
        return_err(mem_Err_OutOfMemory())
    ));
    // Try to remap first (which may be in-place resize or may relocate)
    let new_ptr = mem_Allocator_rawRemap($tracing self, old_bytes, old_mem.type.align, new_byte_count);
    if_some((new_ptr)(ptr)) {
        return_ok({
            .ptr = ptr,
            .len = new_len,
            .type = old_mem.type,
        });
    }
    // Remap failed, need to allocate new memory and copy
    let new_mem = orelse_((mem_Allocator_rawAlloc($tracing self, new_byte_count, old_mem.type.align))(
        return_err(mem_Err_OutOfMemory())
    ));
    // Copy the data from old memory to new memory (use smaller of the two sizes)
    let copy_len = prim_min(old_bytes.len, new_byte_count);
    let new_bytes = P_prefix$((S$u8)(new_mem)(copy_len));
    mem_copyBytes(new_bytes, old_bytes.as_const);
    // Zero out old memory before freeing
    mem_setBytes0(old_bytes);
    mem_Allocator_rawFree($tracing self, old_bytes, old_mem.type.align);
    return_ok({
        .ptr = new_mem,
        .len = new_len,
        .type = old_mem.type,
    });
} $unscoped_(fn);

fn_((mem_Allocator_free($traced mem_Allocator self, u_S$raw mem))(void)) {
    // Special case for zero-sized types or empty slices
    if (mem.type.size == 0 || mem.len == 0) { return; }
    // Create byte slice from memory and free it
    let bytes = P_prefix$((S$u8)(mem.ptr)(mem.type.size * mem.len));
    mem_Allocator_rawFree($tracing self, bytes, mem.type.align);
};

fn_((mem_Allocator_dupe($traced mem_Allocator self, u_S$raw src))(mem_Err$u_S$raw) $scope) {
    // Allocate new memory with same element type and count
    let new_mem = try_(mem_Allocator_alloc($tracing self, src.type, src.len));
    // Copy data from source to new memory
    let src_bytes = P_prefix$((S$u8)(src.ptr)(src.type.size * src.len));
    let dst_bytes = P_prefix$((S$u8)(new_mem.ptr)(src.type.size * src.len));
    mem_copyBytes(dst_bytes, src_bytes.as_const);
    return_ok(new_mem);
} $unscoped_(fn);

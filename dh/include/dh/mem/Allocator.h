/**
 * @copyright Copyright (c) 2024-2025 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    Allocator.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2024-12-07 (date of creation)
 * @updated 2025-03-20 (date of last update)
 * @version v0.1-alpha.2
 * @ingroup dasae-headers(dh)/mem
 * @prefix  mem_Allocator
 *
 * @brief   Memory allocator interface using smart pointers
 * @details Provides type-safe mem allocation and management
 */

#ifndef MEM_ALLOCATOR_INCLUDED
#define MEM_ALLOCATOR_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "cfg.h"
#include "dh/meta/common.h"

/*========== Memory Error ===================================================*/

/// Allocation Error type
config_ErrSet(mem_Allocator_Err, OutOfMemory);

/*========== Allocator Interface ============================================*/

/// Allocator vtable
typedef struct mem_Allocator_VT {
    fn_(((*alloc)(anyptr ctx, usize len, u32 align))(Opt$Ptr$u8)) $must_check;
    fn_(((*resize)(anyptr ctx, Sli$u8 buf, u32 buf_align, usize new_len))(bool)) $must_check;
    fn_(((*remap)(anyptr ctx, Sli$u8 buf, u32 buf_align, usize new_len))(Opt$Ptr$u8)) $must_check;
    fn_(((*free)(anyptr ctx, Sli$u8 buf, u32 buf_align))(void));
} mem_Allocator_VT;

/// Allocator instance
typedef struct mem_Allocator {
    anyptr                  ptr; /**< Context pointer with type info */
    const mem_Allocator_VT* vt;  /**< Virtual table */
} mem_Allocator;
use_Opt$(mem_Allocator);
use_ErrSet$(mem_Allocator_Err, meta_Ptr);
use_ErrSet$(mem_Allocator_Err, meta_Sli);

/*========== Common VTable Functions ========================================*/

/// Default VTable functions for no-op fallbacks
extern fn_((mem_Allocator_VT_noAlloc(anyptr ctx, usize len, u32 align))(Opt$Ptr$u8));
/// Default VTable functions for no-op fallbacks
extern fn_((mem_Allocator_VT_noResize(anyptr ctx, Sli$u8 buf, u32 buf_align, usize new_len))(bool));
/// Default VTable functions for no-op fallbacks
extern fn_((mem_Allocator_VT_noRemap(anyptr ctx, Sli$u8 buf, u32 buf_align, usize new_len))(Opt$Ptr$u8));
/// Default VTable functions for no-op fallbacks
extern fn_((mem_Allocator_VT_noFree(anyptr ctx, Sli$u8 buf, u32 buf_align))(void));

#if !COMP_TIME || (COMP_TIME && !debug_comp_enabled)

/*========== Core Allocator Functions =======================================*/

/// Raw allocation
extern fn_((mem_Allocator_rawAlloc(mem_Allocator self, usize len, u32 align))(Opt$Ptr$u8));
/// Try to resize in-place
extern fn_((mem_Allocator_rawResize(mem_Allocator self, Sli$u8 buf, u32 buf_align, usize new_len))(bool));
/// Try to resize, allowing relocation
extern fn_((mem_Allocator_rawRemap(mem_Allocator self, Sli$u8 buf, u32 buf_align, usize new_len))(Opt$Ptr$u8));
/// Free mem
extern fn_((mem_Allocator_rawFree(mem_Allocator self, Sli$u8 buf, u32 buf_align))(void));

/*========== High-level Allocator Functions =================================*/

/// Single-item allocation
extern fn_((mem_Allocator_create(mem_Allocator self, TypeInfo type))(mem_Allocator_Err$meta_Ptr)) $must_check;
/// Free single-item
extern fn_((mem_Allocator_destroy(mem_Allocator self, AnyType ptr))(void));
/// Slice allocation
extern fn_((mem_Allocator_alloc(mem_Allocator self, TypeInfo type, usize count))(mem_Allocator_Err$meta_Sli)) $must_check;
/// Try to resize slice in-place
extern fn_((mem_Allocator_resize(mem_Allocator self, AnyType old_mem, usize new_len))(bool));
/// Try to resize slice, allowing relocation
extern fn_((mem_Allocator_remap(mem_Allocator self, AnyType old_mem, usize new_len))(Opt$meta_Sli));
/// Reallocate slice with new size
extern fn_((mem_Allocator_realloc(mem_Allocator self, AnyType old_mem, usize new_len))(mem_Allocator_Err$meta_Sli)) $must_check;
/// Free slice
extern fn_((mem_Allocator_free(mem_Allocator self, AnyType mem))(void));

/*========== Helper Functions ===============================================*/

/// Duplicate slice
extern fn_((mem_Allocator_dupe(mem_Allocator self, meta_Sli src))(mem_Allocator_Err$meta_Sli)) $must_check;
/// Duplicate slice with terminated zero sentinel value
extern fn_((mem_Allocator_dupeZ(mem_Allocator self, meta_Sli src))(mem_Allocator_Err$meta_Sli)) $must_check;

#else /* COMP_TIME && (!COMP_TIME || debug_comp_enabled) */

extern fn_((mem_Allocator_rawAlloc_debug(mem_Allocator self, usize len, u32 align, SrcLoc src_loc))(Opt$Ptr$u8));
extern fn_((mem_Allocator_rawResize_debug(mem_Allocator self, Sli$u8 buf, u32 buf_align, usize new_len, SrcLoc src_loc))(bool));
extern fn_((mem_Allocator_rawRemap_debug(mem_Allocator self, Sli$u8 buf, u32 buf_align, usize new_len, SrcLoc src_loc))(Opt$Ptr$u8));
extern fn_((mem_Allocator_rawFree_debug(mem_Allocator self, Sli$u8 buf, u32 buf_align, SrcLoc src_loc))(void));

extern fn_((mem_Allocator_create_debug(mem_Allocator self, TypeInfo type, SrcLoc src_loc))(mem_Allocator_Err$meta_Ptr)) $must_check;
extern fn_((mem_Allocator_destroy_debug(mem_Allocator self, AnyType ptr, SrcLoc src_loc))(void));
extern fn_((mem_Allocator_alloc_debug(mem_Allocator self, TypeInfo type, usize count, SrcLoc src_loc))(mem_Allocator_Err$meta_Sli)) $must_check;
extern fn_((mem_Allocator_resize_debug(mem_Allocator self, AnyType old_mem, usize new_len, SrcLoc src_loc))(bool));
extern fn_((mem_Allocator_remap_debug(mem_Allocator self, AnyType old_mem, usize new_len, SrcLoc src_loc))(Opt$meta_Sli));
extern fn_((mem_Allocator_realloc_debug(mem_Allocator self, AnyType old_mem, usize new_len, SrcLoc src_loc))(mem_Allocator_Err$meta_Sli)) $must_check;
extern fn_((mem_Allocator_free_debug(mem_Allocator self, AnyType mem, SrcLoc src_loc))(void));

extern fn_((mem_Allocator_dupe_debug(mem_Allocator self, meta_Sli src, SrcLoc src_loc))(mem_Allocator_Err$meta_Sli)) $must_check;
extern fn_((mem_Allocator_dupeZ_debug(mem_Allocator self, meta_Sli src, SrcLoc src_loc))(mem_Allocator_Err$meta_Sli)) $must_check;

/*========== Macros and Declarations ========================================*/

/* Debug versions of raw operations */
#define mem_Allocator_rawAlloc(_self, _len, _align...)                mem_Allocator_rawAlloc_callDebug((_self), (_len), (_align), srcLoc())
#define mem_Allocator_rawResize(_self, _buf, _buf_align, _new_len...) mem_Allocator_rawResize_callDebug((_self), (_buf), (_buf_align), (_new_len), srcLoc())
#define mem_Allocator_rawRemap(_self, _buf, _buf_align, _new_len...)  mem_Allocator_rawRemap_callDebug((_self), (_buf), (_buf_align), (_new_len), srcLoc())
#define mem_Allocator_rawFree(_self, _buf, _buf_align...)             mem_Allocator_rawFree_callDebug((_self), (_buf), (_buf_align), srcLoc())

/* Debug versions of high-level operations */
#define mem_Allocator_create(_self, _type...)                         mem_Allocator_create_callDebug((_self), (_type), srcLoc())
#define mem_Allocator_destroy(_self, _ptr...)                         mem_Allocator_destroy_callDebug((_self), (_ptr), srcLoc())
#define mem_Allocator_alloc(_self, _type, _count...)                  mem_Allocator_alloc_callDebug((_self), (_type), (_count), srcLoc())
#define mem_Allocator_resize(_self, _old_mem, _new_len...)            mem_Allocator_resize_callDebug((_self), (_old_mem), (_new_len), srcLoc())
#define mem_Allocator_remap(_self, _old_mem, _new_len...)             mem_Allocator_remap_callDebug((_self), (_old_mem), (_new_len), srcLoc())
#define mem_Allocator_realloc(_self, _old_mem, _new_len...)           mem_Allocator_realloc_callDebug((_self), (_old_mem), (_new_len), srcLoc())
#define mem_Allocator_free(_self, _mem...)                            mem_Allocator_free_callDebug((_self), (_mem), srcLoc())

/* Debug versions of helper operations */
#define mem_Allocator_dupe(_self, _src...)                            mem_Allocator_dupe_callDebug((_self), (_src), srcLoc())
#define mem_Allocator_dupeZ(_self, _src...)                           mem_Allocator_dupeZ_callDebug((_self), (_src), srcLoc())

/*========== Macros and Definitions =========================================*/

#define mem_Allocator_rawAlloc_callDebug(_self, _len, _align, _src_loc)                mem_Allocator_rawAlloc_debug(_self, _len, _align, _src_loc)
#define mem_Allocator_rawResize_callDebug(_self, _buf, _buf_align, _new_len, _src_loc) mem_Allocator_rawResize_debug(_self, _buf, _buf_align, _new_len, _src_loc)
#define mem_Allocator_rawRemap_callDebug(_self, _buf, _buf_align, _new_len, _src_loc)  mem_Allocator_rawRemap_debug(_self, _buf, _buf_align, _new_len, _src_loc)
#define mem_Allocator_rawFree_callDebug(_self, _buf, _buf_align, _src_loc)             mem_Allocator_rawFree_debug(_self, _buf, _buf_align, _src_loc)

#define mem_Allocator_create_callDebug(_self, _type, _src_loc)               mem_Allocator_create_debug(_self, _type, _src_loc)
#define mem_Allocator_destroy_callDebug(_self, _ptr, _src_loc)               mem_Allocator_destroy_debug(_self, _ptr, _src_loc)
#define mem_Allocator_alloc_callDebug(_self, _type, _count, _src_loc)        mem_Allocator_alloc_debug(_self, _type, _count, _src_loc)
#define mem_Allocator_resize_callDebug(_self, _old_mem, _new_len, _src_loc)  mem_Allocator_resize_debug(_self, _old_mem, _new_len, _src_loc)
#define mem_Allocator_remap_callDebug(_self, _old_mem, _new_len, _src_loc)   mem_Allocator_remap_debug(_self, _old_mem, _new_len, _src_loc)
#define mem_Allocator_realloc_callDebug(_self, _old_mem, _new_len, _src_loc) mem_Allocator_realloc_debug(_self, _old_mem, _new_len, _src_loc)
#define mem_Allocator_free_callDebug(_self, _mem, _src_loc)                  mem_Allocator_free_debug(_self, _mem, _src_loc)

#define mem_Allocator_dupe_callDebug(_self, _src, _src_loc)  mem_Allocator_dupe_debug(_self, _src, _src_loc)
#define mem_Allocator_dupeZ_callDebug(_self, _src, _src_loc) mem_Allocator_dupeZ_debug(_self, _src, _src_loc)

#endif /* COMP_TIME && (!COMP_TIME || debug_comp_enabled) */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* MEM_ALLOCATOR_INCLUDED */

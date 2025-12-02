/**
 * @copyright Copyright (c) 2025 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    ArrSet.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2025-11-22 (date of creation)
 * @updated 2025-12-01 (date of last update)
 * @version v0.1
 * @ingroup dasae-headers(dh)
 * @prefix  ArrSet
 *
 * @brief   Dynamic array hash set implementation
 * @details This header provides a hash set implementation with dynamic memory allocation.
 *          The set maintains insertion order and uses a hash index for fast lookups.
 *          For small sets (len < 8), linear scanning is used instead of hash index.
 *          Supports operations for initialization, modification, and element access.
 */
#ifndef ArrSet__included
#define ArrSet__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "prl.h"
#include "mem/Allocator.h"

/*========== Macros and Declarations ========================================*/

/* ArrSet Anonymous */
#define ArrSet$$(_T...) __comp_anon__ArrSet$$(_T)
/* ArrSet Alias */
#define ArrSet$(_T...) __comp_alias__ArrSet$(_T)
/* ArrSet Template */
#define T_decl_ArrSet$(_T...) __comp_gen__T_decl_ArrSet$(_T)
#define T_impl_ArrSet$(_T...) __comp_gen__T_impl_ArrSet$(_T)
#define T_use_ArrSet$(_T...) __comp_gen__T_use_ArrSet$(_T)

/* Hash function context type */
typedef fn_(((*)(u_V$raw item, u_V$raw ctx))(u64) $T) u_HashCtxFn;

/* ArrSet Raw Structure */
typedef struct ArrSet {
    var_(items, S$raw);
    var_(cap, usize);
    var_(index, u_P$raw); // Hash index (null if len < 8)
    var_(hashFn, u_HashCtxFn);
    var_(eqlFn, u_EqlCtxFn);
    var_(ctx, u_P_const$raw);
    debug_only(var_(type, TypeInfo);)
} ArrSet;
T_use$((ArrSet)(O, E));
T_use_E$($set(mem_Err)(ArrSet));
typedef E$$($set(mem_Err)(bool)) ArrSet_mem_Err$bool;

/*========== Function Prototypes ============================================*/

/* Initialization and cleanup */
extern fn_((ArrSet_empty(TypeInfo type, u_HashCtxFn hashFn, u_EqlCtxFn eqlFn, u_P_const$raw ctx))(ArrSet));
extern fn_((ArrSet_fromBuf(u_S$raw buf, u_HashCtxFn hashFn, u_EqlCtxFn eqlFn, u_P_const$raw ctx))(ArrSet));
extern fn_((ArrSet_init(TypeInfo type, mem_Allocator gpa, usize cap, u_HashCtxFn hashFn, u_EqlCtxFn eqlFn, u_P_const$raw ctx))(mem_Err$ArrSet)) $must_check;
extern fn_((ArrSet_fini(ArrSet* self, TypeInfo type, mem_Allocator gpa))(void));
extern fn_((ArrSet_clone(ArrSet self, TypeInfo type, mem_Allocator gpa))(mem_Err$ArrSet)) $must_check;

/* Access operations */
extern fn_((ArrSet_len(ArrSet self))(usize));
extern fn_((ArrSet_cap(ArrSet self))(usize));
extern fn_((ArrSet_contains(ArrSet self, u_V$raw item))(bool));
extern fn_((ArrSet_get(ArrSet self, u_V$raw item))(O$u_P_const$raw));
extern fn_((ArrSet_getMut(ArrSet self, u_V$raw item))(O$u_P$raw));
extern fn_((ArrSet_getIndex(ArrSet self, u_V$raw item))(O$usize));
extern fn_((ArrSet_at(ArrSet self, TypeInfo type, usize idx))(u_P_const$raw));
extern fn_((ArrSet_atMut(ArrSet self, TypeInfo type, usize idx))(u_P$raw));
extern fn_((ArrSet_items(ArrSet self, TypeInfo type))(u_S_const$raw));
extern fn_((ArrSet_itemsMut(ArrSet self, TypeInfo type))(u_S$raw));
extern fn_((ArrSet_itemsCapped(ArrSet self, TypeInfo type))(u_S_const$raw));
extern fn_((ArrSet_itemsCappedMut(ArrSet self, TypeInfo type))(u_S$raw));
extern fn_((ArrSet_itemsUnused(ArrSet self, TypeInfo type))(u_S_const$raw));
extern fn_((ArrSet_itemsUnusedMut(ArrSet self, TypeInfo type))(u_S$raw));

/* Capacity management */
extern fn_((ArrSet_ensureCap(ArrSet* self, TypeInfo type, mem_Allocator gpa, usize new_cap))(mem_Err$void)) $must_check;
extern fn_((ArrSet_ensureCapPrecise(ArrSet* self, TypeInfo type, mem_Allocator gpa, usize new_cap))(mem_Err$void)) $must_check;
extern fn_((ArrSet_ensureUnusedCap(ArrSet* self, TypeInfo type, mem_Allocator gpa, usize additional))(mem_Err$void)) $must_check;
extern fn_((ArrSet_shrinkRetainingCap(ArrSet* self, usize new_len))(void));
extern fn_((ArrSet_shrinkAndFree(ArrSet* self, TypeInfo type, mem_Allocator gpa, usize new_len))(void));
extern fn_((ArrSet_clearRetainingCap(ArrSet* self))(void));
extern fn_((ArrSet_clearAndFree(ArrSet* self, TypeInfo type, mem_Allocator gpa))(void));

/* Insertion operations - three-tier pattern */
extern fn_((ArrSet_insert(ArrSet* self, mem_Allocator gpa, u_V$raw item))(ArrSet_mem_Err$bool)) $must_check;
extern fn_((ArrSet_insertFixed(ArrSet* self, u_V$raw item))(ArrSet_mem_Err$bool)) $must_check;
extern fn_((ArrSet_insertWithin(ArrSet* self, u_V$raw item))(bool));

/* GetOrPut pattern - returns pointer to item and whether it was newly inserted */
typedef struct ArrSet_GetOrPutResult {
    var_(item_ptr, u_P$raw);
    var_(found_existing, bool);
    var_(index, usize);
} ArrSet_GetOrPutResult;
T_use$((ArrSet_GetOrPutResult)(O, E));
T_use_E$($set(mem_Err)(ArrSet_GetOrPutResult));

extern fn_((ArrSet_getOrPut(ArrSet* self, mem_Allocator gpa, u_V$raw item))(mem_Err$ArrSet_GetOrPutResult)) $must_check;
extern fn_((ArrSet_getOrPutFixed(ArrSet* self, u_V$raw item))(mem_Err$ArrSet_GetOrPutResult)) $must_check;
extern fn_((ArrSet_getOrPutWithin(ArrSet* self, u_V$raw item))(ArrSet_GetOrPutResult));

/* Removal operations */
extern fn_((ArrSet_remove(ArrSet* self, u_V$raw item))(bool));
extern fn_((ArrSet_removeOrd(ArrSet* self, usize idx, u_V$raw ret_mem))(u_V$raw));
extern fn_((ArrSet_removeSwap(ArrSet* self, usize idx, u_V$raw ret_mem))(u_V$raw));

/* Fetch operations - get and remove atomically */
extern fn_((ArrSet_fetch(ArrSet* self, u_V$raw item, u_V$raw ret_mem))(O$u_V$raw));
extern fn_((ArrSet_fetchOrd(ArrSet* self, u_V$raw item, u_V$raw ret_mem))(O$u_V$raw));
extern fn_((ArrSet_fetchSwap(ArrSet* self, u_V$raw item, u_V$raw ret_mem))(O$u_V$raw));

/* Set operations */
extern fn_((ArrSet_unionWith(ArrSet* self, mem_Allocator gpa, ArrSet other))(mem_Err$void)) $must_check;
extern fn_((ArrSet_intersectWith(ArrSet* self, ArrSet other))(void));
extern fn_((ArrSet_differenceWith(ArrSet* self, ArrSet other))(void));

/* Utility operations */
extern fn_((ArrSet_reIndex(ArrSet* self, TypeInfo type, mem_Allocator gpa))(mem_Err$void)) $must_check;
extern fn_((ArrSet_sort(ArrSet* self, TypeInfo type, u_OrdCtxFn ordFn, u_P_const$raw ord_ctx))(void));

/* ArrSet_Iter Anonymous */
#define ArrSet_Iter$$(_T...) __comp_anon__ArrSet_Iter$$(_T)
/* ArrSet_Iter Alias */
#define ArrSet_Iter$(_T...) __comp_alias__ArrSet_Iter$(_T)
/* ArrSet_Iter Template */
#define T_decl_ArrSet_Iter$(_T...) __comp_gen__T_decl_ArrSet_Iter$(_T)
#define T_impl_ArrSet_Iter$(_T...) __comp_gen__T_impl_ArrSet_Iter$(_T)
#define T_use_ArrSet_Iter$(_T...) __comp_gen__T_use_ArrSet_Iter$(_T)

/* ArrSet_Iter Raw Structure */
typedef struct ArrSet_Iter {
    var_(set, const ArrSet*);
    var_(index, usize);
    debug_only(var_(type, TypeInfo);)
} ArrSet_Iter;
extern fn_((ArrSet_iter(const ArrSet* self, TypeInfo type))(ArrSet_Iter));
extern fn_((ArrSet_Iter_next(ArrSet_Iter* self, TypeInfo type))(O$u_P_const$raw));
extern fn_((ArrSet_Iter_nextMut(ArrSet_Iter* self, TypeInfo type))(O$u_P$raw));

/*========== Macros and Definitions =========================================*/

#define __comp_anon__ArrSet$$(_T...) \
    union { \
        struct { \
            var_(items, S$$(_T)); \
            var_(cap, usize); \
            var_(index, u_P$raw); \
            var_(hashFn, u_HashCtxFn); \
            var_(eqlFn, u_EqlCtxFn); \
            var_(ctx, u_P_const$raw); \
            debug_only(var_(type, TypeInfo);) \
        }; \
        var_(as_raw $like_ref, ArrSet); \
    }
#define __comp_alias__ArrSet$(_T...) pp_join($, ArrSet, _T)
#define __comp_gen__T_decl_ArrSet$(_T...) \
    $maybe_unused typedef union ArrSet$(_T) ArrSet$(_T); \
    T_decl_O$(ArrSet$(_T)); \
    T_decl_E$(ArrSet$(_T)); \
    T_decl_E$($set(mem_Err)(ArrSet$(_T)))
#define __comp_gen__T_impl_ArrSet$(_T...) \
    union ArrSet$(_T) { \
        struct { \
            var_(items, S$(_T)); \
            var_(cap, usize); \
            var_(index, u_P$raw); \
            var_(hashFn, u_HashCtxFn); \
            var_(eqlFn, u_EqlCtxFn); \
            var_(ctx, u_P_const$raw); \
            debug_only(var_(type, TypeInfo);) \
        }; \
        var_(as_raw $like_ref, ArrSet); \
    }; \
    T_impl_O$(ArrSet$(_T)); \
    T_impl_E$(ArrSet$(_T)); \
    T_impl_E$($set(mem_Err)(ArrSet$(_T)))
#define __comp_gen__T_use_ArrSet$(_T...) \
    T_decl_ArrSet$(_T); \
    T_impl_ArrSet$(_T)

#define __comp_anon__ArrSet_Iter$$(_T...) \
    union { \
        struct { \
            var_(set, P_const$$(ArrSet$$(_T))); \
            var_(index, usize); \
            debug_only(var_(type, TypeInfo);) \
        }; \
        var_(as_raw $like_ref, ArrSet_Iter); \
    }
#define __comp_alias__ArrSet_Iter$(_T...) pp_join($, ArrSet_Iter, _T)
#define __comp_gen__T_decl_ArrSet_Iter$(_T...) \
    $maybe_unused typedef union ArrSet_Iter$(_T) ArrSet_Iter$(_T)
#define __comp_gen__T_impl_ArrSet_Iter$(_T...) \
    union ArrSet_Iter$(_T) { \
        struct { \
            var_(set, P_const$$(ArrSet$(_T))); \
            var_(index, usize); \
            debug_only(var_(type, TypeInfo);) \
        }; \
        var_(as_raw $like_ref, ArrSet_Iter); \
    }
#define __comp_gen__T_use_ArrSet_Iter$(_T...) \
    T_decl_ArrSet_Iter$(_T); \
    T_impl_ArrSet_Iter$(_T)

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* ArrSet__included */

/**
 * @copyright Copyright (c) 2025 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    ArrMap.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2025-01-22 (date of creation)
 * @updated 2025-01-22 (date of last update)
 * @version v0.1-alpha
 * @ingroup dasae-headers(dh)
 * @prefix  ArrMap
 *
 * @brief   Dynamic array hash map implementation
 * @details This header provides a hash map implementation with dynamic memory allocation.
 *          The map maintains insertion order and uses a hash index for fast lookups.
 *          For small maps (len < 8), linear scanning is used instead of hash index.
 *          Supports operations for initialization, modification, and element access.
 */
#ifndef ArrMap__included
#define ArrMap__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "prl.h"
#include "mem/Allocator.h"
#include "ArrSet.h" // For u_HashCtxFn definition

/*========== Macros and Declarations ========================================*/

/* ArrMap Anonymous */
#define ArrMap$$(_K, _V...) __comp_anon__ArrMap$$(_K, _V)
/* ArrMap Alias */
#define ArrMap$(_K, _V...) __comp_alias__ArrMap$(_K, _V)
/* ArrMap Template */
#define T_decl_ArrMap$(_K, _V...) __comp_gen__T_decl_ArrMap$(_K, _V)
#define T_impl_ArrMap$(_K, _V...) __comp_gen__T_impl_ArrMap$(_K, _V)
#define T_use_ArrMap$(_K, _V...) __comp_gen__T_use_ArrMap$(_K, _V)

typedef struct ArrMap_Index {
    var_(hash, u64);
    var_(index, usize);
} ArrMap_Index;
T_use$((ArrMap_Index)(O, E));

/* ArrMap Raw Structure */
typedef struct ArrMap {
    var_(keys, S$raw);
    var_(vals, S$raw);
    var_(cap, usize);
    var_(index, O$ArrMap_Index); // Hash index (null if len < 8)
    var_(hashFn, u_HashCtxFn);
    var_(eqlFn, u_EqlCtxFn);
    var_(ctx, u_P_const$raw);
    debug_only(var_(key_type, TypeInfo);)
    debug_only(var_(val_type, TypeInfo);)
} ArrMap;
T_use$((ArrMap)(O, E));
T_use_E$($set(mem_Err)(ArrMap));
typedef E$$($set(mem_Err)(bool)) ArrMap_mem_Err$bool;

/* ArrMap_KV - Key-Value pair structure */
typedef struct ArrMap_KV {
    var_(key, u_V$raw);
    var_(val, u_V$raw);
} ArrMap_KV;
T_use$((ArrMap_KV)(O));

/*========== Function Prototypes ============================================*/

/* Initialization and cleanup */
extern fn_((ArrMap_empty(TypeInfo key_type, TypeInfo val_type, u_HashCtxFn hashFn, u_EqlCtxFn eqlFn, u_P_const$raw ctx))(ArrMap));
extern fn_((ArrMap_fromBufs(u_S$raw keys_buf, u_S$raw vals_buf, u_HashCtxFn hashFn, u_EqlCtxFn eqlFn, u_P_const$raw ctx))(ArrMap));
extern fn_((ArrMap_init(TypeInfo key_type, TypeInfo val_type, mem_Allocator gpa, usize cap, u_HashCtxFn hashFn, u_EqlCtxFn eqlFn, u_P_const$raw ctx))(mem_Err$ArrMap)) $must_check;
extern fn_((ArrMap_fini(ArrMap* self, TypeInfo key_type, TypeInfo val_type, mem_Allocator gpa))(void));
extern fn_((ArrMap_clone(ArrMap self, TypeInfo key_type, TypeInfo val_type, mem_Allocator gpa))(mem_Err$ArrMap)) $must_check;
extern fn_((ArrMap_move(ArrMap* self))(ArrMap));

/* Access operations */
extern fn_((ArrMap_len(ArrMap self))(usize));
extern fn_((ArrMap_cap(ArrMap self))(usize));
extern fn_((ArrMap_contains(ArrMap self, u_V$raw key))(bool));
extern fn_((ArrMap_get(ArrMap self, u_V$raw key))(O$u_P_const$raw));
extern fn_((ArrMap_getMut(ArrMap self, u_V$raw key))(O$u_P$raw));
extern fn_((ArrMap_getIndex(ArrMap self, u_V$raw key))(O$usize));
extern fn_((ArrMap_atKey(ArrMap self, TypeInfo key_type, usize idx))(u_P_const$raw));
extern fn_((ArrMap_atKeyMut(ArrMap self, TypeInfo key_type, usize idx))(u_P$raw));
extern fn_((ArrMap_atVal(ArrMap self, TypeInfo val_type, usize idx))(u_P_const$raw));
extern fn_((ArrMap_atValMut(ArrMap self, TypeInfo val_type, usize idx))(u_P$raw));
extern fn_((ArrMap_keys(ArrMap self, TypeInfo key_type))(u_S_const$raw));
extern fn_((ArrMap_keysMut(ArrMap self, TypeInfo key_type))(u_S$raw));
extern fn_((ArrMap_vals(ArrMap self, TypeInfo val_type))(u_S_const$raw));
extern fn_((ArrMap_valsMut(ArrMap self, TypeInfo val_type))(u_S$raw));
extern fn_((ArrMap_keysCapped(ArrMap self, TypeInfo key_type))(u_S_const$raw));
extern fn_((ArrMap_keysCappedMut(ArrMap self, TypeInfo key_type))(u_S$raw));
extern fn_((ArrMap_valsCapped(ArrMap self, TypeInfo val_type))(u_S_const$raw));
extern fn_((ArrMap_valsCappedMut(ArrMap self, TypeInfo val_type))(u_S$raw));
extern fn_((ArrMap_keysUnused(ArrMap self, TypeInfo key_type))(u_S_const$raw));
extern fn_((ArrMap_keysUnusedMut(ArrMap self, TypeInfo key_type))(u_S$raw));
extern fn_((ArrMap_valsUnused(ArrMap self, TypeInfo val_type))(u_S_const$raw));
extern fn_((ArrMap_valsUnusedMut(ArrMap self, TypeInfo val_type))(u_S$raw));

/* Capacity management */
extern fn_((ArrMap_ensureCap(ArrMap* self, TypeInfo key_type, TypeInfo val_type, mem_Allocator gpa, usize new_cap))(mem_Err$void)) $must_check;
extern fn_((ArrMap_ensureCapPrecise(ArrMap* self, TypeInfo key_type, TypeInfo val_type, mem_Allocator gpa, usize new_cap))(mem_Err$void)) $must_check;
extern fn_((ArrMap_ensureUnusedCap(ArrMap* self, TypeInfo key_type, TypeInfo val_type, mem_Allocator gpa, usize additional))(mem_Err$void)) $must_check;
extern fn_((ArrMap_shrinkRetainingCap(ArrMap* self, usize new_len))(void));
extern fn_((ArrMap_shrinkAndFree(ArrMap* self, TypeInfo key_type, TypeInfo val_type, mem_Allocator gpa, usize new_len))(void));
extern fn_((ArrMap_clearRetainingCap(ArrMap* self))(void));
extern fn_((ArrMap_clearAndFree(ArrMap* self, TypeInfo key_type, TypeInfo val_type, mem_Allocator gpa))(void));

/* Insertion operations - three-tier pattern */
extern fn_((ArrMap_put(ArrMap* self, mem_Allocator gpa, u_V$raw key, u_V$raw val))(ArrMap_mem_Err$bool)) $must_check;
extern fn_((ArrMap_putFixed(ArrMap* self, u_V$raw key, u_V$raw val))(ArrMap_mem_Err$bool)) $must_check;
extern fn_((ArrMap_putWithin(ArrMap* self, u_V$raw key, u_V$raw val))(ArrMap_mem_Err$bool));

/* GetOrPut pattern - returns pointers to key and value, and whether it was newly inserted */
typedef struct ArrMap_GetOrPutResult {
    var_(key_ptr, u_P$raw);
    var_(val_ptr, u_P$raw);
    var_(found_existing, bool);
    var_(index, usize);
} ArrMap_GetOrPutResult;
T_use$((ArrMap_GetOrPutResult)(O, E));
T_use_E$($set(mem_Err)(ArrMap_GetOrPutResult));

extern fn_((ArrMap_getOrPut(ArrMap* self, mem_Allocator gpa, u_V$raw key))(mem_Err$ArrMap_GetOrPutResult)) $must_check;
extern fn_((ArrMap_getOrPutFixed(ArrMap* self, u_V$raw key))(mem_Err$ArrMap_GetOrPutResult)) $must_check;
extern fn_((ArrMap_getOrPutWithin(ArrMap* self, u_V$raw key))(ArrMap_GetOrPutResult));

/* Removal operations */
extern fn_((ArrMap_remove(ArrMap* self, u_V$raw key))(bool));
extern fn_((ArrMap_removeOrd(ArrMap* self, usize idx, ArrMap_KV* ret_mem))(ArrMap_KV));
extern fn_((ArrMap_removeSwap(ArrMap* self, usize idx, ArrMap_KV* ret_mem))(ArrMap_KV));

/* Fetch operations - get value and remove entry atomically */
extern fn_((ArrMap_fetch(ArrMap* self, u_V$raw key, u_V$raw ret_mem))(O$u_V$raw));
extern fn_((ArrMap_fetchOrd(ArrMap* self, u_V$raw key, u_V$raw ret_mem))(O$u_V$raw));
extern fn_((ArrMap_fetchSwap(ArrMap* self, u_V$raw key, u_V$raw ret_mem))(O$u_V$raw));

/* Fetch key-value pair operations */
extern fn_((ArrMap_fetchKV(ArrMap* self, u_V$raw key, ArrMap_KV* ret_mem))(O$ArrMap_KV));
extern fn_((ArrMap_fetchKVOrd(ArrMap* self, u_V$raw key, ArrMap_KV* ret_mem))(O$ArrMap_KV));
extern fn_((ArrMap_fetchKVSwap(ArrMap* self, u_V$raw key, ArrMap_KV* ret_mem))(O$ArrMap_KV));

/* Utility operations */
extern fn_((ArrMap_reIndex(ArrMap* self, TypeInfo key_type, mem_Allocator gpa))(mem_Err$void)) $must_check;
extern fn_((ArrMap_sort(ArrMap* self, TypeInfo key_type, TypeInfo val_type, u_OrdCtxFn ordFn, u_P_const$raw ord_ctx))(void));

/* ArrMap_Iter Anonymous */
#define ArrMap_Iter$$(_K, _V...) __comp_anon__ArrMap_Iter$$(_K, _V)
/* ArrMap_Iter Alias */
#define ArrMap_Iter$(_K, _V...) __comp_alias__ArrMap_Iter$(_K, _V)
/* ArrMap_Iter Template */
#define T_decl_ArrMap_Iter$(_K, _V...) __comp_gen__T_decl_ArrMap_Iter$(_K, _V)
#define T_impl_ArrMap_Iter$(_K, _V...) __comp_gen__T_impl_ArrMap_Iter$(_K, _V)
#define T_use_ArrMap_Iter$(_K, _V...) __comp_gen__T_use_ArrMap_Iter$(_K, _V)

/* ArrMap_Iter Raw Structure */
typedef struct ArrMap_Iter {
    var_(map, const ArrMap*);
    var_(index, usize);
    debug_only(var_(key_type, TypeInfo);)
    debug_only(var_(val_type, TypeInfo);)
} ArrMap_Iter;
extern fn_((ArrMap_iter(const ArrMap* self, TypeInfo key_type, TypeInfo val_type))(ArrMap_Iter));
extern fn_((ArrMap_Iter_next(ArrMap_Iter* self, TypeInfo key_type, TypeInfo val_type, ArrMap_KV* ret_mem))(O$ArrMap_KV));

/* ArrMap_KeyIter - Iterator over keys only */
typedef struct ArrMap_KeyIter {
    var_(map, const ArrMap*);
    var_(index, usize);
    debug_only(var_(key_type, TypeInfo);)
} ArrMap_KeyIter;
extern fn_((ArrMap_keyIter(const ArrMap* self, TypeInfo key_type))(ArrMap_KeyIter));
extern fn_((ArrMap_KeyIter_next(ArrMap_KeyIter* self, TypeInfo key_type))(O$u_P_const$raw));

/* ArrMap_ValIter - Iterator over values only */
typedef struct ArrMap_ValIter {
    var_(map, const ArrMap*);
    var_(index, usize);
    debug_only(var_(val_type, TypeInfo);)
} ArrMap_ValIter;
extern fn_((ArrMap_valIter(const ArrMap* self, TypeInfo val_type))(ArrMap_ValIter));
extern fn_((ArrMap_ValIter_next(ArrMap_ValIter* self, TypeInfo val_type))(O$u_P_const$raw));
extern fn_((ArrMap_ValIter_nextMut(ArrMap_ValIter* self, TypeInfo val_type))(O$u_P$raw));

/*========== Macros and Definitions =========================================*/

#define __comp_anon__ArrMap$$(_K, _V...) \
    union { \
        struct { \
            var_(keys, S$$(_K)); \
            var_(vals, S$$(_V)); \
            var_(cap, usize); \
            var_(index, u_P$raw); \
            var_(hashFn, u_HashCtxFn); \
            var_(eqlFn, u_EqlCtxFn); \
            var_(ctx, u_P_const$raw); \
            debug_only(var_(key_type, TypeInfo);) debug_only(var_(val_type, TypeInfo);) \
        }; \
        var_(as_raw $like_ref, ArrMap); \
    }
#define __comp_alias__ArrMap$(_K, _V...) pp_join($, ArrMap, _K, _V)
#define __comp_gen__T_decl_ArrMap$(_K, _V...) \
    $maybe_unused typedef union ArrMap$(_K, _V) ArrMap$(_K, _V); \
    T_decl_O$(ArrMap$(_K, _V)); \
    T_decl_E$(ArrMap$(_K, _V)); \
    T_decl_E$($set(mem_Err)(ArrMap$(_K, _V)))
#define __comp_gen__T_impl_ArrMap$(_K, _V...) \
    union ArrMap$(_K, _V) { \
        struct { \
            var_(keys, S$(_K)); \
            var_(vals, S$(_V)); \
            var_(cap, usize); \
            var_(index, u_P$raw); \
            var_(hashFn, u_HashCtxFn); \
            var_(eqlFn, u_EqlCtxFn); \
            var_(ctx, u_P_const$raw); \
            debug_only(var_(key_type, TypeInfo);) debug_only(var_(val_type, TypeInfo);) \
        }; \
        var_(as_raw $like_ref, ArrMap); \
    }; \
    T_impl_O$(ArrMap$(_K, _V)); \
    T_impl_E$(ArrMap$(_K, _V)); \
    T_impl_E$($set(mem_Err)(ArrMap$(_K, _V)))
#define __comp_gen__T_use_ArrMap$(_K, _V...) \
    T_decl_ArrMap$(_K, _V); \
    T_impl_ArrMap$(_K, _V)

#define __comp_anon__ArrMap_Iter$$(_K, _V...) \
    union { \
        struct { \
            var_(map, P_const$$(ArrMap$$(_K, _V))); \
            var_(index, usize); \
            debug_only(var_(key_type, TypeInfo);) debug_only(var_(val_type, TypeInfo);) \
        }; \
        var_(as_raw $like_ref, ArrMap_Iter); \
    }
#define __comp_alias__ArrMap_Iter$(_K, _V...) pp_join($, ArrMap_Iter, _K, _V)
#define __comp_gen__T_decl_ArrMap_Iter$(_K, _V...) \
    $maybe_unused typedef union ArrMap_Iter$(_K, _V) ArrMap_Iter$(_K, _V)
#define __comp_gen__T_impl_ArrMap_Iter$(_K, _V...) \
    union ArrMap_Iter$(_K, _V) { \
        struct { \
            var_(map, P_const$$(ArrMap$$(_K, _V))); \
            var_(index, usize); \
            debug_only(var_(key_type, TypeInfo);) debug_only(var_(val_type, TypeInfo);) \
        }; \
        var_(as_raw $like_ref, ArrMap_Iter); \
    }
#define __comp_gen__T_use_ArrMap_Iter$(_K, _V...) \
    T_decl_ArrMap_Iter$(_K, _V); \
    T_impl_ArrMap_Iter$(_K, _V)

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* ArrMap__included */

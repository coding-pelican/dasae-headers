/**
 * @copyright Copyright (c) 2025 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    HashSet.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2025-12-08 (date of creation)
 * @updated 2025-12-09 (date of last update)
 * @version v0.1
 * @ingroup dasae-headers(dh)
 * @prefix  HashSet
 *
 * @brief   High-performance unordered hash set
 * @details Swiss Table inspired design with single allocation and excellent cache locality.
 *          Control bytes (1 byte per slot) for fast SIMD lookup.
 *          Keys stored in contiguous array (no values - set semantics).
 *          Average O(1) insertion, lookup, and removal.
 */
#ifndef HashSet__included
#define HashSet__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "dh/prl.h"
#include "dh/mem/Allocator.h"
#include "dh/HashMap.h" /* Reuse HashMap_Ctrl, HashMap_Ctx */

/*========== Macros and Declarations ========================================*/

/* --- HashSet_Header: Internal header stored before metadata --- */

#define HashSet_Header$$(_K...) __comp_anon__HashSet_Header$$(_K)
#define HashSet_Header$(_K...) __comp_alias__HashSet_Header$(_K)
#define T_decl_HashSet_Header$(_K...) __comp_gen__T_decl_HashSet_Header$(_K)
#define T_impl_HashSet_Header$(_K...) __comp_gen__T_impl_HashSet_Header$(_K)
#define T_use_HashSet_Header$(_K...) __comp_gen__T_use_HashSet_Header$(_K)

typedef struct HashSet_Header {
    var_(keys, P$raw);
    var_(cap, u32);
    debug_only(var_(key_ty, TypeInfo));
} HashSet_Header;

/* --- HashSet_Sgl: Key single type (1-tuple) */

#define HashSet_Sgl$$(_K...) __comp_anon__HashSet_Sgl$$(_K)
#define HashSet_Sgl$(_K...) __comp_alias__HashSet_Sgl$(_K)
#define T_decl_HashSet_Sgl$(_K...) __comp_gen__T_decl_HashSet_Sgl$(_K)
#define T_impl_HashSet_Sgl$(_K...) __comp_gen__T_impl_HashSet_Sgl$(_K)
#define T_use_HashSet_Sgl$(_K...) __comp_gen__T_use_HashSet_Sgl$(_K)

typedef struct HashSet_Sgl$raw {
    debug_only(var_(key_ty, TypeInfo));
    var_(data, V$raw);
} HashSet_Sgl$raw;
T_use_P$(HashSet_Sgl$raw);
typedef P$HashSet_Sgl$raw V$HashSet_Sgl$raw;
T_use_O$(V$HashSet_Sgl$raw);
T_use_E$($set(mem_Err)(O$V$HashSet_Sgl$raw));
$extern fn_((HashSet_Sgl_key(V$HashSet_Sgl$raw self, u_V$raw ret_mem))(u_V$raw));

/* --- HashSet_Entry: Pointer to key in set --- */

#define HashSet_Entry$$(_K...) __comp_anon__HashSet_Entry$$(_K)
#define HashSet_Entry$(_K...) __comp_alias__HashSet_Entry$(_K)
#define T_decl_HashSet_Entry$(_K...) __comp_gen__T_decl_HashSet_Entry$(_K)
#define T_impl_HashSet_Entry$(_K...) __comp_gen__T_impl_HashSet_Entry$(_K)
#define T_use_HashSet_Entry$(_K...) __comp_gen__T_use_HashSet_Entry$(_K)

typedef struct HashSet_Entry {
    var_(key, P_const$raw);
    debug_only(var_(key_ty, TypeInfo));
} HashSet_Entry;
T_use_O$(HashSet_Entry);
$extern fn_((HashSet_Entry_key(HashSet_Entry self, TypeInfo key_ty))(u_P_const$raw));

#define HashSet_EntryMut$$(_K...) __comp_anon__HashSet_EntryMut$$(_K)
#define HashSet_EntryMut$(_K...) __comp_alias__HashSet_EntryMut$(_K)
#define T_decl_HashSet_EntryMut$(_K...) __comp_gen__T_decl_HashSet_EntryMut$(_K)
#define T_impl_HashSet_EntryMut$(_K...) __comp_gen__T_impl_HashSet_EntryMut$(_K)
#define T_use_HashSet_EntryMut$(_K...) __comp_gen__T_use_HashSet_EntryMut$(_K)

typedef union HashSet_EntryMut {
    struct {
        var_(key, P$raw);
        debug_only(var_(key_ty, TypeInfo));
    };
    var_(as_const, HashSet_Entry);
} HashSet_EntryMut;
T_use_O$(HashSet_EntryMut);
$extern fn_((HashSet_EntryMut_key(HashSet_EntryMut self, TypeInfo key_ty))(u_P$raw));

/* --- HashSet_Ensured: Result from ensure operations --- */

#define HashSet_Ensured$$(_K...) __comp_anon__HashSet_Ensured$$(_K)
#define HashSet_Ensured$(_K...) __comp_alias__HashSet_Ensured$(_K)
#define T_decl_HashSet_Ensured$(_K...) __comp_gen__T_decl_HashSet_Ensured$(_K)
#define T_impl_HashSet_Ensured$(_K...) __comp_gen__T_impl_HashSet_Ensured$(_K)
#define T_use_HashSet_Ensured$(_K...) __comp_gen__T_use_HashSet_Ensured$(_K)

typedef struct HashSet_Ensured {
    var_(key, P$raw);
    var_(found_existing, bool);
    debug_only(var_(key_ty, TypeInfo));
} HashSet_Ensured;
T_use_E$($set(mem_Err)(HashSet_Ensured));
$extern fn_((HashSet_Ensured_key(HashSet_Ensured self, TypeInfo key_ty))(u_P_const$raw));
$extern fn_((HashSet_Ensured_keyMut(HashSet_Ensured self, TypeInfo key_ty))(u_P$raw));
$extern fn_((HashSet_Ensured_foundExisting(HashSet_Ensured self, TypeInfo key_ty))(O$HashSet_Entry));
$extern fn_((HashSet_Ensured_foundExistingMut(HashSet_Ensured self, TypeInfo key_ty))(O$HashSet_EntryMut));

/* --- HashSet_Ctx: Context for hash and equality functions (reuse HashMap_Ctx) --- */

typedef HashMap_HashFn HashSet_HashFn;
$extern fn_((HashSet_HashFn_default(u_V$raw val, u_V$raw ctx))(u64));
claim_assert_static(Type_eq$(HashSet_HashFn, TypeOf(&HashSet_HashFn_default)));
typedef u_EqlCtxFn HashSet_EqlFn;
$extern fn_((HashSet_EqlFn_default(u_V$raw lhs, u_V$raw rhs, u_V$raw ctx))(bool));
claim_assert_static(Type_eq$(HashSet_EqlFn, TypeOf(&HashSet_EqlFn_default)));

typedef HashMap_Ctx HashSet_Ctx;
T_use_P$(HashSet_Ctx);
$extern fn_((HashSet_Ctx_default(void))(P_const$HashSet_Ctx));

/* --- HashSet: Main hash set structure --- */

#define HashSet$$(_K...) __comp_anon__HashSet$$(_K)
#define HashSet$(_K...) __comp_alias__HashSet$(_K)
#define T_decl_HashSet$(_K...) __comp_gen__T_decl_HashSet$(_K)
#define T_impl_HashSet$(_K...) __comp_gen__T_impl_HashSet$(_K)
#define T_use_HashSet$(_K...) __comp_gen__T_use_HashSet$(_K)

typedef struct HashSet {
    /// Pointer to metadata array. At -sizeOf$(HashSet_Header) is Header field.
    var_(metadata, O$P$HashMap_Ctrl);
    /// Current number of elements.
    var_(size, u32);
    /// Number of available slots before grow is needed.
    var_(available, u32);
    /// Context containing hash and equality functions.
    var_(ctx, P_const$HashSet_Ctx);
    debug_only(var_(key_ty, TypeInfo));
} HashSet;
T_use$((HashSet)(O, E));
T_use_E$($set(mem_Err)(HashSet));
#define HashSet_default_max_load_ratio HashMap_default_max_load_ratio
#define HashSet_default_min_cap HashMap_default_min_cap

/* --- Construction/Destruction --- */

$extern fn_((HashSet_empty(TypeInfo key_ty, P_const$HashSet_Ctx ctx))(HashSet));
$attr($must_check)
$extern fn_((HashSet_init(TypeInfo key_ty, P_const$HashSet_Ctx ctx, mem_Allocator gpa, u32 cap))(mem_Err$HashSet));
$extern fn_((HashSet_fini(HashSet* self, TypeInfo key_ty, mem_Allocator gpa))(void));
$attr($must_check)
$extern fn_((HashSet_clone(HashSet self, TypeInfo key_ty, mem_Allocator gpa))(mem_Err$HashSet));
$attr($must_check)
$extern fn_((HashSet_cloneWithCtx(HashSet self, TypeInfo key_ty, P_const$HashSet_Ctx ctx, mem_Allocator gpa))(mem_Err$HashSet));

/* --- Capacity Management --- */

$extern fn_((HashSet_count(HashSet self))(u32));
$extern fn_((HashSet_cap(HashSet self))(u32));

$attr($must_check)
$extern fn_((HashSet_ensureCap(HashSet* self, TypeInfo key_ty, mem_Allocator gpa, u32 new_cap))(mem_Err$void));
$attr($must_check)
$extern fn_((HashSet_ensureUnusedCap(HashSet* self, TypeInfo key_ty, mem_Allocator gpa, u32 additional))(mem_Err$void));
$extern fn_((HashSet_clearRetainingCap(HashSet* self))(void));
$extern fn_((HashSet_clearAndFree(HashSet* self, TypeInfo key_ty, mem_Allocator gpa))(void));

/* --- Lookup Operations --- */

$extern fn_((HashSet_for(HashSet self, u_V$raw key, u_V$raw ret_mem))(O$u_V$raw));
$extern fn_((HashSet_ptrFor(HashSet self, u_V$raw key))(O$u_P_const$raw));
$extern fn_((HashSet_ptrMutFor(HashSet self, u_V$raw key))(O$u_P$raw));

$extern fn_((HashSet_entry(HashSet self, u_V$raw key))(O$HashSet_Entry));
$extern fn_((HashSet_entryMut(HashSet self, u_V$raw key))(O$HashSet_EntryMut));

$extern fn_((HashSet_contains(HashSet self, u_V$raw key))(bool));

/* --- Insertion Operations --- */

/// Insert only if not present. May allocate.
$attr($must_check)
$extern fn_((HashSet_put(HashSet* self, mem_Allocator gpa, u_V$raw key))(mem_Err$void));
/// Insert only if not present. Asserts capacity and key not present.
$extern fn_((HashSet_putWithin(HashSet* self, u_V$raw key))(void));
/// Insert or update, returning previous value if it existed. May allocate.
$attr($must_check)
$extern fn_((HashSet_fetchPut(
    HashSet* self, mem_Allocator gpa, u_V$raw key, V$HashSet_Sgl$raw ret_mem
))(mem_Err$O$V$HashSet_Sgl$raw));
/// Insert or update, returning previous value if it existed. Asserts capacity.
$extern fn_((HashSet_fetchPutWithin(
    HashSet* self, u_V$raw key, V$HashSet_Sgl$raw ret_mem
))(O$V$HashSet_Sgl$raw));

/* --- Ensure Operations --- */

/// Get existing entry or insert new one. May allocate.
$attr($must_check)
$extern fn_((HashSet_ensure(HashSet* self, mem_Allocator gpa, u_V$raw key))(mem_Err$HashSet_Ensured));
/// Get existing entry or insert new one. Asserts capacity is available.
$extern fn_((HashSet_ensureWithin(HashSet* self, u_V$raw key))(HashSet_Ensured));

/* --- Removal Operations --- */

/// Remove element if present, returning true if removed.
$extern fn_((HashSet_remove(HashSet* self, u_V$raw key))(bool));
/// Remove element if present, returning the removed key.
$extern fn_((HashSet_fetchRemove(HashSet* self, u_V$raw key, V$HashSet_Sgl$raw ret_mem))(O$V$HashSet_Sgl$raw));
/// Remove element by key pointer (must be valid pointer into set).
$extern fn_((HashSet_removeByPtr(HashSet* self, u_P$raw key_ptr))(void));

/* --- Maintenance Operations --- */

/// Rehash in place to remove tombstones and improve performance.
$extern fn_((HashSet_rehash(HashSet* self, TypeInfo key_ty))(void));

/* --- Set Operations --- */

/// Returns true if this set is a subset of other.
$extern fn_((HashSet_isSubset(HashSet self, TypeInfo key_ty, HashSet other))(bool));
/// Returns true if this set is a superset of other.
$extern fn_((HashSet_isSuperset(HashSet self, TypeInfo key_ty, HashSet other))(bool));
/// Returns true if this set is disjoint from other (no common elements).
$extern fn_((HashSet_isDisjoint(HashSet self, TypeInfo key_ty, HashSet other))(bool));

/* --- HashSet_Iter: Iterator over entries --- */

#define HashSet_Iter$$(_K...) __comp_anon__HashSet_Iter$$(_K)
#define HashSet_Iter$(_K...) __comp_alias__HashSet_Iter$(_K)
#define T_decl_HashSet_Iter$(_K...) __comp_gen__T_decl_HashSet_Iter$(_K)
#define T_impl_HashSet_Iter$(_K...) __comp_gen__T_impl_HashSet_Iter$(_K)
#define T_use_HashSet_Iter$(_K...) __comp_gen__T_use_HashSet_Iter$(_K)

typedef struct HashSet_Iter {
    var_(set, const HashSet*);
    var_(idx, usize);
    debug_only(var_(key_ty, TypeInfo));
} HashSet_Iter;
$extern fn_((HashSet_iter(const HashSet* self, TypeInfo key_ty))(HashSet_Iter));
$extern fn_((HashSet_Iter_next(HashSet_Iter* self, TypeInfo key_ty))(O$HashSet_Entry));
$extern fn_((HashSet_Iter_nextMut(HashSet_Iter* self, TypeInfo key_ty))(O$HashSet_EntryMut));

/* --- HashSet_KeyIter: Iterator over keys (alias for HashSet_Iter) --- */

#define HashSet_KeyIter$$(_K...) __comp_anon__HashSet_KeyIter$$(_K)
#define HashSet_KeyIter$(_K...) __comp_alias__HashSet_KeyIter$(_K)
#define T_decl_HashSet_KeyIter$(_K...) __comp_gen__T_decl_HashSet_KeyIter$(_K)
#define T_impl_HashSet_KeyIter$(_K...) __comp_gen__T_impl_HashSet_KeyIter$(_K)
#define T_use_HashSet_KeyIter$(_K...) __comp_gen__T_use_HashSet_KeyIter$(_K)

typedef struct HashSet_KeyIter {
    var_(len, u32);
    var_(metadata, P$HashMap_Ctrl);
    var_(keys, P$raw);
    debug_only(var_(key_ty, TypeInfo));
} HashSet_KeyIter;
$extern fn_((HashSet_keyIter(HashSet self, TypeInfo key_ty))(HashSet_KeyIter));
$extern fn_((HashSet_KeyIter_next(HashSet_KeyIter* self, TypeInfo key_ty))(O$u_P_const$raw));
$extern fn_((HashSet_KeyIter_nextMut(HashSet_KeyIter* self, TypeInfo key_ty))(O$u_P$raw));

/*========== Macro and Definitions ==========================================*/

#define __comp_anon__HashSet_Header$$(_K...) \
    union { \
        struct { \
            var_(keys, P$$(_K)); \
            var_(cap, u32); \
            debug_only(var_(key_ty, TypeInfo)); \
        }; \
        var_(as_raw, HashSet_Header) $like_ref; \
    }
#define __comp_alias__HashSet_Header$(_K...) tpl_id$1T(HashSet_Header, _K)
#define __comp_gen__T_decl_HashSet_Header$(_K...) \
    $maybe_unused typedef union HashSet_Header$(_K) HashSet_Header$(_K)
#define __comp_gen__T_impl_HashSet_Header$(_K...) \
    union HashSet_Header$(_K) { \
        struct { \
            var_(keys, P$$(_K)); \
            var_(cap, u32); \
            debug_only(var_(key_ty, TypeInfo)); \
        }; \
        var_(as_raw, HashSet_Header) $like_ref; \
    }
#define __comp_gen__T_use_HashSet_Header$(_K...) \
    T_decl_HashSet_Header$(_K); \
    T_impl_HashSet_Header$(_K)


#define __comp_anon__HashSet_Sgl$$(_K...) \
    union { \
        struct { \
            debug_only(var_(key_ty, TypeInfo)); \
            union { \
                struct { \
                    var_(key, _K); \
                } data; \
                var_(key, _K); \
            }; \
        }; \
        var_(as_raw, HashSet_Sgl$raw) $like_ref; \
    }
#define __comp_alias__HashSet_Sgl$(_K...) \
    pp_join($, HashSet_Sgl, _K)
#define __comp_gen__T_decl_HashSet_Sgl$(_K...) \
    $maybe_unused typedef union HashSet_Sgl$(_K) HashSet_Sgl$(_K); \
    T_decl_O$(HashSet_Sgl$(_K)); \
    T_decl_E$($set(mem_Err)(O$(HashSet_Sgl$(_K))))
#define __comp_gen__T_impl_HashSet_Sgl$(_K...) \
    union HashSet_Sgl$(_K) { \
        struct { \
            debug_only(var_(key_ty, TypeInfo)); \
            union { \
                struct { \
                    var_(key, _K); \
                } data; \
                var_(key, _K); \
            }; \
        }; \
        var_(as_raw, HashSet_Sgl$raw) $like_ref; \
    }; \
    T_impl_O$(HashSet_Sgl$(_K)); \
    T_impl_E$($set(mem_Err)(O$(HashSet_Sgl$(_K))))
#define __comp_gen__T_use_HashSet_Sgl$(_K...) \
    T_decl_HashSet_Sgl$(_K); \
    T_impl_HashSet_Sgl$(_K)

/* clang-format off */
#define T_use_HashSet_Sgl_key$(_K...) \
    $attr($inline_always) \
    $static fn_((tpl_id$1T(HashSet_Sgl_key, _K)(HashSet_Sgl$(_K) self))(_K)) { \
        return u_castV$((_K)(HashSet_Sgl_key(self.as_raw, u_retV$(_K)))); \
    }
/* clang-format on */

#define __comp_anon__HashSet_Entry$$(_K...) \
    union { \
        struct { \
            var_(key, P_const$$(_K)); \
            debug_only(var_(key_ty, TypeInfo)); \
        }; \
        var_(as_raw, HashSet_Entry) $like_ref; \
    }
#define __comp_alias__HashSet_Entry$(_K...) tpl_id$1T(HashSet_Entry, _K)
#define __comp_gen__T_decl_HashSet_Entry$(_K...) \
    $maybe_unused typedef union HashSet_Entry$(_K) HashSet_Entry$(_K); \
    T_decl_O$(HashSet_Entry$(_K))
#define __comp_gen__T_impl_HashSet_Entry$(_K...) \
    union HashSet_Entry$(_K) { \
        struct { \
            var_(key, P_const$$(_K)); \
            debug_only(var_(key_ty, TypeInfo)); \
        }; \
        var_(as_raw, HashSet_Entry) $like_ref; \
    }; \
    T_impl_O$(HashSet_Entry$(_K))
#define __comp_gen__T_use_HashSet_Entry$(_K...) \
    T_decl_HashSet_Entry$(_K); \
    T_impl_HashSet_Entry$(_K)

/* clang-format off */
#define T_use_HashSet_Entry_key$(_K...) \
    $attr($inline_always) \
    $static fn_((tpl_id$1T(HashSet_Entry_key, _K)(HashSet_Entry$(_K) self))(const _K*)) { \
        return u_castP$((const _K*)(HashSet_Entry_key(*self.as_raw, typeInfo$(_K)))); \
    }
/* clang-format on */

#define __comp_anon__HashSet_EntryMut$$(_K...) \
    union { \
        union { \
            struct { \
                var_(key, P$$(_K)); \
                debug_only(var_(key_ty, TypeInfo)); \
            }; \
            var_(as_const, HashSet_Entry$$(_K)); \
        }; \
        var_(as_raw, HashSet_EntryMut) $like_ref; \
    }
#define __comp_alias__HashSet_EntryMut$(_K...) tpl_id$1T(HashSet_EntryMut, _K)
#define __comp_gen__T_decl_HashSet_EntryMut$(_K...) \
    $maybe_unused typedef union HashSet_EntryMut$(_K) HashSet_EntryMut$(_K); \
    T_decl_O$(HashSet_EntryMut$(_K))
#define __comp_gen__T_impl_HashSet_EntryMut$(_K...) \
    union HashSet_EntryMut$(_K) { \
        union { \
            struct { \
                var_(key, P$$(_K)); \
                debug_only(var_(key_ty, TypeInfo)); \
            }; \
            var_(as_const, HashSet_Entry$(_K)); \
        }; \
        var_(as_raw, HashSet_EntryMut) $like_ref; \
    }; \
    T_impl_O$(HashSet_EntryMut$(_K))
#define __comp_gen__T_use_HashSet_EntryMut$(_K...) \
    T_decl_HashSet_EntryMut$(_K); \
    T_impl_HashSet_EntryMut$(_K)

/* clang-format off */
#define T_use_HashSet_EntryMut_key$(_K...) \
    $attr($inline_always) \
    $static fn_((tpl_id$1T(HashSet_EntryMut_key, _K)(HashSet_EntryMut$(_K) self))(_K*)) { \
        return u_castP$((_K*)(HashSet_EntryMut_key(*self.as_raw, typeInfo$(_K)))); \
    }
/* clang-format on */

#define __comp_anon__HashSet_Ensured$$(_K...) \
    union { \
        struct { \
            var_(key, P$$(_K)); \
            var_(found_existing, bool); \
            debug_only(var_(key_ty, TypeInfo)); \
        }; \
        var_(as_raw, HashSet_Ensured) $like_ref; \
    }
#define __comp_alias__HashSet_Ensured$(_K...) tpl_id$1T(HashSet_Ensured, _K)
#define __comp_gen__T_decl_HashSet_Ensured$(_K...) \
    $maybe_unused typedef union HashSet_Ensured$(_K) HashSet_Ensured$(_K); \
    T_decl_O$(HashSet_Ensured$(_K)); \
    T_decl_E$($set(mem_Err)(HashSet_Ensured$(_K)))
#define __comp_gen__T_impl_HashSet_Ensured$(_K...) \
    union HashSet_Ensured$(_K) { \
        struct { \
            var_(key, P$$(_K)); \
            var_(found_existing, bool); \
            debug_only(var_(key_ty, TypeInfo)); \
        }; \
        var_(as_raw, HashSet_Ensured) $like_ref; \
    }; \
    T_impl_O$(HashSet_Ensured$(_K)); \
    T_impl_E$($set(mem_Err)(HashSet_Ensured$(_K)))
#define __comp_gen__T_use_HashSet_Ensured$(_K...) \
    T_decl_HashSet_Ensured$(_K); \
    T_impl_HashSet_Ensured$(_K)

/* clang-format off */
#define T_use_HashSet_Ensured_key$(_K...) \
    $attr($inline_always) \
    $static fn_((tpl_id$1T(HashSet_Ensured_key, _K)(HashSet_Ensured$(_K) self))(const _K*)) { \
        return u_castP$((const _K*)(HashSet_Ensured_key(*self.as_raw, typeInfo$(_K)))); \
    }
#define T_use_HashSet_Ensured_keyMut$(_K...) \
    $attr($inline_always) \
    $static fn_((tpl_id$1T(HashSet_Ensured_keyMut, _K)(HashSet_Ensured$(_K) self))(_K*)) { \
        return u_castP$((_K*)(HashSet_Ensured_keyMut(*self.as_raw, typeInfo$(_K)))); \
    }
#define T_use_HashSet_Ensured_foundExisting$(_K...) \
    $attr($inline_always) \
    $static fn_((tpl_id$1T(HashSet_Ensured_foundExisting, _K)( \
        HashSet_Ensured$(_K) self \
    ))(O$(HashSet_Entry$(_K)))) { \
        return u_castO$((O$(HashSet_Entry$(_K)))( \
            HashSet_Ensured_foundExisting(*self.as_raw, typeInfo$(_K)) \
        )); \
    }
#define T_use_HashSet_Ensured_foundExistingMut$(_K...) \
    $attr($inline_always) \
    $static fn_((tpl_id$1T(HashSet_Ensured_foundExistingMut, _K)( \
        HashSet_Ensured$(_K) self \
    ))(O$(HashSet_EntryMut$(_K)))) { \
        return u_castO$((O$(HashSet_EntryMut$(_K)))( \
            HashSet_Ensured_foundExistingMut(*self.as_raw, typeInfo$(_K)) \
        )); \
    }
/* clang-format on */

#define __comp_anon__HashSet$$(_K...) \
    union { \
        struct { \
            var_(metadata, O$P$HashMap_Ctrl); \
            var_(size, u32); \
            var_(available, u32); \
            var_(ctx, P_const$HashSet_Ctx); \
            debug_only(var_(key_ty, TypeInfo)); \
        }; \
        var_(as_raw, HashSet) $like_ref; \
    }
#define __comp_alias__HashSet$(_K...) tpl_id$1T(HashSet, _K)
#define __comp_gen__T_decl_HashSet$(_K...) \
    $maybe_unused typedef union HashSet$(_K) HashSet$(_K); \
    T_decl_E$($set(mem_Err)(HashSet$(_K)))
#define __comp_gen__T_impl_HashSet$(_K...) \
    union HashSet$(_K) { \
        struct { \
            var_(metadata, O$P$HashMap_Ctrl); \
            var_(size, u32); \
            var_(available, u32); \
            var_(ctx, P_const$HashSet_Ctx); \
            debug_only(var_(key_ty, TypeInfo)); \
        }; \
        var_(as_raw, HashSet) $like_ref; \
    }; \
    T_impl_E$($set(mem_Err)(HashSet$(_K)))
#define __comp_gen__T_use_HashSet$(_K...) \
    T_decl_HashSet$(_K); \
    T_impl_HashSet$(_K)

/* clang-format off */
#define T_use_HashSet_empty$(_K...) \
    $attr($inline_always) \
    $static fn_((tpl_id$1T(HashSet_empty, _K)(P_const$HashSet_Ctx ctx))(HashSet$(_K))) { \
        return type$((HashSet$(_K))(HashSet_empty(typeInfo$(_K), ctx))); \
    }
#define T_use_HashSet_init$(_K...) \
    $attr($inline_always $must_check) \
    $static fn_((tpl_id$1T(HashSet_init, _K)( \
        P_const$HashSet_Ctx ctx, mem_Allocator gpa, u32 cap \
    ))(E$($set(mem_Err)(HashSet$(_K)))) $scope) { \
        return_(typeE$((ReturnT)(HashSet_init(typeInfo$(_K), ctx, gpa, cap)))); \
    } $unscoped_(fn)
#define T_use_HashSet_fini$(_K...) \
    $attr($inline_always) \
    $static fn_((tpl_id$1T(HashSet_fini, _K)(HashSet$(_K)* self, mem_Allocator gpa))(void)) { \
        return HashSet_fini(self->as_raw, typeInfo$(_K), gpa); \
    }
#define T_use_HashSet_clone$(_K...) \
    $attr($inline_always $must_check) \
    $static fn_((tpl_id$1T(HashSet_clone, _K)(HashSet$(_K) self, mem_Allocator gpa))(E$($set(mem_Err)(HashSet$(_K)))) $scope) { \
        return_(typeE$((ReturnT)(HashSet_clone(*self.as_raw, typeInfo$(_K), gpa)))); \
    } $unscoped_(fn)
#define T_use_HashSet_cloneWithCtx$(_K...) \
    $attr($inline_always $must_check) \
    $static fn_((tpl_id$1T(HashSet_cloneWithCtx, _K)( \
        HashSet$(_K) self, P_const$HashSet_Ctx ctx, mem_Allocator gpa \
    ))(E$($set(mem_Err)(HashSet$(_K)))) $scope) { \
        return_(typeE$((ReturnT)(HashSet_cloneWithCtx(*self.as_raw, typeInfo$(_K), ctx, gpa)))); \
    } $unscoped_(fn)

#define T_use_HashSet_count$(_K...) \
    $attr($inline_always) \
    $static fn_((tpl_id$1T(HashSet_count, _K)(HashSet$(_K) self))(u32)) { \
        return HashSet_count(*self.as_raw); \
    }
#define T_use_HashSet_cap$(_K...) \
    $attr($inline_always) \
    $static fn_((tpl_id$1T(HashSet_cap, _K)(HashSet$(_K) self))(u32)) { \
        return HashSet_cap(*self.as_raw); \
    }

#define T_use_HashSet_ensureCap$(_K...) \
    $attr($inline_always $must_check) \
    $static fn_((tpl_id$1T(HashSet_ensureCap, _K)( \
        HashSet$(_K)* self, mem_Allocator gpa, u32 new_cap \
    ))(mem_Err$void)) { \
        return HashSet_ensureCap(self->as_raw, typeInfo$(_K), gpa, new_cap); \
    }
#define T_use_HashSet_ensureUnusedCap$(_K...) \
    $attr($inline_always $must_check) \
    $static fn_((tpl_id$1T(HashSet_ensureUnusedCap, _K)( \
        HashSet$(_K)* self, mem_Allocator gpa, u32 additional \
    ))(mem_Err$void)) { \
        return HashSet_ensureUnusedCap(self->as_raw, typeInfo$(_K), gpa, additional); \
    }
#define T_use_HashSet_clearRetainingCap$(_K...) \
    $attr($inline_always) \
    $static fn_((tpl_id$1T(HashSet_clearRetainingCap, _K)(HashSet$(_K)* self))(void)) { \
        return HashSet_clearRetainingCap(self->as_raw); \
    }
#define T_use_HashSet_clearAndFree$(_K...) \
    $attr($inline_always) \
    $static fn_((tpl_id$1T(HashSet_clearAndFree, _K)(HashSet$(_K)* self, mem_Allocator gpa))(void)) { \
        return HashSet_clearAndFree(self->as_raw, typeInfo$(_K), gpa); \
    }

#define T_use_HashSet_for$(_K...) \
    $attr($inline_always) \
    $static fn_((tpl_id$1T(HashSet_for, _K)(HashSet$(_K) self, _K key))(O$(_K))) { \
        return u_castO$((O$(_K))(HashSet_for(*self.as_raw, u_anyV(key), u_retV$(_K)))); \
    }
#define T_use_HashSet_ptrFor$(_K...) \
    $attr($inline_always) \
    $static fn_((tpl_id$1T(HashSet_ptrFor, _K)(HashSet$(_K) self, _K key))(O$(P_const$(_K)))) { \
        return u_castO$((O$(P_const$(_K)))(HashSet_ptrFor(*self.as_raw, u_anyV(key)))); \
    }
#define T_use_HashSet_ptrMutFor$(_K...) \
    $attr($inline_always) \
    $static fn_((tpl_id$1T(HashSet_ptrMutFor, _K)(HashSet$(_K) self, _K key))(O$(P$(_K)))) { \
        return u_castO$((O$(P$(_K)))(HashSet_ptrMutFor(*self.as_raw, u_anyV(key)))); \
    }

#define T_use_HashSet_entry$(_K...) \
    $attr($inline_always) \
    $static fn_((tpl_id$1T(HashSet_entry, _K)(HashSet$(_K) self, _K key))(O$(HashSet_Entry$(_K)))) { \
        return typeO$((O$(HashSet_Entry$(_K)))(HashSet_entry(*self.as_raw, u_anyV(key)))); \
    }
#define T_use_HashSet_entryMut$(_K...) \
    $attr($inline_always) \
    $static fn_((tpl_id$1T(HashSet_entryMut, _K)(HashSet$(_K) self, _K key))(O$(HashSet_EntryMut$(_K)))) { \
        return typeO$((O$(HashSet_EntryMut$(_K)))(HashSet_entryMut(*self.as_raw, u_anyV(key)))); \
    }

#define T_use_HashSet_contains$(_K...) \
    $attr($inline_always) \
    $static fn_((tpl_id$1T(HashSet_contains, _K)(HashSet$(_K) self, _K key))(bool)) { \
        return HashSet_contains(*self.as_raw, u_anyV(key)); \
    }

#define T_use_HashSet_put$(_K...) \
    $attr($inline_always $must_check) \
    $static fn_((tpl_id$1T(HashSet_put, _K)(HashSet$(_K)* self, mem_Allocator gpa, _K key))(mem_Err$void)) { \
        return HashSet_put(self->as_raw, gpa, u_anyV(key)); \
    }
#define T_use_HashSet_putWithin$(_K...) \
    $attr($inline_always) \
    $static fn_((tpl_id$1T(HashSet_putWithin, _K)(HashSet$(_K)* self, _K key))(void)) { \
        return HashSet_putWithin(self->as_raw, u_anyV(key)); \
    }
#define T_use_HashSet_fetchPut$(_K...) \
    $attr($inline_always $must_check) \
    $static fn_((tpl_id$1T(HashSet_fetchPut, _K)(HashSet$(_K)* self, mem_Allocator gpa, _K key))(E$($set(mem_Err)(O$(HashSet_Sgl$(_K))))) $scope) { \
        let opt_sgl = try_(HashSet_fetchPut(self->as_raw, gpa, u_anyV(key), lit0$((HashSet_Sgl$(_K))).as_raw)); \
        let sgl = orelse_((opt_sgl)(return_ok(none()))); \
        return_ok(some(*as$(HashSet_Sgl$(_K)*)(sgl))); \
    } $unscoped_(fn)
#define T_use_HashSet_fetchPutWithin$(_K...) \
    $attr($inline_always) \
    $static fn_((tpl_id$1T(HashSet_fetchPutWithin, _K)(HashSet$(_K)* self, _K key))(O$(HashSet_Sgl$(_K))) $scope) { \
        let opt_sgl = HashSet_fetchPutWithin(self->as_raw, u_anyV(key), lit0$((HashSet_Sgl$(_K))).as_raw); \
        let sgl = orelse_((opt_sgl)(return_none())); \
        return_some(*as$(HashSet_Sgl$(_K)*)(sgl)); \
    } $unscoped_(fn)

#define T_use_HashSet_ensure$(_K...) \
    $attr($inline_always $must_check) \
    $static fn_((tpl_id$1T(HashSet_ensure, _K)(HashSet$(_K)* self, mem_Allocator gpa, _K key))(E$($set(mem_Err)(HashSet_Ensured$(_K)))) $scope) { \
        return_(typeE$((ReturnT)(HashSet_ensure(self->as_raw, gpa, u_anyV(key))))); \
    } $unscoped_(fn)
#define T_use_HashSet_ensureWithin$(_K...) \
    $attr($inline_always) \
    $static fn_((tpl_id$1T(HashSet_ensureWithin, _K)(HashSet$(_K)* self, _K key))(HashSet_Ensured$(_K))) { \
        return type$((HashSet_Ensured$(_K))(HashSet_ensureWithin(self->as_raw, u_anyV(key)))); \
    }

#define T_use_HashSet_remove$(_K...) \
    $attr($inline_always) \
    $static fn_((tpl_id$1T(HashSet_remove, _K)(HashSet$(_K)* self, _K key))(bool)) { \
        return HashSet_remove(self->as_raw, u_anyV(key)); \
    }
#define T_use_HashSet_fetchRemove$(_K...) \
    $attr($inline_always) \
    $static fn_((tpl_id$1T(HashSet_fetchRemove, _K)(HashSet$(_K)* self, _K key))(O$(HashSet_Sgl$(_K))) $scope) { \
        let opt_sgl = HashSet_fetchRemove(self->as_raw, u_anyV(key), lit0$((HashSet_Sgl$(_K))).as_raw); \
        let sgl = orelse_((opt_sgl)(return_none())); \
        return_some(*as$(HashSet_Sgl$(_K)*)(sgl)); \
    } $unscoped_(fn)
#define T_use_HashSet_removeByPtr$(_K...) \
    $attr($inline_always) \
    $static fn_((tpl_id$1T(HashSet_removeByPtr, _K)(HashSet$(_K)* self, _K* key))(void)) { \
        return HashSet_removeByPtr(self->as_raw, u_anyP(key)); \
    }

#define T_use_HashSet_rehash$(_K...) \
    $attr($inline_always) \
    $static fn_((tpl_id$1T(HashSet_rehash, _K)(HashSet$(_K)* self))(void)) { \
        return HashSet_rehash(self->as_raw, typeInfo$(_K)); \
    }

#define T_use_HashSet_isSubset$(_K...) \
    $attr($inline_always) \
    $static fn_((tpl_id$1T(HashSet_isSubset, _K)(HashSet$(_K) self, HashSet$(_K) other))(bool)) { \
        return HashSet_isSubset(*self.as_raw, typeInfo$(_K), *other.as_raw); \
    }
#define T_use_HashSet_isSuperset$(_K...) \
    $attr($inline_always) \
    $static fn_((tpl_id$1T(HashSet_isSuperset, _K)(HashSet$(_K) self, HashSet$(_K) other))(bool)) { \
        return HashSet_isSuperset(*self.as_raw, typeInfo$(_K), *other.as_raw); \
    }
#define T_use_HashSet_isDisjoint$(_K...) \
    $attr($inline_always) \
    $static fn_((tpl_id$1T(HashSet_isDisjoint, _K)(HashSet$(_K) self, HashSet$(_K) other))(bool)) { \
        return HashSet_isDisjoint(*self.as_raw, typeInfo$(_K), *other.as_raw); \
    }
/* clang-format on */

#define __comp_anon__HashSet_Iter$$(_K...) \
    union { \
        struct { \
            var_(set, P_const$$(HashSet$(_K))); \
            var_(idx, usize); \
            debug_only(var_(key_ty, TypeInfo)); \
        }; \
        var_(as_raw, HashSet_Iter) $like_ref; \
    }
#define __comp_alias__HashSet_Iter$(_K...) tpl_id$1T(HashSet_Iter, _K)
#define __comp_gen__T_decl_HashSet_Iter$(_K...) \
    $maybe_unused typedef union HashSet_Iter$(_K) HashSet_Iter$(_K)
#define __comp_gen__T_impl_HashSet_Iter$(_K...) \
    union HashSet_Iter$(_K) { \
        struct { \
            var_(set, P_const$$(HashSet$(_K))); \
            var_(idx, usize); \
            debug_only(var_(key_ty, TypeInfo)); \
        }; \
        var_(as_raw, HashSet_Iter) $like_ref; \
    }
#define __comp_gen__T_use_HashSet_Iter$(_K...) \
    T_decl_HashSet_Iter$(_K); \
    T_impl_HashSet_Iter$(_K)

/* clang-format off */
#define T_use_HashSet_iter$(_K...) \
    $attr($inline_always) \
    $static fn_((tpl_id$1T(HashSet_iter, _K)(const HashSet$(_K)* self))(HashSet_Iter$(_K))) { \
        return type$((HashSet_Iter$(_K))(HashSet_iter(self->as_raw, typeInfo$(_K)))); \
    }
#define T_use_HashSet_Iter_next$(_K...) \
    $attr($inline_always) \
    $static fn_((tpl_id$1T(HashSet_Iter_next, _K)(HashSet_Iter$(_K)* self))(O$(HashSet_Entry$(_K)))) { \
        return typeO$((O$(HashSet_Entry$(_K)))(HashSet_Iter_next(self->as_raw, typeInfo$(_K)))); \
    }
#define T_use_HashSet_Iter_nextMut$(_K...) \
    $attr($inline_always) \
    $static fn_((tpl_id$1T(HashSet_Iter_nextMut, _K)(HashSet_Iter$(_K)* self))(O$(HashSet_EntryMut$(_K)))) { \
        return typeO$((O$(HashSet_EntryMut$(_K)))(HashSet_Iter_nextMut(self->as_raw, typeInfo$(_K)))); \
    }
/* clang-format on */

#define __comp_anon__HashSet_KeyIter$$(_K...) \
    union { \
        struct { \
            var_(len, u32); \
            var_(metadata, P$HashMap_Ctrl); \
            var_(keys, P$$(_K)); \
            debug_only(var_(key_ty, TypeInfo)); \
        }; \
        var_(as_raw, HashSet_KeyIter) $like_ref; \
    }
#define __comp_alias__HashSet_KeyIter$(_K...) tpl_id$1T(HashSet_KeyIter, _K)
#define __comp_gen__T_decl_HashSet_KeyIter$(_K...) \
    $maybe_unused typedef union HashSet_KeyIter$(_K) HashSet_KeyIter$(_K)
#define __comp_gen__T_impl_HashSet_KeyIter$(_K...) \
    union HashSet_KeyIter$(_K) { \
        struct { \
            var_(len, u32); \
            var_(metadata, P$HashMap_Ctrl); \
            var_(keys, P$$(_K)); \
            debug_only(var_(key_ty, TypeInfo)); \
        }; \
        var_(as_raw, HashSet_KeyIter) $like_ref; \
    }
#define __comp_gen__T_use_HashSet_KeyIter$(_K...) \
    T_decl_HashSet_KeyIter$(_K); \
    T_impl_HashSet_KeyIter$(_K)

/* clang-format off */
#define T_use_HashSet_keyIter$(_K...) \
    $attr($inline_always) \
    $static fn_((tpl_id$1T(HashSet_keyIter, _K)(HashSet$(_K) self))(HashSet_KeyIter$(_K))) { \
        return type$((HashSet_KeyIter$(_K))(HashSet_keyIter(*self.as_raw, typeInfo$(_K)))); \
    }
#define T_use_HashSet_KeyIter_next$(_K...) \
    $attr($inline_always) \
    $static fn_((tpl_id$1T(HashSet_KeyIter_next, _K)(HashSet_KeyIter$(_K)* self))(O$(P_const$(_K)))) { \
        return u_castO$((O$(P_const$(_K)))(HashSet_KeyIter_next(self->as_raw, typeInfo$(_K)))); \
    }
#define T_use_HashSet_KeyIter_nextMut$(_K...) \
    $attr($inline_always) \
    $static fn_((tpl_id$1T(HashSet_KeyIter_nextMut, _K)(HashSet_KeyIter$(_K)* self))(O$(P$(_K)))) { \
        return u_castO$((O$(P$(_K)))(HashSet_KeyIter_nextMut(self->as_raw, typeInfo$(_K)))); \
    }
/* clang-format on */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* HashSet__included */

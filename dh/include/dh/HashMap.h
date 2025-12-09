/**
 * @copyright Copyright (c) 2025 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    HashMap.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2025-11-22 (date of creation)
 * @updated 2025-12-09 (date of last update)
 * @version v0.1
 * @ingroup dasae-headers(dh)
 * @prefix  HashMap
 *
 * @brief   High-performance unordered hash map
 * @details Swiss Table inspired design with single allocation and excellent cache locality.
 *          Control bytes (1 byte per slot) for fast SIMD lookup.
 *          Keys and values stored in contiguous arrays.
 *          Average O(1) insertion, lookup, and removal.
 */
#ifndef HashMap__included
#define HashMap__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "dh/prl.h"
#include "dh/mem/Allocator.h"

/*========== Macros and Declarations ========================================*/

typedef fn_(((*)(u_V$raw val))(u64) $T) u_HashFn;
typedef fn_(((*)(u_V$raw val, u_V$raw ctx))(u64) $T) u_HashCtxFn;

/* --- HashMap_Ctrl: Slot Metadata --- */

#define HashMap_Ctrl_free 0
#define HashMap_Ctrl_tombstone 1
/// Metadata for a slot. It can be in three states: empty, used or
/// tombstone. Tombstones indicate that an entry was previously used,
/// they are a simple way to handle removal.
/// To this state, we add 7 bits from the slot's key hash. These are
/// used as a fast way to disambiguate between entries without
/// having to use the equality function. If two fingerprints are
/// different, we know that we don't have to compare the keys at all.
/// The 7 bits are the highest ones from a 64 bit hash. This way, not
/// only we use the `log2(capacity)` lowest bits from the hash to determine
/// a slot index, but we use 7 more bits to quickly resolve collisions
/// when multiple elements with different hashes end up wanting to be in the same slot.
/// Not using the equality function means we don't have to read into
/// the entries array, likely avoiding a cache miss and a potentially
/// costly function call.
typedef union HashMap_Ctrl {
    struct {
        u8 fingerprint : 7;
        u8 used        : 1;
    };
    u8 bits;
} HashMap_Ctrl;
T_use_P$(HashMap_Ctrl);
T_use_O$(P_const$HashMap_Ctrl);
T_use_O$(P$HashMap_Ctrl);
$static const HashMap_Ctrl HashMap_Ctrl_default = { .fingerprint = HashMap_Ctrl_free, .used = 0 };
#define HashMap_Ctrl_slot_free (lit_n$(HashMap_Ctrl){ .fingerprint = HashMap_Ctrl_free, .used = 0 })
#define HashMap_Ctrl_slot_tombstone (lit_n$(HashMap_Ctrl){ .fingerprint = HashMap_Ctrl_tombstone, .used = 0 })
$attr($inline_always)
$static fn_((HashMap_Ctrl_isUsed(HashMap_Ctrl ctrl))(bool)) {
    return ctrl.used == 1;
}
$attr($inline_always)
$static fn_((HashMap_Ctrl_isTombstone(HashMap_Ctrl ctrl))(bool)) {
    return ctrl.bits == HashMap_Ctrl_tombstone;
}
$attr($inline_always)
$static fn_((HashMap_Ctrl_isFree(HashMap_Ctrl ctrl))(bool)) {
    return ctrl.bits == HashMap_Ctrl_free;
}
$attr($inline_always)
$static fn_((HashMap_Ctrl_takeFingerprint(u64 hash))(u8)) {
    let hash_bits = 64;
    let fp_bits = 7;
    return (hash >> (hash_bits - fp_bits)) & ((2 << 6) - 1);
}
$attr($inline_always)
$static fn_((HashMap_Ctrl_fill(HashMap_Ctrl* ctrl, u8 fingerprint))(void)) {
    ctrl->used = 1;
    ctrl->fingerprint = fingerprint;
}
$attr($inline_always)
$static fn_((HashMap_Ctrl_remove(HashMap_Ctrl* ctrl))(void)) {
    ctrl->used = 0;
    ctrl->fingerprint = HashMap_Ctrl_tombstone;
}

/* --- HashMap_Header: Internal header stored before metadata --- */

#define HashMap_Header$$(_K, _V...) __comp_anon__HashMap_Header$$(_K, _V)
#define HashMap_Header$(_K, _V...) __comp_alias__HashMap_Header$(_K, _V)
#define T_decl_HashMap_Header$(_K, _V...) __comp_gen__T_decl_HashMap_Header$(_K, _V)
#define T_impl_HashMap_Header$(_K, _V...) __comp_gen__T_impl_HashMap_Header$(_K, _V)
#define T_use_HashMap_Header$(_K, _V...) __comp_gen__T_use_HashMap_Header$(_K, _V)

typedef struct HashMap_Header {
    var_(vals, P$raw);
    var_(keys, P$raw);
    var_(cap, u32);
    debug_only(struct {
        var_(key_ty, TypeInfo);
        var_(val_ty, TypeInfo);
    });
} HashMap_Header;

/* --- HashMap_Pair: Key-Value pair type (2-tuple) --- */

#define HashMap_Pair$$(_K, _V...) __comp_anon__HashMap_Pair$$(_K, _V)
#define HashMap_Pair$(_K, _V...) __comp_alias__HashMap_Pair$(_K, _V)
#define T_decl_HashMap_Pair$(_K, _V...) __comp_gen__T_decl_HashMap_Pair$(_K, _V)
#define T_impl_HashMap_Pair$(_K, _V...) __comp_gen__T_impl_HashMap_Pair$(_K, _V)
#define T_use_HashMap_Pair$(_K, _V...) __comp_gen__T_use_HashMap_Pair$(_K, _V)

typedef struct HashMap_Pair$raw {
    debug_only(struct {
        var_(key_ty, TypeInfo);
        var_(val_ty, TypeInfo);
    });
    var_(data, V$raw);
} HashMap_Pair$raw;
T_use_P$(HashMap_Pair$raw);
typedef P$HashMap_Pair$raw V$HashMap_Pair$raw;
T_use_O$(V$HashMap_Pair$raw);
T_use_E$($set(mem_Err)(O$V$HashMap_Pair$raw));
$extern fn_((HashMap_Pair_key(V$HashMap_Pair$raw self, TypeInfo val_ty, u_V$raw ret_mem))(u_V$raw));
$extern fn_((HashMap_Pair_val(V$HashMap_Pair$raw self, TypeInfo key_ty, u_V$raw ret_mem))(u_V$raw));

/* --- HashMap_Entry: Pointers to key and value in map --- */

#define HashMap_Entry$$(_K, _V...) __comp_anon__HashMap_Entry$$(_K, _V)
#define HashMap_Entry$(_K, _V...) __comp_alias__HashMap_Entry$(_K, _V)
#define T_decl_HashMap_Entry$(_K, _V...) __comp_gen__T_decl_HashMap_Entry$(_K, _V)
#define T_impl_HashMap_Entry$(_K, _V...) __comp_gen__T_impl_HashMap_Entry$(_K, _V)
#define T_use_HashMap_Entry$(_K, _V...) __comp_gen__T_use_HashMap_Entry$(_K, _V)

typedef struct HashMap_Entry {
    var_(key, P_const$raw);
    var_(val, P_const$raw);
    debug_only(struct {
        var_(key_ty, TypeInfo);
        var_(val_ty, TypeInfo);
    });
} HashMap_Entry;
T_use_O$(HashMap_Entry);
$extern fn_((HashMap_Entry_key(HashMap_Entry self, TypeInfo key_ty))(u_P_const$raw));
$extern fn_((HashMap_Entry_val(HashMap_Entry self, TypeInfo val_ty))(u_P_const$raw));

#define HashMap_EntryMut$$(_K, _V...) __comp_anon__HashMap_EntryMut$$(_K, _V)
#define HashMap_EntryMut$(_K, _V...) __comp_alias__HashMap_EntryMut$(_K, _V)
#define T_decl_HashMap_EntryMut$(_K, _V...) __comp_gen__T_decl_HashMap_EntryMut$(_K, _V)
#define T_impl_HashMap_EntryMut$(_K, _V...) __comp_gen__T_impl_HashMap_EntryMut$(_K, _V)
#define T_use_HashMap_EntryMut$(_K, _V...) __comp_gen__T_use_HashMap_EntryMut$(_K, _V)

typedef union HashMap_EntryMut {
    struct {
        var_(key, P$raw);
        var_(val, P$raw);
        debug_only(struct {
            var_(key_ty, TypeInfo);
            var_(val_ty, TypeInfo);
        });
    };
    var_(as_const, HashMap_Entry);
} HashMap_EntryMut;
T_use_O$(HashMap_EntryMut);
$extern fn_((HashMap_EntryMut_key(HashMap_EntryMut self, TypeInfo key_ty))(u_P$raw));
$extern fn_((HashMap_EntryMut_val(HashMap_EntryMut self, TypeInfo val_ty))(u_P$raw));

/* --- HashMap_Ensured: Result from ensure operations --- */

#define HashMap_Ensured$$(_K, _V...) __comp_anon__HashMap_Ensured$$(_K, _V)
#define HashMap_Ensured$(_K, _V...) __comp_alias__HashMap_Ensured$(_K, _V)
#define T_decl_HashMap_Ensured$(_K, _V...) __comp_gen__T_decl_HashMap_Ensured$(_K, _V)
#define T_impl_HashMap_Ensured$(_K, _V...) __comp_gen__T_impl_HashMap_Ensured$(_K, _V)
#define T_use_HashMap_Ensured$(_K, _V...) __comp_gen__T_use_HashMap_Ensured$(_K, _V)

typedef struct HashMap_Ensured {
    var_(key, P$raw);
    var_(val, P$raw);
    var_(found_existing, bool);
    debug_only(struct {
        var_(key_ty, TypeInfo);
        var_(val_ty, TypeInfo);
    });
} HashMap_Ensured;
T_use_E$($set(mem_Err)(HashMap_Ensured));
$extern fn_((HashMap_Ensured_key(HashMap_Ensured self, TypeInfo key_ty))(u_P_const$raw));
$extern fn_((HashMap_Ensured_keyMut(HashMap_Ensured self, TypeInfo key_ty))(u_P$raw));
$extern fn_((HashMap_Ensured_val(HashMap_Ensured self, TypeInfo val_ty))(u_P_const$raw));
$extern fn_((HashMap_Ensured_valMut(HashMap_Ensured self, TypeInfo val_ty))(u_P$raw));
$extern fn_((HashMap_Ensured_foundExisting(
    HashMap_Ensured self, TypeInfo key_ty, TypeInfo val_ty
))(O$HashMap_Entry));
$extern fn_((HashMap_Ensured_foundExistingMut(
    HashMap_Ensured self, TypeInfo key_ty, TypeInfo val_ty
))(O$HashMap_EntryMut));

/* --- HashMap_Ctx: Context for hash and equality functions --- */

typedef u_HashCtxFn HashMap_HashFn;
$extern fn_((HashMap_HashFn_default(u_V$raw val, u_V$raw ctx))(u64));
claim_assert_static(Type_eq$(HashMap_HashFn, TypeOf(&HashMap_HashFn_default)));
typedef u_EqlCtxFn HashMap_EqlFn;
$extern fn_((HashMap_EqlFn_default(u_V$raw lhs, u_V$raw rhs, u_V$raw ctx))(bool));
claim_assert_static(Type_eq$(HashMap_EqlFn, TypeOf(&HashMap_EqlFn_default)));

typedef struct HashMap_Ctx {
    var_(inner, u_P_const$raw);
    var_(hashFn, HashMap_HashFn);
    var_(eqlFn, HashMap_EqlFn);
} HashMap_Ctx;
T_use_P$(HashMap_Ctx);
$extern fn_((HashMap_Ctx_default(void))(P_const$HashMap_Ctx));

/* --- HashMap: Main hash map structure --- */

#define HashMap$$(_K, _V...) __comp_anon__HashMap$$(_K, _V)
#define HashMap$(_K, _V...) __comp_alias__HashMap$(_K, _V)
#define T_decl_HashMap$(_K, _V...) __comp_gen__T_decl_HashMap$(_K, _V)
#define T_impl_HashMap$(_K, _V...) __comp_gen__T_impl_HashMap$(_K, _V)
#define T_use_HashMap$(_K, _V...) __comp_gen__T_use_HashMap$(_K, _V)

typedef struct HashMap {
    /// Pointer to metadata array. At -sizeOf$(HashMap_Header) is Header field.
    var_(metadata, O$P$HashMap_Ctrl);
    /// Current number of elements.
    var_(size, u32);
    /// Number of available slots before grow is needed.
    var_(available, u32);
    /// Context containing hash and equality functions.
    var_(ctx, P_const$HashMap_Ctx);
    debug_only(struct {
        var_(key_ty, TypeInfo);
        var_(val_ty, TypeInfo);
    });
} HashMap;
T_use$((HashMap)(O, E));
T_use_E$($set(mem_Err)(HashMap));
#define HashMap_default_max_load_ratio (lit_n$(u32)(80))
#define HashMap_default_min_cap (lit_n$(u32)(8))

/* --- Construction/Destruction --- */

$extern fn_((HashMap_empty(
    TypeInfo key_ty, TypeInfo val_ty, P_const$HashMap_Ctx ctx
))(HashMap));
$attr($must_check)
$extern fn_((HashMap_init(
    TypeInfo key_ty, TypeInfo val_ty, P_const$HashMap_Ctx ctx, mem_Allocator gpa, u32 cap
))(mem_Err$HashMap));
$extern fn_((HashMap_fini(
    HashMap* self, TypeInfo key_ty, TypeInfo val_ty, mem_Allocator gpa
))(void));
$attr($must_check)
$extern fn_((HashMap_clone(
    HashMap self, TypeInfo key_ty, TypeInfo val_ty, mem_Allocator gpa
))(mem_Err$HashMap));
$attr($must_check)
$extern fn_((HashMap_cloneWithCtx(
    HashMap self, TypeInfo key_ty, TypeInfo val_ty, P_const$HashMap_Ctx ctx, mem_Allocator gpa
))(mem_Err$HashMap));

/* --- Capacity Management --- */

$extern fn_((HashMap_count(HashMap self))(u32));
$extern fn_((HashMap_cap(HashMap self))(u32));

$attr($must_check)
$extern fn_((HashMap_ensureCap(
    HashMap* self, TypeInfo key_ty, TypeInfo val_ty, mem_Allocator gpa, u32 new_cap
))(mem_Err$void));
$attr($must_check)
$extern fn_((HashMap_ensureUnusedCap(
    HashMap* self, TypeInfo key_ty, TypeInfo val_ty, mem_Allocator gpa, u32 additional
))(mem_Err$void));
$extern fn_((HashMap_clearRetainingCap(HashMap* self))(void));
$extern fn_((HashMap_clearAndFree(
    HashMap* self, TypeInfo key_ty, TypeInfo val_ty, mem_Allocator gpa
))(void));

/* --- Lookup Operations --- */

$extern fn_((HashMap_by(HashMap self, u_V$raw key, u_V$raw ret_mem))(O$u_V$raw));
$extern fn_((HashMap_ptrBy(HashMap self, TypeInfo val_ty, u_V$raw key))(O$u_P_const$raw));
$extern fn_((HashMap_ptrMutBy(HashMap self, TypeInfo val_ty, u_V$raw key))(O$u_P$raw));

$extern fn_((HashMap_for(HashMap self, TypeInfo val_ty, u_V$raw key, u_V$raw ret_mem))(O$u_V$raw));
$extern fn_((HashMap_ptrFor(HashMap self, TypeInfo val_ty, u_V$raw key))(O$u_P_const$raw));
$extern fn_((HashMap_ptrMutFor(HashMap self, TypeInfo val_ty, u_V$raw key))(O$u_P$raw));

$extern fn_((HashMap_entry(HashMap self, TypeInfo val_ty, u_V$raw key))(O$HashMap_Entry));
$extern fn_((HashMap_entryMut(HashMap self, TypeInfo val_ty, u_V$raw key))(O$HashMap_EntryMut));

$extern fn_((HashMap_contains(HashMap self, TypeInfo val_ty, u_V$raw key))(bool));

/* --- Insertion Operations --- */

/// Insert or update. May allocate.
$attr($must_check)
$extern fn_((HashMap_put(HashMap* self, mem_Allocator gpa, u_V$raw key, u_V$raw val))(mem_Err$void));
/// Insert or update. Asserts capacity is available.
$extern fn_((HashMap_putWithin(HashMap* self, u_V$raw key, u_V$raw val))(void));
/// Insert only if not present. May allocate.
$attr($must_check)
$extern fn_((HashMap_putNoClobber(
    HashMap* self, mem_Allocator gpa, u_V$raw key, u_V$raw val
))(mem_Err$void));
/// Insert only if not present. Asserts capacity and key not present.
$extern fn_((HashMap_putNoClobberWithin(
    HashMap* self, u_V$raw key, u_V$raw val
))(void));
/// Insert or update, returning previous value if it existed. May allocate.
$attr($must_check)
$extern fn_((HashMap_fetchPut(
    HashMap* self, mem_Allocator gpa, u_V$raw key, u_V$raw val, V$HashMap_Pair$raw ret_mem
))(mem_Err$O$V$HashMap_Pair$raw));
/// Insert or update, returning previous value if it existed. Asserts capacity.
$extern fn_((HashMap_fetchPutWithin(
    HashMap* self, u_V$raw key, u_V$raw val, V$HashMap_Pair$raw ret_mem
))(O$V$HashMap_Pair$raw));

/* --- Ensure Operations --- */

/// Get existing entry or insert new one. May allocate.
$attr($must_check)
$extern fn_((HashMap_ensure(
    HashMap* self, TypeInfo val_ty, mem_Allocator gpa, u_V$raw key
))(mem_Err$HashMap_Ensured));
/// Get existing entry or insert new one. Asserts capacity is available.
$extern fn_((HashMap_ensureWithin(
    HashMap* self, TypeInfo val_ty, u_V$raw key
))(HashMap_Ensured));
/// Get existing entry or insert with default value. May allocate.
$attr($must_check)
$extern fn_((HashMap_ensureValue(
    HashMap* self, mem_Allocator gpa, u_V$raw key, u_V$raw default_val
))(mem_Err$HashMap_Ensured));

/* --- Removal Operations --- */

/// Remove entry if present, returning true if removed.
$extern fn_((HashMap_remove(HashMap* self, TypeInfo val_ty, u_V$raw key))(bool));
/// Remove entry if present, returning the removed key-value pair.
$extern fn_((HashMap_fetchRemove(
    HashMap* self, TypeInfo val_ty, u_V$raw key, V$HashMap_Pair$raw ret_mem
))(O$V$HashMap_Pair$raw));
/// Remove entry by key pointer (must be valid pointer into map).
$extern fn_((HashMap_removeByPtr(HashMap* self, TypeInfo val_ty, u_P$raw key_ptr))(void));

/* --- Maintenance Operations --- */

/// Rehash in place to remove tombstones and improve performance.
$extern fn_((HashMap_rehash(HashMap* self, TypeInfo key_ty, TypeInfo val_ty))(void));

/* --- HashMap_Iter: Iterator over entries --- */

#define HashMap_Iter$$(_K, _V...) __comp_anon__HashMap_Iter$$(_K, _V)
#define HashMap_Iter$(_K, _V...) __comp_alias__HashMap_Iter$(_K, _V)
#define T_decl_HashMap_Iter$(_K, _V...) __comp_gen__T_decl_HashMap_Iter$(_K, _V)
#define T_impl_HashMap_Iter$(_K, _V...) __comp_gen__T_impl_HashMap_Iter$(_K, _V)
#define T_use_HashMap_Iter$(_K, _V...) __comp_gen__T_use_HashMap_Iter$(_K, _V)

typedef struct HashMap_Iter {
    var_(map, const HashMap*);
    var_(idx, usize);
    debug_only(struct {
        var_(key_ty, TypeInfo);
        var_(val_ty, TypeInfo);
    });
} HashMap_Iter;
$extern fn_((HashMap_iter(const HashMap* self, TypeInfo key_ty, TypeInfo val_ty))(HashMap_Iter));
$extern fn_((HashMap_Iter_next(HashMap_Iter* self, TypeInfo key_ty, TypeInfo val_ty))(O$HashMap_Entry));
$extern fn_((HashMap_Iter_nextMut(HashMap_Iter* self, TypeInfo key_ty, TypeInfo val_ty))(O$HashMap_EntryMut));

/* --- HashMap_KeyIter: Iterator over keys --- */

#define HashMap_KeyIter$$(_K, _V...) __comp_anon__HashMap_KeyIter$$(_K, _V)
#define HashMap_KeyIter$(_K, _V...) __comp_alias__HashMap_KeyIter$(_K, _V)
#define T_decl_HashMap_KeyIter$(_K, _V...) __comp_gen__T_decl_HashMap_KeyIter$(_K, _V)
#define T_impl_HashMap_KeyIter$(_K, _V...) __comp_gen__T_impl_HashMap_KeyIter$(_K, _V)
#define T_use_HashMap_KeyIter$(_K, _V...) __comp_gen__T_use_HashMap_KeyIter$(_K, _V)

typedef struct HashMap_KeyIter {
    var_(len, u32);
    var_(metadata, P$HashMap_Ctrl);
    var_(keys, P$raw);
    debug_only(var_(key_ty, TypeInfo));
} HashMap_KeyIter;
$extern fn_((HashMap_keyIter(HashMap self, TypeInfo key_ty, TypeInfo val_ty))(HashMap_KeyIter));
$extern fn_((HashMap_KeyIter_next(HashMap_KeyIter* self, TypeInfo key_ty))(O$u_P_const$raw));
$extern fn_((HashMap_KeyIter_nextMut(HashMap_KeyIter* self, TypeInfo key_ty))(O$u_P$raw));

/* --- HashMap_ValIter: Iterator over values --- */

#define HashMap_ValIter$$(_K, _V...) __comp_anon__HashMap_ValIter$$(_K, _V)
#define HashMap_ValIter$(_K, _V...) __comp_alias__HashMap_ValIter$(_K, _V)
#define T_decl_HashMap_ValIter$(_K, _V...) __comp_gen__T_decl_HashMap_ValIter$(_K, _V)
#define T_impl_HashMap_ValIter$(_K, _V...) __comp_gen__T_impl_HashMap_ValIter$(_K, _V)
#define T_use_HashMap_ValIter$(_K, _V...) __comp_gen__T_use_HashMap_ValIter$(_K, _V)

typedef struct HashMap_ValIter {
    var_(len, u32);
    var_(metadata, P$HashMap_Ctrl);
    var_(vals, P$raw);
    debug_only(var_(val_ty, TypeInfo));
} HashMap_ValIter;
$extern fn_((HashMap_valIter(HashMap self, TypeInfo key_ty, TypeInfo val_ty))(HashMap_ValIter));
$extern fn_((HashMap_ValIter_next(HashMap_ValIter* self, TypeInfo val_ty))(O$u_P_const$raw));
$extern fn_((HashMap_ValIter_nextMut(HashMap_ValIter* self, TypeInfo val_ty))(O$u_P$raw));

/*========== Macro and Definitions ==========================================*/

#define __comp_anon__HashMap_Header$$(_K, _V...) \
    union { \
        struct { \
            var_(vals, P$$(_V)); \
            var_(keys, P$$(_K)); \
            var_(cap, u32); \
            debug_only(struct { \
                var_(key_ty, TypeInfo); \
                var_(val_ty, TypeInfo); \
            }); \
        }; \
        var_(as_raw, HashMap_Header) $like_ref; \
    }
#define __comp_alias__HashMap_Header$(_K, _V...) tpl_id$1T$2U(HashMap_Header, _K, _V)
#define __comp_gen__T_decl_HashMap_Header$(_K, _V...) \
    $maybe_unused typedef union HashMap_Header$(_K, _V) HashMap_Header$(_K, _V)
#define __comp_gen__T_impl_HashMap_Header$(_K, _V...) \
    union HashMap_Header$(_K, _V) { \
        struct { \
            var_(vals, P$$(_V)); \
            var_(keys, P$$(_K)); \
            var_(cap, u32); \
            debug_only(struct { \
                var_(key_ty, TypeInfo); \
                var_(val_ty, TypeInfo); \
            }); \
        }; \
        var_(as_raw, HashMap_Header) $like_ref; \
    }
#define __comp_gen__T_use_HashMap_Header$(_K, _V...) \
    T_decl_HashMap_Header$(_K, _V); \
    T_impl_HashMap_Header$(_K, _V)

/* clang-format off */

/* clang-format on */

#define __comp_anon__HashMap_Pair$$(_K, _V...) \
    union { \
        struct { \
            debug_only(struct { \
                var_(key_ty, TypeInfo); \
                var_(val_ty, TypeInfo); \
            }); \
            union { \
                struct { \
                    var_(key, _K); \
                    var_(val, _V); \
                } data; \
                struct { \
                    var_(key, _K); \
                    var_(val, _V); \
                }; \
            }; \
        }; \
        var_(as_raw, HashMap_Pair$raw) $like_ref; \
    }
#define __comp_alias__HashMap_Pair$(_K, _V...) tpl_id$1T$2U(HashMap_Pair, _K, _V)
#define __comp_gen__T_decl_HashMap_Pair$(_K, _V...) \
    $maybe_unused typedef union HashMap_Pair$(_K, _V) HashMap_Pair$(_K, _V); \
    T_decl_O$(HashMap_Pair$(_K, _V)); \
    T_decl_E$($set(mem_Err)(O$(HashMap_Pair$(_K, _V))))
#define __comp_gen__T_impl_HashMap_Pair$(_K, _V...) \
    union HashMap_Pair$(_K, _V) { \
        struct { \
            debug_only(struct { \
                var_(key_ty, TypeInfo); \
                var_(val_ty, TypeInfo); \
            }); \
            union { \
                struct { \
                    var_(key, _K); \
                    var_(val, _V); \
                } data; \
                struct { \
                    var_(key, _K); \
                    var_(val, _V); \
                }; \
            }; \
        }; \
        var_(as_raw, HashMap_Pair$raw) $like_ref; \
    }; \
    T_impl_O$(HashMap_Pair$(_K, _V)); \
    T_impl_E$($set(mem_Err)(O$(HashMap_Pair$(_K, _V))))
#define __comp_gen__T_use_HashMap_Pair$(_K, _V...) \
    T_decl_HashMap_Pair$(_K, _V); \
    T_impl_HashMap_Pair$(_K, _V)

/* clang-format off */
#define T_use_HashMap_Pair_key$(_K, _V...) \
    $attr($inline_always) \
    $static fn_((tpl_id$1T$2U(HashMap_Pair_key, _K, _V)(HashMap_Pair$(_K, _V) self))(u_V$raw)) { \
        return u_castV$((_K)(HashMap_Pair_key(self.as_raw, typeInfo$(_V), u_retV$(_K)))); \
    }
#define T_use_HashMap_Pair_val$(_K, _V...) \
    $attr($inline_always) \
    $static fn_((tpl_id$1T$2U(HashMap_Pair_val, _K, _V)(HashMap_Pair$(_K, _V) self))(u_V$raw)) { \
        return u_castV$((_V)(HashMap_Pair_val(self.as_raw, typeInfo$(_K), u_retV$(_V)))); \
    }
/* clang-format on */

#define __comp_anon__HashMap_Entry$$(_K, _V...) \
    union { \
        struct { \
            var_(key, P_const$$(_K)); \
            var_(val, P_const$$(_V)); \
            debug_only(struct { \
                var_(key_ty, TypeInfo); \
                var_(val_ty, TypeInfo); \
            }); \
        }; \
        var_(as_raw, HashMap_Entry) $like_ref; \
    }
#define __comp_alias__HashMap_Entry$(_K, _V...) tpl_id$1T$2U(HashMap_Entry, _K, _V)
#define __comp_gen__T_decl_HashMap_Entry$(_K, _V...) \
    $maybe_unused typedef union HashMap_Entry$(_K, _V) HashMap_Entry$(_K, _V); \
    T_decl_O$(HashMap_Entry$(_K, _V))
#define __comp_gen__T_impl_HashMap_Entry$(_K, _V...) \
    union HashMap_Entry$(_K, _V) { \
        struct { \
            var_(key, P_const$$(_K)); \
            var_(val, P_const$$(_V)); \
            debug_only(struct { \
                var_(key_ty, TypeInfo); \
                var_(val_ty, TypeInfo); \
            }); \
        }; \
        var_(as_raw, HashMap_Entry) $like_ref; \
    }; \
    T_impl_O$(HashMap_Entry$(_K, _V))
#define __comp_gen__T_use_HashMap_Entry$(_K, _V...) \
    T_decl_HashMap_Entry$(_K, _V); \
    T_impl_HashMap_Entry$(_K, _V)

/* clang-format off */
#define T_use_HashMap_Entry_key$(_K, _V...) \
    $attr($inline_always) \
    $static fn_((tpl_id$1T$2U(HashMap_Entry_key, _K, _V)(HashMap_Entry$(_K, _V) self))(const _K*)) { \
        return u_castP$((const _K*)(HashMap_Entry_key(*self.as_raw, typeInfo$(_K)))); \
    }
#define T_use_HashMap_Entry_val$(_K, _V...) \
    $attr($inline_always) \
    $static fn_((tpl_id$1T$2U(HashMap_Entry_val, _K, _V)(HashMap_Entry$(_K, _V) self))(const _V*)) { \
        return u_castP$((const _V*)(HashMap_Entry_val(*self.as_raw, typeInfo$(_V)))); \
    }
/* clang-format on */

#define __comp_anon__HashMap_EntryMut$$(_K, _V...) \
    union { \
        union { \
            struct { \
                var_(key, P$$(_K)); \
                var_(val, P$$(_V)); \
                debug_only(struct { \
                    var_(key_ty, TypeInfo); \
                    var_(val_ty, TypeInfo); \
                }); \
            }; \
            var_(as_const, HashMap_Entry$$(_K, _V)); \
        }; \
        var_(as_raw, HashMap_EntryMut) $like_ref; \
    }
#define __comp_alias__HashMap_EntryMut$(_K, _V...) tpl_id$1T$2U(HashMap_EntryMut, _K, _V)
#define __comp_gen__T_decl_HashMap_EntryMut$(_K, _V...) \
    $maybe_unused typedef union HashMap_EntryMut$(_K, _V) HashMap_EntryMut$(_K, _V); \
    T_decl_O$(HashMap_EntryMut$(_K, _V))
#define __comp_gen__T_impl_HashMap_EntryMut$(_K, _V...) \
    union HashMap_EntryMut$(_K, _V) { \
        union { \
            struct { \
                var_(key, P$$(_K)); \
                var_(val, P$$(_V)); \
                debug_only(struct { \
                    var_(key_ty, TypeInfo); \
                    var_(val_ty, TypeInfo); \
                }); \
            }; \
            var_(as_const, HashMap_Entry$(_K, _V)); \
        }; \
        var_(as_raw, HashMap_EntryMut) $like_ref; \
    }; \
    T_impl_O$(HashMap_EntryMut$(_K, _V))
#define __comp_gen__T_use_HashMap_EntryMut$(_K, _V...) \
    T_decl_HashMap_EntryMut$(_K, _V); \
    T_impl_HashMap_EntryMut$(_K, _V)

/* clang-format off */
#define T_use_HashMap_EntryMut_key$(_K, _V...) \
    $attr($inline_always) \
    $static fn_((tpl_id$1T$2U(HashMap_EntryMut_key, _K, _V)(HashMap_EntryMut$(_K, _V) self))(_K*)) { \
        return u_castP$((_K*)(HashMap_EntryMut_key(*self.as_raw, typeInfo$(_K)))); \
    }
#define T_use_HashMap_EntryMut_val$(_K, _V...) \
    $attr($inline_always) \
    $static fn_((tpl_id$1T$2U(HashMap_EntryMut_val, _K, _V)(HashMap_EntryMut$(_K, _V) self))(_V*)) { \
        return u_castP$((_V*)(HashMap_EntryMut_val(*self.as_raw, typeInfo$(_V)))); \
    }
/* clang-format on */

#define __comp_anon__HashMap_Ensured$$(_K, _V...) \
    union { \
        struct { \
            var_(key, P$$(_K)); \
            var_(val, P$$(_V)); \
            var_(found_existing, bool); \
            debug_only(struct { \
                var_(key_ty, TypeInfo); \
                var_(val_ty, TypeInfo); \
            }); \
        }; \
        var_(as_raw, HashMap_Ensured) $like_ref; \
    }
#define __comp_alias__HashMap_Ensured$(_K, _V...) tpl_id$1T$2U(HashMap_Ensured, _K, _V)
#define __comp_gen__T_decl_HashMap_Ensured$(_K, _V...) \
    $maybe_unused typedef union HashMap_Ensured$(_K, _V) HashMap_Ensured$(_K, _V); \
    T_decl_O$(HashMap_Ensured$(_K, _V)); \
    T_decl_E$($set(mem_Err)(HashMap_Ensured$(_K, _V)))
#define __comp_gen__T_impl_HashMap_Ensured$(_K, _V...) \
    union HashMap_Ensured$(_K, _V) { \
        struct { \
            var_(key, P$$(_K)); \
            var_(val, P$$(_V)); \
            var_(found_existing, bool); \
            debug_only(struct { \
                var_(key_ty, TypeInfo); \
                var_(val_ty, TypeInfo); \
            }); \
        }; \
        var_(as_raw, HashMap_Ensured) $like_ref; \
    }; \
    T_impl_O$(HashMap_Ensured$(_K, _V)); \
    T_impl_E$($set(mem_Err)(HashMap_Ensured$(_K, _V)))
#define __comp_gen__T_use_HashMap_Ensured$(_K, _V...) \
    T_decl_HashMap_Ensured$(_K, _V); \
    T_impl_HashMap_Ensured$(_K, _V)

/* clang-format off */
#define T_use_HashMap_Ensured_key$(_K, _V...) \
    $attr($inline_always) \
    $static fn_((tpl_id$1T$2U(HashMap_Ensured_key, _K, _V)(HashMap_Ensured$(_K, _V) self))(const _K*)) { \
        return u_castP$((const _K*)(HashMap_Ensured_key(*self.as_raw, typeInfo$(_K)))); \
    }
#define T_use_HashMap_Ensured_keyMut$(_K, _V...) \
    $attr($inline_always) \
    $static fn_((tpl_id$1T$2U(HashMap_Ensured_keyMut, _K, _V)(HashMap_Ensured$(_K, _V) self))(_K*)) { \
        return u_castP$((_K*)(HashMap_Ensured_keyMut(*self.as_raw, typeInfo$(_K)))); \
    }
#define T_use_HashMap_Ensured_val$(_K, _V...) \
    $attr($inline_always) \
    $static fn_((tpl_id$1T$2U(HashMap_Ensured_val, _K, _V)(HashMap_Ensured$(_K, _V) self))(const _V*)) { \
        return u_castP$((const _V*)(HashMap_Ensured_val(*self.as_raw, typeInfo$(_V)))); \
    }
#define T_use_HashMap_Ensured_valMut$(_K, _V...) \
    $attr($inline_always) \
    $static fn_((tpl_id$1T$2U(HashMap_Ensured_valMut, _K, _V)(HashMap_Ensured$(_K, _V) self))(_V*)) { \
        return u_castP$((_V*)(HashMap_Ensured_valMut(*self.as_raw, typeInfo$(_V)))); \
    }
#define T_use_HashMap_Ensured_foundExisting$(_K, _V...) \
    $attr($inline_always) \
    $static fn_((tpl_id$1T$2U(HashMap_Ensured_foundExisting, _K, _V)( \
        HashMap_Ensured$(_K, _V) self \
    ))(O$(HashMap_Entry$(_K, _V)))) { \
        return u_castO$((O$(HashMap_Entry$(_K, _V)))( \
            HashMap_Ensured_foundExisting(*self.as_raw, typeInfo$(_K), typeInfo$(_V)) \
        )); \
    }
#define T_use_HashMap_Ensured_foundExistingMut$(_K, _V...) \
    $attr($inline_always) \
    $static fn_((tpl_id$1T$2U(HashMap_Ensured_foundExistingMut, _K, _V)( \
        HashMap_Ensured$(_K, _V) self \
    ))(O$(HashMap_EntryMut$(_K, _V)))) { \
        return u_castO$((O$(HashMap_EntryMut$(_K, _V)))( \
            HashMap_Ensured_foundExistingMut(*self.as_raw, typeInfo$(_K), typeInfo$(_V)) \
        )); \
    }
/* clang-format on */

#define __comp_anon__HashMap$$(_K, _V...) \
    union { \
        struct { \
            var_(metadata, O$P$HashMap_Ctrl); \
            var_(size, u32); \
            var_(available, u32); \
            var_(ctx, P_const$HashMap_Ctx); \
            debug_only(struct { \
                var_(key_ty, TypeInfo); \
                var_(val_ty, TypeInfo); \
            }); \
        }; \
        var_(as_raw, HashMap) $like_ref; \
    }
#define __comp_alias__HashMap$(_K, _V...) tpl_id$1T$2U(HashMap, _K, _V)
#define __comp_gen__T_decl_HashMap$(_K, _V...) \
    $maybe_unused typedef union HashMap$(_K, _V) HashMap$(_K, _V); \
    T_decl_E$($set(mem_Err)(HashMap$(_K, _V)))
#define __comp_gen__T_impl_HashMap$(_K, _V...) \
    union HashMap$(_K, _V) { \
        struct { \
            var_(metadata, O$P$HashMap_Ctrl); \
            var_(size, u32); \
            var_(available, u32); \
            var_(ctx, P_const$HashMap_Ctx); \
            debug_only(struct { \
                var_(key_ty, TypeInfo); \
                var_(val_ty, TypeInfo); \
            }); \
        }; \
        var_(as_raw, HashMap) $like_ref; \
    }; \
    T_impl_E$($set(mem_Err)(HashMap$(_K, _V)))
#define __comp_gen__T_use_HashMap$(_K, _V...) \
    T_decl_HashMap$(_K, _V); \
    T_impl_HashMap$(_K, _V)

/* clang-format off */
#define T_use_HashMap_empty$(_K, _V...) \
    $attr($inline_always) \
    $static fn_((tpl_id$1T$2U(HashMap_empty, _K, _V)( \
        P_const$HashMap_Ctx ctx \
    ))(HashMap$(_K, _V))) { \
        return type$((HashMap$(_K, _V))(HashMap_empty(typeInfo$(_K), typeInfo$(_V), ctx))); \
    }
#define T_use_HashMap_init$(_K, _V...) \
    $attr($inline_always $must_check) \
    $static fn_((tpl_id$1T$2U(HashMap_init, _K, _V)( \
        P_const$HashMap_Ctx ctx, mem_Allocator gpa, u32 cap \
    ))(E$($set(mem_Err)(HashMap$(_K, _V)))) $scope) { \
        return_(typeE$((ReturnT)(HashMap_init(typeInfo$(_K), typeInfo$(_V), ctx, gpa, cap)))); \
    } $unscoped_(fn)
#define T_use_HashMap_fini$(_K, _V...) \
    $attr($inline_always) \
    $static fn_((tpl_id$1T$2U(HashMap_fini, _K, _V)( \
        HashMap$(_K, _V)* self, mem_Allocator gpa \
    ))(void)) { \
        return HashMap_fini(self->as_raw, typeInfo$(_K), typeInfo$(_V), gpa); \
    }
#define T_use_HashMap_clone$(_K, _V...) \
    $attr($inline_always $must_check) \
    $static fn_((tpl_id$1T$2U(HashMap_clone, _K, _V)( \
        HashMap$(_K, _V) self, mem_Allocator gpa \
    ))(E$($set(mem_Err)(HashMap$(_K, _V)))) $scope) { \
        return_(typeE$((ReturnT)(HashMap_clone(*self.as_raw, typeInfo$(_K), typeInfo$(_V), gpa)))); \
    } $unscoped_(fn)
#define T_use_HashMap_cloneWithCtx$(_K, _V...) \
    $attr($inline_always $must_check) \
    $static fn_((tpl_id$1T$2U(HashMap_cloneWithCtx, _K, _V)( \
        HashMap$(_K, _V) self, P_const$HashMap_Ctx ctx, mem_Allocator gpa \
    ))(E$($set(mem_Err)(HashMap$(_K, _V)))) $scope) { \
        return_(typeE$((ReturnT)(HashMap_cloneWithCtx(*self.as_raw, typeInfo$(_K), typeInfo$(_V), ctx, gpa)))); \
    } $unscoped_(fn)

#define T_use_HashMap_count$(_K, _V...) \
    $attr($inline_always) \
    $static fn_((tpl_id$1T$2U(HashMap_count, _K, _V)(HashMap$(_K, _V) self))(u32)) { \
        return HashMap_count(*self.as_raw); \
    }
#define T_use_HashMap_cap$(_K, _V...) \
    $attr($inline_always) \
    $static fn_((tpl_id$1T$2U(HashMap_cap, _K, _V)(HashMap$(_K, _V) self))(u32)) { \
        return HashMap_cap(*self.as_raw); \
    }

#define T_use_HashMap_ensureCap$(_K, _V...) \
    $attr($inline_always $must_check) \
    $static fn_((tpl_id$1T$2U(HashMap_ensureCap, _K, _V)( \
        HashMap$(_K, _V)* self, mem_Allocator gpa, u32 new_cap \
    ))(mem_Err$void)) { \
        return HashMap_ensureCap(self->as_raw, typeInfo$(_K), typeInfo$(_V), gpa, new_cap); \
    }
#define T_use_HashMap_ensureUnusedCap$(_K, _V...) \
    $attr($inline_always $must_check) \
    $static fn_((tpl_id$1T$2U(HashMap_ensureUnusedCap, _K, _V)( \
        HashMap$(_K, _V)* self, mem_Allocator gpa, u32 additional \
    ))(mem_Err$void)) { \
        return HashMap_ensureUnusedCap(self->as_raw, typeInfo$(_K), typeInfo$(_V), gpa, additional); \
    }
#define T_use_HashMap_clearRetainingCap$(_K, _V...) \
    $attr($inline_always) \
    $static fn_((tpl_id$1T$2U(HashMap_clearRetainingCap, _K, _V)(HashMap$(_K, _V)* self))(void)) { \
        return HashMap_clearRetainingCap(self->as_raw); \
    }
#define T_use_HashMap_clearAndFree$(_K, _V...) \
    $attr($inline_always) \
    $static fn_((tpl_id$1T$2U(HashMap_clearAndFree, _K, _V)( \
        HashMap$(_K, _V)* self, mem_Allocator gpa \
    ))(void)) { \
        return HashMap_clearAndFree(self->as_raw, typeInfo$(_K), typeInfo$(_V), gpa); \
    }

#define T_use_HashMap_by$(_K, _V...) \
    $attr($inline_always) \
    $static fn_((tpl_id$1T$2U(HashMap_by, _K, _V)(HashMap$(_K, _V) self, _K key))(O$(_V))) { \
        return u_castO$((O$(_V))(HashMap_by(*self.as_raw, u_anyV(key), u_retV$(_V)))); \
    }
#define T_use_HashMap_ptrBy$(_K, _V...) \
    $attr($inline_always) \
    $static fn_((tpl_id$1T$2U(HashMap_ptrBy, _K, _V)(HashMap$(_K, _V) self, _K key))(O$(P_const$(_V)))) { \
        return u_castO$((O$(P_const$(_V)))(HashMap_ptrBy(*self.as_raw, typeInfo$(_V), u_anyV(key)))); \
    }
#define T_use_HashMap_ptrMutBy$(_K, _V...) \
    $attr($inline_always) \
    $static fn_((tpl_id$1T$2U(HashMap_ptrMutBy, _K, _V)(HashMap$(_K, _V) self, _K key))(O$(P$(_V)))) { \
        return u_castO$((O$(P$(_V)))(HashMap_ptrMutBy(*self.as_raw, typeInfo$(_V), u_anyV(key)))); \
    }

#define T_use_HashMap_for$(_K, _V...) \
    $attr($inline_always) \
    $static fn_((tpl_id$1T$2U(HashMap_for, _K, _V)(HashMap$(_K, _V) self, _K key))(O$(_K))) { \
        return u_castO$((O$(_K))(HashMap_for(*self.as_raw, typeInfo$(_V), u_anyV(key), u_retV$(_K)))); \
    }
#define T_use_HashMap_ptrFor$(_K, _V...) \
    $attr($inline_always) \
    $static fn_((tpl_id$1T$2U(HashMap_ptrFor, _K, _V)(HashMap$(_K, _V) self, _K key))(O$(P_const$(_K)))) { \
        return u_castO$((O$(P_const$(_K)))(HashMap_ptrFor(*self.as_raw, typeInfo$(_V), u_anyV(key)))); \
    }
#define T_use_HashMap_ptrMutFor$(_K, _V...) \
    $attr($inline_always) \
    $static fn_((tpl_id$1T$2U(HashMap_ptrMutFor, _K, _V)(HashMap$(_K, _V) self, _K key))(O$(P$(_K)))) { \
        return u_castO$((O$(P$(_K)))(HashMap_ptrMutFor(*self.as_raw, typeInfo$(_V), u_anyV(key)))); \
    }

#define T_use_HashMap_entry$(_K, _V...) \
    $attr($inline_always) \
    $static fn_((tpl_id$1T$2U(HashMap_entry, _K, _V)(HashMap$(_K, _V) self, _K key))(O$(HashMap_Entry$(_K, _V)))) { \
        return typeO$((O$(HashMap_Entry$(_K, _V)))(HashMap_entry(*self.as_raw, typeInfo$(_V), u_anyV(key)))); \
    }
#define T_use_HashMap_entryMut$(_K, _V...) \
    $attr($inline_always) \
    $static fn_((tpl_id$1T$2U(HashMap_entryMut, _K, _V)(HashMap$(_K, _V) self, _K key))(O$(HashMap_EntryMut$(_K, _V)))) { \
        return typeO$((O$(HashMap_EntryMut$(_K, _V)))(HashMap_entryMut(*self.as_raw, typeInfo$(_V), u_anyV(key)))); \
    }

#define T_use_HashMap_contains$(_K, _V...) \
    $attr($inline_always) \
    $static fn_((tpl_id$1T$2U(HashMap_contains, _K, _V)(HashMap$(_K, _V) self, _K key))(bool)) { \
        return HashMap_contains(*self.as_raw, typeInfo$(_V), u_anyV(key)); \
    }

#define T_use_HashMap_put$(_K, _V...) \
    $attr($inline_always $must_check) \
    $static fn_((tpl_id$1T$2U(HashMap_put, _K, _V)(HashMap$(_K, _V)* self, mem_Allocator gpa, _K key, _V val))(mem_Err$void)) { \
        return HashMap_put(self->as_raw, gpa, u_anyV(key), u_anyV(val)); \
    }
#define T_use_HashMap_putWithin$(_K, _V...) \
    $attr($inline_always) \
    $static fn_((tpl_id$1T$2U(HashMap_putWithin, _K, _V)(HashMap$(_K, _V)* self, _K key, _V val))(void)) { \
        return HashMap_putWithin(self->as_raw, u_anyV(key), u_anyV(val)); \
    }
#define T_use_HashMap_putNoClobber$(_K, _V...) \
    $attr($inline_always $must_check) \
    $static fn_((tpl_id$1T$2U(HashMap_putNoClobber, _K, _V)(HashMap$(_K, _V)* self, mem_Allocator gpa, _K key, _V val))(mem_Err$void)) { \
        return HashMap_putNoClobber(self->as_raw, gpa, u_anyV(key), u_anyV(val)); \
    }
#define T_use_HashMap_putNoClobberWithin$(_K, _V...) \
    $attr($inline_always) \
    $static fn_((tpl_id$1T$2U(HashMap_putNoClobberWithin, _K, _V)(HashMap$(_K, _V)* self, _K key, _V val))(void)) { \
        return HashMap_putNoClobberWithin(self->as_raw, u_anyV(key), u_anyV(val)); \
    }
#define T_use_HashMap_fetchPut$(_K, _V...) \
    $attr($inline_always $must_check) \
    $static fn_((tpl_id$1T$2U(HashMap_fetchPut, _K, _V)( \
        HashMap$(_K, _V)* self, mem_Allocator gpa, _K key, _V val \
    ))(E$($set(mem_Err)(O$(HashMap_Pair$(_K, _V))))) $scope) { \
        let opt_pair = try_(HashMap_fetchPut(self->as_raw, gpa, u_anyV(key), u_anyV(val), lit0$((HashMap_Pair$(_K, _V))).as_raw)); \
        let pair = orelse_((opt_pair)(return_ok(none()))); \
        return_ok(some(*as$(HashMap_Pair$(_K, _V)*)(pair))); \
    } $unscoped_(fn)
#define T_use_HashMap_fetchPutWithin$(_K, _V...) \
    $attr($inline_always) \
    $static fn_((tpl_id$1T$2U(HashMap_fetchPutWithin, _K, _V)( \
        HashMap$(_K, _V)* self, _K key, _V val \
    ))(O$(HashMap_Pair$(_K, _V))) $scope) { \
        let opt_pair = HashMap_fetchPutWithin(self->as_raw, u_anyV(key), u_anyV(val), lit0$((HashMap_Pair$(_K, _V))).as_raw); \
        let pair = orelse_((opt_pair)(return_none())); \
        return_some(*as$(HashMap_Pair$(_K, _V)*)(pair)); \
    } $unscoped_(fn)

#define T_use_HashMap_ensure$(_K, _V...) \
    $attr($inline_always $must_check) \
    $static fn_((tpl_id$1T$2U(HashMap_ensure, _K, _V)( \
        HashMap$(_K, _V)* self, mem_Allocator gpa, _K key \
    ))(E$($set(mem_Err)(HashMap_Ensured$(_K, _V)))) $scope) { \
        return_(typeE$((ReturnT)(HashMap_ensure(self->as_raw, typeInfo$(_V), gpa, u_anyV(key))))); \
    } $unscoped_(fn)
#define T_use_HashMap_ensureWithin$(_K, _V...) \
    $attr($inline_always) \
    $static fn_((tpl_id$1T$2U(HashMap_ensureWithin, _K, _V)( \
        HashMap$(_K, _V)* self, _K key \
    ))(HashMap_Ensured$(_K, _V))) { \
        return type$((HashMap_Ensured$(_K, _V))(HashMap_ensureWithin(self->as_raw, typeInfo$(_V), u_anyV(key)))); \
    }
#define T_use_HashMap_ensureValue$(_K, _V...) \
    $attr($inline_always $must_check) \
    $static fn_((tpl_id$1T$2U(HashMap_ensureValue, _K, _V)( \
        HashMap$(_K, _V)* self, mem_Allocator gpa, _K key, _V default_val \
    ))(E$($set(mem_Err)(HashMap_Ensured$(_K, _V)))) $scope) { \
        return_(typeE$((ReturnT)(HashMap_ensureValue(self->as_raw, gpa, u_anyV(key), u_anyV(default_val))))); \
    } $unscoped_(fn)

#define T_use_HashMap_remove$(_K, _V...) \
    $attr($inline_always) \
    $static fn_((tpl_id$1T$2U(HashMap_remove, _K, _V)(HashMap$(_K, _V)* self, _K key))(bool)) { \
        return HashMap_remove(self->as_raw, typeInfo$(_V), u_anyV(key)); \
    }
#define T_use_HashMap_fetchRemove$(_K, _V...) \
    $attr($inline_always) \
    $static fn_((tpl_id$1T$2U(HashMap_fetchRemove, _K, _V)(HashMap$(_K, _V)* self, _K key))(O$(HashMap_Pair$(_K, _V))) $scope) { \
        let opt_pair = HashMap_fetchRemove(self->as_raw, typeInfo$(_V), u_anyV(key), lit0$((HashMap_Pair$(_K, _V))).as_raw); \
        let pair = orelse_((opt_pair)(return_none())); \
        return_some(*as$(HashMap_Pair$(_K, _V)*)(pair)); \
    } $unscoped_(fn)
#define T_use_HashMap_removeByPtr$(_K, _V...) \
    $attr($inline_always) \
    $static fn_((tpl_id$1T$2U(HashMap_removeByPtr, _K, _V)(HashMap$(_K, _V)* self, _K* key))(void)) { \
        return HashMap_removeByPtr(self->as_raw, typeInfo$(_V), u_anyP(key)); \
    }

#define T_use_HashMap_rehash$(_K, _V...) \
    $attr($inline_always) \
    $static fn_((tpl_id$1T$2U(HashMap_rehash, _K, _V)(HashMap$(_K, _V)* self))(void)) { \
        return HashMap_rehash(self->as_raw, typeInfo$(_K), typeInfo$(_V)); \
    }
/* clang-format on */

#define __comp_anon__HashMap_Iter$$(_K, _V...) \
    union { \
        struct { \
            var_(map, P_const$$(HashMap$(_K, _V))); \
            var_(idx, usize); \
            debug_only(struct { \
                var_(key_ty, TypeInfo); \
                var_(val_ty, TypeInfo); \
            }); \
        }; \
        var_(as_raw, HashMap_Iter) $like_ref; \
    }
#define __comp_alias__HashMap_Iter$(_K, _V...) tpl_id$1T$2U(HashMap_Iter, _K, _V)
#define __comp_gen__T_decl_HashMap_Iter$(_K, _V...) \
    $maybe_unused typedef union HashMap_Iter$(_K, _V) HashMap_Iter$(_K, _V)
#define __comp_gen__T_impl_HashMap_Iter$(_K, _V...) \
    union HashMap_Iter$(_K, _V) { \
        struct { \
            var_(map, P_const$$(HashMap$(_K, _V))); \
            var_(idx, usize); \
            debug_only(struct { \
                var_(key_ty, TypeInfo); \
                var_(val_ty, TypeInfo); \
            }); \
        }; \
        var_(as_raw, HashMap_Iter) $like_ref; \
    }
#define __comp_gen__T_use_HashMap_Iter$(_K, _V...) \
    T_decl_HashMap_Iter$(_K, _V); \
    T_impl_HashMap_Iter$(_K, _V)

/* clang-format off */
#define T_use_HashMap_iter$(_K, _V...) \
    $attr($inline_always) \
    $static fn_((tpl_id$1T$2U(HashMap_iter, _K, _V)(const HashMap$(_K, _V)* self))(HashMap_Iter$(_K, _V))) { \
        return type$((HashMap_Iter$(_K, _V))(HashMap_iter(self->as_raw, typeInfo$(_K), typeInfo$(_V)))); \
    }
#define T_use_HashMap_Iter_next$(_K, _V...) \
    $attr($inline_always) \
    $static fn_((tpl_id$1T$2U(HashMap_Iter_next, _K, _V)(HashMap_Iter$(_K, _V)* self))(O$(HashMap_Entry$(_K, _V)))) { \
        return typeO$((O$(HashMap_Entry$(_K, _V)))(HashMap_Iter_next(self->as_raw, typeInfo$(_K), typeInfo$(_V)))); \
    }
#define T_use_HashMap_Iter_nextMut$(_K, _V...) \
    $attr($inline_always) \
    $static fn_((tpl_id$1T$2U(HashMap_Iter_nextMut, _K, _V)(HashMap_Iter$(_K, _V)* self))(O$(HashMap_EntryMut$(_K, _V)))) { \
        return typeO$((O$(HashMap_EntryMut$(_K, _V)))(HashMap_Iter_nextMut(self->as_raw, typeInfo$(_K), typeInfo$(_V)))); \
    }
/* clang-format on */

#define __comp_anon__HashMap_KeyIter$$(_K, _V...) \
    union { \
        struct { \
            var_(len, u32); \
            var_(metadata, P$HashMap_Ctrl); \
            var_(keys, P$$(_K)); \
            debug_only(var_(key_ty, TypeInfo)); \
        }; \
        var_(as_raw, HashMap_KeyIter) $like_ref; \
    }
#define __comp_alias__HashMap_KeyIter$(_K, _V...) tpl_id$1T$2U(HashMap_KeyIter, _K, _V)
#define __comp_gen__T_decl_HashMap_KeyIter$(_K, _V...) \
    $maybe_unused typedef union HashMap_KeyIter$(_K, _V) HashMap_KeyIter$(_K, _V)
#define __comp_gen__T_impl_HashMap_KeyIter$(_K, _V...) \
    union HashMap_KeyIter$(_K, _V) { \
        struct { \
            var_(len, u32); \
            var_(metadata, P$HashMap_Ctrl); \
            var_(keys, P$$(_K)); \
            debug_only(var_(key_ty, TypeInfo)); \
        }; \
        var_(as_raw, HashMap_KeyIter) $like_ref; \
    }
#define __comp_gen__T_use_HashMap_KeyIter$(_K, _V...) \
    T_decl_HashMap_KeyIter$(_K, _V); \
    T_impl_HashMap_KeyIter$(_K, _V)

/* clang-format off */
#define T_use_HashMap_keyIter$(_K, _V...) \
    $attr($inline_always) \
    $static fn_((tpl_id$1T$2U(HashMap_keyIter, _K, _V)(HashMap$(_K, _V) self))(HashMap_KeyIter$(_K, _V))) { \
        return type$((HashMap_KeyIter$(_K, _V))(HashMap_keyIter(*self.as_raw, typeInfo$(_K), typeInfo$(_V)))); \
    }
#define T_use_HashMap_KeyIter_next$(_K, _V...) \
    $attr($inline_always) \
    $static fn_((tpl_id$1T$2U(HashMap_KeyIter_next, _K, _V)(HashMap_KeyIter$(_K, _V)* self))(O$(P_const$(_K)))) { \
        return u_castO$((O$(P_const$(_K)))(HashMap_KeyIter_next(self->as_raw, typeInfo$(_K)))); \
    }
#define T_use_HashMap_KeyIter_nextMut$(_K, _V...) \
    $attr($inline_always) \
    $static fn_((tpl_id$1T$2U(HashMap_KeyIter_nextMut, _K, _V)(HashMap_KeyIter$(_K, _V)* self))(O$(P$(_K)))) { \
        return u_castO$((O$(P$(_K)))(HashMap_KeyIter_nextMut(self->as_raw, typeInfo$(_K)))); \
    }
/* clang-format on */

/* clang-format off */
#define __comp_anon__HashMap_ValIter$$(_K, _V...) \
    union { \
        struct { \
            var_(len, u32); \
            var_(metadata, P$HashMap_Ctrl); \
            var_(vals, P$$(_V)); \
            debug_only(var_(val_ty, TypeInfo)); \
        }; \
        var_(as_raw, HashMap_ValIter) $like_ref; \
    }
#define __comp_alias__HashMap_ValIter$(_K, _V...) tpl_id$1T$2U(HashMap_ValIter, _K, _V)
#define __comp_gen__T_decl_HashMap_ValIter$(_K, _V...) \
    $maybe_unused typedef union HashMap_ValIter$(_K, _V) HashMap_ValIter$(_K, _V)
#define __comp_gen__T_impl_HashMap_ValIter$(_K, _V...) \
    union HashMap_ValIter$(_K, _V) { \
        struct { \
            var_(len, u32); \
            var_(metadata, P$HashMap_Ctrl); \
            var_(vals, P$$(_V)); \
            debug_only(var_(val_ty, TypeInfo)); \
        }; \
        var_(as_raw, HashMap_ValIter) $like_ref; \
    }
#define __comp_gen__T_use_HashMap_ValIter$(_K, _V...) \
    T_decl_HashMap_ValIter$(_K, _V); \
    T_impl_HashMap_ValIter$(_K, _V)

/* clang-format off */
#define T_use_HashMap_valIter$(_K, _V...) \
    $attr($inline_always) \
    $static fn_((tpl_id$1T$2U(HashMap_valIter, _K, _V)(HashMap$(_K, _V) self))(HashMap_ValIter$(_K, _V))) { \
        return type$((HashMap_ValIter$(_K, _V))(HashMap_valIter(*self.as_raw, typeInfo$(_K), typeInfo$(_V)))); \
    }
#define T_use_HashMap_ValIter_next$(_K, _V...) \
    $attr($inline_always) \
    $static fn_((tpl_id$1T$2U(HashMap_ValIter_next, _K, _V)(HashMap_ValIter$(_K, _V)* self))(O$(P_const$(_V)))) { \
        return u_castO$((O$(P_const$(_V)))(HashMap_ValIter_next(self->as_raw, typeInfo$(_V)))); \
    }
#define T_use_HashMap_ValIter_nextMut$(_K, _V...) \
    $attr($inline_always) \
    $static fn_((tpl_id$1T$2U(HashMap_ValIter_nextMut, _K, _V)(HashMap_ValIter$(_K, _V)* self))(O$(P$(_V)))) { \
        return u_castO$((O$(P$(_V)))(HashMap_ValIter_nextMut(self->as_raw, typeInfo$(_V)))); \
    }
/* clang-format on */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* HashMap__included */

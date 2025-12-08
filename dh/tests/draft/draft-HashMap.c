/**
 * @copyright Copyright (c) 2025 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    HashMap.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2025-11-22 (date of creation)
 * @updated 2025-12-06 (date of last update)
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
static const HashMap_Ctrl HashMap_Ctrl_default = { .fingerprint = HashMap_Ctrl_free, .used = 0 };
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

/* --- HashMap_Pair: Key-Value pair --- */

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
T_use_O$(HashMap_Pair$raw);
T_use_E$($set(mem_Err)(O$HashMap_Pair$raw));
$extern fn_((HashMap_Pair_key(HashMap_Pair$raw* self, TypeInfo key_ty, u_V$raw ret_mem))(u_V$raw));
$extern fn_((HashMap_Pair_val(HashMap_Pair$raw* self, TypeInfo val_ty, u_V$raw ret_mem))(u_V$raw));

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

/* --- HashMap_EnsuredEntry: Result from ensure operations --- */

#define HashMap_EnsuredEntry$$(_K, _V...) __comp_anon__HashMap_EnsuredEntry$$(_K, _V)
#define HashMap_EnsuredEntry$(_K, _V...) __comp_alias__HashMap_EnsuredEntry$(_K, _V)
#define T_decl_HashMap_EnsuredEntry$(_K, _V...) __comp_gen__T_decl_HashMap_EnsuredEntry$(_K, _V)
#define T_impl_HashMap_EnsuredEntry$(_K, _V...) __comp_gen__T_impl_HashMap_EnsuredEntry$(_K, _V)
#define T_use_HashMap_EnsuredEntry$(_K, _V...) __comp_gen__T_use_HashMap_EnsuredEntry$(_K, _V)

typedef struct HashMap_EnsuredEntry {
    var_(key, P$raw);
    var_(val, P$raw);
    var_(found_existing, bool);
    debug_only(struct {
        var_(key_ty, TypeInfo);
        var_(val_ty, TypeInfo);
    });
} HashMap_EnsuredEntry;
T_use_E$($set(mem_Err)(HashMap_EnsuredEntry));
$extern fn_((HashMap_EnsuredEntry_key(HashMap_EnsuredEntry self, TypeInfo key_ty))(u_P_const$raw));
$extern fn_((HashMap_EnsuredEntry_keyMut(HashMap_EnsuredEntry self, TypeInfo key_ty))(u_P$raw));
$extern fn_((HashMap_EnsuredEntry_val(HashMap_EnsuredEntry self, TypeInfo val_ty))(u_P_const$raw));
$extern fn_((HashMap_EnsuredEntry_valMut(HashMap_EnsuredEntry self, TypeInfo val_ty))(u_P$raw));
$extern fn_((HashMap_EnsuredEntry_foundExisting(
    HashMap_EnsuredEntry self, TypeInfo key_ty, TypeInfo val_ty
))(O$HashMap_Entry));
$extern fn_((HashMap_EnsuredEntry_foundExistingMut(
    HashMap_EnsuredEntry self, TypeInfo key_ty, TypeInfo val_ty
))(O$HashMap_EntryMut));

$extern fn_((HashMap_defaultHash(u_V$raw val, u_V$raw ctx))(u64));
$extern fn_((HashMap_defaultEql(u_V$raw lhs, u_V$raw rhs, u_V$raw ctx))(bool));

/* --- HashMap_Ctx: Context for hash and equality functions --- */

typedef struct HashMap_Ctx {
    var_(inner, u_P$raw);
    var_(hashFn, u_HashCtxFn);
    var_(eqlFn, u_EqlCtxFn);
} HashMap_Ctx;
T_use_P$(HashMap_Ctx);
$static let_(HashMap_default_ctx, HashMap_Ctx) = {
    .inner = u_anyP(&(Void){}),
    .hashFn = HashMap_defaultHash,
    .eqlFn = HashMap_defaultEql,
};

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

$extern fn_((HashMap_empty(TypeInfo key_ty, TypeInfo val_ty, P_const$HashMap_Ctx ctx))(HashMap));
$attr($must_check)
$extern fn_((HashMap_init(TypeInfo key_ty, TypeInfo val_ty, P_const$HashMap_Ctx ctx, mem_Allocator gpa, u32 cap))(mem_Err$HashMap));
$extern fn_((HashMap_fini(HashMap* self, TypeInfo key_ty, TypeInfo val_ty, mem_Allocator gpa))(void));
$attr($must_check)
$extern fn_((HashMap_clone(HashMap self, TypeInfo key_ty, TypeInfo val_ty, mem_Allocator gpa))(mem_Err$HashMap));
$attr($must_check)
$extern fn_((HashMap_cloneWithCtx(HashMap self, TypeInfo key_ty, TypeInfo val_ty, P_const$HashMap_Ctx ctx, mem_Allocator gpa))(mem_Err$HashMap));

/* --- Capacity Management --- */

$extern fn_((HashMap_count(HashMap self))(u32));
$extern fn_((HashMap_cap(HashMap self))(u32));

$extern fn_((HashMap_ensureCap(
    HashMap* self, TypeInfo key_ty, TypeInfo val_ty, mem_Allocator gpa, usize new_cap
))(mem_Err$void)) $must_check;
$extern fn_((HashMap_ensureUnusedCap(
    HashMap* self, TypeInfo key_ty, TypeInfo val_ty, mem_Allocator gpa, usize additional
))(mem_Err$void)) $must_check;
$extern fn_((HashMap_clearRetainingCap(HashMap* self))(void));
$extern fn_((HashMap_clearAndFree(HashMap* self, TypeInfo key_ty, TypeInfo val_ty, mem_Allocator gpa))(void));

/* --- Lookup Operations --- */

$extern fn_((HashMap_by(HashMap self, u_V$raw key, u_V$raw ret_val))(O$u_V$raw));
$extern fn_((HashMap_ptrBy(HashMap self, u_V$raw key, TypeInfo val_ty))(O$u_P_const$raw));
$extern fn_((HashMap_ptrMutBy(HashMap self, u_V$raw key, TypeInfo val_ty))(O$u_P$raw));

$extern fn_((HashMap_for(HashMap self, u_V$raw key, TypeInfo val_ty, u_V$raw ret_key))(O$u_V$raw));
$extern fn_((HashMap_ptrFor(HashMap self, u_V$raw key, TypeInfo val_ty))(O$u_P_const$raw));
$extern fn_((HashMap_ptrMutFor(HashMap self, u_V$raw key, TypeInfo val_ty))(O$u_P$raw));

$extern fn_((HashMap_entry(HashMap self, u_V$raw key, TypeInfo val_ty))(O$HashMap_Entry));
$extern fn_((HashMap_entryMut(HashMap self, u_V$raw key, TypeInfo val_ty))(O$HashMap_EntryMut));

$extern fn_((HashMap_contains(HashMap self, u_V$raw key, TypeInfo val_ty))(bool));

/* --- Insertion Operations --- */

/// Insert or update. May allocate.
$extern fn_((HashMap_put(HashMap* self, mem_Allocator gpa, u_V$raw key, u_V$raw val))(mem_Err$void)) $must_check;
/// Insert or update. Asserts capacity is available.
$extern fn_((HashMap_putWithin(HashMap* self, u_V$raw key, u_V$raw val))(void));
/// Insert only if not present. May allocate.
$extern fn_((HashMap_putNoClobber(HashMap* self, mem_Allocator gpa, u_V$raw key, u_V$raw val))(mem_Err$void)) $must_check;
/// Insert only if not present. Asserts capacity and key not present.
$extern fn_((HashMap_putNoClobberWithin(HashMap* self, u_V$raw key, u_V$raw val))(void));

/* --- Fetch Operations (return previous value if any) --- */

/// Insert or update, returning previous value if it existed. May allocate.
$extern fn_((HashMap_fetchPut(
    HashMap* self, mem_Allocator gpa, u_V$raw key, u_V$raw val
))(mem_Err$O$HashMap_Pair$raw)) $must_check;
/// Insert or update, returning previous value if it existed. Asserts capacity.
$extern fn_((HashMap_fetchPutWithin(
    HashMap* self, u_V$raw key, u_V$raw val
))(O$HashMap_Pair$raw));

/* --- Ensure Operations --- */

/// Get existing entry or insert new one. May allocate.
$extern fn_((HashMap_ensure(
    HashMap* self, TypeInfo val_ty, mem_Allocator gpa, u_V$raw key
))(mem_Err$HashMap_EnsuredEntry)) $must_check;
/// Get existing entry or insert new one. Asserts capacity is available.
$extern fn_((HashMap_ensureWithin(
    HashMap* self, TypeInfo val_ty, u_V$raw key
))(HashMap_EnsuredEntry));
/// Get existing entry or insert with default value. May allocate.
$extern fn_((HashMap_ensureValue(
    HashMap* self,
    mem_Allocator gpa,
    u_V$raw key,
    u_V$raw default_val
))(mem_Err$HashMap_EnsuredEntry)) $must_check;

/* --- Removal Operations --- */

/// Remove entry if present, returning true if removed.
$extern fn_((HashMap_remove(HashMap* self, TypeInfo key_ty, TypeInfo val_ty, u_V$raw key))(bool));
/// Remove entry if present, returning the removed key-value pair.
$extern fn_((HashMap_fetchRemove(HashMap* self, TypeInfo key_ty, TypeInfo val_ty, u_V$raw key))(O$HashMap_Pair$raw));
/// Remove entry by key pointer (must be valid pointer into map).
$extern fn_((HashMap_removeByPtr(HashMap* self, TypeInfo key_ty, TypeInfo val_ty, P$raw key_ptr))(void));

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
        var_(as_raw, HashMap_Pair) $like_ref; \
    }
#define __comp_alias__HashMap_Pair$(_K, _V...) tpl_id$1T$2U(HashMap_Pair, _K, _V)
#define __comp_gen__T_decl_HashMap_Pair$(_K, _V...) \
    $maybe_unused typedef union HashMap_Pair$(_K, _V) HashMap_Pair$(_K, _V)
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
        var_(as_raw, HashMap_Pair) $like_ref; \
    }
#define __comp_gen__T_use_HashMap_Pair$(_K, _V...) \
    T_decl_HashMap_Pair$(_K, _V); \
    T_impl_HashMap_Pair$(_K, _V)

/* clang-format off */

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

/* clang-format on */

#define __comp_anon__HashMap_EnsuredEntry$$(_K, _V...) \
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
        var_(as_raw, HashMap_EnsuredEntry) $like_ref; \
    }
#define __comp_alias__HashMap_EnsuredEntry$(_K, _V...) tpl_id$1T$2U(HashMap_EnsuredEntry, _K, _V)
#define __comp_gen__T_decl_HashMap_EnsuredEntry$(_K, _V...) \
    $maybe_unused typedef union HashMap_EnsuredEntry$(_K, _V) HashMap_EnsuredEntry$(_K, _V); \
    T_decl_O$(HashMap_EnsuredEntry$(_K, _V))
#define __comp_gen__T_impl_HashMap_EnsuredEntry$(_K, _V...) \
    union HashMap_EnsuredEntry$(_K, _V) { \
        struct { \
            var_(key, P$$(_K)); \
            var_(val, P$$(_V)); \
            var_(found_existing, bool); \
            debug_only(struct { \
                var_(key_ty, TypeInfo); \
                var_(val_ty, TypeInfo); \
            }); \
        }; \
        var_(as_raw, HashMap_EnsuredEntry) $like_ref; \
    }; \
    T_impl_O$(HashMap_EnsuredEntry$(_K, _V))
#define __comp_gen__T_use_HashMap_EnsuredEntry$(_K, _V...) \
    T_decl_HashMap_EnsuredEntry$(_K, _V); \
    T_impl_HashMap_EnsuredEntry$(_K, _V)

/* clang-format off */

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

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* HashMap__included */

fn_((HashMap_defaultHash(u_V$raw val, u_V$raw ctx))(u64)) {
    let_ignore = ctx;
    // Wyhash-like mixing for general purpose hashing
    // Uses FNV-1a as a simple but effective default
    let bytes = slice$P(as$(const u8*)(val.inner), $r(0, val.inner_type.size));
    var_(hash, u64) = 0xcbf29ce484222325ull; // FNV offset basis
    for_(($s(bytes))(byte) {
        hash ^= *byte;
        hash *= 0x100000001b3ull; // FNV prime
    });
    return hash;
}

fn_((HashMap_defaultEql(u_V$raw lhs, u_V$raw rhs, u_V$raw ctx))(bool)) {
    let_ignore = ctx;
    return u_eql(lhs, rhs);
}

fn_((HashMap_empty(TypeInfo key_ty, TypeInfo val_ty, P_const$HashMap_Ctx ctx))(HashMap)) {
    claim_assert_nonnull(ctx);
    return (HashMap){
        .metadata = none(),
        .size = 0,
        .available = 0,
        .ctx = ctx,
        debug_only(.key_ty = key_ty, .val_ty = val_ty),
    };
};

#include "dh/main.h"
#include "dh/heap/Page.h"

TEST_fn_("basic usage" $guard) {
    var heap = (heap_Page){};
    let gpa = heap_Page_allocator(&heap);
    var map_value = try_(HashMap_init(
        typeInfo$(u32), typeInfo$(u32), &HashMap_default_ctx, gpa, 16
    ));
    defer_(HashMap_fini(&map_value, typeInfo$(u32), typeInfo$(u32), gpa));

    let_(count, u32) = 5;
    var_(total, u32) = 0;
    for_(($r(0, count))(i) {
        try_(HashMap_put(&map_value, gpa, u_anyV(i), u_anyV(i)));
        total += i;
    });

    with_(var_(sum, u32) = 0) {
        var it = HashMap_iter(&map_value, typeInfo$(u32), typeInfo$(u32));
        while_some(HashMap_Iter_next(&it, typeInfo$(u32), typeInfo$(u32)), entry) {
            sum += *u_castP$((const u32*)(HashMap_Entry_key(entry, typeInfo$(u32))));
        }
        try_(TEST_expect(sum == total));
    }

    with_(var_(sum, u32) = 0) {
        for_(($r(0, count))(i) {
            try_(TEST_expect(i == unwrap_(u_castO$((O$u32)(HashMap_by(map_value, u_anyV(i), u_retV$(u32)))))));
            sum += unwrap_(u_castO$((O$u32)(HashMap_by(map_value, u_anyV(i), u_retV$(u32)))));
        });
        try_(TEST_expect(sum == total));
    }
} $unguarded_(TEST_fn);

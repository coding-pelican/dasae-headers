#include "dh/HashMap.h"
#include "dh/meta.h"

/*========== SIMD Configuration =============================================*/

#ifndef HashMap_use_simd
#if defined(__SSE2__) || defined(_M_X64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 2)
#define HashMap_use_simd 1
#define HashMap_simd_sse2 1
#elif defined(__ARM_NEON) || defined(__ARM_NEON__)
#define HashMap_use_simd 1
#define HashMap_simd_neon 1
#else
#define HashMap_use_simd 0
#endif
#endif /* !HashMap_use_simd */

#if HashMap_use_simd
#if HashMap_simd_sse2
#include <emmintrin.h> /* SSE2 */
#elif HashMap_simd_neon
#include <arm_neon.h> /* NEON */
#endif
#endif /* HashMap_use_simd */

#if HashMap_use_simd
#define HashMap_simd_group_size 16 /* 128-bit SIMD = 16 bytes */
#endif                             /* HashMap_use_simd */

/*========== Definitions ====================================================*/

fn_((HashMap_Pair_init(u_V$raw key, u_V$raw val, V$HashMap_Pair$raw ret_mem))(V$HashMap_Pair$raw)) {
    debug_only({
        ret_mem->key_ty = key.type;
        ret_mem->val_ty = val.type;
    });
    let self_type = u_typeInfoRecord(typeInfosFrom(key.type, val.type));
    let p_self = (u_V$raw){ .type = self_type, .inner = ret_mem->data.inner }.ref;
    let p_key = u_fieldPtrMut(p_self, typeInfosFrom(key.type, val.type), 0);
    let p_val = u_fieldPtrMut(p_self, typeInfosFrom(key.type, val.type), 1);
    u_memcpy(p_key, key.ref.as_const);
    u_memcpy(p_val, val.ref.as_const);
    return ret_mem;
};

fn_((HashMap_Pair_key(V$HashMap_Pair$raw self, TypeInfo val_ty, u_V$raw ret_mem))(u_V$raw)) {
    debug_assert_nonnull(ret_mem.inner);
    debug_assert_eqBy(self->key_ty, ret_mem.type, TypeInfo_eq);
    debug_assert_eqBy(self->val_ty, val_ty, TypeInfo_eq);
    let self_type = u_typeInfoRecord(typeInfosFrom(ret_mem.type, val_ty));
    let p_self = u_load((u_V$raw){ .type = self_type, .inner = self->data.inner }).ref;
    let p_key = u_fieldPtr(p_self.as_const, typeInfosFrom(ret_mem.type, val_ty), 0);
    return u_deref(u_memcpy(ret_mem.ref, p_key));
};

fn_((HashMap_Pair_val(V$HashMap_Pair$raw self, TypeInfo key_ty, u_V$raw ret_mem))(u_V$raw)) {
    debug_assert_nonnull(ret_mem.inner);
    debug_assert_eqBy(self->key_ty, key_ty, TypeInfo_eq);
    debug_assert_eqBy(self->val_ty, ret_mem.type, TypeInfo_eq);
    let self_type = u_typeInfoRecord(typeInfosFrom(key_ty, ret_mem.type));
    let p_self = u_load((u_V$raw){ .type = self_type, .inner = self->data.inner }).ref;
    let p_val = u_fieldPtr(p_self.as_const, typeInfosFrom(key_ty, ret_mem.type), 1);
    return u_deref(u_memcpy(ret_mem.ref, p_val));
};

fn_((HashMap_Entry_key(HashMap_Entry self, TypeInfo key_ty))(u_P_const$raw)) {
    debug_assert_eqBy(self.key_ty, key_ty, TypeInfo_eq);
    return (u_P_const$raw){ .raw = self.key, .type = key_ty };
};

fn_((HashMap_Entry_val(HashMap_Entry self, TypeInfo val_ty))(u_P_const$raw)) {
    debug_assert_eqBy(self.val_ty, val_ty, TypeInfo_eq);
    return (u_P_const$raw){ .raw = self.val, .type = val_ty };
};

fn_((HashMap_EntryMut_key(HashMap_EntryMut self, TypeInfo key_ty))(u_P$raw)) {
    debug_assert_eqBy(self.key_ty, key_ty, TypeInfo_eq);
    return (u_P$raw){ .raw = self.key, .type = key_ty };
};

fn_((HashMap_EntryMut_val(HashMap_EntryMut self, TypeInfo val_ty))(u_P$raw)) {
    debug_assert_eqBy(self.val_ty, val_ty, TypeInfo_eq);
    return (u_P$raw){ .raw = self.val, .type = val_ty };
};

fn_((HashMap_Ensured_key(HashMap_Ensured self, TypeInfo key_ty))(u_P_const$raw)) {
    debug_assert_eqBy(self.key_ty, key_ty, TypeInfo_eq);
    return (u_P_const$raw){ .raw = self.key, .type = key_ty };
};

fn_((HashMap_Ensured_keyMut(HashMap_Ensured self, TypeInfo key_ty))(u_P$raw)) {
    debug_assert_eqBy(self.key_ty, key_ty, TypeInfo_eq);
    return (u_P$raw){ .raw = self.key, .type = key_ty };
};

fn_((HashMap_Ensured_val(HashMap_Ensured self, TypeInfo val_ty))(u_P_const$raw)) {
    debug_assert_eqBy(self.val_ty, val_ty, TypeInfo_eq);
    return (u_P_const$raw){ .raw = self.val, .type = val_ty };
};

fn_((HashMap_Ensured_valMut(HashMap_Ensured self, TypeInfo val_ty))(u_P$raw)) {
    debug_assert_eqBy(self.val_ty, val_ty, TypeInfo_eq);
    return (u_P$raw){ .raw = self.val, .type = val_ty };
};

fn_((HashMap_Ensured_foundExisting(
    HashMap_Ensured self, TypeInfo key_ty, TypeInfo val_ty
))(O$HashMap_Entry)) {
    return expr_(O$HashMap_Entry $scope)(if (self.found_existing) {
        $break_(some({ .key = self.key, .val = self.val, debug_only(.key_ty = key_ty, .val_ty = val_ty) }));
    }) expr_(else)({
        $break_(none());
    }) $unscoped_(expr);
};

fn_((HashMap_Ensured_foundExistingMut(
    HashMap_Ensured self, TypeInfo key_ty, TypeInfo val_ty
))(O$HashMap_EntryMut)) {
    return expr_(O$HashMap_EntryMut $scope)(if (self.found_existing) {
        $break_(some({ .key = self.key, .val = self.val, debug_only(.key_ty = key_ty, .val_ty = val_ty) }));
    }) expr_(else)({
        $break_(none());
    }) $unscoped_(expr);
};

fn_((HashMap_HashFn_default(u_V$raw val, u_V$raw ctx))(u64)) {
    let_ignore = ctx;
    // Wyhash-like mixing for general purpose hashing
    // Uses FNV-1a as a simple but effective default
    let bytes = slice$P(as$(const u8*)(val.inner), $r(0, val.type.size));
    var_(hash, u64) = 0xcbf29ce484222325ull; // FNV offset basis
    for_(($s(bytes))(byte) {
        hash ^= *byte;
        hash *= 0x100000001b3ull; // FNV prime
    });
    return hash;
};

fn_((HashMap_EqlFn_default(u_V$raw lhs, u_V$raw rhs, u_V$raw ctx))(bool)) {
    let_ignore = ctx;
    return u_eql(lhs, rhs);
};

fn_((HashMap_Ctx_default(void))(P_const$HashMap_Ctx)) {
    $static let_(default_ctx_inner, Void) = {};
    $static let_(default_ctx, HashMap_Ctx) = {
        .inner = u_anyP(&default_ctx_inner),
        .hashFn = HashMap_HashFn_default,
        .eqlFn = HashMap_EqlFn_default,
    };
    return &default_ctx;
};

$static fn_((HashMap__header(HashMap self))(HashMap_Header*)) {
    let metadata_ptr = unwrap_(self.metadata);
    return as$(HashMap_Header*)(as$(u8*)(metadata_ptr)-sizeOf$(HashMap_Header));
};

$static fn_((HashMap__keys(HashMap self, TypeInfo key_ty))(u_P$raw)) {
    return (u_P$raw){ .raw = HashMap__header(self)->keys, .type = key_ty };
};

$static fn_((HashMap__vals(HashMap self, TypeInfo val_ty))(u_P$raw)) {
    return (u_P$raw){ .raw = HashMap__header(self)->vals, .type = val_ty };
};

$static fn_((HashMap__keyAt(HashMap self, TypeInfo key_ty, usize idx))(u_P$raw)) {
    return (u_P$raw){ .raw = as$(u8*)(HashMap__keys(self, key_ty).raw) + idx * key_ty.size, .type = key_ty };
};

$static fn_((HashMap__valAt(HashMap self, TypeInfo val_ty, usize idx))(u_P$raw)) {
    return (u_P$raw){ .raw = as$(u8*)(HashMap__vals(self, val_ty).raw) + idx * val_ty.size, .type = val_ty };
};

$static fn_((HashMap__metadataAt(HashMap self, usize idx))(HashMap_Ctrl*)) {
    return unwrap_(self.metadata) + idx;
};

// $static fn_((HashMap__isUnderMaxLoad(u32 size, u32 cap))(bool)) {
//     return as$(u64)(size) * 100 < as$(u64)(HashMap_default_max_load_ratio) * as$(u64)(cap);
// };

$static fn_((HashMap__capacityForSize(u32 size))(u32)) {
    var_(new_cap, u64) = (as$(u64)(size) * 100) / HashMap_default_max_load_ratio + 1;
    // Round up to power of 2
    new_cap--;
    new_cap |= new_cap >> 1;
    new_cap |= new_cap >> 2;
    new_cap |= new_cap >> 4;
    new_cap |= new_cap >> 8;
    new_cap |= new_cap >> 16;
    new_cap |= new_cap >> 32;
    new_cap++;
    return as$(u32)(new_cap);
};

$static fn_((HashMap__initMetadata(HashMap* self))(void)) {
    mem_set0(u_anyS(slice$P(unwrap_(self->metadata), $r(0, HashMap_cap(*self)))));
};

$static fn_((HashMap__alloc(HashMap* self, TypeInfo key_ty, TypeInfo val_ty, mem_Allocator gpa, u32 new_cap))(mem_Err$void) $scope) {
    let header_align = alignOf$(HashMap_Header);
    let key_align = key_ty.size == 0 ? 1 : mem_log2ToAlign(key_ty.align);
    let val_align = val_ty.size == 0 ? 1 : mem_log2ToAlign(val_ty.align);
    let max_align = prim_max(header_align, prim_max(key_align, val_align));

    let meta_size = sizeOf$(HashMap_Header) + as$(usize)(new_cap)*sizeOf$(HashMap_Ctrl);
    let keys_start = mem_alignFwd(meta_size, key_align);
    let keys_end = keys_start + as$(usize)(new_cap)*key_ty.size;
    let vals_start = mem_alignFwd(keys_end, val_align);
    let vals_end = vals_start + as$(usize)(new_cap)*val_ty.size;
    let total_size = mem_alignFwd(vals_end, max_align);

    let slice = u_castS$((S$u8)(try_(mem_Allocator_alloc(gpa, typeInfo$(u8), total_size))));
    let ptr = as$(u8*)(slice.ptr);
    let hdr = as$(HashMap_Header*)(ptr);
    hdr->vals = ptr + vals_start;
    hdr->keys = ptr + keys_start;
    hdr->cap = new_cap;
    debug_only({
        hdr->key_ty = key_ty;
        hdr->val_ty = val_ty;
    });

    asg_lit((&self->metadata)(some(as$(HashMap_Ctrl*)(ptr + sizeOf$(HashMap_Header)))));
    return_ok({});
} $unscoped_(fn);

$static fn_((HashMap__free(HashMap* self, TypeInfo key_ty, TypeInfo val_ty, mem_Allocator gpa))(void)) {
    if_none(self->metadata) { return; }

    let header_align = alignOf$(HashMap_Header);
    let key_align = key_ty.size == 0 ? 1 : (1ull << key_ty.align);
    let val_align = val_ty.size == 0 ? 1 : (1ull << val_ty.align);
    let max_align = prim_max(header_align, prim_max(key_align, val_align));

    let cap = HashMap_cap(*self);
    let meta_size = sizeOf$(HashMap_Header) + as$(usize)(cap)*sizeOf$(HashMap_Ctrl);
    let keys_start = mem_alignFwd(meta_size, key_align);
    let keys_end = keys_start + as$(usize)(cap)*key_ty.size;
    let vals_start = mem_alignFwd(keys_end, val_align);
    let vals_end = vals_start + as$(usize)(cap)*val_ty.size;
    let total_size = mem_alignFwd(vals_end, max_align);

    let ptr = as$(u8*)(HashMap__header(*self));
    mem_Allocator_free(gpa, (u_S$raw){ .ptr = ptr, .len = total_size, .type = typeInfo$(u8) });

    asg_lit((&self->metadata)(none()));
    self->available = 0;
};

#if HashMap_use_simd
$static fn_((HashMap__idx_simd(HashMap self, u_V$raw key))(O$usize));
#endif /* HashMap_use_simd */
$static fn_((HashMap__idx(HashMap self, u_V$raw key))(O$usize) $scope) {
#if HashMap_use_simd
    /* Use SIMD path when capacity is aligned to group size */
    if (HashMap_cap(self) >= HashMap_simd_group_size && (HashMap_cap(self) % HashMap_simd_group_size) == 0) {
        return HashMap__idx_simd(self, key);
    }
#endif /* HashMap_use_simd */

    if (self.size == 0) { return_none(); }

    let ctx = self.ctx;
    let hash = ctx->hashFn(key, u_load(u_deref(ctx->inner)));

    let cap = HashMap_cap(self);
    let mask = cap - 1;
    let fingerprint = HashMap_Ctrl_takeFingerprint(hash);
    var_(idx, usize) = hash & mask;
    var_(limit, u32) = cap;
    while (limit > 0) {
        let ctrl = *HashMap__metadataAt(self, idx);
        if (HashMap_Ctrl_isFree(ctrl)) {
            return_none();
        }
        if (HashMap_Ctrl_isUsed(ctrl) && ctrl.fingerprint == fingerprint) {
            if (ctx->eqlFn(key, u_load(u_deref(HashMap__keyAt(self, key.type, idx))), u_load(u_deref(ctx->inner)))) {
                return_some(idx);
            }
        }
        limit--;
        idx = (idx + 1) & mask;
    }

    return_none();
} $unscoped_(fn);

#if HashMap_use_simd
/// Count trailing zeros (find first set bit position)
$attr($inline_always)
$static fn_((HashMap__ctz(u32 x))(u32));
/// Load 16 control bytes and find matches for fingerprint
$attr($inline_always)
$static fn_((HashMap__simd_match_fingerprint(const HashMap_Ctrl* group, u8 fingerprint))(u32));
/// Find free (not tombstone) slots in a group
$attr($inline_always)
$static fn_((HashMap__simd_match_free(const HashMap_Ctrl* group))(u32));
/// SIMD-accelerated lookup
$static fn_((HashMap__idx_simd(HashMap self, u_V$raw key))(O$usize) $scope) {
    if (self.size == 0) { return_none(); }

    let ctx = self.ctx;
    let hash = ctx->hashFn(key, u_load(u_deref(ctx->inner)));
    let cap = HashMap_cap(self);
    let fingerprint = HashMap_Ctrl_takeFingerprint(hash);

    let start_idx = hash & (cap - 1);
    let start_group = start_idx / HashMap_simd_group_size;
    let start_offset_in_group = start_idx % HashMap_simd_group_size;
    let num_groups = cap / HashMap_simd_group_size;

    for_(($r(0, num_groups))(group_offset) {
        let group_idx = (start_group + group_offset) % num_groups;
        let group_start = group_idx * HashMap_simd_group_size;
        let group = HashMap__metadataAt(self, group_start);
        /* SIMD: Check all 16 control bytes for fingerprint match */
        var_(match_mask, u32) = HashMap__simd_match_fingerprint(group, fingerprint);
        /* Iterate over matching positions */
        while (match_mask != 0) {
            let bit_pos = HashMap__ctz(match_mask);
            let idx = group_start + bit_pos;
            /* Verify with full equality check */
            if (ctx->eqlFn(key, u_load(u_deref(HashMap__keyAt(self, key.type, idx))), u_load(u_deref(ctx->inner)))) {
                return_some(idx);
            }
            /* Clear this bit and continue */
            match_mask &= match_mask - 1;
        }
        /* Check if this group has any free slots that would terminate the probe */
        var_(free_mask, u32) = HashMap__simd_match_free(group);
        /* For the first group in probe order, only consider free slots at positions
         * >= start_offset_in_group (positions in the actual probe sequence).
         * Free slots before start_offset_in_group are not yet part of the probe. */
        if (group_offset == 0 && start_offset_in_group > 0) {
            free_mask &= (~0u << start_offset_in_group);
        }
        if (free_mask != 0) {
            return_none();
        }
    });

    return_none();
} $unscoped_(fn);

$attr($inline_always)
$static fn_((HashMap__ctz(u32 x))(u32)) {
#if defined(__clang__) || defined(__GNUC__)
    return x ? as$(u32)(__builtin_ctz(x)) : 32;
#elif defined(_MSC_VER)
    unsigned long idx;
    return _BitScanForward(&idx, x) ? as$(u32)(idx) : 32;
#else
    /* Fallback */
    if (x == 0) return 32;
    var_(n, u32) = 0;
    if ((x & 0x0000FFFF) == 0) {
        n += 16;
        x >>= 16;
    }
    if ((x & 0x000000FF) == 0) {
        n += 8;
        x >>= 8;
    }
    if ((x & 0x0000000F) == 0) {
        n += 4;
        x >>= 4;
    }
    if ((x & 0x00000003) == 0) {
        n += 2;
        x >>= 2;
    }
    if ((x & 0x00000001) == 0) { n += 1; }
    return n;
#endif
};
#if HashMap_simd_sse2
/* --- SSE2 Implementation --- */
$attr($inline_always)
$static fn_((HashMap__simd_match_fingerprint(const HashMap_Ctrl* group, u8 fingerprint))(u32)) {
    /* Create a vector with the fingerprint in all 16 lanes */
    let needle = _mm_set1_epi8(as$(i8)(fingerprint | 0x80)); /* Set used bit */
    /* Load 16 control bytes */
    let haystack = _mm_loadu_si128(as$(const __m128i*)(group));
    /* Compare for equality - returns 0xFF for matches, 0x00 for non-matches */
    let cmp = _mm_cmpeq_epi8(needle, haystack);
    /* Extract comparison results to bitmask (1 bit per byte) */
    return as$(u32)(_mm_movemask_epi8(cmp));
};
$attr($inline_always)
$static fn_((HashMap__simd_match_free(const HashMap_Ctrl* group))(u32)) {
    /* Free slots are exactly 0x00 */
    let zero = _mm_setzero_si128();
    let haystack = _mm_loadu_si128(as$(const __m128i*)(group));
    let cmp = _mm_cmpeq_epi8(zero, haystack);
    return as$(u32)(_mm_movemask_epi8(cmp));
};
#endif /* HashMap_simd_sse2 */
#if HashMap_simd_neon
/* --- NEON Implementation --- */
$attr($inline_always)
$static fn_((HashMap__simd_match_fingerprint(const HashMap_Ctrl* group, u8 fingerprint))(u32)) {
    /* Create vector with fingerprint (with used bit set) in all lanes */
    let needle = vdupq_n_u8(fingerprint | 0x80);

    /* Load 16 control bytes */
    let haystack = vld1q_u8(as$(const u8*)(group));

    /* Compare for equality */
    let cmp = vceqq_u8(needle, haystack);

    /* Convert to bitmask - NEON doesn't have movemask, need to reduce */
    /* Use vshrn to narrow and combine results */
    let narrowed = vshrn_n_u16(vreinterpretq_u16_u8(cmp), 4);
    return vget_lane_u64(vreinterpret_u64_u8(narrowed), 0) & 0xFFFF;
};
$attr($inline_always)
$static fn_((HashMap__simd_match_free(const HashMap_Ctrl* group))(u32)) {
    let zero = vdupq_n_u8(0);
    let haystack = vld1q_u8(as$(const u8*)(group));
    let cmp = vceqq_u8(zero, haystack);
    let narrowed = vshrn_n_u16(vreinterpretq_u16_u8(cmp), 4);
    return vget_lane_u64(vreinterpret_u64_u8(narrowed), 0) & 0xFFFF;
};
#endif /* HashMap_simd_neon */
#endif /* HashMap_use_simd */

$static fn_((HashMap__grow(HashMap* self, TypeInfo key_ty, TypeInfo val_ty, mem_Allocator gpa, u32 new_capacity))(mem_Err$void) $scope) {
    let new_cap = prim_max(new_capacity, HashMap_default_min_cap);
    claim_assert(new_cap > HashMap_cap(*self));

    var_(new_map, HashMap) = HashMap_empty(key_ty, val_ty, self->ctx);
    try_(HashMap__alloc(&new_map, key_ty, val_ty, gpa, new_cap));
    HashMap__initMetadata(&new_map);
    new_map.available = (new_cap * HashMap_default_max_load_ratio) / 100;

    if (self->size != 0) {
        let old_cap = HashMap_cap(*self);
        for_(($r(0, old_cap))(i) {
            let ctrl = *HashMap__metadataAt(*self, i);
            if (!HashMap_Ctrl_isUsed(ctrl)) { continue; }
            let k = u_load(u_deref(HashMap__keyAt(*self, key_ty, i)));
            let v = u_load(u_deref(HashMap__valAt(*self, val_ty, i)));
            HashMap_putNoClobberWithin(&new_map, k, v);
            if (new_map.size == self->size) { break; }
        });
    }

    HashMap__free(self, key_ty, val_ty, gpa);
    *self = new_map;
    return_ok({});
} $unscoped_(fn);

$static fn_((HashMap__growIfNeeded(HashMap* self, TypeInfo key_ty, TypeInfo val_ty, mem_Allocator gpa, u32 new_count))(mem_Err$void) $scope) {
    if (new_count > self->available) {
        let load = (HashMap_cap(*self) * HashMap_default_max_load_ratio) / 100 - self->available;
        try_(HashMap__grow(self, key_ty, val_ty, gpa, HashMap__capacityForSize(load + new_count)));
    }
    return_ok({});
} $unscoped_(fn);

fn_((HashMap_empty(
    TypeInfo key_ty, TypeInfo val_ty, P_const$HashMap_Ctx ctx
))(HashMap)) {
    claim_assert_nonnull(ctx);
    return (HashMap){
        .metadata = none(),
        .size = 0,
        .available = 0,
        .ctx = ctx,
        debug_only(.key_ty = key_ty, .val_ty = val_ty)
    };
};

fn_((HashMap_init(
    TypeInfo key_ty, TypeInfo val_ty, P_const$HashMap_Ctx ctx, mem_Allocator gpa, u32 cap
))(mem_Err$HashMap) $scope) {
    claim_assert_nonnull(ctx);
    var map = HashMap_empty(key_ty, val_ty, ctx);
    if (cap > 0) {
        let actual_cap = prim_max(HashMap__capacityForSize(cap), HashMap_default_min_cap);
        try_(HashMap__alloc(&map, key_ty, val_ty, gpa, actual_cap));
        HashMap__initMetadata(&map);
        map.available = (actual_cap * HashMap_default_max_load_ratio) / 100;
    }
    return_ok(map);
} $unscoped_(fn);

fn_((HashMap_fini(
    HashMap* self, TypeInfo key_ty, TypeInfo val_ty, mem_Allocator gpa
))(void)) {
    claim_assert_nonnull(self);
    debug_assert_eqBy(self->key_ty, key_ty, TypeInfo_eq);
    debug_assert_eqBy(self->val_ty, val_ty, TypeInfo_eq);
    HashMap__free(self, key_ty, val_ty, gpa);
    *self = HashMap_empty(key_ty, val_ty, self->ctx);
};

fn_((HashMap_clone(
    HashMap self, TypeInfo key_ty, TypeInfo val_ty, mem_Allocator gpa
))(mem_Err$HashMap) $scope) {
    return_ok(try_(HashMap_cloneWithCtx(self, key_ty, val_ty, self.ctx, gpa)));
} $unscoped_(fn);

fn_((HashMap_cloneWithCtx(
    HashMap self, TypeInfo key_ty, TypeInfo val_ty, P_const$HashMap_Ctx ctx, mem_Allocator gpa
))(mem_Err$HashMap) $scope) {
    debug_assert_eqBy(self.key_ty, key_ty, TypeInfo_eq);
    debug_assert_eqBy(self.val_ty, val_ty, TypeInfo_eq);
    var_(other, HashMap) = HashMap_empty(key_ty, val_ty, ctx);
    if (self.size == 0) {
        return_ok(other);
    }

    let new_cap = HashMap__capacityForSize(self.size);
    try_(HashMap__alloc(&other, key_ty, val_ty, gpa, new_cap));
    HashMap__initMetadata(&other);
    other.available = (new_cap * HashMap_default_max_load_ratio) / 100;

    let cap = HashMap_cap(self);
    for_(($r(0, cap))(i) {
        let ctrl = *HashMap__metadataAt(self, i);
        if (HashMap_Ctrl_isUsed(ctrl)) {
            let k = u_load(u_deref(HashMap__keyAt(self, key_ty, i)));
            let v = u_load(u_deref(HashMap__valAt(self, val_ty, i)));
            HashMap_putNoClobberWithin(&other, k, v);
            if (other.size == self.size) { break; }
        }
    });

    return_ok(other);
} $unscoped_(fn);

fn_((HashMap_count(HashMap self))(u32)) {
    return self.size;
};

fn_((HashMap_cap(HashMap self))(u32)) {
    if_none(self.metadata) { return 0; }
    return HashMap__header(self)->cap;
};

fn_((HashMap_ensureCap(
    HashMap* self, TypeInfo key_ty, TypeInfo val_ty, mem_Allocator gpa, u32 new_size
))(mem_Err$void) $scope) {
    claim_assert_nonnull(self);
    debug_assert_eqBy(self->key_ty, key_ty, TypeInfo_eq);
    debug_assert_eqBy(self->val_ty, val_ty, TypeInfo_eq);
    if (new_size > self->size) {
        try_(HashMap__growIfNeeded(self, key_ty, val_ty, gpa, new_size - self->size));
    }
    return_ok({});
} $unscoped_(fn);

fn_((HashMap_ensureUnusedCap(
    HashMap* self, TypeInfo key_ty, TypeInfo val_ty, mem_Allocator gpa, u32 additional
))(mem_Err$void)) {
    claim_assert_nonnull(self);
    debug_assert_eqBy(self->key_ty, key_ty, TypeInfo_eq);
    debug_assert_eqBy(self->val_ty, val_ty, TypeInfo_eq);
    return HashMap_ensureCap(self, key_ty, val_ty, gpa, self->size + additional);
};

fn_((HashMap_clearRetainingCap(HashMap* self))(void)) {
    claim_assert_nonnull(self);
    if_none(self->metadata) { return; }
    HashMap__initMetadata(self);
    self->size = 0;
    self->available = (HashMap_cap(*self) * HashMap_default_max_load_ratio) / 100;
};

fn_((HashMap_clearAndFree(
    HashMap* self, TypeInfo key_ty, TypeInfo val_ty, mem_Allocator gpa
))(void)) {
    claim_assert_nonnull(self);
    debug_assert_eqBy(self->key_ty, key_ty, TypeInfo_eq);
    debug_assert_eqBy(self->val_ty, val_ty, TypeInfo_eq);
    HashMap__free(self, key_ty, val_ty, gpa);
    self->size = 0;
    self->available = 0;
};

fn_((HashMap_by(HashMap self, u_V$raw key, u_V$raw ret_val))(O$u_V$raw) $scope) {
    debug_assert_eqBy(self.key_ty, key.type, TypeInfo_eq);
    debug_assert_eqBy(self.val_ty, ret_val.type, TypeInfo_eq);
    if_some((HashMap__idx(self, key))(idx)) {
        return_some({ .inner = u_memcpy(ret_val.ref, HashMap__valAt(self, ret_val.type, idx).as_const).raw });
    }
    return_none();
} $unscoped_(fn);

fn_((HashMap_ptrBy(HashMap self, TypeInfo val_ty, u_V$raw key))(O$u_P_const$raw) $scope) {
    debug_assert_eqBy(self.key_ty, key.type, TypeInfo_eq);
    debug_assert_eqBy(self.val_ty, val_ty, TypeInfo_eq);
    if_some((HashMap__idx(self, key))(idx)) {
        return_some(HashMap__valAt(self, val_ty, idx).as_const);
    }
    return_none();
} $unscoped_(fn);

fn_((HashMap_ptrMutBy(HashMap self, TypeInfo val_ty, u_V$raw key))(O$u_P$raw) $scope) {
    debug_assert_eqBy(self.key_ty, key.type, TypeInfo_eq);
    debug_assert_eqBy(self.val_ty, val_ty, TypeInfo_eq);
    if_some((HashMap__idx(self, key))(idx)) {
        return_some(HashMap__valAt(self, val_ty, idx));
    }
    return_none();
} $unscoped_(fn);

fn_((HashMap_for(HashMap self, TypeInfo val_ty, u_V$raw key, u_V$raw ret_key))(O$u_V$raw) $scope) {
    debug_assert_eqBy(self.key_ty, key.type, TypeInfo_eq);
    debug_assert_eqBy(self.val_ty, val_ty, TypeInfo_eq);
    if_some((HashMap__idx(self, key))(idx)) {
        return_some({ .inner = u_memcpy(ret_key.ref, HashMap__keyAt(self, ret_key.type, idx).as_const).raw });
    }
    return_none();
} $unscoped_(fn);

fn_((HashMap_ptrFor(HashMap self, TypeInfo val_ty, u_V$raw key))(O$u_P_const$raw) $scope) {
    debug_assert_eqBy(self.key_ty, key.type, TypeInfo_eq);
    debug_assert_eqBy(self.val_ty, val_ty, TypeInfo_eq);
    if_some((HashMap__idx(self, key))(idx)) {
        return_some(HashMap__keyAt(self, key.type, idx).as_const);
    }
    return_none();
} $unscoped_(fn);

fn_((HashMap_ptrMutFor(HashMap self, TypeInfo val_ty, u_V$raw key))(O$u_P$raw) $scope) {
    debug_assert_eqBy(self.key_ty, key.type, TypeInfo_eq);
    debug_assert_eqBy(self.val_ty, val_ty, TypeInfo_eq);
    if_some((HashMap__idx(self, key))(idx)) {
        return_some(HashMap__keyAt(self, key.type, idx));
    }
    return_none();
} $unscoped_(fn);

fn_((HashMap_entry(HashMap self, TypeInfo val_ty, u_V$raw key))(O$HashMap_Entry) $scope) {
    debug_assert_eqBy(self.key_ty, key.type, TypeInfo_eq);
    debug_assert_eqBy(self.val_ty, val_ty, TypeInfo_eq);
    if_some((HashMap__idx(self, key))(idx)) {
        let k = HashMap__keyAt(self, key.type, idx).as_const;
        let v = HashMap__valAt(self, val_ty, idx).as_const;
        return_some({ .key = k.raw, .val = v.raw, debug_only(.key_ty = key.type, .val_ty = val_ty) });
    }
    return_none();
} $unscoped_(fn);

fn_((HashMap_entryMut(HashMap self, TypeInfo val_ty, u_V$raw key))(O$HashMap_EntryMut) $scope) {
    debug_assert_eqBy(self.key_ty, key.type, TypeInfo_eq);
    debug_assert_eqBy(self.val_ty, val_ty, TypeInfo_eq);
    if_some((HashMap__idx(self, key))(idx)) {
        let k = HashMap__keyAt(self, key.type, idx);
        let v = HashMap__valAt(self, val_ty, idx);
        return_some({ .key = k.raw, .val = v.raw, debug_only(.key_ty = key.type, .val_ty = val_ty) });
    }
    return_none();
} $unscoped_(fn);

fn_((HashMap_contains(HashMap self, TypeInfo val_ty, u_V$raw key))(bool)) {
    debug_assert_eqBy(self.key_ty, key.type, TypeInfo_eq);
    debug_assert_eqBy(self.val_ty, val_ty, TypeInfo_eq);
    return isSome(HashMap__idx(self, key));
};

fn_((HashMap_put(HashMap* self, mem_Allocator gpa, u_V$raw key, u_V$raw val))(mem_Err$void) $scope) {
    claim_assert_nonnull(self);
    debug_assert_eqBy(self->key_ty, key.type, TypeInfo_eq);
    debug_assert_eqBy(self->val_ty, val.type, TypeInfo_eq);
    let ensured = try_(HashMap_ensure(self, val.type, gpa, key));
    u_memcpy(HashMap_Ensured_valMut(ensured, val.type), val.ref.as_const);
    return_ok({});
} $unscoped_(fn);

fn_((HashMap_putWithin(HashMap* self, u_V$raw key, u_V$raw val))(void)) {
    claim_assert_nonnull(self);
    debug_assert_eqBy(self->key_ty, key.type, TypeInfo_eq);
    debug_assert_eqBy(self->val_ty, val.type, TypeInfo_eq);
    let ensured = HashMap_ensureWithin(self, val.type, key);
    u_memcpy(HashMap_Ensured_valMut(ensured, val.type), val.ref.as_const);
};

fn_((HashMap_putNoClobber(HashMap* self, mem_Allocator gpa, u_V$raw key, u_V$raw val))(mem_Err$void) $scope) {
    claim_assert_nonnull(self);
    debug_assert_eqBy(self->key_ty, key.type, TypeInfo_eq);
    debug_assert_eqBy(self->val_ty, val.type, TypeInfo_eq);
    try_(HashMap__growIfNeeded(self, key.type, val.type, gpa, 1));
    HashMap_putNoClobberWithin(self, key, val);
    return_ok({});
} $unscoped_(fn);

fn_((HashMap_putNoClobberWithin(HashMap* self, u_V$raw key, u_V$raw val))(void)) {
    claim_assert_nonnull(self);
    debug_assert_eqBy(self->key_ty, key.type, TypeInfo_eq);
    debug_assert_eqBy(self->val_ty, val.type, TypeInfo_eq);
    claim_assert(!HashMap_contains(*self, val.type, key));
    let ctx = self->ctx;
    let hash = ctx->hashFn(key, u_load(u_deref(ctx->inner)));
    let cap = HashMap_cap(*self);
    let mask = cap - 1;
    var_(idx, usize) = hash & mask;
    while (HashMap_Ctrl_isUsed(*HashMap__metadataAt(*self, idx))) {
        idx = (idx + 1) & mask;
    }
    claim_assert(self->available > 0);
    self->available--;

    let fingerprint = HashMap_Ctrl_takeFingerprint(hash);
    HashMap_Ctrl_fill(HashMap__metadataAt(*self, idx), fingerprint);
    u_memcpy(HashMap__keyAt(*self, key.type, idx), key.ref.as_const);
    u_memcpy(HashMap__valAt(*self, val.type, idx), val.ref.as_const);
    self->size++;
};

fn_((HashMap_fetchPut(
    HashMap* self, mem_Allocator gpa, u_V$raw key, u_V$raw val, V$HashMap_Pair$raw ret_mem
))(mem_Err$O$V$HashMap_Pair$raw) $scope) {
    claim_assert_nonnull(self);
    debug_assert_eqBy(self->key_ty, key.type, TypeInfo_eq);
    debug_assert_eqBy(self->val_ty, val.type, TypeInfo_eq);
    let ensured = try_(HashMap_ensure(self, val.type, gpa, key));
    let result = expr_(O$V$HashMap_Pair$raw $scope)(if (ensured.found_existing) {
        let k = HashMap_Ensured_key(ensured, key.type);
        let v = HashMap_Ensured_val(ensured, val.type);
        $break_(some(HashMap_Pair_init(
            u_load(u_deref(k)),
            u_load(u_deref(v)),
            ret_mem
        )));
    } else_none {
        $break_(none());
    }) $unscoped_(expr);
    u_memcpy(HashMap_Ensured_valMut(ensured, val.type), val.ref.as_const);
    return_ok(result);
} $unscoped_(fn);

fn_((HashMap_fetchPutWithin(
    HashMap* self, u_V$raw key, u_V$raw val, V$HashMap_Pair$raw ret_mem
))(O$V$HashMap_Pair$raw)) {
    claim_assert_nonnull(self);
    debug_assert_eqBy(self->key_ty, key.type, TypeInfo_eq);
    debug_assert_eqBy(self->val_ty, val.type, TypeInfo_eq);
    let ensured = HashMap_ensureWithin(self, val.type, key);
    let result = expr_(O$V$HashMap_Pair$raw $scope)(if (ensured.found_existing) {
        let k = HashMap_Ensured_key(ensured, key.type);
        let v = HashMap_Ensured_val(ensured, val.type);
        $break_(some(HashMap_Pair_init(
            u_load(u_deref(k)),
            u_load(u_deref(v)),
            ret_mem
        )));
    } else_none {
        $break_(none());
    }) $unscoped_(expr);
    u_memcpy(HashMap_Ensured_valMut(ensured, val.type), val.ref.as_const);
    return result;
};

fn_((HashMap_ensure(HashMap* self, TypeInfo val_ty, mem_Allocator gpa, u_V$raw key))(mem_Err$HashMap_Ensured) $scope) {
    claim_assert_nonnull(self);
    debug_assert_eqBy(self->key_ty, key.type, TypeInfo_eq);
    debug_assert_eqBy(self->val_ty, val_ty, TypeInfo_eq);
    // Try to grow first, but if it fails and key exists, we can still return it
    catch_((HashMap__growIfNeeded(self, key.type, val_ty, gpa, 1))(err, if_some((HashMap__idx(*self, key))(idx)) {
        let k = HashMap__keyAt(*self, key.type, idx);
        let v = HashMap__valAt(*self, val_ty, idx);
        return_ok((HashMap_Ensured){
            .key = k.raw,
            .val = v.raw,
            .found_existing = true,
            debug_only(.key_ty = key.type, .val_ty = val_ty) });
    } else_none {
        return_err(err);
    }));
    return_ok(HashMap_ensureWithin(self, val_ty, key));
} $unscoped_(fn);

fn_((HashMap_ensureWithin(HashMap* self, TypeInfo val_ty, u_V$raw key))(HashMap_Ensured)) {
    claim_assert_nonnull(self);
    debug_assert_eqBy(self->key_ty, key.type, TypeInfo_eq);
    debug_assert_eqBy(self->val_ty, val_ty, TypeInfo_eq);
    let ctx = self->ctx;
    let hash = ctx->hashFn(key, u_load(u_deref(ctx->inner)));
    let cap = HashMap_cap(*self);
    let mask = cap - 1;
    let fingerprint = HashMap_Ctrl_takeFingerprint(hash);

    var_(first_tombstone_idx, usize) = cap; // Invalid index
    var_(idx, usize) = hash & mask;
    var_(limit, u32) = cap;
    while (limit > 0) {
        let ctrl = *HashMap__metadataAt(*self, idx);
        if (HashMap_Ctrl_isFree(ctrl)) {
            break;
        }
        if (HashMap_Ctrl_isUsed(ctrl) && ctrl.fingerprint == fingerprint) {
            if (ctx->eqlFn(key, u_load(u_deref(HashMap__keyAt(*self, key.type, idx))), u_load(u_deref(ctx->inner)))) {
                return (HashMap_Ensured){
                    .key = HashMap__keyAt(*self, key.type, idx).raw,
                    .val = HashMap__valAt(*self, val_ty, idx).raw,
                    .found_existing = true,
                    debug_only(.key_ty = key.type, .val_ty = val_ty)
                };
            }
        } else if (first_tombstone_idx == cap && HashMap_Ctrl_isTombstone(ctrl)) {
            first_tombstone_idx = idx;
        }
        limit--;
        idx = (idx + 1) & mask;
    }
    // Not found, insert at first tombstone or current free slot
    if (first_tombstone_idx < cap) {
        idx = first_tombstone_idx;
    }
    claim_assert(self->available > 0);
    self->available--;
    HashMap_Ctrl_fill(HashMap__metadataAt(*self, idx), fingerprint);
    u_memcpy(HashMap__keyAt(*self, key.type, idx), key.ref.as_const);
    self->size++;
    return (HashMap_Ensured){
        .key = HashMap__keyAt(*self, key.type, idx).raw,
        .val = HashMap__valAt(*self, val_ty, idx).raw,
        .found_existing = false,
        debug_only(.key_ty = key.type, .val_ty = val_ty)
    };
};

fn_((HashMap_ensureValue(HashMap* self, mem_Allocator gpa, u_V$raw key, u_V$raw default_val))(mem_Err$HashMap_Ensured) $scope) {
    claim_assert_nonnull(self);
    debug_assert_eqBy(self->key_ty, key.type, TypeInfo_eq);
    debug_assert_eqBy(self->val_ty, default_val.type, TypeInfo_eq);
    let ensured = try_(HashMap_ensure(self, default_val.type, gpa, key));
    if (!ensured.found_existing) { u_memcpy(HashMap_Ensured_valMut(ensured, default_val.type), default_val.ref.as_const); }
    return_ok(ensured);
} $unscoped_(fn);

fn_((HashMap_remove(HashMap* self, TypeInfo val_ty, u_V$raw key))(bool)) {
    claim_assert_nonnull(self);
    debug_assert_eqBy(self->key_ty, key.type, TypeInfo_eq);
    debug_assert_eqBy(self->val_ty, val_ty, TypeInfo_eq);
    if_some((HashMap__idx(*self, key))(idx)) {
        HashMap_Ctrl_remove(HashMap__metadataAt(*self, idx));
        self->size--;
        self->available++;
        return true;
    }
    return false;
};

fn_((HashMap_fetchRemove(
    HashMap* self, TypeInfo val_ty, u_V$raw key, V$HashMap_Pair$raw ret_mem
))(O$V$HashMap_Pair$raw) $scope) {
    claim_assert_nonnull(self);
    debug_assert_eqBy(self->key_ty, key.type, TypeInfo_eq);
    debug_assert_eqBy(self->val_ty, val_ty, TypeInfo_eq);
    if_some((HashMap__idx(*self, key))(idx)) {
        let old_key = HashMap__keyAt(*self, key.type, idx);
        let old_val = HashMap__valAt(*self, val_ty, idx);
        let result = HashMap_Pair_init(
            u_load(u_deref(old_key)),
            u_load(u_deref(old_val)),
            ret_mem
        );
        HashMap_Ctrl_remove(HashMap__metadataAt(*self, idx));
        mem_set0(u_prefixP(old_key, 1));
        mem_set0(u_prefixP(old_val, 1));
        self->size--;
        self->available++;
        return_some(result);
    }
    return_none();
} $unscoped_(fn);

fn_((HashMap_removeByPtr(HashMap* self, TypeInfo val_ty, u_P$raw key_ptr))(void)) {
    claim_assert_nonnull(self);
    claim_assert_nonnull(key_ptr.raw);
    debug_assert_eqBy(self->key_ty, key_ptr.type, TypeInfo_eq);
    debug_assert_eqBy(self->val_ty, val_ty, TypeInfo_eq);

    let idx = expr_(u32 $scope)(if (key_ptr.type.size > 0) {
        $break_((intFromPtr(key_ptr.raw) - intFromPtr(HashMap__keys(*self, key_ptr.type).raw)) / key_ptr.type.size);
    }) expr_(else)({
        $break_(0);
    }) $unscoped_(expr);
    HashMap_Ctrl_remove(HashMap__metadataAt(*self, idx));
    self->size--;
    self->available++;
};

fn_((HashMap_rehash(HashMap* self, TypeInfo key_ty, TypeInfo val_ty))(void)) {
    claim_assert_nonnull(self);
    debug_assert_eqBy(self->key_ty, key_ty, TypeInfo_eq);
    debug_assert_eqBy(self->val_ty, val_ty, TypeInfo_eq);

    if_none(self->metadata) { return; }

    let cap = HashMap_cap(*self);
    let mask = cap - 1;
    let ctx = self->ctx;

    // Mark all slots as free but keep used flag
    for_(($r(0, cap))(i) {
        HashMap__metadataAt(*self, i)->fingerprint = HashMap_Ctrl_free;
    });

    // Rehash all used slots
    var_(curr, u32) = 0;
    while (curr < cap) {
        let ctrl = HashMap__metadataAt(*self, curr);
        if (!HashMap_Ctrl_isUsed(*ctrl)) {
            claim_assert(HashMap_Ctrl_isFree(*ctrl));
            curr++;
            continue;
        }
        let key = HashMap__keyAt(*self, key_ty, curr);
        let hash = ctx->hashFn(u_load(u_deref(key)), u_load(u_deref(ctx->inner)));
        let fingerprint = HashMap_Ctrl_takeFingerprint(hash);
        var_(idx, usize) = hash & mask;

        // Find target slot
        while ((idx < curr && HashMap_Ctrl_isUsed(*HashMap__metadataAt(*self, idx)))
               || (idx > curr && HashMap_Ctrl_isTombstone(*HashMap__metadataAt(*self, idx)))) {
            idx = (idx + 1) & mask;
        }
        if (idx < curr) {
            // Move to earlier slot
            claim_assert(HashMap_Ctrl_isFree(*HashMap__metadataAt(*self, idx)));
            HashMap_Ctrl_fill(HashMap__metadataAt(*self, idx), fingerprint);

            // Copy key and value
            let dst_key = HashMap__keyAt(*self, key_ty, idx);
            let src_key = HashMap__keyAt(*self, key_ty, curr);
            let dst_val = HashMap__valAt(*self, val_ty, idx);
            let src_val = HashMap__valAt(*self, val_ty, curr);
            u_memcpy(dst_key, src_key.as_const);
            u_memcpy(dst_val, src_val.as_const);

            ctrl->used = 0;
            claim_assert(HashMap_Ctrl_isFree(*ctrl));
            curr++;
        } else if (idx == curr) {
            ctrl->fingerprint = fingerprint;
            curr++;
        } else {
            // Swap with later slot
            let target_ctrl = HashMap__metadataAt(*self, idx);
            HashMap_Ctrl_remove(target_ctrl);
            if (HashMap_Ctrl_isUsed(*target_ctrl)) {
                mem_swap(
                    u_prefixP(HashMap__keyAt(*self, key_ty, curr), 1),
                    u_suffixP(HashMap__keyAt(*self, key_ty, idx), 1)
                );
                mem_swap(
                    u_prefixP(HashMap__valAt(*self, val_ty, curr), 1),
                    u_suffixP(HashMap__valAt(*self, val_ty, idx), 1)
                );
            } else {
                target_ctrl->used = 1;
                u_memcpy(HashMap__keyAt(*self, key_ty, idx), HashMap__keyAt(*self, key_ty, curr).as_const);
                u_memcpy(HashMap__valAt(*self, val_ty, idx), HashMap__valAt(*self, val_ty, curr).as_const);
                ctrl->fingerprint = HashMap_Ctrl_free;
                ctrl->used = 0;
                curr++;
            }
        }
    }
};

fn_((HashMap_iter(const HashMap* self, TypeInfo key_ty, TypeInfo val_ty))(HashMap_Iter)) {
    claim_assert_nonnull(self);
    debug_assert_eqBy(self->key_ty, key_ty, TypeInfo_eq);
    debug_assert_eqBy(self->val_ty, val_ty, TypeInfo_eq);
    return (HashMap_Iter){
        .map = self,
        .idx = 0,
        debug_only(.key_ty = key_ty, .val_ty = val_ty)
    };
};

fn_((HashMap_Iter_next(HashMap_Iter* self, TypeInfo key_ty, TypeInfo val_ty))(O$HashMap_Entry) $scope) {
    claim_assert_nonnull(self);
    claim_assert_nonnull(self->map);
    debug_assert_eqBy(self->key_ty, key_ty, TypeInfo_eq);
    debug_assert_eqBy(self->val_ty, val_ty, TypeInfo_eq);

    if (self->map->size == 0) { return_none(); }
    let cap = HashMap_cap(*self->map);
    while (cap > self->idx) {
        let ctrl = *HashMap__metadataAt(*self->map, self->idx);
        if (HashMap_Ctrl_isUsed(ctrl)) {
            return_some(expr_(HashMap_Entry $guard)({
                defer_(self->idx++);
                $break_((HashMap_Entry){
                    .key = HashMap__keyAt(*self->map, key_ty, self->idx).as_const.raw,
                    .val = HashMap__valAt(*self->map, val_ty, self->idx).as_const.raw,
                    debug_only(.key_ty = key_ty, .val_ty = val_ty) });
            }) $unguarded_(expr));
        }
        self->idx++;
    }
    return_none();
} $unscoped_(fn);

fn_((HashMap_Iter_nextMut(HashMap_Iter* self, TypeInfo key_ty, TypeInfo val_ty))(O$HashMap_EntryMut) $scope) {
    claim_assert_nonnull(self);
    claim_assert_nonnull(self->map);
    debug_assert_eqBy(self->key_ty, key_ty, TypeInfo_eq);
    debug_assert_eqBy(self->val_ty, val_ty, TypeInfo_eq);

    if (self->map->size == 0) { return_none(); }
    let cap = HashMap_cap(*self->map);
    while (cap > self->idx) {
        let ctrl = *HashMap__metadataAt(*self->map, self->idx);
        if (HashMap_Ctrl_isUsed(ctrl)) {
            return_some(expr_(HashMap_EntryMut $guard)({
                defer_(self->idx++);
                $break_((HashMap_EntryMut){
                    .key = HashMap__keyAt(*self->map, key_ty, self->idx).raw,
                    .val = HashMap__valAt(*self->map, val_ty, self->idx).raw,
                    debug_only(.key_ty = key_ty, .val_ty = val_ty) });
            }) $unguarded_(expr));
        }
        self->idx++;
    }
    return_none();
} $unscoped_(fn);

fn_((HashMap_keyIter(HashMap self, TypeInfo key_ty, TypeInfo val_ty))(HashMap_KeyIter) $scope) {
    debug_assert_eqBy(self.key_ty, key_ty, TypeInfo_eq);
    debug_assert_eqBy(self.val_ty, val_ty, TypeInfo_eq);
    return_(expr_(HashMap_KeyIter $scope)(
        if_some((self.metadata)(metadata)) {
            $break_((HashMap_KeyIter){
                .len = HashMap_cap(self),
                .metadata = metadata,
                .keys = HashMap__keys(self, key_ty).raw,
                debug_only(.key_ty = key_ty) });
        } else_none {
            $break_((HashMap_KeyIter){
                .len = 0,
                .metadata = null,
                .keys = null,
                debug_only(.key_ty = key_ty) });
        }
    ) $unscoped_(expr));
} $unscoped_(fn);

fn_((HashMap_KeyIter_next(HashMap_KeyIter* self, TypeInfo key_ty))(O$u_P_const$raw) $scope) {
    claim_assert_nonnull(self);
    debug_assert_eqBy(self->key_ty, key_ty, TypeInfo_eq);
    while (self->len > 0) {
        self->len--;
        let used = HashMap_Ctrl_isUsed(*self->metadata);
        let key_ptr = self->keys;
        self->metadata++;
        self->keys = as$(u8*)(self->keys) + key_ty.size;
        if (used) {
            return_some({ .raw = key_ptr, .type = key_ty });
        }
    }
    return_none();
} $unscoped_(fn);

fn_((HashMap_KeyIter_nextMut(HashMap_KeyIter* self, TypeInfo key_ty))(O$u_P$raw) $scope) {
    claim_assert_nonnull(self);
    debug_assert_eqBy(self->key_ty, key_ty, TypeInfo_eq);
    while (self->len > 0) {
        self->len--;
        let used = HashMap_Ctrl_isUsed(*self->metadata);
        let key_ptr = self->keys;
        self->metadata++;
        self->keys = as$(u8*)(self->keys) + key_ty.size;
        if (used) {
            return_some({ .raw = key_ptr, .type = key_ty });
        }
    }
    return_none();
} $unscoped_(fn);

fn_((HashMap_valIter(HashMap self, TypeInfo key_ty, TypeInfo val_ty))(HashMap_ValIter) $scope) {
    debug_assert_eqBy(self.key_ty, key_ty, TypeInfo_eq);
    debug_assert_eqBy(self.val_ty, val_ty, TypeInfo_eq);
    return_(expr_(HashMap_ValIter $scope)(
        if_some((self.metadata)(metadata)) {
            $break_((HashMap_ValIter){
                .len = HashMap_cap(self),
                .metadata = metadata,
                .vals = HashMap__vals(self, val_ty).raw,
                debug_only(.val_ty = val_ty) });
        } else_none {
            $break_((HashMap_ValIter){
                .len = 0,
                .metadata = null,
                .vals = null,
                debug_only(.val_ty = val_ty) });
        }
    ) $unscoped_(expr));
} $unscoped_(fn);

fn_((HashMap_ValIter_next(HashMap_ValIter* self, TypeInfo val_ty))(O$u_P_const$raw) $scope) {
    claim_assert_nonnull(self);
    debug_assert_eqBy(self->val_ty, val_ty, TypeInfo_eq);
    while (self->len > 0) {
        self->len--;
        let used = HashMap_Ctrl_isUsed(*self->metadata);
        let val_ptr = self->vals;
        self->metadata++;
        self->vals = as$(u8*)(self->vals) + val_ty.size;
        if (used) {
            return_some({ .raw = val_ptr, .type = val_ty });
        }
    }
    return_none();
} $unscoped_(fn);

fn_((HashMap_ValIter_nextMut(HashMap_ValIter* self, TypeInfo val_ty))(O$u_P$raw) $scope) {
    claim_assert_nonnull(self);
    debug_assert_eqBy(self->val_ty, val_ty, TypeInfo_eq);
    while (self->len > 0) {
        self->len--;
        let used = HashMap_Ctrl_isUsed(*self->metadata);
        let val_ptr = self->vals;
        self->metadata++;
        self->vals = as$(u8*)(self->vals) + val_ty.size;
        if (used) {
            return_some({ .raw = val_ptr, .type = val_ty });
        }
    }
    return_none();
} $unscoped_(fn);

#include "dh/HashSet.h"
#include "dh/meta.h"

/*========== SIMD Configuration =============================================*/

#define HashSet_use_simd arch_simd_use
#define HashSet_simd_sse2 (arch_is_x86_family && arch_has_sse2)
#define HashSet_simd_neon (arch_is_arm_family && arch_has_neon)
#if HashSet_use_simd
#if HashSet_simd_sse2
#include <emmintrin.h>
#elif HashSet_simd_neon
#include <arm_neon.h>
#endif
#define HashSet_simd_group_size (arch_simd_width_bits / arch_bits_per_byte)
#endif /* HashSet_use_simd */

/*========== Definitions ====================================================*/

fn_((HashSet_Sgl_init(u_V$raw key, V$HashSet_Sgl$raw ret_mem))(V$HashSet_Sgl$raw)) {
    debug_only({
        ret_mem->key_ty = key.type;
    });
    let self_type = u_typeInfoRecord(typeInfosFrom(key.type));
    let p_self = (u_V$raw){ .type = self_type, .inner = ret_mem->data.inner }.ref;
    let p_key = u_fieldPtrMut(p_self, typeInfosFrom(key.type), 0);
    u_memcpy(p_key, key.ref.as_const);
    return ret_mem;
};

fn_((HashSet_Sgl_key(V$HashSet_Sgl$raw self, u_V$raw ret_mem))(u_V$raw)) {
    debug_assert_nonnull(ret_mem.inner);
    debug_assert_eqBy(self->key_ty, ret_mem.type, TypeInfo_eq);
    let self_type = u_typeInfoRecord(typeInfosFrom(ret_mem.type));
    let p_self = u_load((u_V$raw){ .type = self_type, .inner = self->data.inner }).ref;
    let p_key = u_fieldPtr(p_self.as_const, typeInfosFrom(ret_mem.type), 0);
    return u_deref(u_memcpy(ret_mem.ref, p_key));
};

fn_((HashSet_Entry_key(HashSet_Entry self, TypeInfo key_ty))(u_P_const$raw)) {
    debug_assert_eqBy(self.key_ty, key_ty, TypeInfo_eq);
    return (u_P_const$raw){ .raw = self.key, .type = key_ty };
};

fn_((HashSet_EntryMut_key(HashSet_EntryMut self, TypeInfo key_ty))(u_P$raw)) {
    debug_assert_eqBy(self.key_ty, key_ty, TypeInfo_eq);
    return (u_P$raw){ .raw = self.key, .type = key_ty };
};

fn_((HashSet_Ensured_key(HashSet_Ensured self, TypeInfo key_ty))(u_P_const$raw)) {
    debug_assert_eqBy(self.key_ty, key_ty, TypeInfo_eq);
    return (u_P_const$raw){ .raw = self.key, .type = key_ty };
};

fn_((HashSet_Ensured_keyMut(HashSet_Ensured self, TypeInfo key_ty))(u_P$raw)) {
    debug_assert_eqBy(self.key_ty, key_ty, TypeInfo_eq);
    return (u_P$raw){ .raw = self.key, .type = key_ty };
};

fn_((HashSet_Ensured_foundExisting(HashSet_Ensured self, TypeInfo key_ty))(O$HashSet_Entry)) {
    debug_assert_eqBy(self.key_ty, key_ty, TypeInfo_eq);
    let_ignore = key_ty;
    return expr_(O$HashSet_Entry $scope)(if (self.found_existing) {
        $break_(some({ .key = self.key, debug_only(.key_ty = key_ty) }));
    }) expr_(else)({
        $break_(none());
    }) $unscoped_(expr);
};

fn_((HashSet_Ensured_foundExistingMut(HashSet_Ensured self, TypeInfo key_ty))(O$HashSet_EntryMut)) {
    debug_assert_eqBy(self.key_ty, key_ty, TypeInfo_eq);
    let_ignore = key_ty;
    return expr_(O$HashSet_EntryMut $scope)(if (self.found_existing) {
        $break_(some({ .key = self.key, debug_only(.key_ty = key_ty) }));
    }) expr_(else)({
        $break_(none());
    }) $unscoped_(expr);
};

fn_((HashSet_HashFn_default(u_V$raw val, u_V$raw ctx))(u64)) {
    return HashMap_HashFn_default(val, ctx);
};

fn_((HashSet_EqlFn_default(u_V$raw lhs, u_V$raw rhs, u_V$raw ctx))(bool)) {
    return HashMap_EqlFn_default(lhs, rhs, ctx);
};

fn_((HashSet_Ctx_default(void))(P_const$HashSet_Ctx)) {
    return HashMap_Ctx_default();
};

$static fn_((HashSet__header(HashSet self))(HashSet_Header*)) {
    let metadata_ptr = unwrap_(self.metadata);
    return as$(HashSet_Header*)(as$(u8*)(metadata_ptr)-sizeOf$(HashSet_Header));
};

$static fn_((HashSet__keys(HashSet self, TypeInfo key_ty))(u_P$raw)) {
    return (u_P$raw){ .raw = HashSet__header(self)->keys, .type = key_ty };
};

$static fn_((HashSet__keyAt(HashSet self, TypeInfo key_ty, usize idx))(u_P$raw)) {
    return (u_P$raw){ .raw = as$(u8*)(HashSet__keys(self, key_ty).raw) + idx * key_ty.size, .type = key_ty };
};

$static fn_((HashSet__metadataAt(HashSet self, usize idx))(HashMap_Ctrl*)) {
    return unwrap_(self.metadata) + idx;
};

$static fn_((HashSet__capacityForSize(u32 size))(u32)) {
    var_(new_cap, u64) = (as$(u64)(size) * 100) / HashSet_default_max_load_ratio + 1;
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

$static fn_((HashSet__initMetadata(HashSet* self))(void)) {
    mem_set0(u_anyS(slice$P(unwrap_(self->metadata), $r(0, HashSet_cap(*self)))));
};

$static fn_((HashSet__alloc(HashSet* self, TypeInfo key_ty, mem_Allocator gpa, u32 new_cap))(mem_Err$void) $scope) {
    let header_align = alignOf$(HashSet_Header);
    let key_align = key_ty.size == 0 ? 1 : mem_log2ToAlign(key_ty.align);
    let max_align = prim_max(header_align, key_align);

    let meta_size = sizeOf$(HashSet_Header) + as$(usize)(new_cap)*sizeOf$(HashMap_Ctrl);
    let keys_start = mem_alignFwd(meta_size, key_align);
    let keys_end = keys_start + as$(usize)(new_cap)*key_ty.size;
    let total_size = mem_alignFwd(keys_end, max_align);

    let slice = u_castS$((S$u8)(try_(mem_Allocator_alloc(gpa, typeInfo$(u8), total_size))));
    let ptr = as$(u8*)(slice.ptr);
    let hdr = as$(HashSet_Header*)(ptr);
    hdr->keys = ptr + keys_start;
    hdr->cap = new_cap;
    debug_only({
        hdr->key_ty = key_ty;
    });

    asg_lit((&self->metadata)(some(as$(HashMap_Ctrl*)(ptr + sizeOf$(HashSet_Header)))));
    return_ok({});
} $unscoped_(fn);

$static fn_((HashSet__free(HashSet* self, TypeInfo key_ty, mem_Allocator gpa))(void)) {
    if_none(self->metadata) { return; }

    let header_align = alignOf$(HashSet_Header);
    let key_align = key_ty.size == 0 ? 1 : (1ull << key_ty.align);
    let max_align = prim_max(header_align, key_align);

    let cap = HashSet_cap(*self);
    let meta_size = sizeOf$(HashSet_Header) + as$(usize)(cap)*sizeOf$(HashMap_Ctrl);
    let keys_start = mem_alignFwd(meta_size, key_align);
    let keys_end = keys_start + as$(usize)(cap)*key_ty.size;
    let total_size = mem_alignFwd(keys_end, max_align);

    let ptr = as$(u8*)(HashSet__header(*self));
    mem_Allocator_free(gpa, (u_S$raw){ .ptr = ptr, .len = total_size, .type = typeInfo$(u8) });

    asg_lit((&self->metadata)(none()));
    self->available = 0;
};

#if HashSet_use_simd
$static fn_((HashSet__idx_simd(HashSet self, u_V$raw key))(O$usize));
#endif /* HashSet_use_simd */
$static fn_((HashSet__idx(HashSet self, u_V$raw key))(O$usize) $scope) {
#if HashSet_use_simd
    /* Use SIMD path when capacity is aligned to group size */
    if (HashSet_cap(self) >= HashSet_simd_group_size && (HashSet_cap(self) % HashSet_simd_group_size) == 0) {
        return HashSet__idx_simd(self, key);
    }
#endif /* HashSet_use_simd */

    if (self.size == 0) { return_none(); }

    let ctx = self.ctx;
    let hash = ctx->hashFn(key, u_load(u_deref(ctx->inner)));

    let cap = HashSet_cap(self);
    let mask = cap - 1;
    let fingerprint = HashMap_Ctrl_takeFingerprint(hash);
    var_(idx, usize) = hash & mask;
    var_(limit, u32) = cap;
    while (limit > 0) {
        let ctrl = *HashSet__metadataAt(self, idx);
        if (HashMap_Ctrl_isFree(ctrl)) {
            return_none();
        }
        if (HashMap_Ctrl_isUsed(ctrl) && ctrl.fingerprint == fingerprint) {
            if (ctx->eqlFn(key, u_load(u_deref(HashSet__keyAt(self, key.type, idx))), u_load(u_deref(ctx->inner)))) {
                return_some(idx);
            }
        }
        limit--;
        idx = (idx + 1) & mask;
    }

    return_none();
} $unscoped_(fn);

#if HashSet_use_simd
/// Load 16 control bytes and find matches for fingerprint
$attr($inline_always)
$static fn_((HashSet__simd_match_fingerprint(const HashMap_Ctrl* group, u8 fingerprint))(u32));
/// Find free (not tombstone) slots in a group
$attr($inline_always)
$static fn_((HashSet__simd_match_free(const HashMap_Ctrl* group))(u32));
/// SIMD-accelerated lookup
$static fn_((HashSet__idx_simd(HashSet self, u_V$raw key))(O$usize) $scope) {
    if (self.size == 0) { return_none(); }

    let ctx = self.ctx;
    let hash = ctx->hashFn(key, u_load(u_deref(ctx->inner)));
    let cap = HashSet_cap(self);
    let fingerprint = HashMap_Ctrl_takeFingerprint(hash);

    let start_idx = hash & (cap - 1);
    let start_group = start_idx / HashSet_simd_group_size;
    let start_offset_in_group = start_idx % HashSet_simd_group_size;
    let num_groups = cap / HashSet_simd_group_size;

    for_(($r(0, num_groups))(group_offset) {
        let group_idx = (start_group + group_offset) % num_groups;
        let group_start = group_idx * HashSet_simd_group_size;
        let group = HashSet__metadataAt(self, group_start);
        /* SIMD: Check all control bytes for fingerprint match */
        var_(match_mask, u32) = HashSet__simd_match_fingerprint(group, fingerprint);
        /* Iterate over matching positions */
        while (match_mask != 0) {
            let bit_pos = mem_trailingZeros32(match_mask);
            let idx = group_start + bit_pos;
            /* Verify with full equality check */
            if (ctx->eqlFn(key, u_load(u_deref(HashSet__keyAt(self, key.type, idx))), u_load(u_deref(ctx->inner)))) {
                return_some(idx);
            }
            /* Clear this bit and continue */
            match_mask &= match_mask - 1;
        }
        /* Check if this group has any free slots that would terminate the probe */
        var_(free_mask, u32) = HashSet__simd_match_free(group);
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

#if HashSet_simd_sse2
/* --- SSE2 Implementation --- */
$attr($inline_always)
$static fn_((HashSet__simd_match_fingerprint(const HashMap_Ctrl* group, u8 fingerprint))(u32)) {
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
$static fn_((HashSet__simd_match_free(const HashMap_Ctrl* group))(u32)) {
    /* Free slots are exactly 0x00 */
    let zero = _mm_setzero_si128();
    let haystack = _mm_loadu_si128(as$(const __m128i*)(group));
    let cmp = _mm_cmpeq_epi8(zero, haystack);
    return as$(u32)(_mm_movemask_epi8(cmp));
};
#endif /* HashSet_simd_sse2 */
#if HashSet_simd_neon
/* --- NEON Implementation --- */
$attr($inline_always)
$static fn_((HashSet__simd_match_fingerprint(const HashMap_Ctrl* group, u8 fingerprint))(u32)) {
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
$static fn_((HashSet__simd_match_free(const HashMap_Ctrl* group))(u32)) {
    let zero = vdupq_n_u8(0);
    let haystack = vld1q_u8(as$(const u8*)(group));
    let cmp = vceqq_u8(zero, haystack);
    let narrowed = vshrn_n_u16(vreinterpretq_u16_u8(cmp), 4);
    return vget_lane_u64(vreinterpret_u64_u8(narrowed), 0) & 0xFFFF;
};
#endif /* HashSet_simd_neon */
#endif /* HashSet_use_simd */

$static fn_((HashSet__grow(HashSet* self, TypeInfo key_ty, mem_Allocator gpa, u32 new_capacity))(mem_Err$void) $scope) {
    let new_cap = prim_max(new_capacity, HashSet_default_min_cap);
    claim_assert(new_cap > HashSet_cap(*self));

    var_(new_set, HashSet) = HashSet_empty(key_ty, self->ctx);
    try_(HashSet__alloc(&new_set, key_ty, gpa, new_cap));
    HashSet__initMetadata(&new_set);
    new_set.available = (new_cap * HashSet_default_max_load_ratio) / 100;

    if (self->size != 0) {
        let old_cap = HashSet_cap(*self);
        for_(($r(0, old_cap))(i) {
            let ctrl = *HashSet__metadataAt(*self, i);
            if (!HashMap_Ctrl_isUsed(ctrl)) { continue; }
            let k = u_load(u_deref(HashSet__keyAt(*self, key_ty, i)));
            HashSet_putWithin(&new_set, k);
            if (new_set.size == self->size) { break; }
        });
    }

    HashSet__free(self, key_ty, gpa);
    *self = new_set;
    return_ok({});
} $unscoped_(fn);

$static fn_((HashSet__growIfNeeded(HashSet* self, TypeInfo key_ty, mem_Allocator gpa, u32 new_count))(mem_Err$void) $scope) {
    if (new_count > self->available) {
        let load = (HashSet_cap(*self) * HashSet_default_max_load_ratio) / 100 - self->available;
        try_(HashSet__grow(self, key_ty, gpa, HashSet__capacityForSize(load + new_count)));
    }
    return_ok({});
} $unscoped_(fn);

/* --- Public API implementation --- */

fn_((HashSet_empty(TypeInfo key_ty, P_const$HashSet_Ctx ctx))(HashSet)) {
    claim_assert_nonnull(ctx);
    let_ignore = key_ty;
    return (HashSet){
        .metadata = none(),
        .size = 0,
        .available = 0,
        .ctx = ctx,
        debug_only(.key_ty = key_ty)
    };
};

fn_((HashSet_init(TypeInfo key_ty, P_const$HashSet_Ctx ctx, mem_Allocator gpa, u32 cap))(mem_Err$HashSet) $scope) {
    claim_assert_nonnull(ctx);
    var set = HashSet_empty(key_ty, ctx);
    if (cap > 0) {
        let actual_cap = prim_max(HashSet__capacityForSize(cap), HashSet_default_min_cap);
        try_(HashSet__alloc(&set, key_ty, gpa, actual_cap));
        HashSet__initMetadata(&set);
        set.available = (actual_cap * HashSet_default_max_load_ratio) / 100;
    }
    return_ok(set);
} $unscoped_(fn);

fn_((HashSet_fini(HashSet* self, TypeInfo key_ty, mem_Allocator gpa))(void)) {
    claim_assert_nonnull(self);
    debug_assert_eqBy(self->key_ty, key_ty, TypeInfo_eq);
    HashSet__free(self, key_ty, gpa);
    *self = HashSet_empty(key_ty, self->ctx);
};

fn_((HashSet_clone(HashSet self, TypeInfo key_ty, mem_Allocator gpa))(mem_Err$HashSet) $scope) {
    return_ok(try_(HashSet_cloneWithCtx(self, key_ty, self.ctx, gpa)));
} $unscoped_(fn);

fn_((HashSet_cloneWithCtx(
    HashSet self, TypeInfo key_ty, P_const$HashSet_Ctx ctx, mem_Allocator gpa
))(mem_Err$HashSet) $scope) {
    debug_assert_eqBy(self.key_ty, key_ty, TypeInfo_eq);

    var_(other, HashSet) = HashSet_empty(key_ty, ctx);
    if (self.size == 0) {
        return_ok(other);
    }

    let new_cap = HashSet__capacityForSize(self.size);
    try_(HashSet__alloc(&other, key_ty, gpa, new_cap));
    HashSet__initMetadata(&other);
    other.available = (new_cap * HashSet_default_max_load_ratio) / 100;

    let cap = HashSet_cap(self);
    for_(($r(0, cap))(i) {
        let ctrl = *HashSet__metadataAt(self, i);
        if (HashMap_Ctrl_isUsed(ctrl)) {
            let k = u_load(u_deref(HashSet__keyAt(self, key_ty, i)));
            HashSet_putWithin(&other, k);
            if (other.size == self.size) { break; }
        }
    });

    return_ok(other);
} $unscoped_(fn);

fn_((HashSet_count(HashSet self))(u32)) {
    return self.size;
};

fn_((HashSet_cap(HashSet self))(u32)) {
    if_none(self.metadata) { return 0; }
    return HashSet__header(self)->cap;
};

fn_((HashSet_ensureCap(HashSet* self, TypeInfo key_ty, mem_Allocator gpa, u32 new_size))(mem_Err$void) $scope) {
    claim_assert_nonnull(self);
    debug_assert_eqBy(self->key_ty, key_ty, TypeInfo_eq);
    if (new_size > self->size) {
        try_(HashSet__growIfNeeded(self, key_ty, gpa, new_size - self->size));
    }
    return_ok({});
} $unscoped_(fn);

fn_((HashSet_ensureUnusedCap(HashSet* self, TypeInfo key_ty, mem_Allocator gpa, u32 additional))(mem_Err$void)) {
    claim_assert_nonnull(self);
    debug_assert_eqBy(self->key_ty, key_ty, TypeInfo_eq);
    return HashSet_ensureCap(self, key_ty, gpa, self->size + additional);
};

fn_((HashSet_clearRetainingCap(HashSet* self))(void)) {
    claim_assert_nonnull(self);
    if_none(self->metadata) { return; }
    HashSet__initMetadata(self);
    self->size = 0;
    self->available = (HashSet_cap(*self) * HashSet_default_max_load_ratio) / 100;
};

fn_((HashSet_clearAndFree(HashSet* self, TypeInfo key_ty, mem_Allocator gpa))(void)) {
    claim_assert_nonnull(self);
    debug_assert_eqBy(self->key_ty, key_ty, TypeInfo_eq);
    HashSet__free(self, key_ty, gpa);
    self->size = 0;
    self->available = 0;
};

fn_((HashSet_for(HashSet self, u_V$raw key, u_V$raw ret_mem))(O$u_V$raw) $scope) {
    debug_assert_eqBy(self.key_ty, key.type, TypeInfo_eq);
    if_some((HashSet__idx(self, key))(idx)) {
        return_some({ .inner = u_memcpy(ret_mem.ref, HashSet__keyAt(self, key.type, idx).as_const).raw });
    }
    return_none();
} $unscoped_(fn);

fn_((HashSet_ptrFor(HashSet self, u_V$raw key))(O$u_P_const$raw) $scope) {
    debug_assert_eqBy(self.key_ty, key.type, TypeInfo_eq);
    if_some((HashSet__idx(self, key))(idx)) {
        return_some(HashSet__keyAt(self, key.type, idx).as_const);
    }
    return_none();
} $unscoped_(fn);

fn_((HashSet_ptrMutFor(HashSet self, u_V$raw key))(O$u_P$raw) $scope) {
    debug_assert_eqBy(self.key_ty, key.type, TypeInfo_eq);
    if_some((HashSet__idx(self, key))(idx)) {
        return_some(HashSet__keyAt(self, key.type, idx));
    }
    return_none();
} $unscoped_(fn);

fn_((HashSet_entry(HashSet self, u_V$raw key))(O$HashSet_Entry) $scope) {
    debug_assert_eqBy(self.key_ty, key.type, TypeInfo_eq);
    if_some((HashSet__idx(self, key))(idx)) {
        let k = HashSet__keyAt(self, key.type, idx).as_const;
        return_some({ .key = k.raw, debug_only(.key_ty = key.type) });
    }
    return_none();
} $unscoped_(fn);

fn_((HashSet_entryMut(HashSet self, u_V$raw key))(O$HashSet_EntryMut) $scope) {
    debug_assert_eqBy(self.key_ty, key.type, TypeInfo_eq);
    if_some((HashSet__idx(self, key))(idx)) {
        let k = HashSet__keyAt(self, key.type, idx);
        return_some({ .key = k.raw, debug_only(.key_ty = key.type) });
    }
    return_none();
} $unscoped_(fn);

fn_((HashSet_contains(HashSet self, u_V$raw key))(bool)) {
    debug_assert_eqBy(self.key_ty, key.type, TypeInfo_eq);
    return isSome(HashSet__idx(self, key));
};

#if UNUSED_CODE
fn_((HashSet_add(HashSet* self, mem_Allocator gpa, u_V$raw key))(mem_Err$bool) $scope) {
    claim_assert_nonnull(self);
    debug_assert_eqBy(self->key_ty, key.type, TypeInfo_eq);
    let prev_count = self->size;
    let ensured = try_(HashSet_ensure(self, gpa, key));
    let_ignore = ensured;
    return_ok(prev_count != self->size);
} $unscoped_(fn);

fn_((HashSet_addWithin(HashSet* self, u_V$raw key))(bool)) {
    claim_assert_nonnull(self);
    debug_assert_eqBy(self->key_ty, key.type, TypeInfo_eq);
    let prev_count = self->size;
    let ensured = HashSet_ensureWithin(self, key);
    let_ignore = ensured;
    return prev_count != self->size;
};
#endif /* UNUSED_CODE */

fn_((HashSet_put(HashSet* self, mem_Allocator gpa, u_V$raw key))(mem_Err$void) $scope) {
    claim_assert_nonnull(self);
    debug_assert_eqBy(self->key_ty, key.type, TypeInfo_eq);
    try_(HashSet__growIfNeeded(self, key.type, gpa, 1));
    HashSet_putWithin(self, key);
    return_ok({});
} $unscoped_(fn);

fn_((HashSet_putWithin(HashSet* self, u_V$raw key))(void)) {
    claim_assert_nonnull(self);
    debug_assert_eqBy(self->key_ty, key.type, TypeInfo_eq);
    claim_assert(!HashSet_contains(*self, key));
    let ctx = self->ctx;
    let hash = ctx->hashFn(key, u_load(u_deref(ctx->inner)));
    let cap = HashSet_cap(*self);
    let mask = cap - 1;
    var_(idx, usize) = hash & mask;
    while (HashMap_Ctrl_isUsed(*HashSet__metadataAt(*self, idx))) {
        idx = (idx + 1) & mask;
    }
    claim_assert(self->available > 0);
    self->available--;

    let fingerprint = HashMap_Ctrl_takeFingerprint(hash);
    HashMap_Ctrl_fill(HashSet__metadataAt(*self, idx), fingerprint);
    u_memcpy(HashSet__keyAt(*self, key.type, idx), key.ref.as_const);
    self->size++;
};

fn_((HashSet_fetchPut(HashSet* self, mem_Allocator gpa, u_V$raw key, V$HashSet_Sgl$raw ret_mem))(mem_Err$O$V$HashSet_Sgl$raw) $scope) {
    claim_assert_nonnull(self);
    debug_assert_eqBy(self->key_ty, key.type, TypeInfo_eq);
    let ensured = try_(HashSet_ensure(self, gpa, key));
    let result = expr_(O$V$HashSet_Sgl$raw $scope)(if (ensured.found_existing) {
        let k = HashSet_Ensured_key(ensured, key.type);
        $break_(some(HashSet_Sgl_init(u_load(u_deref(k)), ret_mem)));
    } else_none {
        $break_(none());
    }) $unscoped_(expr);
    return_ok(result);
} $unscoped_(fn);

fn_((HashSet_fetchPutWithin(HashSet* self, u_V$raw key, V$HashSet_Sgl$raw ret_mem))(O$V$HashSet_Sgl$raw) $scope) {
    claim_assert_nonnull(self);
    debug_assert_eqBy(self->key_ty, key.type, TypeInfo_eq);
    let ensured = HashSet_ensureWithin(self, key);
    let result = expr_(O$V$HashSet_Sgl$raw $scope)(if (ensured.found_existing) {
        let k = HashSet_Ensured_key(ensured, key.type);
        $break_(some(HashSet_Sgl_init(u_load(u_deref(k)), ret_mem)));
    } else_none {
        $break_(none());
    }) $unscoped_(expr);
    return result;
} $unscoped_(fn);

fn_((HashSet_ensure(HashSet* self, mem_Allocator gpa, u_V$raw key))(mem_Err$HashSet_Ensured) $scope) {
    claim_assert_nonnull(self);
    debug_assert_eqBy(self->key_ty, key.type, TypeInfo_eq);
    // Try to grow first, but if it fails and key exists, we can still return it
    catch_((HashSet__growIfNeeded(self, key.type, gpa, 1))(err, if_some((HashSet__idx(*self, key))(idx)) {
        let k = HashSet__keyAt(*self, key.type, idx);
        return_ok((HashSet_Ensured){
            .key = k.raw,
            .found_existing = true,
            debug_only(.key_ty = key.type) });
    } else_none {
        return_err(err);
    }));
    return_ok(HashSet_ensureWithin(self, key));
} $unscoped_(fn);

fn_((HashSet_ensureWithin(HashSet* self, u_V$raw key))(HashSet_Ensured)) {
    claim_assert_nonnull(self);
    debug_assert_eqBy(self->key_ty, key.type, TypeInfo_eq);
    let ctx = self->ctx;
    let hash = ctx->hashFn(key, u_load(u_deref(ctx->inner)));
    let cap = HashSet_cap(*self);
    let mask = cap - 1;
    let fingerprint = HashMap_Ctrl_takeFingerprint(hash);

    var_(first_tombstone_idx, usize) = cap; // Invalid index
    var_(idx, usize) = hash & mask;
    var_(limit, u32) = cap;
    while (limit > 0) {
        let ctrl = *HashSet__metadataAt(*self, idx);
        if (HashMap_Ctrl_isFree(ctrl)) {
            break;
        }
        if (HashMap_Ctrl_isUsed(ctrl) && ctrl.fingerprint == fingerprint) {
            if (ctx->eqlFn(key, u_load(u_deref(HashSet__keyAt(*self, key.type, idx))), u_load(u_deref(ctx->inner)))) {
                return (HashSet_Ensured){
                    .key = HashSet__keyAt(*self, key.type, idx).raw,
                    .found_existing = true,
                    debug_only(.key_ty = key.type)
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
    HashMap_Ctrl_fill(HashSet__metadataAt(*self, idx), fingerprint);
    u_memcpy(HashSet__keyAt(*self, key.type, idx), key.ref.as_const);
    self->size++;
    return (HashSet_Ensured){
        .key = HashSet__keyAt(*self, key.type, idx).raw,
        .found_existing = false,
        debug_only(.key_ty = key.type)
    };
};

fn_((HashSet_remove(HashSet* self, u_V$raw key))(bool)) {
    claim_assert_nonnull(self);
    debug_assert_eqBy(self->key_ty, key.type, TypeInfo_eq);
    if_some((HashSet__idx(*self, key))(idx)) {
        HashMap_Ctrl_remove(HashSet__metadataAt(*self, idx));
        self->size--;
        self->available++;
        return true;
    }
    return false;
};

fn_((HashSet_fetchRemove(HashSet* self, u_V$raw key, V$HashSet_Sgl$raw ret_mem))(O$V$HashSet_Sgl$raw) $scope) {
    claim_assert_nonnull(self);
    debug_assert_eqBy(self->key_ty, key.type, TypeInfo_eq);
    if_some((HashSet__idx(*self, key))(idx)) {
        let old_key = HashSet__keyAt(*self, key.type, idx);
        let result = HashSet_Sgl_init(u_load(u_deref(old_key)), ret_mem);
        HashMap_Ctrl_remove(HashSet__metadataAt(*self, idx));
        mem_set0(u_prefixP(old_key, 1));
        self->size--;
        self->available++;
        return_some(result);
    }
    return_none();
} $unscoped_(fn);

fn_((HashSet_removeByPtr(HashSet* self, u_P$raw key_ptr))(void)) {
    claim_assert_nonnull(self);
    claim_assert_nonnull(key_ptr.raw);
    debug_assert_eqBy(self->key_ty, key_ptr.type, TypeInfo_eq);
    let idx = expr_(u32 $scope)(if (key_ptr.type.size > 0) {
        $break_((intFromPtr(key_ptr.raw) - intFromPtr(HashSet__keys(*self, key_ptr.type).raw)) / key_ptr.type.size);
    }) expr_(else)({
        $break_(0);
    }) $unscoped_(expr);
    HashMap_Ctrl_remove(HashSet__metadataAt(*self, idx));
    self->size--;
    self->available++;
};

fn_((HashSet_rehash(HashSet* self, TypeInfo key_ty))(void)) {
    claim_assert_nonnull(self);
    debug_assert_eqBy(self->key_ty, key_ty, TypeInfo_eq);

    if_none(self->metadata) { return; }

    let cap = HashSet_cap(*self);
    let mask = cap - 1;
    let ctx = self->ctx;

    // Mark all slots as free but keep used flag
    for_(($r(0, cap))(i) {
        HashSet__metadataAt(*self, i)->fingerprint = HashMap_Ctrl_free;
    });

    // Rehash all used slots
    var_(curr, u32) = 0;
    while (curr < cap) {
        let ctrl = HashSet__metadataAt(*self, curr);
        if (!HashMap_Ctrl_isUsed(*ctrl)) {
            claim_assert(HashMap_Ctrl_isFree(*ctrl));
            curr++;
            continue;
        }
        let key = HashSet__keyAt(*self, key_ty, curr);
        let hash = ctx->hashFn(u_load(u_deref(key)), u_load(u_deref(ctx->inner)));
        let fingerprint = HashMap_Ctrl_takeFingerprint(hash);
        var_(idx, usize) = hash & mask;

        // Find target slot
        while ((idx < curr && HashMap_Ctrl_isUsed(*HashSet__metadataAt(*self, idx)))
               || (idx > curr && HashMap_Ctrl_isTombstone(*HashSet__metadataAt(*self, idx)))) {
            idx = (idx + 1) & mask;
        }
        if (idx < curr) {
            // Move to earlier slot
            claim_assert(HashMap_Ctrl_isFree(*HashSet__metadataAt(*self, idx)));
            HashMap_Ctrl_fill(HashSet__metadataAt(*self, idx), fingerprint);

            // Copy key
            let dst_key = HashSet__keyAt(*self, key_ty, idx);
            let src_key = HashSet__keyAt(*self, key_ty, curr);
            u_memcpy(dst_key, src_key.as_const);

            ctrl->used = 0;
            claim_assert(HashMap_Ctrl_isFree(*ctrl));
            curr++;
        } else if (idx == curr) {
            ctrl->fingerprint = fingerprint;
            curr++;
        } else {
            // Swap with later slot
            let target_ctrl = HashSet__metadataAt(*self, idx);
            HashMap_Ctrl_remove(target_ctrl);
            if (HashMap_Ctrl_isUsed(*target_ctrl)) {
                mem_swap(
                    u_prefixP(HashSet__keyAt(*self, key_ty, curr), 1),
                    u_suffixP(HashSet__keyAt(*self, key_ty, idx), 1)
                );
            } else {
                target_ctrl->used = 1;
                u_memcpy(HashSet__keyAt(*self, key_ty, idx), HashSet__keyAt(*self, key_ty, curr).as_const);
                ctrl->fingerprint = HashMap_Ctrl_free;
                ctrl->used = 0;
                curr++;
            }
        }
    }
};

fn_((HashSet_isSubset(HashSet self, TypeInfo key_ty, HashSet other))(bool)) {
    debug_assert_eqBy(self.key_ty, key_ty, TypeInfo_eq);
    debug_assert_eqBy(key_ty, other.key_ty, TypeInfo_eq);
    if (self.size > other.size) { return false; }
    if (self.size == 0) { return true; }

    let cap = HashSet_cap(self);
    for_(($r(0, cap))(i) {
        let ctrl = *HashSet__metadataAt(self, i);
        if (HashMap_Ctrl_isUsed(ctrl)) {
            let key = HashSet__keyAt(self, key_ty, i);
            if (!HashSet_contains(other, u_load(u_deref(key)))) {
                return false;
            }
        }
    });
    return true;
};

fn_((HashSet_isSuperset(HashSet self, TypeInfo key_ty, HashSet other))(bool)) {
    debug_assert_eqBy(self.key_ty, key_ty, TypeInfo_eq);
    debug_assert_eqBy(key_ty, other.key_ty, TypeInfo_eq);
    prim_swap(&self, &other);
    return HashSet_isSubset(self, key_ty, other);
};

fn_((HashSet_isDisjoint(HashSet self, TypeInfo key_ty, HashSet other))(bool)) {
    debug_assert_eqBy(self.key_ty, key_ty, TypeInfo_eq);
    debug_assert_eqBy(key_ty, other.key_ty, TypeInfo_eq);
    // Check the smaller set's elements against the larger set
    let smaller = self.size <= other.size ? self : other;
    let larger = self.size <= other.size ? other : self;

    if (smaller.size == 0) { return true; }

    let cap = HashSet_cap(smaller);
    for_(($r(0, cap))(i) {
        let ctrl = *HashSet__metadataAt(smaller, i);
        if (HashMap_Ctrl_isUsed(ctrl)) {
            let key = HashSet__keyAt(smaller, key_ty, i);
            if (HashSet_contains(larger, u_load(u_deref(key)))) {
                return false;
            }
        }
    });
    return true;
};

/* --- Iterator implementations --- */

fn_((HashSet_iter(const HashSet* self, TypeInfo key_ty))(HashSet_Iter)) {
    claim_assert_nonnull(self);
    debug_assert_eqBy(self->key_ty, key_ty, TypeInfo_eq);
    let_ignore = key_ty;
    return (HashSet_Iter){
        .set = self,
        .idx = 0,
        debug_only(.key_ty = key_ty)
    };
};

fn_((HashSet_Iter_next(HashSet_Iter* self, TypeInfo key_ty))(O$HashSet_Entry) $scope) {
    claim_assert_nonnull(self);
    claim_assert_nonnull(self->set);
    debug_assert_eqBy(self->key_ty, key_ty, TypeInfo_eq);
    if (self->set->size == 0) { return_none(); }
    let cap = HashSet_cap(*self->set);
    while (cap > self->idx) {
        let ctrl = *HashSet__metadataAt(*self->set, self->idx);
        if (HashMap_Ctrl_isUsed(ctrl)) {
            return_some(expr_(HashSet_Entry $guard)({
                defer_(self->idx++);
                $break_((HashSet_Entry){
                    .key = HashSet__keyAt(*self->set, key_ty, self->idx).as_const.raw,
                    debug_only(.key_ty = key_ty) });
            }) $unguarded_(expr));
        }
        self->idx++;
    }
    return_none();
} $unscoped_(fn);

fn_((HashSet_Iter_nextMut(HashSet_Iter* self, TypeInfo key_ty))(O$HashSet_EntryMut) $scope) {
    claim_assert_nonnull(self);
    claim_assert_nonnull(self->set);
    debug_assert_eqBy(self->key_ty, key_ty, TypeInfo_eq);
    if (self->set->size == 0) { return_none(); }
    let cap = HashSet_cap(*self->set);
    while (cap > self->idx) {
        let ctrl = *HashSet__metadataAt(*self->set, self->idx);
        if (HashMap_Ctrl_isUsed(ctrl)) {
            return_some(expr_(HashSet_EntryMut $guard)({
                defer_(self->idx++);
                $break_((HashSet_EntryMut){
                    .key = HashSet__keyAt(*self->set, key_ty, self->idx).raw,
                    debug_only(.key_ty = key_ty) });
            }) $unguarded_(expr));
        }
        self->idx++;
    }
    return_none();
} $unscoped_(fn);

fn_((HashSet_keyIter(HashSet self, TypeInfo key_ty))(HashSet_KeyIter) $scope) {
    debug_assert_eqBy(self.key_ty, key_ty, TypeInfo_eq);
    return_(expr_(HashSet_KeyIter $scope)(
        if_some((self.metadata)(metadata)) {
        $break_((HashSet_KeyIter){
            .len = HashSet_cap(self),
            .metadata = metadata,
            .keys = HashSet__keys(self, key_ty).raw,
                debug_only(.key_ty = key_ty) });
    } else_none {
        $break_((HashSet_KeyIter){
            .len = 0,
            .metadata = null,
            .keys = null,
                debug_only(.key_ty = key_ty) });
    }) $unscoped_(expr));
} $unscoped_(fn);

fn_((HashSet_KeyIter_next(HashSet_KeyIter* self, TypeInfo key_ty))(O$u_P_const$raw) $scope) {
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

fn_((HashSet_KeyIter_nextMut(HashSet_KeyIter* self, TypeInfo key_ty))(O$u_P$raw) $scope) {
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

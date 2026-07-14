#include "dh/HashMap.h"
#include "dh/hash.h"
#include "dh/prl/simd.h"
#include "dh/u-meta.h"

/*========== SIMD Configuration =============================================*/

#define HashMap__use_simd simd_supported
#if HashMap__use_simd
#define HashMap__simd_group_size (simd_width_bits / arch_bits_per_byte)
T_alias$((HashMap__SimdGroup)(simd_V$$(HashMap__simd_group_size, u8)));
#endif /* HashMap__use_simd */

/*========== Definitions ====================================================*/

$static fn_((HashMap_Pair__init(u_V$raw key, u_V$raw val, V$HashMap_Pair$raw ret_mem))(V$HashMap_Pair$raw)) {
    ret_mem->key_ty = $typing(key.type);
    ret_mem->val_ty = $typing(val.type);
    u_memcpy(HashMap_Pair_keyMut(ret_mem, key.type, val.type), key.ref.as_const);
    u_memcpy(HashMap_Pair_valMut(ret_mem, key.type, val.type), val.ref.as_const);
    return ret_mem;
};

fn_((HashMap_Pair_key(const HashMap_Pair$raw* self, TypeInfo key_ty, TypeInfo val_ty))(u_P_const$raw)) {
    debug_assert_eqBy($typed(self->key_ty), key_ty, TypeInfo_eql);
    debug_assert_eqBy($typed(self->val_ty), val_ty, TypeInfo_eql);
    let ty_fields = A_ref$((S_const$TypeInfo)(with_((u_Fields_type$HashMap_Pair)(
        (.val[u_Fields_Idx_key_$HashMap_Pair])(key_ty),
        (.val[u_Fields_Idx_val_$HashMap_Pair])(val_ty)
    ))));
    let u_self = P_meta((u_typeInfoRecord(ty_fields))(as$(P_const$raw)(self)));
    return u_fieldPtr(u_self, ty_fields, u_Fields_Idx_key_$HashMap_Pair);
};

fn_((HashMap_Pair_keyMut(HashMap_Pair$raw* self, TypeInfo key_ty, TypeInfo val_ty))(u_P$raw)) {
    debug_assert_eqBy($typed(self->key_ty), key_ty, TypeInfo_eql);
    debug_assert_eqBy($typed(self->val_ty), val_ty, TypeInfo_eql);
    let ty_fields = A_ref$((S_const$TypeInfo)(with_((u_Fields_type$HashMap_Pair)(
        (.val[u_Fields_Idx_key_$HashMap_Pair])(key_ty),
        (.val[u_Fields_Idx_val_$HashMap_Pair])(val_ty)
    ))));
    let u_self = P_meta((u_typeInfoRecord(ty_fields))(as$(P$raw)(self)));
    return u_fieldPtrMut(u_self, ty_fields, u_Fields_Idx_key_$HashMap_Pair);
};

fn_((HashMap_Pair_val(const HashMap_Pair$raw* self, TypeInfo key_ty, TypeInfo val_ty))(u_P_const$raw)) {
    debug_assert_eqBy($typed(self->key_ty), key_ty, TypeInfo_eql);
    debug_assert_eqBy($typed(self->val_ty), val_ty, TypeInfo_eql);
    let ty_fields = A_ref$((S_const$TypeInfo)(with_((u_Fields_type$HashMap_Pair)(
        (.val[u_Fields_Idx_key_$HashMap_Pair])(key_ty),
        (.val[u_Fields_Idx_val_$HashMap_Pair])(val_ty)
    ))));
    let u_self = P_meta((u_typeInfoRecord(ty_fields))(as$(P_const$raw)(self)));
    return u_fieldPtr(u_self, ty_fields, u_Fields_Idx_val_$HashMap_Pair);
};

fn_((HashMap_Pair_valMut(HashMap_Pair$raw* self, TypeInfo key_ty, TypeInfo val_ty))(u_P$raw)) {
    debug_assert_eqBy($typed(self->key_ty), key_ty, TypeInfo_eql);
    debug_assert_eqBy($typed(self->val_ty), val_ty, TypeInfo_eql);
    let ty_fields = A_ref$((S_const$TypeInfo)(with_((u_Fields_type$HashMap_Pair)(
        (.val[u_Fields_Idx_key_$HashMap_Pair])(key_ty),
        (.val[u_Fields_Idx_val_$HashMap_Pair])(val_ty)
    ))));
    let u_self = P_meta((u_typeInfoRecord(ty_fields))(as$(P$raw)(self)));
    return u_fieldPtrMut(u_self, ty_fields, u_Fields_Idx_val_$HashMap_Pair);
};

fn_((HashMap_Entry_key(HashMap_Entry self, TypeInfo key_ty))(u_P_const$raw)) {
    debug_assert_eqBy($typed(self.key_ty), key_ty, TypeInfo_eql);
    return (u_P_const$raw){ .raw = self.key, .type = key_ty };
};

fn_((HashMap_Entry_val(HashMap_Entry self, TypeInfo val_ty))(u_P_const$raw)) {
    debug_assert_eqBy($typed(self.val_ty), val_ty, TypeInfo_eql);
    return (u_P_const$raw){ .raw = self.val, .type = val_ty };
};

fn_((HashMap_EntryMut_key(HashMap_EntryMut self, TypeInfo key_ty))(u_P$raw)) {
    debug_assert_eqBy($typed(self.key_ty), key_ty, TypeInfo_eql);
    return (u_P$raw){ .raw = self.key, .type = key_ty };
};

fn_((HashMap_EntryMut_val(HashMap_EntryMut self, TypeInfo val_ty))(u_P$raw)) {
    debug_assert_eqBy($typed(self.val_ty), val_ty, TypeInfo_eql);
    return (u_P$raw){ .raw = self.val, .type = val_ty };
};

fn_((HashMap_Ensured_key(HashMap_Ensured self, TypeInfo key_ty))(u_P_const$raw)) {
    debug_assert_eqBy($typed(self.key_ty), key_ty, TypeInfo_eql);
    return (u_P_const$raw){ .raw = self.key, .type = key_ty };
};

fn_((HashMap_Ensured_keyMut(HashMap_Ensured self, TypeInfo key_ty))(u_P$raw)) {
    debug_assert_eqBy($typed(self.key_ty), key_ty, TypeInfo_eql);
    return (u_P$raw){ .raw = self.key, .type = key_ty };
};

fn_((HashMap_Ensured_val(HashMap_Ensured self, TypeInfo val_ty))(u_P_const$raw)) {
    debug_assert_eqBy($typed(self.val_ty), val_ty, TypeInfo_eql);
    return (u_P_const$raw){ .raw = self.val, .type = val_ty };
};

fn_((HashMap_Ensured_valMut(HashMap_Ensured self, TypeInfo val_ty))(u_P$raw)) {
    debug_assert_eqBy($typed(self.val_ty), val_ty, TypeInfo_eql);
    return (u_P$raw){ .raw = self.val, .type = val_ty };
};

fn_((HashMap_Ensured_foundExisting(
    HashMap_Ensured self, TypeInfo key_ty, TypeInfo val_ty
))(O$HashMap_Entry)) {
    return expr_(O$HashMap_Entry $scope)(if (self.found_existing) {
        $break_(some({
            .key = self.key,
            .key_ty = $typing(key_ty),
            .val = self.val,
            .val_ty = $typing(val_ty),
        }));
    }) expr_(else)({
        $break_(none());
    }) $unscoped(expr);
};

fn_((HashMap_Ensured_foundExistingMut(
    HashMap_Ensured self, TypeInfo key_ty, TypeInfo val_ty
))(O$HashMap_EntryMut)) {
    return expr_(O$HashMap_EntryMut $scope)(if (self.found_existing) {
        $break_(some({
            .key = self.key,
            .key_ty = $typing(key_ty),
            .val = self.val,
            .val_ty = $typing(val_ty),
        }));
    }) expr_(else)({
        $break_(none());
    }) $unscoped(expr);
};

fn_((HashMap_HashFn_default(u_V$raw val, u_V$raw ctx))(u64)) {
    let_ignore = ctx;
    return hash_bytes64(mem_asBytes(val.ref.as_const));
};

fn_((HashMap_EqlFn_default(u_V$raw lhs, u_V$raw rhs, u_V$raw ctx))(bool)) {
    let_ignore = ctx;
    return u_eql(lhs, rhs);
};

fn_((HashMap_LoadRatio_default(void))(HashMap_LoadRatio)) {
    return HashMap_LoadRatio_limit(HashMap_LoadRatio_default_max);
};

fn_((HashMap_Ctx_default(void))(HashMap_Ctx)) {
    $static let_(default_ctx_inner, Void) = {};
    $static let_(default_ctx, HashMap_Ctx) = {
        .inner = u_anyP(&default_ctx_inner),
        .hashFn = HashMap_HashFn_default,
        .eqlFn = HashMap_EqlFn_default,
        .load_ratio = cleared(),
    };
    return HashMap_Ctx_ensureValid(
        with_((default_ctx)((.load_ratio)(
            HashMap_LoadRatio_default()
        ))));
};

$static fn_((HashMap__header(HashMap self))(HashMap_Header*)) {
    let metadata_ptr = unwrap_(self.metadata);
    return ptrAlignCast$((HashMap_Header*)((as$(u8*)(metadata_ptr)) - sizeOf$(HashMap_Header)));
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

$static fn_((HashMap__capForSize(HashMap_LoadRatio load_ratio, u32 size))(u32)) {
    var_(new_cap, u64) = (as$(u64)(size) * 100) / HashMap_LoadRatio_max(load_ratio) + 1;
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
    mem_set0(u_anyS(P_slice((unwrap_(self->metadata))($r(0, HashMap_cap(*self))))));
};

$static fn_((HashMap__alloc(HashMap* self, TypeInfo key_ty, TypeInfo val_ty, mem_Alctr gpa, u32 new_cap))(mem_E$void) $scope) {
    let header_align = alignOf$(HashMap_Header);
    let key_align = key_ty.size == 0 ? 1 : TypeInfo_align(key_ty);
    let val_align = val_ty.size == 0 ? 1 : TypeInfo_align(val_ty);
    let max_align = pri_max(header_align, pri_max(key_align, val_align));

    let meta_size = sizeOf$(HashMap_Header) + as$(usize)(new_cap)*sizeOf$(HashMap_Ctrl);
    let keys_start = mem_alignFwd(meta_size, key_align);
    let keys_end = keys_start + as$(usize)(new_cap)*key_ty.size;
    let vals_start = mem_alignFwd(keys_end, val_align);
    let vals_end = vals_start + as$(usize)(new_cap)*val_ty.size;
    let total_size = mem_alignFwd(vals_end, max_align);

    let slice = try_(mem_Alctr_allocBytes($trace gpa, total_size));
    let ptr = slice.ptr;
    let hdr = ptrAlignCast$((HashMap_Header*)(ptr));
    asg_l((hdr)({
        .vals = ptr + vals_start,
        .val_ty = $typing(val_ty),
        .keys = ptr + keys_start,
        .key_ty = $typing(key_ty),
        .cap = new_cap,
    }));

    asg_l((&self->metadata)(some(as$(HashMap_Ctrl*)(ptr + sizeOf$(HashMap_Header)))));
    return_ok({});
} $unscoped(fn);

$static fn_((HashMap__free(HashMap* self, TypeInfo key_ty, TypeInfo val_ty, mem_Alctr gpa))(void)) {
    if_none(self->metadata) { return; }

    let header_align = alignOf$(HashMap_Header);
    let key_align = key_ty.size == 0 ? 1 : TypeInfo_align(key_ty);
    let val_align = val_ty.size == 0 ? 1 : TypeInfo_align(val_ty);
    let max_align = pri_max(header_align, pri_max(key_align, val_align));

    let cap = HashMap_cap(*self);
    let meta_size = sizeOf$(HashMap_Header) + as$(usize)(cap)*sizeOf$(HashMap_Ctrl);
    let keys_start = mem_alignFwd(meta_size, key_align);
    let keys_end = keys_start + as$(usize)(cap)*key_ty.size;
    let vals_start = mem_alignFwd(keys_end, val_align);
    let vals_end = vals_start + as$(usize)(cap)*val_ty.size;
    let total_size = mem_alignFwd(vals_end, max_align);

    let ptr = as$(u8*)(HashMap__header(*self));
    mem_Alctr_freeBytes($trace gpa, P_prefix$((S$u8)(ptr)(total_size)));

    asg_l((&self->metadata)(none()));
    self->available = 0;
};

#if HashMap__use_simd
$static fn_((HashMap__idx_simd(HashMap self, u_V$raw key))(O$usize));
#endif /* HashMap__use_simd */
$static fn_((HashMap__idx(HashMap self, u_V$raw key))(O$usize) $scope) {
#if HashMap__use_simd
    /* Use SIMD path when capacity is aligned to group size */
    if (HashMap_cap(self) >= HashMap__simd_group_size && (HashMap_cap(self) % HashMap__simd_group_size) == 0) {
        return HashMap__idx_simd(self, key);
    }
#endif /* HashMap__use_simd */

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
} $unscoped(fn);

#if HashMap__use_simd
/// Load control bytes and find matches for fingerprint
$attr($inline_always)
$static fn_((HashMap__simd_match_fingerprint(const HashMap_Ctrl* group, u8 fingerprint))(u32));
/// Find free (not tombstone) slots in a group
$attr($inline_always)
$static fn_((HashMap__simd_match_free(const HashMap_Ctrl* group))(u32));
/// SIMD-accelerated lookup
fn_((HashMap__idx_simd(HashMap self, u_V$raw key))(O$usize) $scope) {
    if (self.size == 0) return_none();

    let ctx = self.ctx;
    let hash = ctx->hashFn(key, u_load(u_deref(ctx->inner)));
    let cap = HashMap_cap(self);
    let fingerprint = HashMap_Ctrl_takeFingerprint(hash);

    let start_idx = hash & (cap - 1);
    let start_group = start_idx / HashMap__simd_group_size;
    let start_offset_in_group = start_idx % HashMap__simd_group_size;
    let num_groups = cap / HashMap__simd_group_size;

    for_(($r(0, num_groups))(group_offset)) {
        let group_idx = (start_group + group_offset) % num_groups;
        let group_start = group_idx * HashMap__simd_group_size;
        let group = HashMap__metadataAt(self, group_start);

        var_(match_mask, u32) = HashMap__simd_match_fingerprint(group, fingerprint);
        if (group_offset == 0 && start_offset_in_group > 0) {
            match_mask &= (~0u << start_offset_in_group);
        }
        while (match_mask != 0) {
            let bit_pos = mem_trailingZeros32(match_mask);
            let idx = group_start + bit_pos;
            if (ctx->eqlFn(key, u_load(u_deref(HashMap__keyAt(self, key.type, idx))), u_load(u_deref(ctx->inner)))) {
                return_some(idx);
            }
            match_mask &= match_mask - 1;
        }

        var_(free_mask, u32) = HashMap__simd_match_free(group);
        if (group_offset == 0 && start_offset_in_group > 0) {
            free_mask &= (~0u << start_offset_in_group);
        }
        if (free_mask != 0) {
            return_none();
        }
    } $end(for);

    return_none();
} $unscoped(fn);

fn_((HashMap__simd_match_fingerprint(const HashMap_Ctrl* group, u8 fingerprint))(u32)) {
    let haystack = simd_V_load$((HashMap__SimdGroup)(mem_asBytes(u_anyP(ensureNonnull(group))).ptr));
    let needle = simd_V_splat(haystack, fingerprint | 0x80);
    return as$(u32)(simd_V_bool_bitMask(simd_V_int_eq(haystack, needle)));
};
fn_((HashMap__simd_match_free(const HashMap_Ctrl* group))(u32)) {
    let haystack = simd_V_load$((HashMap__SimdGroup)(mem_asBytes(u_anyP(ensureNonnull(group))).ptr));
    return as$(u32)(simd_V_bool_bitMask(simd_V_int_eq(haystack, simd_V_splat(haystack, 0))));
};
#endif /* HashMap__use_simd */

$static fn_((HashMap__grow(HashMap* self, TypeInfo key_ty, TypeInfo val_ty, mem_Alctr gpa, u32 new_capacity))(mem_E$void) $scope) {
    let new_cap = pri_max(new_capacity, HashMap_default_min_cap);
    claim_assert(new_cap > HashMap_cap(*self));

    var_(new_map, HashMap) = HashMap_empty(key_ty, val_ty, self->ctx);
    try_(HashMap__alloc(&new_map, key_ty, val_ty, gpa, new_cap));
    HashMap__initMetadata(&new_map);
    new_map.available = (new_cap * HashMap_LoadRatio_max(self->ctx->load_ratio)) / 100;

    if (self->size != 0) {
        let old_cap = HashMap_cap(*self);
        for_(($r(0, old_cap))(i)) {
            let ctrl = *HashMap__metadataAt(*self, i);
            if (!HashMap_Ctrl_isUsed(ctrl)) { continue; }
            let k = u_load(u_deref(HashMap__keyAt(*self, key_ty, i)));
            let v = u_load(u_deref(HashMap__valAt(*self, val_ty, i)));
            HashMap_putNoClobberWithin(&new_map, k, v);
            if (new_map.size == self->size) { break; }
        } $end(for);
    }

    HashMap__free(self, key_ty, val_ty, gpa);
    *self = new_map;
    return_ok({});
} $unscoped(fn);

$static fn_((HashMap__growIfNeeded(HashMap* self, TypeInfo key_ty, TypeInfo val_ty, mem_Alctr gpa, u32 new_count))(mem_E$void) $scope) {
    if (new_count > self->available) {
        let load = (HashMap_cap(*self) * HashMap_LoadRatio_max(self->ctx->load_ratio)) / 100 - self->available;
        try_(HashMap__grow(self, key_ty, val_ty, gpa, HashMap__capForSize(self->ctx->load_ratio, load + new_count)));
    }
    return_ok({});
} $unscoped(fn);

fn_((HashMap_empty(
    TypeInfo key_ty, TypeInfo val_ty, P_const$HashMap_Ctx ctx
))(HashMap)) {
    claim_assert_nonnull(ctx);
    return (HashMap){
        .metadata = none(),
        .size = 0,
        .available = 0,
        .ctx = ctx,
        .key_ty = $typing(key_ty),
        .val_ty = $typing(val_ty),
    };
};

fn_((HashMap_init(
    TypeInfo key_ty, TypeInfo val_ty, P_const$HashMap_Ctx ctx, mem_Alctr gpa, u32 cap
))(mem_E$HashMap) $scope) {
    claim_assert_nonnull(ctx);
    var map = HashMap_empty(key_ty, val_ty, ctx);
    if (cap > 0) {
        let actual_cap = pri_max(HashMap__capForSize(ctx->load_ratio, cap), HashMap_default_min_cap);
        try_(HashMap__alloc(&map, key_ty, val_ty, gpa, actual_cap));
        HashMap__initMetadata(&map);
        map.available = (actual_cap * HashMap_LoadRatio_max(ctx->load_ratio)) / 100;
    }
    return_ok(map);
} $unscoped(fn);

fn_((HashMap_fini(
    HashMap* self, TypeInfo key_ty, TypeInfo val_ty, mem_Alctr gpa
))(void)) {
    claim_assert_nonnull(self);
    debug_assert_eqBy($typed(self->key_ty), key_ty, TypeInfo_eql);
    debug_assert_eqBy($typed(self->val_ty), val_ty, TypeInfo_eql);
    HashMap__free(self, key_ty, val_ty, gpa);
    *self = HashMap_empty(key_ty, val_ty, self->ctx);
};

fn_((HashMap_clone(
    HashMap self, TypeInfo key_ty, TypeInfo val_ty, mem_Alctr gpa
))(mem_E$HashMap) $scope) {
    return_ok(try_(HashMap_cloneWithCtx(self, key_ty, val_ty, self.ctx, gpa)));
} $unscoped(fn);

fn_((HashMap_cloneWithCtx(
    HashMap self, TypeInfo key_ty, TypeInfo val_ty, P_const$HashMap_Ctx ctx, mem_Alctr gpa
))(mem_E$HashMap) $scope) {
    debug_assert_eqBy($typed(self.key_ty), key_ty, TypeInfo_eql);
    debug_assert_eqBy($typed(self.val_ty), val_ty, TypeInfo_eql);
    var_(other, HashMap) = HashMap_empty(key_ty, val_ty, ctx);
    if (self.size == 0) {
        return_ok(other);
    }

    let new_cap = HashMap__capForSize(ctx->load_ratio, self.size);
    try_(HashMap__alloc(&other, key_ty, val_ty, gpa, new_cap));
    HashMap__initMetadata(&other);
    other.available = (new_cap * HashMap_LoadRatio_max(ctx->load_ratio)) / 100;

    let cap = HashMap_cap(self);
    for_(($r(0, cap))(i)) {
        let ctrl = *HashMap__metadataAt(self, i);
        if (HashMap_Ctrl_isUsed(ctrl)) {
            let k = u_load(u_deref(HashMap__keyAt(self, key_ty, i)));
            let v = u_load(u_deref(HashMap__valAt(self, val_ty, i)));
            HashMap_putNoClobberWithin(&other, k, v);
            if (other.size == self.size) { break; }
        }
    } $end(for);

    return_ok(other);
} $unscoped(fn);

fn_((HashMap_count(HashMap self))(u32)) {
    return self.size;
};

fn_((HashMap_cap(HashMap self))(u32)) {
    if_none(self.metadata) { return 0; }
    return HashMap__header(self)->cap;
};

fn_((HashMap_ensureCap(
    HashMap* self, TypeInfo key_ty, TypeInfo val_ty, mem_Alctr gpa, u32 new_size
))(mem_E$void) $scope) {
    claim_assert_nonnull(self);
    debug_assert_eqBy($typed(self->key_ty), key_ty, TypeInfo_eql);
    debug_assert_eqBy($typed(self->val_ty), val_ty, TypeInfo_eql);
    if (new_size > self->size) {
        try_(HashMap__growIfNeeded(self, key_ty, val_ty, gpa, new_size - self->size));
    }
    return_ok({});
} $unscoped(fn);

fn_((HashMap_ensureUnusedCap(
    HashMap* self, TypeInfo key_ty, TypeInfo val_ty, mem_Alctr gpa, u32 additional
))(mem_E$void)) {
    claim_assert_nonnull(self);
    debug_assert_eqBy($typed(self->key_ty), key_ty, TypeInfo_eql);
    debug_assert_eqBy($typed(self->val_ty), val_ty, TypeInfo_eql);
    return HashMap_ensureCap(self, key_ty, val_ty, gpa, self->size + additional);
};

fn_((HashMap_clearRetainingCap(HashMap* self))(void)) {
    claim_assert_nonnull(self);
    if_none(self->metadata) { return; }
    HashMap__initMetadata(self);
    self->size = 0;
    self->available = (HashMap_cap(*self) * HashMap_LoadRatio_max(self->ctx->load_ratio)) / 100;
};

fn_((HashMap_clearAndFree(
    HashMap* self, TypeInfo key_ty, TypeInfo val_ty, mem_Alctr gpa
))(void)) {
    claim_assert_nonnull(self);
    debug_assert_eqBy($typed(self->key_ty), key_ty, TypeInfo_eql);
    debug_assert_eqBy($typed(self->val_ty), val_ty, TypeInfo_eql);
    HashMap__free(self, key_ty, val_ty, gpa);
    self->size = 0;
    self->available = 0;
};

fn_((HashMap_by(HashMap self, u_V$raw key, u_V$raw ret_val))(O$u_V$raw) $scope) {
    debug_assert_eqBy($typed(self.key_ty), key.type, TypeInfo_eql);
    debug_assert_eqBy($typed(self.val_ty), ret_val.type, TypeInfo_eql);
    if_some((HashMap__idx(self, key))(idx)) {
        return_some({ .inner = u_memcpy(ret_val.ref, HashMap__valAt(self, ret_val.type, idx).as_const).raw });
    }
    return_none();
} $unscoped(fn);

fn_((HashMap_ptrBy(HashMap self, TypeInfo val_ty, u_V$raw key))(O$u_P_const$raw) $scope) {
    debug_assert_eqBy($typed(self.key_ty), key.type, TypeInfo_eql);
    debug_assert_eqBy($typed(self.val_ty), val_ty, TypeInfo_eql);
    if_some((HashMap__idx(self, key))(idx)) {
        return_some(HashMap__valAt(self, val_ty, idx).as_const);
    }
    return_none();
} $unscoped(fn);

fn_((HashMap_ptrMutBy(HashMap self, TypeInfo val_ty, u_V$raw key))(O$u_P$raw) $scope) {
    debug_assert_eqBy($typed(self.key_ty), key.type, TypeInfo_eql);
    debug_assert_eqBy($typed(self.val_ty), val_ty, TypeInfo_eql);
    if_some((HashMap__idx(self, key))(idx)) {
        return_some(HashMap__valAt(self, val_ty, idx));
    }
    return_none();
} $unscoped(fn);

fn_((HashMap_for(HashMap self, u_V$raw key, u_V$raw ret_key))(O$u_V$raw) $scope) {
    debug_assert_eqBy($typed(self.key_ty), key.type, TypeInfo_eql);
    if_some((HashMap__idx(self, key))(idx)) {
        return_some({ .inner = u_memcpy(ret_key.ref, HashMap__keyAt(self, ret_key.type, idx).as_const).raw });
    }
    return_none();
} $unscoped(fn);

fn_((HashMap_ptrFor(HashMap self, u_V$raw key))(O$u_P_const$raw) $scope) {
    debug_assert_eqBy($typed(self.key_ty), key.type, TypeInfo_eql);
    if_some((HashMap__idx(self, key))(idx)) {
        return_some(HashMap__keyAt(self, key.type, idx).as_const);
    }
    return_none();
} $unscoped(fn);

fn_((HashMap_ptrMutFor(HashMap self, u_V$raw key))(O$u_P$raw) $scope) {
    debug_assert_eqBy($typed(self.key_ty), key.type, TypeInfo_eql);
    if_some((HashMap__idx(self, key))(idx)) {
        return_some(HashMap__keyAt(self, key.type, idx));
    }
    return_none();
} $unscoped(fn);

fn_((HashMap_entry(HashMap self, TypeInfo val_ty, u_V$raw key))(O$HashMap_Entry) $scope) {
    debug_assert_eqBy($typed(self.key_ty), key.type, TypeInfo_eql);
    debug_assert_eqBy($typed(self.val_ty), val_ty, TypeInfo_eql);
    if_some((HashMap__idx(self, key))(idx)) {
        let k = HashMap__keyAt(self, key.type, idx).as_const;
        let v = HashMap__valAt(self, val_ty, idx).as_const;
        return_some({
            .key = k.raw,
            .key_ty = $typing(key.type),
            .val = v.raw,
            .val_ty = $typing(val_ty),
        });
    }
    return_none();
} $unscoped(fn);

fn_((HashMap_entryMut(HashMap self, TypeInfo val_ty, u_V$raw key))(O$HashMap_EntryMut) $scope) {
    debug_assert_eqBy($typed(self.key_ty), key.type, TypeInfo_eql);
    debug_assert_eqBy($typed(self.val_ty), val_ty, TypeInfo_eql);
    if_some((HashMap__idx(self, key))(idx)) {
        let k = HashMap__keyAt(self, key.type, idx);
        let v = HashMap__valAt(self, val_ty, idx);
        return_some({
            .key = k.raw,
            .key_ty = $typing(key.type),
            .val = v.raw,
            .val_ty = $typing(val_ty),
        });
    }
    return_none();
} $unscoped(fn);

fn_((HashMap_contains(HashMap self, u_V$raw key))(bool)) {
    debug_assert_eqBy($typed(self.key_ty), key.type, TypeInfo_eql);
    return isSome(HashMap__idx(self, key));
};

fn_((HashMap_put(HashMap* self, mem_Alctr gpa, u_V$raw key, u_V$raw val))(mem_E$void) $scope) {
    claim_assert_nonnull(self);
    debug_assert_eqBy($typed(self->key_ty), key.type, TypeInfo_eql);
    debug_assert_eqBy($typed(self->val_ty), val.type, TypeInfo_eql);
    let ensured = try_(HashMap_ensure(self, val.type, gpa, key));
    u_memcpy(HashMap_Ensured_valMut(ensured, val.type), val.ref.as_const);
    return_ok({});
} $unscoped(fn);

fn_((HashMap_putWithin(HashMap* self, u_V$raw key, u_V$raw val))(void)) {
    claim_assert_nonnull(self);
    debug_assert_eqBy($typed(self->key_ty), key.type, TypeInfo_eql);
    debug_assert_eqBy($typed(self->val_ty), val.type, TypeInfo_eql);
    let ensured = HashMap_ensureWithin(self, val.type, key);
    u_memcpy(HashMap_Ensured_valMut(ensured, val.type), val.ref.as_const);
};

fn_((HashMap_putNoClobber(HashMap* self, mem_Alctr gpa, u_V$raw key, u_V$raw val))(mem_E$void) $scope) {
    claim_assert_nonnull(self);
    debug_assert_eqBy($typed(self->key_ty), key.type, TypeInfo_eql);
    debug_assert_eqBy($typed(self->val_ty), val.type, TypeInfo_eql);
    try_(HashMap__growIfNeeded(self, key.type, val.type, gpa, 1));
    HashMap_putNoClobberWithin(self, key, val);
    return_ok({});
} $unscoped(fn);

fn_((HashMap_putNoClobberWithin(HashMap* self, u_V$raw key, u_V$raw val))(void)) {
    claim_assert_nonnull(self);
    debug_assert_eqBy($typed(self->key_ty), key.type, TypeInfo_eql);
    debug_assert_eqBy($typed(self->val_ty), val.type, TypeInfo_eql);
    claim_assert(!HashMap_contains(*self, key));
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
    HashMap* self, mem_Alctr gpa, u_V$raw key, u_V$raw val, V$HashMap_Pair$raw ret_mem
))(mem_E$O$V$HashMap_Pair$raw) $scope) {
    claim_assert_nonnull(self);
    debug_assert_eqBy($typed(self->key_ty), key.type, TypeInfo_eql);
    debug_assert_eqBy($typed(self->val_ty), val.type, TypeInfo_eql);
    let ensured = try_(HashMap_ensure(self, val.type, gpa, key));
    let result = expr_(O$V$HashMap_Pair$raw $scope)(if (ensured.found_existing) {
        let k = HashMap_Ensured_key(ensured, key.type);
        let v = HashMap_Ensured_val(ensured, val.type);
        $break_(some(HashMap_Pair__init(
            u_load(u_deref(k)),
            u_load(u_deref(v)),
            ret_mem
        )));
    } else_none {
        $break_(none());
    }) $unscoped(expr);
    u_memcpy(HashMap_Ensured_valMut(ensured, val.type), val.ref.as_const);
    return_ok(result);
} $unscoped(fn);

fn_((HashMap_fetchPutWithin(
    HashMap* self, u_V$raw key, u_V$raw val, V$HashMap_Pair$raw ret_mem
))(O$V$HashMap_Pair$raw)) {
    claim_assert_nonnull(self);
    debug_assert_eqBy($typed(self->key_ty), key.type, TypeInfo_eql);
    debug_assert_eqBy($typed(self->val_ty), val.type, TypeInfo_eql);
    let ensured = HashMap_ensureWithin(self, val.type, key);
    let result = expr_(O$V$HashMap_Pair$raw $scope)(if (ensured.found_existing) {
        let k = HashMap_Ensured_key(ensured, key.type);
        let v = HashMap_Ensured_val(ensured, val.type);
        $break_(some(HashMap_Pair__init(
            u_load(u_deref(k)),
            u_load(u_deref(v)),
            ret_mem
        )));
    } else_none {
        $break_(none());
    }) $unscoped(expr);
    u_memcpy(HashMap_Ensured_valMut(ensured, val.type), val.ref.as_const);
    return result;
};

fn_((HashMap_ensure(HashMap* self, TypeInfo val_ty, mem_Alctr gpa, u_V$raw key))(mem_E$HashMap_Ensured) $scope) {
    claim_assert_nonnull(self);
    debug_assert_eqBy($typed(self->key_ty), key.type, TypeInfo_eql);
    debug_assert_eqBy($typed(self->val_ty), val_ty, TypeInfo_eql);
    // Try to grow first, but if it fails and key exists, we can still return it
    catch_((HashMap__growIfNeeded(self, key.type, val_ty, gpa, 1))(err, if_some((HashMap__idx(*self, key))(idx)) {
        let k = HashMap__keyAt(*self, key.type, idx);
        let v = HashMap__valAt(*self, val_ty, idx);
        return_ok({
            .key = k.raw,
            .key_ty = $typing(key.type),
            .val = v.raw,
            .val_ty = $typing(val_ty),
            .found_existing = true,
        });
    } else_none {
        return_err(err);
    }));
    return_ok(HashMap_ensureWithin(self, val_ty, key));
} $unscoped(fn);

fn_((HashMap_ensureWithin(HashMap* self, TypeInfo val_ty, u_V$raw key))(HashMap_Ensured)) {
    claim_assert_nonnull(self);
    debug_assert_eqBy($typed(self->key_ty), key.type, TypeInfo_eql);
    debug_assert_eqBy($typed(self->val_ty), val_ty, TypeInfo_eql);
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
                    .key_ty = $typing(key.type),
                    .val = HashMap__valAt(*self, val_ty, idx).raw,
                    .val_ty = $typing(val_ty),
                    .found_existing = true,
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
        .key_ty = $typing(key.type),
        .val = HashMap__valAt(*self, val_ty, idx).raw,
        .val_ty = $typing(val_ty),
        .found_existing = false,
    };
};

fn_((HashMap_ensureValue(HashMap* self, mem_Alctr gpa, u_V$raw key, u_V$raw default_val))(mem_E$HashMap_Ensured) $scope) {
    claim_assert_nonnull(self);
    debug_assert_eqBy($typed(self->key_ty), key.type, TypeInfo_eql);
    debug_assert_eqBy($typed(self->val_ty), default_val.type, TypeInfo_eql);
    let ensured = try_(HashMap_ensure(self, default_val.type, gpa, key));
    if (!ensured.found_existing) { u_memcpy(HashMap_Ensured_valMut(ensured, default_val.type), default_val.ref.as_const); }
    return_ok(ensured);
} $unscoped(fn);

fn_((HashMap_remove(HashMap* self, u_V$raw key))(bool)) {
    claim_assert_nonnull(self);
    debug_assert_eqBy($typed(self->key_ty), key.type, TypeInfo_eql);
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
    debug_assert_eqBy($typed(self->key_ty), key.type, TypeInfo_eql);
    debug_assert_eqBy($typed(self->val_ty), val_ty, TypeInfo_eql);
    if_some((HashMap__idx(*self, key))(idx)) {
        let old_key = HashMap__keyAt(*self, key.type, idx);
        let old_val = HashMap__valAt(*self, val_ty, idx);
        let result = HashMap_Pair__init(
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
} $unscoped(fn);

fn_((HashMap_removeByPtr(HashMap* self, u_P$raw key_ptr))(void)) {
    claim_assert_nonnull(self);
    claim_assert_nonnull(key_ptr.raw);
    debug_assert_eqBy($typed(self->key_ty), key_ptr.type, TypeInfo_eql);

    let idx = expr_(u32 $scope)(if (key_ptr.type.size > 0) {
        $break_(intCast$((u32)((ptrToInt(key_ptr.raw) - ptrToInt(HashMap__keys(*self, key_ptr.type).raw))
                               / TypeInfo_size(key_ptr.type))));
    }) expr_(else)({
        $break_(0);
    }) $unscoped(expr);
    HashMap_Ctrl_remove(HashMap__metadataAt(*self, idx));
    self->size--;
    self->available++;
};

fn_((HashMap_rehash(HashMap* self, TypeInfo key_ty, TypeInfo val_ty))(void)) {
    claim_assert_nonnull(self);
    debug_assert_eqBy($typed(self->key_ty), key_ty, TypeInfo_eql);
    debug_assert_eqBy($typed(self->val_ty), val_ty, TypeInfo_eql);

    if_none(self->metadata) { return; }

    let cap = HashMap_cap(*self);
    let mask = cap - 1;
    let ctx = self->ctx;

    // Mark all slots as free but keep used flag
    for_(($r(0, cap))(i)) {
        HashMap__metadataAt(*self, i)->fingerprint = HashMap_Ctrl_free;
    } $end(for);

    // Rehash all used slots
    var_(curr, u32) = 0;
    while (curr < cap) {
        let curr_ctrl = HashMap__metadataAt(*self, curr);
        if (!HashMap_Ctrl_isUsed(*curr_ctrl)) {
            claim_assert(HashMap_Ctrl_isFree(*curr_ctrl));
            curr++;
            continue;
        }
        let key = HashMap__keyAt(*self, key_ty, curr);
        let hash = ctx->hashFn(u_load(u_deref(key)), u_load(u_deref(ctx->inner)));
        let fingerprint = HashMap_Ctrl_takeFingerprint(hash);
        var_(idx, usize) = hash & mask;

        // Find target slot
        while ((idx < curr && HashMap_Ctrl_isUsed(*HashMap__metadataAt(*self, idx)))
               || (idx > curr && HashMap__metadataAt(*self, idx)->fingerprint == HashMap_Ctrl_tombstone)) {
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

            curr_ctrl->used = 0;
            claim_assert(HashMap_Ctrl_isFree(*curr_ctrl));
            curr++;
        } else if (idx == curr) {
            curr_ctrl->fingerprint = fingerprint;
            curr++;
        } else {
            // Swap with later slot
            let target_ctrl = HashMap__metadataAt(*self, idx);
            claim_assert(target_ctrl->fingerprint != HashMap_Ctrl_tombstone);
            let was_used = HashMap_Ctrl_isUsed(*target_ctrl);
            target_ctrl->fingerprint = HashMap_Ctrl_tombstone;
            if (was_used) {
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
                curr_ctrl->fingerprint = HashMap_Ctrl_free;
                curr_ctrl->used = 0;
                u_memset0(HashMap__keyAt(*self, key_ty, curr));
                u_memset0(HashMap__valAt(*self, val_ty, curr));
                curr++;
            }
        }
    }
};

fn_((HashMap_iter(const HashMap* self, TypeInfo key_ty, TypeInfo val_ty))(HashMap_Iter)) {
    claim_assert_nonnull(self);
    debug_assert_eqBy($typed(self->key_ty), key_ty, TypeInfo_eql);
    debug_assert_eqBy($typed(self->val_ty), val_ty, TypeInfo_eql);
    return (HashMap_Iter){
        .map = self,
        .idx = 0,
        .key_ty = $typing(key_ty),
        .val_ty = $typing(val_ty),
    };
};

fn_((HashMap_Iter_next(HashMap_Iter* self, TypeInfo key_ty, TypeInfo val_ty))(O$HashMap_Entry) $scope) {
    claim_assert_nonnull(self);
    claim_assert_nonnull(self->map);
    debug_assert_eqBy($typed(self->key_ty), key_ty, TypeInfo_eql);
    debug_assert_eqBy($typed(self->val_ty), val_ty, TypeInfo_eql);

    if (self->map->size == 0) { return_none(); }
    let cap = HashMap_cap(*self->map);
    while (cap > self->idx) {
        let ctrl = *HashMap__metadataAt(*self->map, self->idx);
        if (HashMap_Ctrl_isUsed(ctrl)) {
            return_some(expr_(HashMap_Entry $guard)({
                defer_(self->idx++);
                $break_({
                    .key = HashMap__keyAt(*self->map, key_ty, self->idx).as_const.raw,
                    .val = HashMap__valAt(*self->map, val_ty, self->idx).as_const.raw,
                    .key_ty = $typing(key_ty),
                    .val_ty = $typing(val_ty),
                });
            }) $unguarded(expr));
        }
        self->idx++;
    }
    return_none();
} $unscoped(fn);

fn_((HashMap_Iter_nextMut(HashMap_Iter* self, TypeInfo key_ty, TypeInfo val_ty))(O$HashMap_EntryMut) $scope) {
    claim_assert_nonnull(self);
    claim_assert_nonnull(self->map);
    debug_assert_eqBy($typed(self->key_ty), key_ty, TypeInfo_eql);
    debug_assert_eqBy($typed(self->val_ty), val_ty, TypeInfo_eql);

    if (self->map->size == 0) { return_none(); }
    let cap = HashMap_cap(*self->map);
    while (cap > self->idx) {
        let ctrl = *HashMap__metadataAt(*self->map, self->idx);
        if (HashMap_Ctrl_isUsed(ctrl)) {
            return_some(expr_(HashMap_EntryMut $guard)({
                defer_(self->idx++);
                $break_({
                    .key = HashMap__keyAt(*self->map, key_ty, self->idx).raw,
                    .val = HashMap__valAt(*self->map, val_ty, self->idx).raw,
                    .key_ty = $typing(key_ty),
                    .val_ty = $typing(val_ty),
                });
            }) $unguarded(expr));
        }
        self->idx++;
    }
    return_none();
} $unscoped(fn);

fn_((HashMap_keyIter(HashMap self, TypeInfo key_ty))(HashMap_KeyIter) $scope) {
    debug_assert_eqBy($typed(self.key_ty), key_ty, TypeInfo_eql);
    return_(expr_(HashMap_KeyIter $scope)(if_some((self.metadata)(metadata)) {
        $break_({
            .len = HashMap_cap(self),
            .metadata = metadata,
            .keys = HashMap__keys(self, key_ty).raw,
            .key_ty = $typing(key_ty),
        });
    } else_none {
        $break_({
            .len = 0,
            .metadata = null,
            .keys = null,
            .key_ty = $typing(key_ty),
        });
    }) $unscoped(expr));
} $unscoped(fn);

fn_((HashMap_KeyIter_next(HashMap_KeyIter* self, TypeInfo key_ty))(O$u_P_const$raw) $scope) {
    claim_assert_nonnull(self);
    debug_assert_eqBy($typed(self->key_ty), key_ty, TypeInfo_eql);
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
} $unscoped(fn);

fn_((HashMap_KeyIter_nextMut(HashMap_KeyIter* self, TypeInfo key_ty))(O$u_P$raw) $scope) {
    claim_assert_nonnull(self);
    debug_assert_eqBy($typed(self->key_ty), key_ty, TypeInfo_eql);
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
} $unscoped(fn);

fn_((HashMap_valIter(HashMap self, TypeInfo val_ty))(HashMap_ValIter) $scope) {
    debug_assert_eqBy($typed(self.val_ty), val_ty, TypeInfo_eql);
    return_(expr_(HashMap_ValIter $scope)(if_some((self.metadata)(metadata)) {
        $break_({
            .len = HashMap_cap(self),
            .metadata = metadata,
            .vals = HashMap__vals(self, val_ty).raw,
            .val_ty = $typing(val_ty),
        });
    } else_none {
        $break_({
            .len = 0,
            .metadata = null,
            .vals = null,
            .val_ty = $typing(val_ty),
        });
    }) $unscoped(expr));
} $unscoped(fn);

fn_((HashMap_ValIter_next(HashMap_ValIter* self, TypeInfo val_ty))(O$u_P_const$raw) $scope) {
    claim_assert_nonnull(self);
    debug_assert_eqBy($typed(self->val_ty), val_ty, TypeInfo_eql);
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
} $unscoped(fn);

fn_((HashMap_ValIter_nextMut(HashMap_ValIter* self, TypeInfo val_ty))(O$u_P$raw) $scope) {
    claim_assert_nonnull(self);
    debug_assert_eqBy($typed(self->val_ty), val_ty, TypeInfo_eql);
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
} $unscoped(fn);

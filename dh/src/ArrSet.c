/**
 * @file ArrSet.c
 * @brief Array-based hash set implementation
 */

#include "dh/ArrSet.h"
#include "dh/mem/common.h"

// Small set threshold - use linear scan instead of hash index for small sets
#define SMALL_SET_THRESHOLD 8

// Hash index constants
#define INDEX_EMPTY usize_limit_max
#define INDEX_TOMBSTONE (usize_limit_max - 1)

// Hash index entry
typedef struct HashIndexEntry {
    usize hash;
    usize index; // Index into items array
} HashIndexEntry;

/*========== Helper Functions ===============================================*/

T_use_E$($set(mem_Err)(usize));
$static fn_((addOrOom(usize lhs, usize rhs))(mem_Err$usize) $scope) {
    if_some((usize_addChkd(lhs, rhs))(result)) {
        return_ok(result);
    } else_none {
        return_err(mem_Err_OutOfMemory());
    }
    claim_unreachable;
} $unscoped_(fn);

$inline_always
$static fn_((calcInitCap(TypeInfo type))(usize)) {
    return as$(usize)(prim_max(1, arch_cache_line / type.size));
}

$static fn_((growCap(TypeInfo type, usize current, usize minimum))(usize)) {
    let init_cap = calcInitCap(type);
    usize new = current;
    do { new = usize_addSat(new, new / 2 + init_cap); } while (new < minimum);
    return new;
}

// Calculate index capacity (must be power of 2 and >= 2 * items capacity)
$static fn_((calcIndexCap(usize items_cap))(usize)) {
    // Start with items_cap * 2 for good load factor
    usize cap = items_cap * 2;
    // Round up to next power of 2
    cap--;
    cap |= cap >> 1;
    cap |= cap >> 2;
    cap |= cap >> 4;
    cap |= cap >> 8;
    cap |= cap >> 16;
#if usize_limit_max > UINT32_MAX
    cap |= cap >> 32;
#endif
    cap++;
    return prim_max(cap, 16); // Minimum 16 slots
}

// Hash with context
$static fn_((hashItem(ArrSet self, u_V$raw item))(u64)) {
    return self.hashFn(item, u_anyV(self.ctx));
}

// Check equality with context
$static fn_((eqlItems(ArrSet self, u_V$raw lhs, u_V$raw rhs))(bool)) {
    return self.eqlFn(lhs, rhs, u_anyV(self.ctx));
}

// Find item using linear search (for small sets)
$static fn_((findLinear(ArrSet self, TypeInfo type, u_V$raw item))(O$usize) $scope) {
    debug_assert_eqBy(self.type, type, TypeInfo_eq);
    u_S$raw items = (u_S$raw){ .ptr = self.items.ptr, .len = self.items.len, .type = type };

    for (usize i = 0; i < items.len; i++) {
        u_V$raw current = u_deref(u_atS(items, i));
        if (eqlItems(self, item, current)) {
            return_some(i);
        }
    }
    return_none();
} $unscoped_(fn);

// Find item using hash index
$static fn_((findHashed(ArrSet self, TypeInfo type, u_V$raw item, usize hash))(O$usize) $scope) {
    debug_assert_eqBy(self.type, type, TypeInfo_eq);
    claim_assert_nonnull(self.index);

    HashIndexEntry* index = (HashIndexEntry*)self.index;
    usize index_cap = calcIndexCap(self.cap);
    usize mask = index_cap - 1;
    usize pos = hash & mask;

    u_S$raw items = (u_S$raw){ .ptr = self.items.ptr, .len = self.items.len, .type = type };

    // Linear probing
    for (usize i = 0; i < index_cap; i++) {
        usize slot = (pos + i) & mask;

        if (index[slot].index == INDEX_EMPTY) {
            return_none();
        }

        if (index[slot].index != INDEX_TOMBSTONE && index[slot].hash == hash) {
            usize item_idx = index[slot].index;
            if (item_idx < items.len) {
                u_V$raw current = u_deref(u_atS(items, item_idx));
                if (eqlItems(self, item, current)) {
                    return_some(item_idx);
                }
            }
        }
    }

    return_none();
} $unscoped_(fn);

// Build hash index from items
$static fn_((buildIndex(ArrSet* self, TypeInfo type, mem_Allocator gpa))(mem_Err$void) $scope) {
    claim_assert_nonnull(self);
    debug_assert_eqBy(self->type, type, TypeInfo_eq);

    if (self->items.len < SMALL_SET_THRESHOLD) {
        // No index needed for small sets
        if (self->index != NULL) {
            mem_free(gpa, self->index);
            self->index = NULL;
        }
        return_ok({});
    }

    usize index_cap = calcIndexCap(self->cap);
    usize index_size = index_cap * sizeOf(HashIndexEntry);

    // Allocate or reallocate index
    HashIndexEntry* new_index = (HashIndexEntry*)mem_alloc(gpa, index_size, alignOf(HashIndexEntry));
    if (new_index == NULL) {
        return_err(mem_Err_OutOfMemory());
    }

    // Initialize all slots to empty
    for (usize i = 0; i < index_cap; i++) {
        new_index[i].index = INDEX_EMPTY;
    }

    // Free old index
    if (self->index != NULL) {
        mem_free(gpa, self->index);
    }

    self->index = new_index;

    // Insert all items into index
    u_S$raw items = (u_S$raw){ .ptr = self->items.ptr, .len = self->items.len, .type = type };
    usize mask = index_cap - 1;

    for (usize item_idx = 0; item_idx < items.len; item_idx++) {
        u_V$raw item = u_deref(u_atS(items, item_idx));
        u64 hash = hashItem(*self, item);
        usize pos = hash & mask;

        // Find empty slot using linear probing
        for (usize i = 0; i < index_cap; i++) {
            usize slot = (pos + i) & mask;
            if (new_index[slot].index == INDEX_EMPTY) {
                new_index[slot].hash = hash;
                new_index[slot].index = item_idx;
                break;
            }
        }
    }

    return_ok({});
} $unscoped_(fn);

/*========== Public API =====================================================*/

fn_((ArrSet_empty(TypeInfo type, u_HashCtxFn hashFn, u_EqlCtxFn eqlFn, u_P_const$raw ctx))(ArrSet)) {
    let_ignore = type;
    return (ArrSet){
        .items = zero$S(),
        .cap = 0,
        .index = NULL,
        .hashFn = hashFn,
        .eqlFn = eqlFn,
        .ctx = ctx,
        debug_only(.type = type)
    };
}

fn_((ArrSet_fromBuf(u_S$raw buf, u_HashCtxFn hashFn, u_EqlCtxFn eqlFn, u_P_const$raw ctx))(ArrSet)) {
    return (ArrSet){
        .items = u_sliceS(buf, $r(0, 0)).raw,
        .cap = buf.len,
        .index = NULL,
        .hashFn = hashFn,
        .eqlFn = eqlFn,
        .ctx = ctx,
        debug_only(.type = buf.type)
    };
}

fn_((ArrSet_init(TypeInfo type, mem_Allocator gpa, usize cap, u_HashCtxFn hashFn, u_EqlCtxFn eqlFn, u_P_const$raw ctx))(mem_Err$ArrSet) $scope) {
    var set = ArrSet_empty(type, hashFn, eqlFn, ctx);
    try_(ArrSet_ensureCapPrecise(&set, type, gpa, cap));
    return_ok(set);
} $unscoped_(fn);

fn_((ArrSet_fini(ArrSet* self, TypeInfo type, mem_Allocator gpa))(void)) {
    claim_assert_nonnull(self);
    debug_assert_eqBy(self->type, type, TypeInfo_eq);

    // Free items array
    u_S$raw items_capped = (u_S$raw){ .ptr = self->items.ptr, .len = self->cap, .type = type };
    mem_Allocator_free(gpa, items_capped);

    // Free hash index
    if (self->index != NULL) {
        mem_free(gpa, self->index);
    }

    *self = ArrSet_empty(type, self->hashFn, self->eqlFn, self->ctx);
}

fn_((ArrSet_clone(ArrSet self, TypeInfo type, mem_Allocator gpa))(mem_Err$ArrSet) $scope) {
    debug_assert_eqBy(self.type, type, TypeInfo_eq);
    var cloned = try_(ArrSet_init(type, gpa, self.cap, self.hashFn, self.eqlFn, self.ctx));

    // Copy items
    u_S$raw src_items = (u_S$raw){ .ptr = self.items.ptr, .len = self.items.len, .type = type };
    u_S$raw dst_items = (u_S$raw){ .ptr = cloned.items.ptr, .len = 0, .type = type };
    mem_copy(dst_items.ptr, src_items.ptr, src_items.len * type.size);
    cloned.items.len = src_items.len;

    // Rebuild index
    try_(buildIndex(&cloned, type, gpa));

    return_ok(cloned);
} $unscoped_(fn);

fn_((ArrSet_move(ArrSet* self))(ArrSet)) {
    claim_assert_nonnull(self);
    ArrSet result = *self;
    *self = ArrSet_empty(self->type, self->hashFn, self->eqlFn, self->ctx);
    return result;
}

fn_((ArrSet_len(ArrSet self))(usize)) {
    return self.items.len;
}

fn_((ArrSet_cap(ArrSet self))(usize)) {
    return self.cap;
}

fn_((ArrSet_contains(ArrSet self, u_V$raw item))(bool) $scope) {
    if_some((ArrSet_getIndex(self, item))(idx)) {
        let_ignore = idx;
        return true;
    } else_none {
        return false;
    }
} $unscoped_(fn);

fn_((ArrSet_get(ArrSet self, u_V$raw item))(O$u_P_const$raw) $scope) {
    if_some((ArrSet_getIndex(self, item))(idx)) {
        u_S$raw items = (u_S$raw){ .ptr = self.items.ptr, .len = self.items.len, .type = self.type };
        return_some(u_atS(items, idx));
    } else_none {
        return_none();
    }
} $unscoped_(fn);

fn_((ArrSet_getMut(ArrSet self, u_V$raw item))(O$u_P$raw) $scope) {
    if_some((ArrSet_getIndex(self, item))(idx)) {
        u_S$raw items = (u_S$raw){ .ptr = self.items.ptr, .len = self.items.len, .type = self.type };
        return_some(u_atS(items, idx));
    } else_none {
        return_none();
    }
} $unscoped_(fn);

fn_((ArrSet_getIndex(ArrSet self, u_V$raw item))(O$usize) $scope) {
    if (self.items.len == 0) {
        return_none();
    }

    if (self.items.len < SMALL_SET_THRESHOLD || self.index == NULL) {
        return findLinear(self, self.type, item);
    } else {
        u64 hash = hashItem(self, item);
        return findHashed(self, self.type, item, hash);
    }
} $unscoped_(fn);

fn_((ArrSet_at(ArrSet self, TypeInfo type, usize idx))(u_P_const$raw)) {
    debug_assert_eqBy(self.type, type, TypeInfo_eq);
    u_S$raw items = (u_S$raw){ .ptr = self.items.ptr, .len = self.items.len, .type = type };
    return u_atS(items, idx);
}

fn_((ArrSet_atMut(ArrSet self, TypeInfo type, usize idx))(u_P$raw)) {
    debug_assert_eqBy(self.type, type, TypeInfo_eq);
    u_S$raw items = (u_S$raw){ .ptr = self.items.ptr, .len = self.items.len, .type = type };
    return u_atS(items, idx);
}

fn_((ArrSet_items(ArrSet self, TypeInfo type))(u_S_const$raw)) {
    debug_assert_eqBy(self.type, type, TypeInfo_eq);
    return u_from$S((const type)(self.items.as_const));
}

fn_((ArrSet_itemsMut(ArrSet self, TypeInfo type))(u_S$raw)) {
    debug_assert_eqBy(self.type, type, TypeInfo_eq);
    return u_from$S((type)(self.items));
}

fn_((ArrSet_itemsCapped(ArrSet self, TypeInfo type))(u_S_const$raw)) {
    debug_assert_eqBy(self.type, type, TypeInfo_eq);
    return u_init$S((const type)(ArrSet_items(self, type).ptr, self.cap));
}

fn_((ArrSet_itemsCappedMut(ArrSet self, TypeInfo type))(u_S$raw)) {
    debug_assert_eqBy(self.type, type, TypeInfo_eq);
    return u_init$S((type)(ArrSet_itemsMut(self, type).ptr, self.cap));
}

fn_((ArrSet_itemsUnused(ArrSet self, TypeInfo type))(u_S_const$raw)) {
    debug_assert_eqBy(self.type, type, TypeInfo_eq);
    return u_sliceS(ArrSet_itemsCapped(self, type), $r(self.items.len, self.cap - self.items.len));
}

fn_((ArrSet_itemsUnusedMut(ArrSet self, TypeInfo type))(u_S$raw)) {
    debug_assert_eqBy(self.type, type, TypeInfo_eq);
    return u_sliceS(ArrSet_itemsCappedMut(self, type), $r(self.items.len, self.cap - self.items.len));
}

fn_((ArrSet_ensureCap(ArrSet* self, TypeInfo type, mem_Allocator gpa, usize new_cap))(mem_Err$void) $scope) {
    claim_assert_nonnull(self);
    debug_assert_eqBy(self->type, type, TypeInfo_eq);
    if (new_cap <= self->cap) { return_ok({}); }
    return ArrSet_ensureCapPrecise(self, type, gpa, growCap(type, self->cap, new_cap));
} $unscoped_(fn);

fn_((ArrSet_ensureCapPrecise(ArrSet* self, TypeInfo type, mem_Allocator gpa, usize new_cap))(mem_Err$void) $scope) {
    claim_assert_nonnull(self);
    debug_assert_eqBy(self->type, type, TypeInfo_eq);
    if (type.size == 0) {
        self->cap = usize_limit_max;
        return_ok({});
    }
    if (new_cap <= self->cap) { return_ok({}); }

    u_S$raw old_items = (u_S$raw){ .ptr = self->items.ptr, .len = self->cap, .type = type };
    if_some((mem_Allocator_remap(gpa, old_items, new_cap))(new_items)) {
        self->items.ptr = new_items.ptr;
        self->cap = new_items.len;
    } else_none {
        return_err(mem_Err_OutOfMemory());
    }

    // Rebuild index if needed
    if (self->items.len >= SMALL_SET_THRESHOLD) {
        try_(buildIndex(self, type, gpa));
    }

    return_ok({});
} $unscoped_(fn);

fn_((ArrSet_ensureUnusedCap(ArrSet* self, TypeInfo type, mem_Allocator gpa, usize additional))(mem_Err$void) $scope) {
    claim_assert_nonnull(self);
    debug_assert_eqBy(self->type, type, TypeInfo_eq);
    let new_len = try_(addOrOom(self->items.len, additional));
    return ArrSet_ensureCap(self, type, gpa, new_len);
} $unscoped_(fn);

fn_((ArrSet_shrinkRetainingCap(ArrSet* self, usize new_len))(void)) {
    claim_assert_nonnull(self);
    claim_assert(new_len <= self->items.len);
    self->items.len = new_len;
}

fn_((ArrSet_shrinkAndFree(ArrSet* self, TypeInfo type, mem_Allocator gpa, usize new_len))(void)) {
    claim_assert_nonnull(self);
    debug_assert_eqBy(self->type, type, TypeInfo_eq);
    claim_assert(new_len <= self->items.len);

    self->items.len = new_len;

    if (new_len == 0) {
        ArrSet_clearAndFree(self, type, gpa);
        return;
    }

    // Shrink capacity if significantly over-allocated
    if (self->cap > new_len * 2) {
        u_S$raw old_items = (u_S$raw){ .ptr = self->items.ptr, .len = self->cap, .type = type };
        if_some((mem_Allocator_remap(gpa, old_items, new_len))(new_items)) {
            self->items.ptr = new_items.ptr;
            self->cap = new_items.len;
        } else_none {
            // Remap failed, keep existing capacity
        }
    }

    // Rebuild index if needed
    if (self->items.len >= SMALL_SET_THRESHOLD) {
        buildIndex(self, type, gpa); // Ignore errors here
    } else if (self->index != NULL) {
        mem_free(gpa, self->index);
        self->index = NULL;
    }
}

fn_((ArrSet_clearRetainingCap(ArrSet* self))(void)) {
    claim_assert_nonnull(self);
    self->items.len = 0;
}

fn_((ArrSet_clearAndFree(ArrSet* self, TypeInfo type, mem_Allocator gpa))(void)) {
    claim_assert_nonnull(self);
    debug_assert_eqBy(self->type, type, TypeInfo_eq);

    u_S$raw items_capped = (u_S$raw){ .ptr = self->items.ptr, .len = self->cap, .type = type };
    mem_Allocator_free(gpa, items_capped);

    if (self->index != NULL) {
        mem_free(gpa, self->index);
    }

    self->items.ptr = NULL;
    self->items.len = 0;
    self->cap = 0;
    self->index = NULL;
}

fn_((ArrSet_insert(ArrSet* self, mem_Allocator gpa, u_V$raw item))(mem_Err$bool) $scope) {
    claim_assert_nonnull(self);

    // Check if item already exists
    if (ArrSet_contains(*self, item)) {
        return_ok(false);
    }

    // Ensure capacity
    try_(ArrSet_ensureUnusedCap(self, self->type, gpa, 1));

    // Insert item
    ArrSet_insertWithin(self, item);
    return_ok(true);
} $unscoped_(fn);

fn_((ArrSet_insertFixed(ArrSet* self, u_V$raw item))(mem_Err$bool) $scope) {
    claim_assert_nonnull(self);

    // Check if item already exists
    if (ArrSet_contains(*self, item)) {
        return_ok(false);
    }

    // Check capacity
    if (self->items.len >= self->cap) {
        return_err(mem_Err_OutOfMemory());
    }

    // Insert item
    ArrSet_insertWithin(self, item);
    return_ok(true);
} $unscoped_(fn);

fn_((ArrSet_insertWithin(ArrSet* self, u_V$raw item))(bool)) {
    claim_assert_nonnull(self);
    claim_assert(self->items.len < self->cap);

    // Check if item already exists
    if (ArrSet_contains(*self, item)) {
        return false;
    }

    // Add item to end of array
    u_S$raw items = (u_S$raw){ .ptr = self->items.ptr, .len = self->items.len, .type = self->type };
    u_P$raw dest = u_atS(items, items.len);
    mem_copy(dest, item.ref.as_const, self->type.size);
    self->items.len++;

    // Update index if we crossed the threshold
    if (self->items.len == SMALL_SET_THRESHOLD && self->index == NULL) {
        // We need to build an index, but we can't allocate in Within variant
        // So we'll defer index building until next allocation operation
    }
    // TODO: Update index incrementally if it exists

    return true;
}

fn_((ArrSet_getOrPut(ArrSet* self, mem_Allocator gpa, u_V$raw item))(mem_Err$ArrSet_GetOrPutResult) $scope) {
    claim_assert_nonnull(self);

    // Check if item exists
    if_some((ArrSet_getIndex(*self, item))(idx)) {
        u_S$raw items = (u_S$raw){ .ptr = self->items.ptr, .len = self->items.len, .type = self->type };
        return_ok((ArrSet_GetOrPutResult){
            .item_ptr = u_atS(items, idx),
            .found_existing = true,
            .index = idx });
    } else_none {
        // Ensure capacity
        try_(ArrSet_ensureUnusedCap(self, self->type, gpa, 1));

        // Add new item
        u_S$raw items = (u_S$raw){ .ptr = self->items.ptr, .len = self->items.len, .type = self->type };
        usize idx = items.len;
        u_P$raw dest = u_atS(items, idx);
        mem_copy(dest, item.ref.as_const, self->type.size);
        self->items.len++;

        // Rebuild index if needed
        if (self->items.len >= SMALL_SET_THRESHOLD) {
            try_(buildIndex(self, self->type, gpa));
        }

        return_ok((ArrSet_GetOrPutResult){
            .item_ptr = dest,
            .found_existing = false,
            .index = idx });
    }
} $unscoped_(fn);

fn_((ArrSet_getOrPutFixed(ArrSet* self, u_V$raw item))(mem_Err$ArrSet_GetOrPutResult) $scope) {
    claim_assert_nonnull(self);

    // Check if item exists
    if_some((ArrSet_getIndex(*self, item))(idx)) {
        u_S$raw items = (u_S$raw){ .ptr = self->items.ptr, .len = self->items.len, .type = self->type };
        return_ok((ArrSet_GetOrPutResult){
            .item_ptr = u_atS(items, idx),
            .found_existing = true,
            .index = idx });
    } else_none {
        // Check capacity
        if (self->items.len >= self->cap) {
            return_err(mem_Err_OutOfMemory());
        }

        // Add new item
        u_S$raw items = (u_S$raw){ .ptr = self->items.ptr, .len = self->items.len, .type = self->type };
        usize idx = items.len;
        u_P$raw dest = u_atS(items, idx);
        mem_copy(dest, item.ref.as_const, self->type.size);
        self->items.len++;

        return_ok((ArrSet_GetOrPutResult){
            .item_ptr = dest,
            .found_existing = false,
            .index = idx });
    }
} $unscoped_(fn);

fn_((ArrSet_getOrPutWithin(ArrSet* self, u_V$raw item))(ArrSet_GetOrPutResult) $scope) {
    claim_assert_nonnull(self);

    // Check if item exists
    if_some((ArrSet_getIndex(*self, item))(idx)) {
        u_S$raw items = (u_S$raw){ .ptr = self->items.ptr, .len = self->items.len, .type = self->type };
        return (ArrSet_GetOrPutResult){
            .item_ptr = u_atS(items, idx),
            .found_existing = true,
            .index = idx
        };
    } else_none {
        claim_assert(self->items.len < self->cap);

        // Add new item
        u_S$raw items = (u_S$raw){ .ptr = self->items.ptr, .len = self->items.len, .type = self->type };
        usize idx = items.len;
        u_P$raw dest = u_atS(items, idx);
        mem_copy(dest, item.ref.as_const, self->type.size);
        self->items.len++;

        return (ArrSet_GetOrPutResult){
            .item_ptr = dest,
            .found_existing = false,
            .index = idx
        };
    }
} $unscoped_(fn);

fn_((ArrSet_remove(ArrSet* self, u_V$raw item))(bool) $scope) {
    claim_assert_nonnull(self);

    if_some((ArrSet_getIndex(*self, item))(idx)) {
        // Remove by swapping with last element
        u_S$raw items = (u_S$raw){ .ptr = self->items.ptr, .len = self->items.len, .type = self->type };
        if (idx < items.len - 1) {
            u_P$raw dest = u_atS(items, idx);
            u_P$raw src = u_atS(items, items.len - 1);
            mem_copy(dest, src, self->type.size);
        }
        self->items.len--;

        // Rebuild index if needed
        if (self->items.len >= SMALL_SET_THRESHOLD && self->index != NULL) {
            buildIndex(self, self->type, mem_Allocator_noop()); // Using noop allocator - should we pass gpa?
        } else if (self->items.len < SMALL_SET_THRESHOLD && self->index != NULL) {
            // Free index for small sets
            // Can't free without allocator - this is a design issue
        }

        return true;
    } else_none {
        return false;
    }
} $unscoped_(fn);

fn_((ArrSet_removeOrd(ArrSet* self, usize idx, u_V$raw ret_mem))(u_V$raw)) {
    claim_assert_nonnull(self);
    claim_assert(idx < self->items.len);

    u_S$raw items = (u_S$raw){ .ptr = self->items.ptr, .len = self->items.len, .type = self->type };
    u_P$raw item_ptr = u_atS(items, idx);

    // Copy item to return memory
    mem_copy(ret_mem.ref, item_ptr, self->type.size);

    // Shift items down
    if (idx < items.len - 1) {
        u_P$raw dest = u_atS(items, idx);
        u_P$raw src = u_atS(items, idx + 1);
        usize count = items.len - idx - 1;
        mem_move(dest, src, count * self->type.size);
    }

    self->items.len--;

    // TODO: Update or rebuild index

    return ret_mem;
}

fn_((ArrSet_removeSwap(ArrSet* self, usize idx, u_V$raw ret_mem))(u_V$raw)) {
    claim_assert_nonnull(self);
    claim_assert(idx < self->items.len);

    u_S$raw items = (u_S$raw){ .ptr = self->items.ptr, .len = self->items.len, .type = self->type };
    u_P$raw item_ptr = u_atS(items, idx);

    // Copy item to return memory
    mem_copy(ret_mem.ref, item_ptr, self->type.size);

    // Swap with last element
    if (idx < items.len - 1) {
        u_P$raw src = u_atS(items, items.len - 1);
        mem_copy(item_ptr, src, self->type.size);
    }

    self->items.len--;

    // TODO: Update or rebuild index

    return ret_mem;
}

fn_((ArrSet_fetch(ArrSet* self, u_V$raw item, u_V$raw ret_mem))(O$u_V$raw) $scope) {
    return ArrSet_fetchSwap(self, item, ret_mem);
} $unscoped_(fn);

fn_((ArrSet_fetchOrd(ArrSet* self, u_V$raw item, u_V$raw ret_mem))(O$u_V$raw) $scope) {
    claim_assert_nonnull(self);

    if_some((ArrSet_getIndex(*self, item))(idx)) {
        return_some(ArrSet_removeOrd(self, idx, ret_mem));
    } else_none {
        return_none();
    }
} $unscoped_(fn);

fn_((ArrSet_fetchSwap(ArrSet* self, u_V$raw item, u_V$raw ret_mem))(O$u_V$raw) $scope) {
    claim_assert_nonnull(self);

    if_some((ArrSet_getIndex(*self, item))(idx)) {
        return_some(ArrSet_removeSwap(self, idx, ret_mem));
    } else_none {
        return_none();
    }
} $unscoped_(fn);

fn_((ArrSet_unionWith(ArrSet* self, mem_Allocator gpa, ArrSet other))(mem_Err$void) $scope) {
    claim_assert_nonnull(self);
    debug_assert_eqBy(self->type, other.type, TypeInfo_eq);

    u_S$raw other_items = (u_S$raw){ .ptr = other.items.ptr, .len = other.items.len, .type = other.type };

    for (usize i = 0; i < other_items.len; i++) {
        u_V$raw item = u_deref(u_atS(other_items, i));
        try_(ArrSet_insert(self, gpa, item));
    }

    return_ok({});
} $unscoped_(fn);

fn_((ArrSet_intersectWith(ArrSet* self, ArrSet other))(void)) {
    claim_assert_nonnull(self);
    debug_assert_eqBy(self->type, other.type, TypeInfo_eq);

    // Remove items that are not in other
    usize write_idx = 0;
    u_S$raw items = (u_S$raw){ .ptr = self->items.ptr, .len = self->items.len, .type = self->type };

    for (usize read_idx = 0; read_idx < items.len; read_idx++) {
        u_V$raw item = u_deref(u_atS(items, read_idx));
        if (ArrSet_contains(other, item)) {
            if (write_idx != read_idx) {
                u_P$raw dest = u_atS(items, write_idx);
                u_P$raw src = u_atS(items, read_idx);
                mem_copy(dest, src, self->type.size);
            }
            write_idx++;
        }
    }

    self->items.len = write_idx;

    // TODO: Rebuild index
}

fn_((ArrSet_differenceWith(ArrSet* self, ArrSet other))(void)) {
    claim_assert_nonnull(self);
    debug_assert_eqBy(self->type, other.type, TypeInfo_eq);

    // Remove items that are in other
    usize write_idx = 0;
    u_S$raw items = (u_S$raw){ .ptr = self->items.ptr, .len = self->items.len, .type = self->type };

    for (usize read_idx = 0; read_idx < items.len; read_idx++) {
        u_V$raw item = u_deref(u_atS(items, read_idx));
        if (!ArrSet_contains(other, item)) {
            if (write_idx != read_idx) {
                u_P$raw dest = u_atS(items, write_idx);
                u_P$raw src = u_atS(items, read_idx);
                mem_copy(dest, src, self->type.size);
            }
            write_idx++;
        }
    }

    self->items.len = write_idx;

    // TODO: Rebuild index
}

fn_((ArrSet_reIndex(ArrSet* self, TypeInfo type, mem_Allocator gpa))(mem_Err$void) $scope) {
    return buildIndex(self, type, gpa);
} $unscoped_(fn);

fn_((ArrSet_sort(ArrSet* self, TypeInfo type, u_OrdCtxFn ordFn, u_P_const$raw ord_ctx))(void)) {
    claim_assert_nonnull(self);
    debug_assert_eqBy(self->type, type, TypeInfo_eq);

    // TODO: Implement sorting
    // We would need to implement a sorting algorithm or use qsort
    // For now, this is a placeholder
    let_ignore = ordFn;
    let_ignore = ord_ctx;
}

/* Iterator implementation */

fn_((ArrSet_iter(const ArrSet* self, TypeInfo type))(ArrSet_Iter)) {
    claim_assert_nonnull(self);
    debug_assert_eqBy(self->type, type, TypeInfo_eq);
    return (ArrSet_Iter){
        .set = self,
        .index = 0,
        debug_only(.type = type)
    };
}

fn_((ArrSet_Iter_next(ArrSet_Iter* self, TypeInfo type))(O$u_P_const$raw) $scope) {
    claim_assert_nonnull(self);
    claim_assert_nonnull(self->set);
    debug_assert_eqBy(self->type, type, TypeInfo_eq);

    if (self->index >= self->set->items.len) {
        return_none();
    }

    u_S$raw items = (u_S$raw){ .ptr = self->set->items.ptr, .len = self->set->items.len, .type = type };
    u_P$raw item_ptr = u_atS(items, self->index);
    self->index++;

    return_some(item_ptr);
} $unscoped_(fn);

fn_((ArrSet_Iter_nextMut(ArrSet_Iter* self, TypeInfo type))(O$u_P$raw) $scope) {
    claim_assert_nonnull(self);
    claim_assert_nonnull(self->set);
    debug_assert_eqBy(self->type, type, TypeInfo_eq);

    if (self->index >= self->set->items.len) {
        return_none();
    }

    u_S$raw items = (u_S$raw){ .ptr = self->set->items.ptr, .len = self->set->items.len, .type = type };
    u_P$raw item_ptr = u_atS(items, self->index);
    self->index++;

    return_some(item_ptr);
} $unscoped_(fn);

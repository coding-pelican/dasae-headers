#include "dh/ArrPQue.h"

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

// ============================================================================
// Heap Operations (Binary Min-Heap)
// ============================================================================

$inline_always
$static fn_((parentIdx(usize idx))(usize)) {
    return (idx - 1) >> 1;
}

$inline_always
$static fn_((leftChildIdx(usize idx))(usize)) {
    return (idx << 1) + 1;
}

$inline_always
$static fn_((rightChildIdx(usize idx))(usize)) {
    return (idx << 1) + 2;
}

// Sift up: restore heap property after insertion at idx
$static fn_((siftUp(ArrPQue* self, TypeInfo type, usize start_idx))(void)) {
    var child_idx = start_idx;
    let child = u_deref(u_memcpy(u_allocV(type).ref, u_atS(ArrPQue_itemsMut(*self, type), child_idx).as_const));
    while (child_idx > 0) {
        let parent_idx = parentIdx(child_idx);
        let parent = u_atS(ArrPQue_itemsMut(*self, type), parent_idx);
        // If child >= parent (min-heap), we're done
        if (!u_ltCtx(child, u_load(u_deref(parent)), self->ordFn, u_load(u_deref(self->ord_ctx)))) { break; }
        // Move parent down
        u_memcpy(u_atS(ArrPQue_itemsMut(*self, type), child_idx), parent.as_const);
        child_idx = parent_idx;
    }
    // Place child in final position
    u_memcpy(u_atS(ArrPQue_itemsMut(*self, type), child_idx), child.ref.as_const);
}

// Sift down: restore heap property after removal at idx
$static fn_((siftDown(ArrPQue* self, TypeInfo type, usize start_idx))(void)) {
    let half = self->items.len >> 1;
    var idx = start_idx;
    let elem = u_deref(u_memcpy(u_allocV(type).ref, u_atS(ArrPQue_itemsMut(*self, type), idx).as_const));
    while (idx < half) {
        let left_idx = leftChildIdx(idx);
        let right_idx = rightChildIdx(idx);
        var min_child_idx = left_idx;
        // Find the smaller child
        if (right_idx < self->items.len) {
            let left = u_atS(ArrPQue_items(*self, type), left_idx);
            let right = u_atS(ArrPQue_items(*self, type), right_idx);
            // If right < left, then right is the smaller child
            if (u_ltCtx(u_load(u_deref(right)), u_load(u_deref(left)), self->ordFn, u_load(u_deref(self->ord_ctx)))) {
                min_child_idx = right_idx;
            }
        }
        let min_child = u_atS(ArrPQue_items(*self, type), min_child_idx);
        // If elem <= min_child, we're done
        if (!u_gtCtx(elem, u_load(u_deref(min_child)), self->ordFn, u_load(u_deref(self->ord_ctx)))) {
            break;
        }
        // Move min_child up
        u_memcpy(u_atS(ArrPQue_itemsMut(*self, type), idx), min_child);
        idx = min_child_idx;
    }
    // Place elem in final position
    u_memcpy(u_atS(ArrPQue_itemsMut(*self, type), idx), elem.ref.as_const);
}

// Heapify: build heap from unordered array
$static fn_((heapify(ArrPQue* self, TypeInfo type))(void)) {
    if (self->items.len <= 1) { return; }
    // Start from last non-leaf node and sift down
    var i = (self->items.len >> 1);
    while (i > 0) {
        siftDown(self, type, --i);
    }
}

// ============================================================================
// Core Functions
// ============================================================================

fn_((ArrPQue_empty(TypeInfo type, u_OrdCtxFn ordFn, u_P_const$raw ord_ctx))(ArrPQue)) {
    let_ignore = type;
    return (ArrPQue){
        .items = zero$S(),
        .cap = 0,
        .ordFn = ordFn,
        .ord_ctx = ord_ctx,
        debug_only(.type = type)
    };
}

fn_((ArrPQue_fromBuf(u_S$raw buf, u_OrdCtxFn ordFn, u_P_const$raw ord_ctx))(ArrPQue)) {
    return (ArrPQue){
        .items = u_sliceS(buf, $r(0, 0)).raw,
        .cap = buf.len,
        .ordFn = ordFn,
        .ord_ctx = ord_ctx,
        debug_only(.type = buf.type)
    };
}

fn_((ArrPQue_init(TypeInfo type, mem_Allocator gpa, usize cap, u_OrdCtxFn ordFn, u_P_const$raw ord_ctx))(mem_Err$ArrPQue) $scope) {
    var pq = ArrPQue_empty(type, ordFn, ord_ctx);
    try_(ArrPQue_ensureCapPrecise(&pq, type, gpa, cap));
    return_ok(pq);
} $unscoped_(fn);

fn_((ArrPQue_fini(ArrPQue* self, TypeInfo type, mem_Allocator gpa))(void)) {
    claim_assert_nonnull(self);
    debug_assert_eqBy(self->type, type, TypeInfo_eq);
    mem_Allocator_free(gpa, ArrPQue_itemsCappedMut(*self, type));
    *self = ArrPQue_empty(type, self->ordFn, self->ord_ctx);
}

fn_((ArrPQue_len(ArrPQue self))(usize)) {
    return self.items.len;
}

fn_((ArrPQue_cap(ArrPQue self))(usize)) {
    return self.cap;
}

fn_((ArrPQue_peek(ArrPQue self, TypeInfo type))(O$u_P_const$raw) $scope) {
    debug_assert_eqBy(self.type, type, TypeInfo_eq);
    if (self.items.len == 0) { return_none(); }
    return_some(u_atS(ArrPQue_items(self, type), 0));
} $unscoped_(fn);

fn_((ArrPQue_peekMut(ArrPQue self, TypeInfo type))(O$u_P$raw) $scope) {
    debug_assert_eqBy(self.type, type, TypeInfo_eq);
    if (self.items.len == 0) { return_none(); }
    return_some(u_atS(ArrPQue_itemsMut(self, type), 0));
} $unscoped_(fn);

fn_((ArrPQue_at(ArrPQue self, TypeInfo type, usize idx))(u_P_const$raw)) {
    debug_assert_eqBy(self.type, type, TypeInfo_eq);
    return u_atS(ArrPQue_items(self, type), idx);
}

fn_((ArrPQue_atMut(ArrPQue self, TypeInfo type, usize idx))(u_P$raw)) {
    debug_assert_eqBy(self.type, type, TypeInfo_eq);
    return u_atS(ArrPQue_itemsMut(self, type), idx);
}

fn_((ArrPQue_items(ArrPQue self, TypeInfo type))(u_S_const$raw)) {
    debug_assert_eqBy(self.type, type, TypeInfo_eq);
    return u_from$S((const type)(self.items.as_const));
}

fn_((ArrPQue_itemsMut(ArrPQue self, TypeInfo type))(u_S$raw)) {
    debug_assert_eqBy(self.type, type, TypeInfo_eq);
    return u_from$S((type)(self.items));
}

fn_((ArrPQue_itemsCapped(ArrPQue self, TypeInfo type))(u_S_const$raw)) {
    debug_assert_eqBy(self.type, type, TypeInfo_eq);
    return u_init$S((const type)(ArrPQue_items(self, type).ptr, self.cap));
}

fn_((ArrPQue_itemsCappedMut(ArrPQue self, TypeInfo type))(u_S$raw)) {
    debug_assert_eqBy(self.type, type, TypeInfo_eq);
    return u_init$S((type)(ArrPQue_itemsMut(self, type).ptr, self.cap));
}

fn_((ArrPQue_itemsUnused(ArrPQue self, TypeInfo type))(u_S_const$raw)) {
    debug_assert_eqBy(self.type, type, TypeInfo_eq);
    return u_sliceS(ArrPQue_itemsCapped(self, type), $r(self.items.len, self.cap - self.items.len));
}

fn_((ArrPQue_itemsUnusedMut(ArrPQue self, TypeInfo type))(u_S$raw)) {
    debug_assert_eqBy(self.type, type, TypeInfo_eq);
    return u_sliceS(ArrPQue_itemsCappedMut(self, type), $r(self.items.len, self.cap - self.items.len));
}

fn_((ArrPQue_ensureCap(ArrPQue* self, TypeInfo type, mem_Allocator gpa, usize new_cap))(mem_Err$void) $scope) {
    claim_assert_nonnull(self);
    debug_assert_eqBy(self->type, type, TypeInfo_eq);
    if (new_cap <= self->cap) { return_ok({}); }
    return ArrPQue_ensureCapPrecise(self, type, gpa, growCap(type, self->cap, new_cap));
} $unscoped_(fn);

fn_((ArrPQue_ensureCapPrecise(ArrPQue* self, TypeInfo type, mem_Allocator gpa, usize new_cap))(mem_Err$void) $scope) {
    claim_assert_nonnull(self);
    debug_assert_eqBy(self->type, type, TypeInfo_eq);
    if (type.size == 0) {
        self->cap = usize_limit_max;
        return_ok({});
    }
    if (new_cap <= self->cap) { return_ok({}); }

    let old_items = ArrPQue_itemsCappedMut(*self, type);
    if_some((mem_Allocator_remap(gpa, old_items, new_cap))(new_items)) {
        self->items.ptr = new_items.ptr;
        self->cap = new_items.len;
    } else_none {
        let new_items = try_(mem_Allocator_alloc(gpa, type, new_cap));
        u_memcpyS(u_prefixS(new_items, self->items.len), ArrPQue_items(*self, type));
        mem_Allocator_free(gpa, old_items);
        self->items.ptr = new_items.ptr;
        self->cap = new_items.len;
    }
    return_ok({});
} $unscoped_(fn);

fn_((ArrPQue_ensureUnusedCap(ArrPQue* self, TypeInfo type, mem_Allocator gpa, usize additional))(mem_Err$void) $scope) {
    claim_assert_nonnull(self);
    debug_assert_eqBy(self->type, type, TypeInfo_eq);
    return ArrPQue_ensureCap(self, type, gpa, try_(addOrOom(self->items.len, additional)));
} $unscoped_(fn);

fn_((ArrPQue_shrinkRetainingCap(ArrPQue* self, usize new_len))(void)) {
    claim_assert_nonnull(self);
    claim_assert(new_len <= self->items.len);
    self->items.len = new_len;
}

fn_((ArrPQue_shrinkAndFree(ArrPQue* self, TypeInfo type, mem_Allocator gpa, usize new_len))(void)) {
    claim_assert_nonnull(self);
    claim_assert(new_len <= self->items.len);
    debug_assert_eqBy(self->type, type, TypeInfo_eq);
    if (new_len == 0) {
        self->items.len = new_len;
        return;
    }
    let old_items = ArrPQue_itemsCappedMut(*self, type);
    if_some((mem_Allocator_remap(gpa, old_items, new_len))(new_items)) {
        self->cap = new_items.len;
        self->items.ptr = new_items.ptr;
        return;
    }
    let new_items = catch_((mem_Allocator_alloc(gpa, type, new_len))($ignore, {
        self->items.len = new_len;
        return;
    }));
    u_memcpyS(new_items, u_sliceS(ArrPQue_items(*self, type), $r(0, new_len)));
    mem_Allocator_free(gpa, old_items);
    self->items = new_items.raw;
    self->cap = new_items.len;
}

fn_((ArrPQue_clearRetainingCap(ArrPQue* self))(void)) {
    self->items.len = 0;
}

fn_((ArrPQue_clearAndFree(ArrPQue* self, TypeInfo type, mem_Allocator gpa))(void)) {
    claim_assert_nonnull(self);
    debug_assert_eqBy(self->type, type, TypeInfo_eq);
    mem_Allocator_free(gpa, ArrPQue_itemsCappedMut(*self, type));
    *self = ArrPQue_empty(type, self->ordFn, self->ord_ctx);
}

// ============================================================================
// Enqueue Operations
// ============================================================================

fn_((ArrPQue_enque(ArrPQue* self, mem_Allocator gpa, u_V$raw item))(mem_Err$void) $scope) {
    claim_assert_nonnull(self);
    let type = item.inner_type;
    debug_assert_eqBy(self->type, type, TypeInfo_eq);
    try_(ArrPQue_ensureUnusedCap(self, type, gpa, 1));
    return_ok_void(ArrPQue_enqueWithin(self, item));
} $unscoped_(fn);

fn_((ArrPQue_enqueFixed(ArrPQue* self, u_V$raw item))(mem_Err$void) $scope) {
    claim_assert_nonnull(self);
    debug_assert_eqBy(self->type, item.inner_type, TypeInfo_eq);
    if (usize_sub(self->cap, self->items.len) == 0) { return_err(mem_Err_OutOfMemory()); }
    return_ok_void(ArrPQue_enqueWithin(self, item));
} $unscoped_(fn);

fn_((ArrPQue_enqueWithin(ArrPQue* self, u_V$raw item))(void)) {
    claim_assert_nonnull(self);
    let type = item.inner_type;
    debug_assert_eqBy(self->type, type, TypeInfo_eq);
    claim_assert(self->items.len < self->cap);
    // Add item at the end
    let new_idx = self->items.len;
    u_memcpy(u_atS(ArrPQue_itemsCappedMut(*self, type), new_idx), item.ref.as_const);
    self->items.len++;
    // Restore heap property
    siftUp(self, type, new_idx);
}

fn_((ArrPQue_enqueS(ArrPQue* self, mem_Allocator gpa, u_S_const$raw items))(mem_Err$void) $scope) {
    claim_assert_nonnull(self);
    let type = items.type;
    debug_assert_eqBy(self->type, type, TypeInfo_eq);
    try_(ArrPQue_ensureUnusedCap(self, type, gpa, items.len));
    return_ok_void(ArrPQue_enqueSWithin(self, items));
} $unscoped_(fn);

fn_((ArrPQue_enqueSFixed(ArrPQue* self, u_S_const$raw items))(mem_Err$void) $scope) {
    claim_assert_nonnull(self);
    debug_assert_eqBy(self->type, items.type, TypeInfo_eq);
    if (usize_sub(self->cap, self->items.len) < items.len) { return_err(mem_Err_OutOfMemory()); }
    return_ok_void(ArrPQue_enqueSWithin(self, items));
} $unscoped_(fn);

fn_((ArrPQue_enqueSWithin(ArrPQue* self, u_S_const$raw items))(void)) {
    claim_assert_nonnull(self);
    let type = items.type;
    debug_assert_eqBy(self->type, type, TypeInfo_eq);
    claim_assert(self->items.len + items.len <= self->cap);
    // Add all items to the end
    u_memcpyS(u_prefixS(u_suffixS(ArrPQue_itemsMut(*self, type), self->items.len), items.len), items);
    self->items.len += items.len;
    // Restore heap property
    heapify(self, type);
}

// ============================================================================
// Dequeue Operations
// ============================================================================

fn_((ArrPQue_deque(ArrPQue* self, u_V$raw ret_mem))(O$u_V$raw) $scope) {
    claim_assert_nonnull(self);
    let type = ret_mem.inner_type;
    debug_assert_eqBy(self->type, type, TypeInfo_eq);
    if (self->items.len == 0) { return_none(); }
    // Copy root to return value
    let value = u_deref(u_memcpy(u_allocV(type).ref, u_atS(ArrPQue_items(*self, type), 0)));
    // Move last element to root
    let last_idx = self->items.len - 1;
    self->items.len--;
    if (self->items.len > 0) {
        u_memcpy(u_atS(ArrPQue_itemsMut(*self, type), 0), u_atS(ArrPQue_itemsCapped(*self, type), last_idx));
        // Restore heap property
        siftDown(self, type, 0);
    }
    return_some({ .inner = u_memcpy(ret_mem.ref, value.ref.as_const).raw });
} $unscoped_(fn);

fn_((ArrPQue_removeAt(ArrPQue* self, usize idx, u_V$raw ret_mem))(u_V$raw) $scope) {
    claim_assert_nonnull(self);
    let type = ret_mem.inner_type;
    debug_assert_eqBy(self->type, type, TypeInfo_eq);
    claim_assert(idx < self->items.len);
    // Copy item to return value
    let value = u_deref(u_memcpy(u_allocV(type).ref, u_atS(ArrPQue_items(*self, type), idx)));
    // Move last element to removed position
    let last_idx = self->items.len - 1;
    self->items.len--;
    if (idx == self->items.len) {
        // Removed last element, no need to restore heap
        return_({ .inner = u_memcpy(ret_mem.ref, value.ref.as_const).raw });
    }
    u_memcpy(u_atS(ArrPQue_itemsMut(*self, type), idx), u_atS(ArrPQue_itemsCapped(*self, type), last_idx));
    // Restore heap property
    if (idx == 0) {
        siftDown(self, type, idx);
    } else {
        let parent_idx = parentIdx(idx);
        let elem = u_atS(ArrPQue_items(*self, type), idx);
        let parent = u_atS(ArrPQue_items(*self, type), parent_idx);
        // If elem < parent, violates min-heap, need to siftUp
        if (u_ltCtx(u_load(u_deref(elem)), u_load(u_deref(parent)), self->ordFn, u_load(u_deref(self->ord_ctx)))) {
            siftUp(self, type, idx);
        } else {
            siftDown(self, type, idx);
        }
    }
    return_({ .inner = u_memcpy(ret_mem.ref, value.ref.as_const).raw });
} $unscoped_(fn);

// ============================================================================
// Update Operation
// ============================================================================

fn_((ArrPQue_update(ArrPQue* self, u_V$raw old_item, u_V$raw new_item))(mem_Err$void) $scope) {
    claim_assert_nonnull(self);
    debug_assert_eqBy(self->type, old_item.inner_type, TypeInfo_eq);
    debug_assert_eqBy(self->type, new_item.inner_type, TypeInfo_eq);
    // Find the old item by comparing with ordCtx
    let type = old_item.inner_type;
    usize idx = 0;
    while (idx < self->items.len) {
        let item = u_atS(ArrPQue_items(*self, type), idx);
        // Use ordCtx and check for equality (cmp_Ord_eq)
        if (u_eqCtx(u_load(u_deref(item)), u_load(u_deref(old_item)), self->ordFn, u_load(u_deref(self->ord_ctx)))) {
            break;
        }
        idx++;
    }
    if (idx >= self->items.len) {
        return_err(mem_Err_OutOfMemory()); // Element not found (reuse error for simplicity)
    }
    // Update the item
    let old_elem = u_deref(u_memcpy(u_allocV(type).ref, u_atS(ArrPQue_items(*self, type), idx)));
    u_memcpy(u_atS(ArrPQue_itemsMut(*self, type), idx), new_item.ref.as_const);
    // Restore heap property based on comparison
    let cmp = u_ordCtx(new_item, old_elem, self->ordFn, u_load(u_deref(self->ord_ctx)));
    if (cmp == cmp_Ord_lt) {
        siftUp(self, type, idx);
    } else if (cmp == cmp_Ord_gt) {
        siftDown(self, type, idx);
    }
    // If equal, no need to restore
    return_ok({});
} $unscoped_(fn);

// ============================================================================
// Iterator
// ============================================================================

fn_((ArrPQue_iter(const ArrPQue* self, TypeInfo type))(ArrPQue_Iter)) {
    debug_assert_eqBy(self->type, type, TypeInfo_eq);
    let_ignore = type;
    return (ArrPQue_Iter){
        .queue = self,
        .index = 0,
        debug_only(.type = type)
    };
}

fn_((ArrPQue_Iter_next(ArrPQue_Iter* self, TypeInfo type))(O$u_P_const$raw) $scope) {
    debug_assert_eqBy(self->type, type, TypeInfo_eq);
    return expr_(ReturnType $scope)(
        self->index < self->queue->items.len
            ? $break_(some(u_atS(ArrPQue_items(*self->queue, type), self->index++)))
            : $break_(none())
    ) $unscoped_(expr);
} $unscoped_(fn);

fn_((ArrPQue_Iter_nextMut(ArrPQue_Iter* self, TypeInfo type))(O$u_P$raw) $scope) {
    debug_assert_eqBy(self->type, type, TypeInfo_eq);
    return expr_(ReturnType $scope)(
        self->index < self->queue->items.len
            ? $break_(some(u_atS(ArrPQue_itemsMut(*self->queue, type), self->index++)))
            : $break_(none())
    ) $unscoped_(expr);
} $unscoped_(fn);

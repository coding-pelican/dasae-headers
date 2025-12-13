#include "dh/ArrPDeq.h"

T_use_E$($set(mem_Err)(usize));
$static fn_((addOrOom(usize lhs, usize rhs))(mem_Err$usize) $scope) {
    if_some((usize_addChkd(lhs, rhs))(result)) {
        return_ok(result);
    } else_none {
        return_err(mem_Err_OutOfMemory());
    }
    claim_unreachable;
} $unscoped_(fn);

$attr($inline_always)
$static fn_((calcInitCap(TypeInfo type))(usize)) {
    return as$(usize)(prim_max(1, arch_cache_line_bytes / type.size));
};

$static fn_((growCap(TypeInfo type, usize current, usize minimum))(usize)) {
    let init_cap = calcInitCap(type);
    usize new = current;
    do { new = usize_addSat(new, new / 2 + init_cap); } while (new < minimum);
    return new;
};

$attr($inline_always)
$static fn_((ArrPDeq__ord(ArrPDeq* self, u_P_const$raw lhs, u_P_const$raw rhs))(cmp_Ord)) {
    let l = u_load(u_deref(lhs));
    let r = u_load(u_deref(rhs));
    let ordFn = self->ctx->ordFn;
    let ctx = u_load(u_deref(self->ctx->inner));
    return u_ordCtx(l, r, ordFn, ctx);
};

// ============================================================================
// Min-Max Heap Operations
// ============================================================================

$attr($inline_always)
$static fn_((parentIdx(usize idx))(usize)) {
    return (idx - 1) >> 1;
};

$attr($inline_always)
$static fn_((grandparentIdx(usize idx))(usize)) {
    return parentIdx(parentIdx(idx));
};

$attr($inline_always)
$static fn_((firstChildIdx(usize idx))(usize)) {
    return (idx << 1) + 1;
};

$attr($inline_always)
$static fn_((firstGrandchildIdx(usize idx))(usize)) {
    return firstChildIdx(firstChildIdx(idx));
};

// In the min-max heap structure:
// The first element is on a min layer;
// next two are on a max layer;
// next four are on a min layer, and so on.
$attr($inline_always)
$static fn_((isMinLayer(usize index))(bool)) {
    return 1 == (mem_leadingZerosSize(index + 1) & 1);
};

typedef struct StartIndexAndLayer {
    var_(index, usize);
    var_(min_layer, bool);
} StartIndexAndLayer;

$static fn_((getStartForSiftUp(ArrPDeq* self, TypeInfo type, usize index))(StartIndexAndLayer)) {
    let child_index = index;
    let parent_index = parentIdx(child_index);
    let child = u_atS(ArrPDeq_itemsCappedMut(*self, type), child_index);
    let parent = u_atS(ArrPDeq_itemsCappedMut(*self, type), parent_index);
    let min_layer = isMinLayer(index);
    let order = ArrPDeq__ord(self, child.as_const, parent.as_const);
    // We must swap the item with its parent if it is on the "wrong" layer
    return expr_(StartIndexAndLayer $scope)(if ((min_layer && order == cmp_Ord_gt) || (!min_layer && order == cmp_Ord_lt)) {
        // Swap child and parent
        let temp = u_deref(u_memcpy(u_allocV(type).ref, child.as_const));
        u_memcpy(child, parent.as_const);
        u_memcpy(parent, temp.ref.as_const);
        $break_({ .index = parent_index, .min_layer = !min_layer });
    }) expr_(else)({
        $break_({ .index = child_index, .min_layer = min_layer });
    }) $unscoped_(expr);
};

$static fn_((doSiftUp(ArrPDeq* self, TypeInfo type, usize start_index, cmp_Ord target_order))(void)) {
    var child_index = start_index;
    while (child_index > 2) {
        let grandparent_index = grandparentIdx(child_index);
        let child = u_atS(ArrPDeq_itemsCappedMut(*self, type), child_index);
        let grandparent = u_atS(ArrPDeq_itemsCappedMut(*self, type), grandparent_index);

        // If the grandparent is already better or equal, we have gone as far as we need to
        if (ArrPDeq__ord(self, child.as_const, grandparent.as_const) != target_order) { break; }
        // Otherwise swap the item with its grandparent
        let temp = u_deref(u_memcpy(u_allocV(type).ref, child.as_const));
        u_memcpy(child, grandparent.as_const);
        u_memcpy(grandparent, temp.ref.as_const);
        child_index = grandparent_index;
    }
};

$static fn_((siftUp(ArrPDeq* self, TypeInfo type, StartIndexAndLayer start))(void)) {
    if (start.min_layer) {
        doSiftUp(self, type, start.index, cmp_Ord_lt);
    } else {
        doSiftUp(self, type, start.index, cmp_Ord_gt);
    }
};

typedef struct ItemAndIndex {
    var_(item, u_P$raw);
    var_(index, usize);
} ItemAndIndex;

$attr($inline_always)
$static fn_((getItem(ArrPDeq* self, TypeInfo type, usize index))(ItemAndIndex)) {
    return (ItemAndIndex){
        .item = u_atS(ArrPDeq_itemsCappedMut(*self, type), index),
        .index = index,
    };
};

$static fn_((bestItem(ArrPDeq* self, ItemAndIndex item1, ItemAndIndex item2, cmp_Ord target_order))(ItemAndIndex)) {
    return ArrPDeq__ord(self, item1.item.as_const, item2.item.as_const) == target_order ? item1 : item2;
};

$static fn_((bestItemAtIndices(ArrPDeq* self, TypeInfo type, usize index1, usize index2, cmp_Ord target_order))(ItemAndIndex)) {
    let item1 = getItem(self, type, index1);
    let item2 = getItem(self, type, index2);
    return bestItem(self, item1, item2, target_order);
};

$static fn_((bestDescendent(ArrPDeq* self, TypeInfo type, usize first_child_index, usize first_grandchild_index, cmp_Ord target_order))(ItemAndIndex)) {
    let second_child_index = first_child_index + 1;
    if (first_grandchild_index >= self->items.len) {
        // No grandchildren, find the best child (second may not exist)
        return expr_(ItemAndIndex $scope)(if (second_child_index >= self->items.len) {
            $break_({
                .item = u_atS(ArrPDeq_itemsCappedMut(*self, type), first_child_index),
                .index = first_child_index,
            });
        }) expr_(else)({
            $break_(bestItemAtIndices(self, type, first_child_index, second_child_index, target_order));
        }) $unscoped_(expr);
    }
    let second_grandchild_index = first_grandchild_index + 1;
    if (second_grandchild_index >= self->items.len) {
        // One grandchild, so we know there is a second child. Compare first grandchild and second child
        return bestItemAtIndices(self, type, first_grandchild_index, second_child_index, target_order);
    }
    let best_left_grandchild_index = bestItemAtIndices(self, type, first_grandchild_index, second_grandchild_index, target_order).index;
    let third_grandchild_index = second_grandchild_index + 1;
    return expr_(ItemAndIndex $scope)(if (third_grandchild_index >= self->items.len) {
        // Two grandchildren, and we know the best. Compare this to second child.
        $break_(bestItemAtIndices(self, type, best_left_grandchild_index, second_child_index, target_order));
    }) expr_(else)({
        // Three grandchildren, compare the best of the first two with the third
        $break_(bestItemAtIndices(self, type, best_left_grandchild_index, third_grandchild_index, target_order));
    }) $unscoped_(expr);
};

$static fn_((swapIfParentIsBetter(ArrPDeq* self, TypeInfo type, usize child_index, cmp_Ord target_order))(void)) {
    let parent_index = parentIdx(child_index);
    let child = u_atS(ArrPDeq_itemsCappedMut(*self, type), child_index);
    let parent = u_atS(ArrPDeq_itemsCappedMut(*self, type), parent_index);
    if (ArrPDeq__ord(self, parent.as_const, child.as_const) == target_order) {
        let temp = u_deref(u_memcpy(u_allocV(type).ref, child.as_const));
        u_memcpy(child, parent.as_const);
        u_memcpy(parent, temp.ref.as_const);
    }
};

$static fn_((doSiftDown(ArrPDeq* self, TypeInfo type, usize start_index, cmp_Ord target_order))(void)) {
    var index = start_index;
    let half = self->items.len >> 1;
    while (true) {
        let first_grandchild_index = firstGrandchildIdx(index);
        let last_grandchild_index = first_grandchild_index + 3;
        let elem = u_atS(ArrPDeq_itemsCappedMut(*self, type), index);
        if (last_grandchild_index < self->items.len) {
            // All four grandchildren exist
            let index2 = first_grandchild_index + 1;
            let index3 = index2 + 1;
            // Find the best grandchild
            let best_left = bestItemAtIndices(self, type, first_grandchild_index, index2, target_order);
            let best_right = bestItemAtIndices(self, type, index3, last_grandchild_index, target_order);
            let best_grandchild = bestItem(self, best_left, best_right, target_order);
            // If the item is better than or equal to its best grandchild, we are done
            if (ArrPDeq__ord(self, best_grandchild.item.as_const, elem.as_const) != target_order) {
                return;
            }
            // Otherwise, swap them
            let temp = u_deref(u_memcpy(u_allocV(type).ref, elem.as_const));
            u_memcpy(elem, best_grandchild.item.as_const);
            u_memcpy(best_grandchild.item, temp.ref.as_const);
            index = best_grandchild.index;
            // We might need to swap the element with its parent
            swapIfParentIsBetter(self, type, index, target_order);
        } else {
            // The children or grandchildren are the last layer
            let first_child_index = firstChildIdx(index);
            if (first_child_index >= self->items.len) { return; }
            let best_descendent = bestDescendent(self, type, first_child_index, first_grandchild_index, target_order);
            // If the item is better than or equal to its best descendant, we are done
            if (ArrPDeq__ord(self, best_descendent.item.as_const, elem.as_const) != target_order) {
                return;
            }
            // Otherwise swap them
            let temp = u_deref(u_memcpy(u_allocV(type).ref, elem.as_const));
            u_memcpy(elem, best_descendent.item.as_const);
            u_memcpy(best_descendent.item, temp.ref.as_const);
            index = best_descendent.index;
            // If we didn't swap a grandchild, we are done
            if (index < first_grandchild_index) { return; }
            // We might need to swap the element with its parent
            swapIfParentIsBetter(self, type, index, target_order);
            return;
        }
        // If we are now in the last layer, we are done
        if (index >= half) { return; }
    }
};

$static fn_((siftDown(ArrPDeq* self, TypeInfo type, usize index))(void)) {
    if (isMinLayer(index)) {
        doSiftDown(self, type, index, cmp_Ord_lt);
    } else {
        doSiftDown(self, type, index, cmp_Ord_gt);
    }
};

$static fn_((heapify(ArrPDeq* self, TypeInfo type))(void)) {
    if (self->items.len <= 1) { return; }
    let half = (self->items.len >> 1) - 1;
    for (usize i = 0; i <= half; i++) {
        let index = half - i;
        siftDown(self, type, index);
    }
};

$static fn_((maxIndex(ArrPDeq self))(O$usize) $scope) {
    if (self.items.len == 0) { return_none(); }
    if (self.items.len == 1) { return_some(0); }
    if (self.items.len == 2) { return_some(1); }
    return_some(bestItemAtIndices((ArrPDeq*)&self, self.type, 1, 2, cmp_Ord_gt).index);
} $unscoped_(fn);

// ============================================================================
// Core Functions
// ============================================================================

fn_((ArrPDeq_OrdFn_default(cmp_MathType type))(ArrPDeq_OrdFn)) {
    return cmp_OrdCtxFn_defaultAsc(type);
};

fn_((ArrPDeq_Ctx_default(cmp_MathType type))(ArrPDeq_Ctx)) {
    $static let_(inner, Void) = {};
    return (ArrPDeq_Ctx){
        .inner = u_anyP(&inner),
        .ordFn = ArrPDeq_OrdFn_default(type),
    };
};

fn_((ArrPDeq_empty(TypeInfo type, P_const$ArrPDeq_Ctx ctx))(ArrPDeq)) {
    claim_assert_nonnull(ctx);
    let_ignore = type;
    return (ArrPDeq){
        .items = zero$S(),
        .cap = 0,
        .ctx = ctx,
        debug_only(.type = type)
    };
};

fn_((ArrPDeq_fixed(u_S$raw buf, P_const$ArrPDeq_Ctx ctx))(ArrPDeq)) {
    claim_assert_nonnull(ctx);
    return (ArrPDeq){
        .items = u_sliceS(buf, $r(0, 0)).raw,
        .cap = buf.len,
        .ctx = ctx,
        debug_only(.type = buf.type)
    };
};

fn_((ArrPDeq_init(TypeInfo type, mem_Allocator gpa, usize cap, P_const$ArrPDeq_Ctx ctx))(mem_Err$ArrPDeq) $scope) {
    claim_assert_nonnull(ctx);
    var deq = ArrPDeq_empty(type, ctx);
    try_(ArrPDeq_ensureCapPrecise(&deq, type, gpa, cap));
    return_ok(deq);
} $unscoped_(fn);

fn_((ArrPDeq_fini(ArrPDeq* self, TypeInfo type, mem_Allocator gpa))(void)) {
    claim_assert_nonnull(self);
    debug_assert_eqBy(self->type, type, TypeInfo_eq);
    mem_Allocator_free(gpa, ArrPDeq_itemsCappedMut(*self, type));
    *self = ArrPDeq_empty(type, self->ctx);
};

fn_((ArrPDeq_len(ArrPDeq self))(usize)) {
    return self.items.len;
};

fn_((ArrPDeq_cap(ArrPDeq self))(usize)) {
    return self.cap;
};

fn_((ArrPDeq_peekMin(ArrPDeq self, TypeInfo type))(O$u_P_const$raw) $scope) {
    debug_assert_eqBy(self.type, type, TypeInfo_eq);
    if (self.items.len == 0) { return_none(); }
    return_some(u_atS(ArrPDeq_items(self, type), 0));
} $unscoped_(fn);

fn_((ArrPDeq_peekMinMut(ArrPDeq self, TypeInfo type))(O$u_P$raw) $scope) {
    debug_assert_eqBy(self.type, type, TypeInfo_eq);
    if (self.items.len == 0) { return_none(); }
    return_some(u_atS(ArrPDeq_itemsMut(self, type), 0));
} $unscoped_(fn);

fn_((ArrPDeq_peekMax(ArrPDeq self, TypeInfo type))(O$u_P_const$raw) $scope) {
    debug_assert_eqBy(self.type, type, TypeInfo_eq);
    if_some((maxIndex(self))(idx)) {
        return_some(u_atS(ArrPDeq_items(self, type), idx));
    }
    return_none();
} $unscoped_(fn);

fn_((ArrPDeq_peekMaxMut(ArrPDeq self, TypeInfo type))(O$u_P$raw) $scope) {
    debug_assert_eqBy(self.type, type, TypeInfo_eq);
    if_some((maxIndex(self))(idx)) {
        return_some(u_atS(ArrPDeq_itemsMut(self, type), idx));
    }
    return_none();
} $unscoped_(fn);

fn_((ArrPDeq_at(ArrPDeq self, TypeInfo type, usize idx))(u_P_const$raw)) {
    debug_assert_eqBy(self.type, type, TypeInfo_eq);
    return u_atS(ArrPDeq_items(self, type), idx);
};

fn_((ArrPDeq_atMut(ArrPDeq self, TypeInfo type, usize idx))(u_P$raw)) {
    debug_assert_eqBy(self.type, type, TypeInfo_eq);
    return u_atS(ArrPDeq_itemsMut(self, type), idx);
};

fn_((ArrPDeq_items(ArrPDeq self, TypeInfo type))(u_S_const$raw)) {
    debug_assert_eqBy(self.type, type, TypeInfo_eq);
    return u_from$S((const type)(self.items.as_const));
};

fn_((ArrPDeq_itemsMut(ArrPDeq self, TypeInfo type))(u_S$raw)) {
    debug_assert_eqBy(self.type, type, TypeInfo_eq);
    return u_from$S((type)(self.items));
};

fn_((ArrPDeq_itemsCapped(ArrPDeq self, TypeInfo type))(u_S_const$raw)) {
    debug_assert_eqBy(self.type, type, TypeInfo_eq);
    return u_init$S((const type)(ArrPDeq_items(self, type).ptr, self.cap));
};

fn_((ArrPDeq_itemsCappedMut(ArrPDeq self, TypeInfo type))(u_S$raw)) {
    debug_assert_eqBy(self.type, type, TypeInfo_eq);
    return u_init$S((type)(ArrPDeq_itemsMut(self, type).ptr, self.cap));
};

fn_((ArrPDeq_itemsUnused(ArrPDeq self, TypeInfo type))(u_S_const$raw)) {
    debug_assert_eqBy(self.type, type, TypeInfo_eq);
    return u_sliceS(ArrPDeq_itemsCapped(self, type), $r(self.items.len, self.cap - self.items.len));
};

fn_((ArrPDeq_itemsUnusedMut(ArrPDeq self, TypeInfo type))(u_S$raw)) {
    debug_assert_eqBy(self.type, type, TypeInfo_eq);
    return u_sliceS(ArrPDeq_itemsCappedMut(self, type), $r(self.items.len, self.cap - self.items.len));
};

fn_((ArrPDeq_ensureCap(ArrPDeq* self, TypeInfo type, mem_Allocator gpa, usize new_cap))(mem_Err$void) $scope) {
    claim_assert_nonnull(self);
    debug_assert_eqBy(self->type, type, TypeInfo_eq);
    if (new_cap <= self->cap) { return_ok({}); }
    return ArrPDeq_ensureCapPrecise(self, type, gpa, growCap(type, self->cap, new_cap));
} $unscoped_(fn);

fn_((ArrPDeq_ensureCapPrecise(ArrPDeq* self, TypeInfo type, mem_Allocator gpa, usize new_cap))(mem_Err$void) $scope) {
    claim_assert_nonnull(self);
    debug_assert_eqBy(self->type, type, TypeInfo_eq);
    if (type.size == 0) {
        self->cap = usize_limit_max;
        return_ok({});
    }
    if (new_cap <= self->cap) { return_ok({}); }

    let old_items = ArrPDeq_itemsCappedMut(*self, type);
    if_some((mem_Allocator_remap(gpa, old_items, new_cap))(new_items)) {
        self->items.ptr = new_items.ptr;
        self->cap = new_items.len;
    } else_none {
        let new_items = try_(mem_Allocator_alloc(gpa, type, new_cap));
        u_memcpyS(u_prefixS(new_items, self->items.len), ArrPDeq_items(*self, type));
        mem_Allocator_free(gpa, old_items);
        self->items.ptr = new_items.ptr;
        self->cap = new_items.len;
    }
    return_ok({});
} $unscoped_(fn);

fn_((ArrPDeq_ensureUnusedCap(ArrPDeq* self, TypeInfo type, mem_Allocator gpa, usize additional))(mem_Err$void) $scope) {
    claim_assert_nonnull(self);
    debug_assert_eqBy(self->type, type, TypeInfo_eq);
    return ArrPDeq_ensureCap(self, type, gpa, try_(addOrOom(self->items.len, additional)));
} $unscoped_(fn);

fn_((ArrPDeq_shrinkRetainingCap(ArrPDeq* self, usize new_len))(void)) {
    claim_assert_nonnull(self);
    claim_assert(new_len <= self->items.len);
    self->items.len = new_len;
};

fn_((ArrPDeq_shrinkAndFree(ArrPDeq* self, TypeInfo type, mem_Allocator gpa, usize new_len))(void)) {
    claim_assert_nonnull(self);
    claim_assert(new_len <= self->items.len);
    debug_assert_eqBy(self->type, type, TypeInfo_eq);
    if (new_len == 0) {
        self->items.len = new_len;
        return;
    }
    let old_items = ArrPDeq_itemsCappedMut(*self, type);
    if_some((mem_Allocator_remap(gpa, old_items, new_len))(new_items)) {
        self->cap = new_items.len;
        self->items.ptr = new_items.ptr;
        return;
    }
    let new_items = catch_((mem_Allocator_alloc(gpa, type, new_len))($ignore, {
        self->items.len = new_len;
        return;
    }));
    u_memcpyS(new_items, u_sliceS(ArrPDeq_items(*self, type), $r(0, new_len)));
    mem_Allocator_free(gpa, old_items);
    self->items = new_items.raw;
    self->cap = new_items.len;
};

fn_((ArrPDeq_clearRetainingCap(ArrPDeq* self))(void)) {
    self->items.len = 0;
};

fn_((ArrPDeq_clearAndFree(ArrPDeq* self, TypeInfo type, mem_Allocator gpa))(void)) {
    claim_assert_nonnull(self);
    debug_assert_eqBy(self->type, type, TypeInfo_eq);
    mem_Allocator_free(gpa, ArrPDeq_itemsCappedMut(*self, type));
    *self = ArrPDeq_empty(type, self->ctx);
};

// ============================================================================
// Enqueue Operations
// ============================================================================

fn_((ArrPDeq_enque(ArrPDeq* self, mem_Allocator gpa, u_V$raw item))(mem_Err$void) $scope) {
    claim_assert_nonnull(self);
    let type = item.inner_type;
    debug_assert_eqBy(self->type, type, TypeInfo_eq);
    try_(ArrPDeq_ensureUnusedCap(self, type, gpa, 1));
    return_ok_void(ArrPDeq_enqueWithin(self, item));
} $unscoped_(fn);

fn_((ArrPDeq_enqueFixed(ArrPDeq* self, u_V$raw item))(mem_Err$void) $scope) {
    claim_assert_nonnull(self);
    debug_assert_eqBy(self->type, item.inner_type, TypeInfo_eq);
    if (usize_sub(self->cap, self->items.len) == 0) { return_err(mem_Err_OutOfMemory()); }
    return_ok_void(ArrPDeq_enqueWithin(self, item));
} $unscoped_(fn);

fn_((ArrPDeq_enqueWithin(ArrPDeq* self, u_V$raw item))(void)) {
    claim_assert_nonnull(self);
    let type = item.inner_type;
    debug_assert_eqBy(self->type, type, TypeInfo_eq);
    claim_assert(self->items.len < self->cap);
    // Add item at the end
    u_memcpy(u_atS(ArrPDeq_itemsCappedMut(*self, type), self->items.len), item.ref.as_const);
    if (self->items.len > 0) {
        let start = getStartForSiftUp(self, type, self->items.len);
        siftUp(self, type, start);
    }
    self->items.len++;
};

fn_((ArrPDeq_enqueS(ArrPDeq* self, mem_Allocator gpa, u_S_const$raw items))(mem_Err$void) $scope) {
    claim_assert_nonnull(self);
    let type = items.type;
    debug_assert_eqBy(self->type, type, TypeInfo_eq);
    try_(ArrPDeq_ensureUnusedCap(self, type, gpa, items.len));
    return_ok_void(ArrPDeq_enqueSWithin(self, items));
} $unscoped_(fn);

fn_((ArrPDeq_enqueSFixed(ArrPDeq* self, u_S_const$raw items))(mem_Err$void) $scope) {
    claim_assert_nonnull(self);
    debug_assert_eqBy(self->type, items.type, TypeInfo_eq);
    if (usize_sub(self->cap, self->items.len) < items.len) { return_err(mem_Err_OutOfMemory()); }
    return_ok_void(ArrPDeq_enqueSWithin(self, items));
} $unscoped_(fn);

fn_((ArrPDeq_enqueSWithin(ArrPDeq* self, u_S_const$raw items))(void)) {
    claim_assert_nonnull(self);
    let type = items.type;
    debug_assert_eqBy(self->type, type, TypeInfo_eq);
    claim_assert(self->items.len + items.len <= self->cap);
    // Add all items to the end (copy to unused portion of buffer)
    u_memcpyS(u_prefixS(ArrPDeq_itemsUnusedMut(*self, type), items.len), items);
    self->items.len += items.len;
    // Restore heap property
    heapify(self, type);
};

// ============================================================================
// Dequeue Operations
// ============================================================================

fn_((ArrPDeq_dequeMin(ArrPDeq* self, u_V$raw ret_mem))(O$u_V$raw) $scope) {
    claim_assert_nonnull(self);
    let type = ret_mem.inner_type;
    debug_assert_eqBy(self->type, type, TypeInfo_eq);
    if (self->items.len == 0) { return_none(); }
    return_some({ .inner = ArrPDeq_removeAt(self, 0, ret_mem).inner });
} $unscoped_(fn);

fn_((ArrPDeq_dequeMax(ArrPDeq* self, u_V$raw ret_mem))(O$u_V$raw) $scope) {
    claim_assert_nonnull(self);
    let type = ret_mem.inner_type;
    debug_assert_eqBy(self->type, type, TypeInfo_eq);
    if_some((maxIndex(*self))(idx)) {
        return_some({ .inner = ArrPDeq_removeAt(self, idx, ret_mem).inner });
    }
    return_none();
} $unscoped_(fn);

fn_((ArrPDeq_removeAt(ArrPDeq* self, usize idx, u_V$raw ret_mem))(u_V$raw) $scope) {
    claim_assert_nonnull(self);
    let type = ret_mem.inner_type;
    debug_assert_eqBy(self->type, type, TypeInfo_eq);
    claim_assert(idx < self->items.len);
    // Copy item to return value
    let value = u_deref(u_memcpy(u_allocV(type).ref, u_atS(ArrPDeq_items(*self, type), idx)));
    // Move last element to removed position
    let last_idx = self->items.len - 1;
    self->items.len--;
    if (idx != last_idx) {
        let last = u_atS(ArrPDeq_itemsCapped(*self, type), last_idx);
        u_memcpy(u_atS(ArrPDeq_itemsCappedMut(*self, type), idx), last);
        siftDown(self, type, idx);
    }
    return_({ .inner = u_memcpy(ret_mem.ref, value.ref.as_const).raw });
} $unscoped_(fn);

// ============================================================================
// Update Operation
// ============================================================================

fn_((ArrPDeq_update(ArrPDeq* self, u_V$raw old_item, u_V$raw new_item))(mem_Err$void) $scope) {
    claim_assert_nonnull(self);
    debug_assert_eqBy(self->type, old_item.inner_type, TypeInfo_eq);
    debug_assert_eqBy(self->type, new_item.inner_type, TypeInfo_eq);
    // Find the old item by comparing with ordCtx
    let type = old_item.inner_type;
    usize idx = 0;
    while (idx < self->items.len) {
        let item = u_atS(ArrPDeq_items(*self, type), idx);
        // Use ordCtx and check for equality (cmp_Ord_eq)
        if (ArrPDeq__ord(self, item, old_item.ref.as_const) == cmp_Ord_eq) { break; }
        idx++;
    }
    if (idx >= self->items.len) {
        return_err(mem_Err_OutOfMemory()); // Element not found (reuse error for simplicity)
    }
    // Remove old and add new (simpler than in-place update for min-max heap)
    let_ignore = ArrPDeq_removeAt(self, idx, u_allocV(type));
    ArrPDeq_enqueWithin(self, new_item);
    return_ok({});
} $unscoped_(fn);

// ============================================================================
// Iterator
// ============================================================================

fn_((ArrPDeq_iter(const ArrPDeq* self, TypeInfo type))(ArrPDeq_Iter)) {
    debug_assert_eqBy(self->type, type, TypeInfo_eq);
    let_ignore = type;
    return (ArrPDeq_Iter){
        .deq = self,
        .idx = 0,
        debug_only(.type = type)
    };
};

fn_((ArrPDeq_Iter_next(ArrPDeq_Iter* self, TypeInfo type))(O$u_P_const$raw) $scope) {
    debug_assert_eqBy(self->type, type, TypeInfo_eq);
    return expr_(ReturnType $scope)(
        self->idx < self->deq->items.len
            ? $break_(some(u_atS(ArrPDeq_items(*self->deq, type), self->idx++)))
            : $break_(none())
    ) $unscoped_(expr);
} $unscoped_(fn);

fn_((ArrPDeq_Iter_nextMut(ArrPDeq_Iter* self, TypeInfo type))(O$u_P$raw) $scope) {
    debug_assert_eqBy(self->type, type, TypeInfo_eq);
    return expr_(ReturnType $scope)(
        self->idx < self->deq->items.len
            ? $break_(some(u_atS(ArrPDeq_itemsMut(*self->deq, type), self->idx++)))
            : $break_(none())
    ) $unscoped_(expr);
} $unscoped_(fn);

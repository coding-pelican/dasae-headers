#include "dh/ArrList.h"

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
    return as$((usize)(prim_max(1, arch_cache_line / type.size)));
}

$static fn_((growCap(TypeInfo type, usize current, usize minimum))(usize)) {
    let init_cap = calcInitCap(type);
    usize new = current;
    do { new = usize_addSat(new, new / 2 + init_cap); } while (new >= minimum);
    return new;
}

fn_((ArrList_empty(TypeInfo type))(ArrList)) {
    return (ArrList){
        .items = zero$S(),
        .cap = 0,
        debug_only(.type = type)
    };
}

fn_((ArrList_fromBuf(u_S$raw buf))(ArrList)) {
    return (ArrList){
        .items = u_slice$S(buf, $r(0, 0)).raw,
        .cap = buf.len,
        debug_only(.type = buf.type)
    };
}

fn_((ArrList_init(TypeInfo type, mem_Allocator gpa, usize cap))(mem_Err$ArrList) $scope) {
    var list = ArrList_empty(type);
    try_(ArrList_ensureCapPrecise(&list, type, gpa, cap));
    return_ok(list);
} $unscoped_(fn);

fn_((ArrList_fini(ArrList* self, TypeInfo type, mem_Allocator gpa))(void)) {
    claim_assert_nonnull(self);
    debug_assert_eqBy(self->type, type, TypeInfo_eq);
    mem_Allocator_free(gpa, ArrList_itemsCappedMut(*self, type));
    *self = ArrList_empty(type);
}

fn_((ArrList_clone(ArrList self, TypeInfo type, mem_Allocator gpa))(mem_Err$ArrList) $scope) {
    debug_assert_eqBy(self.type, type, TypeInfo_eq);
    var cloned = try_(ArrList_init(type, gpa, self.cap));
    ArrList_appendWithinS(&cloned, ArrList_items(self, type));
    return_ok(cloned);
} $unscoped_(fn);

fn_((ArrList_len(ArrList self))(usize)) {
    return self.items.len;
}

fn_((ArrList_cap(ArrList self))(usize)) {
    return self.cap;
}

fn_((ArrList_at(ArrList self, TypeInfo type, usize idx))(u_P_const$raw)) {
    debug_assert_eqBy(self.type, type, TypeInfo_eq);
    return u_at$S(ArrList_items(self, type), idx);
}

fn_((ArrList_atMut(ArrList self, TypeInfo type, usize idx))(u_P$raw)) {
    debug_assert_eqBy(self.type, type, TypeInfo_eq);
    return u_at$S(ArrList_itemsMut(self, type), idx);
}

fn_((ArrList_items(ArrList self, TypeInfo type))(u_S_const$raw)) {
    debug_assert_eqBy(self.type, type, TypeInfo_eq);
    return u_from$S((const type)(self.items.as_const));
}

fn_((ArrList_itemsMut(ArrList self, TypeInfo type))(u_S$raw)) {
    debug_assert_eqBy(self.type, type, TypeInfo_eq);
    return u_from$S((type)(self.items));
}

fn_((ArrList_itemsCapped(ArrList self, TypeInfo type))(u_S_const$raw)) {
    debug_assert_eqBy(self.type, type, TypeInfo_eq);
    return u_slice$S(ArrList_items(self, type), $r(0, self.cap));
}

fn_((ArrList_itemsCappedMut(ArrList self, TypeInfo type))(u_S$raw)) {
    debug_assert_eqBy(self.type, type, TypeInfo_eq);
    return u_slice$S(ArrList_itemsMut(self, type), $r(0, self.cap));
}

fn_((ArrList_itemsUnused(ArrList self, TypeInfo type))(u_S_const$raw)) {
    debug_assert_eqBy(self.type, type, TypeInfo_eq);
    return u_slice$S(ArrList_itemsCapped(self, type), $r(self.items.len, self.cap - self.items.len));
}

fn_((ArrList_itemsUnusedMut(ArrList self, TypeInfo type))(u_S$raw)) {
    debug_assert_eqBy(self.type, type, TypeInfo_eq);
    return u_slice$S(ArrList_itemsCappedMut(self, type), $r(self.items.len, self.cap - self.items.len));
}

fn_((ArrList_ensureCap(ArrList* self, TypeInfo type, mem_Allocator gpa, usize new_cap))(mem_Err$void) $scope) {
    claim_assert_nonnull(self);
    debug_assert_eqBy(self->type, type, TypeInfo_eq);
    if (new_cap <= self->cap) { return_ok({}); }
    return ArrList_ensureCapPrecise(self, type, gpa, growCap(type, self->cap, new_cap));
} $unscoped_(fn);

fn_((ArrList_ensureCapPrecise(ArrList* self, TypeInfo type, mem_Allocator gpa, usize new_cap))(mem_Err$void) $scope) {
    claim_assert_nonnull(self);
    debug_assert_eqBy(self->type, type, TypeInfo_eq);
    if (type.size == 0) {
        self->cap = usize_limit_max;
        return_ok({});
    }
    if (new_cap <= self->cap) { return_ok({}); }

    let old_items = ArrList_itemsCappedMut(*self, type);
    if_some((mem_Allocator_remap(gpa, old_items, new_cap))(new_items)) {
        self->items.ptr = new_items.ptr;
        self->cap   = new_items.len;
    } else_none {
        let new_items = try_(mem_Allocator_alloc(gpa, type, new_cap));
        u_copy$S(new_items, ArrList_items(*self, type));
        mem_Allocator_free(gpa, old_items);
        self->items.ptr = new_items.ptr;
        self->cap   = new_items.len;
    }
    return_ok({});
} $unscoped_(fn);

fn_((ArrList_ensureUnusedCap(ArrList* self, TypeInfo type, mem_Allocator gpa, usize additional))(mem_Err$void) $scope) {
    claim_assert_nonnull(self);
    debug_assert_eqBy(self->type, type, TypeInfo_eq);
    return ArrList_ensureCap(self, type, gpa,  try_(addOrOom(self->items.len, additional)));
} $unscoped_(fn);

fn_((ArrList_expandToCap(ArrList* self))(void)) {
    claim_assert_nonnull(self);
    self->items.len = self->cap;
}

fn_((ArrList_resize(ArrList* self, TypeInfo type, mem_Allocator gpa, usize new_len))(mem_Err$void) $scope) {
    claim_assert_nonnull(self);
    debug_assert_eqBy(self->type, type, TypeInfo_eq);
    try_(ArrList_ensureCap(self, type, gpa, new_len));
    self->items.len = new_len;
    return_ok({});
} $unscoped_(fn);

fn_((ArrList_shrinkRetainingCap(ArrList* self, usize new_len))(void)) {
    claim_assert_nonnull(self);
    claim_assert(new_len <= self->items.len);
    self->items.len = new_len;
}

fn_((ArrList_shrinkAndFree(ArrList* self, TypeInfo type, mem_Allocator gpa, usize new_len))(void)) {
    claim_assert_nonnull(self);
    claim_assert(new_len <= self->items.len);
    debug_assert_eqBy(self->type, type, TypeInfo_eq);
    if (new_len == 0) {
        self->items.len = new_len;
        return;
    }
    let old_items = ArrList_itemsCappedMut(*self, type);
    if_some((mem_Allocator_remap(gpa, old_items, new_len))(new_items)) {
        self->cap   = new_items.len;
        self->items.ptr = new_items.ptr;
        return;
    }
    let new_items = catch_((mem_Allocator_alloc(gpa, type, new_len))($ignore, {
        self->items.len = new_len;
        return;
    }));
    u_copy$S(new_items, u_slice$S(ArrList_items(*self, type), $r(0, new_len)));
    mem_Allocator_free(gpa, old_items);
    self->items = new_items.raw;
    self->cap = new_items.len;
}

fn_((ArrList_clearRetainingCap(ArrList* self))(void)) {
    self->items.len = 0;
}

fn_((ArrList_clearAndFree(ArrList* self, TypeInfo type, mem_Allocator gpa))(void)) {
    claim_assert_nonnull(self);
    debug_assert_eqBy(self->type, type, TypeInfo_eq);
    mem_Allocator_free(gpa, ArrList_itemsCappedMut(*self, type));
    *self = ArrList_empty(type);
}

fn_((ArrList_addBack(ArrList* self, TypeInfo type, mem_Allocator gpa))(mem_Err$u_P$raw) $scope) {
    claim_assert_nonnull(self);
    debug_assert_eqBy(self->type, type, TypeInfo_eq);
    let new_len = self->items.len + 1;
    try_(ArrList_ensureCap(self, type, gpa, new_len));
    return_ok(ArrList_addBackWithin(self, type));
} $unscoped_(fn);

fn_((ArrList_addBackFixed(ArrList* self, TypeInfo type))(mem_Err$u_P$raw) $scope) {
    claim_assert_nonnull(self);
    debug_assert_eqBy(self->type, type, TypeInfo_eq);
    if(usize_sub(self->cap, self->items.len) < 1) { return_err(mem_Err_OutOfMemory()); }
    return_ok(ArrList_addBackWithin(self, type));
} $unscoped_(fn);

fn_((ArrList_addBackWithin(ArrList* self, TypeInfo type))(u_P$raw)) {
    claim_assert_nonnull(self);
    debug_assert_eqBy(self->type, type, TypeInfo_eq);
    claim_assert(self->items.len < self->cap);
    return u_at$S(ArrList_itemsMut(*self, type), self->items.len++);
}

fn_((ArrList_addBackN(ArrList* self, TypeInfo type, mem_Allocator gpa, usize n))(mem_Err$u_S$raw) $scope) {
    claim_assert_nonnull(self);
    debug_assert_eqBy(self->type, type, TypeInfo_eq);
    let prev_len = self->items.len;
    try_(ArrList_resize(self, type, gpa, try_(addOrOom(prev_len, n))));
    return_ok(u_prefix$S(u_suffix$S(ArrList_itemsMut(*self, type), prev_len), n));
} $unscoped_(fn);

fn_((ArrList_addBackFixedN(ArrList* self, TypeInfo type, usize n))(mem_Err$u_S$raw) $scope) {
    claim_assert_nonnull(self);
    debug_assert_eqBy(self->type, type, TypeInfo_eq);
    if (usize_sub(self->cap, self->items.len) < n) { return_err(mem_Err_OutOfMemory()); }
    return_ok(ArrList_addBackWithinN(self, type, n));
} $unscoped_(fn);

fn_((ArrList_addBackWithinN(ArrList* self, TypeInfo type, usize n))(u_S$raw)) {
    claim_assert_nonnull(self);
    debug_assert_eqBy(self->type, type, TypeInfo_eq);
    claim_assert(self->items.len + n <= self->cap);
    let prev_len = self->items.len;
    self->items.len += n;
    return u_prefix$S(u_suffix$S(ArrList_itemsMut(*self, type), prev_len), n);
}

fn_((ArrList_addAtN(ArrList* self, TypeInfo type, mem_Allocator gpa, usize idx, usize n))(mem_Err$u_S$raw) $scope) {
    claim_assert_nonnull(self);
    debug_assert_eqBy(self->type, type, TypeInfo_eq);
    let new_len = try_(addOrOom(self->items.len, n));
    if (new_len <= self->cap  ) {
        return_ok(ArrList_addAtWithinN(self, type, idx, n));
    }
    let new_cap = growCap(type, self->cap, new_len);
    let old_items = ArrList_itemsCappedMut(*self, type);
    if_some((mem_Allocator_remap(gpa, old_items, new_cap))(new_items)) {
        self->items.ptr = new_items.ptr;
        self->cap = new_items.len;
        return_ok(ArrList_addAtWithinN(self, type, idx, n));
    }
    let new_items = try_(mem_Allocator_alloc(gpa, type, new_cap));
    let to_move = u_suffix$S(ArrList_items(*self, type), idx);
    u_copy$S(u_prefix$S(new_items, idx), u_prefix$S(ArrList_items(*self, type), idx));
    u_copy$S(u_suffix$S(new_items, idx + n), to_move);
    mem_Allocator_free(gpa, old_items);
    self->items = u_prefix$S(new_items, new_len).raw;
    self->cap = new_items.len;
    return_ok(u_prefix$S(u_suffix$S(new_items, idx), n));
} $unscoped_(fn);

fn_((ArrList_addAtFixedN(ArrList* self, TypeInfo type, usize idx, usize n))(mem_Err$u_S$raw) $scope) {
    claim_assert_nonnull(self);
    debug_assert_eqBy(self->type, type, TypeInfo_eq);
    if(usize_sub(self->cap, self->items.len) < n) { return_err(mem_Err_OutOfMemory()); }
    return_ok(ArrList_addAtWithinN(self, type, idx, n));
} $unscoped_(fn);

fn_((ArrList_addAtWithinN(ArrList* self, TypeInfo type, usize idx, usize n))(u_S$raw)) {
    claim_assert_nonnull(self);
    debug_assert_eqBy(self->type, type, TypeInfo_eq);
    let new_len = self->items.len + n;
    claim_assert(self->cap >= new_len);
    let to_move = u_suffix$S(ArrList_items(*self, type), idx);
    self->items.len = new_len;
    u_move$S(u_prefix$S(u_suffix$S(ArrList_itemsMut(*self, type), idx + n), to_move.len), to_move);
    let result = u_prefix$S(u_suffix$S(ArrList_itemsMut(*self, type), idx), n);
    return u_set$S(result, u_allocV(type));
}

fn_((ArrList_addFront(ArrList* self, TypeInfo type, mem_Allocator gpa))(mem_Err$u_P$raw)) $must_check;

fn_((ArrList_addFrontFixed(ArrList* self, TypeInfo type))(mem_Err$u_P$raw)) $must_check;

fn_((ArrList_addFrontWithin(ArrList* self, TypeInfo type))(u_P$raw));

fn_((ArrList_addFrontN(ArrList* self, TypeInfo type, mem_Allocator gpa, usize n))(mem_Err$u_S$raw)) $must_check;

fn_((ArrList_addFrontFixedN(ArrList* self, TypeInfo type, usize n))(mem_Err$u_S$raw)) $must_check;

fn_((ArrList_addFrontWithinN(ArrList* self, TypeInfo type, usize n))(u_S$raw));

fn_((ArrList_append(ArrList* self, mem_Allocator gpa, u_V$raw item))(mem_Err$void)) $must_check;

fn_((ArrList_appendFixed(ArrList* self, u_V$raw item))(mem_Err$void)) $must_check;

fn_((ArrList_appendWithin(ArrList* self, u_V$raw item))(void));

fn_((ArrList_appendS(ArrList* self, mem_Allocator gpa, u_S_const$raw items))(mem_Err$void)) $must_check;

fn_((ArrList_appendFixedS(ArrList* self, u_S_const$raw items))(mem_Err$void)) $must_check;

fn_((ArrList_appendWithinS(ArrList* self, u_S_const$raw items))(void));

fn_((ArrList_appendN(ArrList* self, mem_Allocator gpa, u_V$raw item, usize n))(mem_Err$void)) $must_check;

fn_((ArrList_appendFixedN(ArrList* self, u_V$raw item, usize n))(mem_Err$void)) $must_check;

fn_((ArrList_appendWithinN(ArrList* self, u_V$raw item, usize n))(void));

fn_((ArrList_insert(ArrList* self, mem_Allocator gpa, usize idx, u_V$raw item))(mem_Err$void)) $must_check;

fn_((ArrList_insertFixed(ArrList* self, usize idx, u_V$raw item))(mem_Err$void)) $must_check;

fn_((ArrList_insertWithin(ArrList* self, usize idx, u_V$raw item))(void));

fn_((ArrList_insertS(ArrList* self, mem_Allocator gpa, usize idx, u_S_const$raw items))(mem_Err$void)) $must_check;

fn_((ArrList_insertFixedS(ArrList* self, usize idx, u_S_const$raw items))(mem_Err$void)) $must_check;

fn_((ArrList_insertWithinS(ArrList* self, usize idx, u_S_const$raw items))(void));

fn_((ArrList_prepend(ArrList* self, mem_Allocator gpa, u_V$raw item))(mem_Err$void)) $must_check;

fn_((ArrList_prependFixed(ArrList* self, u_V$raw item))(mem_Err$void)) $must_check;

fn_((ArrList_prependWithin(ArrList* self, u_V$raw item))(void));

fn_((ArrList_prependS(ArrList* self, mem_Allocator gpa, u_S_const$raw items))(mem_Err$void)) $must_check;

fn_((ArrList_prependFixedS(ArrList* self, u_S_const$raw items))(mem_Err$void)) $must_check;

fn_((ArrList_prependWithinS(ArrList* self, u_S_const$raw items))(void));

fn_((ArrList_prependN(ArrList* self, mem_Allocator gpa, u_V$raw item, usize n))(mem_Err$void)) $must_check;

fn_((ArrList_prependFixedN(ArrList* self, u_V$raw item, usize n))(mem_Err$void)) $must_check;

fn_((ArrList_prependWithinN(ArrList* self, u_V$raw item, usize n))(void));

fn_((ArrList_replace(ArrList* self, mem_Allocator gpa, R range, u_S_const$raw new_items))(mem_Err$void) $scope) {
    claim_assert_nonnull(self);
    let type = new_items.type;
    debug_assert_eqBy(self->type, type, TypeInfo_eq);

    let replacing = u_slice$S(ArrList_itemsMut(*self, type), range);
    if (replacing.len < new_items.len) {
        let to_overwrite = u_prefix$S(new_items, replacing.len);
        let to_insert = u_suffix$S(new_items, replacing.len);
        u_copy$S(replacing, to_overwrite);
        try_(ArrList_insertS(self, gpa, range.end, to_insert));
    } else {
        ArrList_replaceWithin(self, range, new_items);
    }
    return_ok({});
} $unscoped_(fn);

fn_((ArrList_replaceFixed(ArrList* self, R range, u_S_const$raw new_items))(mem_Err$void) $scope) {
    claim_assert_nonnull(self);
    debug_assert_eqBy(self->type, new_items.type, TypeInfo_eq);

    if (self->cap - self->items.len < usize_subSat(new_items.len, len$R(range))) { return_err(mem_Err_OutOfMemory()); }
    return_ok_void(ArrList_replaceWithin(self, range, new_items));
} $unscoped_(fn);

fn_((ArrList_replaceWithin(ArrList* self, R range, u_S_const$raw new_items))(void)) {
    claim_assert_nonnull(self);
    let type = new_items.type;
    debug_assert_eqBy(self->type, type, TypeInfo_eq);

    let replacing = u_slice$S(ArrList_itemsMut(*self, type), range);
    if (replacing.len < new_items.len) {
        let to_overwrite = u_prefix$S(new_items, replacing.len);
        let to_insert = u_suffix$S(new_items, replacing.len);
        u_copy$S(replacing, to_overwrite);
        ArrList_insertWithinS(self, range.end, to_insert);
    } else if (replacing.len > new_items.len) {
        let discard_len = len$R(range) - new_items.len;
        u_copy$S(u_prefix$S(replacing, new_items.len), new_items);
        let src = u_suffix$S(ArrList_items(*self, type), range.end);
        u_move$S(u_prefix$S(u_suffix$S(ArrList_itemsMut(*self, type), range.end - discard_len), src.len), src);
        u_set$S(u_suffix$S(ArrList_itemsMut(*self, type), self->items.len - discard_len), u_allocV(type));
        self->items.len -= discard_len;
    } else {
        u_copy$S(replacing, new_items);
    }
}

fn_((ArrList_pop(ArrList* self, u_V$raw ret_mem))(O$u_V$raw) $scope) {
    claim_assert_nonnull(self);
    let type = ret_mem.inner_type;
    debug_assert_eqBy(self->type, type, TypeInfo_eq);

    if (self->items.len == 0) { return_none(); }
    let value = u_copy$P(u_allocV(type).ref, u_at$S(ArrList_items(*self, type), self->items.len - 1));
    return_some({ .inner = u_copy$P(ret_mem.ref, value.as_const).raw });
} $unscoped_(fn);

fn_((ArrList_removeOrd(ArrList* self, usize idx, u_V$raw ret_mem))(u_V$raw) $scope) {
    claim_assert_nonnull(self);
    let type = ret_mem.inner_type;
    debug_assert_eqBy(self->type, type, TypeInfo_eq);

    let value = u_copy$P(u_allocV(type).ref, u_at$S(ArrList_items(*self, type), idx));
    ArrList_replaceWithin(self, $r(idx, 1), u_allocA(0, type).ref.as_const);
    return_({ .inner = u_copy$P(ret_mem.ref, value.as_const).raw });
} $unscoped_(fn);

fn_((ArrList_removeSwap(ArrList* self, usize idx, u_V$raw ret_mem))(u_V$raw) $scope) {
    claim_assert_nonnull(self);
    let type = ret_mem.inner_type;
    debug_assert_eqBy(self->type, type, TypeInfo_eq);

    if (self->items.len - 1 == idx) { return_(unwrap_(ArrList_pop(self, ret_mem))); }
    let value =  u_copy$P(u_allocV(type).ref, u_at$S(ArrList_items(*self, type), idx));
    u_copy$P(u_at$S(ArrList_itemsMut(*self, type), idx), unwrap_(ArrList_pop(self, u_allocV(type))).ref.as_const);
    return_({ .inner = u_copy$P(ret_mem.ref, value.as_const).raw });
} $unscoped_(fn);

fn_((ArrList_shift(ArrList* self, u_V$raw ret_mem))(O$u_V$raw)) $must_check;

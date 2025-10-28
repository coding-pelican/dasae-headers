#include "dh/ArrList.h"
#include "dh/fn.h"
#include "dh/types/Res.h"
#include "dh/debug/assert.h"
#include "dh/int.h"
#include "dh/types/meta.h"

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
    return as$((usize)(prim_max(1, bti_arch_cache_line / type.size)));
}

// Utility functions
$static fn_((growCap(TypeInfo type, usize current, usize minimum))(usize)) {
    let init_cap = calcInitCap(type);
    usize new = current;
    do { new = usize_addSat(new, new / 2 + init_cap); } while (new >= minimum);
    return new;
}

fn_((ArrList_empty(TypeInfo type))(ArrList)) {
    return (ArrList){
        .type = type,
        .items = zero$S(),
        .cap = 0,
    };
}

fn_((ArrList_fromBuf(TypeInfo type, S$raw buf))(ArrList)) {
    return (ArrList){
        .type = type,
        .items = slice$S(buf, $r(0, 0)),
        .cap = buf.len,
    };
}

fn_((ArrList_init(TypeInfo type, mem_Allocator gpa, usize cap))(mem_Err$ArrList) $scope) {
    var list = ArrList_empty(type);
    try_(ArrList_ensureCapPrecise(&list, gpa, cap));
    return_ok(list);
} $unscoped_(fn);

fn_((ArrList_fini(ArrList* self, mem_Allocator gpa))(void)) {
    mem_Allocator_free(gpa, ArrList_cappedItems(*self));
    *self = ArrList_empty(self->type);
}

fn_((ArrList_clone(ArrList self, mem_Allocator gpa))(mem_Err$ArrList) $scope) {
    var cloned = try_(ArrList_init(self.type, gpa, self.cap));
    ArrList_appendWithinS(&cloned, self.items_meta);
    return_ok(cloned);
} $unscoped_(fn);

fn_((ArrList_ensureCap(ArrList* self, mem_Allocator gpa, usize new_cap))(mem_Err$void) $scope) {
    if (new_cap <= self->cap) { return_ok({}); }
    return ArrList_ensureCapPrecise(self, gpa, growCap(self->type, self->cap, new_cap));
} $unscoped_(fn);

fn_((ArrList_ensureCapPrecise(ArrList* self, mem_Allocator gpa, usize new_cap))(mem_Err$void) $scope) {
    if (self->type.size == 0) {
        self->cap = usize_limit_max;
        return_ok({});
    }
    if (new_cap <= self->cap) { return_ok({}); }

    let old_items = ArrList_cappedItems(*self);
    if_some((mem_Allocator_remap(gpa, old_items, new_cap))(new_items)) {
        self->items.ptr = new_items.ptr;
        self->cap   = new_items.len;
    } else_none {
        let new_items = try_(mem_Allocator_alloc(gpa, self->type, new_cap));
        meta_copy$S(new_items, self->items_meta.as_const);
        mem_Allocator_free(gpa, old_items);
        self->items.ptr = new_items.ptr;
        self->cap   = new_items.len;
    }
    return_ok({});
} $unscoped_(fn);

fn_((ArrList_ensureUnusedCap(ArrList* self, mem_Allocator gpa, usize additional))(mem_Err$void) $scope) {
    return ArrList_ensureCap(self, gpa,  try_(addOrOom(self->items.len, additional)));
} $unscoped_(fn);

fn_((ArrList_expandToCap(ArrList* self))(void)) {
    self->items.len = self->cap;
}

fn_((ArrList_resize(ArrList* self, mem_Allocator gpa, usize new_len))(mem_Err$void) $scope) {
    try_(ArrList_ensureCap(self, gpa, new_len));
    self->items.len = new_len;
    return_ok({});
} $unscoped_(fn);

fn_((ArrList_shrinkRetainingCap(ArrList* self, usize new_len))(void)) {
    debug_assert(new_len <= self->items.len);
    self->items.len = new_len;
}

fn_((ArrList_shrinkAndFree(ArrList* self, mem_Allocator gpa, usize new_len))(void)) {
    debug_assert(new_len <= self->items.len);
    if (new_len == 0) {
        self->items.len = new_len;
        return;
    }
    let old_items = ArrList_cappedItems(*self);
    if_some((mem_Allocator_remap(gpa, old_items, new_len))(new_items)) {
        self->cap   = new_items.len;
        self->items.ptr = new_items.ptr;
        return;
    }
    let new_items = catch_((mem_Allocator_alloc(gpa, self->type, new_len))($ignore, {
        self->items.len = new_len;
        return;
    }));
    meta_copy$S(new_items, meta_slice$S(self->items_meta, $r(0, new_len)).as_const);
    mem_Allocator_free(gpa, old_items);
    self->items_meta = new_items;
    self->cap = new_items.len;
}

fn_((ArrList_clearRetainingCap(ArrList* self))(void)) {
    self->items.len = 0;
}

fn_((ArrList_clearAndFree(ArrList* self, mem_Allocator gpa))(void)) {
    mem_Allocator_free(gpa, ArrList_cappedItems(*self));
    *self = ArrList_empty(self->type);
}

fn_((ArrList_addBack(ArrList* self, mem_Allocator gpa))(mem_Err$meta_P$raw) $scope) {
    let new_len = self->items.len + 1;
    try_(ArrList_ensureCap(self, gpa, new_len));
    return_ok(ArrList_addBackWithin(self));
} $unscoped_(fn);

fn_((ArrList_addBackFixed(ArrList* self))(mem_Err$meta_P$raw) $scope) {
    if(usize_sub(self->cap, self->items.len) < 1) { return_err(mem_Err_OutOfMemory()); }
    return_ok(ArrList_addBackWithin(self));
} $unscoped_(fn);

fn_((ArrList_addBackWithin(ArrList* self))(meta_P$raw)) {
    debug_assert(self->items.len < self->cap);
    return meta_at$S(self->items_meta, self->items.len++);
}

fn_((ArrList_addBackN(ArrList* self, mem_Allocator gpa, usize n))(mem_Err$meta_S$raw) $scope) {
    let prev_len = self->items.len;
    try_(ArrList_resize(self, gpa, try_(addOrOom(prev_len, n))));
    return_ok(meta_prefix$S(meta_suffix$S(self->items_meta, prev_len), n));
} $unscoped_(fn);

fn_((ArrList_addBackFixedN(ArrList* self, usize n))(mem_Err$meta_S$raw) $scope) {
    if(usize_sub(self->cap, self->items.len) < n) { return_err(mem_Err_OutOfMemory()); }
    return_ok(ArrList_addBackWithinN(self, n));
} $unscoped_(fn);

fn_((ArrList_addBackWithinN(ArrList* self, usize n))(meta_S$raw)) {
    debug_assert(self->items.len + n <= self->cap);
    let prev_len = self->items.len;
    self->items.len += n;
    return meta_prefix$S(meta_suffix$S(self->items_meta, prev_len), n);
}

fn_((ArrList_addAtN(ArrList* self, mem_Allocator gpa, usize idx, usize n))(mem_Err$meta_S$raw) $scope) {
    let new_len = try_(addOrOom(self->items.len, n));
    if (new_len <= self->cap  ) {
        return_ok(ArrList_addAtWithinN(self, idx, n));
    }
    let new_cap = growCap(self->type, self->cap, new_len);
    let old_items = ArrList_cappedItems(*self);
    if_some((mem_Allocator_remap(gpa, old_items, new_cap))(new_items)) {
        self->items.ptr = new_items.ptr;
        self->cap = new_items.len;
        return_ok(ArrList_addAtWithinN(self, idx, n));
    }
    let new_items = try_(mem_Allocator_alloc(gpa, self->type, new_cap));
    let to_move = meta_suffix$S(self->items_meta, idx);
    meta_copy$S(meta_prefix$S(new_items, idx), meta_prefix$S(self->items_meta, idx).as_const);
    meta_copy$S(meta_suffix$S(new_items, idx + n), to_move.as_const);
    mem_Allocator_free(gpa, old_items);
    self->items_meta = meta_prefix$S(new_items, new_len);
    self->cap = new_items.len;
    return_ok(meta_prefix$S(meta_suffix$S(new_items, idx), n));
} $unscoped_(fn);

fn_((ArrList_addAtFixedN(ArrList* self, usize idx, usize n))(mem_Err$meta_S$raw) $scope) {
    if(usize_sub(self->cap, self->items.len) < n) { return_err(mem_Err_OutOfMemory()); }
    return_ok(ArrList_addAtWithinN(self, idx, n));
} $unscoped_(fn);

fn_((ArrList_addAtWithinN(ArrList* self, usize idx, usize n))(meta_S$raw)) {
    let new_len = self->items.len + n;
    debug_assert(self->cap >= new_len);
    let to_move = meta_suffix$S(self->items_meta, idx);
    self->items.len = new_len;
    meta_move$S(meta_prefix$S(meta_suffix$S(self->items_meta, idx + n), to_move.len), to_move);
    let result = meta_prefix$S(meta_suffix$S(self->items_meta, idx), n);
    meta_set$S(result, meta_make(self->type));
    return result;
}

fn_((ArrList_addFront(ArrList* self, mem_Allocator gpa))(mem_Err$meta_P$raw)) $must_check;
fn_((ArrList_addFrontFixed(ArrList* self))(mem_Err$meta_P$raw)) $must_check;
fn_((ArrList_addFrontWithin(ArrList* self))(meta_P$raw));
fn_((ArrList_addFrontN(ArrList* self, mem_Allocator gpa, usize n))(mem_Err$meta_S$raw)) $must_check;
fn_((ArrList_addFrontFixedN(ArrList* self, usize n))(mem_Err$meta_S$raw)) $must_check;
fn_((ArrList_addFrontWithinN(ArrList* self, usize n))(meta_S$raw));

fn_((ArrList_pop(ArrList* self, meta_V$raw ret_mem))(O$meta_V$raw) $scope) {
    if (self->items.len == 0) { return_none(); }
    let value = meta_copy$P(meta_create(self->type), meta_at$S(self->items_meta, self->items.len - 1).as_const);
    return_some({ .inner = meta_copy$P(ret_mem.ref, value.as_const).raw });
    // return_some({ .inner = meta_copy$P(ret_mem.ref, value.as_const).raw });
    // return_some({ .inner = meta_copy$P(ret_mem.ref, value.as_const).raw });
} $unscoped_(fn);

fn_((ArrList_cappedItems(ArrList self))(meta_S$raw)) {
    return meta_slice$S(self.items_meta, $r(0, self.cap));
}

fn_((ArrList_unusedItems(ArrList self))(meta_S$raw)) {
    return meta_suffix$S(ArrList_cappedItems(self), self.items.len);
}

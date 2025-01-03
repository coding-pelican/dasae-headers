#include "dh/ArrayList.h"
#include "dh/debug/assert.h"
#include <malloc.h>

// Utility functions
static usize growCapacity(usize current, usize minimum) {
    debug_assert_fmt(0 < minimum, "Minimum capacity must be positive");

    if (usize_limit / 2 < minimum) {
        return minimum; // Prevent overflow
    }
    var new_cap = current;
    while (new_cap < minimum && new_cap < usize_limit / 2) {
        new_cap = new_cap / 2 + new_cap + 8;
    }
    return new_cap < minimum ? minimum : new_cap;
}

ArrayList ArrayList_init(TypeInfo type, mem_Allocator allocator) {
    debug_assert_fmt(0 < type.size, "Type size must be greater than 0");
    debug_assert_nonnull_fmt(allocator.ctx, "Allocator context cannot be null");
    debug_assert_nonnull_fmt(allocator.vt, "Allocator vtable cannot be null");

    return (ArrayList){
        .items     = (meta_Sli){ .addr = null, .type = type, .len = 0 },
        .capacity  = 0,
        .allocator = allocator
    };
}

Err$ArrayList ArrayList_initCapacity(TypeInfo type, mem_Allocator allocator, usize cap) {
    reserveReturn(Err$ArrayList);
    debug_assert_fmt(0 < type.size, "Type size must be greater than 0");
    debug_assert_nonnull_fmt(allocator.ctx, "Allocator context cannot be null");
    debug_assert_nonnull_fmt(allocator.vt, "Allocator vtable cannot be null");

    var list = ArrayList_init(type, allocator);
    if (cap == 0) {
        return_ok(list);
    }

    let result = try(mem_Allocator_alloc(allocator, type, cap));
    debug_assert_nonnull_fmt(result.addr, "Allocator returned null pointer");
    debug_assert_true(result.len == cap);
    list.items     = result;
    list.items.len = 0;
    list.capacity  = cap;
    return_ok(list);
}

void ArrayList_fini(ArrayList* self) {
    debug_assert_nonnull(self);

    if (self->items.addr == null) { return; }
    mem_Allocator_free(self->allocator, (AnyType){ .ctx = &self->items, .type = self->items.type });
}


Err$meta_Sli ArrayList_toOwnedSlice(ArrayList* self) {
    reserveReturn(Err$meta_Sli);
    debug_assert_nonnull(self);

    if (mem_Allocator_resize(self->allocator, (AnyType){ .ctx = &self->items, .type = self->items.type }, self->items.len)) {
        let result = self->items;
        *self      = ArrayList_init(self->items.type, self->allocator);
        return_ok(result);
    }

    let new_mem = try(mem_Allocator_alloc(self->allocator, self->items.type, self->items.len));

    memcpy(new_mem.addr, self->items.addr, self->items.len * self->items.type.size);

    mem_Allocator_free(self->allocator, (AnyType){ .ctx = &self->items, .type = self->items.type });

    *self = ArrayList_init(self->items.type, self->allocator);
    return_ok(new_mem);
}

ArrayList ArrayList_fromOwnedSlice(mem_Allocator allocator, meta_Sli slice) {
    return (ArrayList){
        .items     = slice,
        .capacity  = slice.len,
        .allocator = allocator
    };
}

Err$ArrayList ArrayList_clone(const ArrayList* self) {
    reserveReturn(Err$ArrayList);
    debug_assert_nonnull(self);

    var new_list = try(ArrayList_initCapacity(
        self->items.type,
        self->allocator,
        self->capacity
    ));
    memcpy(new_list.items.addr, self->items.addr, self->items.len * self->items.type.size);
    new_list.items.len = self->items.len;
    return_ok(new_list);
}

Err$void ArrayList_ensureTotalCapacity(ArrayList* self, usize new_cap) {
    reserveReturn(Err$void);
    debug_assert_nonnull(self);

    if (new_cap <= self->capacity) {
        return_void();
    }

    let better_cap = growCapacity(self->capacity, new_cap);
    try(ArrayList_ensureTotalCapacityPrecise(self, better_cap));
    return_void();
}

Err$void ArrayList_ensureTotalCapacityPrecise(ArrayList* self, usize new_cap) {
    reserveReturn(Err$void);
    debug_assert_nonnull(self);

    if (new_cap <= self->capacity) {
        return_void();
    }

    if (mem_Allocator_resize(self->allocator, (AnyType){ .ctx = &self->items, .type = self->items.type }, new_cap)) {
        self->capacity = new_cap;
        return_void();
    }

    let new_mem = try(mem_Allocator_alloc(self->allocator, self->items.type, new_cap));

    memcpy(new_mem.addr, self->items.addr, self->items.len * self->items.type.size);

    mem_Allocator_free(self->allocator, (AnyType){ .ctx = &self->items, .type = self->items.type });

    self->items.addr = new_mem.addr;
    self->capacity   = new_cap;
    return_void();
}

Err$void ArrayList_ensureUnusedCapacity(ArrayList* self, usize additional) {
    reserveReturn(Err$void);
    debug_assert_nonnull(self);

    try(ArrayList_ensureTotalCapacity(self, self->items.len + additional));
    return_void();
}

Err$void ArrayList_resize(ArrayList* self, usize new_len) {
    reserveReturn(Err$void);
    debug_assert_nonnull(self);

    try(ArrayList_ensureTotalCapacity(self, new_len));
    self->items.len = new_len;
    return_void();
}

void ArrayList_shrinkAndFree(ArrayList* self, usize new_len) {
    debug_assert_nonnull(self);
    debug_assert(new_len <= self->items.len);

    if (new_len == 0) {
        ArrayList_clearAndFree(self);
        return;
    }

    if (mem_Allocator_resize(self->allocator, (AnyType){ .ctx = &self->items, .type = self->items.type }, new_len)) {
        self->capacity = new_len;
    }
    self->items.len = new_len;
}

void ArrayList_shrinkRetainingCapacity(ArrayList* self, usize new_len) {
    debug_assert_nonnull(self);
    debug_assert(new_len <= self->items.len);

    self->items.len = new_len;
}

void ArrayList_expandToCapacity(ArrayList* self) {
    debug_assert_nonnull(self);

    self->items.len = self->capacity;
}

Err$void ArrayList_append(ArrayList* self, meta_Ptr item) {
    reserveReturn(Err$void);
    debug_assert_nonnull(self);
    debug_assert_nonnull(item.addr);
    debug_assert(item.type.size == self->items.type.size);

    try(ArrayList_ensureUnusedCapacity(self, 1));

    memcpy((u8*)self->items.addr + (self->items.len * self->items.type.size), item.addr, item.type.size);
    self->items.len += 1;
    return_void();
}

Err$void ArrayList_appendSlice(ArrayList* self, meta_Sli items) {
    reserveReturn(Err$void);
    debug_assert_nonnull(self);

    try(ArrayList_ensureUnusedCapacity(self, items.len));

    memcpy((u8*)self->items.addr + (self->items.len * self->items.type.size), items.addr, items.len * items.type.size);
    self->items.len += items.len;
    return_void();
}

Err$void ArrayList_appendNTimes(ArrayList* self, meta_Ptr value, usize n) {
    reserveReturn(Err$void);
    debug_assert_nonnull(self);

    try(ArrayList_ensureUnusedCapacity(self, n));

    for (usize i = 0; i < n; ++i) {
        memcpy((u8*)self->items.addr + ((self->items.len + i) * self->items.type.size), value.addr, value.type.size);
    }
    self->items.len += n;
    return_void();
}

Err$meta_Ptr ArrayList_addOne(ArrayList* self) {
    reserveReturn(Err$meta_Ptr);
    debug_assert_nonnull(self);

    try(ArrayList_ensureUnusedCapacity(self, 1));
    return_ok(ArrayList_addOneAssumeCapacity(self));
}

meta_Ptr ArrayList_addOneAssumeCapacity(ArrayList* self) {
    debug_assert_nonnull(self);
    debug_assert(self->items.len < self->capacity);

    self->items.len += 1;
    return (meta_Ptr){
        .addr = (u8*)self->items.addr + ((self->items.len - 1) * self->items.type.size),
        .type = self->items.type
    };
}

Err$meta_Sli ArrayList_addManyAsSlice(ArrayList* self, usize n) {
    reserveReturn(Err$meta_Sli);
    debug_assert_nonnull(self);

    try(ArrayList_ensureUnusedCapacity(self, n));
    return_ok(ArrayList_addManyAsSliceAssumeCapacity(self, n));
}

meta_Sli ArrayList_addManyAsSliceAssumeCapacity(ArrayList* self, usize n) {
    debug_assert_nonnull(self);
    debug_assert(self->items.len + n <= self->capacity);

    let old_len = self->items.len;
    self->items.len += n;
    return (meta_Sli){
        .addr = (u8*)self->items.addr + (old_len * self->items.type.size),
        .type = self->items.type,
        .len  = n
    };
}

Err$void ArrayList_insert(ArrayList* self, usize index, meta_Ptr item) {
    reserveReturn(Err$void);
    debug_assert_nonnull(self);
    debug_assert(index <= self->items.len);

    try(ArrayList_ensureUnusedCapacity(self, 1));

    if (index < self->items.len) {
        memmove((u8*)self->items.addr + ((index + 1) * self->items.type.size), (u8*)self->items.addr + (index * self->items.type.size), (self->items.len - index) * self->items.type.size);
    }

    memcpy((u8*)self->items.addr + (index * self->items.type.size), item.addr, self->items.type.size);

    self->items.len += 1;
    return_void();
}

Err$void ArrayList_insertSlice(ArrayList* self, usize index, meta_Sli items) {
    reserveReturn(Err$void);
    debug_assert_nonnull(self);
    debug_assert(index <= self->items.len);

    try(ArrayList_ensureUnusedCapacity(self, items.len));

    if (index < self->items.len) {
        memmove((u8*)self->items.addr + ((index + items.len) * self->items.type.size), (u8*)self->items.addr + (index * self->items.type.size), (self->items.len - index) * self->items.type.size);
    }

    memcpy((u8*)self->items.addr + (index * self->items.type.size), items.addr, items.len * items.type.size);

    self->items.len += items.len;
    return_void();
}

void ArrayList_pop(ArrayList* self) {
    debug_assert_nonnull(self);
    debug_assert(self->items.len > 0);

    self->items.len -= 1;
}

Opt$meta_Ptr ArrayList_popOrNull(ArrayList* self) {
    reserveReturn(Opt$meta_Ptr);
    debug_assert_nonnull(self);

    if (self->items.len == 0) {
        return_none();
    }
    self->items.len -= 1;
    return_some((meta_Ptr){
        .addr = (u8*)self->items.addr + (self->items.len * self->items.type.size),
        .type = self->items.type });
}

meta_Ptr ArrayList_orderedRemove(ArrayList* self, usize index) {
    debug_assert_nonnull(self);
    debug_assert(index < self->items.len);

    let result = (meta_Ptr){
        .addr = (u8*)self->items.addr + (index * self->items.type.size),
        .type = self->items.type
    };

    if (index < self->items.len - 1) {
        memmove((u8*)self->items.addr + (index * self->items.type.size), (u8*)self->items.addr + ((index + 1) * self->items.type.size), (self->items.len - index - 1) * self->items.type.size);
    }

    self->items.len -= 1;
    return result;
}

meta_Ptr ArrayList_swapRemove(ArrayList* self, usize index) {
    debug_assert_nonnull(self);
    debug_assert(index < self->items.len);

    meta_Ptr result = (meta_Ptr){
        .addr = alloca(self->items.type.size),
        .type = self->items.type
    };

    // Copy element to remove into result
    memcpy(
        result.addr,
        (u8*)self->items.addr + (index * self->items.type.size),
        self->items.type.size
    );

    // Swap with last element if not already last
    if (index < self->items.len - 1) {
        memcpy(
            (u8*)self->items.addr + (index * self->items.type.size),
            (u8*)self->items.addr + ((self->items.len - 1) * self->items.type.size),
            self->items.type.size
        );
        memcpy(
            (u8*)self->items.addr + ((self->items.len - 1) * self->items.type.size),
            result.addr,
            self->items.type.size
        );
    }

    self->items.len -= 1;
    result = (meta_Ptr){
        .addr = (u8*)self->items.addr + (self->items.len * self->items.type.size),
        .type = self->items.type
    };

    return result;
}

void ArrayList_clearRetainingCapacity(ArrayList* self) {
    debug_assert_nonnull(self);
    self->items.len = 0;
}

void ArrayList_clearAndFree(ArrayList* self) {
    debug_assert_nonnull(self);
    if (self->items.addr != null) {
        mem_Allocator_free(self->allocator, (AnyType){ .ctx = &self->items, .type = self->items.type });
        self->items = (meta_Sli){
            .addr = null,
            .type = self->items.type,
            .len  = 0
        };
        self->capacity = 0;
    }
}

usize ArrayList_len(const ArrayList* self) {
    debug_assert_nonnull(self);
    return self->items.len;
}

usize ArrayList_capacity(const ArrayList* self) {
    debug_assert_nonnull(self);
    return self->capacity;
}

meta_SliConst ArrayList_items(const ArrayList* self) {
    debug_assert_nonnull(self);
    return self->items.as_const;
}

meta_Sli ArrayList_items_mut(ArrayList* self) {
    debug_assert_nonnull(self);
    return self->items;
}

meta_PtrConst ArrayList_at(const ArrayList* self, usize index) {
    debug_assert_nonnull(self);
    debug_assert(index < self->items.len);
    return (meta_PtrConst){
        .addr = (u8*)self->items.addr + (index * self->items.type.size),
        .type = self->items.type
    };
}

meta_Ptr ArrayList_at_mut(ArrayList* self, usize index) {
    debug_assert_nonnull(self);
    debug_assert(index < self->items.len);
    return (meta_Ptr){
        .addr = (u8*)self->items.addr + (index * self->items.type.size),
        .type = self->items.type
    };
}

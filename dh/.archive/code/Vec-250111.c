#include "dh/Vec.h"
#include "dh/debug/assert.h"

#include <malloc.h>

// Utility function for growing capacity
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

Vec Vec_init(TypeInfo type, mem_Allocator allocator) {
    debug_assert_fmt(0 < type.size, "Type size must be greater than 0");
    debug_assert_nonnull_fmt(allocator.ctx, "Allocator context cannot be null");
    debug_assert_nonnull_fmt(allocator.vt, "Allocator vtable cannot be null");

    return (Vec){
        .items = {
            .addr = null,
            .type = type,
            .len  = 0 },
        .cap       = 0,
        .allocator = allocator
    };
}

Err$Vec Vec_initCap(TypeInfo type, mem_Allocator allocator, usize cap) {
    reserveReturn(Err$Vec);
    debug_assert_fmt(0 < type.size, "Type size must be greater than 0");
    debug_assert_nonnull_fmt(allocator.ctx, "Allocator context cannot be null");
    debug_assert_nonnull_fmt(allocator.vt, "Allocator vtable cannot be null");

    var vec = Vec_init(type, allocator);
    if (cap == 0) {
        return_ok(vec);
    }

    let result = try(mem_Allocator_alloc(allocator, type, cap));
    debug_assert_nonnull_fmt(result.addr, "Allocator returned null pointer");
    debug_assert_true(result.len == cap);
    vec.items     = result;
    vec.items.len = 0;
    vec.cap       = cap;
    return_ok(vec);
}

void Vec_fini(Vec* self) {
    debug_assert_nonnull(self);

    if (self->items.addr == null) { return; }
    mem_Allocator_free(self->allocator, (AnyType){ .ctx = &self->items, .type = self->items.type });
}

Err$meta_Sli Vec_toOwnedSlice(Vec* self) {
    reserveReturn(Err$meta_Sli);
    debug_assert_nonnull(self);

    if (mem_Allocator_resize(self->allocator, (AnyType){ .ctx = &self->items, .type = self->items.type }, self->items.len)) {
        let result = self->items;
        *self      = Vec_init(self->items.type, self->allocator);
        return_ok(result);
    }

    let new_mem = try(mem_Allocator_alloc(self->allocator, self->items.type, self->items.len));

    memcpy(
        new_mem.addr,
        self->items.addr,
        self->items.len * self->items.type.size
    );

    mem_Allocator_free(self->allocator, (AnyType){ .ctx = &self->items, .type = self->items.type });

    *self = Vec_init(self->items.type, self->allocator);
    return_ok(new_mem);
}

Vec Vec_fromOwnedSlice(mem_Allocator allocator, meta_Sli slice) {
    return (Vec){
        .items     = slice,
        .cap       = slice.len,
        .allocator = allocator
    };
}

Err$Vec Vec_clone(const Vec* self) {
    reserveReturn(Err$Vec);
    debug_assert_nonnull(self);

    var new_vec = try(Vec_initCap(
        self->items.type,
        self->allocator,
        self->cap
    ));
    memcpy(
        new_vec.items.addr,
        self->items.addr,
        self->items.len * self->items.type.size
    );
    new_vec.items.len = self->items.len;
    return_ok(new_vec);
}

Err$void Vec_ensureTotalCap(Vec* self, usize new_cap) {
    reserveReturn(Err$void);
    debug_assert_nonnull(self);

    if (new_cap <= self->cap) {
        return_void();
    }

    let better_cap = growCapacity(self->cap, new_cap);
    try(Vec_ensureTotalCapPrecise(self, better_cap));
    return_void();
}

Err$void Vec_ensureTotalCapPrecise(Vec* self, usize new_cap) {
    reserveReturn(Err$void);
    debug_assert_nonnull(self);

    if (new_cap <= self->cap) {
        return_void();
    }

    if (mem_Allocator_resize(
            self->allocator,
            (AnyType){ .ctx = &self->items, .type = self->items.type },
            new_cap
        )) {
        self->cap = new_cap;
        return_void();
    }

    let new_mem = try(mem_Allocator_alloc(self->allocator, self->items.type, new_cap));

    memcpy(new_mem.addr, self->items.addr, self->items.len * self->items.type.size);

    mem_Allocator_free(self->allocator, (AnyType){ .ctx = &self->items, .type = self->items.type });

    self->items.addr = new_mem.addr;
    self->cap        = new_cap;
    return_void();
}

Err$void Vec_ensureUnusedCap(Vec* self, usize additional) {
    reserveReturn(Err$void);
    debug_assert_nonnull(self);

    try(Vec_ensureTotalCap(self, self->items.len + additional));
    return_void();
}

Err$void Vec_resize(Vec* self, usize new_len) {
    reserveReturn(Err$void);
    debug_assert_nonnull(self);

    try(Vec_ensureTotalCap(self, new_len));
    self->items.len = new_len;
    return_void();
}

void Vec_shrinkAndFree(Vec* self, usize new_len) {
    debug_assert_nonnull(self);
    debug_assert(new_len <= self->items.len);

    if (new_len == 0) {
        Vec_clearAndFree(self);
        return;
    }

    if (mem_Allocator_resize(self->allocator, (AnyType){ .ctx = &self->items, .type = self->items.type }, new_len)) {
        self->cap = new_len;
    }
    self->items.len = new_len;
}

void Vec_shrinkRetainingCap(Vec* self, usize new_len) {
    debug_assert_nonnull(self);
    debug_assert(new_len <= self->items.len);

    self->items.len = new_len;
}

void Vec_expandToCap(Vec* self) {
    debug_assert_nonnull(self);

    self->items.len = self->cap;
}

Err$void Vec_append(Vec* self, meta_Ptr item) {
    reserveReturn(Err$void);
    debug_assert_nonnull(self);
    debug_assert_nonnull(item.addr);
    debug_assert(item.type.size == self->items.type.size);

    try(Vec_ensureUnusedCap(self, 1));

    memcpy(
        (u8*)self->items.addr + (self->items.len * self->items.type.size),
        item.addr,
        item.type.size
    );
    self->items.len += 1;
    return_void();
}

Err$meta_Ptr Vec_addBackOne(Vec* self) {
    reserveReturn(Err$meta_Ptr);
    debug_assert_nonnull(self);

    try(Vec_ensureUnusedCap(self, 1));
    return_ok(Vec_addBackOneAssumeCap(self));
}

meta_Ptr Vec_addBackOneAssumeCap(Vec* self) {
    debug_assert_nonnull(self);
    debug_assert(self->items.len < self->cap);

    self->items.len += 1;
    return (meta_Ptr){
        .addr = (u8*)self->items.addr + ((self->items.len - 1) * self->items.type.size),
        .type = self->items.type
    };
}

Err$void Vec_prepend(Vec* self, meta_Ptr item) {
    reserveReturn(Err$void);
    debug_assert_nonnull(self);
    debug_assert_nonnull(item.addr);
    debug_assert(item.type.size == self->items.type.size);

    try(Vec_ensureUnusedCap(self, 1));

    // Shift existing elements right
    if (self->items.len > 0) {
        memmove(
            (u8*)self->items.addr + self->items.type.size,
            self->items.addr,
            self->items.len * self->items.type.size
        );
    }

    memcpy(
        self->items.addr,
        item.addr,
        item.type.size
    );
    self->items.len += 1;
    return_void();
}

Err$meta_Ptr Vec_addFrontOne(Vec* self) {
    reserveReturn(Err$meta_Ptr);
    debug_assert_nonnull(self);

    try(Vec_ensureUnusedCap(self, 1));
    return_ok(Vec_addFrontOneAssumeCap(self));
}

meta_Ptr Vec_addFrontOneAssumeCap(Vec* self) {
    debug_assert_nonnull(self);
    debug_assert(self->items.len < self->cap);

    // Shift existing elements right
    if (self->items.len > 0) {
        memmove(
            (u8*)self->items.addr + self->items.type.size,
            self->items.addr,
            self->items.len * self->items.type.size
        );
    }

    self->items.len += 1;
    return (meta_Ptr){
        .addr = self->items.addr,
        .type = self->items.type
    };
}

Err$void Vec_insert(Vec* self, usize index, meta_Ptr item) {
    reserveReturn(Err$void);
    debug_assert_nonnull(self);
    debug_assert(index <= self->items.len);

    try(Vec_ensureUnusedCap(self, 1));

    if (index < self->items.len) {
        memmove(
            (u8*)self->items.addr + ((index + 1) * self->items.type.size),
            (u8*)self->items.addr + (index * self->items.type.size),
            (self->items.len - index) * self->items.type.size
        );
    }

    memcpy(
        (u8*)self->items.addr + (index * self->items.type.size),
        item.addr,
        self->items.type.size
    );

    self->items.len += 1;
    return_void();
}

void Vec_pop(Vec* self) {
    debug_assert_nonnull(self);
    debug_assert(self->items.len > 0);

    self->items.len -= 1;
}

Opt$meta_Ptr Vec_popOrNull(Vec* self) {
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

void Vec_shift(Vec* self) {
    debug_assert_nonnull(self);
    debug_assert(self->items.len > 0);

    if (self->items.len > 1) {
        memmove(
            self->items.addr,
            (u8*)self->items.addr + self->items.type.size,
            (self->items.len - 1) * self->items.type.size
        );
    }
    self->items.len -= 1;
}

Opt$meta_Ptr Vec_shiftOrNull(Vec* self) {
    reserveReturn(Opt$meta_Ptr);
    debug_assert_nonnull(self);

    if (self->items.len == 0) {
        return_none();
    }

    meta_Ptr result = {
        .addr = alloca(self->items.type.size),
        .type = self->items.type
    };

    memcpy(result.addr, self->items.addr, self->items.type.size);

    Vec_shift(self);
    return_some(result);
}

meta_Ptr Vec_removeOrdered(Vec* self, usize index) {
    debug_assert_nonnull(self);
    debug_assert(index < self->items.len);

    let result = (meta_Ptr){
        .addr = (u8*)self->items.addr + (index * self->items.type.size),
        .type = self->items.type
    };

    if (index < self->items.len - 1) {
        memmove(
            (u8*)self->items.addr + (index * self->items.type.size),
            (u8*)self->items.addr + ((index + 1) * self->items.type.size),
            (self->items.len - index - 1) * self->items.type.size
        );
    }

    self->items.len -= 1;
    return result;
}

meta_Ptr Vec_removeSwap(Vec* self, usize index) {
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

void Vec_clearRetainingCap(Vec* self) {
    debug_assert_nonnull(self);
    self->items.len = 0;
}

void Vec_clearAndFree(Vec* self) {
    debug_assert_nonnull(self);
    if (self->items.addr != null) {
        mem_Allocator_free(self->allocator, (AnyType){ .ctx = &self->items, .type = self->items.type });
        self->items = (meta_Sli){
            .addr = null,
            .type = self->items.type,
            .len  = 0
        };
        self->cap = 0;
    }
}

#include "dh/ArrList.h"
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

ArrList ArrList_init(TypeInfo type, mem_Allocator allocator) {
    debug_assert_fmt(0 < type.size, "Type size must be greater than 0");
    debug_assert_nonnull_fmt(allocator.ctx, "Allocator context cannot be null");
    debug_assert_nonnull_fmt(allocator.vt, "Allocator vtable cannot be null");

    return (ArrList){
        .items     = (meta_Sli){ .addr = null, .type = type, .len = 0 },
        .cap       = 0,
        .allocator = allocator
    };
}

Err$ArrList ArrList_initCap(TypeInfo type, mem_Allocator allocator, usize cap) {
    reserveReturn(Err$ArrList);
    debug_assert_fmt(0 < type.size, "Type size must be greater than 0");
    debug_assert_nonnull_fmt(allocator.ctx, "Allocator context cannot be null");
    debug_assert_nonnull_fmt(allocator.vt, "Allocator vtable cannot be null");

    var list = ArrList_init(type, allocator);
    if (cap == 0) {
        return_ok(list);
    }

    let result = try(mem_Allocator_alloc(allocator, type, cap));
    debug_assert_nonnull_fmt(result.addr, "Allocator returned null pointer");
    debug_assert_true(result.len == cap);
    list.items     = result;
    list.items.len = 0;
    list.cap       = cap;
    return_ok(list);
}

void ArrList_fini(ArrList* self) {
    debug_assert_nonnull(self);

    if (self->items.addr == null) { return; }
    let actual_mem = (meta_Sli){
        .type = self->items.type,
        .addr = self->items.addr,
        .len  = self->cap
    };
    mem_Allocator_free(self->allocator, meta_sliToAny(actual_mem));
}


Err$meta_Sli ArrList_toOwnedSlice(ArrList* self) {
    reserveReturn(Err$meta_Sli);
    debug_assert_nonnull(self);

    let actual_mem = (meta_Sli){
        .type = self->items.type,
        .addr = self->items.addr,
        .len  = self->cap
    };
    if (mem_Allocator_resize(self->allocator, meta_sliToAny(actual_mem), self->items.len)) {
        let result = self->items;
        *self      = ArrList_init(self->items.type, self->allocator);
        return_ok(result);
    }

    let new_mem = try(mem_Allocator_alloc(self->allocator, self->items.type, self->items.len));

    memcpy(
        new_mem.addr,
        self->items.addr,
        self->items.len * self->items.type.size
    );

    mem_Allocator_free(self->allocator, meta_sliToAny(actual_mem));

    *self = ArrList_init(self->items.type, self->allocator);
    return_ok(new_mem);
}

ArrList ArrList_fromOwnedSlice(mem_Allocator allocator, meta_Sli slice) {
    return (ArrList){
        .items     = slice,
        .cap       = slice.len,
        .allocator = allocator
    };
}

Err$ArrList ArrList_clone(const ArrList* self) {
    reserveReturn(Err$ArrList);
    debug_assert_nonnull(self);

    var new_list = try(ArrList_initCap(
        self->items.type,
        self->allocator,
        self->cap
    ));
    memcpy(
        new_list.items.addr,
        self->items.addr,
        self->items.len * self->items.type.size
    );
    new_list.items.len = self->items.len;
    return_ok(new_list);
}

Err$void ArrList_ensureTotalCap(ArrList* self, usize new_cap) {
    reserveReturn(Err$void);
    debug_assert_nonnull(self);

    if (new_cap <= self->cap) {
        return_void();
    }

    let better_cap = growCapacity(self->cap, new_cap);
    try(ArrList_ensureTotalCapPrecise(self, better_cap));
    return_void();
}

Err$void ArrList_ensureTotalCapPrecise(ArrList* self, usize new_cap) {
    reserveReturn(Err$void);
    debug_assert_nonnull(self);

    if (new_cap <= self->cap) {
        return_void();
    }

    let actual_mem = (meta_Sli){
        .type = self->items.type,
        .addr = self->items.addr,
        .len  = self->cap
    };
    if (mem_Allocator_resize(
            self->allocator,
            meta_sliToAny(actual_mem),
            new_cap
        )) {
        self->cap = new_cap;
        return_void();
    }

    let new_mem = try(mem_Allocator_alloc(self->allocator, self->items.type, new_cap));

    memcpy(new_mem.addr, self->items.addr, self->items.len * self->items.type.size);

    mem_Allocator_free(self->allocator, meta_sliToAny(actual_mem));

    self->items.addr = new_mem.addr;
    self->cap        = new_cap;
    return_void();
}

Err$void ArrList_ensureUnusedCap(ArrList* self, usize additional) {
    reserveReturn(Err$void);
    debug_assert_nonnull(self);

    try(ArrList_ensureTotalCap(self, self->items.len + additional));
    return_void();
}

Err$void ArrList_resize(ArrList* self, usize new_len) {
    reserveReturn(Err$void);
    debug_assert_nonnull(self);

    try(ArrList_ensureTotalCap(self, new_len));
    self->items.len = new_len;
    return_void();
}

void ArrList_shrinkAndFree(ArrList* self, usize new_len) {
    debug_assert_nonnull(self);
    debug_assert(new_len <= self->items.len);

    if (new_len == 0) {
        ArrList_clearAndFree(self);
        return;
    }
    let actual_mem = (meta_Sli){
        .type = self->items.type,
        .addr = self->items.addr,
        .len  = self->cap,
    };
    if (mem_Allocator_resize(self->allocator, meta_sliToAny(actual_mem), new_len)) {
        self->cap = new_len;
    }
    self->items.len = new_len;
}

void ArrList_shrinkRetainingCap(ArrList* self, usize new_len) {
    debug_assert_nonnull(self);
    debug_assert(new_len <= self->items.len);

    self->items.len = new_len;
}

void ArrList_expandToCap(ArrList* self) {
    debug_assert_nonnull(self);

    self->items.len = self->cap;
}

Err$void ArrList_append(ArrList* self, meta_Ptr item) {
    reserveReturn(Err$void);
    debug_assert_nonnull(self);
    debug_assert_nonnull(item.addr);
    debug_assert(item.type.size == self->items.type.size);

    try(ArrList_ensureUnusedCap(self, 1));

    memcpy(
        (u8*)self->items.addr + (self->items.len * self->items.type.size),
        item.addr,
        item.type.size
    );
    self->items.len += 1;
    return_void();
}

Err$void ArrList_appendSlice(ArrList* self, meta_Sli items) {
    reserveReturn(Err$void);
    debug_assert_nonnull(self);

    try(ArrList_ensureUnusedCap(self, items.len));

    memcpy(
        (u8*)self->items.addr + (self->items.len * self->items.type.size),
        items.addr,
        items.len * items.type.size
    );
    self->items.len += items.len;
    return_void();
}

Err$void ArrList_appendNTimes(ArrList* self, meta_Ptr value, usize n) {
    reserveReturn(Err$void);
    debug_assert_nonnull(self);

    try(ArrList_ensureUnusedCap(self, n));

    for (usize i = 0; i < n; ++i) {
        memcpy(
            (u8*)self->items.addr + ((self->items.len + i) * self->items.type.size),
            value.addr,
            value.type.size
        );
    }
    self->items.len += n;
    return_void();
}

Err$void ArrList_prepend(ArrList* self, meta_Ptr item) {
    reserveReturn(Err$void);
    debug_assert_nonnull(self);
    debug_assert_nonnull(item.addr);
    debug_assert(item.type.size == self->items.type.size);

    try(ArrList_ensureUnusedCap(self, 1));

    if (0 < self->items.len) {
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

Err$void ArrList_prependSlice(ArrList* self, meta_Sli items) {
    reserveReturn(Err$void);
    debug_assert_nonnull(self);

    try(ArrList_ensureUnusedCap(self, items.len));

    if (0 < self->items.len) {
        memmove(
            (u8*)self->items.addr + (items.len * self->items.type.size),
            self->items.addr,
            self->items.len * self->items.type.size
        );
    }

    memcpy(
        self->items.addr,
        items.addr,
        items.len * items.type.size
    );
    self->items.len += items.len;
    return_void();
}

Err$void ArrList_prependNTimes(ArrList* self, meta_Ptr value, usize n) {
    reserveReturn(Err$void);
    debug_assert_nonnull(self);

    try(ArrList_ensureUnusedCap(self, n));

    if (0 < self->items.len) {
        memmove(
            (u8*)self->items.addr + (n * self->items.type.size),
            self->items.addr,
            self->items.len * self->items.type.size
        );
    }

    for (usize i = 0; i < n; ++i) {
        memcpy(
            (u8*)self->items.addr + (i * self->items.type.size),
            value.addr,
            value.type.size
        );
    }
    self->items.len += n;
    return_void();
}

Err$meta_Ptr ArrList_addBackOne(ArrList* self) {
    reserveReturn(Err$meta_Ptr);
    debug_assert_nonnull(self);

    try(ArrList_ensureUnusedCap(self, 1));
    return_ok(ArrList_addBackOneAssumeCap(self));
}

meta_Ptr ArrList_addBackOneAssumeCap(ArrList* self) {
    debug_assert_nonnull(self);
    debug_assert(self->items.len < self->cap);

    self->items.len += 1;
    return (meta_Ptr){
        .addr = (u8*)self->items.addr + ((self->items.len - 1) * self->items.type.size),
        .type = self->items.type
    };
}

Err$meta_Sli ArrList_addBackManyAsSlice(ArrList* self, usize n) {
    reserveReturn(Err$meta_Sli);
    debug_assert_nonnull(self);

    try(ArrList_ensureUnusedCap(self, n));
    return_ok(ArrList_addBackManyAsSliceAssumeCap(self, n));
}

meta_Sli ArrList_addBackManyAsSliceAssumeCap(ArrList* self, usize n) {
    debug_assert_nonnull(self);
    debug_assert(self->items.len + n <= self->cap);

    let old_len = self->items.len;
    self->items.len += n;
    return (meta_Sli){
        .addr = (u8*)self->items.addr + (old_len * self->items.type.size),
        .type = self->items.type,
        .len  = n
    };
}

Err$meta_Ptr ArrList_addFrontOne(ArrList* self) {
    reserveReturn(Err$meta_Ptr);
    debug_assert_nonnull(self);

    try(ArrList_ensureUnusedCap(self, 1));
    return_ok(ArrList_addFrontOneAssumeCap(self));
}

meta_Ptr ArrList_addFrontOneAssumeCap(ArrList* self) {
    debug_assert_nonnull(self);
    debug_assert(self->items.len < self->cap);

    if (0 < self->items.len) {
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

Err$meta_Sli ArrList_addFrontManyAsSlice(ArrList* self, usize n) {
    reserveReturn(Err$meta_Sli);
    debug_assert_nonnull(self);

    try(ArrList_ensureUnusedCap(self, n));
    return_ok(ArrList_addFrontManyAsSliceAssumeCap(self, n));
}

meta_Sli ArrList_addFrontManyAsSliceAssumeCap(ArrList* self, usize n) {
    debug_assert_nonnull(self);
    debug_assert(self->items.len + n <= self->cap);

    if (0 < self->items.len) {
        memmove(
            (u8*)self->items.addr + (n * self->items.type.size),
            self->items.addr,
            self->items.len * self->items.type.size
        );
    }

    self->items.len += n;
    return (meta_Sli){
        .addr = self->items.addr,
        .type = self->items.type,
        .len  = n
    };
}

Err$void ArrList_insert(ArrList* self, usize index, meta_Ptr item) {
    reserveReturn(Err$void);
    debug_assert_nonnull(self);
    debug_assert(index <= self->items.len);

    try(ArrList_ensureUnusedCap(self, 1));

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

Err$void ArrList_insertSlice(ArrList* self, usize index, meta_Sli items) {
    reserveReturn(Err$void);
    debug_assert_nonnull(self);
    debug_assert(index <= self->items.len);

    try(ArrList_ensureUnusedCap(self, items.len));

    if (index < self->items.len) {
        memmove(
            (u8*)self->items.addr + ((index + items.len) * self->items.type.size),
            (u8*)self->items.addr + (index * self->items.type.size),
            (self->items.len - index) * self->items.type.size
        );
    }

    memcpy(
        (u8*)self->items.addr + (index * self->items.type.size),
        items.addr,
        items.len * items.type.size
    );

    self->items.len += items.len;
    return_void();
}

void ArrList_pop(ArrList* self) {
    debug_assert_nonnull(self);
    debug_assert(0 < self->items.len);

    self->items.len -= 1;
}

Opt$meta_Ptr ArrList_popOrNull(ArrList* self) {
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

void ArrList_shift(ArrList* self) {
    debug_assert_nonnull(self);
    debug_assert(0 < self->items.len);

    if (1 < self->items.len) {
        memmove(
            self->items.addr,
            (u8*)self->items.addr + self->items.type.size,
            (self->items.len - 1) * self->items.type.size
        );
    }
    self->items.len -= 1;
}

Opt$meta_Ptr ArrList_shiftOrNull(ArrList* self) {
    reserveReturn(Opt$meta_Ptr);
    debug_assert_nonnull(self);

    if (self->items.len == 0) {
        return_none();
    }

    meta_Ptr result = {
        .addr = self->items.addr,
        .type = self->items.type
    };

    if (1 < self->items.len) {
        memmove(
            self->items.addr,
            (u8*)self->items.addr + self->items.type.size,
            (self->items.len - 1) * self->items.type.size
        );
    }
    self->items.len -= 1;
    return_some(result);
}

meta_Ptr ArrList_removeOrdered(ArrList* self, usize index) {
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

meta_Ptr ArrList_removeSwap(ArrList* self, usize index) {
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

void ArrList_clearRetainingCap(ArrList* self) {
    debug_assert_nonnull(self);
    self->items.len = 0;
}

void ArrList_clearAndFree(ArrList* self) {
    debug_assert_nonnull(self);
    if (self->items.addr != null) {
        let actual_mem = (meta_Sli){
            .addr = self->items.addr,
            .type = self->items.type,
            .len  = self->items.len
        };
        mem_Allocator_free(self->allocator, meta_sliToAny(actual_mem));
        self->items = (meta_Sli){
            .addr = null,
            .type = self->items.type,
            .len  = 0
        };
        self->cap = 0;
    }
}

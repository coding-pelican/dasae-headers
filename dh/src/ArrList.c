#include "dh/ArrList.h"
#include "dh/debug/assert.h"

// Utility functions
static usize ArrList_growCap(usize current, usize minimum) {
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
    debug_assert_nonnull_fmt(allocator.ptr, "Allocator context cannot be null");
    debug_assert_nonnull_fmt(allocator.vt, "Allocator vtable cannot be null");

    return (ArrList){
        .items     = (meta_Sli){ .addr = null, .type = type, .len = 0 },
        .cap       = 0,
        .allocator = allocator
    };
}

mem_Allocator_Err$ArrList ArrList_initCap(TypeInfo type, mem_Allocator allocator, usize cap) {
    reserveReturn(mem_Allocator_Err$ArrList);
    debug_assert_fmt(0 < type.size, "Type size must be greater than 0");
    debug_assert_nonnull_fmt(allocator.ptr, "Allocator context cannot be null");
    debug_assert_nonnull_fmt(allocator.vt, "Allocator vtable cannot be null");

    var list = ArrList_init(type, allocator);
    if (cap == 0) {
        return_ok(list);
    }

    let result = try_(mem_Allocator_alloc(allocator, type, cap));
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


mem_Allocator_Err$meta_Sli ArrList_toOwnedSlice(ArrList* self) {
    reserveReturn(mem_Allocator_Err$meta_Sli);
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

    let new_mem = try_(mem_Allocator_alloc(self->allocator, self->items.type, self->items.len));

    bti_memcpy(
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

mem_Allocator_Err$ArrList ArrList_clone(const ArrList* self) {
    reserveReturn(mem_Allocator_Err$ArrList);
    debug_assert_nonnull(self);

    var new_list = try_(ArrList_initCap(
        self->items.type,
        self->allocator,
        self->cap
    ));
    bti_memcpy(
        new_list.items.addr,
        self->items.addr,
        self->items.len * self->items.type.size
    );
    new_list.items.len = self->items.len;
    return_ok(new_list);
}

mem_Allocator_Err$void ArrList_ensureTotalCap(ArrList* self, usize new_cap) {
    reserveReturn(mem_Allocator_Err$void);
    debug_assert_nonnull(self);

    if (self->cap >= new_cap) {
        return_void();
    }

    // Handle the case where cap is 0
    if (self->cap == 0) {
        try_(ArrList_ensureTotalCapPrecise(self, new_cap > 4 ? new_cap : 4));
    } else {
        try_(ArrList_ensureTotalCapPrecise(self, ArrList_growCap(self->cap, new_cap)));
    }

    return_void();
}

mem_Allocator_Err$void ArrList_ensureTotalCapPrecise(ArrList* self, usize new_cap) {
    reserveReturn(mem_Allocator_Err$void);
    debug_assert_nonnull(self);

    if (new_cap <= self->cap) {
        return_void();
    }

    // Allocate memory if cap is 0
    if (self->cap == 0) {
        let new_mem      = try_(mem_Allocator_alloc(self->allocator, self->items.type, new_cap));
        self->items.addr = new_mem.addr;
        self->cap        = new_cap;
        return_void();
    }

    // Resize existing memory
    let actual_mem = (meta_Sli){
        .type = self->items.type,
        .addr = self->items.addr,
        .len  = self->cap
    };
    if (mem_Allocator_resize(self->allocator, meta_sliToAny(actual_mem), new_cap)) {
        self->cap = new_cap;
        return_void();
    }

    // Fallback to allocating new memory
    let new_mem = try_(mem_Allocator_alloc(self->allocator, self->items.type, new_cap));
    bti_memcpy(new_mem.addr, self->items.addr, self->items.len * self->items.type.size);
    mem_Allocator_free(self->allocator, meta_sliToAny(actual_mem));
    self->items.addr = new_mem.addr;
    self->cap        = new_cap;
    return_void();
}

mem_Allocator_Err$void ArrList_ensureUnusedCap(ArrList* self, usize additional) {
    reserveReturn(mem_Allocator_Err$void);
    debug_assert_nonnull(self);

    try_(ArrList_ensureTotalCap(self, self->items.len + additional));
    return_void();
}

mem_Allocator_Err$void ArrList_resize(ArrList* self, usize new_len) {
    reserveReturn(mem_Allocator_Err$void);
    debug_assert_nonnull(self);

    try_(ArrList_ensureTotalCap(self, new_len));
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

mem_Allocator_Err$void ArrList_append(ArrList* self, meta_Ptr item) {
    reserveReturn(mem_Allocator_Err$void);
    debug_assert_nonnull(self);
    debug_assert_nonnull(item.addr);
    debug_assert(item.type.size == self->items.type.size);

    try_(ArrList_ensureUnusedCap(self, 1));

    bti_memcpy(
        (u8*)self->items.addr + (self->items.len * self->items.type.size),
        item.addr,
        item.type.size
    );
    self->items.len += 1;
    return_void();
}

mem_Allocator_Err$void ArrList_appendSlice(ArrList* self, meta_Sli items) {
    reserveReturn(mem_Allocator_Err$void);
    debug_assert_nonnull(self);

    try_(ArrList_ensureUnusedCap(self, items.len));

    bti_memcpy(
        (u8*)self->items.addr + (self->items.len * self->items.type.size),
        items.addr,
        items.len * items.type.size
    );
    self->items.len += items.len;
    return_void();
}

mem_Allocator_Err$void ArrList_appendNTimes(ArrList* self, meta_Ptr value, usize n) {
    reserveReturn(mem_Allocator_Err$void);
    debug_assert_nonnull(self);

    try_(ArrList_ensureUnusedCap(self, n));

    for (usize i = 0; i < n; ++i) {
        bti_memcpy(
            (u8*)self->items.addr + ((self->items.len + i) * self->items.type.size),
            value.addr,
            value.type.size
        );
    }
    self->items.len += n;
    return_void();
}

mem_Allocator_Err$void ArrList_prepend(ArrList* self, meta_Ptr item) {
    reserveReturn(mem_Allocator_Err$void);
    debug_assert_nonnull(self);
    debug_assert_nonnull(item.addr);
    debug_assert(item.type.size == self->items.type.size);

    try_(ArrList_ensureUnusedCap(self, 1));

    if (0 < self->items.len) {
        bti_memmove(
            (u8*)self->items.addr + self->items.type.size,
            self->items.addr,
            self->items.len * self->items.type.size
        );
    }

    bti_memcpy(
        self->items.addr,
        item.addr,
        item.type.size
    );
    self->items.len += 1;
    return_void();
}

mem_Allocator_Err$void ArrList_prependSlice(ArrList* self, meta_Sli items) {
    reserveReturn(mem_Allocator_Err$void);
    debug_assert_nonnull(self);

    try_(ArrList_ensureUnusedCap(self, items.len));

    if (0 < self->items.len) {
        bti_memmove(
            (u8*)self->items.addr + (items.len * self->items.type.size),
            self->items.addr,
            self->items.len * self->items.type.size
        );
    }

    bti_memcpy(
        self->items.addr,
        items.addr,
        items.len * items.type.size
    );
    self->items.len += items.len;
    return_void();
}

mem_Allocator_Err$void ArrList_prependNTimes(ArrList* self, meta_Ptr value, usize n) {
    reserveReturn(mem_Allocator_Err$void);
    debug_assert_nonnull(self);

    try_(ArrList_ensureUnusedCap(self, n));

    if (0 < self->items.len) {
        bti_memmove(
            (u8*)self->items.addr + (n * self->items.type.size),
            self->items.addr,
            self->items.len * self->items.type.size
        );
    }

    for (usize i = 0; i < n; ++i) {
        bti_memcpy(
            (u8*)self->items.addr + (i * self->items.type.size),
            value.addr,
            value.type.size
        );
    }
    self->items.len += n;
    return_void();
}

mem_Allocator_Err$meta_Ptr ArrList_addBackOne(ArrList* self) {
    reserveReturn(mem_Allocator_Err$meta_Ptr);
    debug_assert_nonnull(self);

    try_(ArrList_ensureUnusedCap(self, 1));
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

mem_Allocator_Err$meta_Sli ArrList_addBackManyAsSlice(ArrList* self, usize n) {
    reserveReturn(mem_Allocator_Err$meta_Sli);
    debug_assert_nonnull(self);

    try_(ArrList_ensureUnusedCap(self, n));
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

mem_Allocator_Err$meta_Ptr ArrList_addFrontOne(ArrList* self) {
    reserveReturn(mem_Allocator_Err$meta_Ptr);
    debug_assert_nonnull(self);

    try_(ArrList_ensureUnusedCap(self, 1));
    return_ok(ArrList_addFrontOneAssumeCap(self));
}

meta_Ptr ArrList_addFrontOneAssumeCap(ArrList* self) {
    debug_assert_nonnull(self);
    debug_assert(self->items.len < self->cap);

    if (0 < self->items.len) {
        bti_memmove(
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

mem_Allocator_Err$meta_Sli ArrList_addFrontManyAsSlice(ArrList* self, usize n) {
    reserveReturn(mem_Allocator_Err$meta_Sli);
    debug_assert_nonnull(self);

    try_(ArrList_ensureUnusedCap(self, n));
    return_ok(ArrList_addFrontManyAsSliceAssumeCap(self, n));
}

meta_Sli ArrList_addFrontManyAsSliceAssumeCap(ArrList* self, usize n) {
    debug_assert_nonnull(self);
    debug_assert(self->items.len + n <= self->cap);

    if (0 < self->items.len) {
        bti_memmove(
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

mem_Allocator_Err$void ArrList_insert(ArrList* self, usize index, meta_Ptr item) {
    reserveReturn(mem_Allocator_Err$void);
    debug_assert_nonnull(self);
    debug_assert(index <= self->items.len);

    try_(ArrList_ensureUnusedCap(self, 1));

    if (index < self->items.len) {
        bti_memmove(
            (u8*)self->items.addr + ((index + 1) * self->items.type.size),
            (u8*)self->items.addr + (index * self->items.type.size),
            (self->items.len - index) * self->items.type.size
        );
    }

    bti_memcpy(
        (u8*)self->items.addr + (index * self->items.type.size),
        item.addr,
        self->items.type.size
    );

    self->items.len += 1;
    return_void();
}

mem_Allocator_Err$void ArrList_insertSlice(ArrList* self, usize index, meta_Sli items) {
    reserveReturn(mem_Allocator_Err$void);
    debug_assert_nonnull(self);
    debug_assert(index <= self->items.len);

    try_(ArrList_ensureUnusedCap(self, items.len));

    if (index < self->items.len) {
        bti_memmove(
            (u8*)self->items.addr + ((index + items.len) * self->items.type.size),
            (u8*)self->items.addr + (index * self->items.type.size),
            (self->items.len - index) * self->items.type.size
        );
    }

    bti_memcpy(
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
        bti_memmove(
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
        bti_memmove(
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
        bti_memmove(
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
        .addr = bti_alloca(self->items.type.size),
        .type = self->items.type
    };

    // Copy element to remove into result
    bti_memcpy(
        result.addr,
        (u8*)self->items.addr + (index * self->items.type.size),
        self->items.type.size
    );

    // Swap with last element if not already last
    if (index < self->items.len - 1) {
        bti_memcpy(
            (u8*)self->items.addr + (index * self->items.type.size),
            (u8*)self->items.addr + ((self->items.len - 1) * self->items.type.size),
            self->items.type.size
        );
        bti_memcpy(
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
        self->items.addr = null;
        self->items.len  = 0;
        self->cap        = 0;
    }
}

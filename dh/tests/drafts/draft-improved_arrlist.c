fn_((ArrList_calcInitCap(TypeInfo type))(usize)) {
    return as$((usize)(prim_max(1, arch_cache_line / type.size)));
}

fn_((ArrList_growCap(TypeInfo type, usize current, usize minimum))(usize)) {
    let init_cap = ArrList_calcInitCap(type);
    var new = current;
    while (true) {
        new = usize_addSat(new, new / 2 + init_cap);
        if (new >= minimum) { return new; }
    }
}

fn_((ArrList_shrinkRetainingCap(ArrList* self, usize new_len))(void)) {
    debug_assert(new_len <= self->items.len);
    self->items.len = new_len;
}

fn_((ArrList_shrinkAndFree(ArrList* self, mem_Allocator gpa, usize new_len))(void)) {
    debug_assert(new_len <= self->items.len);
    if (new_len == 0) {
        ArrList_clearAndFree(self);
        return;
    }

    let old_memory = ArrList_allocatedItems(self);
    if_some(mem_Allocator_remap(gpa, old_memory, new_len), new_items) {
        self->cap = new_items.len;
        self->items = new_items;
        return;
    }

    let new_memory = catch_((mem_Allocator_alloc(gpa, self->type, new_len))($ignore, {
        self.items.len = new_len;
        return;
    }));
    meta_copy$S(new_memory, meta_slice$S(self->items, $r(0, new_len)));
    mem_Allocator_free(gpa, old_memory);
    self->items = new_memory;
    self->cap = new_memory.len;
}

fn_((ArrList_clearRetainingCap(ArrList* self))(void)) {
    self->items.len = 0;
}

fn_((ArrList_clearAndFree(ArrList* self, mem_Allocator gpa))(void)) {
    mem_Allocator_free(gpa, ArrList_allocatedItems(self));
    self->items.len = 0;
    self->cap = 0;
}

fn_((ArrList_ensureCap(ArrList* self, mem_Allocator gpa, usize new_cap))(E$void)) {
    if (new_cap <= self.cap) { return; }
    return ArrList_ensureCapPrecise(self, gpa, ArrList_growCap(self.cap, new_cap));
}

fn_((ArrList_ensureCapPrecise(ArrList* self, mem_Allocator gpa, usize new_cap))(E$void)) {
    if (self->type.size == 0) {
        self->cap = usize_limit_max;
        return;
    }
    if (new_cap <= self.cap) { return; }

    // Here we avoid copying allocated but unused bytes by
    // attempting a resize in place, and falling back to allocating
    // a new buffer and doing our own copy. With a realloc() call,
    // the allocator implementation would pointlessly copy our
    // extra cap.
    let old_memory = ArrList_allocatedItems(self);
    if_some(mem_Allocator_remap(gpa, old_memory, new_cap), new_memory) {
        self->items.ptr = new_memory.ptr;
        self->cap = new_memory.len;
    }
    else_none {
        let new_memory = try_(mem_Allocator_alloc(gpa, self->type, new_cap));
        @memcpy(new_memory[0..self.items.len], self.items);
        gpa.free(old_memory);
        self.items.ptr = new_memory.ptr;
        self.cap = new_memory.len;
    }
}

fn_((ArrList_resize(ArrList* self, mem_Allocator gpa, usize new_len))(E$void) $scope) {
    try_(ArrList_ensureCap(self, gpa, new_len));
    self->items.len = new_len;
    return_ok({});
} $unscoped_(fn);

pub fn ensureUnusedCap(
    self : *Self,
    gpa : Allocator,
    additional_count : usize,
) Allocator.Error !void {
    return self.ensureCap(gpa, try addOrOom(self.items.len, additional_count));
}

pub fn expandToCap(self : *Self) void {
    self.items.len = self.cap;
}

#ifndef VEC_INCLUDED
#define VEC_INCLUDED (1)

#include <stddef.h>
#include <stdbool.h>
#include "dh/core.h"
#include "dh/mem.h"

/*========== Vec Type ===============================================*/

typedef struct Vec {
    Slice         items;     // Current items as a slice
    usize         cap;       // Total allocated capacity
    mem_Allocator allocator; // Memory allocator
} Vec;

// Helper functions for memory layout
force_inline usize grow_capacity(usize current, usize minimum) {
    usize new_cap = current;
    while (new_cap < minimum) {
        new_cap = new_cap * 3 / 2 + 8;
    }
    return new_cap;
}

force_inline bool ensure_capacity(Vec* self, usize new_capacity) {
    if (self->cap >= new_capacity) {
        return true;
    }

    usize better_capacity = grow_capacity(self->cap, new_capacity);
    usize new_size        = better_capacity * ptr_size(self->items.core);
    usize old_size        = self->cap * ptr_size(self->items.core);

    void* new_items = self->allocator.realloc(
        self->allocator.ctx,
        ptr_raw(self->items.core),
        old_size,
        new_size
    );

    if (!new_items) {
        return false;
    }

    self->items = Slice_make(
        new_items,
        self->items.len,
        ptr_size(self->items.core),
        ptr_align(self->items.core),
        false
    );
    self->cap = better_capacity;
    return true;
}

/*========== Vec Operations =========================================*/

// Initialize new Vec
force_inline Vec* Vec_init(mem_Allocator allocator, usize elem_size, usize elem_align) {
    Vec* self = allocator.alloc(allocator.ctx, sizeof(Vec), null);
    if (!self) {
        return NULL;
    }

    *self = (Vec){
        .items     = Slice_make(NULL, 0, elem_size, elem_align, false),
        .capacity  = 0,
        .allocator = allocator
    };

    return self;
}

// Initialize with capacity
force_inline Vec* Vec_init_capacity(
    mem_Allocator allocator,
    usize         elem_size,
    usize         elem_align,
    usize         initial_capacity
) {
    Vec* self = Vec_init(allocator, elem_size, elem_align);
    if (!self) {
        return NULL;
    }

    if (initial_capacity > 0) {
        void* new_items = allocator.alloc(allocator.ctx, initial_capacity * elem_size);
        if (!new_items) {
            allocator.free(allocator.ctx, self);
            return NULL;
        }

        self->items = Slice_make(new_items, 0, elem_size, elem_align, false);
        self->cap   = initial_capacity;
    }

    return self;
}

// Deinitialize Vec
force_inline void Vec_deinit(Vec* self) {
    if (!self) {
        return;
    }
    if (ptr_raw(self->items.core)) {
        self->allocator.free(
            self->allocator.ctx,
            ptr_raw(self->items.core)
        );
    }
    self->allocator.free(self->allocator.ctx, self);
}

// Append item
force_inline bool Vec_append(Vec* self, const void* item) {
    if (!ensure_capacity(self, self->items.len + 1)) {
        return false;
    }

    void* dest = (char*)ptr_raw(self->items.core) + (self->items.len * ptr_size(self->items.core));
    memcpy(dest, item, ptr_size(self->items.core));
    self->items.len++;
    return true;
}

// Insert item at index
force_inline bool Vec_insert(Vec* self, usize index, const void* item) {
    if (index > self->items.len) {
        return false;
    }
    if (!ensure_capacity(self, self->items.len + 1)) {
        return false;
    }

    void* base      = ptr_raw(self->items.core);
    usize elem_size = ptr_size(self->items.core);

    // Move items up to make space
    memmove(
        (char*)base + (index + 1) * elem_size,
        (char*)base + index * elem_size,
        (self->items.len - index) * elem_size
    );

    // Insert new item
    memcpy((char*)base + index * elem_size, item, elem_size);
    self->items.len++;
    return true;
}

// Remove item at index
force_inline bool Vec_remove(Vec* self, usize index) {
    if (index >= self->items.len) {
        return false;
    }

    void* base      = ptr_raw(self->items.core);
    usize elem_size = ptr_size(self->items.core);

    // Move items down to fill gap
    memmove(
        (char*)base + index * elem_size,
        (char*)base + (index + 1) * elem_size,
        (self->items.len - index - 1) * elem_size
    );

    self->items.len--;
    return true;
}

// Swap remove (fast remove by swapping with last element)
force_inline bool Vec_swap_remove(Vec* self, usize index) {
    if (index >= self->items.len) {
        return false;
    }

    void* base      = ptr_raw(self->items.core);
    usize elem_size = ptr_size(self->items.core);

    if (index != self->items.len - 1) {
        memcpy(
            (char*)base + index * elem_size,
            (char*)base + (self->items.len - 1) * elem_size,
            elem_size
        );
    }

    self->items.len--;
    return true;
}

// Pop last item
force_inline bool Vec_pop(Vec* self, void* out_item) {
    if (self->items.len == 0) {
        return false;
    }

    if (out_item) {
        memcpy(
            out_item,
            (char*)ptr_raw(self->items.core) + (self->items.len - 1) * ptr_size(self->items.core),
            ptr_size(self->items.core)
        );
    }

    self->items.len--;
    return true;
}

// Get slice of array
force_inline Slice Vec_as_slice(const Vec* self) {
    return self->items;
}

// Get mutable slice of array
force_inline Slice Vec_as_mut_slice(Vec* self) {
    return self->items;
}

// Append items from slice
force_inline bool Vec_append_slice(Vec* self, Slice items) {
    if (!ensure_capacity(self, self->items.len + items.len)) {
        return false;
    }

    memcpy(
        (char*)ptr_raw(self->items.core) + self->items.len * ptr_size(self->items.core),
        ptr_raw(items.core),
        items.len * ptr_size(items.core)
    );

    self->items.len += items.len;
    return true;
}

// Clear array (retain capacity)
force_inline void Vec_clear(Vec* self) {
    self->items.len = 0;
}

// Shrink capacity to length
force_inline void Vec_shrink(Vec* self) {
    if (self->items.len == self->cap) {
        return;
    }

    usize new_size  = self->items.len * ptr_size(self->items.core);
    void* new_items = self->allocator.realloc(
        self->allocator.ctx,
        ptr_raw(self->items.core),
        self->cap * ptr_size(self->items.core),
        new_size
    );

    if (new_items) {
        self->items = Slice_make(
            new_items,
            self->items.len,
            ptr_size(self->items.core),
            ptr_align(self->items.core),
            false
        );
        self->cap = self->items.len;
    }
}

#endif // VEC_INCLUDED

// Vec.h
#ifndef VEC_INCLUDED
#define VEC_INCLUDED (1)

#include <stddef.h>
#include <stdbool.h>

/*========== ArrayList Type ===============================================*/

typedef struct {
    Slice     items;     // Current items as a slice
    size_t    capacity;  // Total allocated capacity
    Allocator allocator; // Memory allocator
} ArrayList;

// Helper functions for memory layout
static inline size_t grow_capacity(size_t current, size_t minimum) {
    size_t new_cap = current;
    while (new_cap < minimum) {
        new_cap = new_cap * 3 / 2 + 8;
    }
    return new_cap;
}

static inline bool ensure_capacity(ArrayList* list, size_t new_capacity) {
    if (list->capacity >= new_capacity) {
        return true;
    }

    size_t better_capacity = grow_capacity(list->capacity, new_capacity);
    size_t new_size        = better_capacity * core_ptr_size(list->items.core);
    size_t old_size        = list->capacity * core_ptr_size(list->items.core);

    void* new_items = list->allocator.realloc(
        list->allocator.ctx,
        core_ptr_raw(list->items.core),
        old_size,
        new_size
    );

    if (!new_items) {
        return false;
    }

    list->items = slice_create(
        new_items,
        list->items.len,
        core_ptr_size(list->items.core),
        core_ptr_align(list->items.core),
        false
    );
    list->capacity = better_capacity;
    return true;
}

/*========== ArrayList Operations =========================================*/

// Initialize new ArrayList
static inline ArrayList* arraylist_init(Allocator allocator, size_t elem_size, size_t elem_align) {
    ArrayList* list = allocator.alloc(allocator.ctx, sizeof(ArrayList));
    if (!list) {
        return NULL;
    }

    *list = (ArrayList){
        .items     = slice_create(NULL, 0, elem_size, elem_align, false),
        .capacity  = 0,
        .allocator = allocator
    };

    return list;
}

// Initialize with capacity
static inline ArrayList* arraylist_init_capacity(
    Allocator allocator,
    size_t    elem_size,
    size_t    elem_align,
    size_t    initial_capacity
) {
    ArrayList* list = arraylist_init(allocator, elem_size, elem_align);
    if (!list) {
        return NULL;
    }

    if (initial_capacity > 0) {
        void* new_items = allocator.alloc(allocator.ctx, initial_capacity * elem_size);
        if (!new_items) {
            allocator.dealloc(allocator.ctx, list);
            return NULL;
        }

        list->items    = slice_create(new_items, 0, elem_size, elem_align, false);
        list->capacity = initial_capacity;
    }

    return list;
}

// Deinitialize ArrayList
static inline void arraylist_deinit(ArrayList* list) {
    if (!list) {
        return;
    }
    if (core_ptr_raw(list->items.core)) {
        list->allocator.dealloc(
            list->allocator.ctx,
            core_ptr_raw(list->items.core)
        );
    }
    list->allocator.dealloc(list->allocator.ctx, list);
}

// Append item
static inline bool arraylist_append(ArrayList* list, const void* item) {
    if (!ensure_capacity(list, list->items.len + 1)) {
        return false;
    }

    void* dest = (char*)core_ptr_raw(list->items.core) + (list->items.len * core_ptr_size(list->items.core));
    memcpy(dest, item, core_ptr_size(list->items.core));
    list->items.len++;
    return true;
}

// Insert item at index
static inline bool arraylist_insert(ArrayList* list, size_t index, const void* item) {
    if (index > list->items.len) {
        return false;
    }
    if (!ensure_capacity(list, list->items.len + 1)) {
        return false;
    }

    void*  base      = core_ptr_raw(list->items.core);
    size_t elem_size = core_ptr_size(list->items.core);

    // Move items up to make space
    memmove(
        (char*)base + (index + 1) * elem_size,
        (char*)base + index * elem_size,
        (list->items.len - index) * elem_size
    );

    // Insert new item
    memcpy((char*)base + index * elem_size, item, elem_size);
    list->items.len++;
    return true;
}

// Remove item at index
static inline bool arraylist_remove(ArrayList* list, size_t index) {
    if (index >= list->items.len) {
        return false;
    }

    void*  base      = core_ptr_raw(list->items.core);
    size_t elem_size = core_ptr_size(list->items.core);

    // Move items down to fill gap
    memmove(
        (char*)base + index * elem_size,
        (char*)base + (index + 1) * elem_size,
        (list->items.len - index - 1) * elem_size
    );

    list->items.len--;
    return true;
}

// Swap remove (fast remove by swapping with last element)
static inline bool arraylist_swap_remove(ArrayList* list, size_t index) {
    if (index >= list->items.len) {
        return false;
    }

    void*  base      = core_ptr_raw(list->items.core);
    size_t elem_size = core_ptr_size(list->items.core);

    if (index != list->items.len - 1) {
        memcpy(
            (char*)base + index * elem_size,
            (char*)base + (list->items.len - 1) * elem_size,
            elem_size
        );
    }

    list->items.len--;
    return true;
}

// Pop last item
static inline bool arraylist_pop(ArrayList* list, void* out_item) {
    if (list->items.len == 0) {
        return false;
    }

    if (out_item) {
        memcpy(
            out_item,
            (char*)core_ptr_raw(list->items.core) + (list->items.len - 1) * core_ptr_size(list->items.core),
            core_ptr_size(list->items.core)
        );
    }

    list->items.len--;
    return true;
}

// Get slice of array
static inline Slice arraylist_as_slice(const ArrayList* list) {
    return list->items;
}

// Get mutable slice of array
static inline Slice arraylist_as_mut_slice(ArrayList* list) {
    return list->items;
}

// Append items from slice
static inline bool arraylist_append_slice(ArrayList* list, Slice items) {
    if (!ensure_capacity(list, list->items.len + items.len)) {
        return false;
    }

    memcpy(
        (char*)core_ptr_raw(list->items.core) + list->items.len * core_ptr_size(list->items.core),
        core_ptr_raw(items.core),
        items.len * core_ptr_size(items.core)
    );

    list->items.len += items.len;
    return true;
}

// Clear array (retain capacity)
static inline void arraylist_clear(ArrayList* list) {
    list->items.len = 0;
}

// Shrink capacity to length
static inline void arraylist_shrink(ArrayList* list) {
    if (list->items.len == list->capacity) {
        return;
    }

    size_t new_size  = list->items.len * core_ptr_size(list->items.core);
    void*  new_items = list->allocator.realloc(
        list->allocator.ctx,
        core_ptr_raw(list->items.core),
        list->capacity * core_ptr_size(list->items.core),
        new_size
    );

    if (new_items) {
        list->items = slice_create(
            new_items,
            list->items.len,
            core_ptr_size(list->items.core),
            core_ptr_align(list->items.core),
            false
        );
        list->capacity = list->items.len;
    }
}

#endif // VEC_INCLUDED

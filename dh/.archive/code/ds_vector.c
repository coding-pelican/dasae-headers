/**
 * @copyright Copyright 2024. Gyeongtae Kim All rights reserved.
 *
 * @file    vector.c
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2024-11-05 (date of creation)
 * @updated  2024-11-05 (date of last
update)
 * @version v1.0.0
 * @ingroup dasae-headers(dh)
 * @prefix  NONE
 *
 * @brief   Source of some software
 * @details Some detailed explanation
 */


/*========== Includes =======================================================*/

#include "dh/ds/vector.h"
#include "dh/mem.h"
#include "dh/debug/assert.h"

/*========== Macros and Definitions =========================================*/
/*========== Static Constant and Variable Definitions =======================*/
/*========== Extern Constant and Variable Definitions =======================*/
/*========== Static Function Prototypes =====================================*/
/*========== Static Function Implementations ================================*/
/*========== Extern Function Implementations ================================*/

usize ds_Vec__capacity(ds_Vec* vec) {
    return vec->capacity;
}

bool ds_Vec__append(ds_Vec* vec, anyptr item) {
    if (vec->container.length >= vec->capacity) {
        usize new_cap = vec->capacity * 2 + 8;
        if (!ds_Vec__ensureCapacity(vec, new_cap)) {
            return false;
        }
    }

    u8* dest = (u8*)vec->container.data + (vec->container.length * vec->container.elem_size);
    mem_copy(dest, item, vec->container.elem_size);
    vec->container.length++;
    return true;
}

void ds_Vec__appendAssumeCapacity(ds_Vec* vec, anyptr item) {
    debug_assert(vec->container.length < vec->capacity);
    u8* dest = (u8*)vec->container.data + (vec->container.length * vec->container.elem_size);
    mem_copy(dest, item, vec->container.elem_size);
    vec->container.length++;
}

bool ds_Vec__prepend(ds_Vec* vec, anyptr item) {
    if (vec->container.length >= vec->capacity) {
        usize new_cap = vec->capacity * 2 + 8;
        if (!ds_Vec__ensureCapacity(vec, new_cap)) {
            return false;
        }
    }

    // Move existing elements forward
    u8* src = vec->container.data;
    u8* dst = src + vec->container.elem_size;
    mem_move(dst, src, vec->container.length * vec->container.elem_size);

    // Insert new item at front
    mem_copy(src, item, vec->container.elem_size);
    vec->container.length++;
    return true;
}

void ds_Vec__prependAssumeCapacity(ds_Vec* vec, anyptr item) {
    debug_assert(vec->container.length < vec->capacity);

    u8* src = vec->container.data;
    u8* dst = src + vec->container.elem_size;
    mem_move(dst, src, vec->container.length * vec->container.elem_size);

    mem_copy(src, item, vec->container.elem_size);
    vec->container.length++;
}

anyptr ds_Vec__pop(ds_Vec* vec) {
    debug_assert(vec->container.length > 0);
    vec->container.length--;
    return (u8*)vec->container.data + (vec->container.length * vec->container.elem_size);
}

anyptr ds_Vec__popOrNull(ds_Vec* vec) {
    if (vec->container.length == 0) {
        return null;
    }
    return ds_Vec__pop(vec);
}

anyptr ds_Vec__shift(ds_Vec* vec) {
    debug_assert(vec->container.length > 0);

    // Store first element for return
    u8* first = mem_alloc(u8, vec->container.elem_size);
    mem_copy(first, vec->container.data, vec->container.elem_size);

    // Move remaining elements backward
    u8* dst = vec->container.data;
    u8* src = dst + vec->container.elem_size;
    mem_move(dst, src, (vec->container.length - 1) * vec->container.elem_size);

    vec->container.length--;
    return first;
}

anyptr ds_Vec__shiftOrNull(ds_Vec* vec) {
    if (vec->container.length == 0) {
        return null;
    }
    return ds_Vec__shift(vec);
}

bool ds_Vec__ensureCapacity(ds_Vec* vec, usize new_capacity) {
    if (vec->capacity >= new_capacity) {
        return true;
    }

    usize  new_size = new_capacity * vec->container.elem_size;
    anyptr new_data = mem_reallocate(vec->container.data, new_size);
    if (!new_data) {
        return false;
    }

    vec->container.data = new_data;
    vec->capacity       = new_capacity;
    return true;
}

bool ds_Vec__ensurePreciseCapacity(ds_Vec* vec, usize new_capacity) {
    if (vec->capacity == new_capacity) {
        return true;
    }

    anyptr new_data = mem_reallocate(vec->container.data, new_capacity * vec->container.elem_size);
    if (!new_data) {
        return false;
    }

    vec->container.data = new_data;
    vec->capacity       = new_capacity;
    return true;
}

void ds_Vec__expandToCapacity(ds_Vec* vec) {
    vec->container.length = vec->capacity;
}

bool ds_Vec__insert(ds_Vec* vec, usize index, anyptr item) {
    debug_assert(index <= vec->container.length);

    if (vec->container.length >= vec->capacity) {
        usize new_cap = vec->capacity * 2 + 8;
        if (!ds_Vec__ensureCapacity(vec, new_cap)) {
            return false;
        }
    }

    // Move elements after index forward
    u8* src = (u8*)vec->container.data + (index * vec->container.elem_size);
    u8* dst = src + vec->container.elem_size;
    mem_move(dst, src, (vec->container.length - index) * vec->container.elem_size);

    // Insert new item
    mem_copy(src, item, vec->container.elem_size);
    vec->container.length++;
    return true;
}

void ds_Vec__insertAssumeCapacity(ds_Vec* vec, usize index, anyptr item) {
    debug_assert(index <= vec->container.length);
    debug_assert(vec->container.length < vec->capacity);

    u8* src = (u8*)vec->container.data + (index * vec->container.elem_size);
    u8* dst = src + vec->container.elem_size;
    mem_move(dst, src, (vec->container.length - index) * vec->container.elem_size);

    mem_copy(src, item, vec->container.elem_size);
    vec->container.length++;
}

anyptr ds_Vec__orderedRemove(ds_Vec* vec, usize index) {
    debug_assert(index < vec->container.length);

    u8* removed = (u8*)vec->container.data + (index * vec->container.elem_size);
    u8* src     = removed + vec->container.elem_size;
    mem_move(removed, src, (vec->container.length - index - 1) * vec->container.elem_size);

    vec->container.length--;
    return removed;
}

anyptr ds_Vec__swapRemove(ds_Vec* vec, usize index) {
    debug_assert(index < vec->container.length);

    u8* removed = (u8*)vec->container.data + (index * vec->container.elem_size);
    u8* last    = (u8*)vec->container.data + ((vec->container.length - 1) * vec->container.elem_size);

    if (index != vec->container.length - 1) {
        mem_copy(removed, last, vec->container.elem_size);
    }

    vec->container.length--;
    return removed;
}

bool ds_Vec__resize(ds_Vec* vec, usize new_len) {
    if (new_len > vec->capacity) {
        if (!ds_Vec__ensureCapacity(vec, new_len)) {
            return false;
        }
    }
    vec->container.length = new_len;
    return true;
}

void ds_Vec__shrinkAndFree(ds_Vec* vec, usize new_len) {
    debug_assert(new_len <= vec->container.length);

    if (new_len < vec->container.length) {
        vec->container.length = new_len;
        ds_Vec__ensurePreciseCapacity(vec, new_len);
    }
}

void ds_Vec__shrinkRetainingCapacity(ds_Vec* vec, usize new_len) {
    debug_assert(new_len <= vec->container.length);
    vec->container.length = new_len;
}

anyptr ds_Vec__getFirst(ds_Vec* vec) {
    debug_assert(vec->container.length > 0);
    return vec->container.data;
}

anyptr ds_Vec__getFirstOrNull(ds_Vec* vec) {
    if (vec->container.length == 0) {
        return null;
    }
    return vec->container.data;
}

anyptr ds_Vec__getLast(ds_Vec* vec) {
    debug_assert(vec->container.length > 0);
    return (u8*)vec->container.data + ((vec->container.length - 1) * vec->container.elem_size);
}

anyptr ds_Vec__getLastOrNull(ds_Vec* vec) {
    if (vec->container.length == 0) {
        return null;
    }
    return (u8*)vec->container.data + ((vec->container.length - 1) * vec->container.elem_size);
}

/*========== Externalized Static Function Implementations (Unit Test) =======*/

#ifdef UNIT_TEST
#endif /* UNIT_TEST */

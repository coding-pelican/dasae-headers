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

#include <dh/ds/vector.h>
#include <dh/mem.h>
#include <dh/debug/assert.h>

/*========== Macros and Definitions =========================================*/
/*========== Static Constant and Variable Definitions =======================*/
/*========== Extern Constant and Variable Definitions =======================*/
/*========== Static Function Prototypes =====================================*/
/*========== Static Function Implementations ================================*/
/*========== Extern Function Implementations ================================*/

usize Vector__capacity(Vector* vec) {
    return vec->capacity;
}

bool Vector__append(Vector* vec, anyptr item) {
    if (vec->container.length >= vec->capacity) {
        usize new_cap = vec->capacity * 2 + 8;
        if (!Vector__ensureCapacity(vec, new_cap)) {
            return false;
        }
    }

    u8* dest = (u8*)vec->container.data + (vec->container.length * vec->container.elem_size);
    mem_copy(dest, item, vec->container.elem_size);
    vec->container.length++;
    return true;
}

void Vector__appendAssumeCapacity(Vector* vec, anyptr item) {
    debug_assert(vec->container.length < vec->capacity);
    u8* dest = (u8*)vec->container.data + (vec->container.length * vec->container.elem_size);
    mem_copy(dest, item, vec->container.elem_size);
    vec->container.length++;
}

bool Vector__prepend(Vector* vec, anyptr item) {
    if (vec->container.length >= vec->capacity) {
        usize new_cap = vec->capacity * 2 + 8;
        if (!Vector__ensureCapacity(vec, new_cap)) {
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

void Vector__prependAssumeCapacity(Vector* vec, anyptr item) {
    debug_assert(vec->container.length < vec->capacity);

    u8* src = vec->container.data;
    u8* dst = src + vec->container.elem_size;
    mem_move(dst, src, vec->container.length * vec->container.elem_size);

    mem_copy(src, item, vec->container.elem_size);
    vec->container.length++;
}

anyptr Vector__pop(Vector* vec) {
    debug_assert(vec->container.length > 0);
    vec->container.length--;
    return (u8*)vec->container.data + (vec->container.length * vec->container.elem_size);
}

anyptr Vector__popOrNull(Vector* vec) {
    if (vec->container.length == 0) {
        return null;
    }
    return Vector__pop(vec);
}

anyptr Vector__shift(Vector* vec) {
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

anyptr Vector__shiftOrNull(Vector* vec) {
    if (vec->container.length == 0) {
        return null;
    }
    return Vector__shift(vec);
}

bool Vector__ensureCapacity(Vector* vec, usize new_capacity) {
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

bool Vector__ensurePreciseCapacity(Vector* vec, usize new_capacity) {
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

void Vector__expandToCapacity(Vector* vec) {
    vec->container.length = vec->capacity;
}

bool Vector__insert(Vector* vec, usize index, anyptr item) {
    debug_assert(index <= vec->container.length);

    if (vec->container.length >= vec->capacity) {
        usize new_cap = vec->capacity * 2 + 8;
        if (!Vector__ensureCapacity(vec, new_cap)) {
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

void Vector__insertAssumeCapacity(Vector* vec, usize index, anyptr item) {
    debug_assert(index <= vec->container.length);
    debug_assert(vec->container.length < vec->capacity);

    u8* src = (u8*)vec->container.data + (index * vec->container.elem_size);
    u8* dst = src + vec->container.elem_size;
    mem_move(dst, src, (vec->container.length - index) * vec->container.elem_size);

    mem_copy(src, item, vec->container.elem_size);
    vec->container.length++;
}

anyptr Vector__orderedRemove(Vector* vec, usize index) {
    debug_assert(index < vec->container.length);

    u8* removed = (u8*)vec->container.data + (index * vec->container.elem_size);
    u8* src     = removed + vec->container.elem_size;
    mem_move(removed, src, (vec->container.length - index - 1) * vec->container.elem_size);

    vec->container.length--;
    return removed;
}

anyptr Vector__swapRemove(Vector* vec, usize index) {
    debug_assert(index < vec->container.length);

    u8* removed = (u8*)vec->container.data + (index * vec->container.elem_size);
    u8* last    = (u8*)vec->container.data + ((vec->container.length - 1) * vec->container.elem_size);

    if (index != vec->container.length - 1) {
        mem_copy(removed, last, vec->container.elem_size);
    }

    vec->container.length--;
    return removed;
}

bool Vector__resize(Vector* vec, usize new_len) {
    if (new_len > vec->capacity) {
        if (!Vector__ensureCapacity(vec, new_len)) {
            return false;
        }
    }
    vec->container.length = new_len;
    return true;
}

void Vector__shrinkAndFree(Vector* vec, usize new_len) {
    debug_assert(new_len <= vec->container.length);

    if (new_len < vec->container.length) {
        vec->container.length = new_len;
        Vector__ensurePreciseCapacity(vec, new_len);
    }
}

void Vector__shrinkRetainingCapacity(Vector* vec, usize new_len) {
    debug_assert(new_len <= vec->container.length);
    vec->container.length = new_len;
}

anyptr Vector__getFirst(Vector* vec) {
    debug_assert(vec->container.length > 0);
    return vec->container.data;
}

anyptr Vector__getFirstOrNull(Vector* vec) {
    if (vec->container.length == 0) {
        return null;
    }
    return vec->container.data;
}

anyptr Vector__getLast(Vector* vec) {
    debug_assert(vec->container.length > 0);
    return (u8*)vec->container.data + ((vec->container.length - 1) * vec->container.elem_size);
}

anyptr Vector__getLastOrNull(Vector* vec) {
    if (vec->container.length == 0) {
        return null;
    }
    return (u8*)vec->container.data + ((vec->container.length - 1) * vec->container.elem_size);
}

/*========== Externalized Static Function Implementations (Unit Test) =======*/

#ifdef UNIT_TEST
#endif /* UNIT_TEST */

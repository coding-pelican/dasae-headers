#include "dh/ds/Vec.h"
#include "dh/mem.h"
#include "dh/debug/assert.h"


ds_Vec* ds_Vec__init(ds_Vec* vec, usize elem_size) {
    vec->cap       = 0;
    vec->len       = 0;
    vec->elem_size = elem_size;
    vec->data      = null;
    return vec;
}

ds_Vec* ds_Vec__initWithCap(ds_Vec* vec, usize elem_size, usize cap) {
    vec->cap       = cap;
    vec->len       = 0;
    vec->elem_size = elem_size;
    vec->data      = mem_allocate(cap * elem_size);
    return vec;
}

ds_Vec* ds_Vec_fini(ds_Vec* vec) {
    mem_free(&vec->data);
    vec->data      = null;
    vec->cap       = 0;
    vec->len       = 0;
    vec->elem_size = 0;
    return vec;
}


usize ds_Vec_cap(const ds_Vec* vec) {
    return vec->cap;
}
usize ds_Vec_len(const ds_Vec* vec) {
    return vec->len;
}
bool ds_Vec_isEmpty(const ds_Vec* vec) {
    return vec->len == 0;
}
bool ds_Vec_isFull(const ds_Vec* vec) {
    return vec->len == vec->cap;
}


const anyptr ds_Vec_data(const ds_Vec* vec) {
    debug_assert(vec->data);
    return vec->data;
}
anyptr ds_Vec_mut_data(ds_Vec* vec) {
    debug_assert(vec->data);
    return vec->data;
}
const anyptr ds_Vec_dataOrNull(const ds_Vec* vec) {
    return vec->data;
}
anyptr ds_Vec_mut_dataOrNull(ds_Vec* vec) {
    return vec->data;
}


const u8* ds_Vec_dataAsBytes(const ds_Vec* vec) {
    debug_assert(vec->data);
    return vec->bytes;
}
u8* ds_Vec_mut_dataAsBytes(ds_Vec* vec) {
    debug_assert(vec->data);
    return vec->bytes;
}
const u8* ds_Vec_dataAsBytesOrNull(const ds_Vec* vec) {
    return vec->bytes;
}
u8* ds_Vec_mut_dataAsBytesOrNull(ds_Vec* vec) {
    return vec->bytes;
}


const anyptr ds_Vec_at(const ds_Vec* vec, usize index) {
    debug_assert(index < vec->len);
    return (u8*)vec->data + index * vec->elem_size;
}
anyptr ds_Vec_mut_at(ds_Vec* vec, usize index) {
    debug_assert(index < vec->len);
    return (u8*)vec->data + index * vec->elem_size;
}
const anyptr ds_Vec_atOrNull(const ds_Vec* vec, usize index) {
    if (vec->len <= index) { return null; }
    return (u8*)vec->data + index * vec->elem_size;
}
anyptr ds_Vec_mut_atOrNull(ds_Vec* vec, usize index) {
    if (vec->len <= index) { return null; }
    return (u8*)vec->data + index * vec->elem_size;
}


const anyptr ds_Vec_first(const ds_Vec* vec) {
    debug_assert(0 < vec->len);
    return vec->data;
}
anyptr ds_Vec_mut_first(ds_Vec* vec) {
    debug_assert(0 < vec->len);
    return vec->data;
}
const anyptr ds_Vec_firstOrNull(const ds_Vec* vec) {
    if (vec->len == 0) { return null; }
    return vec->data;
}
anyptr ds_Vec_mut_firstOrNull(ds_Vec* vec) {
    if (vec->len == 0) { return null; }
    return vec->data;
}


const anyptr ds_Vec_last(const ds_Vec* vec) {
    debug_assert(0 < vec->len);
    return (u8*)vec->data + (vec->len - 1) * vec->elem_size;
}
anyptr ds_Vec_mut_last(ds_Vec* vec) {
    debug_assert(0 < vec->len);
    return (u8*)vec->data + (vec->len - 1) * vec->elem_size;
}
const anyptr ds_Vec_lastOrNull(const ds_Vec* vec) {
    if (vec->len == 0) { return null; }
    return (u8*)vec->data + (vec->len - 1) * vec->elem_size;
}
anyptr ds_Vec_mut_lastOrNull(ds_Vec* vec) {
    if (vec->len == 0) { return null; }
    return (u8*)vec->data + (vec->len - 1) * vec->elem_size;
}


bool ds_Vec_reserve(ds_Vec* vec, usize new_cap) {
    if (new_cap <= vec->cap) {
        return true;
    }
    anyptr new_data = mem_reallocate(vec->data, new_cap * vec->elem_size);
    if (!new_data) {
        return false;
    }
    vec->data = new_data;
    vec->cap  = new_cap;
    return true;
}

bool ds_Vec_resize(ds_Vec* vec, usize new_len) {
    if (new_len > vec->cap) {
        if (!ds_Vec_reserve(vec, new_len)) {
            return false;
        }
    }
    if (new_len > vec->len) {
        mem_set((u8*)vec->data + vec->len * vec->elem_size, 0, (new_len - vec->len) * vec->elem_size);
    }
    vec->len = new_len;
    return true;
}


bool ds_Vec_append(ds_Vec* vec, const anyptr elem) {
    if (vec->len == vec->cap) {
        usize  new_cap  = vec->cap == 0 ? 1 : vec->cap * 2;
        anyptr new_data = mem_reallocate(vec->data, new_cap * vec->elem_size);
        if (!new_data) {
            return false;
        }
        vec->data = new_data;
        vec->cap  = new_cap;
    }
    mem_copy((u8*)vec->data + vec->len * vec->elem_size, elem, vec->elem_size);
    vec->len++;
    return true;
}

bool ds_Vec_prepend(ds_Vec* vec, const anyptr elem) {
    if (vec->len == vec->cap) {
        usize  new_cap  = vec->cap == 0 ? 1 : vec->cap * 2;
        anyptr new_data = mem_reallocate(vec->data, new_cap * vec->elem_size);
        if (!new_data) {
            return false;
        }
        vec->data = new_data;
        vec->cap  = new_cap;
    }
    mem_move((u8*)vec->data + vec->elem_size, vec->data, vec->len * vec->elem_size);
    mem_copy(vec->data, elem, vec->elem_size);
    vec->len++;
    return true;
}

bool ds_Vec_pop(ds_Vec* vec, anyptr const out_elem) {
    if (vec->len == 0) {
        return false;
    }
    vec->len--;
    if (out_elem) {
        mem_copy(out_elem, (u8*)vec->data + vec->len * vec->elem_size, vec->elem_size);
    }
    return true;
}

bool ds_Vec_shift(ds_Vec* vec, anyptr const out_elem) {
    if (vec->len == 0) {
        return false;
    }
    if (out_elem) {
        mem_copy(out_elem, vec->data, vec->elem_size);
    }
    mem_move(vec->data, (u8*)vec->data + vec->elem_size, (vec->len - 1) * vec->elem_size);
    vec->len--;
    return true;
}

bool ds_Vec_insert(ds_Vec* vec, usize index, const anyptr elem) {
    if (index > vec->len) {
        return false;
    }
    if (vec->len == vec->cap) {
        usize  new_cap  = vec->cap == 0 ? 1 : vec->cap * 2;
        anyptr new_data = mem_reallocate(vec->data, new_cap * vec->elem_size);
        if (!new_data) {
            return false;
        }
        vec->data = new_data;
        vec->cap  = new_cap;
    }
    mem_move((u8*)vec->data + (index + 1) * vec->elem_size, (u8*)vec->data + index * vec->elem_size, (vec->len - index) * vec->elem_size);
    mem_copy((u8*)vec->data + index * vec->elem_size, elem, vec->elem_size);
    vec->len++;
    return true;
}

bool ds_Vec_removeOrdered(ds_Vec* vec, usize index, anyptr const out_elem) {
    if (index >= vec->len) {
        return false;
    }
    if (out_elem) {
        mem_copy(out_elem, (u8*)vec->data + index * vec->elem_size, vec->elem_size);
    }
    mem_move((u8*)vec->data + index * vec->elem_size, (u8*)vec->data + (index + 1) * vec->elem_size, (vec->len - index - 1) * vec->elem_size);
    vec->len--;
    return true;
}

bool ds_Vec_removeSwap(ds_Vec* vec, usize index, anyptr const out_elem) {
    if (index >= vec->len) {
        return false;
    }
    if (out_elem) {
        mem_copy(out_elem, (u8*)vec->data + index * vec->elem_size, vec->elem_size);
    }
    if (index < vec->len - 1) {
        mem_copy((u8*)vec->data + index * vec->elem_size, (u8*)vec->data + (vec->len - 1) * vec->elem_size, vec->elem_size);
    }
    vec->len--;
    return true;
}

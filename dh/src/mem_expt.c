#include "dh/mem_expt.h"
#include <string.h>
#include <stdlib.h>

/* Default allocator implementation */
static anyptr mem_general_allocRaw(usize size) {
    return malloc(size);
}

static void mem_general_freeRaw(anyptr ptr) {
    free(ptr);
}

static anyptr mem_general_reallocRaw(anyptr ptr, usize size) {
    return realloc(ptr, size);
}

const Allocator mem_general = {
    .allocRaw   = mem_general_allocRaw,
    .freeRaw    = mem_general_freeRaw,
    .reallocRaw = mem_general_reallocRaw
};

/* Core memory operations */
Ptr mem_createSize(const Allocator* alloc, usize elem_size) {
    if (elem_size == 0) { return Ptr_null; }
    anyptr raw = alloc->allocRaw(elem_size);
    return Ptr_from(raw, elem_size);
}

void mem_destroy(const Allocator* alloc, Ptr ptr) {
    if (Ptr_isNull(ptr)) { return; }
    alloc->freeRaw(ptr.raw);
}

Ptr mem_allocSize(const Allocator* alloc, usize elem_size) {
    if (elem_size == 0) { return Ptr_null; }
    anyptr raw = alloc->allocRaw(elem_size);
    return Ptr_from(raw, elem_size);
}

Slice mem_allocSizeN(const Allocator* alloc, usize elem_size, usize count) {
    if (elem_size == 0 || count == 0) { return Slice_null; }
    anyptr raw = alloc->allocRaw(elem_size * count);
    return Slice_fromRaw(raw, elem_size, count);
}

void mem_free(const Allocator* alloc, Ptr ptr) {
    if (Ptr_isNull(ptr)) { return; }
    alloc->freeRaw(ptr.raw);
}

void mem_freeSlice(const Allocator* alloc, Slice slice) {
    if (Slice_isNull(slice)) { return; }
    alloc->freeRaw(slice.ptr.raw);
}

Slice mem_reallocSize(const Allocator* alloc, Slice slice, usize new_count) {
    if (Slice_isNull(slice)) {
        return slice;
    }

    anyptr new_raw = alloc->reallocRaw(slice.ptr.raw, slice.ptr.elem_size * new_count);
    return Slice_fromRaw(new_raw, slice.ptr.elem_size, new_count);
}

/* Raw memory operations */
anyptr mem_allocRaw(const Allocator* alloc, usize size) {
    return alloc->allocRaw(size);
}

void mem_freeRaw(const Allocator* alloc, anyptr ptr) {
    if (!ptr) { return; }
    alloc->freeRaw(ptr);
}

anyptr mem_reallocRaw(const Allocator* alloc, anyptr ptr, usize new_size) {
    return alloc->reallocRaw(ptr, new_size);
}

/* Memory manipulation */
void mem_copySize(Ptr dest, Ptr src, usize size) {
    if (Ptr_isNull(dest) || Ptr_isNull(src)) {
        return;
    }
    if (!Ptr_hasMinSize(dest, size)
        || !Ptr_hasMinSize(src, size)) {
        return;
    }
    memcpy(dest.raw, src.raw, size);
}

void mem_moveSize(Ptr dest, Ptr src, usize size) {
    if (Ptr_isNull(dest) || Ptr_isNull(src)) {
        return;
    }
    if (!Ptr_hasMinSize(dest, size)
        || !Ptr_hasMinSize(src, size)) {
        return;
    }
    memmove(dest.raw, src.raw, size);
}

void mem_setSize(Ptr ptr, i32 val, usize size) {
    if (Ptr_isNull(ptr) || !Ptr_hasMinSize(ptr, size)) {
        return;
    }
    memset(ptr.raw, val, size);
}

i32 mem_cmpSize(Ptr lhs, Ptr rhs, usize size) {
    if (Ptr_isNull(lhs) || Ptr_isNull(rhs)) { return 0; }
    if (!Ptr_hasMinSize(lhs, size)
        || !Ptr_hasMinSize(rhs, size)) { return 0; }
    return memcmp(lhs.raw, rhs.raw, size);
}

/* Ptr operations */
Ptr Ptr_from(anyptr raw, usize elem_size) {
    return (Ptr){
        .raw       = raw,
        .elem_size = elem_size
    };
}

bool Ptr_isNull(Ptr p) {
    return p.raw == null;
}

usize Ptr_elemSize(Ptr p) {
    return p.elem_size;
}

anyptr Ptr_raw(Ptr p) {
    return p.raw;
}

anyptr Ptr_at(Ptr p, usize idx) {
    if (Ptr_isNull(p)) { return null; }
    return (u8*)p.raw + (idx * p.elem_size);
}

bool Ptr_hasMinSize(Ptr p, usize required_size) {
    return !Ptr_isNull(p) && p.elem_size >= required_size;
}

/* Slice operations */
Slice Slice_from(Ptr ptr, usize len) {
    return (Slice){
        .ptr = ptr,
        .len = len
    };
}

Slice Slice_fromRaw(anyptr raw, usize elem_size, usize len) {
    return Slice_from(Ptr_from(raw, elem_size), len);
}

bool Slice_isNull(Slice sl) {
    return Ptr_isNull(sl.ptr);
}

bool Slice_isEmpty(Slice sl) {
    return sl.len == 0;
}

usize Slice_len(Slice sl) {
    return sl.len;
}

usize Slice_elemSize(Slice sl) {
    return Ptr_elemSize(sl.ptr);
}

Ptr Slice_ptr(Slice sl) {
    return sl.ptr;
}

anyptr Slice_at(Slice sl, usize idx) {
    if (Slice_isNull(sl) || idx >= sl.len) { return null; }
    return Ptr_at(sl.ptr, idx);
}

Slice Slice_subslice(Slice sl, usize start, usize end) {
    if (start >= sl.len || end > sl.len || start > end) { return Slice_null; }

    return (Slice){
        .ptr = Ptr_from(
            (u8*)sl.ptr.raw + (start * sl.ptr.elem_size),
            sl.ptr.elem_size
        ),
        .len = end - start
    };
}

Slice Slice_prefix(Slice sl, usize end) {
    return Slice_subslice(sl, 0, end);
}

Slice Slice_suffix(Slice sl, usize start) {
    return Slice_subslice(sl, start, sl.len);
}

/* Type checking */
bool mem_isTypeSize(usize elem_size, usize type_size) {
    return elem_size == type_size;
}

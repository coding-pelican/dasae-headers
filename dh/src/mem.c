/**
 * @copyright Copyright 2024. Gyeongtae Kim All rights reserved.
 *
 * @file    mem.c
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2024-11-20 (date of creation)
 * @updated 2024-11-21 (date of last update)
 * @version v1.0.0
 * @ingroup dasae-headers(dh)
 * @prefix  mem
 *
 * @brief   Memory management with Zig-style semantics
 * @details Provides type-safe memory management with clear separation between
 *          single-element pointers (Ptr) and multi-element views (Slice).
 *          Replaces anyptr with type-safe alternatives.
 */

#include "dh/claim.h"
#include "dh/mem.h"

#include <stdlib.h>
#include <string.h>

/*========== Default Allocator Implementation ==============================*/

static anyptr mem_general_allocRaw(usize size) {
    return malloc(size);
}

static void mem_general_freeRaw(anyptr raw) {
    free(raw);
}

static anyptr mem_general_reallocRaw(anyptr raw, usize size) {
    return realloc(raw, size);
}

const Allocator mem_general = {
    .allocRaw   = mem_general_allocRaw,
    .freeRaw    = mem_general_freeRaw,
    .reallocRaw = mem_general_reallocRaw
};

/*========== Core Memory Operations ========================================*/

Ptr mem_createSize(const Allocator* alloc, usize elem_size) {
    claim_assert_nonnull(alloc);
    claim_assert_nonnull(alloc->allocRaw);

    if (elem_size == 0) {
        return Ptr_null;
    }

    anyptr raw = alloc->allocRaw(elem_size);
    return Ptr_from(raw, elem_size);
}

void mem_destroy(const Allocator* alloc, Ptr ptr) {
    claim_assert_nonnull(alloc);
    claim_assert_nonnull(alloc->freeRaw);

    if (!Ptr_isNull(ptr)) {
        alloc->freeRaw(ptr.raw);
    }
}

Slice mem_allocSize(const Allocator* alloc, usize elem_size, usize count) {
    claim_assert_nonnull(alloc);
    claim_assert_nonnull(alloc->allocRaw);

    if (elem_size == 0 || count == 0) {
        return Slice_null;
    }

    anyptr raw = alloc->allocRaw(elem_size * count);
    return Slice_fromRaw(raw, elem_size, count);
}

void mem_free(const Allocator* alloc, Slice slice) {
    claim_assert_nonnull(alloc);
    claim_assert_nonnull(alloc->freeRaw);

    if (!Slice_isNull(slice)) {
        alloc->freeRaw(slice.raw);
    }
}

Slice mem_reallocSize(const Allocator* alloc, Slice slice, usize elem_size, usize new_count) {
    claim_assert_nonnull(alloc);
    claim_assert_nonnull(alloc->reallocRaw);

    if (Slice_isNull(slice) || elem_size == 0) {
        return Slice_null;
    }

    if (new_count == 0) {
        mem_free(alloc, slice);
        return Slice_null;
    }

    anyptr new_raw = alloc->reallocRaw(slice.raw, elem_size * new_count);
    return Slice_fromRaw(new_raw, elem_size, new_count);
}

anyptr mem_allocRaw(const Allocator* alloc, usize size) {
    claim_assert_nonnull(alloc);
    claim_assert_nonnull(alloc->allocRaw);
    return alloc->allocRaw(size);
}

void mem_freeRaw(const Allocator* alloc, anyptr raw) {
    claim_assert_nonnull(alloc);
    claim_assert_nonnull(alloc->freeRaw);
    if (raw != null) {
        alloc->freeRaw(raw);
    }
}

anyptr mem_reallocRaw(const Allocator* alloc, anyptr raw, usize new_size) {
    claim_assert_nonnull(alloc);
    claim_assert_nonnull(alloc->reallocRaw);
    return alloc->reallocRaw(raw, new_size);
}

/*========== Memory Manipulation ===========================================*/

// TODO: Separate these functions with Ptr and Slice

void mem_copySize(Ptr dest, Ptr src, usize size) {
    if (Ptr_isNull(dest) || Ptr_isNull(src)) {
        return;
    }
    // claim_assert(Ptr_hasMinSize(dest, size));
    // claim_assert(Ptr_hasMinSize(src, size));
    mem_copyRaw(dest.raw, src.raw, size);
}

void mem_moveSize(Ptr dest, Ptr src, usize size) {
    if (Ptr_isNull(dest) || Ptr_isNull(src)) {
        return;
    }
    // claim_assert(Ptr_hasMinSize(dest, size));
    // claim_assert(Ptr_hasMinSize(src, size));
    mem_moveRaw(dest.raw, src.raw, size);
}

void mem_setSize(Ptr ptr, i32 val, usize size) {
    if (Ptr_isNull(ptr)) {
        return;
    }
    // claim_assert(Ptr_hasMinSize(ptr, size));
    mem_setRaw(ptr.raw, val, size);
}

i32 mem_cmpSize(Ptr lhs, Ptr rhs, usize size) {
    if (Ptr_isNull(lhs) || Ptr_isNull(rhs)) {
        return 0;
    }
    // claim_assert(Ptr_hasMinSize(lhs, size));
    // claim_assert(Ptr_hasMinSize(rhs, size));
    return mem_cmpRaw(lhs.raw, rhs.raw, size);
}

void mem_copyRaw(anyptr dest, anyptr src, usize size) {
    if (dest && src && size > 0) {
        memcpy(dest, src, size);
    }
}

void mem_moveRaw(anyptr dest, anyptr src, usize size) {
    if (dest && src && size > 0) {
        memmove(dest, src, size);
    }
}

void mem_setRaw(anyptr raw, i32 val, usize size) {
    if (raw && size > 0) {
        memset(raw, val, size);
    }
}

i32 mem_cmpRaw(anyptr lhs, anyptr rhs, usize size) {
    if (!lhs || !rhs) {
        return 0;
    }
    return memcmp(lhs, rhs, size);
}

/*========== Ptr Operations ==============================================*/

Ptr Ptr_from(anyptr raw, usize elem_size) {
    if (!raw || elem_size == 0) {
        return Ptr_null;
    }
    return (Ptr){ raw, elem_size };
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

u8* Ptr_bytes(Ptr p) {
    return (u8*)p.raw;
}

Ptr Ptr_offset(Ptr p, usize count) {
    if (Ptr_isNull(p)) {
        return Ptr_null;
    }
    return Ptr_from((u8*)p.raw + (count * p.elem_size), p.elem_size);
}

Ptr Ptr_byteOffset(Ptr p, usize byte_offset) {
    if (Ptr_isNull(p)) {
        return Ptr_null;
    }
    return Ptr_from((u8*)p.raw + byte_offset, p.elem_size);
}

bool Ptr_hasMinSize(Ptr p, usize required_size) {
    return !Ptr_isNull(p) && p.elem_size >= required_size;
}

/*========== Slice Operations ===========================================*/

Slice Slice_from(Ptr ptr, usize len) {
    if (Ptr_isNull(ptr)) {
        return Slice_null;
    }
    return (Slice){ .ptr = ptr, .len = len };
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
    return sl.ptr.elem_size;
}

Ptr Slice_ptr(Slice sl) {
    return sl.ptr;
}

anyptr Slice_raw(Slice sl) {
    return sl.raw;
}

u8* Slice_bytes(Slice sl) {
    return (u8*)sl.raw;
}

anyptr Slice_at(Slice sl, usize idx) {
    if (Slice_isNull(sl) || idx >= sl.len) {
        return null;
    }
    return (u8*)sl.raw + (idx * sl.elem_size);
}

u8* Slice_byteAt(Slice sl, usize byte_offset) {
    if (Slice_isNull(sl)) {
        return null;
    }
    return (u8*)sl.raw + byte_offset;
}

Slice Slice_subslice(Slice sl, usize start, usize end) {
    if (start >= sl.len || end > sl.len || start > end) {
        return Slice_null;
    }
    return Slice_fromRaw(
        (u8*)sl.raw + (start * sl.elem_size),
        sl.elem_size,
        end - start
    );
}

Slice Slice_byteSlice(Slice sl, usize start, usize end) {
    usize total_size = sl.len * sl.elem_size;
    if (start >= total_size || end > total_size || start > end) {
        return Slice_null;
    }
    return Slice_fromRaw(
        (u8*)sl.raw + start,
        sl.elem_size,
        (end - start) / sl.elem_size
    );
}

Slice Slice_prefix(Slice sl, usize end) {
    return Slice_subslice(sl, 0, end);
}

Slice Slice_suffix(Slice sl, usize start) {
    return Slice_subslice(sl, start, sl.len);
}

/*========== Type Checking ==============================================*/

bool mem_isTypeSize(usize elem_size, usize type_size) {
    return elem_size == type_size;
}

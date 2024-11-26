/**
 * @copyright Copyright 2024. Gyeongtae Kim All rights reserved.
 */

#include "dh/claim.h"
#include "dh/claim/assert.h"
#include "dh/mem.h"

#include <stdlib.h>
#include <string.h>

/*========== Default mem_Allocator Implementation ===========================*/

static anyptr mem_general_allocRaw(usize size) {
    return malloc(size);
}

static void mem_general_freeRaw(anyptr raw) {
    if (raw != null) {
        free(raw);
    }
}

static anyptr mem_general_reallocRaw(anyptr raw, usize size) {
    return realloc(raw, size);
}

const mem_Allocator mem_general = {
    .allocRaw   = mem_general_allocRaw,
    .freeRaw    = mem_general_freeRaw,
    .reallocRaw = mem_general_reallocRaw
};

/*========== Core Memory Operations =========================================*/

Ptr mem_createSize(const mem_Allocator* alloc, usize elem_size) {
    claim_assert_fmt(alloc != null, "Allocator cannot be null");
    claim_assert_fmt(alloc->allocRaw != null, "Allocator must implement allocRaw");
    claim_assert_fmt(elem_size > 0, "Element size must be positive");

    anyptr raw = alloc->allocRaw(elem_size);
    claim_assert_fmt(raw != null, "Allocation failed");

    return Ptr_fromRaw(raw, elem_size);
}

bool mem_tryCreateSize(const mem_Allocator* alloc, usize elem_size, Ptr* out_ptr) {
    if (alloc == null
        || alloc->allocRaw == null
        || elem_size == 0
        || out_ptr == null) {
        return false;
    }

    anyptr raw = alloc->allocRaw(elem_size);
    if (raw == null) { return false; }

    *out_ptr = Ptr_fromRaw(raw, elem_size);
    return true;
}

Ptr mem_createSizeOrNull(const mem_Allocator* alloc, usize elem_size) {
    if (alloc == null || alloc->allocRaw == null || elem_size == 0) {
        return Ptr_null;
    }

    anyptr raw = alloc->allocRaw(elem_size);
    if (raw == null) {
        return Ptr_null;
    }

    return Ptr_fromRaw(raw, elem_size);
}

void mem_destroy(const mem_Allocator* alloc, Ptr* ptr) {
    claim_assert_fmt(alloc != null, "Allocator cannot be null");
    claim_assert_fmt(alloc->freeRaw != null, "Allocator must implement freeRaw");
    claim_assert_fmt(ptr != null, "Ptr pointer cannot be null");

    if (!Ptr_isNull(*ptr)) {
        anyptr raw = Ptr_raw(*ptr); // Get raw pointer first
        alloc->freeRaw(raw);        // Free the raw pointer
        *ptr = Ptr_null;            // Set to null after freeing
    }
}

Slice mem_allocSize(const mem_Allocator* alloc, usize elem_size, usize count) {
    claim_assert_fmt(alloc != null, "Allocator cannot be null");
    claim_assert_fmt(alloc->allocRaw != null, "Allocator must implement allocRaw");
    claim_assert_fmt(elem_size > 0, "Element size must be positive");
    claim_assert_fmt(count > 0, "Count must be positive");

    anyptr raw = alloc->allocRaw(elem_size * count);
    claim_assert_fmt(raw != null, "Allocation failed");

    return Slice_fromRaw(raw, elem_size, count);
}

bool mem_tryAllocSize(const mem_Allocator* alloc, usize elem_size, usize count, Slice* out_slice) {
    if (alloc == null
        || alloc->allocRaw == null
        || elem_size == 0
        || count == 0
        || out_slice == null) {
        return false;
    }

    anyptr raw = alloc->allocRaw(elem_size * count);
    if (raw == null) { return false; }

    *out_slice = Slice_fromPtr(Ptr_fromRaw(raw, elem_size), count);
    return true;
}

Slice mem_allocSizeOrNull(const mem_Allocator* alloc, usize elem_size, usize count) {
    if (alloc == null
        || alloc->allocRaw == null
        || elem_size == 0
        || count == 0) {
        return Slice_null;
    }

    anyptr raw = alloc->allocRaw(elem_size * count);
    if (raw == null) { return Slice_null; }

    return Slice_fromPtr(Ptr_fromRaw(raw, elem_size), count);
}

void mem_free(const mem_Allocator* alloc, Slice* slice) {
    claim_assert_fmt(alloc != null, "Allocator cannot be null");
    claim_assert_fmt(alloc->freeRaw != null, "Allocator must implement freeRaw");
    claim_assert_fmt(!Slice_isNull(*slice), "Slice cannot be null");

    if (!Slice_isNull(*slice)) {
        anyptr ptr = Slice_raw(*slice); // Get raw pointer first
        alloc->freeRaw(ptr);            // Free the raw pointer
        *slice = Slice_null;            // Set to null after freeing
    }
}

Slice mem_reallocSize(const mem_Allocator* alloc, Slice slice, usize elem_size, usize new_count) {
    claim_assert_fmt(alloc != null, "Allocator cannot be null");
    claim_assert_fmt(alloc->reallocRaw != null, "Allocator must implement reallocRaw");
    claim_assert_fmt(!Slice_isNull(slice), "Slice cannot be null");
    claim_assert_fmt(elem_size > 0, "Element size must be positive");
    claim_assert_fmt(new_count > 0, "New count must be positive");

    anyptr new_raw = alloc->reallocRaw(Slice_raw(slice), elem_size * new_count);
    claim_assert_fmt(new_raw != null, "Reallocation failed");

    return Slice_fromRaw(new_raw, elem_size, new_count);
}

bool mem_tryReallocSize(const mem_Allocator* alloc, Slice slice, usize elem_size, usize new_count, Slice* out_slice) {
    if (alloc == null
        || alloc->reallocRaw == null
        || Slice_isNull(slice)
        || elem_size == 0
        || new_count == 0
        || out_slice == null) {
        return false;
    }

    anyptr new_raw = alloc->reallocRaw(Slice_raw(slice), elem_size * new_count);
    if (new_raw == null) { return false; }

    *out_slice = Slice_fromRaw(new_raw, elem_size, new_count);
    return true;
}

/*========== Raw Memory Operations (when needed) ============================*/

anyptr mem_allocRaw(const mem_Allocator* alloc, usize size) {
    claim_assert_fmt(alloc != null, "Allocator cannot be null");
    claim_assert_fmt(alloc->allocRaw != null, "Allocator must implement allocRaw");
    claim_assert_fmt(size > 0, "Size must be positive");

    anyptr raw = alloc->allocRaw(size);
    claim_assert_fmt(raw != null, "Allocation failed");

    return raw;
}

void mem_freeRaw(const mem_Allocator* alloc, anyptr* raw) {
    claim_assert_fmt(alloc != null, "Allocator cannot be null");
    claim_assert_fmt(alloc->freeRaw != null, "Allocator must implement freeRaw");
    claim_assert_fmt(raw != null, "Raw pointer cannot be null");

    if (*raw != null) {
        alloc->freeRaw(*raw); // Free the raw pointer
        *raw = null;          // Set to null after freeing
    }
}

anyptr mem_reallocRaw(const mem_Allocator* alloc, anyptr raw, usize new_size) {
    claim_assert_fmt(alloc != null, "Allocator cannot be null");
    claim_assert_fmt(alloc->reallocRaw != null, "Allocator must implement reallocRaw");
    claim_assert_fmt(raw != null, "Raw pointer cannot be null");
    claim_assert_fmt(new_size > 0, "New size must be positive");

    anyptr new_raw = alloc->reallocRaw(raw, new_size);
    claim_assert_fmt(new_raw != null, "Reallocation failed");

    return new_raw;
}

/*========== Memory Manipulation ============================================*/

void mem_copySize(Ptr dest, Ptr src, usize size) {
    claim_assert_fmt(!Ptr_isNull(dest), "Destination cannot be null");
    claim_assert_fmt(!Ptr_isNull(src), "Source cannot be null");
    claim_assert_fmt(Ptr_hasMinSize(dest, size), "Destination too small");
    claim_assert_fmt(Ptr_hasMinSize(src, size), "Source too small");

    memcpy(Ptr_raw(dest), Ptr_raw(src), size);
}

bool mem_tryCopySize(Ptr dest, Ptr src, usize size) {
    if (Ptr_isNull(dest)
        || Ptr_isNull(src)
        || !Ptr_hasMinSize(dest, size)
        || !Ptr_hasMinSize(src, size)) {
        return false;
    }

    memcpy(Ptr_raw(dest), Ptr_raw(src), size);
    return true;
}

void mem_moveSize(Ptr dest, Ptr src, usize size) {
    claim_assert_fmt(!Ptr_isNull(dest), "Destination cannot be null");
    claim_assert_fmt(!Ptr_isNull(src), "Source cannot be null");
    claim_assert_fmt(Ptr_hasMinSize(dest, size), "Destination too small");
    claim_assert_fmt(Ptr_hasMinSize(src, size), "Source too small");

    memmove(Ptr_raw(dest), Ptr_raw(src), size);
}

bool mem_tryMoveSize(Ptr dest, Ptr src, usize size) {
    if (Ptr_isNull(dest)
        || Ptr_isNull(src)
        || !Ptr_hasMinSize(dest, size)
        || !Ptr_hasMinSize(src, size)) {
        return false;
    }

    memmove(Ptr_raw(dest), Ptr_raw(src), size);
    return true;
}

void mem_setSize(Ptr ptr, i32 value, usize size) {
    claim_assert_fmt(!Ptr_isNull(ptr), "Pointer cannot be null");
    claim_assert_fmt(Ptr_hasMinSize(ptr, size), "Pointer too small");

    memset(Ptr_raw(ptr), value, size);
}

bool mem_trySetSize(Ptr ptr, i32 value, usize size) {
    if (Ptr_isNull(ptr) || !Ptr_hasMinSize(ptr, size)) {
        return false;
    }

    memset(Ptr_raw(ptr), value, size);
    return true;
}

i32 mem_cmpSize(Ptr lhs, Ptr rhs, usize size) {
    claim_assert_fmt(
        !Ptr_isNull(lhs),
        "Left pointer cannot be null"
    );
    claim_assert_fmt(
        !Ptr_isNull(rhs),
        "Right pointer cannot be null"
    );
    claim_assert_fmt(
        Ptr_hasMinSize(lhs, size),
        "Left pointer too small"
    );
    claim_assert_fmt(
        Ptr_hasMinSize(rhs, size),
        "Right pointer too small"
    );

    return memcmp(
        Ptr_raw(lhs),
        Ptr_raw(rhs),
        size
    );
}

bool mem_tryCmpSize(Ptr lhs, Ptr rhs, usize size, i32* out_result) {
    if (Ptr_isNull(lhs)
        || Ptr_isNull(rhs)
        || !Ptr_hasMinSize(lhs, size)
        || !Ptr_hasMinSize(rhs, size)
        || out_result == null) {
        return false;
    }

    *out_result = memcmp(
        Ptr_raw(lhs),
        Ptr_raw(rhs),
        size
    );
    return true;
}

/*========== Slice Memory Operations ========================================*/

void mem_copySlice(Slice dest, Slice src) {
    claim_assert_fmt(
        !Slice_isNull(dest),
        "Destination slice cannot be null"
    );
    claim_assert_fmt(
        !Slice_isNull(src),
        "Source slice cannot be null"
    );
    claim_assert_fmt(
        Slice_len(dest) >= Slice_len(src),
        "Destination slice too small (%zu < %zu)",
        Slice_len(dest),
        Slice_len(src)
    );
    claim_assert_fmt(
        Slice_elemSize(dest) == Slice_elemSize(src),
        "Element size mismatch (%zu != %zu)",
        Slice_elemSize(dest),
        Slice_elemSize(src)
    );

    memcpy(
        Slice_raw(dest),
        Slice_raw(src),
        Slice_len(src) * Slice_elemSize(src)
    );
}


bool mem_tryCopySlice(Slice dest, Slice src) {
    if (Slice_isNull(dest)
        || Slice_isNull(src)
        || Slice_len(dest) < Slice_len(src)
        || Ptr_size(dest.ptr_) != Ptr_size(src.ptr_)) {
        return false;
    }

    memcpy(
        Slice_raw(dest),
        Slice_raw(src),
        Slice_len(src) * Ptr_size(src.ptr_)
    );
    return true;
}

void mem_moveSlice(Slice dest, Slice src) {
    claim_assert_fmt(
        !Slice_isNull(dest),
        "Destination slice cannot be null"
    );
    claim_assert_fmt(
        !Slice_isNull(src),
        "Source slice cannot be null"
    );
    claim_assert_fmt(
        Slice_len(dest) >= Slice_len(src),
        "Destination slice too small (%zu < %zu)",
        Slice_len(dest),
        Slice_len(src)
    );
    claim_assert_fmt(
        Ptr_size(dest.ptr_) == Ptr_size(src.ptr_),
        "Element size mismatch (%zu != %zu)",
        Ptr_size(dest.ptr_),
        Ptr_size(src.ptr_)
    );

    memmove(
        Slice_raw(dest),
        Slice_raw(src),
        Slice_len(src) * Ptr_size(src.ptr_)
    );
}

bool mem_tryMoveSlice(Slice dest, Slice src) {
    if (Slice_isNull(dest)
        || Slice_isNull(src)
        || Slice_len(dest) < Slice_len(src)
        || Ptr_size(dest.ptr_) != Ptr_size(src.ptr_)) {
        return false;
    }

    memmove(
        Slice_raw(dest),
        Slice_raw(src),
        Slice_len(src) * Ptr_size(src.ptr_)
    );
    return true;
}

void mem_setSlice(Slice slice, i32 value) {
    claim_assert_fmt(!Slice_isNull(slice), "Slice cannot be null");

    memset(
        Slice_raw(slice),
        value,
        Slice_len(slice) * Ptr_size(slice.ptr_)
    );
}

bool mem_trySetSlice(Slice slice, i32 value) {
    if (Slice_isNull(slice)) { return false; }

    memset(
        Slice_raw(slice),
        value,
        Slice_len(slice) * Ptr_size(slice.ptr_)
    );
    return true;
}

i32 mem_cmpSlice(Slice lhs, Slice rhs) {
    claim_assert_fmt(
        !Slice_isNull(lhs),
        "Left slice cannot be null"
    );
    claim_assert_fmt(
        !Slice_isNull(rhs),
        "Right slice cannot be null"
    );
    claim_assert_fmt(
        Ptr_size(lhs.ptr_) == Ptr_size(rhs.ptr_),
        "Element size mismatch (%zu != %zu)",
        Ptr_size(lhs.ptr_),
        Ptr_size(rhs.ptr_)
    );

    // First compare by content up to the shorter length
    usize min_len
        = Slice_len(lhs) < Slice_len(rhs)
            ? Slice_len(lhs)
            : Slice_len(rhs);

    i32 result = memcmp(
        Slice_raw(lhs),
        Slice_raw(rhs),
        min_len * Ptr_size(lhs.ptr_)
    );

    if (result != 0) { return result; }

    // If contents are equal up to min_len, shorter slice is "less than"
    return (i32)(Slice_len(lhs) - Slice_len(rhs));
}

bool mem_tryCmpSlice(Slice lhs, Slice rhs, i32* out_result) {
    if (Slice_isNull(lhs)
        || Slice_isNull(rhs)
        || Ptr_size(lhs.ptr_) != Ptr_size(rhs.ptr_)
        || out_result == null) {
        return false;
    }

    usize min_len
        = Slice_len(lhs) < Slice_len(rhs)
            ? Slice_len(lhs)
            : Slice_len(rhs);

    i32 result = memcmp(
        Slice_raw(lhs),
        Slice_raw(rhs),
        min_len * Ptr_size(lhs.ptr_)
    );

    if (result != 0) {
        *out_result = result;
        return true;
    }

    *out_result = (i32)(Slice_len(lhs) - Slice_len(rhs));
    return true;
}

/*========== Type Checking ==================================================*/

bool mem_isTypeSize(usize elem_size, usize type_size) {
    return elem_size == type_size;
}

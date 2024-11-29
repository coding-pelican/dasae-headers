/**
 * @copyright Copyright 2024. Gyeongtae Kim All rights reserved.
 */

#include "dh/claim.h"
#include "dh/claim/assert.h"
#include "dh/core.h"
#include "dh/mem.h"

#include <stdlib.h>
#include <string.h>

/*========== Default mem_Allocator Implementation ===========================*/

static anyptr mem_general_allocRaw(usize size) {
    return malloc(size);
}

static void mem_general_freeRaw(anyptr_mut raw) {
    if (raw != null) {
        free(raw);
    }
}

static anyptr mem_general_reallocRaw(anyptr_mut raw, usize size) {
    return realloc(raw, size);
}

const mem_Allocator mem_general = {
    .allocRaw   = mem_general_allocRaw,
    .freeRaw    = mem_general_freeRaw,
    .reallocRaw = mem_general_reallocRaw
};

/*========== Core Memory Operations =========================================*/

sptr mem_createSize(const mem_Allocator* alloc, usize elem_size) {
    claim_assert_fmt(alloc != null, "Allocator cannot be null");
    claim_assert_fmt(alloc->allocRaw != null, "Allocator must implement allocRaw");
    claim_assert_fmt(elem_size > 0, "Element size must be positive");

    anyptr raw = alloc->allocRaw(elem_size);
    claim_assert_fmt(raw != null, "Allocation failed");

    return sptr_make(elem_size, raw);
}

bool mem_tryCreateSize(const mem_Allocator* alloc, usize elem_size, sptr* out_ptr) {
    if (alloc == null
        || alloc->allocRaw == null
        || elem_size == 0
        || out_ptr == null) {
        return false;
    }

    anyptr raw = alloc->allocRaw(elem_size);
    if (raw == null) { return false; }

    *out_ptr = sptr_make(elem_size, raw);
    return true;
}

// sptr mem_createSizeOrNull(const mem_Allocator* alloc, usize elem_size) {
//     if (alloc == null || alloc->allocRaw == null || elem_size == 0) {
//         return sptr_null;
//     }

//     anyptr raw = alloc->allocRaw(elem_size);
//     if (raw == null) {
//         return sptr_null;
//     }

//     return sptr_make(elem_size, raw);
// }

void mem_destroy(const mem_Allocator* alloc, sptr* ptr) {
    claim_assert_fmt(alloc != null, "Allocator cannot be null");
    claim_assert_fmt(alloc->freeRaw != null, "Allocator must implement freeRaw");
    claim_assert_fmt(ptr != null, "sptr pointer cannot be null");

    if (!sptr_isValid(*ptr)) {
        anyptr_mut raw = sptr_raw_mut(ptr); // Get raw pointer first
        alloc->freeRaw(raw);                // Free the raw pointer
        *ptr = (sptr){ 0 };                 // Set to null after freeing
    }
}

Slice mem_allocSize(const mem_Allocator* alloc, usize elem_size, usize count) {
    claim_assert_fmt(alloc != null, "Allocator cannot be null");
    claim_assert_fmt(alloc->allocRaw != null, "Allocator must implement allocRaw");
    claim_assert_fmt(elem_size > 0, "Element size must be positive");
    claim_assert_fmt(count > 0, "Count must be positive");

    anyptr raw = alloc->allocRaw(elem_size * count);
    claim_assert_fmt(raw != null, "Allocation failed");

    return Slice_make(elem_size, raw, count);
}

// bool mem_tryAllocSize(const mem_Allocator* alloc, usize elem_size, usize count, Slice* out_slice) {
//     if (alloc == null
//         || alloc->allocRaw == null
//         || elem_size == 0
//         || count == 0
//         || out_slice == null) {
//         return false;
//     }

//     anyptr raw = alloc->allocRaw(elem_size * count);
//     if (raw == null) { return false; }

//     *out_slice = Slice_make(sptr_make(elem_size, raw), count);
//     return true;
// }

// Slice mem_allocSizeOrNull(const mem_Allocator* alloc, usize elem_size, usize count) {
//     if (alloc == null
//         || alloc->allocRaw == null
//         || elem_size == 0
//         || count == 0) {
//         return Slice_null;
//     }

//     anyptr raw = alloc->allocRaw(elem_size * count);
//     if (raw == null) { return Slice_null; }

//     return Slice_fromsptr(sptr_make(elem_size, raw), count);
// }

void mem_free(const mem_Allocator* alloc, Slice* slice) {
    claim_assert_fmt(alloc != null, "Allocator cannot be null");
    claim_assert_fmt(alloc->freeRaw != null, "Allocator must implement freeRaw");
    claim_assert_fmt(!Slice_isValid(*slice), "Slice cannot be null");

    if (!Slice_isValid(*slice)) {
        anyptr_mut ptr = Slice_raw_mut(slice); // Get raw pointer first
        alloc->freeRaw(ptr);                   // Free the raw pointer
        *slice = (Slice){ 0 };                 // Set to null after freeing
    }
}

// Slice mem_reallocSize(const mem_Allocator* alloc, Slice* slice, usize elem_size, usize new_count) {
//     claim_assert_fmt(alloc != null, "Allocator cannot be null");
//     claim_assert_fmt(alloc->reallocRaw != null, "Allocator must implement reallocRaw");
//     claim_assert_fmt(!Slice_isValid(*slice), "Slice cannot be null");
//     claim_assert_fmt(elem_size > 0, "Element size must be positive");
//     claim_assert_fmt(new_count > 0, "New count must be positive");

//     anyptr new_raw = alloc->reallocRaw(Slice_raw_mut(slice), elem_size * new_count);
//     claim_assert_fmt(new_raw != null, "Reallocation failed");

//     return Slice_fromRaw(elem_size, new_raw, new_count);
// }

// bool mem_tryReallocSize(const mem_Allocator* alloc, Slice slice, usize elem_size, usize new_count, Slice* out_slice) {
//     if (alloc == null
//         || alloc->reallocRaw == null
//         || Slice_isNull(slice)
//         || elem_size == 0
//         || new_count == 0
//         || out_slice == null) {
//         return false;
//     }

//     anyptr new_raw = alloc->reallocRaw(Slice_raw(slice), elem_size * new_count);
//     if (new_raw == null) { return false; }

//     *out_slice = Slice_fromRaw(elem_size, new_raw, new_count);
//     return true;
// }

/*========== Raw Memory Operations (when needed) ============================*/

anyptr mem_allocRaw(const mem_Allocator* alloc, usize size) {
    claim_assert_fmt(alloc != null, "Allocator cannot be null");
    claim_assert_fmt(alloc->allocRaw != null, "Allocator must implement allocRaw");
    claim_assert_fmt(size > 0, "Size must be positive");

    anyptr raw = alloc->allocRaw(size);
    claim_assert_fmt(raw != null, "Allocation failed");

    return raw;
}

void mem_freeRaw(const mem_Allocator* alloc, anyptr_mut* raw) {
    claim_assert_fmt(alloc != null, "Allocator cannot be null");
    claim_assert_fmt(alloc->freeRaw != null, "Allocator must implement freeRaw");
    claim_assert_fmt(raw != null, "Raw pointer cannot be null");

    if (*raw != null) {
        alloc->freeRaw(*raw); // Free the raw pointer
        *raw = null;          // Set to null after freeing
    }
}

anyptr mem_reallocRaw(const mem_Allocator* alloc, anyptr_mut raw, usize new_size) {
    claim_assert_fmt(alloc != null, "Allocator cannot be null");
    claim_assert_fmt(alloc->reallocRaw != null, "Allocator must implement reallocRaw");
    claim_assert_fmt(raw != null, "Raw pointer cannot be null");
    claim_assert_fmt(new_size > 0, "New size must be positive");

    anyptr new_raw = alloc->reallocRaw(raw, new_size);
    claim_assert_fmt(new_raw != null, "Reallocation failed");

    return new_raw;
}

/*========== Memory Manipulation ============================================*/

void mem_copySize(sptr dest, sptr src, usize size) {
    claim_assert_fmt(!sptr_isValid(dest), "Destination cannot be null");
    claim_assert_fmt(!sptr_isValid(src), "Source cannot be null");
    claim_assert_fmt(cptr_hasMinSize(dest.core, size), "Destination too small");
    claim_assert_fmt(cptr_hasMinSize(src.core, size), "Source too small");

    memcpy(sptr_raw_mut(&dest), sptr_raw(src), size);
}

bool mem_tryCopySize(sptr dest, sptr src, usize size) {
    if (sptr_isNull(dest)
        || sptr_isNull(src)
        || !sptr_hasMinSize(dest, size)
        || !sptr_hasMinSize(src, size)) {
        return false;
    }

    memcpy(sptr_raw(dest), sptr_raw(src), size);
    return true;
}

void mem_moveSize(sptr dest, sptr src, usize size) {
    claim_assert_fmt(!sptr_isNull(dest), "Destination cannot be null");
    claim_assert_fmt(!sptr_isNull(src), "Source cannot be null");
    claim_assert_fmt(sptr_hasMinSize(dest, size), "Destination too small");
    claim_assert_fmt(sptr_hasMinSize(src, size), "Source too small");

    memmove(sptr_raw(dest), sptr_raw(src), size);
}

bool mem_tryMoveSize(sptr dest, sptr src, usize size) {
    if (sptr_isNull(dest)
        || sptr_isNull(src)
        || !sptr_hasMinSize(dest, size)
        || !sptr_hasMinSize(src, size)) {
        return false;
    }

    memmove(sptr_raw(dest), sptr_raw(src), size);
    return true;
}

void mem_setSize(sptr ptr, i32 value, usize size) {
    claim_assert_fmt(!sptr_isNull(ptr), "Pointer cannot be null");
    claim_assert_fmt(sptr_hasMinSize(ptr, size), "Pointer too small");

    memset(sptr_raw(ptr), value, size);
}

bool mem_trySetSize(sptr ptr, i32 value, usize size) {
    if (sptr_isNull(ptr) || !sptr_hasMinSize(ptr, size)) {
        return false;
    }

    memset(sptr_raw(ptr), value, size);
    return true;
}

i32 mem_cmpSize(sptr lhs, sptr rhs, usize size) {
    claim_assert_fmt(
        !sptr_isNull(lhs),
        "Left pointer cannot be null"
    );
    claim_assert_fmt(
        !sptr_isNull(rhs),
        "Right pointer cannot be null"
    );
    claim_assert_fmt(
        sptr_hasMinSize(lhs, size),
        "Left pointer too small"
    );
    claim_assert_fmt(
        sptr_hasMinSize(rhs, size),
        "Right pointer too small"
    );

    return memcmp(
        sptr_raw(lhs),
        sptr_raw(rhs),
        size
    );
}

bool mem_tryCmpSize(sptr lhs, sptr rhs, usize size, i32* out_result) {
    if (sptr_isNull(lhs)
        || sptr_isNull(rhs)
        || !sptr_hasMinSize(lhs, size)
        || !sptr_hasMinSize(rhs, size)
        || out_result == null) {
        return false;
    }

    *out_result = memcmp(
        sptr_raw(lhs),
        sptr_raw(rhs),
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
        || sptr_size(dest.ptr_) != sptr_size(src.ptr_)) {
        return false;
    }

    memcpy(
        Slice_raw(dest),
        Slice_raw(src),
        Slice_len(src) * sptr_size(src.ptr_)
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
        sptr_size(dest.ptr_) == sptr_size(src.ptr_),
        "Element size mismatch (%zu != %zu)",
        sptr_size(dest.ptr_),
        sptr_size(src.ptr_)
    );

    memmove(
        Slice_raw(dest),
        Slice_raw(src),
        Slice_len(src) * sptr_size(src.ptr_)
    );
}

bool mem_tryMoveSlice(Slice dest, Slice src) {
    if (Slice_isNull(dest)
        || Slice_isNull(src)
        || Slice_len(dest) < Slice_len(src)
        || sptr_size(dest.ptr_) != sptr_size(src.ptr_)) {
        return false;
    }

    memmove(
        Slice_raw(dest),
        Slice_raw(src),
        Slice_len(src) * sptr_size(src.ptr_)
    );
    return true;
}

void mem_setSlice(Slice slice, i32 value) {
    claim_assert_fmt(!Slice_isNull(slice), "Slice cannot be null");

    memset(
        Slice_raw(slice),
        value,
        Slice_len(slice) * sptr_size(slice.ptr_)
    );
}

bool mem_trySetSlice(Slice slice, i32 value) {
    if (Slice_isNull(slice)) { return false; }

    memset(
        Slice_raw(slice),
        value,
        Slice_len(slice) * sptr_size(slice.ptr_)
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
        sptr_size(lhs.ptr_) == sptr_size(rhs.ptr_),
        "Element size mismatch (%zu != %zu)",
        sptr_size(lhs.ptr_),
        sptr_size(rhs.ptr_)
    );

    // First compare by content up to the shorter length
    usize min_len
        = Slice_len(lhs) < Slice_len(rhs)
            ? Slice_len(lhs)
            : Slice_len(rhs);

    i32 result = memcmp(
        Slice_raw(lhs),
        Slice_raw(rhs),
        min_len * sptr_size(lhs.ptr_)
    );

    if (result != 0) { return result; }

    // If contents are equal up to min_len, shorter slice is "less than"
    return (i32)(Slice_len(lhs) - Slice_len(rhs));
}

bool mem_tryCmpSlice(Slice lhs, Slice rhs, i32* out_result) {
    if (Slice_isNull(lhs)
        || Slice_isNull(rhs)
        || sptr_size(lhs.ptr_) != sptr_size(rhs.ptr_)
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
        min_len * sptr_size(lhs.ptr_)
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

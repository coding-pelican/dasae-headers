/**
 * @copyright Copyright 2024. Gyeongtae Kim All rights reserved.
 *
 * @file    core_Ptr.c
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2024-11-19 (date of creation)
 * @updated 2024-11-19 (date of last update)
 * @version v1.0.0
 * @ingroup dasae-headers(dh)/core
 * @prefix  Ptr
 */


/*========== Includes =======================================================*/

#include "dh/core/Ptr_expt.h"
#include "dh/mem.h"

#include <stdlib.h>
#include <string.h>

/*========== Function Implementations =======================================*/

/* Construction and validation */
Ptr Ptr_from(anyptr raw, usize size) {
    if (raw == null || size == 0) {
        return Ptr_null;
    }
    return (Ptr){
        .raw_  = raw,
        .size_ = size
    };
}

Ptr Ptr_withAlloc(usize size) {
    if (size == 0) {
        return Ptr_null;
    }

    anyptr raw = mem_alloc(u8, size);
    if (raw == null) {
        return Ptr_null;
    }

    return (Ptr){
        .raw_  = raw,
        .size_ = size
    };
}

Ptr Ptr_withAllocCleared(usize size) {
    if (size == 0) {
        return Ptr_null;
    }

    anyptr raw = mem_allocCleared(u8, size);
    if (raw == null) {
        return Ptr_null;
    }

    return (Ptr){
        .raw_  = raw,
        .size_ = size
    };
}

Ptr Ptr_nullWithSize(usize size) {
    return (Ptr){
        .raw_  = null,
        .size_ = size
    };
}

bool Ptr_isValid(Ptr p) {
    // A valid pointer must either:
    // 1. Have both a non-null pointer and non-zero size
    // 2. Be a proper null pointer (null pointer AND zero size)
    return (p.raw_ != null && 0 < p.size_) || (p.raw_ == null && p.size_ == 0);
}

bool Ptr_isNull(Ptr p) {
    // A proper null pointer must have both null pointer and zero size
    return p.raw_ == null && p.size_ == 0;
}

bool Ptr_hasMinSize(Ptr p, usize required_size) {
    // First ensure the pointer is valid
    if (!Ptr_isValid(p)) {
        return false;
    }

    // Null pointers don't have any size
    if (Ptr_isNull(p)) {
        return false;
    }

    return required_size <= p.size_;
}

/* Metadata access */
usize Ptr_size(Ptr p) {
    return p.size_;
}

anyptr Ptr_raw(Ptr p) {
    return p.raw_;
}

bool Ptr_hasData(Ptr p) {
    // A pointer has data if it's valid and not null
    return Ptr_isValid(p) && !Ptr_isNull(p);
}

/*========== Internal Helper Functions =====================================*/

static bool isAligned(anyptr ptr, usize alignment) {
    return ((usize)ptr & (alignment - 1)) == 0;
}

static bool isPowerOfTwo(usize x) {
    return x != 0 && (x & (x - 1)) == 0;
}

static bool willOverflowAdd(usize a, usize b) {
    return a > (SIZE_MAX - b);
}

static bool isValidSize(usize size) {
    // Implement any size validation rules specific to your system
    // For example, maybe sizes must be <= than some maximum
    return size <= (SIZE_MAX / 2); // Example rule
}

/*========== Additional Safety Functions ==================================*/

// These aren't in the header but could be useful internal helpers
// or exposed in a future version

static bool validateAlignment(usize alignment) {
    // Alignment must be a power of 2 and not zero
    return alignment > 0 && isPowerOfTwo(alignment);
}

static bool validateRange(Ptr p, usize offset, usize length) {
    if (!Ptr_isValid(p) || Ptr_isNull(p)) {
        return false;
    }

    // Check for overflow in offset + length
    if (willOverflowAdd(offset, length)) {
        return false;
    }

    // Check if range exceeds pointer size
    return (offset + length) <= p.size_;
}

static bool validatePointers(Ptr dest, Ptr src, usize count) {
    // Validate both pointers and sizes
    if (!Ptr_isValid(dest) || !Ptr_isValid(src)) {
        return false;
    }

    if (Ptr_isNull(dest) || Ptr_isNull(src)) {
        return false;
    }

    // Check sizes
    return validateRange(dest, 0, count) && validateRange(src, 0, count);
}

/*========== Memory Management Helpers ===================================*/

static bool freeIfAllocated(Ptr* p) {
    if (p == null) {
        return false;
    }

    if (!Ptr_isValid(*p)) {
        return false;
    }

    if (p->raw_ != null) {
        free(p->raw_);
        *p = Ptr_null;
        return true;
    }

    return false;
}

static bool reallocIfNeeded(Ptr* p, usize new_size) {
    if (p == null || !Ptr_isValid(*p)) {
        return false;
    }

    if (!isValidSize(new_size)) {
        return false;
    }

    // If current size is sufficient, no need to realloc
    if (p->size_ >= new_size) {
        return true;
    }

    anyptr new_raw = realloc(p->raw_, new_size);
    if (new_raw == null) {
        return false;
    }

    p->raw_  = new_raw;
    p->size_ = new_size;
    return true;
}

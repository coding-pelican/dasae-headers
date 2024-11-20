/**
 * @copyright Copyright 2024. Gyeongtae Kim All rights reserved.
 *
 * @file    core_Slice.c
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2024-11-19 (date of creation)
 * @updated 2024-11-19 (date of last update)
 * @version v1.0.0
 * @ingroup dasae-headers(dh)/core
 * @prefix  Ptr
 */


/*========== Includes =======================================================*/

#include "dh/core/Slice_expt.h"
#include <string.h>

/*========== Function Implementations =======================================*/

/* Construction and validation */
Slice Slice_from(Ptr ptr, usize len) {
    if (!Ptr_isValid(ptr) || Ptr_isNull(ptr)) {
        return Slice_empty;
    }

    // Validate that size divides evenly by len
    if (len == 0 || Ptr_size(ptr) % len != 0) {
        return Slice_empty;
    }

    return (Slice){
        .ptr_ = ptr,
        .len_ = len
    };
}

bool Slice_isValid(Slice s) {
    if (Ptr_isNull(s.ptr_)) {
        return s.len_ == 0;
    }
    return Ptr_isValid(s.ptr_) && s.len_ > 0 && (Ptr_size(s.ptr_) % s.len_ == 0);
}

bool Slice_isEmpty(Slice s) {
    return Ptr_isNull(s.ptr_) && s.len_ == 0;
}

usize Slice_stride(Slice s) {
    if (!Slice_isValid(s) || s.len_ == 0) {
        return 0;
    }
    return Ptr_size(s.ptr_) / s.len_;
}

/* Slice operations */
Slice Slice_slice(Slice s, usize start, usize end) {
    if (!Slice_isValid(s) || start > end || end > s.len_) {
        return Slice_empty;
    }

    usize stride   = Slice_stride(s);
    usize offset   = start * stride;
    usize new_size = (end - start) * stride;

    return (Slice){
        .ptr_ = Ptr_from((char*)Ptr_raw(s.ptr_) + offset, new_size),
        .len_ = end - start
    };
}

Slice Slice_subslice(Slice s, usize offset, usize len) {
    return Slice_slice(s, offset, offset + len);
}

Ptr Slice_ptr(Slice s) {
    return s.ptr_;
}

/* Element access */
Ptr Slice__at(Slice s, usize idx) {
    if (!Slice_isValid(s) || idx >= s.len_) {
        return Ptr_null;
    }
    return Ptr_from((char*)Ptr_raw(s.ptr_) + idx * Slice_stride(s), Slice_stride(s));
}

Ptr Slice__first(Slice s) {
    if (!Slice_isValid(s)) {
        return Ptr_null;
    }
    return Slice__at(s, 0);
}

Ptr Slice__last(Slice s) {
    if (!Slice_isValid(s)) {
        return Ptr_null;
    }
    return Slice__at(s, s.len_ - 1);
}

/* Memory operations */
bool Slice_copy(Slice dest, Slice src) {
    if (!Slice_isValid(dest) || !Slice_isValid(src)) {
        return false;
    }

    if (dest.len_ != src.len_ || Slice_stride(dest) != Slice_stride(src)) {
        return false;
    }

    memcpy(Ptr_raw(dest.ptr_), Ptr_raw(src.ptr_), Ptr_size(dest.ptr_));
    return true;
}

/* Metadata access */
usize Slice_len(Slice s) {
    return s.len_;
}

usize Slice_totalSize(Slice s) {
    return Ptr_size(s.ptr_);
}

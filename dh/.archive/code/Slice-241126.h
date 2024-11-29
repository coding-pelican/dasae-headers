/**
 * @copyright Copyright 2024. Gyeongtae Kim All rights reserved.
 *
 * @file    Slice.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2024-11-22 (date of creation)
 * @updated 2024-11-26 (date of last update)
 * @version v1.0.0
 * @ingroup dasae-headers(dh)/core
 * @prefix  Slice
 *
 * @brief   View into sequence of elements with type information
 * @details A Slice represents a view into a sequence of elements, consisting of:
 *          - A Ptr to the first element with type metadata
 *          - Length (number of elements)
 *
 * Key features:
 * - No ownership or memory management
 * - Type-safe access to elements
 * - Bounds checking in debug builds
 * - Iterator support
 */

#ifndef CORE_SLICE_INCLUDED
#define CORE_SLICE_INCLUDED (1)

/*========== Includes =======================================================*/

#include "Ptr.h"
#include "dh/debug/assert.h"
#include "dh/claim/assert_static.h"

/*========== Type Definition ===============================================*/

typedef struct Slice {
    Ptr   ptr_; // Pointer to first element with type info
    usize len_; // Number of elements
} Slice;

claim_assert_static_msg(sizeof(Slice) == sizeof(usize) * 2, "Slice size mismatch");
// claim_assert_static_msg(alignof(Slice) >= alignof(Ptr), "Slice alignment mismatch");

#define Slice(THint) Slice

/*========== Constants ===================================================*/

#define Slice_null ((Slice){ .ptr_ = Ptr_null, .len_ = 0 })

/*========== Core Functions ==============================================*/

#define Slice_from(var)                          IMPL_Slice_from(var)
#define Slice_fromRange(var, begin_idx, end_idx) IMPL_Slice_fromRange(var, begin_idx, end_idx)
#define Slice_fromPrefix(var, end_idx)           IMPL_Slice_fromPrefix(var, end_idx)
#define Slice_fromSuffix(var, begin_idx)         IMPL_Slice_fromSuffix(var, begin_idx)

#define Slice_cast(TElemPtr, self) IMPL_Slice_cast(TElemPtr, self)

force_inline Slice Slice_fromRaw(anyptr raw, usize elem_size, usize len);
force_inline Slice Slice_fromPtr(Ptr ptr, usize len);

force_inline anyptr Slice_raw(Slice self);
force_inline Ptr    Slice_ptr(Slice self);
force_inline usize  Slice_elemSize(Slice self);
force_inline usize  Slice_align(Slice self);
force_inline usize  Slice_len(Slice self);
force_inline usize  Slice_size(Slice self);

force_inline bool Slice_isNull(Slice self);
force_inline bool Slice_isNonnull(Slice self);
force_inline bool Slice_isEmpty(Slice self);

#define Slice_at(TElem, self, idx)       IMPL_Slice_at(TElem, self, idx)
#define Slice_atOrNull(TElem, self, idx) IMPL_Slice_atOrNull(TElem, self, idx)

// #define Slice_get(TElem, self, idx)      IMPL_Slice_get(TElem, self, idx)
// #define Slice_set(TElem, self, idx, val) IMPL_Slice_set(TElem, self, idx, val)

force_inline anyptr Slice_atRaw(Slice self, usize idx);
force_inline Ptr    Slice_atPtr(Slice self, usize idx);
force_inline anyptr Slice_atRawOrNull(Slice self, usize idx);
force_inline Ptr    Slice_atPtrOrNull(Slice self, usize idx);

// force_inline anyptr Slice_getRaw(Slice self, usize idx);
// force_inline Ptr    Slice_getPtr(Slice self, usize idx);

// force_inline void Slice_setRaw(Slice self, usize idx, anyptr val);
// force_inline void Slice_setPtr(Slice self, usize idx, Ptr val);

#define Slice_first(TElem, self)       IMPL_Slice_first(TElem, self)
#define Slice_last(TElem, self)        IMPL_Slice_last(TElem, self)
#define Slice_firstOrNull(TElem, self) IMPL_Slice_firstOrNull(TElem, self)
#define Slice_lastOrNull(TElem, self)  IMPL_Slice_lastOrNull(TElem, self)

force_inline Ptr Slice_firstPtr(Slice self);
force_inline Ptr Slice_lastPtr(Slice self);
force_inline Ptr Slice_firstPtrOrNull(Slice self);
force_inline Ptr Slice_lastPtrOrNull(Slice self);

force_inline Slice Slice_subslice(Slice self, usize begin, usize end);
force_inline Slice Slice_subsliceOrNull(Slice self, usize begin, usize end);
force_inline Slice Slice_prefix(Slice self, usize end);
force_inline Slice Slice_suffix(Slice self, usize begin);

/**
 * Iterate over elements of slice
 *
 * @param TElem Element type
 * @param var_iter Iterator variable name
 * @param sl Slice to iterate
 */
#define Slice_iter(TElem, var_iter, s)                                                        \
    for (                                                                                     \
        rawptr(TElem) used(var_iter) = Ptr_cast(rawptr(TElem), Slice_firstPtr(TElem, s));     \
        used(var_iter) && used(var_iter) <= Ptr_cast(rawptr(TElem), Slice_lastPtr(TElem, s)); \
        ++used(var_iter)                                                                      \
    )

/*========== Implementation =================================================*/

#ifndef CORE_SLICE_IMPL_INCLUDED
#define CORE_SLICE_IMPL_INCLUDED (1)

#define IMPL_Slice_from(var)           \
    Slice_fromRaw(                     \
        rawaddr(var),                  \
        sizeof((var)[0]),              \
        sizeof(var) / sizeof((var)[0]) \
    )

#define IMPL_Slice_fromRange(var, begin_idx, end_idx)  \
    Slice_fromRaw(                                     \
        rawaddr(var) + (begin_idx) * sizeof((var)[0]), \
        sizeof((var)[0]),                              \
        (end_idx) - (begin_idx)                        \
    )

#define IMPL_Slice_fromPrefix(var, end_idx) \
    Slice_fromRaw(                          \
        rawaddr(var),                       \
        sizeof((var)[0]),                   \
        (end_idx)                           \
    )

#define IMPL_Slice_fromSuffix(var, begin_idx)          \
    Slice_fromRaw(                                     \
        rawaddr(var) + (begin_idx) * sizeof((var)[0]), \
        sizeof((var)[0]),                              \
        sizeof(var) / sizeof((var)[0]) - (begin_idx)   \
    )

#define IMPL_Slice_cast(TElemPtr, self) \
    ((TElemPtr)Slice_raw(self))

force_inline Slice Slice_fromRaw(anyptr raw, usize elem_size, usize len) {
    return (Slice){
        .ptr_ = Ptr_fromRaw(raw, elem_size),
        .len_ = len
    };
}

force_inline Slice Slice_fromPtr(Ptr ptr, usize len) {
    return (Slice){
        .ptr_ = ptr,
        .len_ = len
    };
}

force_inline anyptr Slice_raw(Slice self) {
    return Ptr_raw(self.ptr_);
}

force_inline Ptr Slice_ptr(Slice self) {
    return self.ptr_;
}

force_inline usize Slice_elemSize(Slice self) {
    return Ptr_size(self.ptr_);
}

force_inline usize Slice_align(Slice self) {
    return Ptr_align(self.ptr_);
}

force_inline usize Slice_len(Slice self) {
    return self.len_;
}

force_inline usize Slice_size(Slice self) {
    return self.len_ * Slice_elemSize(self);
}

force_inline bool Slice_isNull(Slice self) {
    return Ptr_isNull(self.ptr_);
}

force_inline bool Slice_isNonnull(Slice self) {
    return Ptr_isNonnull(self.ptr_);
}

force_inline bool Slice_isEmpty(Slice self) {
    return self.len_ == 0;
}

#define IMPL_Slice_at(TElem, self, idx) \
    (Ptr_deref(TElem, Slice_atPtr(self, idx)))

#define IMPL_Slice_atOrNull(TElem, self, idx) \
    Slice_atPtrOrNull(self, idx)

// #define IMPL_Slice_get(TElem, self, idx) \
//     Ptr_deref(Slice_getPtr(TElem, self, idx))

// #define IMPL_Slice_set(TElem, self, idx, val) \
//     Slice_setPtr(self, idx, Ptr_ref(val))

force_inline anyptr Slice_atRaw(Slice self, usize idx) {
    return Ptr_raw(Slice_atPtr(self, idx));
}

force_inline Ptr Slice_atPtr(Slice self, usize idx) {
    debug_assert_fmt(idx < self.len_, "Slice index out of bounds: %u >= %u", idx, self.len_);
    return Ptr_addOffset(self.ptr_, idx);
}

force_inline anyptr Slice_atRawOrNull(Slice self, usize idx) {
    if (idx >= self.len_) { return null; }
    return Slice_atRaw(self, idx);
}

force_inline Ptr Slice_atPtrOrNull(Slice self, usize idx) {
    if (idx >= self.len_) { return Ptr_null; }
    return Slice_atPtr(self, idx);
}

#define IMPL_Slice_first(TElem, self) \
    (Ptr_deref(TElem, Slice_firstPtr(self)))

#define IMPL_Slice_last(TElem, self) \
    (Ptr_deref(TElem, Slice_lastRaw(self)))

#define IMPL_Slice_firstOrNull(TElem, self) \
    Slice_firstPtrOrNull(self)

#define IMPL_Slice_lastOrNull(TElem, self) \
    Slice_lastPtrOrNull(self)

force_inline Ptr Slice_firstPtr(Slice self) {
    return Slice_atPtr(self, 0);
}

force_inline Ptr Slice_lastPtr(Slice self) {
    return Slice_atPtr(self, Slice_len(self) - 1);
}

force_inline Ptr Slice_firstPtrOrNull(Slice self) {
    return Slice_len(self) == 0 ? Ptr_null : Slice_firstPtr(self);
}

force_inline Ptr Slice_lastPtrOrNull(Slice self) {
    return Slice_len(self) == 0 ? Ptr_null : Slice_lastPtr(self);
}

force_inline Slice Slice_subslice(Slice self, usize begin, usize end) {
    debug_assert_fmt(begin <= end, "Slice subslice bounds out of order: %u >= %u", begin, end);
    debug_assert_fmt(end <= Slice_len(self), "Slice subslice bounds out of bounds: %u >= %u", end, Slice_len(self));
    debug_assert_fmt(Slice_isNonnull(self), "Slice is null");
    return Slice_fromRaw(Slice_atRaw(self, begin), Slice_elemSize(self), end - begin);
}

force_inline Slice Slice_subsliceOrNull(Slice self, usize begin, usize end) {
    if (begin > end) { return Slice_null; }
    if (end > Slice_len(self)) { return Slice_null; }
    if (Slice_isNull(self)) { return Slice_null; }
    return Slice_fromRaw(Slice_atRawOrNull(self, begin), Slice_elemSize(self), end - begin);
}

force_inline Slice Slice_prefix(Slice self, usize end) {
    return Slice_subslice(self, 0, end);
}

force_inline Slice Slice_suffix(Slice self, usize begin) {
    return Slice_subslice(self, begin, Slice_len(self));
}

#endif /* CORE_SLICE_IMPL_INCLUDED */

#endif /* CORE_SLICE_INCLUDED */

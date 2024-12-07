/**
 * @copyright Copyright 2024. Gyeongtae Kim All rights reserved.
 *
 * @file    ptr.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2024-11-29 (date of creation)
 * @updated 2024-11-29 (date of last update)
 * @version v1.0.0
 * @ingroup dasae-headers(dh)/core
 * @prefix  NONE
 *
 * @brief Smart pointer implementation with metadata
 * @details Implements a smart pointer type that carries type information
 * and metadata along with the pointer. Two implementations are provided:
 *
 * 64-bit systems:
 * 1. Bitfield within pointer (CORE_CORE_PTR_IMPL_BITFIELD)
 *    - Uses high 16 bits of (unused bits in) aligned pointers
 *    - 48-bit address space (256 TB) + metadata
 *    - Single word size
 *
 * 2. Double-width structure (CORE_CORE_PTR_IMPL_DOUBLE)
 *    - Full 64-bit pointer + metadata word
 *    - Full 64-bit address space support
 *    - Two word size
 *
 * 32-bit systems:
 * - Always uses double-width structure
 * - Simpler implementation
 *
 * Metadata layout:
 * - size:  12 bits (0 to 4096 bytes)
 * - align: 3 bits  (2^0 to 2^7 alignment)
 * - flags: 1 bit   (user-defined)
 */

#ifndef CORE_PTR_INCLUDED
#define CORE_PTR_INCLUDED
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "dh/core/prim/int.h"
#include "dh/core/prim/bool.h"
#include "dh/debug/assert.h"
#include "dh/claim/assert_static.h"
#include "dh/claim/unreachable.h"

#include <stdalign.h>
#include <string.h>

/*========== Metadata Layout ================================================*/

// Core pointer metadata layout
#define cptr_bits_addr_len (48) // 256 TB address space

#define cptr_bits_size_len  (12) // Up to 4096 bytes size
#define cptr_bits_algin_len (3)  // Up to 128 bytes alignment
#define cptr_bits_flags_len (1)  // boolean flags

// Masks and shifts
#define cptr_mask_bits_size  ((1ull << cptr_bits_size_len) - 1)
#define cptr_mask_bits_align ((1ull << cptr_bits_algin_len) - 1)
#define cptr_mask_bits_flags ((1ull << cptr_bits_flags_len) - 1)

#define cptr_shift_bits_size  (0)
#define cptr_shift_bits_align (cptr_bits_size_len)
#define cptr_shift_bits_flags (cptr_bits_size_len + cptr_bits_algin_len)

/*========== Core Pointer Type ==============================================*/

typedef union cptr {
    anyptr raw_;
    usize  bits_;
    struct {
        usize addr : cptr_bits_addr_len;   // 256TB address space
        usize size : cptr_bits_size_len;   // Up to 4096 bytes size
        usize align : cptr_bits_algin_len; // Up to 128 bytes alignment
        usize flags : cptr_bits_flags_len; // For allowzero, etc
    } meta_;
} cptr;
claim_assert_static_msg(sizeof(cptr) == sizeof(anyptr), "cptr size mismatch");

// void test() {
//     cptr p = *(cptr*)&((union {
//         i32* raw_;
//         usize bits_;
//         struct {
//             usize addr : cptr_bits_addr_len;
//             usize size : cptr_bits_size_len;
//             usize align : cptr_bits_algin_len;
//             usize flags : cptr_bits_flags_len;
//         } meta_;
//     }){ .raw_ = 0 });
// }

/*========== Core Operations ================================================*/

// Creation
force_inline cptr ptr_make(anyptr raw, usize elem_size, usize elem_align);
force_inline cptr ptr_makeWithFlags(anyptr raw, usize elem_size, usize elem_align, bool flags);
force_inline cptr ptr_withFlags(cptr self, bool flags);

// Access
force_inline const anyptr ptr_raw(cptr self);
force_inline usize        ptr_size(cptr self);
force_inline usize        ptr_align(cptr self);
force_inline bool         ptr_flags(cptr self);
force_inline void         ptr_setFlags(cptr* self, bool flags);

// Safety
force_inline bool ptr_isNull(cptr self);
force_inline bool ptr_hasMinSize(cptr self, usize required_size);
force_inline bool ptr_isAligned(cptr self);

// Internal helpers
force_inline unsigned int ptr__calcAlignOrder(usize align);

/*========== Single-Item Pointer ============================================*/

typedef cptr Sptr; // Alias for semantic clarity
#define Sptr(THint) typedef Sptr

// Creation
#define Sptr_make(T, raw) \
    ptr_make(raw, sizeof(T), alignof(T))

#define Sptr_ref(T, var) \
    Sptr_make(T, &(var))

// Access
#define Sptr_deref(T, self) \
    (*(T*)ptr_raw(self))

// #define Sptr_get(T, self) \
//     (*(T*)ptr_raw(self))

// #define Sptr_set(T, self, val) \
//     (*(T*)ptr_raw(self) = (val))

/*========== Many-Item Pointer ==============================================*/

typedef cptr mptr; // Alias for semantic clarity
#define mptr(THint) typedef mptr

// Creation
#define mptr_make(T, raw) \
    ptr_make(raw, sizeof(T), alignof(T))

#define mptr_ref(T, var) \
    mptr_make(T, &(var))

// Access
#define mptr_at(T, self, idx) \
    (((T*)ptr_raw(self))[idx])

// Arithmetic
force_inline mptr    mptr_add(mptr self, ptrdiff offset);
force_inline mptr    mptr_sub(mptr self, ptrdiff offset);
force_inline ptrdiff mptr_diff(mptr lhs, mptr rhs);

/*========== Slice Type =====================================================*/

typedef struct Slice {
    mptr  ptr;
    usize len;
} Slice;
#define Slice(THint) typedef Slice

// Creation
force_inline Slice Slice_make(mptr ptr, usize len);

#define Slice_fromArray(T, arr) \
    Slice_make(mptr_make(T, arr), sizeof(arr) / sizeof(T))

#define Slice_fromRange(T, cptr, begin, end) \
    Slice_make(mptr_add(mptr_make(T, cptr), begin), (end) - (begin))

// Access
force_inline const anyptr Slice_raw(Slice self);
force_inline usize        Slice_len(Slice self);

#define Slice_at(T, self, idx) \
    (((T*)Slice_raw(self))[idx])

// #define Slice_get(T, self, idx) \
//     (((T*)Slice_raw(self))[idx])

// #define Slice_set(T, self, idx, val) \
//     (((T*)Slice_raw(self))[idx] = (val))

// Sub-slice
#define Slice_slice(T, self, begin, end) \
    Slice_fromRange(T, (anyptr)Slice_raw(self), begin, end)

// Memory operations
force_inline void Slice_copy(Slice dest, Slice src);
force_inline void Slice_clear(Slice self);

/*========== Type-Safe Macros ===============================================*/

// Type checking
#define ptr_isType(T, self) \
    ptr_size(self) == sizeof(T)

// Safe conversion helpers
#define ptr_asSlice(T, self, len) \
    Slice_make(mptr_make(T, ptr_raw(self)), len)

/*========== Implementation =================================================*/

force_inline unsigned int ptr__calcAlignOrder(usize align) {
    // Calculate log2 of alignment for compact storage
#if defined(__GNUC__) || defined(__clang__)
    return (unsigned int)__builtin_ctzll((unsigned long long)align);
#elif defined(_MSC_VER)
#if BUILTIN_PLTF_64BIT
    unsigned long index = 0;
    _BitScanForward64(&index, (unsigned __int64)align);
    return (unsigned int)index;
#else
    unsigned long index = 0;
    _BitScanForward(&index, (unsigned long)align);
    return (unsigned int)index;
#endif
#else
    unsigned int order = 0;
    while (align > 1) {
        align >>= 1;
        order++;
    }
    return order;
#endif
}

/*========== Core Creation Implementation ===================================*/

force_inline cptr ptr_make(anyptr raw, usize elem_size, usize elem_align) {
    cptr self = cleared();
    if (!rawptrIsNull(raw)) {
        self.raw_        = raw;
        self.meta_.size  = elem_size & cptr_mask_bits_size;
        self.meta_.align = ptr__calcAlignOrder(elem_align) & cptr_mask_bits_align;
        self.meta_.flags = 0;
    }
    return self;
}

force_inline cptr ptr_makeWithFlags(anyptr raw, usize elem_size, usize elem_align, bool flags) {
    cptr self        = ptr_make(raw, elem_size, elem_align);
    self.meta_.flags = flags & 1;
    return self;
}

force_inline cptr ptr_withFlags(cptr self, bool flags) {
    cptr result        = self;
    result.meta_.flags = flags & 1;
    return result;
}

/*========== Core Access Implementation =====================================*/

force_inline const anyptr ptr_raw(cptr self) {
    return intToRawptr(anyptr, self.bits_ & ((1ull << cptr_bits_addr_len) - 1));
}

force_inline usize ptr_size(cptr self) {
    return self.meta_.size;
}

force_inline usize ptr_align(cptr self) {
    return 1ull << self.meta_.align;
}

force_inline bool ptr_flags(cptr self) {
    return self.meta_.flags != 0;
}

force_inline void ptr_setFlags(cptr* const self, bool flags) {
    self->meta_.flags = flags & 1;
}

/*========== Core Safety Implementation =====================================*/

force_inline bool ptr_isNull(cptr self) {
    return rawptrIsNull(self.raw_) || (!ptr_flags(self) && rawptrToInt(self.raw_) == 0);
}

force_inline bool ptr_hasMinSize(cptr self, usize required_size) {
    return self.meta_.size >= required_size;
}

force_inline bool ptr_isAligned(cptr self) {
    return (rawptrToInt(self.raw_) % (1ull << self.meta_.align)) == 0;
}

/*========== Many-item Pointer Implementation ===============================*/

force_inline mptr mptr_add(mptr self, ptrdiff offset) {
    return ptr_make(
        (u8*)ptr_raw(self) + offset * ptr_size(self),
        ptr_size(self),
        ptr_align(self)
    );
}

force_inline mptr mptr_sub(mptr self, ptrdiff offset) {
    return ptr_make(
        (u8*)ptr_raw(self) - offset * ptr_size(self),
        ptr_size(self),
        ptr_align(self)
    );
}

force_inline ptrdiff mptr_diff(mptr lhs, mptr rhs) {
    debug_assert_eq(ptr_size(lhs), ptr_size(rhs));
    return as(ptrdiff, ((u8*)ptr_raw(lhs) - (u8*)ptr_raw(rhs)) / ptr_size(lhs));
}

/*========== Slice Implementation ===========================================*/

force_inline Slice Slice_make(mptr ptr, usize len) {
    return (Slice){ .ptr = ptr, .len = len };
}

force_inline const anyptr Slice_raw(Slice self) {
    debug_assert_nonnull(ptr_raw(self.ptr));
    debug_assert_true(ptr_isAligned(self.ptr));
    return ptr_raw(self.ptr);
}

force_inline usize Slice_len(Slice self) {
    return self.len;
}

force_inline void Slice_copy(Slice dest, Slice src) {
    debug_assert_eq(dest.len, src.len);
    debug_assert_true(ptr_size(dest.ptr) == ptr_size(src.ptr));

    memcpy(
        (anyptr)Slice_raw(dest),
        Slice_raw(src),
        dest.len * ptr_size(dest.ptr)
    );
}

force_inline void Slice_clear(Slice self) {
    memset(
        (anyptr)Slice_raw(self),
        0,
        self.len * ptr_size(self.ptr)
    );
}

/*======== Built-in Types ===================================================*/

// Single-Item Pointers
Sptr(u8) Sptr_u8;
Sptr(u16) Sptr_u16;
Sptr(u32) Sptr_u32;
Sptr(u64) Sptr_u64;
Sptr(usize) Sptr_usize;
Sptr(usize) Sptr_usize;

Sptr(i8) Sptr_i8;
Sptr(i16) Sptr_i16;
Sptr(i32) Sptr_i32;
Sptr(i64) Sptr_i64;
Sptr(isize) Sptr_isize;
Sptr(isize) Sptr_isize;

Sptr(f32) Sptr_f32;
Sptr(f64) Sptr_f64;

Sptr(bool) Sptr_bool;
Sptr(char) Sptr_char;

Sptr(anyptr) Sptr_anyptr;
Sptr(cptr) Sptr_cptr;
Sptr(Sptr) Sptr_Sptr;
Sptr(mptr) Sptr_mptr;
Sptr(Slice) Sptr_Slice;

// Many-item Pointers
mptr(u8) mptr_u8;
mptr(u16) mptr_u16;
mptr(u32) mptr_u32;
mptr(u64) mptr_u64;
mptr(usize) mptr_usize;
mptr(usize) mptr_usize;

mptr(i8) mptr_i8;
mptr(i16) mptr_i16;
mptr(i32) mptr_i32;
mptr(i64) mptr_i64;
mptr(isize) mptr_isize;
mptr(isize) mptr_isize;

mptr(f32) mptr_f32;
mptr(f64) mptr_f64;

mptr(bool) mptr_bool;
mptr(char) mptr_char;

mptr(anyptr) mptr_anyptr;
mptr(cptr) mptr_cptr;
mptr(Sptr) mptr_Sptr;
mptr(mptr) mptr_mptr;
mptr(Slice) mptr_Slice;

// Slice Types
Slice(u8) Slice_u8;
Slice(u16) Slice_u16;
Slice(u32) Slice_u32;
Slice(u64) Slice_u64;
Slice(usize) Slice_usize;
Slice(usize) Slice_usize;

Slice(i8) Slice_i8;
Slice(i16) Slice_i16;
Slice(i32) Slice_i32;
Slice(i64) Slice_i64;
Slice(isize) Slice_isize;
Slice(isize) Slice_isize;

Slice(f32) Slice_f32;
Slice(f64) Slice_f64;

Slice(bool) Slice_bool;
Slice(char) Slice_char;

Slice(anyptr) Slice_anyptr;
Slice(cptr) Slice_cptr;
Slice(Sptr) Slice_Sptr;
Slice(mptr) Slice_mptr;
Slice(Slice) Slice_Slice;

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* CORE_PTR_INCLUDED */

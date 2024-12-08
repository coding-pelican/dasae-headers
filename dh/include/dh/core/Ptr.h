/**
 * @copyright Copyright 2024. Gyeongtae Kim All rights reserved.
 *
 * @file    ptr.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2024-11-29 (date of creation)
 * @updated 2024-12-06 (date of last update)
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
#define CORE_PTR_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

#include "prim/int.h"
#include "prim/flt.h"
#include "prim/bool.h"
#include "prim/ptr.h"
#include "dh/debug/assert.h"
#include "dh/claim/assert_static.h"

#include <stdalign.h>
#include <string.h>


/*========== Metadata Layout ================================================*/

// Metadata layout
#define cptr_bits_addr_len  (48) // 256 TB address space
#define cptr_bits_size_len  (12) // Up to 4096 bytes size
#define cptr_bits_align_len (3)  // Up to 128 bytes alignment
#define cptr_bits_flags_len (1)  // boolean flags

// Masks and shifts
#define cptr_mask_bits_size  ((1ull << cptr_bits_size_len) - 1)
#define cptr_mask_bits_align ((1ull << cptr_bits_align_len) - 1)
#define cptr_mask_bits_flags ((1ull << cptr_bits_flags_len) - 1)

#define cptr_shift_bits_size  (0)
#define cptr_shift_bits_align (cptr_bits_size_len)
#define cptr_shift_bits_flags (cptr_bits_size_len + cptr_bits_align_len)

/*========== Core Pointer (cptr) ============================================*/

// Core pointer type with metadata bitfields
typedef union cptr {
    anyptr raw_;
    usize  bits_;
    struct {
        usize addr : cptr_bits_addr_len;   // 256TB address space
        usize size : cptr_bits_size_len;   // Up to 4096 bytes size
        usize align : cptr_bits_align_len; // Up to 128 bytes alignment
        usize flags : cptr_bits_flags_len; // Not used: maybe for future allowzero/freed/mutable, etc or custom
    } meta_;
} cptr;
claim_assert_static_msg(sizeof(cptr) == sizeof(anyptr), "cptr size mismatch");

// Core pointer operations
force_inline cptr   cptr_make(anyptr raw, usize size, usize align);
force_inline anyptr cptr_raw(cptr self);
force_inline usize  cptr_size(cptr self);
force_inline usize  cptr_align(cptr self);
force_inline bool   cptr_flags(cptr self);
force_inline void   cptr_setFlags(cptr* self, bool flags);
force_inline bool   cptr_isZero(cptr self);
force_inline bool   cptr_isAligned(cptr self);
force_inline bool   cptr_hasMinSize(cptr self, usize required_size);
force_inline usize  cptr__calcLog2AlignOrder(usize align);

/*========== Single-Item Pointer (Sptr) =====================================*/

typedef cptr Sptr;
#define Sptr(THint) typedef Sptr

// Creation
#define Sptr_make(T, raw) \
    cptr_make(raw, sizeof(T), alignof(T))

#define Sptr_ref(T, var) \
    Sptr_make(T, &(var))

// Access
#define Sptr_deref(T, self) \
    (*(T*)cptr_raw(self))

force_inline anyptr Sptr_raw(Sptr self);
force_inline usize  Sptr_size(Sptr self);
force_inline usize  Sptr_align(Sptr self);
force_inline bool   Sptr_isZero(Sptr self);
force_inline bool   Sptr_isAligned(Sptr self);

/*========== Many-Item Pointer (Mptr) =======================================*/

typedef cptr Mptr;
#define Mptr(THint) typedef Mptr

// Creation
#define Mptr_make(T, raw) \
    cptr_make(raw, sizeof(T), alignof(T))

#define Mptr_ref(T, var) \
    Mptr_make(T, &(var))

// Access
#define Mptr_at(T, self, idx) \
    (((T*)cptr_raw(self))[idx])

force_inline anyptr Mptr_raw(Mptr self);
force_inline usize  Mptr_size(Mptr self);
force_inline usize  Mptr_align(Mptr self);
force_inline bool   Mptr_isZero(Mptr self);
force_inline bool   Mptr_isAligned(Mptr self);

// Arithmetic
force_inline Mptr    Mptr_add(Mptr self, ptrdiff offset);
force_inline Mptr    Mptr_sub(Mptr self, ptrdiff offset);
force_inline ptrdiff Mptr_diff(Mptr lhs, Mptr rhs);

/*========== Slice Type =====================================================*/

typedef struct Slice {
    Mptr  ptr;
    usize len;
} Slice;
#define Slice(THint) typedef Slice
claim_assert_static_msg(sizeof(Slice) == 2 * sizeof(anyptr), "Slice size mismatch");

// Creation
force_inline Slice Slice_make(Mptr ptr, usize len);

#define Slice_fromArray(T, arr) \
    Slice_make(Mptr_make(T, arr), sizeof(arr) / sizeof(T))

#define Slice_fromRange(T, ptr, begin, end) \
    Slice_make(Mptr_add(Mptr_make(T, ptr), begin), (end) - (begin))

// Access
#define Slice_at(T, self, idx) \
    (((T*)Slice_raw(self))[idx])

force_inline anyptr Slice_raw(Slice self);
force_inline Mptr   Slice_ptr(Slice self);
force_inline usize  Slice_size(Slice self);
force_inline usize  Slice_align(Slice self);
force_inline usize  Slice_len(Slice self);
force_inline bool   Slice_isZero(Slice self);
force_inline bool   Slice_isAligned(Slice self);

// Sub-slice
#define Slice_slice(T, self, begin, end) \
    Slice_fromRange(T, Slice_raw(self), begin, end)

// Memory operations
force_inline void Slice_copy(Slice dest, Slice src);
force_inline void Slice_clear(Slice self);

/*========== Void Type ======================================================*/

typedef struct Void {
    u8 unused_[sizeof(void)];
} Void;

/*========== Built-in Types =================================================*/

// Single-Item Pointers
Sptr(u8) Sptr_u8;
Sptr(u16) Sptr_u16;
Sptr(u32) Sptr_u32;
Sptr(u64) Sptr_u64;
Sptr(usize) Sptr_usize;

Sptr(i8) Sptr_i8;
Sptr(i16) Sptr_i16;
Sptr(i32) Sptr_i32;
Sptr(i64) Sptr_i64;
Sptr(isize) Sptr_isize;

Sptr(f32) Sptr_f32;
Sptr(f64) Sptr_f64;

Sptr(bool) Sptr_bool;
Sptr(char) Sptr_char;

Sptr(anyptr) Sptr_anyptr;
Sptr(cptr) Sptr_cptr;
Sptr(Sptr) Sptr_Sptr;
Sptr(Mptr) Sptr_Mptr;
Sptr(Slice) Sptr_Slice;

// Many-item Pointers
Mptr(u8) Mptr_u8;
Mptr(u16) Mptr_u16;
Mptr(u32) Mptr_u32;
Mptr(u64) Mptr_u64;
Mptr(usize) Mptr_usize;

Mptr(i8) Mptr_i8;
Mptr(i16) Mptr_i16;
Mptr(i32) Mptr_i32;
Mptr(i64) Mptr_i64;
Mptr(isize) Mptr_isize;

Mptr(f32) Mptr_f32;
Mptr(f64) Mptr_f64;

Mptr(bool) Mptr_bool;
Mptr(char) Mptr_char;

Mptr(anyptr) Mptr_anyptr;
Mptr(cptr) Mptr_cptr;
Mptr(Sptr) Mptr_Sptr;
Mptr(Mptr) Mptr_Mptr;
Mptr(Slice) Mptr_Slice;

// Slice Types
Slice(u8) Slice_u8;
Slice(u16) Slice_u16;
Slice(u32) Slice_u32;
Slice(u64) Slice_u64;
Slice(usize) Slice_usize;

Slice(i8) Slice_i8;
Slice(i16) Slice_i16;
Slice(i32) Slice_i32;
Slice(i64) Slice_i64;
Slice(isize) Slice_isize;

Slice(f32) Slice_f32;
Slice(f64) Slice_f64;

Slice(bool) Slice_bool;
Slice(char) Slice_char;

Slice(anyptr) Slice_anyptr;
Slice(cptr) Slice_cptr;
Slice(Sptr) Slice_Sptr;
Slice(Mptr) Slice_Mptr;
Slice(Slice) Slice_Slice;

/*========== Implementation =================================================*/

force_inline usize cptr__calcLog2AlignOrder(usize align) {
#if defined(__GNUC__) || defined(__clang__)
    return (usize)__builtin_ctzll((unsigned long long)align);
#elif defined(_MSC_VER)
#if BUILTIN_PLTF_64BIT
    unsigned long index = 0;
    _BitScanForward64(&index, (unsigned __int64)align);
    return (usize)index;
#else
    unsigned long index = 0;
    _BitScanForward(&index, (unsigned long)align);
    return (usize)index;
#endif
#else
    usize order = 0;
    while (align > 1) {
        align >>= 1;
        order++;
    }
    return order;
#endif
}

/*========== Core Pointer (cptr) Implementation =============================*/

force_inline cptr cptr_make(anyptr raw, usize size, usize align) {
    cptr self = cleared();
    if (!rawptrIsNull(raw)) {
        self.raw_        = raw;
        self.meta_.size  = size & cptr_mask_bits_size;
        self.meta_.align = cptr__calcLog2AlignOrder(align) & cptr_mask_bits_align;
        self.meta_.flags = 0;
    }
    return self;
}

force_inline anyptr cptr_raw(cptr self) {
    return intToRawptr(anyptr, self.bits_ & ((1ull << cptr_bits_addr_len) - 1));
}

force_inline usize cptr_size(cptr self) {
    return self.meta_.size;
}

force_inline usize cptr_align(cptr self) {
    return 1ull << self.meta_.align;
}

force_inline bool cptr_flags(cptr self) {
    return self.meta_.flags != 0;
}

force_inline void cptr_setFlags(cptr* self, bool flags) {
    debug_assert_nonnull(self);
    self->meta_.flags = flags & 1;
}

force_inline bool cptr_isZero(cptr self) {
    return rawptrIsNull(self.raw_) || (!cptr_flags(self) && rawptrToInt(self.raw_) == 0);
}

force_inline bool cptr_isAligned(cptr self) {
    return (rawptrToInt(self.raw_) % (1ull << self.meta_.align)) == 0;
}

force_inline bool cptr_hasMinSize(cptr self, usize required_size) {
    return self.meta_.size >= required_size;
}

/*========== Single-Item Pointer (Sptr) Implementation ======================*/

force_inline anyptr Sptr_raw(Sptr self) {
    debug_assert_nonnull(cptr_raw(self));
    return cptr_raw(self);
}

force_inline usize Sptr_size(Sptr self) {
    return cptr_size(self);
}

force_inline usize Sptr_align(Sptr self) {
    return cptr_align(self);
}

force_inline bool Sptr_isZero(Sptr self) {
    return cptr_isZero(self);
}

force_inline bool Sptr_isAligned(Sptr self) {
    return cptr_isAligned(self);
}

/*========== Many-Item Pointer (Mptr) Implementation ========================*/

force_inline anyptr Mptr_raw(Mptr self) {
    debug_assert_nonnull(cptr_raw(self));
    return cptr_raw(self);
}

force_inline usize Mptr_size(Mptr self) {
    return cptr_size(self);
}

force_inline usize Mptr_align(Mptr self) {
    return cptr_align(self);
}

force_inline bool Mptr_isZero(Mptr self) {
    return cptr_isZero(self);
}

force_inline bool Mptr_isAligned(Mptr self) {
    return cptr_isAligned(self);
}

force_inline Mptr Mptr_add(Mptr self, ptrdiff offset) {
    return cptr_make(
        (u8*)cptr_raw(self) + offset * cptr_size(self),
        cptr_size(self),
        cptr_align(self)
    );
}

force_inline Mptr Mptr_sub(Mptr self, ptrdiff offset) {
    return cptr_make(
        (u8*)cptr_raw(self) - offset * cptr_size(self),
        cptr_size(self),
        cptr_align(self)
    );
}

force_inline ptrdiff Mptr_diff(Mptr lhs, Mptr rhs) {
    debug_assert_eq(cptr_size(lhs), cptr_size(rhs));
    return as(ptrdiff, ((u8*)cptr_raw(lhs) - (u8*)cptr_raw(rhs)) / cptr_size(lhs));
}

/*========== Slice Implementation ===========================================*/

force_inline Slice Slice_make(Mptr ptr, usize len) {
    return (Slice){ .ptr = ptr, .len = len };
}

force_inline anyptr Slice_raw(Slice self) {
    return Mptr_raw(self.ptr);
}

force_inline Mptr Slice_ptr(Slice self) {
    return self.ptr;
}

force_inline usize Slice_size(Slice self) {
    return Mptr_size(self.ptr);
}

force_inline usize Slice_align(Slice self) {
    return Mptr_align(self.ptr);
}

force_inline usize Slice_len(Slice self) {
    return self.len;
}

force_inline bool Slice_isZero(Slice self) {
    return Mptr_isZero(self.ptr);
}

force_inline bool Slice_isAligned(Slice self) {
    return Mptr_isAligned(self.ptr);
}

force_inline void Slice_copy(Slice dest, Slice src) {
    debug_assert_eq(dest.len, src.len);
    debug_assert_eq(Slice_size(dest), Slice_size(src));
    memcpy(
        (anyptr)Slice_raw(dest),
        Slice_raw(src),
        dest.len * Slice_size(dest)
    );
}

force_inline void Slice_clear(Slice self) {
    memset(
        (anyptr)Slice_raw(self),
        0,
        self.len * Slice_size(self)
    );
}

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* CORE_PTR_INCLUDED */

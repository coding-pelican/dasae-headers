/**
 * @copyright Copyright 2024. Gyeongtae Kim All rights reserved.
 *
 * @file    Ptr.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2024-11-22 (date of creation)
 * @updated 2024-11-26 (date of last update)
 * @version v1.0.0
 * @ingroup dasae-headers(dh)/core
 * @prefix  Ptr
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
 * - size:  12 bits (0-4095 bytes)
 * - align: 3 bits  (2^0 to 2^7 alignment)
 * - flags: 1 bit   (user-defined)
 */

#ifndef CORE_PTR_INCLUDED
#define CORE_PTR_INCLUDED (1)

/*========== Includes =======================================================*/

#include "Ptr/cfg.h"
#include "prim.h"
#include "dh/claim/unreachable.h"

/*========== Metadata Layout ================================================*/

#define Ptr_bits_addr_len (48) // 256 TB address space

#define Ptr_bits_size_len  (12) // Up to 4096 bytes size
#define Ptr_bits_algin_len (3)  // Up to 128 bytes alignment
#define Ptr_bits_flags_len (1)  // boolean flags

#define Ptr_mask_bits_size  ((1ull << Ptr_bits_size_len) - 1)
#define Ptr_mask_bits_align ((1ull << Ptr_bits_algin_len) - 1)
#define Ptr_mask_bits_flags ((1ull << Ptr_bits_flags_len) - 1)

#define Ptr_shift_bits_size  (0)
#define Ptr_shift_bits_align (Ptr_bits_size_len)
#define Ptr_shift_bits_flags (Ptr_bits_size_len + Ptr_bits_algin_len)

/*========== Type Definition ================================================*/

typedef struct Ptr {
    union {
        anyptr raw;  // Direct pointer access
        usize  bits; // Raw bits access
        struct {
            usize addr : Ptr_bits_addr_len;   // Address bits
            usize size : Ptr_bits_size_len;   // Size bits
            usize align : Ptr_bits_algin_len; // Alignment bits
            usize flags : Ptr_bits_flags_len; // Flags bits
        } meta;
    };
} Ptr;

// Verify type sizes and alignments at compile time
claim_assert_static(sizeof(Ptr) == (sizeof(usize)));

#define Ptr(THint) Ptr

/*========== Constants ======================================================*/

#define Ptr_null ((Ptr){ .raw = null })

/*========== Core Functions =================================================*/

#define Ptr_from(raw)        IMPL_Ptr_from(raw)
#define Ptr_cast(TPtr, self) IMPL_Ptr_cast(TPtr, self)

#define Ptr_addr(var)          IMPL_Ptr_addr(var)
#define Ptr_ref(var)           IMPL_Ptr_ref(var)
#define Ptr_deref(TSelf, self) IMPL_Ptr_deref(TSelf, self)

force_inline Ptr  Ptr_fromRaw(anyptr raw, usize size);
force_inline Ptr  Ptr_withFlags(Ptr self, bool flags);
force_inline void Ptr_setFlags(Ptr* const self, bool flags);

force_inline anyptr Ptr_raw(Ptr self);
force_inline usize  Ptr_bits(Ptr self);
force_inline usize  Ptr_size(Ptr self);
force_inline usize  Ptr_align(Ptr self);
force_inline bool   Ptr_flags(Ptr self);

force_inline bool Ptr_isNull(Ptr self);
force_inline bool Ptr_isNonnull(Ptr self);
force_inline bool Ptr_hasMinSize(Ptr self, usize required_size);

force_inline isize Ptr_offsetSize(Ptr self, Ptr other);
force_inline isize Ptr_offset(Ptr self, Ptr other);
force_inline Ptr   Ptr_addOffset(Ptr self, usize offset_count);
force_inline Ptr   Ptr_subOffset(Ptr self, usize offset_count);

/*========== Implementation =================================================*/

#ifndef CORE_PTR_IMPL_INCLUDED
#define CORE_PTR_IMPL_INCLUDED (1)

#define IMPL_Ptr_from(raw) \
    Ptr_fromRaw(orerr(raw), sizeof(rawderef(raw)))

#define IMPL_Ptr_cast(TPtr, self) \
    ((TPtr)Ptr_raw(self))

#define IMPL_Ptr_addr(var) \
    Ptr_fromRaw(rawaddr(var), sizeof(var))

#define IMPL_Ptr_ref(var) \
    Ptr_fromRaw(rawaddr(var), sizeof(var))

#define IMPL_Ptr_deref(TSelf, self) \
    (Ptr_hasMinSize(self, sizeof(TSelf)) ? *(TSelf*)Ptr_raw(self) : claim_unreachable_val(TSelf))

force_inline usize get_alignment_order(usize align) {
#if defined(__GNUC__) || defined(__clang__)
    return (usize)__builtin_ctzll((usize)align);
#elif defined(_MSC_VER)
#if defined(_WIN64)
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

force_inline Ptr Ptr_fromRaw(anyptr raw, usize size) {
    Ptr p = cleared();
    if (raw != null) {
        p.raw        = raw;
        p.meta.size  = size & Ptr_mask_bits_size;
        // Get alignment from pointer value itself
        p.meta.align = get_alignment_order((usize)raw) & Ptr_mask_bits_align;
        p.meta.flags = 0;
    }
    return p;
}

force_inline Ptr Ptr_withFlags(Ptr self, bool flags) {
    Ptr result        = self;
    result.meta.flags = flags & 1;
    return result;
}

force_inline void Ptr_setFlags(Ptr* const self, bool flags) {
    self->meta.flags = flags & 1;
}

force_inline anyptr Ptr_raw(Ptr self) {
    return intToRaw(anyptr, self.bits & ((1ull << Ptr_bits_addr_len) - 1));
}

force_inline usize Ptr_bits(Ptr self) {
    return self.bits;
}

force_inline usize Ptr_size(Ptr self) {
    return self.meta.size;
}

force_inline usize Ptr_align(Ptr self) {
    return 1ull << self.meta.align;
}

force_inline bool Ptr_flags(Ptr self) {
    return self.meta.flags != 0;
}

force_inline bool Ptr_isNull(Ptr self) {
    return self.raw == null;
}

force_inline bool Ptr_isNonnull(Ptr self) {
    return !Ptr_isNull(self);
}

force_inline bool Ptr_hasMinSize(Ptr self, usize required_size) {
    return self.meta.size >= required_size;
}

force_inline isize Ptr_offsetSize(Ptr self, Ptr other) {
    return rawToInt(Ptr_cast(u8*, self)) - rawToInt(Ptr_cast(u8*, other));
}

force_inline isize Ptr_offset(Ptr self, Ptr other) {
    return Ptr_offsetSize(self, other) / (isize)Ptr_size(self);
}

force_inline Ptr Ptr_addOffset(Ptr self, usize offset_count) {
    const usize size   = Ptr_size(self);
    const usize offset = size * offset_count;
    return Ptr_fromRaw(Ptr_cast(u8*, self) + offset, size);
}

force_inline Ptr Ptr_subOffset(Ptr self, usize offset_count) {
    const usize size   = Ptr_size(self);
    const usize offset = size * offset_count;
    return Ptr_fromRaw(Ptr_cast(u8*, self) - offset, size);
}

#endif // CORE_PTR_IMPL_INCLUDED

#endif // CORE_PTR_INCLUDED

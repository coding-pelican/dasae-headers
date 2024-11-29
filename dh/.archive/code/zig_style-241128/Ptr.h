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
 * - size:  12 bits (0 to 4096 bytes)
 * - align: 3 bits  (2^0 to 2^7 alignment)
 * - flags: 1 bit   (user-defined)
 */

#ifndef PTR_INCLUDED
#define PTR_INCLUDED (1)

#include "dh/core/prim/int.h"
#include "dh/core/prim/bool.h"
#include "dh/core/prim/ptr.h"

#include <assert.h>
#include <string.h>

/*========== Configuration =================================================*/

/*
 * 1. ensures 'Ptr == SPtr == MPtr'
 * 2. add option 'allow using raw ptr (pass as params or return value)'
 * 3. decide implementation option mutable or immutable based on bitfields (bool checking) / separated structs
    such as 'Ptr' and 'PtrMut'
 */

// Check if we're on a 64-bit system
#if defined(_WIN64) || defined(__x86_64__) || defined(__aarch64__)
#define ZIG_PTR_64BIT 1
#else
#define ZIG_PTR_64BIT 0
#endif

/*========== Metadata Layout ===============================================*/

// Metadata bit layout for 64-bit systems
#define Ptr_bits_addr_len  (48) // 256 TB address space
#define Ptr_bits_size_len  (12) // Up to 4096 bytes size
#define Ptr_bits_algin_len (3)  // Up to 128 bytes alignment
#define Ptr_bits_flags_len (1)  // boolean flags

// Masks and shifts
#define Ptr_mask_bits_size  ((1ull << Ptr_bits_size_len) - 1)
#define Ptr_mask_bits_align ((1ull << Ptr_bits_algin_len) - 1)
#define Ptr_mask_bits_flags ((1ull << Ptr_bits_flags_len) - 1)

#define Ptr_shift_bits_size  (0)
#define Ptr_shift_bits_align (Ptr_bits_size_len)
#define Ptr_shift_bits_flags (Ptr_bits_size_len + Ptr_bits_algin_len)

/*========== Type Definition ================================================*/

#if ZIG_PTR_64BIT
// Optimized single-word pointer for 64-bit systems
typedef union {
    const anyptr raw;  // Direct pointer access
    uptr         bits; // Raw bits access
    struct {
        uptr addr : Ptr_bits_addr_len;   // Address bits
        uptr size : Ptr_bits_size_len;   // Size bits
        uptr align : Ptr_bits_algin_len; // Alignment bits
        uptr flags : Ptr_bits_flags_len; // Flags bits
    } meta;
} Ptr;
#else
// Double-word pointer for 32-bit systems
typedef struct {
    anyptr raw;
    struct {
        u16 size : Ptr_bits_size_len;
        u8  align : Ptr_bits_algin_len;
        u8  flags : Ptr_bits_flags_len;
    } meta;
} Ptr;
#endif

// Verify type sizes and alignments at compile time
claim_assert_static(sizeof(Ptr) == (sizeof(usize)));

#define Ptr(THint) Ptr

/*========== Constants ======================================================*/

#define Ptr_null ((Ptr){ .raw = null })

/*========== Core Functions =================================================*/

// Make a Ptr from raw pointer and metadata
force_inline Ptr          Ptr_make(const anyptr raw, usize size, usize alignment);
// Make a Ptr from raw pointer and metadata with flags
force_inline Ptr          Ptr_makeWithFlags(const anyptr raw, usize size, usize alignment, bool flags);
// Get raw pointer from Ptr
force_inline const anyptr Ptr_raw(Ptr self);
// Get size from Ptr
force_inline usize        Ptr_size(Ptr self);
// Get alignment from Ptr
force_inline usize        Ptr_align(Ptr self);
// Get flags from Ptr
force_inline bool         Ptr_flags(Ptr self);
// Set flags on Ptr
force_inline void         Ptr_setFlags(Ptr* const self, bool flags);

/*========== Safety Checks ==================================================*/

force_inline bool Ptr_isNull(Ptr self);
force_inline bool Ptr_isNonnull(Ptr self);

// Check if pointer has minimum required size
force_inline bool Ptr_hasMinSize(Ptr self, usize required_size);
// Check if pointer is properly aligned
force_inline bool Ptr_isAligned(Ptr self);

/*========== Implementation =================================================*/

// Get alignment order (log2 of alignment)
force_inline unsigned int get_alignment_order(uptr align) {
#if defined(__GNUC__) || defined(__clang__)
    return (unsigned int)__builtin_ctzll((unsigned long long)align);
#elif defined(_MSC_VER)
    unsigned long index = 0;
#if defined(_WIN64)
    _BitScanForward64(&index, (unsigned __int64)align);
#else
    _BitScanForward(&index, (unsigned long)align);
#endif
    return (unsigned int)index;
#else
    unsigned int order = 0;
    while (align > 1) {
        align >>= 1;
        order++;
    }
    return order;
#endif
}

force_inline Ptr Ptr_make(const anyptr raw, usize size, usize alignment) {
    unused(alignment);
    Ptr ptr = cleared();
    if (raw != null) {
#if ZIG_PTR_64BIT
        ptr.raw        = raw;
        ptr.meta.size  = size & Ptr_mask_bits_size;
        ptr.meta.align = get_alignment_order((uptr)raw) & Ptr_mask_bits_align;
        ptr.meta.flags = 0;
#else
        ptr.raw        = raw;
        ptr.meta.size  = size & Ptr_mask_bits_size;
        ptr.meta.align = get_alignment_order((uptr)raw) & Ptr_mask_bits_align;
        ptr.meta.flags = 0;
#endif
    }
    return ptr;
}

force_inline Ptr Ptr_makeWithFlags(const anyptr raw, usize size, usize alignment, bool flags) {
    unused(alignment);
    Ptr ptr = cleared();
    if (raw != null) {
#if ZIG_PTR_64BIT
        ptr.raw        = raw;
        ptr.meta.size  = size & Ptr_mask_bits_size;
        ptr.meta.align = get_alignment_order((uptr)raw) & Ptr_mask_bits_align;
        ptr.meta.flags = flags;
#else
        ptr.raw        = raw;
        ptr.meta.size  = size & Ptr_mask_bits_size;
        ptr.meta.align = get_alignment_order((uptr)raw) & Ptr_mask_bits_align;
        ptr.meta.flags = flags;
#endif
    }
    return ptr;
}

force_inline const anyptr Ptr_raw(Ptr self) {
#if ZIG_PTR_64BIT
    return intToRaw(const anyptr, self.bits & ((1ull << Ptr_bits_addr_len) - 1)); // NOLINT
#else
    return self.raw;
#endif
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

force_inline void Ptr_setFlags(Ptr* const self, bool flags) {
    self->meta.flags = flags & 1;
}

force_inline bool Ptr_isNull(Ptr self) {
    return self.raw == null || Ptr_size(self) == 0;
}

force_inline bool Ptr_isNonnull(Ptr self) {
    return !Ptr_isNull(self);
}

force_inline bool Ptr_hasMinSize(Ptr self, usize required_size) {
    return Ptr_size(self) >= required_size;
}

force_inline bool Ptr_isAligned(Ptr self) {
    uptr addr = (uptr)Ptr_raw(self);
    return (addr % Ptr_align(self)) == 0;
}

/*========== Accessor Macros ============================================*/

// Dereference a single-item pointer
#define ptr_deref(ptr_struct, T) \
    (*(T*)Ptr_raw((ptr_struct).core))

// Index into a many-item pointer
#define ptr_index(ptr_struct, T, i) \
    (((T*)Ptr_raw((ptr_struct).core))[i])

/*========== Pointer Arithmetic =================================================*/

// // Add offset to many-item pointer
// force_inline MPtr MPtr_add(MPtr ptr, iptr offset) {
//     void* base      = Ptr_raw(ptr.core);
//     usize elem_size = Ptr_size(ptr.core);
//     return many_item_ptr(
//         (u8*)base + offset * elem_size,
//         elem_size,
//         Ptr_align(ptr.core)
//     );
// }

// // Subtract pointers
// force_inline iptr MPtr_diff(Ptr a, Ptr b) {
//     assert(Ptr_size(a) == Ptr_size(b));
//     return ((u8*)Ptr_raw(a) - (u8*)Ptr_raw(b)) / Ptr_size(a);
// }

// /*========== Sentinel-Terminated Pointer Operations ==============================*/

// // Create sentinel-terminated pointer
// force_inline SentinelPtr sentinel_ptr(void* ptr, usize size, usize alignment, const void* sentinel, usize sentinel_size) {
//     assert(sentinel_size <= 16);
//     SentinelPtr result = {
//         .core          = Ptr_create(ptr, size, alignment),
//         .sentinel_size = sentinel_size
//     };
//     memcpy(result.sentinel, sentinel, sentinel_size);
//     return result;
// }

// // Check if value matches sentinel
// force_inline bool is_sentinel(const void* value, const void* sentinel, usize size) {
//     return memcmp(value, sentinel, size) == 0;
// }

// // Get length of sentinel-terminated pointer
// force_inline usize sentinel_len(SentinelPtr ptr) {
//     void* base      = Ptr_raw(ptr.core);
//     usize elem_size = Ptr_size(ptr.core);
//     usize len       = 0;
//     while (!is_sentinel(
//         (u8*)base + len * elem_size,
//         ptr.sentinel,
//         ptr.sentinel_size
//     )) {
//         len++;
//     }
//     return len;
// }




#endif // PTR_INCLUDED

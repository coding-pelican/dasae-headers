/**
 * @copyright Copyright 2024. Gyeongtae Kim All rights reserved.
 *
 * @file    Ptr.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2024-11-22 (date of creation)
 * @updated 2024-11-22 (date of last update)
 * @version v1.0.0
 * @ingroup dasae-headers(dh)/core
 * @prefix  Ptr
 *
 * @brief Smart pointer implementation with metadata
 * @details Implements a smart pointer type that carries type information
 * and metadata along with the pointer. Two implementations are provided:
 *
 * 64-bit systems:
 * 1. Bitfield within pointer (CORE_PTR_IMPL_BITFIELD)
 *    - Uses high 16 bits of (unused bits in) aligned pointers
 *    - 48-bit address space + metadata
 *    - Single word size
 *
 * 2. Double-width structure (CORE_PTR_IMPL_DOUBLE)
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

/*========== Metadata Layout ================================================*/

#define Ptr_size_bits  (12) // Up to 4096 bytes
#define Ptr_align_bits (3)  // Up to 128-byte alignment
#define Ptr_flags_bits (1)  // Boolean flags

#define Ptr_size_mask  ((1U << Ptr_size_bits) - 1)
#define Ptr_align_mask ((1U << Ptr_align_bits) - 1)
#define Ptr_flags_mask ((1U << Ptr_flags_bits) - 1)

#define Ptr_size_shift  (0)
#define Ptr_align_shift (Ptr_size_bits)
#define Ptr_flags_shift (Ptr_size_bits + Ptr_align_bits)

/*========== Type Definition ================================================*/

#if defined(BUILTIN_PLTF_64BIT)
#if CORE_PTR_IMPL == CORE_PTR_IMPL_BITFIELD
typedef struct Ptr {
    union {
        anyptr raw;
        struct {
            usize ptr : 48;               // Actual pointer bits
            usize size : Ptr_size_bits;   // Element size
            usize align : Ptr_align_bits; // Alignment
            usize flags : Ptr_flags_bits; // Extra flags
        } bits;
    };
} Ptr;
// Verify type sizes and alignments at compile time
claim_assert_static(sizeof(Ptr) == (sizeof(usize)));
#else // CORE_PTR_IMPL_DOUBLE
typedef struct Ptr {
    union {
        struct {
            anyptr raw;                     // Pointer in first word
            struct {                        // Metadata in second word
                u64 size : Ptr_size_bits;   // Element size
                u64 align : Ptr_align_bits; // Alignment
                u64 flags : Ptr_flags_bits; // Extra flags
                u64 reserved : 48;          // Future use
            } meta;
        };
        u64 words[2]; // Access as raw 64-bit words
    };
} Ptr;
// Verify type sizes and alignments at compile time
claim_assert_static(sizeof(Ptr) == (sizeof(u64) * 2));
#endif
#else // 32-bit
typedef struct Ptr {
    union {
        struct {
            anyptr raw;                     // Pointer in first word
            struct {                        // Metadata in second word
                u32 size : Ptr_size_bits;   // Element size
                u32 align : Ptr_align_bits; // Alignment
                u32 flags : Ptr_flags_bits; // Extra flags
                u32 reserved : 16;          // Future use
            } meta;
        };
        u32 words[2]; // Access as raw 32-bit words
    };
} Ptr;
// Verify type sizes and alignments at compile time
claim_assert_static(sizeof(Ptr) == (sizeof(u32) * 2));
#endif

/*
 * static_inline u32 Ptr_calcAlignOrder(usize align);
 * static_inline usize Ptr_decodeAlign(u32 align_order);
 */

#define Ptr(_T) Ptr

/*========== Constants ======================================================*/

#define Ptr_null (literal(Ptr, .raw = 0))

/*========== Core Functions =================================================*/

/**
 * Create a new Ptr from raw pointer and size
 *
 * @param raw Raw pointer to wrap
 * @param elem_size Size of pointed-to element
 * @param flags Optional flags (e.g., ownership)
 * @return Ptr wrapping the raw pointer
 */
extern Ptr    Ptr_from(anyptr raw, usize elem_size, bool flags);
/**
 * Get raw pointer
 */
extern anyptr Ptr_raw(Ptr p);
/**
 * Get element size
 */
extern usize  Ptr_size(Ptr p);
/**
 * Get alignment
 */
extern usize  Ptr_align(Ptr p);
/**
 * Get flags
 */
extern bool   Ptr_flags(Ptr p);
/**
 * Check if null
 */
extern bool   Ptr_isNull(Ptr p);
/**
 * Check if has minimum size
 */
extern bool   Ptr_hasMinSize(Ptr p, usize required_size);

/**
 * Create from variable address
 */
#define Ptr_addr(_var) \
    Ptr_from(&(_var), sizeof(_var), false)

/*========== Type-Safe Access ===============================================*/

/**
 * @brief Cast to type
 * @param _p Pointer
 * @param _T Type
 * @return Pointer to casted type
 */
#define Ptr_as(_p, _T) \
    ((_T*)Ptr_raw(_p))

/**
 * @brief Cast to type
 * @param _p Pointer
 * @param _T Type
 * @return Cast to type if size sufficient
 */
#define Ptr_cast(_p, _T) \
    (Ptr_hasMinSize(_p, sizeof(_T)) ? (_T*)Ptr_raw(_p) : null)

/*========== Memory Operations =================================*/

/**
 * Copy pointed to destination
 */
extern bool Ptr_copy(Ptr dest, Ptr src);

/**
 * Set all bytes in pointed to value
 */
extern void Ptr_set(Ptr p, i32 value);

/**
 * Compare two pointers
 */
extern i32 Ptr_cmp(Ptr lhs, Ptr rhs);

/*========== Debug Support ==================================================*/

#ifdef CORE_PTR_DEBUG_CHECKS
#include "dh/debug/assert.h"
extern bool Ptr_isValid(Ptr p);
#define Ptr_check(_p) debug_assert(Ptr_isValid(_p))
#else
#define Ptr_check(_p) unused(0)
#endif

/*========== Platform Specific Extensions ===================================*/

#ifdef BUILTIN_PLTF_64BIT
#if CORE_PTR_IMPL == CORE_PTR_IMPL_BITFIELD
// Functions specific to 64-bit bitfield implementation
extern usize Ptr_getBits(Ptr p);
extern bool  Ptr_isCanonical(Ptr p);
#endif
#endif

#endif // CORE_PTR_INCLUDED

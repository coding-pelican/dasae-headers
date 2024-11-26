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
 * 1. Bitfield within pointer (CORE_CORE_PTR_IMPL_BITFIELD)
 *    - Uses high 16 bits of (unused bits in) aligned pointers
 *    - 48-bit address space (256TB) + metadata
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

#include <stdatomic.h>

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
#if CORE_CORE_PTR_IMPL == CORE_CORE_PTR_IMPL_BITFIELD
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
#else // CORE_CORE_PTR_IMPL_DOUBLE
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
extern Ptr Ptr_fromRaw(anyptr raw, usize elem_size, bool flags);

/**
 * Create from variable address
 */
#define Ptr_addr(_var) \
    Ptr_from(&(_var), sizeof(_var), false)

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

/*========== Platform Specific Extensions ===================================*/

#ifdef BUILTIN_PLTF_64BIT
#if CORE_PTR_IMPL == CORE_PTR_IMPL_BITFIELD
// Functions specific to 64-bit bitfield implementation
extern usize Ptr_getBits(Ptr p);
// extern bool  Ptr_isCanonical(Ptr p);
#endif
#endif

/*========== Memory Barriers =========================================*/

// Memory ordering utilities
#define PTR_SEQ_CST memory_order_seq_cst
#define PTR_ACQUIRE memory_order_acquire
#define PTR_RELEASE memory_order_release
#define PTR_RELAXED memory_order_relaxed

force_inline void Ptr_barrier(void) {
    atomic_thread_fence(PTR_SEQ_CST);
}

force_inline void Ptr_acquire(void) {
    atomic_thread_fence(PTR_ACQUIRE);
}

force_inline void Ptr_release(void) {
    atomic_thread_fence(PTR_RELEASE);
}

/*========== Atomic Operations =======================================*/

force_inline Ptr Ptr_loadAcquire(const Ptr* p) {
#if defined(BUILTIN_PLTF_64BIT) && (CORE_CORE_PTR_IMPL == CORE_CORE_PTR_IMPL_BITFIELD)
    return (Ptr){ .raw = (anyptr)atomic_load_explicit( // NOLINT
                      (const atomic_uintptr_t*)&p->raw,
                      PTR_ACQUIRE
                  ) };
#else
    Ptr result;
    atomic_load_explicit((const atomic_uintptr_t*)&p->words[0], PTR_ACQUIRE);
    atomic_load_explicit((const atomic_uintptr_t*)&p->words[1], PTR_RELAXED);
    return result;
#endif
}

force_inline void Ptr_storeRelease(Ptr* p, Ptr value) {
#if defined(BUILTIN_PLTF_64BIT) && (CORE_CORE_PTR_IMPL == CORE_CORE_PTR_IMPL_BITFIELD)
    atomic_store_explicit((atomic_uintptr_t*)&p->raw, (usize)value.raw, PTR_RELEASE);
#else
    atomic_store_explicit((atomic_uintptr_t*)&p->words[1], value.words[1], PTR_RELAXED);
    atomic_store_explicit((atomic_uintptr_t*)&p->words[0], value.words[0], PTR_RELEASE);
#endif
}

/*========== Platform Optimizations =================================*/

#ifdef BUILTIN_PLTF_64BIT
#if CORE_CORE_PTR_IMPL == CORE_CORE_PTR_IMPL_BITFIELD
// Fast pointer compression
force_inline usize Ptr_compress(Ptr p) {
    return (usize)p.raw;
}

force_inline Ptr Ptr_decompress(usize compressed) {
    return (Ptr){ .raw = (anyptr)compressed }; // NOLINT
}

// Canonical address checks
force_inline bool Ptr_isCanonical(Ptr p) {
    usize addr = p.bits.ptr;
    return (addr >> 47) == 0 || (addr >> 47) == 0x1FFFF;
}

// Fast metadata manipulation
force_inline Ptr Ptr_withFlags(Ptr p, bool flags) {
    p.bits.flags = flags;
    return p;
}

// SIMD-friendly operations
#if defined(__AVX2__)
#include <immintrin.h>

force_inline bool Ptr_isAligned32(Ptr p) {
    return !((usize)p.raw & 0x1F);
}

force_inline void Ptr_prefetch(Ptr p) {
    _mm_prefetch(p.raw, _MM_HINT_T0);
}
#endif
#endif

// Cache line optimization
#if defined(__x86_64__)
#define PTR_ALIGNED __attribute__((aligned(BUILTIN_ARCH_CACHE_LINE)))
#elif defined(__aarch64__)
#undef BUILTIN_ARCH_CACHE_LINE
#define BUILTIN_ARCH_CACHE_LINE (128)
#define PTR_ALIGNED             __attribute__((aligned(BUILTIN_ARCH_CACHE_LINE)))
#endif
#endif

/*========== Enhanced Debug Checks ==================================*/

#ifdef CORE_PTR_DEBUG_CHECKS
// Additional validation functions
force_inline bool Ptr_isAlignedFor(Ptr p, usize required_align) {
    if (Ptr_isNull(p)) {
        return true;
    }
    return ((uintptr_t)Ptr_raw(p) & (required_align - 1)) == 0;
}

force_inline bool Ptr_isInUserSpace(Ptr p) {
    if (Ptr_isNull(p)) {
        return true;
    }
    uintptr_t addr = (uintptr_t)Ptr_raw(p);
#if defined(BUILTIN_PLTF_64BIT)
    return addr < (1ULL << 47); // Typical user space limit
#else
    return addr < (1ULL << 31); // 2GB user space
#endif
}

force_inline bool Ptr_isAccessible(Ptr p) {
    if (Ptr_isNull(p)) {
        return true;
    }
    // // Platform-specific memory access check
    // #if defined(__GNUC__)
    //     char dummy = 0;
    //     return !__builtin_trap(
    //         __builtin_read1(Ptr_raw(p), &dummy)
    //     );
    // #else
    return true; // Can't check easily
    // #endif
}

// Comprehensive validation
force_inline bool Ptr_isValid(Ptr p) {
    if (Ptr_isNull(p)) {
        return true;
    }

    // Basic checks
    if (Ptr_raw(p) == NULL || Ptr_size(p) == 0) {
        return false;
    }

    // Size checks
    if (Ptr_size(p) > Ptr_size_mask) {
        return false;
    }

    // Alignment checks
    usize align = Ptr_align(p);
    if (!Ptr_isAlignedFor(p, align)) {
        return false;
    }

#ifdef BUILTIN_PLTF_64BIT
#if CORE_PTR_IMPL == CORE_PTR_IMPL_BITFIELD
    // Check canonical form
    if (!Ptr_isCanonical(p)) {
        return false;
    }
#endif
#endif

    // Address space check
    if (!Ptr_isInUserSpace(p)) {
        return false;
    }

    return true;
}

// Debug assertion helpers
#define PTR_ASSERT_VALID(p) \
    assert(Ptr_isValid(p) && "Invalid Ptr")

#define PTR_ASSERT_ALIGNED(p, align) \
    assert(Ptr_isAlignedFor(p, align) && "Misaligned Ptr")

#define PTR_ASSERT_SIZE(p, size) \
    assert(Ptr_hasMinSize(p, size) && "Insufficient Ptr size")

// Memory tracking (if enabled)
#ifdef CORE_PTR_DEBUG_TRACE
typedef struct {
    const char* file;
    i32         line;
    const char* func;
    anyptr      ptr;
    usize       size;
} PtrTrace;

extern void Ptr_recordAlloc(PtrTrace trace);
extern void Ptr_recordFree(PtrTrace trace);

#define Ptr_traceAlloc(p)       \
    Ptr_recordAlloc((PtrTrace){ \
        .file = __FILE__,       \
        .line = __LINE__,       \
        .func = __func__,       \
        .ptr  = Ptr_raw(p),     \
        .size = Ptr_size(p) })

#define Ptr_traceFree(p)       \
    Ptr_recordFree((PtrTrace){ \
        .file = __FILE__,      \
        .line = __LINE__,      \
        .func = __func__,      \
        .ptr  = Ptr_raw(p),    \
        .size = Ptr_size(p) })
#endif
#endif // CORE_PTR_DEBUG_CHECKS

/*========== Sanitizer Support ======================================*/

#if defined(__has_feature)
#if __has_feature(address_sanitizer)
#define PTR_ASAN_ENABLED
#endif
#endif

#ifdef PTR_ASAN_ENABLED
#include <sanitizer/asan_interface.h>

force_inline void Ptr_poisonMemory(Ptr p) {
    ASAN_POISON_MEMORY_REGION(Ptr_raw(p), Ptr_size(p));
}

force_inline void Ptr_unpoisonMemory(Ptr p) {
    ASAN_UNPOISON_MEMORY_REGION(Ptr_raw(p), Ptr_size(p));
}
#endif

#endif // CORE_PTR_INCLUDED

/**
 * @copyright Copyright 2025. Gyeongtae Kim All rights reserved.
 *
 * @file    simd.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2025-01-22 (date of creation)
 * @updated 2025-01-22 (date of last update)
 * @version v0.1-alpha
 * @ingroup dasae-headers(dh)/simd
 * @prefix  simd
 *
 * @brief   SIMD (Single Instruction; Multiple Data) convenience functions.
 * @details Provides vector operations optimized for various CPU architectures.
 *          May offer a potential boost in performance on some targets by performing
 *          the same operation on multiple elements at once.
 */

#ifndef SIMD_INCLUDED
#define SIMD_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "builtin.h"

/*========== Vector Length Detection =======================================*/

/**
 * The suggested vector length for each architecture based on CPU features.
 * Returns a compile-time value or 0 if SIMD is not recommended for this type/platform.
 */
#define simd_suggestLen$(T) VAL__simd_suggestLen$(T)


#if bti_arch_x64
#if bti_arch_has_avx512f && !bti_arch_has_prefer_256bit && !bti_arch_has_prefer_128bit
/* e.g., 512-bit wide registers => # elements = 512 bits / (bits per element) */
#define VAL__simd_suggestLen$(T) ((512) / (sizeOf$(T) * 8))
#elif (bti_arch_has_prefer_256bit || bti_arch_has_avx2) && !bti_arch_has_prefer_128bit
/* If user or arch prefers 256-bit or has AVX2 => use 256 bits */
#define VAL__simd_suggestLen$(T) ((256) / (sizeOf$(T) * 8))
#elif bti_arch_has_sse
/* SSE => 128 bits */
#define VAL__simd_suggestLen$(T) ((128) / (sizeOf$(T) * 8))
#elif bti_arch_has_mmx || bti_arch_has_3dnow
/* Old x86 => 64 bits */
#define VAL__simd_suggestLen$(T) ((64) / (sizeOf$(T) * 8))
#else
#define VAL__simd_suggestLen$(T) (0)
#endif

#elif bti_arch_arm64
/* ARM64 => might have SVE or NEON */
#if bti_arch_has_sve
#define VAL__simd_suggestLen$(T) ((128) / (sizeOf$(T) * 8))
#elif bti_arch_has_neon
#define VAL__simd_suggestLen$(T) ((128) / (sizeOf$(T) * 8))
#else
#define VAL__simd_suggestLen$(T) (0)
#endif

#elif bti_arch_arm32
#if bti_arch_has_neon
#define VAL__simd_suggestLen$(T) ((128) / (sizeOf$(T) * 8))
#else
#define VAL__simd_suggestLen$(T) (0)
#endif

#elif bti_arch_powerpc
/* If AltiVec => 128 bits */
#if bti_arch_has_altivec
#define VAL__simd_suggestLen$(T) ((128) / (sizeOf$(T) * 8))
#else
#define VAL__simd_suggestLen$(T) (0)
#endif

#elif (bti_arch_riscv32 || bti_arch_riscv64)
#if bti_arch_has_v
/* Suppose we pick 256 bits by default for RISC-V vector.
 * Actual length may vary, especially if it's a scalable vector.
 */
#define VAL__simd_suggestLen$(T) ((256) / (sizeOf$(T) * 8))
#else
#define VAL__simd_suggestLen$(T) (0)
#endif

#else
/* Fallback for unknown or no SIMD */
#define VAL__simd_suggestLen$(T) (0)
#endif

/*========== Vector Operations ============================================*/

/**
 * Returns a vector containing the first len integers starting from 0.
 * Example: simd_iota(int32_t, 4) = {0, 1, 2, 3}
 */
#define simd_iota$(T, len) VAL__simd_iota$(T, len)

/**
 * Repeat vector elements until the target length is reached.
 * Example: simd_repeat(int32_t, 6, {1,2,3}) = {1,2,3,1,2,3}
 */
#define simd_repeat$(T, len, vec) VAL__simd_repeat$(T, len, vec)

/**
 * Join two vectors together.
 * Example: simd_join({1,2}, {3,4}) = {1,2,3,4}
 */
#define simd_join(a, b) VAL__simd_join(a, b)

/**
 * Alternate elements from both vectors.
 * Example: simd_interlace({1,2}, {3,4}) = {1,3,2,4}
 */
#define simd_interlace(a, b) VAL__simd_interlace(a, b)

/**
 * Split an interleaved vector into separate vectors.
 */
#define simd_deinterlace(vec_count, vec) VAL__simd_deinterlace(vec_count, vec)

/**
 * Extract a subsequence from a vector.
 */
#define simd_extract(vec, start, count) VAL__simd_extract(vec, start, count)

/**
 * Rotate vector elements left by amount positions.
 */
#define simd_rotateLeft(vec, amount) VAL__simd_rotateLeft(vec, amount)

/**
 * Rotate vector elements right by amount positions.
 */
#define simd_rotateRight(vec, amount) VAL__simd_rotateRight(vec, amount)

/**
 * Reverse the order of elements in the vector.
 */
#define simd_reverse(vec) VAL__simd_reverse(vec)

/*========== Vector Searching ============================================*/

/**
 * Find first true element index.
 */
#define simd_firstTrue(vec) VAL__simd_firstTrue(vec)

/**
 * Find last true element index.
 */
#define simd_lastTrue(vec) VAL__simd_lastTrue(vec)

/**
 * Count number of true elements.
 */
#define simd_countTrues(vec) VAL__simd_countTrues(vec)

/**
 * Find index of first occurrence of a value.
 */
#define simd_firstIndexOfValue(vec, val) VAL__simd_firstIndexOfValue(vec, val)

/**
 * Find index of last occurrence of a value.
 */
#define simd_lastIndexOfValue(vec, val) VAL__simd_lastIndexOfValue(vec, val)

/**
 * Count occurrences of a value.
 */
#define simd_countElementsWithValue(vec, val) VAL__simd_countElementsWithValue(vec, val)

/*======== spinLoopHint: Platform-specific CPU hints for spin-loops =========
 *
 * On various architectures, there are instructions to reduce power usage
 * or improve back-off behavior in tight spin loops, e.g.:
 *   - x86/x86_64: 'pause'
 *   - ARM64:      'isb' or 'yield' (sometimes 'wfe')
 *   - ARMv6K:     'yield'
 *   - RISC-V:     'pause' if Zihintpause is available
 *
 * Adjust as needed for your usage patterns or correct instructions.
 *======================================================================== */
force_inline void simd_spinLoopHint(void) {
    // NOLINTBEGIN
#if bti_arch_x86 || bti_arch_x64
    __asm__ volatile("pause");
#elif bti_arch_arm64
    /*
     * Some code uses 'isb' in spin-wait.
     * Alternatively, "yield" or "wfe" might be used for different strategies.
     */
    __asm__ volatile("isb");
#elif bti_arch_arm32
#if bti_arch_has_armv6k || bti_arch_has_armv6m
    __asm__ volatile("yield");
#else
    /* On older ARM architectures without yield,
     * consider a simple 'nop' or other fallback. */
    __asm__ volatile("nop");
#endif

#elif (bti_arch_riscv32 || bti_arch_riscv64)
#if bti_arch_has_zihintpause
    /* RISC-V Zihintpause extension offers 'pause'
     * (some compilers also accept "__asm__ volatile(\"pause\")").
     */
    __asm__ volatile("pause");
#else
    /* Fallback: 'nop' if no Zihintpause ext. */
    __asm__ volatile("nop");
#endif

#else
    /* Fallback for unknown or unhandled architectures */
    __asm__ volatile("nop");
#endif
    // NOLINTEND
}

/*========== Cache Line Size ===========================================*/

#define simd_cache_line bti_arch_cache_line

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* SIMD_INCLUDED */

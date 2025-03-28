/**
 * @copyright Copyright 2024-2025. Gyeongtae Kim All rights reserved.
 *
 * @file    arch.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2024-10-27 (date of creation)
 * @updated 2025-03-28 (date of last update)
 * @version v0.1-alpha.2
 * @ingroup dasae-headers(dh)/bti
 * @prefix  bti_arch
 *
 * @brief   Architecture detection and configuration
 * @details Detects CPU architecture, instruction sets, and hardware capabilities.
 *          Includes CPU family, SIMD support, and architecture-specific features.
 *          Part of the BTI (Built-in) module for core system configuration.
 */

#ifndef BTI_ARCH_INCLUDED
#define BTI_ARCH_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "common.h"

/*========== Macros and Declarations ========================================*/

/* Enumerable Architecture Properties */

#define bti_arch_type                                                 \
    /**                                                               \
     * @brief The CPU architecture type                               \
     * @return Enumerated value representing the current architecture \
     * @see bti_arch_type_* constants                                 \
     */                                                               \
    comp_value__bti_arch_type
/* Architecture Type Constants */
#define bti_arch_type_unknown (0)
#define bti_arch_type_x86     (1)
#define bti_arch_type_x64     (2)
#define bti_arch_type_arm32   (3)
#define bti_arch_type_arm64   (4)
#define bti_arch_type_count   (5)

#define bti_arch_type_family                                          \
    /**                                                               \
     * @brief The CPU architecture family                             \
     * @return Enumerated value representing the current architecture \
     * @see bti_arch_type_family_* constants                          \
     */                                                               \
    comp_value__bti_arch_type_family
/* Architecture Family Constants */
#define bti_arch_type_family_unknown (0)
#define bti_arch_type_family_x86     (1)
#define bti_arch_type_family_arm     (2)
#define bti_arch_type_family_count   (3)

#define bti_arch_endian                                               \
    /**                                                               \
     * @brief The endianness of the architecture                      \
     * @return Enumerated value representing the current architecture \
     * @see bti_arch_endian_* constants                               \
     */                                                               \
    comp_value__bti_arch_endian
/* Architecture Endianness Constants */
#define bti_arch_endian_unknown (0)
#define bti_arch_endian_little  (0)
#define bti_arch_endian_big     (1)
#define bti_arch_endian_count   (2)

#define bti_arch_word_size                                              \
    /**                                                                 \
     * @brief Word size of the current architecture                     \
     * @return 64 for 64-bit architectures, 32 for 32-bit architectures \
     */                                                                 \
    comp_value__bti_arch_word_size
// #define bti_arch_cache_line_size                                        \
//     /**                                                                 \
//      * @brief Cache line size of the current architecture               \
//      * @return 64 for 64-bit architectures, 32 for 32-bit architectures \
//      */                                                                 \
//     comp_value__bti_arch_cache_line_size

/* Baseline CPU Architecture Detection */

#define bti_arch_detects_x64                                    \
    /**                                                         \
     * @brief AMD64/Intel64 architecture detection              \
     * @return 1 if running on x86-64 architecture, 0 otherwise \
     */                                                         \
    comp_bool__bti_arch_detects_x64
#define bti_arch_detects_x86                                        \
    /**                                                             \
     * @brief x86 32-bit architecture detection                     \
     * @return 1 if running on x86 32-bit architecture, 0 otherwise \
     */                                                             \
    comp_bool__bti_arch_detects_x86
#define bti_arch_detects_arm64                                      \
    /**                                                             \
     * @brief ARM 64-bit architecture detection                     \
     * @return 1 if running on ARM 64-bit architecture, 0 otherwise \
     */                                                             \
    comp_bool__bti_arch_detects_arm64
#define bti_arch_detects_arm32                                      \
    /**                                                             \
     * @brief ARM 32-bit architecture detection                     \
     * @return 1 if running on ARM 32-bit architecture, 0 otherwise \
     */                                                             \
    comp_bool__bti_arch_detects_arm32

/* Architecture Family */

#define bti_arch_detects_x86_family                              \
    /**                                                          \
     * @brief x86 architecture family detection (x86 or x64)     \
     * @return 1 if running on any x86 architecture, 0 otherwise \
     */                                                          \
    comp_bool__bti_arch_detects_x86_family
#define bti_arch_detects_arm_family                              \
    /**                                                          \
     * @brief ARM architecture family detection (ARM32 or ARM64) \
     * @return 1 if running on any ARM architecture, 0 otherwise \
     */                                                          \
    comp_bool__bti_arch_detects_arm_family

/* Instruction Set Extensions */

#define bti_arch_has_sse2                                             \
    /**                                                               \
     * @brief SSE2 SIMD instructions support detection                \
     * @return 1 if processor supports SSE2 instructions, 0 otherwise \
     */                                                               \
    comp_bool__bti_arch_has_sse2
#define bti_arch_has_avx                                             \
    /**                                                              \
     * @brief AVX SIMD instructions support detection                \
     * @return 1 if processor supports AVX instructions, 0 otherwise \
     */                                                              \
    comp_bool__bti_arch_has_avx
#define bti_arch_has_avx2                                             \
    /**                                                               \
     * @brief AVX2 SIMD instructions support detection                \
     * @return 1 if processor supports AVX2 instructions, 0 otherwise \
     */                                                               \
    comp_bool__bti_arch_has_avx2
#define bti_arch_has_neon                                             \
    /**                                                               \
     * @brief ARM NEON SIMD instructions support detection            \
     * @return 1 if processor supports NEON instructions, 0 otherwise \
     */                                                               \
    comp_bool__bti_arch_has_neon

/* Endianness Detection */

#define bti_arch_is_big_endian                                      \
    /**                                                             \
     * @brief Big endian architecture detection                     \
     * @return 1 if running on big endian architecture, 0 otherwise \
     */                                                             \
    comp_bool__bti_arch_is_big_endian
#define bti_arch_is_little_endian                                      \
    /**                                                                \
     * @brief Little endian architecture detection                     \
     * @return 1 if running on little endian architecture, 0 otherwise \
     */                                                                \
    comp_bool__bti_arch_is_little_endian

/* Word Size Detection */

#define bti_arch_is_64bit                                       \
    /**                                                         \
     * @brief 64-bit architecture detection                     \
     * @return 1 if running on 64-bit architecture, 0 otherwise \
     */                                                         \
    comp_bool__bti_arch_is_64bit
#define bti_arch_is_32bit                                       \
    /**                                                         \
     * @brief 32-bit architecture detection                     \
     * @return 1 if running on 32-bit architecture, 0 otherwise \
     */                                                         \
    comp_bool__bti_arch_is_32bit

/*========== Macros and Definitions =========================================*/

/* Baseline CPU Architecture Detection - Implementations */

#if defined(__x86_64__) || defined(__x86_64) || defined(_M_X64) || defined(_M_AMD64)
#define comp_bool__bti_arch_detects_x64 (1)
#else
#define comp_bool__bti_arch_detects_x64 (0)
#endif

#if (defined(__i386__) || defined(__i386) || defined(_M_IX86)) && !bti_arch_detects_x64
#define comp_bool__bti_arch_detects_x86 (1)
#else
#define comp_bool__bti_arch_detects_x86 (0)
#endif

#if defined(__aarch64__) || defined(_M_ARM64)
#define comp_bool__bti_arch_detects_arm64 (1)
#else
#define comp_bool__bti_arch_detects_arm64 (0)
#endif

#if (defined(__arm__) || defined(_M_ARM)) && !bti_arch_detects_arm64
#define comp_bool__bti_arch_detects_arm32 (1)
#else
#define comp_bool__bti_arch_detects_arm32 (0)
#endif

/* Architecture Family - Implementations */

#define comp_bool__bti_arch_detects_x86_family (bti_arch_detects_x64 || bti_arch_detects_x86)

#define comp_bool__bti_arch_detects_arm_family (bti_arch_detects_arm64 || bti_arch_detects_arm32)

/* Instruction Set Extensions - Implementations */

#if bti_arch_detects_x64
#define comp_bool__bti_arch_has_sse2 (1)
#elif bti_arch_detects_x86
#if defined(__SSE2__) || (defined(_M_IX86_FP) && _M_IX86_FP >= 2)
#define comp_bool__bti_arch_has_sse2 (1)
#else
#define comp_bool__bti_arch_has_sse2 (0)
#endif
#else
#define comp_bool__bti_arch_has_sse2 (0)
#endif

#if defined(__AVX__) || defined(__AVX2__)
#define comp_bool__bti_arch_has_avx (1)
#else
#define comp_bool__bti_arch_has_avx (0)
#endif

#if defined(__AVX2__)
#define comp_bool__bti_arch_has_avx2 (1)
#else
#define comp_bool__bti_arch_has_avx2 (0)
#endif

#if bti_arch_detects_arm64
#define comp_bool__bti_arch_has_neon (1)
#elif bti_arch_detects_arm32 && defined(__ARM_NEON)
#define comp_bool__bti_arch_has_neon (1)
#else
#define comp_bool__bti_arch_has_neon (0)
#endif

/* Endianness Detection - Implementations */

#if defined(__BYTE_ORDER__) && defined(__ORDER_BIG_ENDIAN__)
#define comp_bool__bti_arch_is_big_endian (__BYTE_ORDER__ == __ORDER_BIG_ENDIAN__)
#elif defined(__BIG_ENDIAN__) || defined(__ARMEB__) || defined(__THUMBEB__) || defined(__AARCH64EB__) || defined(_MIPSEB) || defined(__MIPSEB) || defined(__MIPSEB__)
#define comp_bool__bti_arch_is_big_endian (1)
#else
#define comp_bool__bti_arch_is_big_endian (0)
#endif

#if defined(__BYTE_ORDER__) && defined(__ORDER_LITTLE_ENDIAN__)
#define comp_bool__bti_arch_is_little_endian (__BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__)
#elif defined(__LITTLE_ENDIAN__) || defined(__ARMEL__) || defined(__THUMBEL__) || defined(__AARCH64EL__) || defined(_MIPSEL) || defined(__MIPSEL) || defined(__MIPSEL__) || bti_arch_detects_x86_family
#define comp_bool__bti_arch_is_little_endian (1)
#else
#define comp_bool__bti_arch_is_little_endian (!bti_arch_is_big_endian)
#endif

/* Word Size Detection - Implementations */

#if bti_arch_detects_x64 || bti_arch_detects_arm64 || defined(__powerpc64__) || defined(__sparcv9) || defined(__s390x__) || defined(__mips64)
#define comp_bool__bti_arch_is_64bit (1)
#else
#define comp_bool__bti_arch_is_64bit (0)
#endif

#define comp_bool__bti_arch_is_32bit (!bti_arch_is_64bit)

/* Enumerable Architecture Properties - Implementations */

#if comp_bool__bti_arch_is_64bit
#define comp_value__bti_arch_word_size (64)
#else
#define comp_value__bti_arch_word_size (32)
#endif

#if comp_bool__bti_arch_detects_x64
#define comp_value__bti_arch_type bti_arch_type_x64
#elif comp_bool__bti_arch_detects_x86
#define comp_value__bti_arch_type bti_arch_type_x86
#elif comp_bool__bti_arch_detects_arm64
#define comp_value__bti_arch_type bti_arch_type_arm64
#elif comp_bool__bti_arch_detects_arm32
#define comp_value__bti_arch_type bti_arch_type_arm32
#else
#define comp_value__bti_arch_type bti_arch_type_unknown
#endif

#if comp_bool__bti_arch_detects_x86_family
#define comp_value__bti_arch_type_family bti_arch_type_family_x86
#elif comp_bool__bti_arch_detects_arm_family
#define comp_value__bti_arch_type_family bti_arch_type_family_arm
#else
#define comp_value__bti_arch_type_family bti_arch_type_family_unknown
#endif

#if comp_bool__bti_arch_is_little_endian
#define comp_value__bti_arch_endian bti_arch_endian_little
#elif comp_bool__bti_arch_is_big_endian
#define comp_value__bti_arch_endian bti_arch_endian_big
#else
#define comp_value__bti_arch_endian bti_arch_endian_unknown /* Default to unknown endianness */
#endif

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* BTI_ARCH_INCLUDED */

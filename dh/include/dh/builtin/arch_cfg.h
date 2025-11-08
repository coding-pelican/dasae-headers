/**
 * @copyright Copyright (c) 2024-2025 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    arch_cfg.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2024-10-27 (date of creation)
 * @updated 2025-01-22 (date of last update)
 * @version v0.1-alpha.1
 * @ingroup dasae-headers(dh)/builtin
 * @prefix  arch
 *
 * @brief   Architecture detection configuration
 * @details Detects CPU architecture, instruction sets, and hardware capabilities.
 *          Includes CPU family, SIMD support, and architecture-specific features.
 */

#ifndef builtin_arch_cfg__included
#define builtin_arch_cfg__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Baseline Architecture Detection ================================*/

#define arch_name        __comp_str__arch_name
#define arch_family_name __comp_str__arch_family_name

#define arch_is_x64     __comp_bool__arch_is_x64
#define arch_is_x86     __comp_bool__arch_is_x86
#define arch_is_arm64   __comp_bool__arch_is_arm64
#define arch_is_arm32   __comp_bool__arch_is_arm32
#define arch_is_mips32  __comp_bool__arch_is_mips32
#define arch_is_mips64  __comp_bool__arch_is_mips64
#define arch_is_s390x   __comp_bool__arch_is_s390x
#define arch_is_m68k    __comp_bool__arch_is_m68k
#define arch_is_msp430  __comp_bool__arch_is_msp430
#define arch_is_riscv32 __comp_bool__arch_is_riscv32
#define arch_is_riscv64 __comp_bool__arch_is_riscv64
#define arch_is_powerpc __comp_bool__arch_is_powerpc

#define arch_family_is_x86     __comp_bool__arch_family_is_x86
#define arch_family_is_arm     __comp_bool__arch_family_is_arm
#define arch_family_is_mips    __comp_bool__arch_family_is_mips
#define arch_family_is_systemz __comp_bool__arch_family_is_systemz
#define arch_family_is_m68k    __comp_bool__arch_family_is_m68k
#define arch_family_is_msp430  __comp_bool__arch_family_is_msp430
#define arch_family_is_riscv   __comp_bool__arch_family_is_riscv
#define arch_family_is_powerpc __comp_bool__arch_family_is_powerpc

/*========== Additional Feature Detection ===================================*/

#define arch_has_armv6k      __comp_bool__arch_has_armv6k
#define arch_has_armv6m      __comp_bool__arch_has_armv6m
#define arch_has_zihintpause __comp_bool__arch_has_zihintpause

/* x86 extensions */
#define arch_has_avx512f       __comp_bool__arch_has_avx512f
#define arch_has_mmx           __comp_bool__arch_has_mmx
#define arch_has_3dnow         __comp_bool__arch_has_3dnow
#define arch_has_prefer_256bit __comp_bool__arch_has_prefer_256bit
#define arch_has_prefer_128bit __comp_bool__arch_has_prefer_128bit

/* ARM64 extension */
#define arch_has_sve __comp_bool__arch_has_sve

/* PowerPC extension */
#define arch_has_altivec __comp_bool__arch_has_altivec

/* RISC-V extension */
#define arch_has_v __comp_bool__arch_has_v

/*========== SIMD Support Detection (x86 SSE, ARM NEON, etc.) =================
 * These are examples for x86 SSE/AVX detection and ARM NEON/ASIMD detection.
 * Adjust or expand for other ISAs as needed.
 *===========================================================================*/

#define arch_has_sse    __comp_bool__arch_has_sse
#define arch_has_sse2   __comp_bool__arch_has_sse2
#define arch_has_sse3   __comp_bool__arch_has_sse3
#define arch_has_ssse3  __comp_bool__arch_has_ssse3
#define arch_has_sse4_1 __comp_bool__arch_has_sse4_1
#define arch_has_sse4_2 __comp_bool__arch_has_sse4_2
#define arch_has_avx    __comp_bool__arch_has_avx
#define arch_has_avx2   __comp_bool__arch_has_avx2

#define arch_has_neon  __comp_bool__arch_has_neon
#define arch_has_asimd __comp_bool__arch_has_asimd

/*========== Cache Line Size (Typical) ========================================
 * This macro reflects typical hardware line sizes for L1 caches:
 *   - x86_64, x86:     64 bytes
 *   - ARM (32/64):     32 bytes or 64 bytes (varies, but 32 is common on older)
 *   - MIPS:            32 bytes (common on many MIPS cores)
 *   - s390x:           256 bytes
 *   - m68k:            16 bytes
 *   - MSP430:           8 bytes
 *
 * Many modern x86_64 chips have a 64-byte L1 line size, but s390x is known to
 * have a larger line. Choose a conservative default of 64 for unknown archs.
 *
 * If you need runtime detection (e.g. to query sysfs on Linux), it is best done
 * in platform-specific code, not via static macros.
 *===========================================================================*/

#define arch_cache_line __comp_int__arch_cache_line

/*========== Implementations CPU Architecture Detection =====================*/
/*========== CPU Architecture / Feature Macros (Initial Defaults) ===========*/

#define __comp_str__arch_name        "Unknown"
#define __comp_str__arch_family_name "Unknown"

#define __comp_bool__arch_is_x64     0
#define __comp_bool__arch_is_x86     0
#define __comp_bool__arch_is_arm64   0
#define __comp_bool__arch_is_arm32   0
#define __comp_bool__arch_is_mips32  0
#define __comp_bool__arch_is_mips64  0
#define __comp_bool__arch_is_s390x   0
#define __comp_bool__arch_is_m68k    0
#define __comp_bool__arch_is_msp430  0
#define __comp_bool__arch_is_riscv32 0
#define __comp_bool__arch_is_riscv64 0
#define __comp_bool__arch_is_powerpc 0

#define __comp_bool__arch_family_is_x86     0
#define __comp_bool__arch_family_is_arm     0
#define __comp_bool__arch_family_is_mips    0
#define __comp_bool__arch_family_is_systemz 0
#define __comp_bool__arch_family_is_m68k    0
#define __comp_bool__arch_family_is_msp430  0
#define __comp_bool__arch_family_is_riscv   0
#define __comp_bool__arch_family_is_powerpc 0

#define __comp_bool__arch_has_armv6k      0
#define __comp_bool__arch_has_armv6m      0
#define __comp_bool__arch_has_zihintpause 0

/* x86/AMD64 features */
#define __comp_bool__arch_has_avx512f       0
#define __comp_bool__arch_has_mmx           0
#define __comp_bool__arch_has_3dnow         0
#define __comp_bool__arch_has_prefer_256bit 0
#define __comp_bool__arch_has_prefer_128bit 0

/* ARM64 features */
#define __comp_bool__arch_has_sve 0

/* RISC-V features */
#define __comp_bool__arch_has_v 0

/* PowerPC features */
#define __comp_bool__arch_has_altivec 0

/* Existing SSE/AVX/NEON macros (already known or set to 0) */
#ifndef __comp_bool__arch_has_sse
#define __comp_bool__arch_has_sse 0
#endif
#ifndef __comp_bool__arch_has_sse2
#define __comp_bool__arch_has_sse2 0
#endif
#ifndef __comp_bool__arch_has_sse3
#define __comp_bool__arch_has_sse3 0
#endif
#ifndef __comp_bool__arch_has_ssse3
#define __comp_bool__arch_has_ssse3 0
#endif
#ifndef __comp_bool__arch_has_sse4_1
#define __comp_bool__arch_has_sse4_1 0
#endif
#ifndef __comp_bool__arch_has_sse4_2
#define __comp_bool__arch_has_sse4_2 0
#endif
#ifndef __comp_bool__arch_has_avx
#define __comp_bool__arch_has_avx 0
#endif
#ifndef __comp_bool__arch_has_avx2
#define __comp_bool__arch_has_avx2 0
#endif

#ifndef __comp_bool__arch_has_neon
#define __comp_bool__arch_has_neon 0
#endif
#ifndef __comp_bool__arch_has_asimd
#define __comp_bool__arch_has_asimd 0
#endif

/*======== Architecture Detection ===========================================*/

/* Detect x86_64 / x86 */
#if defined(__x86_64__) || defined(_M_X64)
#undef __comp_bool__arch_is_x64
#define __comp_bool__arch_is_x64 1
#undef __comp_bool__arch_family_is_x86
#define __comp_bool__arch_family_is_x86 1
#undef __comp_str__arch_name
#define __comp_str__arch_name "x86_64"
#undef __comp_str__arch_family_name
#define __comp_str__arch_family_name "x86"

#elif defined(__i386__) || defined(_M_IX86)
#undef __comp_bool__arch_is_x86
#define __comp_bool__arch_is_x86 1
#undef __comp_bool__arch_family_is_x86
#define __comp_bool__arch_family_is_x86 1
#undef __comp_str__arch_name
#define __comp_str__arch_name "x86"
#undef __comp_str__arch_family_name
#define __comp_str__arch_family_name "x86"

/* Detect ARM64 / ARM32 */
#elif defined(__aarch64__) || defined(_M_ARM64)
#undef __comp_bool__arch_is_arm64
#define __comp_bool__arch_is_arm64 1
#undef __comp_bool__arch_family_is_arm
#define __comp_bool__arch_family_is_arm 1
#undef __comp_str__arch_name
#define __comp_str__arch_name "ARM64"
#undef __comp_str__arch_family_name
#define __comp_str__arch_family_name "ARM"

#elif defined(__arm__) || defined(_M_ARM)
#undef __comp_bool__arch_is_arm32
#define __comp_bool__arch_is_arm32 1
#undef __comp_bool__arch_family_is_arm
#define __comp_bool__arch_family_is_arm 1
#undef __comp_str__arch_name
#define __comp_str__arch_name "ARM"
#undef __comp_str__arch_family_name
#define __comp_str__arch_family_name "ARM"

/* Detect MIPS (32 or 64) */
#elif defined(__mips__)
#if defined(__mips64) || defined(__mips64__)
#undef __comp_bool__arch_is_mips64
#define __comp_bool__arch_is_mips64 1
#undef __comp_bool__arch_family_is_mips
#define __comp_bool__arch_family_is_mips 1
#undef __comp_str__arch_name
#define __comp_str__arch_name "MIPS64"
#else
#undef __comp_bool__arch_is_mips32
#define __comp_bool__arch_is_mips32 1
#undef __comp_bool__arch_family_is_mips
#define __comp_bool__arch_family_is_mips 1
#undef __comp_str__arch_name
#define __comp_str__arch_name "MIPS"
#endif
#undef __comp_str__arch_family_name
#define __comp_str__arch_family_name "MIPS"

/* Detect s390x */
#elif defined(__s390x__)
#undef __comp_bool__arch_is_s390x
#define __comp_bool__arch_is_s390x 1
#undef __comp_bool__arch_family_is_systemz
#define __comp_bool__arch_family_is_systemz 1
#undef __comp_str__arch_name
#define __comp_str__arch_name "s390x"
#undef __comp_str__arch_family_name
#define __comp_str__arch_family_name "SystemZ"

/* Detect m68k */
#elif defined(__m68k__)
#undef __comp_bool__arch_is_m68k
#define __comp_bool__arch_is_m68k 1
#undef __comp_bool__arch_family_is_m68k
#define __comp_bool__arch_family_is_m68k 1
#undef __comp_str__arch_name
#define __comp_str__arch_name "m68k"
#undef __comp_str__arch_family_name
#define __comp_str__arch_family_name "m68k"

/* Detect MSP430 (very uncommon in hosted environments) */
#elif defined(__MSP430__)
#undef __comp_bool__arch_is_msp430
#define __comp_bool__arch_is_msp430 1
#undef __comp_bool__arch_family_is_msp430
#define __comp_bool__arch_family_is_msp430 1
#undef __comp_str__arch_name
#define __comp_str__arch_name "MSP430"
#undef __comp_str__arch_family_name
#define __comp_str__arch_family_name "MSP430"

/* Detect RISC-V (32 or 64).  For reference:
 *   __riscv_xlen = 32 or 64,
 *   __riscv = 1 implies some RISC-V toolchain. */
#elif defined(__riscv)
#if (__riscv_xlen == 64)
#undef __comp_bool__arch_is_riscv64
#define __comp_bool__arch_is_riscv64 1
#undef __comp_bool__arch_family_is_riscv
#define __comp_bool__arch_family_is_riscv 1
#undef __comp_str__arch_name
#define __comp_str__arch_name "RISC-V64"
#else
#undef __comp_bool__arch_is_riscv32
#define __comp_bool__arch_is_riscv32 1
#undef __comp_bool__arch_family_is_riscv
#define __comp_bool__arch_family_is_riscv 1
#undef __comp_str__arch_name
#define __comp_str__arch_name "RISC-V32"
#endif
#undef __comp_str__arch_family_name
#define __comp_str__arch_family_name "RISC-V"

/* PowerPC / Power Architecture */
#elif defined(__powerpc__) || defined(__powerpc64__) || defined(_M_PPC)
#undef __comp_bool__arch_is_powerpc
#define __comp_bool__arch_is_powerpc 1
#undef __comp_bool__arch_family_is_powerpc
#define __comp_bool__arch_family_is_powerpc 1
#if defined(__powerpc64__) || defined(__ppc64__)
#undef __comp_str__arch_name
#define __comp_str__arch_name "PowerPC64"
#else
#undef __comp_str__arch_name
#define __comp_str__arch_name "PowerPC"
#endif
#undef __comp_str__arch_family_name
#define __comp_str__arch_family_name "PowerPC"

#else
#warning "Unknown architecture detected. Please check your architecture settings."
#endif

/*========== Feature Detection: ARMv6K, ARMv6M, RISC-V Zihintpause ==========*/

/*
 * On ARM, __ARM_ARCH_6K__ or __ARM_ARCH_6KZ__ indicate ARMv6K / kernel support,
 * and __ARM_ARCH_6M__ indicates ARMv6M (Cortex-M). Adjust as needed.
 */
#if defined(__ARM_ARCH_6K__) || defined(__ARM_ARCH_6KZ__)
#undef __comp_bool__arch_has_armv6k
#define __comp_bool__arch_has_armv6k 1
#endif
#if defined(__ARM_ARCH_6M__)
#undef __comp_bool__arch_has_armv6m
#define __comp_bool__arch_has_armv6m 1
#endif

/*
 * For RISC-V, some toolchains define __riscv_zihintpause or similar macros
 * when the 'Zihintpause' extension is supported.
 */
#if defined(__riscv_zihintpause) /* or check your compiler docs */
#undef __comp_bool__arch_has_zihintpause
#define __comp_bool__arch_has_zihintpause 1
#endif

/*======== Feature Detection: x86/AMD64 Extra (MMX, 3DNow, AVX512) ==========*/

/*
 * Common GCC/Clang macros:
 *   - __MMX__ => MMX
 *   - __3dNOW__ => 3DNow
 *   - __AVX512F__ => AVX-512 Foundation
 */
#if (__comp_bool__arch_is_x86 || __comp_bool__arch_is_x64)
#if defined(__MMX__)
#undef __comp_bool__arch_has_mmx
#define __comp_bool__arch_has_mmx 1
#endif

#if defined(__3dNOW__)
#undef __comp_bool__arch_has_3dnow
#define __comp_bool__arch_has_3dnow 1
#endif

#if defined(__AVX512F__)
#undef __comp_bool__arch_has_avx512f
#define __comp_bool__arch_has_avx512f 1
#endif
#endif /* x86/x64 */

/* Example "preferred widths" (often project-specific).
 * For instance, you might define them via build-system flags:
 *   -D__comp_bool__arch_has_prefer_256bit=1
 *   or do runtime checks for microarchitecture.
 * Here we leave them as 0 unless you define them externally. */

/*======== Feature Detection: ARM64 SVE =====================================*/

/*
 * Some compilers define __ARM_FEATURE_SVE when SVE is enabled,
 * or a similar macro. Check your compiler docs for exact naming.
 */
#if __comp_bool__arch_is_arm64
#if defined(__ARM_FEATURE_SVE)
#undef __comp_bool__arch_has_sve
#define __comp_bool__arch_has_sve 1
#endif
#endif

/*======== Feature Detection: PowerPC AltiVec ===============================*/

/*
 * AltiVec: Some compilers define __ALTIVEC__ or __VEC__
 *
 * Clang/GCC often define __ALTIVEC__ when using -maltivec or on
 * certain default configurations for PowerPC.
 */
#if __comp_bool__arch_is_powerpc
#if defined(__ALTIVEC__) || defined(__VEC__)
#undef __comp_bool__arch_has_altivec
#define __comp_bool__arch_has_altivec 1
#endif
#endif

/*======== Feature Detection: RISC-V Vector (V) =============================*/

/*
 * RISC-V vector extension: Some compilers define __riscv_vector.
 * Adjust if your compiler uses a different macro for v-extension.
 */
#if (__comp_bool__arch_is_riscv32 || __comp_bool__arch_is_riscv64)
#if defined(__riscv_vector)
#undef __comp_bool__arch_has_v
#define __comp_bool__arch_has_v 1
#endif
#endif

/*========== Implementations SIMD Support Detection =========================*/

#ifndef __comp_bool__arch_has_sse
#define __comp_bool__arch_has_sse 0
#endif
#ifndef __comp_bool__arch_has_sse2
#define __comp_bool__arch_has_sse2 0
#endif
#ifndef __comp_bool__arch_has_sse3
#define __comp_bool__arch_has_sse3 0
#endif
#ifndef __comp_bool__arch_has_ssse3
#define __comp_bool__arch_has_ssse3 0
#endif
#ifndef __comp_bool__arch_has_sse4_1
#define __comp_bool__arch_has_sse4_1 0
#endif
#ifndef __comp_bool__arch_has_sse4_2
#define __comp_bool__arch_has_sse4_2 0
#endif
#ifndef __comp_bool__arch_has_avx
#define __comp_bool__arch_has_avx 0
#endif
#ifndef __comp_bool__arch_has_avx2
#define __comp_bool__arch_has_avx2 0
#endif

#if (arch_is_x86 || arch_is_x64)
#if defined(__SSE__)
#undef __comp_bool__arch_has_sse
#define __comp_bool__arch_has_sse 1
#include <xmmintrin.h>
#endif
#if defined(__SSE2__)
#undef __comp_bool__arch_has_sse2
#define __comp_bool__arch_has_sse2 1
#include <emmintrin.h>
#endif
#if defined(__SSE3__)
#undef __comp_bool__arch_has_sse3
#define __comp_bool__arch_has_sse3 1
#endif
#if defined(__SSSE3__)
#undef __comp_bool__arch_has_ssse3
#define __comp_bool__arch_has_ssse3 1
#endif
#if defined(__SSE4_1__)
#undef __comp_bool__arch_has_sse4_1
#define __comp_bool__arch_has_sse4_1 1
#endif
#if defined(__SSE4_2__)
#undef __comp_bool__arch_has_sse4_2
#define __comp_bool__arch_has_sse4_2 1
#endif
#if defined(__AVX__)
#undef __comp_bool__arch_has_avx
#define __comp_bool__arch_has_avx 1
#endif
#if defined(__AVX2__)
#undef __comp_bool__arch_has_avx2
#define __comp_bool__arch_has_avx2 1
#include <immintrin.h>
#endif
#endif /* x86 or x86_64 */

#ifndef __comp_bool__arch_has_neon
#define __comp_bool__arch_has_neon 0
#endif
#ifndef __comp_bool__arch_has_asimd
#define __comp_bool__arch_has_asimd 0
#endif

#if (arch_is_arm32 || arch_is_arm64)
#if defined(__ARM_NEON__) || defined(__ARM_NEON)
#undef __comp_bool__arch_has_neon
#define __comp_bool__arch_has_neon 1
#include <arm_neon.h>
#endif
/* ARMv8.2+ typically has ASIMD (floating-point + Advanced SIMD) */
#if defined(__ARM_FEATURE_SIMD32) || defined(__aarch64__)
#undef __comp_bool__arch_has_asimd
#define __comp_bool__arch_has_asimd 1
#endif
#endif /* ARM */

/*========== Implementations Cache Line Size (Typical) ======================*/

/**
 * The recommended padding/alignment size to avoid false sharing when
 * atomically updating memory.  In some cases, you may choose an overestimate
 * (e.g. 128 for x86_64) to ensure memory used for atomic ops does not cause
 * invalidation in a neighboring cache line. This macro is a project-specific
 * policy; it may exceed the typical hardware line size in order to further
 * reduce false sharing.
 *
 * References:
 *  - https://en.wikipedia.org/wiki/False_sharing
 *  - https://github.com/golang/go/search?q=CacheLinePadSize
 */
#undef __comp_int__arch_cache_line
#if arch_is_s390x
#define __comp_int__arch_cache_line 256
#elif arch_is_msp430
#define __comp_int__arch_cache_line 8
#elif arch_is_m68k
#define __comp_int__arch_cache_line 16
#elif (arch_is_mips32 || arch_is_mips64 || arch_is_arm32 || arch_is_arm64)
#define __comp_int__arch_cache_line 32
#elif (arch_is_x86 || arch_is_x64)
/* Most x86 and x86_64: 64 bytes typical in L1. */
#define __comp_int__arch_cache_line 64
#elif (arch_is_riscv32 || arch_is_riscv64)
/* Some RISC-V cores implement 32 or 64 bytes; 64 is common in new designs. */
#define __comp_int__arch_cache_line 64
#elif arch_is_powerpc
#define __comp_int__arch_cache_line 128 /* some PPC L1 lines are 128, vary by CPU */
#else
#define __comp_int__arch_cache_line 64 /* Fallback for unknown arches */
#endif

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* builtin_arch_cfg__included */

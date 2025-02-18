/**
 * @copyright Copyright 2024-2025. Gyeongtae Kim All rights reserved.
 *
 * @file    arch_cfg.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2024-10-27 (date of creation)
 * @updated 2025-01-22 (date of last update)
 * @version v0.1-alpha.1
 * @ingroup dasae-headers(dh)/bti
 * @prefix  bti_arch
 *
 * @brief   Architecture detection configuration
 * @details Detects CPU architecture, instruction sets, and hardware capabilities.
 *          Includes CPU family, SIMD support, and architecture-specific features.
 */

#ifndef BTI_ARCH_CFG_INCLUDED
#define BTI_ARCH_CFG_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Baseline Architecture Detection ================================*/

#define bti_arch_x64     VAL__bti_arch_x64
#define bti_arch_x86     VAL__bti_arch_x86
#define bti_arch_arm64   VAL__bti_arch_arm64
#define bti_arch_arm32   VAL__bti_arch_arm32
#define bti_arch_mips32  VAL__bti_arch_mips32
#define bti_arch_mips64  VAL__bti_arch_mips64
#define bti_arch_s390x   VAL__bti_arch_s390x
#define bti_arch_m68k    VAL__bti_arch_m68k
#define bti_arch_msp430  VAL__bti_arch_msp430
#define bti_arch_riscv32 VAL__bti_arch_riscv32
#define bti_arch_riscv64 VAL__bti_arch_riscv64
#define bti_arch_powerpc VAL__bti_arch_powerpc

#define bti_arch_family_x86     VAL__bti_arch_family_x86
#define bti_arch_family_arm     VAL__bti_arch_family_arm
#define bti_arch_family_mips    VAL__bti_arch_family_mips
#define bti_arch_family_systemz VAL__bti_arch_family_systemz
#define bti_arch_family_m68k    VAL__bti_arch_family_m68k
#define bti_arch_family_msp430  VAL__bti_arch_family_msp430
#define bti_arch_family_riscv   VAL__bti_arch_family_riscv
#define bti_arch_family_powerpc VAL__bti_arch_family_powerpc

#define bti_arch_name        VAL__bti_arch_name
#define bti_arch_family_name VAL__bti_arch_family_name

/*========== Additional Feature Detection ===================================*/

#define bti_arch_has_armv6k      VAL__bti_arch_has_armv6k
#define bti_arch_has_armv6m      VAL__bti_arch_has_armv6m
#define bti_arch_has_zihintpause VAL__bti_arch_has_zihintpause

/* x86 extensions */
#define bti_arch_has_avx512f       VAL__bti_arch_has_avx512f
#define bti_arch_has_mmx           VAL__bti_arch_has_mmx
#define bti_arch_has_3dnow         VAL__bti_arch_has_3dnow
#define bti_arch_has_prefer_256bit VAL__bti_arch_has_prefer_256bit
#define bti_arch_has_prefer_128bit VAL__bti_arch_has_prefer_128bit

/* ARM64 extension */
#define bti_arch_has_sve VAL__bti_arch_has_sve

/* PowerPC extension */
#define bti_arch_has_altivec VAL__bti_arch_has_altivec

/* RISC-V extension */
#define bti_arch_has_v VAL__bti_arch_has_v

/*========== SIMD Support Detection (x86 SSE, ARM NEON, etc.) =================
 * These are examples for x86 SSE/AVX detection and ARM NEON/ASIMD detection.
 * Adjust or expand for other ISAs as needed.
 *===========================================================================*/

#define bti_arch_has_sse    VAL__bti_arch_has_sse
#define bti_arch_has_sse2   VAL__bti_arch_has_sse2
#define bti_arch_has_sse3   VAL__bti_arch_has_sse3
#define bti_arch_has_ssse3  VAL__bti_arch_has_ssse3
#define bti_arch_has_sse4_1 VAL__bti_arch_has_sse4_1
#define bti_arch_has_sse4_2 VAL__bti_arch_has_sse4_2
#define bti_arch_has_avx    VAL__bti_arch_has_avx
#define bti_arch_has_avx2   VAL__bti_arch_has_avx2

#define bti_arch_has_neon  VAL__bti_arch_has_neon
#define bti_arch_has_asimd VAL__bti_arch_has_asimd

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

#define bti_arch_cache_line VAL__bti_arch_cache_line

/*========== Implementations CPU Architecture Detection =====================*/
/*========== CPU Architecture / Feature Macros (Initial Defaults) ===========*/

#define VAL__bti_arch_x64     0
#define VAL__bti_arch_x86     0
#define VAL__bti_arch_arm64   0
#define VAL__bti_arch_arm32   0
#define VAL__bti_arch_mips32  0
#define VAL__bti_arch_mips64  0
#define VAL__bti_arch_s390x   0
#define VAL__bti_arch_m68k    0
#define VAL__bti_arch_msp430  0
#define VAL__bti_arch_riscv32 0
#define VAL__bti_arch_riscv64 0
#define VAL__bti_arch_powerpc 0

#define VAL__bti_arch_family_x86     0
#define VAL__bti_arch_family_arm     0
#define VAL__bti_arch_family_mips    0
#define VAL__bti_arch_family_systemz 0
#define VAL__bti_arch_family_m68k    0
#define VAL__bti_arch_family_msp430  0
#define VAL__bti_arch_family_riscv   0
#define VAL__bti_arch_family_powerpc 0

#define VAL__bti_arch_has_armv6k      0
#define VAL__bti_arch_has_armv6m      0
#define VAL__bti_arch_has_zihintpause 0

#define VAL__bti_arch_name        "Unknown"
#define VAL__bti_arch_family_name "Unknown"

/* x86/AMD64 features */
#define VAL__bti_arch_has_avx512f       0
#define VAL__bti_arch_has_mmx           0
#define VAL__bti_arch_has_3dnow         0
#define VAL__bti_arch_has_prefer_256bit 0
#define VAL__bti_arch_has_prefer_128bit 0

/* ARM64 features */
#define VAL__bti_arch_has_sve 0

/* RISC-V features */
#define VAL__bti_arch_has_v 0

/* PowerPC features */
#define VAL__bti_arch_has_altivec 0

/* Existing SSE/AVX/NEON macros (already known or set to 0) */
#ifndef VAL__bti_arch_has_sse
#define VAL__bti_arch_has_sse 0
#endif
#ifndef VAL__bti_arch_has_sse2
#define VAL__bti_arch_has_sse2 0
#endif
#ifndef VAL__bti_arch_has_sse3
#define VAL__bti_arch_has_sse3 0
#endif
#ifndef VAL__bti_arch_has_ssse3
#define VAL__bti_arch_has_ssse3 0
#endif
#ifndef VAL__bti_arch_has_sse4_1
#define VAL__bti_arch_has_sse4_1 0
#endif
#ifndef VAL__bti_arch_has_sse4_2
#define VAL__bti_arch_has_sse4_2 0
#endif
#ifndef VAL__bti_arch_has_avx
#define VAL__bti_arch_has_avx 0
#endif
#ifndef VAL__bti_arch_has_avx2
#define VAL__bti_arch_has_avx2 0
#endif

#ifndef VAL__bti_arch_has_neon
#define VAL__bti_arch_has_neon 0
#endif
#ifndef VAL__bti_arch_has_asimd
#define VAL__bti_arch_has_asimd 0
#endif

/*======== Architecture Detection ===========================================*/

/* Detect x86_64 / x86 */
#if defined(__x86_64__) || defined(_M_X64)
#undef VAL__bti_arch_x64
#define VAL__bti_arch_x64 1
#undef VAL__bti_arch_family_x86
#define VAL__bti_arch_family_x86 1
#undef VAL__bti_arch_name
#define VAL__bti_arch_name "x86_64"
#undef VAL__bti_arch_family_name
#define VAL__bti_arch_family_name "x86"

#elif defined(__i386__) || defined(_M_IX86)
#undef VAL__bti_arch_x86
#define VAL__bti_arch_x86 1
#undef VAL__bti_arch_family_x86
#define VAL__bti_arch_family_x86 1
#undef VAL__bti_arch_name
#define VAL__bti_arch_name "x86"
#undef VAL__bti_arch_family_name
#define VAL__bti_arch_family_name "x86"

/* Detect ARM64 / ARM32 */
#elif defined(__aarch64__) || defined(_M_ARM64)
#undef VAL__bti_arch_arm64
#define VAL__bti_arch_arm64 1
#undef VAL__bti_arch_family_arm
#define VAL__bti_arch_family_arm 1
#undef VAL__bti_arch_name
#define VAL__bti_arch_name "ARM64"
#undef VAL__bti_arch_family_name
#define VAL__bti_arch_family_name "ARM"

#elif defined(__arm__) || defined(_M_ARM)
#undef VAL__bti_arch_arm32
#define VAL__bti_arch_arm32 1
#undef VAL__bti_arch_family_arm
#define VAL__bti_arch_family_arm 1
#undef VAL__bti_arch_name
#define VAL__bti_arch_name "ARM"
#undef VAL__bti_arch_family_name
#define VAL__bti_arch_family_name "ARM"

/* Detect MIPS (32 or 64) */
#elif defined(__mips__)
#if defined(__mips64) || defined(__mips64__)
#undef VAL__bti_arch_mips64
#define VAL__bti_arch_mips64 1
#undef VAL__bti_arch_family_mips
#define VAL__bti_arch_family_mips 1
#undef VAL__bti_arch_name
#define VAL__bti_arch_name "MIPS64"
#else
#undef VAL__bti_arch_mips32
#define VAL__bti_arch_mips32 1
#undef VAL__bti_arch_family_mips
#define VAL__bti_arch_family_mips 1
#undef VAL__bti_arch_name
#define VAL__bti_arch_name "MIPS"
#endif
#undef VAL__bti_arch_family_name
#define VAL__bti_arch_family_name "MIPS"

/* Detect s390x */
#elif defined(__s390x__)
#undef VAL__bti_arch_s390x
#define VAL__bti_arch_s390x 1
#undef VAL__bti_arch_family_systemz
#define VAL__bti_arch_family_systemz 1
#undef VAL__bti_arch_name
#define VAL__bti_arch_name "s390x"
#undef VAL__bti_arch_family_name
#define VAL__bti_arch_family_name "SystemZ"

/* Detect m68k */
#elif defined(__m68k__)
#undef VAL__bti_arch_m68k
#define VAL__bti_arch_m68k 1
#undef VAL__bti_arch_family_m68k
#define VAL__bti_arch_family_m68k 1
#undef VAL__bti_arch_name
#define VAL__bti_arch_name "m68k"
#undef VAL__bti_arch_family_name
#define VAL__bti_arch_family_name "m68k"

/* Detect MSP430 (very uncommon in hosted environments) */
#elif defined(__MSP430__)
#undef VAL__bti_arch_msp430
#define VAL__bti_arch_msp430 1
#undef VAL__bti_arch_family_msp430
#define VAL__bti_arch_family_msp430 1
#undef VAL__bti_arch_name
#define VAL__bti_arch_name "MSP430"
#undef VAL__bti_arch_family_name
#define VAL__bti_arch_family_name "MSP430"

/* Detect RISC-V (32 or 64).  For reference:
 *   __riscv_xlen = 32 or 64,
 *   __riscv = 1 implies some RISC-V toolchain. */
#elif defined(__riscv)
#if (__riscv_xlen == 64)
#undef VAL__bti_arch_riscv64
#define VAL__bti_arch_riscv64 1
#undef VAL__bti_arch_family_riscv
#define VAL__bti_arch_family_riscv 1
#undef VAL__bti_arch_name
#define VAL__bti_arch_name "RISC-V64"
#else
#undef VAL__bti_arch_riscv32
#define VAL__bti_arch_riscv32 1
#undef VAL__bti_arch_family_riscv
#define VAL__bti_arch_family_riscv 1
#undef VAL__bti_arch_name
#define VAL__bti_arch_name "RISC-V32"
#endif
#undef VAL__bti_arch_family_name
#define VAL__bti_arch_family_name "RISC-V"

/* PowerPC / Power Architecture */
#elif defined(__powerpc__) || defined(__powerpc64__) || defined(_M_PPC)
#undef VAL__bti_arch_powerpc
#define VAL__bti_arch_powerpc 1
#undef VAL__bti_arch_family_powerpc
#define VAL__bti_arch_family_powerpc 1
#if defined(__powerpc64__) || defined(__ppc64__)
#undef VAL__bti_arch_name
#define VAL__bti_arch_name "PowerPC64"
#else
#undef VAL__bti_arch_name
#define VAL__bti_arch_name "PowerPC"
#endif
#undef VAL__bti_arch_family_name
#define VAL__bti_arch_family_name "PowerPC"

#else
#warning "Unknown architecture detected. Please check your architecture settings."
#endif

/*========== Feature Detection: ARMv6K, ARMv6M, RISC-V Zihintpause ==========*/

/*
 * On ARM, __ARM_ARCH_6K__ or __ARM_ARCH_6KZ__ indicate ARMv6K / kernel support,
 * and __ARM_ARCH_6M__ indicates ARMv6M (Cortex-M). Adjust as needed.
 */
#if defined(__ARM_ARCH_6K__) || defined(__ARM_ARCH_6KZ__)
#undef VAL__bti_arch_has_armv6k
#define VAL__bti_arch_has_armv6k 1
#endif
#if defined(__ARM_ARCH_6M__)
#undef VAL__bti_arch_has_armv6m
#define VAL__bti_arch_has_armv6m 1
#endif

/*
 * For RISC-V, some toolchains define __riscv_zihintpause or similar macros
 * when the 'Zihintpause' extension is supported.
 */
#if defined(__riscv_zihintpause) /* or check your compiler docs */
#undef VAL__bti_arch_has_zihintpause
#define VAL__bti_arch_has_zihintpause 1
#endif

/*======== Feature Detection: x86/AMD64 Extra (MMX, 3DNow, AVX512) ==========*/

/*
 * Common GCC/Clang macros:
 *   - __MMX__ => MMX
 *   - __3dNOW__ => 3DNow
 *   - __AVX512F__ => AVX-512 Foundation
 */
#if (VAL__bti_arch_x86 || VAL__bti_arch_x64)
#if defined(__MMX__)
#undef VAL__bti_arch_has_mmx
#define VAL__bti_arch_has_mmx 1
#endif

#if defined(__3dNOW__)
#undef VAL__bti_arch_has_3dnow
#define VAL__bti_arch_has_3dnow 1
#endif

#if defined(__AVX512F__)
#undef VAL__bti_arch_has_avx512f
#define VAL__bti_arch_has_avx512f 1
#endif
#endif /* x86/x64 */

/* Example "preferred widths" (often project-specific).
 * For instance, you might define them via build-system flags:
 *   -DVAL__bti_arch_has_prefer_256bit=1
 *   or do runtime checks for microarchitecture.
 * Here we leave them as 0 unless you define them externally. */

/*======== Feature Detection: ARM64 SVE =====================================*/

/*
 * Some compilers define __ARM_FEATURE_SVE when SVE is enabled,
 * or a similar macro. Check your compiler docs for exact naming.
 */
#if VAL__bti_arch_arm64
#if defined(__ARM_FEATURE_SVE)
#undef VAL__bti_arch_has_sve
#define VAL__bti_arch_has_sve 1
#endif
#endif

/*======== Feature Detection: PowerPC AltiVec ===============================*/

/*
 * AltiVec: Some compilers define __ALTIVEC__ or __VEC__
 *
 * Clang/GCC often define __ALTIVEC__ when using -maltivec or on
 * certain default configurations for PowerPC.
 */
#if VAL__bti_arch_powerpc
#if defined(__ALTIVEC__) || defined(__VEC__)
#undef VAL__bti_arch_has_altivec
#define VAL__bti_arch_has_altivec 1
#endif
#endif

/*======== Feature Detection: RISC-V Vector (V) =============================*/

/*
 * RISC-V vector extension: Some compilers define __riscv_vector.
 * Adjust if your compiler uses a different macro for v-extension.
 */
#if (VAL__bti_arch_riscv32 || VAL__bti_arch_riscv64)
#if defined(__riscv_vector)
#undef VAL__bti_arch_has_v
#define VAL__bti_arch_has_v 1
#endif
#endif

/*========== Implementations SIMD Support Detection =========================*/

#ifndef VAL__bti_arch_has_sse
#define VAL__bti_arch_has_sse 0
#endif
#ifndef VAL__bti_arch_has_sse2
#define VAL__bti_arch_has_sse2 0
#endif
#ifndef VAL__bti_arch_has_sse3
#define VAL__bti_arch_has_sse3 0
#endif
#ifndef VAL__bti_arch_has_ssse3
#define VAL__bti_arch_has_ssse3 0
#endif
#ifndef VAL__bti_arch_has_sse4_1
#define VAL__bti_arch_has_sse4_1 0
#endif
#ifndef VAL__bti_arch_has_sse4_2
#define VAL__bti_arch_has_sse4_2 0
#endif
#ifndef VAL__bti_arch_has_avx
#define VAL__bti_arch_has_avx 0
#endif
#ifndef VAL__bti_arch_has_avx2
#define VAL__bti_arch_has_avx2 0
#endif

#if (bti_arch_x86 || bti_arch_x64)
#if defined(__SSE__)
#undef VAL__bti_arch_has_sse
#define VAL__bti_arch_has_sse 1
#endif
#if defined(__SSE2__)
#undef VAL__bti_arch_has_sse2
#define VAL__bti_arch_has_sse2 1
#endif
#if defined(__SSE3__)
#undef VAL__bti_arch_has_sse3
#define VAL__bti_arch_has_sse3 1
#endif
#if defined(__SSSE3__)
#undef VAL__bti_arch_has_ssse3
#define VAL__bti_arch_has_ssse3 1
#endif
#if defined(__SSE4_1__)
#undef VAL__bti_arch_has_sse4_1
#define VAL__bti_arch_has_sse4_1 1
#endif
#if defined(__SSE4_2__)
#undef VAL__bti_arch_has_sse4_2
#define VAL__bti_arch_has_sse4_2 1
#endif
#if defined(__AVX__)
#undef VAL__bti_arch_has_avx
#define VAL__bti_arch_has_avx 1
#endif
#if defined(__AVX2__)
#undef VAL__bti_arch_has_avx2
#define VAL__bti_arch_has_avx2 1
#endif
#endif /* x86 or x86_64 */

#ifndef VAL__bti_arch_has_neon
#define VAL__bti_arch_has_neon 0
#endif
#ifndef VAL__bti_arch_has_asimd
#define VAL__bti_arch_has_asimd 0
#endif

#if (bti_arch_arm32 || bti_arch_arm64)
#if defined(__ARM_NEON__) || defined(__ARM_NEON)
#undef VAL__bti_arch_has_neon
#define VAL__bti_arch_has_neon 1
#endif
/* ARMv8.2+ typically has ASIMD (floating-point + Advanced SIMD) */
#if defined(__ARM_FEATURE_SIMD32) || defined(__aarch64__)
#undef VAL__bti_arch_has_asimd
#define VAL__bti_arch_has_asimd 1
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
#undef VAL__bti_arch_cache_line
#if bti_arch_s390x
#define VAL__bti_arch_cache_line 256
#elif bti_arch_msp430
#define VAL__bti_arch_cache_line 8
#elif bti_arch_m68k
#define VAL__bti_arch_cache_line 16
#elif (bti_arch_mips32 || bti_arch_mips64 || bti_arch_arm32 || bti_arch_arm64)
#define VAL__bti_arch_cache_line 32
#elif (bti_arch_x86 || bti_arch_x64)
/* Most x86 and x86_64: 64 bytes typical in L1. */
#define VAL__bti_arch_cache_line 64
#elif (bti_arch_riscv32 || bti_arch_riscv64)
/* Some RISC-V cores implement 32 or 64 bytes; 64 is common in new designs. */
#define VAL__bti_arch_cache_line 64
#elif bti_arch_powerpc
#define VAL__bti_arch_cache_line 128 /* some PPC L1 lines are 128, vary by CPU */
#else
#define VAL__bti_arch_cache_line 64 /* Fallback for unknown arches */
#endif

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* BTI_ARCH_CFG_INCLUDED */

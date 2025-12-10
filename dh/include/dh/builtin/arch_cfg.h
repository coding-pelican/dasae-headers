/**
 * @copyright Copyright (c) 2024-2025 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    arch_cfg.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2024-10-27 (date of creation)
 * @updated 2025-11-27 (date of last update)
 * @version v0.1
 * @ingroup dasae-headers(dh)/foundation
 * @prefix  arch
 *
 * @brief   Architecture detection and configuration
 * @details Detects CPU architecture and hardware capabilities.
 *          Focus: x86_64, x86, aarch64, ARM, RISC-V, wasm64, wasm32.
 *          Single source of truth: all properties derived from arch_type.
 */
#ifndef foundation_arch_cfg__included
#define foundation_arch_cfg__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "pp.h"

/*========== Macros and Declarations ========================================*/

/* --- Architecture Type --- */

#define arch_type __comp_enum__arch_type
#define arch_type_unknown __comp_enum__arch_type_unknown
#define arch_type_x86_64 __comp_enum__arch_type_x86_64
#define arch_type_x86 __comp_enum__arch_type_x86
#define arch_type_aarch64 __comp_enum__arch_type_aarch64
#define arch_type_arm __comp_enum__arch_type_arm
#define arch_type_riscv64 __comp_enum__arch_type_riscv64
#define arch_type_riscv32 __comp_enum__arch_type_riscv32
#define arch_type_wasm64 __comp_enum__arch_type_wasm64
#define arch_type_wasm32 __comp_enum__arch_type_wasm32

#define arch_is_x86_64 __comp_bool__arch_is_x86_64
#define arch_is_x86 __comp_bool__arch_is_x86
#define arch_is_aarch64 __comp_bool__arch_is_aarch64
#define arch_is_arm __comp_bool__arch_is_arm
#define arch_is_riscv64 __comp_bool__arch_is_riscv64
#define arch_is_riscv32 __comp_bool__arch_is_riscv32
#define arch_is_wasm64 __comp_bool__arch_is_wasm64
#define arch_is_wasm32 __comp_bool__arch_is_wasm32

/* --- Architecture Name --- */

#define arch_name __comp_str__arch_name
#define arch_name_unknown __comp_str__arch_name_unknown
#define arch_name_x86_64 __comp_str__arch_name_x86_64
#define arch_name_x86 __comp_str__arch_name_x86
#define arch_name_aarch64 __comp_str__arch_name_aarch64
#define arch_name_arm __comp_str__arch_name_arm
#define arch_name_riscv64 __comp_str__arch_name_riscv64
#define arch_name_riscv32 __comp_str__arch_name_riscv32
#define arch_name_wasm64 __comp_str__arch_name_wasm64
#define arch_name_wasm32 __comp_str__arch_name_wasm32

/* --- Architecture Family --- */

#define arch_family_type __comp_enum__arch_family_type
#define arch_family_type_unknown __comp_enum__arch_family_type_unknown
#define arch_family_type_x86 __comp_enum__arch_family_type_x86
#define arch_family_type_arm __comp_enum__arch_family_type_arm
#define arch_family_type_riscv __comp_enum__arch_family_type_riscv
#define arch_family_type_wasm __comp_enum__arch_family_type_wasm

#define arch_is_x86_family __comp_bool__arch_is_x86_family
#define arch_is_arm_family __comp_bool__arch_is_arm_family
#define arch_is_riscv_family __comp_bool__arch_is_riscv_family
#define arch_is_wasm_family __comp_bool__arch_is_wasm_family

/* --- Architecture Family name */

#define arch_family_name __comp_str__arch_family_name
#define arch_family_name_unknown __comp_str__arch_family_name_unknown
#define arch_family_name_x86 __comp_str__arch_family_name_x86
#define arch_family_name_arm __comp_str__arch_family_name_arm
#define arch_family_name_riscv __comp_str__arch_family_name_riscv
#define arch_family_name_wasm __comp_str__arch_family_name_wasm

/* --- Bit Width --- */

#define arch_bits_unit __comp_enum__arch_bits_unit
#define arch_bits_unit_unknown __comp_enum__arch_bits_unit_unknown
#define arch_bits_unit_64bit __comp_enum__arch_bits_unit_64bit
#define arch_bits_unit_32bit __comp_enum__arch_bits_unit_32bit

#define arch_bits_per_crumb __comp_int__arch_bits_per_crumb
#define arch_bits_per_nibble __comp_int__arch_bits_per_nibble
#define arch_bits_per_byte __comp_int__arch_bits_per_byte
#define arch_bits_per_word __comp_int__arch_bits_per_word

#define arch_bits_wide __comp_int__arch_bits_wide
#define arch_bits_wide_unknown __comp_int__arch_bits_wide_unknown
#define arch_bits_wide_64bit __comp_int__arch_bits_wide_64bit
#define arch_bits_wide_32bit __comp_int__arch_bits_wide_32bit

#define arch_bits_is_64bit __comp_bool__arch_bits_is_64bit
#define arch_bits_is_32bit __comp_bool__arch_bits_is_32bit

/* --- Endianness --- */

#define arch_byte_order __comp_enum__arch_byte_order
#define arch_byte_order_little_endian __comp_enum__arch_byte_order_little_endian
#define arch_byte_order_big_endian __comp_enum__arch_byte_order_big_endian
#define arch_byte_order_is_little_endian __comp_bool__arch_byte_order_is_little_endian
#define arch_byte_order_is_big_endian __comp_bool__arch_byte_order_is_big_endian

/* --- Cache Line --- */

#define arch_cache_line_bytes __comp_int__arch_cache_line_bytes

/* --- SIMD Feature --- */
/* SIMD Intrinsic Headers
 * This is for reference on what headers correspond to what SIMD features:
 *
 * x86/x86_64:
 *   - SSE/SSE2:      <emmintrin.h> or <xmmintrin.h>
 *   - SSE3:          <pmmintrin.h>
 *   - SSSE3:         <tmmintrin.h>
 *   - SSE4.1/4.2:    <smmintrin.h> / <nmmintrin.h>
 *   - AVX/AVX2/AVX512/FMA: <immintrin.h> (unified header)
 *
 * ARM:
 *   - NEON:          <arm_neon.h>
 *   - SVE:           <arm_sve.h>
 *
 * RISC-V:
 *   - RVV:           <riscv_vector.h>
 */

/* --- x86/x86_64 SIMD Features --- */
#define arch_has_sse __comp_bool__arch_has_sse
#define arch_has_sse2 __comp_bool__arch_has_sse2
#define arch_has_sse3 __comp_bool__arch_has_sse3
#define arch_has_ssse3 __comp_bool__arch_has_ssse3
#define arch_has_sse4_1 __comp_bool__arch_has_sse4_1
#define arch_has_sse4_2 __comp_bool__arch_has_sse4_2
#define arch_has_avx __comp_bool__arch_has_avx
#define arch_has_avx2 __comp_bool__arch_has_avx2
#define arch_has_avx512f __comp_bool__arch_has_avx512f
#define arch_has_fma __comp_bool__arch_has_fma
/* --- ARM SIMD Features --- */
#define arch_has_neon __comp_bool__arch_has_neon
#define arch_has_sve __comp_bool__arch_has_sve
/* --- RISC-V Vector Extension --- */
#define arch_has_rvv __comp_bool__arch_has_rvv

/* --- SIMD Availability Summary --- */

#define arch_simd_use __comp_bool__arch_simd_use
#define arch_simd_width_bits __comp_int__arch_simd_width_bits
#define arch_simd_align_bytes __comp_int__arch_simd_align_bytes

/*========== Macros and Definitions =========================================*/

/* --- Architecture Type --- */

/* Default: unknown architecture */
#define __comp_enum__arch_type arch_type_unknown
#define __comp_enum__arch_type_unknown 0
#define __comp_enum__arch_type_x86_64 1
#define __comp_enum__arch_type_x86 2
#define __comp_enum__arch_type_aarch64 3
#define __comp_enum__arch_type_arm 4
#define __comp_enum__arch_type_riscv64 5
#define __comp_enum__arch_type_riscv32 6
#define __comp_enum__arch_type_wasm64 7
#define __comp_enum__arch_type_wasm32 8

#define __comp_bool__arch_is_x86_64 pp_Tok_eq(arch_type, arch_type_x86_64)
#define __comp_bool__arch_is_x86 pp_Tok_eq(arch_type, arch_type_x86)
#define __comp_bool__arch_is_aarch64 pp_Tok_eq(arch_type, arch_type_aarch64)
#define __comp_bool__arch_is_arm pp_Tok_eq(arch_type, arch_type_arm)
#define __comp_bool__arch_is_riscv64 pp_Tok_eq(arch_type, arch_type_riscv64)
#define __comp_bool__arch_is_riscv32 pp_Tok_eq(arch_type, arch_type_riscv32)
#define __comp_bool__arch_is_wasm64 pp_Tok_eq(arch_type, arch_type_wasm64)
#define __comp_bool__arch_is_wasm32 pp_Tok_eq(arch_type, arch_type_wasm32)

/* Detect x86_64 */
#if defined(__x86_64__) || defined(_M_X64)
#undef __comp_enum__arch_type
#define __comp_enum__arch_type arch_type_x86_64

/* Detect x86 (32-bit) */
#elif defined(__i386__) || defined(_M_IX86)
#undef __comp_enum__arch_type
#define __comp_enum__arch_type arch_type_x86

/* Detect ARM64 (aarch64) */
#elif defined(__aarch64__) || defined(_M_ARM64)
#undef __comp_enum__arch_type
#define __comp_enum__arch_type arch_type_aarch64

/* Detect ARM (32-bit) */
#elif defined(__arm__) || defined(_M_ARM)
#undef __comp_enum__arch_type
#define __comp_enum__arch_type arch_type_arm

/* Detect RISC-V */
#elif defined(__riscv)
#if defined(__riscv_xlen) && (__riscv_xlen == 64)
#undef __comp_enum__arch_type
#define __comp_enum__arch_type arch_type_riscv64
#else
#undef __comp_enum__arch_type
#define __comp_enum__arch_type arch_type_riscv32
#endif

/* Detect WebAssembly */
#elif defined(__wasm__) || defined(__EMSCRIPTEN__)
#if defined(__wasm64__) || defined(__wasm64)
#undef __comp_enum__arch_type
#define __comp_enum__arch_type arch_type_wasm64
#else
#undef __comp_enum__arch_type
#define __comp_enum__arch_type arch_type_wasm32
#endif

#else
#warning "Unknown architecture detected. Please check your architecture settings."
#endif

/* --- Architecture Name --- */

/* Derive name from type */
#define __comp_str__arch_name pp_expand( \
    pp_switch_ pp_begin(arch_type)( \
        pp_case_((arch_type_x86_64)(arch_name_x86_64)), \
        pp_case_((arch_type_x86)(arch_name_x86)), \
        pp_case_((arch_type_aarch64)(arch_name_aarch64)), \
        pp_case_((arch_type_arm)(arch_name_arm)), \
        pp_case_((arch_type_riscv64)(arch_name_riscv64)), \
        pp_case_((arch_type_riscv32)(arch_name_riscv32)), \
        pp_case_((arch_type_wasm64)(arch_name_wasm64)), \
        pp_case_((arch_type_wasm32)(arch_name_wasm32)), \
        pp_default_(arch_name_unknown) \
    ) pp_end \
)
#define __comp_str__arch_name_unknown "unknown"
#define __comp_str__arch_name_x86_64 "x86_64"
#define __comp_str__arch_name_x86 "x86"
#define __comp_str__arch_name_aarch64 "aarch64"
#define __comp_str__arch_name_arm "ARM"
#define __comp_str__arch_name_riscv64 "RISC-V64"
#define __comp_str__arch_name_riscv32 "RISC-V32"
#define __comp_str__arch_name_wasm64 "wasm64"
#define __comp_str__arch_name_wasm32 "wasm32"

/* --- Architecture Family --- */

/* Derive family from type */
#define __comp_enum__arch_family_type pp_expand( \
    pp_switch_ pp_begin(arch_type)( \
        pp_case_((arch_type_x86_64)(arch_family_type_x86)), \
        pp_case_((arch_type_x86)(arch_family_type_x86)), \
        pp_case_((arch_type_aarch64)(arch_family_type_arm)), \
        pp_case_((arch_type_arm)(arch_family_type_arm)), \
        pp_case_((arch_type_riscv64)(arch_family_type_riscv)), \
        pp_case_((arch_type_riscv32)(arch_family_type_riscv)), \
        pp_case_((arch_type_wasm64)(arch_family_type_wasm)), \
        pp_case_((arch_type_wasm32)(arch_family_type_wasm)), \
        pp_default_(arch_family_type_unknown) \
    ) pp_end \
)
#define __comp_enum__arch_family_type_unknown 0
#define __comp_enum__arch_family_type_x86 1
#define __comp_enum__arch_family_type_arm 2
#define __comp_enum__arch_family_type_riscv 3
#define __comp_enum__arch_family_type_wasm 4

#define __comp_bool__arch_is_x86_family pp_Tok_eq(arch_family_type, arch_family_type_x86)
#define __comp_bool__arch_is_arm_family pp_Tok_eq(arch_family_type, arch_family_type_arm)
#define __comp_bool__arch_is_riscv_family pp_Tok_eq(arch_family_type, arch_family_type_riscv)
#define __comp_bool__arch_is_wasm_family pp_Tok_eq(arch_family_type, arch_family_type_wasm)

/* Derive family name from type */
#define __comp_str__arch_family_name pp_expand( \
    pp_switch_ pp_begin(arch_family_type)( \
        pp_case_((arch_family_type_x86)(arch_family_name_x86)), \
        pp_case_((arch_family_type_arm)(arch_family_name_arm)), \
        pp_case_((arch_family_type_riscv)(arch_family_name_riscv)), \
        pp_case_((arch_family_type_wasm)(arch_family_name_wasm)), \
        pp_default_(arch_family_name_unknown) \
    ) pp_end \
)
#define __comp_str__arch_family_name_unknown "unknown"
#define __comp_str__arch_family_name_x86 "x86"
#define __comp_str__arch_family_name_arm "ARM"
#define __comp_str__arch_family_name_riscv "RISC-V"
#define __comp_str__arch_family_name_wasm "WebAssembly"

/* --- Bit Width --- */

/* Derive bit width unit from type */
#define __comp_enum__arch_bits_unit pp_expand( \
    pp_switch_ pp_begin(arch_type)( \
        pp_case_((arch_type_x86_64)(arch_bits_unit_64bit)), \
        pp_case_((arch_type_x86)(arch_bits_unit_32bit)), \
        pp_case_((arch_type_aarch64)(arch_bits_unit_64bit)), \
        pp_case_((arch_type_arm)(arch_bits_unit_32bit)), \
        pp_case_((arch_type_riscv64)(arch_bits_unit_64bit)), \
        pp_case_((arch_type_riscv32)(arch_bits_unit_32bit)), \
        pp_case_((arch_type_wasm64)(arch_bits_unit_64bit)), \
        pp_case_((arch_type_wasm32)(arch_bits_unit_32bit)), \
        pp_default_(arch_bits_unit_unknown) \
    ) pp_end \
)
#define __comp_enum__arch_bits_unit_unknown 0
#define __comp_enum__arch_bits_unit_64bit 1
#define __comp_enum__arch_bits_unit_32bit 2

/* Derive bits per byte from family */
#define __comp_int__arch_bits_per_crumb 2
#define __comp_int__arch_bits_per_nibble 4
#define __comp_int__arch_bits_per_byte 8
#define __comp_int__arch_bits_per_word arch_bits_wide

/* Derive bit width from unit */
#define __comp_int__arch_bits_wide pp_expand( \
    pp_switch_ pp_begin(arch_bits_unit)( \
        pp_case_((arch_bits_unit_64bit)(arch_bits_wide_64bit)), \
        pp_case_((arch_bits_unit_32bit)(arch_bits_wide_32bit)), \
        pp_default_(arch_bits_wide_unknown) \
    ) pp_end \
)
#define __comp_int__arch_bits_wide_unknown 0
#define __comp_int__arch_bits_wide_64bit 64
#define __comp_int__arch_bits_wide_32bit 32

#define __comp_bool__arch_bits_is_64bit pp_eq(arch_bits_unit, arch_bits_unit_64bit)
#define __comp_bool__arch_bits_is_32bit pp_eq(arch_bits_unit, arch_bits_unit_32bit)

/* --- Endianness --- */

/* Default to little-endian */
#define __comp_enum__arch_byte_order arch_byte_order_little_endian
#define __comp_enum__arch_byte_order_little_endian 0
#define __comp_enum__arch_byte_order_big_endian 1
#define __comp_bool__arch_byte_order_is_little_endian pp_eq(arch_byte_order, arch_byte_order_little_endian)
#define __comp_bool__arch_byte_order_is_big_endian pp_eq(arch_byte_order, arch_byte_order_big_endian)

/* Detect byte order */
#if arch_family_type != arch_family_type_wasm
#if defined(__BYTE_ORDER__) && defined(__ORDER_LITTLE_ENDIAN__) && defined(__ORDER_BIG_ENDIAN__)
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
#define __comp_enum__arch_byte_order arch_byte_order_little_endian
#elif __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
#define __comp_enum__arch_byte_order arch_byte_order_big_endian
#else
#warning "Unknown byte order detected. Please check your byte order settings."
#endif
#else /* !defined(__BYTE_ORDER__) || !defined(__ORDER_LITTLE_ENDIAN__) || !defined(__ORDER_BIG_ENDIAN__) */
#warning "No defined byte orders - defaulting to little-endian"
#endif
#endif /* arch_family_type != arch_family_type_wasm */

/* --- Cache Line --- */

/* Derive cache line from family */
#define __comp_int__arch_cache_line_bytes 64

/* --- SIMD Feature --- */

/* Default: no SIMD features */
#define __comp_bool__arch_has_sse 0
#define __comp_bool__arch_has_sse2 0
#define __comp_bool__arch_has_sse3 0
#define __comp_bool__arch_has_ssse3 0
#define __comp_bool__arch_has_sse4_1 0
#define __comp_bool__arch_has_sse4_2 0
#define __comp_bool__arch_has_avx 0
#define __comp_bool__arch_has_avx2 0
#define __comp_bool__arch_has_avx512f 0
#define __comp_bool__arch_has_fma 0
#define __comp_bool__arch_has_neon 0
#define __comp_bool__arch_has_sve 0
#define __comp_bool__arch_has_rvv 0

/* --- x86/x86_64 SIMD Detection --- */

#if arch_is_x86_family

/* SSE (Streaming SIMD Extensions) */
#if defined(__SSE__) || defined(_M_X64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 1)
#undef __comp_bool__arch_has_sse
#define __comp_bool__arch_has_sse 1
#endif /* defined(__SSE__) || defined(_M_X64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 1) */

/* SSE2 */
#if defined(__SSE2__) || defined(_M_X64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 2)
#undef __comp_bool__arch_has_sse2
#define __comp_bool__arch_has_sse2 1
#endif /* defined(__SSE2__) || defined(_M_X64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 2) */

/* SSE3 */
#if defined(__SSE3__)
#undef __comp_bool__arch_has_sse3
#define __comp_bool__arch_has_sse3 1
#endif /* defined(__SSE3__) */

/* SSSE3 (Supplemental SSE3) */
#if defined(__SSSE3__)
#undef __comp_bool__arch_has_ssse3
#define __comp_bool__arch_has_ssse3 1
#endif /* defined(__SSSE3__) */

/* SSE4.1 */
#if defined(__SSE4_1__)
#undef __comp_bool__arch_has_sse4_1
#define __comp_bool__arch_has_sse4_1 1
#endif /* defined(__SSE4_1__) */

/* SSE4.2 */
#if defined(__SSE4_2__)
#undef __comp_bool__arch_has_sse4_2
#define __comp_bool__arch_has_sse4_2 1
#endif /* defined(__SSE4_2__) */

/* AVX (Advanced Vector Extensions) */
#if defined(__AVX__)
#undef __comp_bool__arch_has_avx
#define __comp_bool__arch_has_avx 1
#endif /* defined(__AVX__) */

/* AVX2 */
#if defined(__AVX2__)
#undef __comp_bool__arch_has_avx2
#define __comp_bool__arch_has_avx2 1
#endif /* defined(__AVX2__) */

/* AVX-512 Foundation */
#if defined(__AVX512F__)
#undef __comp_bool__arch_has_avx512f
#define __comp_bool__arch_has_avx512f 1
#endif /* defined(__AVX512F__) */

/* FMA (Fused Multiply-Add) */
#if defined(__FMA__)
#undef __comp_bool__arch_has_fma
#define __comp_bool__arch_has_fma 1
#endif /* defined(__FMA__) */

#endif /* arch_is_x86_family */

/* --- ARM SIMD Detection --- */

#if arch_is_arm_family

/* NEON (Advanced SIMD) */
#if defined(__ARM_NEON) || defined(__ARM_NEON__)
#undef __comp_bool__arch_has_neon
#define __comp_bool__arch_has_neon 1
#endif /* defined(__ARM_NEON) || defined(__ARM_NEON__) */

/* SVE (Scalable Vector Extension) */
#if defined(__ARM_FEATURE_SVE)
#undef __comp_bool__arch_has_sve
#define __comp_bool__arch_has_sve 1
#endif /* defined(__ARM_FEATURE_SVE) */

#endif /* arch_is_arm_family */

/* --- RISC-V Vector Extension Detection --- */

#if arch_is_riscv_family

#if defined(__riscv_v) || defined(__riscv_vector)
#undef __comp_bool__arch_has_rvv
#define __comp_bool__arch_has_rvv 1
#endif /* defined(__riscv_v) || defined(__riscv_vector) */

#endif /* arch_is_riscv_family */

/* --- SIMD Availability Summary --- */

/* Determine if any SIMD is available */
#define __comp_bool__arch_simd_use pp_or(arch_has_sse2, pp_or(arch_has_neon, arch_has_rvv))
/* Determine maximum SIMD width */
#define __comp_int__arch_simd_width_bits pp_if_(arch_has_avx512f)( \
    pp_then_(512), \
    pp_else_(pp_if_(pp_or(arch_has_avx, arch_has_avx2))( \
        pp_then_(256), \
        pp_else_(pp_if_(pp_or(arch_has_sse, pp_or(arch_has_sse2, arch_has_neon)))( \
            pp_then_(128), \
            pp_else_(pp_if_(arch_has_rvv)( \
                pp_then_(128), /* RVV width is implementation-defined, assume minimum 128 */ \
                pp_else_(0) \
            )) \
        )) \
    )) \
)
/* Determine SIMD alignment requirement */
#define __comp_int__arch_simd_align_bytes pp_if_(arch_has_avx512f)( \
    pp_then_(64), \
    pp_else_(pp_if_(pp_or(arch_has_avx, arch_has_avx2))( \
        pp_then_(32), \
        pp_else_(pp_if_(pp_or(arch_has_sse, pp_or(arch_has_sse2, pp_or(arch_has_neon, arch_has_rvv))))( \
            pp_then_(16), \
            pp_else_(1) \
        )) \
    )) \
)

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* foundation_arch_cfg__included */

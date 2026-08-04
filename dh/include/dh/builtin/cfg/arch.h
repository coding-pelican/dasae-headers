/**
 * @copyright Copyright (c) 2024-2026 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    arch.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2024-10-27 (date of creation)
 * @updated 2026-06-21 (date of last update)
 * @ingroup dal-project/da/foundation/cfg
 * @prefix  arch
 *
 * @brief   Architecture detection and configuration
 * @details Detects CPU architecture and hardware capabilities.
 *          Focus: x86, ARM, RISC-V, WebAssembly, PowerPC, and Hexagon.
 *          Single source of truth: all properties derived from arch_type.
 */
#pragma once
#ifndef foundation_cfg_arch__included
#define foundation_cfg_arch__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "comp.h"

/*========== Macros and Declarations ========================================*/

/*--- Architecture Type ---*/

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
#define arch_type_avr __comp_enum__arch_type_avr
#define arch_type_mips64 __comp_enum__arch_type_mips64
#define arch_type_mips32 __comp_enum__arch_type_mips32
#define arch_type_powerpc64 __comp_enum__arch_type_powerpc64
#define arch_type_powerpc32 __comp_enum__arch_type_powerpc32
#define arch_type_s390x __comp_enum__arch_type_s390x

#define arch_is_x86_64 __comp_bool__arch_is_x86_64
#define arch_is_x86 __comp_bool__arch_is_x86
#define arch_is_aarch64 __comp_bool__arch_is_aarch64
#define arch_is_arm __comp_bool__arch_is_arm
#define arch_is_riscv64 __comp_bool__arch_is_riscv64
#define arch_is_riscv32 __comp_bool__arch_is_riscv32
#define arch_is_wasm64 __comp_bool__arch_is_wasm64
#define arch_is_wasm32 __comp_bool__arch_is_wasm32
#define arch_is_avr __comp_bool__arch_is_avr
#define arch_is_mips64 __comp_bool__arch_is_mips64
#define arch_is_mips32 __comp_bool__arch_is_mips32
#define arch_is_powerpc64 __comp_bool__arch_is_powerpc64
#define arch_is_powerpc32 __comp_bool__arch_is_powerpc32
#define arch_is_s390x __comp_bool__arch_is_s390x

/*--- Architecture Name ---*/

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
#define arch_name_avr __comp_str__arch_name_avr
#define arch_name_mips64 __comp_str__arch_name_mips64
#define arch_name_mips32 __comp_str__arch_name_mips32
#define arch_name_powerpc64 __comp_str__arch_name_powerpc64
#define arch_name_powerpc32 __comp_str__arch_name_powerpc32
#define arch_name_s390x __comp_str__arch_name_s390x

/*--- Architecture Family ---*/

#define arch_family_type __comp_enum__arch_family_type
#define arch_family_type_unknown __comp_enum__arch_family_type_unknown
#define arch_family_type_x86 __comp_enum__arch_family_type_x86
#define arch_family_type_arm __comp_enum__arch_family_type_arm
#define arch_family_type_riscv __comp_enum__arch_family_type_riscv
#define arch_family_type_wasm __comp_enum__arch_family_type_wasm
#define arch_family_type_avr __comp_enum__arch_family_type_avr
#define arch_family_type_mips __comp_enum__arch_family_type_mips
#define arch_family_type_powerpc __comp_enum__arch_family_type_powerpc
#define arch_family_type_s390x __comp_enum__arch_family_type_s390x

#define arch_family_is_x86 __comp_bool__arch_family_is_x86
#define arch_family_is_arm __comp_bool__arch_family_is_arm
#define arch_family_is_riscv __comp_bool__arch_family_is_riscv
#define arch_family_is_wasm __comp_bool__arch_family_is_wasm
#define arch_family_is_avr __comp_bool__arch_family_is_avr
#define arch_family_is_mips __comp_bool__arch_family_is_mips
#define arch_family_is_powerpc __comp_bool__arch_family_is_powerpc
#define arch_family_is_s390x __comp_bool__arch_family_is_s390x

/*--- Architecture Family name */

#define arch_family_name __comp_str__arch_family_name
#define arch_family_name_unknown __comp_str__arch_family_name_unknown
#define arch_family_name_x86 __comp_str__arch_family_name_x86
#define arch_family_name_arm __comp_str__arch_family_name_arm
#define arch_family_name_riscv __comp_str__arch_family_name_riscv
#define arch_family_name_wasm __comp_str__arch_family_name_wasm
#define arch_family_name_avr __comp_str__arch_family_name_avr
#define arch_family_name_mips __comp_str__arch_family_name_mips
#define arch_family_name_powerpc __comp_str__arch_family_name_powerpc
#define arch_family_name_s390x __comp_str__arch_family_name_s390x

/*--- Bit Width ---*/

#define arch_bits_unit __comp_enum__arch_bits_unit
#define arch_bits_unit_unknown __comp_enum__arch_bits_unit_unknown
#define arch_bits_unit_64bit __comp_enum__arch_bits_unit_64bit
#define arch_bits_unit_32bit __comp_enum__arch_bits_unit_32bit
#define arch_bits_unit_16bit __comp_enum__arch_bits_unit_16bit
#define arch_bits_unit_8bit __comp_enum__arch_bits_unit_8bit

#define arch_bits_per_word __comp_int__arch_bits_per_word
#define arch_bits_per_byte __comp_int__arch_bits_per_byte
#define arch_bits_per_nibble __comp_int__arch_bits_per_nibble
#define arch_bits_per_crumb __comp_int__arch_bits_per_crumb

#define arch_bits_wide __comp_int__arch_bits_wide
#define arch_bits_wide_unknown __comp_int__arch_bits_wide_unknown
#define arch_bits_wide_64bit __comp_int__arch_bits_wide_64bit
#define arch_bits_wide_32bit __comp_int__arch_bits_wide_32bit
#define arch_bits_wide_16bit __comp_int__arch_bits_wide_16bit
#define arch_bits_wide_8bit __comp_int__arch_bits_wide_8bit

#define arch_bits_is_64bit __comp_bool__arch_bits_is_64bit
#define arch_bits_is_32bit __comp_bool__arch_bits_is_32bit
#define arch_bits_is_16bit __comp_bool__arch_bits_is_16bit
#define arch_bits_is_8bit __comp_bool__arch_bits_is_8bit

/*--- Endianness ---*/

#define arch_byte_order __comp_enum__arch_byte_order
#define arch_byte_order_native arch_byte_order
#define arch_byte_order_foreign __comp_enum__arch_byte_order_foreign
#define arch_byte_order_unknown __comp_enum__arch_byte_order_unknown
#define arch_byte_order_big_endian __comp_enum__arch_byte_order_big_endian
#define arch_byte_order_little_endian __comp_enum__arch_byte_order_little_endian

#define arch_byte_order_is_big_endian __comp_bool__arch_byte_order_is_big_endian
#define arch_byte_order_is_little_endian __comp_bool__arch_byte_order_is_little_endian

/*--- Cache Line ---*/

#define arch_cache_line_bytes __comp_int__arch_cache_line_bytes

/*--- Spin-loop Features ---*/

#define arch_has_arm_yield __comp_bool__arch_has_arm_yield
#define arch_has_riscv_zihintpause __comp_bool__arch_has_riscv_zihintpause
#define arch_has_spin_loop_hint __comp_bool__arch_has_spin_loop_hint

/*--- SIMD Feature ---*/
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

/*--- x86/x86_64 SIMD Features ---*/
#define arch_has_sse __comp_bool__arch_has_sse
#define arch_has_sse2 __comp_bool__arch_has_sse2
#define arch_has_sse3 __comp_bool__arch_has_sse3
#define arch_has_ssse3 __comp_bool__arch_has_ssse3
#define arch_has_sse4_1 __comp_bool__arch_has_sse4_1
#define arch_has_sse4_2 __comp_bool__arch_has_sse4_2
#define arch_has_avx __comp_bool__arch_has_avx
#define arch_has_avx2 __comp_bool__arch_has_avx2
#define arch_has_avx512f __comp_bool__arch_has_avx512f
#define arch_has_avx512bw __comp_bool__arch_has_avx512bw
#define arch_has_avx512dq __comp_bool__arch_has_avx512dq
#define arch_has_avx512vl __comp_bool__arch_has_avx512vl
#define arch_has_fma __comp_bool__arch_has_fma
/*--- ARM SIMD Features ---*/
#define arch_has_neon __comp_bool__arch_has_neon
#define arch_has_sve __comp_bool__arch_has_sve
#define arch_has_sve2 __comp_bool__arch_has_sve2
#define arch_has_fp16_vector_arith __comp_bool__arch_has_fp16_vector_arith
#define arch_has_dotprod __comp_bool__arch_has_dotprod
#define arch_has_i8mm __comp_bool__arch_has_i8mm
/*--- RISC-V Vector Extension ---*/
#define arch_has_rvv __comp_bool__arch_has_rvv
#define arch_rvv_min_vlen_bits __comp_int__arch_rvv_min_vlen_bits
/*--- WebAssembly SIMD ---*/
#define arch_has_wasm_simd128 __comp_bool__arch_has_wasm_simd128
#define arch_has_wasm_relaxed_simd __comp_bool__arch_has_wasm_relaxed_simd
#define arch_has_wasm_bulk_memory __comp_bool__arch_has_wasm_bulk_memory
#define arch_has_wasm_exception_handling __comp_bool__arch_has_wasm_exception_handling
#define arch_has_wasm_atomics __comp_bool__arch_has_wasm_atomics
#define arch_has_wasm_nontrapping_fptoint __comp_bool__arch_has_wasm_nontrapping_fptoint
#define arch_has_wasm_reference_types __comp_bool__arch_has_wasm_reference_types
#define arch_has_wasm_gc __comp_bool__arch_has_wasm_gc
#define arch_has_wasm_fp16 __comp_bool__arch_has_wasm_fp16

/*--- SIMD Availability Summary ---*/

#define arch_simd_supported __comp_bool__arch_simd_supported
#define arch_simd_scalable __comp_bool__arch_simd_scalable
#define arch_simd_width_bits __comp_int__arch_simd_width_bits
#define arch_simd_align_bytes __comp_int__arch_simd_align_bytes

/*========== Macros and Definitions =========================================*/

/*--- Architecture Type ---*/

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
#define __comp_enum__arch_type_avr 9
#define __comp_enum__arch_type_mips64 10
#define __comp_enum__arch_type_mips32 11
#define __comp_enum__arch_type_powerpc64 12
#define __comp_enum__arch_type_powerpc32 13
#define __comp_enum__arch_type_s390x 14

#define __comp_bool__arch_is_x86_64 pp_Tok_eql(arch_type, arch_type_x86_64)
#define __comp_bool__arch_is_x86 pp_Tok_eql(arch_type, arch_type_x86)
#define __comp_bool__arch_is_aarch64 pp_Tok_eql(arch_type, arch_type_aarch64)
#define __comp_bool__arch_is_arm pp_Tok_eql(arch_type, arch_type_arm)
#define __comp_bool__arch_is_riscv64 pp_Tok_eql(arch_type, arch_type_riscv64)
#define __comp_bool__arch_is_riscv32 pp_Tok_eql(arch_type, arch_type_riscv32)
#define __comp_bool__arch_is_wasm64 pp_Tok_eql(arch_type, arch_type_wasm64)
#define __comp_bool__arch_is_wasm32 pp_Tok_eql(arch_type, arch_type_wasm32)
#define __comp_bool__arch_is_avr pp_Tok_eql(arch_type, arch_type_avr)
#define __comp_bool__arch_is_mips64 pp_Tok_eql(arch_type, arch_type_mips64)
#define __comp_bool__arch_is_mips32 pp_Tok_eql(arch_type, arch_type_mips32)
#define __comp_bool__arch_is_powerpc64 pp_Tok_eql(arch_type, arch_type_powerpc64)
#define __comp_bool__arch_is_powerpc32 pp_Tok_eql(arch_type, arch_type_powerpc32)
#define __comp_bool__arch_is_s390x pp_Tok_eql(arch_type, arch_type_s390x)

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

/* Detect AVR */
#elif defined(__AVR__)
#undef __comp_enum__arch_type
#define __comp_enum__arch_type arch_type_avr

/* Detect MIPS */
#elif defined(__mips__)
#if defined(__mips64)
#undef __comp_enum__arch_type
#define __comp_enum__arch_type arch_type_mips64
#else
#undef __comp_enum__arch_type
#define __comp_enum__arch_type arch_type_mips32
#endif

/* Detect PowerPC */
#elif defined(__powerpc64__) || defined(__ppc64__)
#undef __comp_enum__arch_type
#define __comp_enum__arch_type arch_type_powerpc64
#elif defined(__powerpc__) || defined(__ppc__) || defined(_M_PPC)
#undef __comp_enum__arch_type
#define __comp_enum__arch_type arch_type_powerpc32

/* Detect IBM Z */
#elif defined(__s390x__)
#undef __comp_enum__arch_type
#define __comp_enum__arch_type arch_type_s390x

/* Detect WebAssembly */
#elif defined(__wasm__) || defined(__EMSCRIPTEN__)
#if defined(__wasm64__) || defined(__wasm64)
#undef __comp_enum__arch_type
#define __comp_enum__arch_type arch_type_wasm64
#else
#undef __comp_enum__arch_type
#define __comp_enum__arch_type arch_type_wasm32
#endif
#endif

/*--- Architecture Name ---*/

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
        pp_case_((arch_type_avr)(arch_name_avr)), \
        pp_case_((arch_type_mips64)(arch_name_mips64)), \
        pp_case_((arch_type_mips32)(arch_name_mips32)), \
        pp_case_((arch_type_powerpc64)(arch_name_powerpc64)), \
        pp_case_((arch_type_powerpc32)(arch_name_powerpc32)), \
        pp_case_((arch_type_s390x)(arch_name_s390x)), \
        pp_default_(()(arch_name_unknown)) \
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
#define __comp_str__arch_name_avr "AVR"
#define __comp_str__arch_name_mips64 "MIPS64"
#define __comp_str__arch_name_mips32 "MIPS32"
#define __comp_str__arch_name_powerpc64 "PowerPC64"
#define __comp_str__arch_name_powerpc32 "PowerPC32"
#define __comp_str__arch_name_s390x "s390x"

/*--- Architecture Family ---*/

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
        pp_case_((arch_type_avr)(arch_family_type_avr)), \
        pp_case_((arch_type_mips64)(arch_family_type_mips)), \
        pp_case_((arch_type_mips32)(arch_family_type_mips)), \
        pp_case_((arch_type_powerpc64)(arch_family_type_powerpc)), \
        pp_case_((arch_type_powerpc32)(arch_family_type_powerpc)), \
        pp_case_((arch_type_s390x)(arch_family_type_s390x)), \
        pp_default_(()(arch_family_type_unknown)) \
    ) pp_end \
)
#define __comp_enum__arch_family_type_unknown 0
#define __comp_enum__arch_family_type_x86 1
#define __comp_enum__arch_family_type_arm 2
#define __comp_enum__arch_family_type_riscv 3
#define __comp_enum__arch_family_type_wasm 4
#define __comp_enum__arch_family_type_avr 5
#define __comp_enum__arch_family_type_mips 6
#define __comp_enum__arch_family_type_powerpc 7
#define __comp_enum__arch_family_type_s390x 8

#define __comp_bool__arch_family_is_x86 pp_Tok_eql(arch_family_type, arch_family_type_x86)
#define __comp_bool__arch_family_is_arm pp_Tok_eql(arch_family_type, arch_family_type_arm)
#define __comp_bool__arch_family_is_riscv pp_Tok_eql(arch_family_type, arch_family_type_riscv)
#define __comp_bool__arch_family_is_wasm pp_Tok_eql(arch_family_type, arch_family_type_wasm)
#define __comp_bool__arch_family_is_avr pp_Tok_eql(arch_family_type, arch_family_type_avr)
#define __comp_bool__arch_family_is_mips pp_Tok_eql(arch_family_type, arch_family_type_mips)
#define __comp_bool__arch_family_is_powerpc pp_Tok_eql(arch_family_type, arch_family_type_powerpc)
#define __comp_bool__arch_family_is_s390x pp_Tok_eql(arch_family_type, arch_family_type_s390x)

/* Derive family name from type */
#define __comp_str__arch_family_name pp_expand( \
    pp_switch_ pp_begin(arch_family_type)( \
        pp_case_((arch_family_type_x86)(arch_family_name_x86)), \
        pp_case_((arch_family_type_arm)(arch_family_name_arm)), \
        pp_case_((arch_family_type_riscv)(arch_family_name_riscv)), \
        pp_case_((arch_family_type_wasm)(arch_family_name_wasm)), \
        pp_case_((arch_family_type_avr)(arch_family_name_avr)), \
        pp_case_((arch_family_type_mips)(arch_family_name_mips)), \
        pp_case_((arch_family_type_powerpc)(arch_family_name_powerpc)), \
        pp_case_((arch_family_type_s390x)(arch_family_name_s390x)), \
        pp_default_(()(arch_family_name_unknown)) \
    ) pp_end \
)
#define __comp_str__arch_family_name_unknown "unknown"
#define __comp_str__arch_family_name_x86 "x86"
#define __comp_str__arch_family_name_arm "ARM"
#define __comp_str__arch_family_name_riscv "RISC-V"
#define __comp_str__arch_family_name_wasm "WebAssembly"
#define __comp_str__arch_family_name_avr "AVR"
#define __comp_str__arch_family_name_mips "MIPS"
#define __comp_str__arch_family_name_powerpc "PowerPC"
#define __comp_str__arch_family_name_s390x "IBM Z"

/*--- Bit Width ---*/

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
        pp_case_((arch_type_avr)(arch_bits_unit_8bit)), \
        pp_case_((arch_type_mips64)(arch_bits_unit_64bit)), \
        pp_case_((arch_type_mips32)(arch_bits_unit_32bit)), \
        pp_case_((arch_type_powerpc64)(arch_bits_unit_64bit)), \
        pp_case_((arch_type_powerpc32)(arch_bits_unit_32bit)), \
        pp_case_((arch_type_s390x)(arch_bits_unit_64bit)), \
        pp_default_(()(arch_bits_unit_unknown)) \
    ) pp_end \
)
#define __comp_enum__arch_bits_unit_unknown 0
#define __comp_enum__arch_bits_unit_64bit 1
#define __comp_enum__arch_bits_unit_32bit 2
#define __comp_enum__arch_bits_unit_16bit 3
#define __comp_enum__arch_bits_unit_8bit 4

/* Derive bits per byte from family */
#define __comp_int__arch_bits_per_word arch_bits_wide
#define __comp_int__arch_bits_per_byte 8
#define __comp_int__arch_bits_per_nibble 4
#define __comp_int__arch_bits_per_crumb 2

/* Derive bit width from unit */
#define __comp_int__arch_bits_wide pp_expand( \
    pp_switch_ pp_begin(arch_bits_unit)( \
        pp_case_((arch_bits_unit_64bit)(arch_bits_wide_64bit)), \
        pp_case_((arch_bits_unit_32bit)(arch_bits_wide_32bit)), \
        pp_case_((arch_bits_unit_16bit)(arch_bits_wide_16bit)), \
        pp_case_((arch_bits_unit_8bit)(arch_bits_wide_8bit)), \
        pp_default_(()(arch_bits_wide_unknown)) \
    ) pp_end \
)
#define __comp_int__arch_bits_wide_unknown 0
#define __comp_int__arch_bits_wide_64bit 64
#define __comp_int__arch_bits_wide_32bit 32
#define __comp_int__arch_bits_wide_16bit 16
#define __comp_int__arch_bits_wide_8bit 8

#define __comp_bool__arch_bits_is_64bit pp_Tok_eql(arch_bits_unit, arch_bits_unit_64bit)
#define __comp_bool__arch_bits_is_32bit pp_Tok_eql(arch_bits_unit, arch_bits_unit_32bit)
#define __comp_bool__arch_bits_is_16bit pp_Tok_eql(arch_bits_unit, arch_bits_unit_16bit)
#define __comp_bool__arch_bits_is_8bit pp_Tok_eql(arch_bits_unit, arch_bits_unit_8bit)

/*--- Endianness ---*/

/* Byte order is a compiler-selected target fact. */
#define __comp_enum__arch_byte_order arch_byte_order_unknown
#define __comp_enum__arch_byte_order_unknown 0
#define __comp_enum__arch_byte_order_big_endian 1
#define __comp_enum__arch_byte_order_little_endian 2
#define __comp_enum__arch_byte_order_foreign pp_expand( \
    pp_switch_ pp_begin(arch_byte_order)( \
        pp_case_((arch_byte_order_big_endian)(arch_byte_order_little_endian)), \
        pp_case_((arch_byte_order_little_endian)(arch_byte_order_big_endian)), \
        pp_default_(()(arch_byte_order_unknown)) \
    ) pp_end \
)
#define __comp_bool__arch_byte_order_is_little_endian pp_Tok_eql(arch_byte_order, arch_byte_order_little_endian)
#define __comp_bool__arch_byte_order_is_big_endian pp_Tok_eql(arch_byte_order, arch_byte_order_big_endian)

/* Detect byte order. WebAssembly's linear memory is defined as little-endian. */
#if arch_family_type == arch_family_type_wasm
#undef __comp_enum__arch_byte_order
#define __comp_enum__arch_byte_order arch_byte_order_little_endian
#elif defined(__BYTE_ORDER__) && defined(__ORDER_LITTLE_ENDIAN__) && defined(__ORDER_BIG_ENDIAN__)
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
#undef __comp_enum__arch_byte_order
#define __comp_enum__arch_byte_order arch_byte_order_little_endian
#elif __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
#undef __comp_enum__arch_byte_order
#define __comp_enum__arch_byte_order arch_byte_order_big_endian
#endif
#endif

/*--- Cache Line ---*/

/* Estimated cache-line boundary for atomically updated memory. */
#define __comp_int__arch_cache_line_bytes pp_expand( \
    pp_switch_ pp_begin(arch_type)( \
        pp_case_((arch_type_x86_64)(128)), \
        pp_case_((arch_type_aarch64)(128)), \
        pp_case_((arch_type_powerpc64)(128)), \
        pp_case_((arch_type_arm)(32)), \
        pp_case_((arch_type_mips64)(32)), \
        pp_case_((arch_type_mips32)(32)), \
        pp_case_((arch_type_s390x)(256)), \
        pp_default_(()(64)) \
    ) pp_end \
)

/*--- Spin-loop Features ---*/

#if arch_type == arch_type_arm \
    && (defined(__ARM_ARCH_6K__) || defined(__ARM_ARCH_6ZK__) || defined(__ARM_ARCH_6M__) \
        || (defined(__ARM_ARCH) && (__ARM_ARCH >= 7)))
#define __comp_bool__arch_has_arm_yield 1
#else
#define __comp_bool__arch_has_arm_yield 0
#endif

#if arch_family_is_riscv && defined(__riscv_zihintpause)
#define __comp_bool__arch_has_riscv_zihintpause 1
#else
#define __comp_bool__arch_has_riscv_zihintpause 0
#endif

#define __comp_bool__arch_has_spin_loop_hint pp_or( \
    arch_family_is_x86, \
    pp_or(arch_is_aarch64, pp_or(arch_has_arm_yield, arch_has_riscv_zihintpause)) \
)

#if arch_is_avr && (arch_bits_wide != 8)
#error "AVR has an inconsistent natural execution width"
#endif

/*--- SIMD Feature ---*/

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
#define __comp_bool__arch_has_avx512bw 0
#define __comp_bool__arch_has_avx512dq 0
#define __comp_bool__arch_has_avx512vl 0
#define __comp_bool__arch_has_fma 0
#define __comp_bool__arch_has_neon 0
#define __comp_bool__arch_has_sve 0
#define __comp_bool__arch_has_sve2 0
#define __comp_bool__arch_has_fp16_vector_arith 0
#define __comp_bool__arch_has_dotprod 0
#define __comp_bool__arch_has_i8mm 0
#define __comp_bool__arch_has_rvv 0
#define __comp_int__arch_rvv_min_vlen_bits 0
#define __comp_bool__arch_has_wasm_simd128 0
#define __comp_bool__arch_has_wasm_relaxed_simd 0
#define __comp_bool__arch_has_wasm_bulk_memory 0
#define __comp_bool__arch_has_wasm_exception_handling 0
#define __comp_bool__arch_has_wasm_atomics 0
#define __comp_bool__arch_has_wasm_nontrapping_fptoint 0
#define __comp_bool__arch_has_wasm_reference_types 0
#define __comp_bool__arch_has_wasm_gc 0
#define __comp_bool__arch_has_wasm_fp16 0

/*--- x86/x86_64 SIMD Detection ---*/

#if arch_family_is_x86

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

#if defined(__AVX512BW__)
#undef __comp_bool__arch_has_avx512bw
#define __comp_bool__arch_has_avx512bw 1
#endif /* defined(__AVX512BW__) */

#if defined(__AVX512DQ__)
#undef __comp_bool__arch_has_avx512dq
#define __comp_bool__arch_has_avx512dq 1
#endif /* defined(__AVX512DQ__) */

#if defined(__AVX512VL__)
#undef __comp_bool__arch_has_avx512vl
#define __comp_bool__arch_has_avx512vl 1
#endif /* defined(__AVX512VL__) */

/* FMA (Fused Multiply-Add) */
#if defined(__FMA__)
#undef __comp_bool__arch_has_fma
#define __comp_bool__arch_has_fma 1
#endif /* defined(__FMA__) */

#endif /* arch_family_is_x86 */

/*--- ARM SIMD Detection ---*/

#if arch_family_is_arm

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

#if defined(__ARM_FEATURE_SVE2)
#undef __comp_bool__arch_has_sve2
#define __comp_bool__arch_has_sve2 1
#endif /* defined(__ARM_FEATURE_SVE2) */

#if defined(__ARM_FEATURE_FP16_VECTOR_ARITHMETIC)
#undef __comp_bool__arch_has_fp16_vector_arith
#define __comp_bool__arch_has_fp16_vector_arith 1
#endif /* defined(__ARM_FEATURE_FP16_VECTOR_ARITHMETIC) */

#if defined(__ARM_FEATURE_DOTPROD)
#undef __comp_bool__arch_has_dotprod
#define __comp_bool__arch_has_dotprod 1
#endif /* defined(__ARM_FEATURE_DOTPROD) */

#if defined(__ARM_FEATURE_MATMUL_INT8)
#undef __comp_bool__arch_has_i8mm
#define __comp_bool__arch_has_i8mm 1
#endif /* defined(__ARM_FEATURE_MATMUL_INT8) */

#endif /* arch_family_is_arm */

/*--- RISC-V Vector Extension Detection ---*/

#if arch_family_is_riscv

#if defined(__riscv_v) || defined(__riscv_vector)
#undef __comp_bool__arch_has_rvv
#define __comp_bool__arch_has_rvv 1
#endif /* defined(__riscv_v) || defined(__riscv_vector) */

#if defined(__riscv_v_min_vlen)
#undef __comp_int__arch_rvv_min_vlen_bits
#define __comp_int__arch_rvv_min_vlen_bits __riscv_v_min_vlen
#elif defined(__riscv_zvl128b)
#undef __comp_int__arch_rvv_min_vlen_bits
#define __comp_int__arch_rvv_min_vlen_bits 128
#endif

#endif /* arch_family_is_riscv */

/*--- WebAssembly SIMD Detection ---*/

#if arch_family_is_wasm

#if defined(__wasm_simd128__)
#undef __comp_bool__arch_has_wasm_simd128
#define __comp_bool__arch_has_wasm_simd128 1
#endif /* defined(__wasm_simd128__) */

#if defined(__wasm_relaxed_simd__)
#undef __comp_bool__arch_has_wasm_relaxed_simd
#define __comp_bool__arch_has_wasm_relaxed_simd 1
#endif /* defined(__wasm_relaxed_simd__) */
#if defined(__wasm_bulk_memory__)
#undef __comp_bool__arch_has_wasm_bulk_memory
#define __comp_bool__arch_has_wasm_bulk_memory 1
#endif
#if defined(__wasm_exception_handling__)
#undef __comp_bool__arch_has_wasm_exception_handling
#define __comp_bool__arch_has_wasm_exception_handling 1
#endif
#if defined(__wasm_atomics__)
#undef __comp_bool__arch_has_wasm_atomics
#define __comp_bool__arch_has_wasm_atomics 1
#endif
#if defined(__wasm_nontrapping_fptoint__)
#undef __comp_bool__arch_has_wasm_nontrapping_fptoint
#define __comp_bool__arch_has_wasm_nontrapping_fptoint 1
#endif
#if defined(__wasm_reference_types__)
#undef __comp_bool__arch_has_wasm_reference_types
#define __comp_bool__arch_has_wasm_reference_types 1
#endif
#if defined(__wasm_gc__)
#undef __comp_bool__arch_has_wasm_gc
#define __comp_bool__arch_has_wasm_gc 1
#endif
#if defined(__wasm_fp16__)
#undef __comp_bool__arch_has_wasm_fp16
#define __comp_bool__arch_has_wasm_fp16 1
#endif

#endif /* arch_family_is_wasm */

/*--- SIMD Availability Summary ---*/

/* Determine if any SIMD is available */
#define __comp_bool__arch_simd_supported pp_or( \
    arch_has_sse2, pp_or(arch_has_neon, pp_or(arch_has_sve, pp_or(arch_has_rvv, arch_has_wasm_simd128))) \
)
#define __comp_bool__arch_simd_scalable pp_or(arch_has_sve, arch_has_rvv)
/* A scalable vector ISA has no single compile-time register width or
 * alignment.  Keep those facts unknown instead of inventing a representative
 * value; the minimum RVV length remains available separately. */
#define __comp_int__arch_simd_width_bits pp_if_(arch_simd_scalable)( \
    pp_then_(0), \
    pp_else_(pp_if_(arch_has_avx512f)( \
        pp_then_(512), \
        pp_else_(pp_if_(pp_or(arch_has_avx, arch_has_avx2))( \
            pp_then_(256), \
            pp_else_(pp_if_(pp_or(arch_has_sse, pp_or(arch_has_sse2, pp_or(arch_has_neon, arch_has_wasm_simd128))))( \
                pp_then_(128), \
                pp_else_(0) \
            )) \
        )) \
    )) \
)
#define __comp_int__arch_simd_align_bytes pp_if_(arch_simd_scalable)( \
    pp_then_(0), \
    pp_else_(pp_if_(arch_has_avx512f)( \
        pp_then_(64), \
        pp_else_(pp_if_(pp_or(arch_has_avx, arch_has_avx2))( \
            pp_then_(32), \
            pp_else_(pp_if_(pp_or(arch_has_sse, pp_or(arch_has_sse2, pp_or(arch_has_neon, arch_has_wasm_simd128))))( \
                pp_then_(16), \
                pp_else_(1) \
            )) \
        )) \
    )) \
)

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* foundation_cfg_arch__included */

/**
 * @copyright Copyright (c) 2026 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    simd.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2026-06-21 (date of creation)
 * @updated 2026-06-21 (date of last update)
 * @ingroup dasae-headers(dh)/builtin
 * @prefix  simd
 *
 * @brief   SIMD target capabilities and intrinsic headers
 * @details Aliases architecture detection and owns target-specific intrinsic
 *          header selection. Portable SIMD operations live in dh/prl/simd.h.
 */
#pragma once
#ifndef builtin_simd__included
#define builtin_simd__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "cfg.h"

#if arch_is_x86_family && arch_has_sse
#include <immintrin.h>
#elif arch_is_arm_family
#if arch_has_neon
#include <arm_neon.h>
#endif /* arch_has_neon */
#if arch_has_sve
#include <arm_sve.h>
#endif /* arch_has_sve */
#elif arch_is_riscv_family && arch_has_rvv
#include <riscv_vector.h>
#elif arch_is_wasm_family && arch_has_wasm_simd128
#include <wasm_simd128.h>
#endif

/*========== Macros and Declarations ========================================*/

#define simd_supported arch_simd_supported
#define simd_scalable arch_simd_scalable
#define simd_width_bits arch_simd_width_bits
#define simd_align_bytes arch_simd_align_bytes

#define simd_has_sse arch_has_sse
#define simd_has_sse2 arch_has_sse2
#define simd_has_sse3 arch_has_sse3
#define simd_has_ssse3 arch_has_ssse3
#define simd_has_sse4_1 arch_has_sse4_1
#define simd_has_sse4_2 arch_has_sse4_2
#define simd_has_avx arch_has_avx
#define simd_has_avx2 arch_has_avx2
#define simd_has_avx512f arch_has_avx512f
#define simd_has_avx512bw arch_has_avx512bw
#define simd_has_avx512dq arch_has_avx512dq
#define simd_has_avx512vl arch_has_avx512vl
#define simd_has_fma arch_has_fma
#define simd_has_neon arch_has_neon
#define simd_has_sve arch_has_sve
#define simd_has_sve2 arch_has_sve2
#define simd_has_fp16_vector_arith arch_has_fp16_vector_arith
#define simd_has_dotprod arch_has_dotprod
#define simd_has_i8mm arch_has_i8mm
#define simd_has_rvv arch_has_rvv
#define simd_rvv_min_vlen_bits arch_rvv_min_vlen_bits
#define simd_has_wasm_simd128 arch_has_wasm_simd128
#define simd_has_wasm_relaxed_simd arch_has_wasm_relaxed_simd

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* builtin_simd__included */

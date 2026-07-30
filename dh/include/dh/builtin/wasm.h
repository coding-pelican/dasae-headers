/**
 * @copyright Copyright (c) 2026 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    wasm.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2026-06-28 (date of creation)
 * @updated 2026-07-30 (date of last update)
 * @ingroup dasae-headers(dh)/builtin
 * @prefix  wasm
 *
 * @brief   WebAssembly target builtin wrappers
 * @details Thin dh-style aliases around Clang's WebAssembly target builtins.
 *          SIMD operations are intentionally left to <wasm_simd128.h>, which
 *          is selected by dh/builtin/simd.h when wasm SIMD is enabled.
 */
#pragma once
#ifndef builtin_wasm__included
#define builtin_wasm__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "comp.h"

/*========== Macros and Declarations ========================================*/

#define wasm_builtin_supported pp_and(comp_is_clang, arch_family_is_wasm)
#define wasm_has_bulk_memory arch_has_wasm_bulk_memory
#define wasm_has_exception_handling arch_has_wasm_exception_handling
#define wasm_has_atomics arch_has_wasm_atomics
#define wasm_has_nontrapping_fptoint arch_has_wasm_nontrapping_fptoint
#define wasm_has_reference_types arch_has_wasm_reference_types
#define wasm_has_gc arch_has_wasm_gc
#define wasm_has_fp16 arch_has_wasm_fp16

#if wasm_builtin_supported

#define wasm_mem_size(_$mem_idx) \
    (as$(usize)(__builtin_wasm_memory_size(_$mem_idx)))
#define wasm_mem_grow(_$mem_idx, _$delta) \
    (as$(usize)(__builtin_wasm_memory_grow(_$mem_idx, _$delta)))

#define wasm_minF32(_$lhs, _$rhs) \
    (as$(f32)(__builtin_wasm_min_f32(_$lhs, _$rhs)))
#define wasm_maxF32(_$lhs, _$rhs) \
    (as$(f32)(__builtin_wasm_max_f32(_$lhs, _$rhs)))
#define wasm_minF64(_$lhs, _$rhs) \
    (as$(f64)(__builtin_wasm_min_f64(_$lhs, _$rhs)))
#define wasm_maxF64(_$lhs, _$rhs) \
    (as$(f64)(__builtin_wasm_max_f64(_$lhs, _$rhs)))

#define wasm_truncSI32F32(_$val) \
    (as$(i32)(__builtin_wasm_trunc_s_i32_f32(_$val)))
#define wasm_truncUI32F32(_$val) \
    (as$(u32)(__builtin_wasm_trunc_u_i32_f32(_$val)))
#define wasm_truncSI32F64(_$val) \
    (as$(i32)(__builtin_wasm_trunc_s_i32_f64(_$val)))
#define wasm_truncUI32F64(_$val) \
    (as$(u32)(__builtin_wasm_trunc_u_i32_f64(_$val)))
#define wasm_truncSI64F32(_$val) \
    (as$(i64)(__builtin_wasm_trunc_s_i64_f32(_$val)))
#define wasm_truncUI64F32(_$val) \
    (as$(u64)(__builtin_wasm_trunc_u_i64_f32(_$val)))
#define wasm_truncSI64F64(_$val) \
    (as$(i64)(__builtin_wasm_trunc_s_i64_f64(_$val)))
#define wasm_truncUI64F64(_$val) \
    (as$(u64)(__builtin_wasm_trunc_u_i64_f64(_$val)))

#if wasm_has_bulk_memory
#define wasm_tls_size() \
    (as$(usize)(__builtin_wasm_tls_size()))
#define wasm_tls_align() \
    (as$(usize)(__builtin_wasm_tls_align()))
#define wasm_tls_base() \
    __builtin_wasm_tls_base()
#endif /* wasm_has_bulk_memory */

#if wasm_has_exception_handling
#define wasm_throw(_$tag, _$payload) \
    __builtin_wasm_throw(_$tag, _$payload)
#define wasm_rethrow() \
    __builtin_wasm_rethrow()
#endif /* wasm_has_exception_handling */

#if wasm_has_atomics
#define wasm_atom_wait32(_$addr, _$expected, _$timeout) \
    __builtin_wasm_memory_atomic_wait32(_$addr, _$expected, _$timeout)
#define wasm_atom_wait64(_$addr, _$expected, _$timeout) \
    __builtin_wasm_memory_atomic_wait64(_$addr, _$expected, _$timeout)
#define wasm_atom_notify(_$addr, _$count) \
    __builtin_wasm_memory_atomic_notify(_$addr, _$count)
#endif /* wasm_has_atomics */

#if wasm_has_nontrapping_fptoint
#define wasm_truncSatSI32F32(_$val) \
    (as$(i32)(__builtin_wasm_trunc_saturate_s_i32_f32(_$val)))
#define wasm_truncSatUI32F32(_$val) \
    (as$(u32)(__builtin_wasm_trunc_saturate_u_i32_f32(_$val)))
#define wasm_truncSatSI32F64(_$val) \
    (as$(i32)(__builtin_wasm_trunc_saturate_s_i32_f64(_$val)))
#define wasm_truncSatUI32F64(_$val) \
    (as$(u32)(__builtin_wasm_trunc_saturate_u_i32_f64(_$val)))
#define wasm_truncSatSI64F32(_$val) \
    (as$(i64)(__builtin_wasm_trunc_saturate_s_i64_f32(_$val)))
#define wasm_truncSatUI64F32(_$val) \
    (as$(u64)(__builtin_wasm_trunc_saturate_u_i64_f32(_$val)))
#define wasm_truncSatSI64F64(_$val) \
    (as$(i64)(__builtin_wasm_trunc_saturate_s_i64_f64(_$val)))
#define wasm_truncSatUI64F64(_$val) \
    (as$(u64)(__builtin_wasm_trunc_saturate_u_i64_f64(_$val)))
#endif /* wasm_has_nontrapping_fptoint */

#if wasm_has_fp16
#define wasm_loadF16F32(_$ptr) \
    __builtin_wasm_loadf16_f32(_$ptr)
#define wasm_storeF16F32(_$val, _$ptr) \
    __builtin_wasm_storef16_f32(_$val, _$ptr)
#endif /* wasm_has_fp16 */

#if wasm_has_reference_types
#define wasm_ref_nullExtern() \
    __builtin_wasm_ref_null_extern()
#define wasm_ref_isNullExtern(_$ref) \
    __builtin_wasm_ref_is_null_extern(_$ref)
#define wasm_ref_nullFn() \
    __builtin_wasm_ref_null_func()

#define wasm_table_set(_$table, _$idx, _$ref) \
    __builtin_wasm_table_set(_$table, _$idx, _$ref)
#define wasm_table_get(_$table, _$idx) \
    __builtin_wasm_table_get(_$table, _$idx)
#define wasm_table_size(_$table) \
    __builtin_wasm_table_size(_$table)
#define wasm_table_grow(_$table, _$ref, _$delta) \
    __builtin_wasm_table_grow(_$table, _$ref, _$delta)
#define wasm_table_fill(_$table, _$idx, _$ref, _$len) \
    __builtin_wasm_table_fill(_$table, _$idx, _$ref, _$len)
#define wasm_table_copy(_$dst_table, _$src_table, _$dst_idx, _$src_idx, _$len) \
    __builtin_wasm_table_copy(_$dst_table, _$src_table, _$dst_idx, _$src_idx, _$len)
#endif /* wasm_has_reference_types */

#if wasm_has_gc
#define wasm_testFnPtrSignature(_$fn_ptr) \
    __builtin_wasm_test_function_pointer_signature(_$fn_ptr)
#endif /* wasm_has_gc */

#endif /* wasm_builtin_supported */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* builtin_wasm__included */

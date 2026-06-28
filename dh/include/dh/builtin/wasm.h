/**
 * @copyright Copyright (c) 2026 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    wasm.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2026-06-28 (date of creation)
 * @updated 2026-06-28 (date of last update)
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

#include <stddef.h>
#include <stdint.h>

/*========== Macros and Declarations ========================================*/

#if defined(__clang__) && arch_family_is_wasm
#define wasm_builtin_supported 1
#else
#define wasm_builtin_supported 0
#endif

#if defined(__wasm_bulk_memory__)
#define wasm_has_bulk_memory 1
#else
#define wasm_has_bulk_memory 0
#endif

#if defined(__wasm_exception_handling__)
#define wasm_has_exception_handling 1
#else
#define wasm_has_exception_handling 0
#endif

#if defined(__wasm_atomics__)
#define wasm_has_atomics 1
#else
#define wasm_has_atomics 0
#endif

#if defined(__wasm_nontrapping_fptoint__)
#define wasm_has_nontrapping_fptoint 1
#else
#define wasm_has_nontrapping_fptoint 0
#endif

#if defined(__wasm_reference_types__)
#define wasm_has_reference_types 1
#else
#define wasm_has_reference_types 0
#endif

#if defined(__wasm_gc__)
#define wasm_has_gc 1
#else
#define wasm_has_gc 0
#endif

#if defined(__wasm_fp16__)
#define wasm_has_fp16 1
#else
#define wasm_has_fp16 0
#endif

#if wasm_builtin_supported

#define wasm_mem_size(_memidx) \
    (as$(usize)(__builtin_wasm_memory_size(_memidx)))
#define wasm_mem_grow(_memidx, _delta) \
    (as$(usize)(__builtin_wasm_memory_grow(_memidx, _delta)))

#define wasm_minF32(_lhs, _rhs) \
    (as$(f32)(__builtin_wasm_min_f32(_lhs, _rhs)))
#define wasm_maxF32(_lhs, _rhs) \
    (as$(f32)(__builtin_wasm_max_f32(_lhs, _rhs)))
#define wasm_minF64(_lhs, _rhs) \
    (as$(f64)(__builtin_wasm_min_f64(_lhs, _rhs)))
#define wasm_maxF64(_lhs, _rhs) \
    (as$(f64)(__builtin_wasm_max_f64(_lhs, _rhs)))

#define wasm_truncSI32F32(_value) \
    (as$(i32)(__builtin_wasm_trunc_s_i32_f32(_value)))
#define wasm_truncUI32F32(_value) \
    (as$(u32)(__builtin_wasm_trunc_u_i32_f32(_value)))
#define wasm_truncSI32F64(_value) \
    (as$(i32)(__builtin_wasm_trunc_s_i32_f64(_value)))
#define wasm_truncUI32F64(_value) \
    (as$(u32)(__builtin_wasm_trunc_u_i32_f64(_value)))
#define wasm_truncSI64F32(_value) \
    (as$(i64)(__builtin_wasm_trunc_s_i64_f32(_value)))
#define wasm_truncUI64F32(_value) \
    (as$(u64)(__builtin_wasm_trunc_u_i64_f32(_value)))
#define wasm_truncSI64F64(_value) \
    (as$(i64)(__builtin_wasm_trunc_s_i64_f64(_value)))
#define wasm_truncUI64F64(_value) \
    (as$(u64)(__builtin_wasm_trunc_u_i64_f64(_value)))

#if wasm_has_bulk_memory
#define wasm_tls_size() \
    (as$(usize)(__builtin_wasm_tls_size()))
#define wasm_tls_align() \
    (as$(usize)(__builtin_wasm_tls_align()))
#define wasm_tls_base() \
    __builtin_wasm_tls_base()
#endif /* wasm_has_bulk_memory */

#if wasm_has_exception_handling
#define wasm_throw(_tag, _payload) \
    __builtin_wasm_throw(_tag, _payload)
#define wasm_rethrow() \
    __builtin_wasm_rethrow()
#endif /* wasm_has_exception_handling */

#if wasm_has_atomics
#define wasm_atom_wait32(_addr, _expected, _timeout) \
    __builtin_wasm_memory_atomic_wait32(_addr, _expected, _timeout)
#define wasm_atom_wait64(_addr, _expected, _timeout) \
    __builtin_wasm_memory_atomic_wait64(_addr, _expected, _timeout)
#define wasm_atom_notify(_addr, _count) \
    __builtin_wasm_memory_atomic_notify(_addr, _count)
#endif /* wasm_has_atomics */

#if wasm_has_nontrapping_fptoint
#define wasm_truncSatSI32F32(_value) \
    (as$(i32)(__builtin_wasm_trunc_saturate_s_i32_f32(_value)))
#define wasm_truncSatUI32F32(_value) \
    (as$(u32)(__builtin_wasm_trunc_saturate_u_i32_f32(_value)))
#define wasm_truncSatSI32F64(_value) \
    (as$(i32)(__builtin_wasm_trunc_saturate_s_i32_f64(_value)))
#define wasm_truncSatUI32F64(_value) \
    (as$(u32)(__builtin_wasm_trunc_saturate_u_i32_f64(_value)))
#define wasm_truncSatSI64F32(_value) \
    (as$(i64)(__builtin_wasm_trunc_saturate_s_i64_f32(_value)))
#define wasm_truncSatUI64F32(_value) \
    (as$(u64)(__builtin_wasm_trunc_saturate_u_i64_f32(_value)))
#define wasm_truncSatSI64F64(_value) \
    (as$(i64)(__builtin_wasm_trunc_saturate_s_i64_f64(_value)))
#define wasm_truncSatUI64F64(_value) \
    (as$(u64)(__builtin_wasm_trunc_saturate_u_i64_f64(_value)))
#endif /* wasm_has_nontrapping_fptoint */

#if wasm_has_fp16
#define wasm_loadF16F32(_ptr) \
    __builtin_wasm_loadf16_f32(_ptr)
#define wasm_storeF16F32(_value, _ptr) \
    __builtin_wasm_storef16_f32(_value, _ptr)
#endif /* wasm_has_fp16 */

#if wasm_has_reference_types
#define wasm_ref_nullExtern() \
    __builtin_wasm_ref_null_extern()
#define wasm_ref_isNullExtern(_ref) \
    __builtin_wasm_ref_is_null_extern(_ref)
#define wasm_ref_nullFn() \
    __builtin_wasm_ref_null_func()

#define wasm_table_set(_table, _idx, _ref) \
    __builtin_wasm_table_set(_table, _idx, _ref)
#define wasm_table_get(_table, _idx) \
    __builtin_wasm_table_get(_table, _idx)
#define wasm_table_size(_table) \
    __builtin_wasm_table_size(_table)
#define wasm_table_grow(_table, _ref, _delta) \
    __builtin_wasm_table_grow(_table, _ref, _delta)
#define wasm_table_fill(_table, _idx, _ref, _len) \
    __builtin_wasm_table_fill(_table, _idx, _ref, _len)
#define wasm_table_copy(_dst_table, _src_table, _dst_idx, _src_idx, _len) \
    __builtin_wasm_table_copy(_dst_table, _src_table, _dst_idx, _src_idx, _len)
#endif /* wasm_has_reference_types */

#if wasm_has_gc
#define wasm_testFnPtrSignature(_fn_ptr) \
    __builtin_wasm_test_function_pointer_signature(_fn_ptr)
#endif /* wasm_has_gc */

#endif /* wasm_builtin_supported */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* builtin_wasm__included */

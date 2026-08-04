/**
 * @copyright Copyright (c) 2024-2026 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    abi.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2026-07-29 (date of creation)
 * @updated 2026-07-30 (date of last update)
 * @ingroup dasae-headers(dh)/builtin/cfg
 * @prefix  abi
 *
 * @brief   Native C ABI facts required by DH
 * @details Reads the compiler-selected C implementation directly.  This file
 *          does not infer representation from an operating system or CPU name.
 */
#pragma once
#ifndef foundation_cfg_abi__included
#define foundation_cfg_abi__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

#include "comp.h"

#if !defined(__CHAR_BIT__) \
    || !defined(__SIZEOF_SHORT__) \
    || !defined(__SIZEOF_INT__) \
    || !defined(__SIZEOF_LONG__) \
    || !defined(__SIZEOF_LONG_LONG__) \
    || !defined(__SIZEOF_POINTER__) \
    || !defined(__SIZEOF_SIZE_T__)
#error "DH requires compiler-provided native C ABI width facts"
#endif

#define abi_bits_unit_16bit __comp_enum__abi_bits_unit_16bit
#define abi_bits_unit_32bit __comp_enum__abi_bits_unit_32bit
#define abi_bits_unit_64bit __comp_enum__abi_bits_unit_64bit
#define abi_bits_16 16
#define abi_bits_32 32
#define abi_bits_64 64

#define abi_char_bits __comp_int__abi_char_bits
#define abi_short_bits __comp_int__abi_short_bits
#define abi_int_bits __comp_int__abi_int_bits
#define abi_long_bits __comp_int__abi_long_bits
#define abi_long_long_bits __comp_int__abi_long_long_bits
#define abi_pointer_bits __comp_int__abi_pointer_bits
#define abi_size_bits __comp_int__abi_size_bits

#define abi_char_bytes __comp_int__abi_char_bytes
#define abi_short_bytes __comp_int__abi_short_bytes
#define abi_int_bytes __comp_int__abi_int_bytes
#define abi_long_bytes __comp_int__abi_long_bytes
#define abi_long_long_bytes __comp_int__abi_long_long_bytes
#define abi_pointer_bytes __comp_int__abi_pointer_bytes
#define abi_size_bytes __comp_int__abi_size_bytes

#define abi_long_unit __comp_enum__abi_long_unit
#define abi_size_unit __comp_enum__abi_size_unit

#define abi_long_is_32bit __comp_bool__abi_long_is_32bit
#define abi_long_is_64bit __comp_bool__abi_long_is_64bit
#define abi_size_is_16bit __comp_bool__abi_size_is_16bit
#define abi_size_is_32bit __comp_bool__abi_size_is_32bit
#define abi_size_is_64bit __comp_bool__abi_size_is_64bit
#define abi_pointer_is_16bit __comp_bool__abi_pointer_is_16bit
#define abi_pointer_is_32bit __comp_bool__abi_pointer_is_32bit
#define abi_pointer_is_64bit __comp_bool__abi_pointer_is_64bit

/* `long` is exposed only when its compatible fixed-width type would otherwise
 * duplicate a `_Generic` association. */
#define abi_long_needs_distinct_int_cases __comp_bool__abi_long_needs_distinct_int_cases

#define __comp_enum__abi_bits_unit_16bit 1
#define __comp_enum__abi_bits_unit_32bit 2
#define __comp_enum__abi_bits_unit_64bit 3

#define __comp_int__abi_char_bits (__CHAR_BIT__)
#define __comp_int__abi_short_bits (__SIZEOF_SHORT__ * __CHAR_BIT__)
#define __comp_int__abi_int_bits (__SIZEOF_INT__ * __CHAR_BIT__)
#define __comp_int__abi_long_bits (__SIZEOF_LONG__ * __CHAR_BIT__)
#define __comp_int__abi_long_long_bits (__SIZEOF_LONG_LONG__ * __CHAR_BIT__)
#define __comp_int__abi_pointer_bits (__SIZEOF_POINTER__ * __CHAR_BIT__)
#define __comp_int__abi_size_bits (__SIZEOF_SIZE_T__ * __CHAR_BIT__)

#define __comp_int__abi_char_bytes (__CHAR_BIT__ / 8)
#define __comp_int__abi_short_bytes (__SIZEOF_SHORT__)
#define __comp_int__abi_int_bytes (__SIZEOF_INT__)
#define __comp_int__abi_long_bytes (__SIZEOF_LONG__)
#define __comp_int__abi_long_long_bytes (__SIZEOF_LONG_LONG__)
#define __comp_int__abi_pointer_bytes (__SIZEOF_POINTER__)
#define __comp_int__abi_size_bytes (__SIZEOF_SIZE_T__)

#if __CHAR_BIT__ != 8
#error "DH requires 8-bit bytes"
#endif /* __CHAR_BIT__ != 8 */
#if __SIZEOF_LONG_LONG__ != 8
#error "DH requires 64-bit long long"
#endif /* __SIZEOF_LONG_LONG__ != 8 */
#if __SIZEOF_POINTER__ != __SIZEOF_SIZE_T__
#error "DH requires pointer and size types to share one representation width"
#endif /* __SIZEOF_POINTER__ != __SIZEOF_SIZE_T__ */
#if defined(__INTPTR_WIDTH__) && (__INTPTR_WIDTH__ != (__SIZEOF_POINTER__ * __CHAR_BIT__))
#error "DH requires intptr_t and pointer widths to agree"
#endif /* defined(__INTPTR_WIDTH__) && (__INTPTR_WIDTH__ != (__SIZEOF_POINTER__ * __CHAR_BIT__)) */
#if defined(__UINTPTR_WIDTH__) && (__UINTPTR_WIDTH__ != (__SIZEOF_POINTER__ * __CHAR_BIT__))
#error "DH requires uintptr_t and pointer widths to agree"
#endif /* defined(__UINTPTR_WIDTH__) && (__UINTPTR_WIDTH__ != (__SIZEOF_POINTER__ * __CHAR_BIT__)) */
#if defined(__SIZE_WIDTH__) && (__SIZE_WIDTH__ != (__SIZEOF_SIZE_T__ * __CHAR_BIT__))
#error "DH requires size_t width facts to agree"
#endif /* defined(__SIZE_WIDTH__) && (__SIZE_WIDTH__ != (__SIZEOF_SIZE_T__ * __CHAR_BIT__)) */
#if (__SIZEOF_POINTER__ != 2) && (__SIZEOF_POINTER__ != 4) && (__SIZEOF_POINTER__ != 8)
#error "DH supports only 16-bit, 32-bit, or 64-bit native pointer-size domains"
#endif /* (__SIZEOF_POINTER__ != 2) && (__SIZEOF_POINTER__ != 4) && (__SIZEOF_POINTER__ != 8) */
#if (__SIZEOF_LONG__ != 4) && (__SIZEOF_LONG__ != 8)
#error "DH requires long to be 32-bit or 64-bit"
#endif /* (__SIZEOF_LONG__ != 4) && (__SIZEOF_LONG__ != 8) */

#if __SIZEOF_LONG__ == 8
#define __comp_enum__abi_long_unit abi_bits_unit_64bit
#else
#define __comp_enum__abi_long_unit abi_bits_unit_32bit
#endif
#if __SIZEOF_SIZE_T__ == 8
#define __comp_enum__abi_size_unit abi_bits_unit_64bit
#elif __SIZEOF_SIZE_T__ == 4
#define __comp_enum__abi_size_unit abi_bits_unit_32bit
#else
#define __comp_enum__abi_size_unit abi_bits_unit_16bit
#endif

#define __comp_bool__abi_long_is_32bit pp_Tok_eql(abi_long_unit, abi_bits_unit_32bit)
#define __comp_bool__abi_long_is_64bit pp_Tok_eql(abi_long_unit, abi_bits_unit_64bit)
#define __comp_bool__abi_size_is_16bit pp_Tok_eql(abi_size_unit, abi_bits_unit_16bit)
#define __comp_bool__abi_size_is_32bit pp_Tok_eql(abi_size_unit, abi_bits_unit_32bit)
#define __comp_bool__abi_size_is_64bit pp_Tok_eql(abi_size_unit, abi_bits_unit_64bit)
#define __comp_bool__abi_pointer_is_16bit abi_size_is_16bit
#define __comp_bool__abi_pointer_is_32bit abi_size_is_32bit
#define __comp_bool__abi_pointer_is_64bit abi_size_is_64bit

/* Clang exposes `__INT{32,64}_C_SUFFIX__`; GCC exposes the equivalent
 * function-like `__INT{32,64}_C(value)`.  In both forms, an `L` suffix means
 * the exact-width signed integer is already `long`.  An empty suffix or `LL`
 * means `long` is a distinct compatible type and needs its own `_Generic`
 * association. */
#define __abi_suffix_is_L(_$suffix) __abi_suffix_is_L_I(_$suffix)
#define __abi_suffix_is_L_I(_$suffix) __abi_suffix_is_L_II(_$suffix)
#define __abi_suffix_is_L_II(_$suffix) __abi_suffix_is_L_##_$suffix
#define __abi_suffix_is_L_ 0
#define __abi_suffix_is_L_L 1
#define __abi_suffix_is_L_LL 0

#define __abi_int_literal_is_L(_$literal) __abi_int_literal_is_L_I(_$literal)
#define __abi_int_literal_is_L_I(_$literal) __abi_int_literal_is_L_II(_$literal)
#define __abi_int_literal_is_L_II(_$literal) __abi_int_literal_is_L_##_$literal
#define __abi_int_literal_is_L_1 0
#define __abi_int_literal_is_L_1L 1
#define __abi_int_literal_is_L_1LL 0

#if __SIZEOF_LONG__ == 8
#if defined(__INT64_C_SUFFIX__)
#define __comp_bool__abi_long_needs_distinct_int_cases pp_not(__abi_suffix_is_L(__INT64_C_SUFFIX__))
#elif defined(__INT64_C)
#define __comp_bool__abi_long_needs_distinct_int_cases pp_not(__abi_int_literal_is_L(__INT64_C(1)))
#else
#error "DH requires the compiler to expose the exact-width 64-bit integer literal form"
#endif
#else
#if defined(__INT32_C_SUFFIX__)
#define __comp_bool__abi_long_needs_distinct_int_cases pp_not(__abi_suffix_is_L(__INT32_C_SUFFIX__))
#elif defined(__INT32_C)
#define __comp_bool__abi_long_needs_distinct_int_cases pp_not(__abi_int_literal_is_L(__INT32_C(1)))
#else
#error "DH requires the compiler to expose the exact-width 32-bit integer literal form"
#endif
#endif

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* foundation_cfg_abi__included */

/**
 * @copyright Copyright (c) 2025 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    mem.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2025-02-12 (date of creation)
 * @updated 2025-02-12 (date of last update)
 * @version v0.1-alpha
 * @ingroup dasae-headers(dh)/builtin
 * @prefix  (none)
 */
#ifndef builtin_mem__included
#define builtin_mem__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Macros and Declarations ========================================*/

#define bits_per_bit __comp_int__bits_per_bit
#define bits_per_octet __comp_int__bits_per_octet
#define bits_per_hextet __comp_int__bits_per_hextet
#define bits_per_quadlet __comp_int__bits_per_quadlet
#define bits_per_octlet __comp_int__bits_per_octlet
#define bits_per_hexlet __comp_int__bits_per_hexlet

#define popcount(_Expr...) \
    /* \
    This function counts the number of set bits (1s) in an unsigned integer. \
    For example, __builtin_popcount(5) would return 2, \
    as the binary representation of 5 is 101. \
    */ \
    __op__popcount(_Expr)
#define countLeadingZero(_Expr...) \
    /* \
    This function returns the number of leading zero bits in an unsigned integer. \
    For instance, __builtin_clz(16) would return 27 on a 32-bit system, \
    as the binary representation of 16 is 10000, which has 27 leading zeros. \
    */ \
    __op__countLeadingZero(_Expr)
#define countTrailingZero(_Expr...) \
    /* \
    This function counts the number of trailing zero bits in an unsigned integer. \
    For example, __builtin_ctz(16) would return 4, as the binary representation of 16 is 10000, \
    which has 4 trailing zeros. \
    */ \
    __op__countTrailingZero(_Expr)

#undef memcpy
#define memcpy(_p_dst, _p_src, _len...) __op__memcpy(_p_dst, _p_src, _len)
#define raw_memcpy(_p_dst, _p_src, _len...) ____raw_memcpy(_p_dst, _p_src, _len)
#undef memmove
#define memmove(_p_dst, _p_src, _len...) __op__memmove(_p_dst, _p_src, _len)
#define raw_memmove(_p_dst, _p_src, _len...) ____raw_memmove(_p_dst, _p_src, _len)
#undef memset
#define memset(_p_dst, _byte_src, _len...) __op__memset(_p_dst, _byte_src, _len)
#define raw_memset(_p_dst, _byte_src, _len...) ____raw_memset(_p_dst, _byte_src, _len)
#define memset0(_p_dst, _len...) __op__memset0(_p_dst, _len)
#define raw_memset0(_p_dst, _len...) ____raw_memset0(_p_dst, _len)
#undef memcmp
#define memcmp(_p_lhs, _p_rhs, _len...) __op__memcmp(_p_lhs, _p_rhs, _len)
#define raw_memcmp(_p_lhs, _p_rhs, _len...) ____raw_memcmp(_p_lhs, _p_rhs, _len)
#undef alloca
#define alloca(_len_bytes...) ____alloca(_len_bytes)

/*========== Macros and Definitions =========================================*/

#define __comp_int__bits_per_bit 1
#define __comp_int__bits_per_octet 8
#define __comp_int__bits_per_hextet 16
#define __comp_int__bits_per_quadlet 32
#define __comp_int__bits_per_octlet 64
#define __comp_int__bits_per_hexlet 128

#define __op__popcount(_Expr...) __builtin_popcount(_Expr)
#define __op__countLeadingZero(_Expr...) __builtin_clz(_Expr)
#define __op__countTrailingZero(_Expr...) __builtin_ctz(_Expr)

#define __op__memcpy(_p_dst, _p_src, _len...) __builtin_memcpy(_p_dst, _p_src, _len)
#define ____raw_memcpy(_p_dst, _p_src, _len...) __builtin_memcpy(_p_dst, _p_src, _len)
#define __op__memmove(_p_dst, _p_src, _len...) __builtin_memmove(_p_dst, _p_src, _len)
#define ____raw_memmove(_p_dst, _p_src, _len...) __builtin_memmove(_p_dst, _p_src, _len)
#define __op__memset(_p_dst, _byte_src, _len...) __builtin_memset(_p_dst, _byte_src, _len)
#define ____raw_memset(_p_dst, _byte_src, _len...) __builtin_memset(_p_dst, _byte_src, _len)
#define __op__memset0(_p_dst, _len...) __builtin_memset(_p_dst, 0x00, _len)
#define ____raw_memset0(_p_dst, _len...) __builtin_memset(_p_dst, 0x00, _len)
#define __op__memcmp(_p_lhs, _p_rhs, _len...) __builtin_memcmp(_p_lhs, _p_rhs, _len)
#define ____raw_memcmp(_p_lhs, _p_rhs, _len...) __builtin_memcmp(_p_lhs, _p_rhs, _len)
#define ____alloca(_len_bytes...) __builtin_alloca(_len_bytes)

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* builtin_mem__included */

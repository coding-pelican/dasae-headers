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
 * @prefix  bti
 *
 * @brief
 * @details
 */

#ifndef BTI_MEM_INCLUDED
#define BTI_MEM_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Macros and Definitions =========================================*/

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

#define memset(val_ptr, i32_value, usize_len...) __op__memset(val_ptr, i32_value, usize_len)

#define memcpy(val_dst_ptr, val_src_ptr, usize_len...) __op__memcpy(val_dst_ptr, val_src_ptr, usize_len)

#define memmove(val_dst_ptr, val_src_ptr, usize_len...) __op__memmove(val_dst_ptr, val_src_ptr, usize_len)

#define memcmp(val_lhs_ptr, val_rhs_ptr, usize_len...) __op__memcmp(val_lhs_ptr, val_rhs_ptr, usize_len)

#undef alloca
#define alloca(usize_bytes...) __op__alloca(usize_bytes)

/*========== Macros and Implementations =====================================*/

#define __op__popcount(_Expr...) __builtin_popcount(_Expr)
#define __op__countLeadingZero(_Expr...) __builtin_clz(_Expr)
#define __op__countTrailingZero(_Expr...) __builtin_ctz(_Expr)

#define __op__memset(val_ptr, i32_value, usize_len...) __builtin_memset(val_ptr, i32_value, usize_len)
#define __op__memcpy(val_dst_ptr, val_src_ptr, usize_len...) __builtin_memcpy(val_dst_ptr, val_src_ptr, usize_len)
#define __op__memmove(val_dst_ptr, val_src_ptr, usize_len...) __builtin_memmove(val_dst_ptr, val_src_ptr, usize_len)
#define __op__memcmp(val_lhs_ptr, val_rhs_ptr, usize_len...) __builtin_memcmp(val_lhs_ptr, val_rhs_ptr, usize_len)
#define __op__alloca(usize_bytes...) __builtin_alloca(usize_bytes)

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* BTI_MEM_INCLUDED */

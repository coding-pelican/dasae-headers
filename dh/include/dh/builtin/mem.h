/**
 * @copyright Copyright (c) 2025 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    mem.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2025-02-12 (date of creation)
 * @updated 2025-02-12 (date of last update)
 * @version v0.1-alpha
 * @ingroup dasae-headers(dh)/bti
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

#define bti_popcount(_Expr...) \
    /* \
    This function counts the number of set bits (1s) in an unsigned integer. \
    For example, __builtin_popcount(5) would return 2, \
    as the binary representation of 5 is 101. \
    */ \
    FUNC__bti_popcount(_Expr)

#define bti_clz(_Expr...) \
    /* \
    This function returns the number of leading zero bits in an unsigned integer. \
    For instance, __builtin_clz(16) would return 27 on a 32-bit system, \
    as the binary representation of 16 is 10000, which has 27 leading zeros. \
    */ \
    FUNC__bti_clz(_Expr)

#define bti_ctz(_Expr...) \
    /* \
    This function counts the number of trailing zero bits in an unsigned integer. \
    For example, __builtin_ctz(16) would return 4, as the binary representation of 16 is 10000, \
    which has 4 trailing zeros. \
    */ \
    FUNC__bti_ctz(_Expr)

#define bti_memset(val_ptr, i32_value, usize_len...) \
    FUNC__bti_memset(val_ptr, i32_value, usize_len)

#define bti_memcpy(val_dst_ptr, val_src_ptr, usize_len...) \
    FUNC__bti_memcpy(val_dst_ptr, val_src_ptr, usize_len)

#define bti_memmove(val_dst_ptr, val_src_ptr, usize_len...) \
    FUNC__bti_memmove(val_dst_ptr, val_src_ptr, usize_len)

#define bti_memcmp(val_lhs_ptr, val_rhs_ptr, usize_len...) \
    FUNC__bti_memcmp(val_lhs_ptr, val_rhs_ptr, usize_len)

#define bti_memchr(val_ptr, i32_value, usize_len...) \
    FUNC__bti_memchr(val_ptr, i32_value, usize_len)

#define bti_alloca(usize_bytes...) \
    FUNC__bti_alloca(usize_bytes)

/*========== Macros and Implementations =====================================*/

#define FUNC__bti_popcount(_Expr...) __builtin_popcount(_Expr)
#define FUNC__bti_clz(_Expr...)      __builtin_clz(_Expr)
#define FUNC__bti_ctz(_Expr...)      __builtin_ctz(_Expr)

#define FUNC__bti_memset(val_ptr, i32_value, usize_len...)        __builtin_memset(val_ptr, i32_value, usize_len)
#define FUNC__bti_memcpy(val_dst_ptr, val_src_ptr, usize_len...)  __builtin_memcpy(val_dst_ptr, val_src_ptr, usize_len)
#define FUNC__bti_memmove(val_dst_ptr, val_src_ptr, usize_len...) __builtin_memmove(val_dst_ptr, val_src_ptr, usize_len)
#define FUNC__bti_memcmp(val_lhs_ptr, val_rhs_ptr, usize_len...)  __builtin_memcmp(val_lhs_ptr, val_rhs_ptr, usize_len)
#define FUNC__bti_memchr(val_ptr, i32_value, usize_len...)        __builtin_memchr(val_ptr, i32_value, usize_len)
#define FUNC__bti_alloca(usize_bytes...)                          __builtin_alloca(usize_bytes)

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* BTI_MEM_INCLUDED */

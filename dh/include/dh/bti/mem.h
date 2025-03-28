/**
 * @copyright Copyright 2024-2025. Gyeongtae Kim All rights reserved.
 *
 * @file    bti_mem.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2025-02-12 (date of creation)
 * @updated 2025-03-28 (date of last update)
 * @version v0.1-alpha.2
 * @ingroup dasae-headers(dh)/bti
 * @prefix  bti_mem
 *
 * @brief   Memory operations and bit manipulation
 * @details Provides memory manipulation functions and bit operations.
 *          Part of the BTI (Built-in) module for core system configuration.
 */

#ifndef BTI_MEM_INCLUDED
#define BTI_MEM_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "common.h"

/*========== Macros and Declarations =========================================*/

/* Bit Operations */

#define bti_mem_popcnt(_Expr...)                        \
    /**                                                 \
     * @brief Count number of set bits in integer       \
     * @param _Expr Integer expression to count bits in \
     * @return Number of bits set to 1                  \
     */                                                 \
    comp_impl__bti_mem_popcnt(_Expr)
#define bti_mem_clz(_Expr...)                    \
    /**                                          \
     * @brief Count leading zero bits in integer \
     * @param _Expr Integer expression           \
     * @return Number of leading zeros           \
     */                                          \
    comp_impl__bti_mem_clz(_Expr)
#define bti_mem_ctz(_Expr...)                     \
    /**                                           \
     * @brief Count trailing zero bits in integer \
     * @param _Expr Integer expression            \
     * @return Number of trailing zeros           \
     */                                           \
    comp_impl__bti_mem_ctz(_Expr)

/* Memory Operations */

#define bti_mem_set(val_ptr, i32_value, usize_len...) \
    /**                                               \
     * @brief Set memory to specified value           \
     * @param val_ptr Pointer to memory               \
     * @param i32_value Value to set (as int)         \
     * @param usize_len Number of bytes               \
     * @return Pointer to the memory area             \
     */                                               \
    comp_impl__bti_mem_set(val_ptr, i32_value, usize_len)
#define bti_mem_copy(val_dst_ptr, val_src_ptr, usize_len...) \
    /**                                                      \
     * @brief Copy memory from source to destination         \
     * @param val_dst_ptr Destination pointer                \
     * @param val_src_ptr Source pointer                     \
     * @param usize_len Number of bytes                      \
     * @return Pointer to the destination memory area        \
     */                                                      \
    comp_impl__bti_mem_copy(val_dst_ptr, val_src_ptr, usize_len)
#define bti_mem_move(val_dst_ptr, val_src_ptr, usize_len...) \
    /**                                                      \
     * @brief Move memory (handles overlapping regions)      \
     * @param val_dst_ptr Destination pointer                \
     * @param val_src_ptr Source pointer                     \
     * @param usize_len Number of bytes                      \
     * @return Pointer to the destination memory area        \
     */                                                      \
    comp_impl__bti_mem_move(val_dst_ptr, val_src_ptr, usize_len)
#define bti_mem_cmp(val_lhs_ptr, val_rhs_ptr, usize_len...) \
    /**                                                     \
     * @brief Compare memory regions                        \
     * @param val_lhs_ptr First memory region               \
     * @param val_rhs_ptr Second memory region              \
     * @param usize_len Number of bytes to compare          \
     * @return <0 if lhs<rhs, 0 if equal, >0 if lhs>rhs     \
     */                                                     \
    comp_impl__bti_mem_cmp(val_lhs_ptr, val_rhs_ptr, usize_len)
#define bti_mem_char(val_ptr, i32_value, usize_len...) \
    /**                                                \
     * @brief Find character in memory block           \
     * @param val_ptr Memory block to search           \
     * @param i32_value Character to find (as int)     \
     * @param usize_len Number of bytes to search      \
     * @return Pointer to first occurrence or NULL     \
     */                                                \
    comp_impl__bti_mem_char(val_ptr, i32_value, usize_len)
#define bti_mem_allocStack(usize_bytes...)          \
    /**                                             \
     * @brief Allocate memory on stack              \
     * @param usize_bytes Size in bytes to allocate \
     * @return Pointer to allocated memory          \
     */                                             \
    comp_impl__bti_mem_allocStack(usize_bytes)

/*========== Macros and Definitions =========================================*/

/* Bit Operations - Implementations */

#define comp_impl__bti_mem_popcnt(_Expr...) __builtin_popcount(_Expr)
#define comp_impl__bti_mem_clz(_Expr...)    __builtin_clz(_Expr)
#define comp_impl__bti_mem_ctz(_Expr...)    __builtin_ctz(_Expr)

/* Memory Operations - Implementations */

#define comp_impl__bti_mem_set(val_ptr, i32_value, usize_len...) \
    __builtin_memset(val_ptr, i32_value, usize_len)
#define comp_impl__bti_mem_copy(val_dst_ptr, val_src_ptr, usize_len...) \
    __builtin_memcpy(val_dst_ptr, val_src_ptr, usize_len)
#define comp_impl__bti_mem_move(val_dst_ptr, val_src_ptr, usize_len...) \
    __builtin_memmove(val_dst_ptr, val_src_ptr, usize_len)
#define comp_impl__bti_mem_cmp(val_lhs_ptr, val_rhs_ptr, usize_len...) \
    __builtin_memcmp(val_lhs_ptr, val_rhs_ptr, usize_len)
#define comp_impl__bti_mem_char(val_ptr, i32_value, usize_len...) \
    __builtin_memchr(val_ptr, i32_value, usize_len)
#define comp_impl__bti_mem_allocStack(usize_bytes...) \
    __builtin_alloca(usize_bytes)

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* BTI_MEM_INCLUDED */

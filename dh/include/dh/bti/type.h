/**
 * @copyright Copyright 2024-2025. Gyeongtae Kim All rights reserved.
 *
 * @file    type.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2024-12-23 (date of creation)
 * @updated 2025-03-28 (date of last update)
 * @version v0.1-alpha.2
 * @ingroup dasae-headers(dh)/bti
 * @prefix  bti_type
 *
 * @brief   Type information and checking utilities
 * @details Provides compile-time type analysis and validation capabilities.
 *          Part of the BTI (Built-in) module for core system configuration.
 */

#ifndef BTI_TYPE_INCLUDED
#define BTI_TYPE_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include <stdalign.h>
#include "common.h"

/*========== Macros and Declarations =========================================*/

/* Type Size and Alignment */

#define bti_type_sizeOf$(T_or_val)                        \
    /**                                                   \
     * @brief Get size of a type or value at compile time \
     * @param T_or_val Type or value to measure           \
     * @return usize Size in bytes                        \
     */                                                   \
    comp_impl__bti_type_sizeOf$(T_or_val)
#define bti_type_alignOf$(T)                        \
    /**                                             \
     * @brief Get alignment requirement of a type   \
     * @param T Type to check alignment of          \
     * @return usize Alignment requirement in bytes \
     */                                             \
    comp_impl__bti_type_alignOf$(T)
#define bti_type_alignAs(val_align)                    \
    /**                                                \
     * @brief Specify alignment requirement for a type \
     * @param val_align Alignment value in bytes       \
     * @return Alignment specifier                     \
     */                                                \
    comp_impl__bti_type_alignAs(val_align)
#define bti_type_countOf(var_arr)                \
    /**                                          \
     * @brief Get number of elements in an array \
     * @param var_arr Array to count elements of \
     * @return usize Number of elements          \
     */                                          \
    comp_impl__bti_type_countOf(var_arr)

/* Type Information */

#define bti_type_TypeOf(_Expr)                          \
    /**                                                 \
     * @brief Get type of an expression at compile time \
     * @param _Expr Value or expression to get type of  \
     * @return Type of the expression                   \
     */                                                 \
    comp_impl__bti_type_TypeOf(_Expr)
#define bti_type_TypeUnqualOf(_Expr)                   \
    /**                                                \
     * @brief Get unqualified type of an expression    \
     * @param _Expr Value or expression to get type of \
     * @return Unqualified type of the expression      \
     */                                                \
    comp_impl__bti_type_TypeUnqualOf(_Expr)
#define bti_type_isConstant(_Expr)                         \
    /**                                                    \
     * @brief Check if expression is compile-time constant \
     * @param _Expr Expression to check                    \
     * @return bool True if constant expression            \
     */                                                    \
    comp_impl__bti_type_isConstant(_Expr)
#define bti_type_isSame$(T_lhs, T_rhs)         \
    /**                                        \
     * @brief Compare two types for equality   \
     * @param T_lhs First type to compare      \
     * @param T_rhs Second type to compare     \
     * @return bool True if types are the same \
     */                                        \
    comp_impl__bti_type_isSame$(T_lhs, T_rhs)

/* Type Classification */

#define bti_type_isUnsigned$(T)              \
    /**                                      \
     * @brief Check if type is unsigned      \
     * @param T Type to check                \
     * @return bool True if unsigned integer \
     */                                      \
    comp_impl__bti_type_isUnsigned$(T)
#define bti_type_isSigned$(T)              \
    /**                                    \
     * @brief Check if type is signed      \
     * @param T Type to check              \
     * @return bool True if signed integer \
     */                                    \
    comp_impl__bti_type_isSigned$(T)
#define bti_type_isInt$(T)              \
    /**                                 \
     * @brief Check if type is integer  \
     * @param T Type to check           \
     * @return bool True if any integer \
     */                                 \
    comp_impl__bti_type_isInt$(T)
#define bti_type_isFlt$(T)                 \
    /**                                    \
     * @brief Check if type is float       \
     * @param T Type to check              \
     * @return bool True if floating-point \
     */                                    \
    comp_impl__bti_type_isFlt$(T)
#define bti_type_isBool$(T)            \
    /**                                \
     * @brief Check if type is boolean \
     * @param T Type to check          \
     * @return bool True if boolean    \
     */                                \
    comp_impl__bti_type_isBool$(T)

/*========== Macros and Definitions =========================================*/

/* Type Size and Alignment - Implementations */

#define comp_impl__bti_type_sizeOf$(T_or_val)  sizeof(T_or_val)
#define comp_impl__bti_type_alignOf$(T)        alignof(T)
#define comp_impl__bti_type_alignAs(val_align) alignas(val_align)
#define comp_impl__bti_type_countOf(var_arr)   (sizeof(var_arr) / sizeof((var_arr)[0]))

/* Type Information - Implementations */

#define comp_impl__bti_type_TypeOf(_Expr)         __typeof__(_Expr)
#define comp_impl__bti_type_TypeUnqualOf(_Expr)   __typeof_unqual(_Expr)
#define comp_impl__bti_type_isConstant(_Expr)     __builtin_constant_p(_Expr)
#define comp_impl__bti_type_isSame$(T_lhs, T_rhs) __builtin_types_compatible_p(T_lhs, T_rhs)

/* Type Classification - Implementations */

#define comp_impl__bti_type_isUnsigned$(T) _Generic( \
    (T){ 0 },                                        \
    u8: true,                                        \
    u16: true,                                       \
    u32: true,                                       \
    u64: true,                                       \
    default: false                                   \
)
#define comp_impl__bti_type_isSigned$(T) _Generic( \
    (T){ 0 },                                      \
    i8: true,                                      \
    i16: true,                                     \
    i32: true,                                     \
    i64: true,                                     \
    default: false                                 \
)
#define comp_impl__bti_type_isInt$(T) \
    (comp_impl__bti_type_isUnsigned$(T) || comp_impl__bti_type_isSigned$(T))
#define comp_impl__bti_type_isFlt$(T) _Generic( \
    (T){ 0 },                                   \
    f32: true,                                  \
    f64: true,                                  \
    default: false                              \
)
#define comp_impl__bti_type_isBool$(T) _Generic( \
    (T){ 0 },                                    \
    bool: true,                                  \
    default: false                               \
)

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* BTI_TYPE_INCLUDED */

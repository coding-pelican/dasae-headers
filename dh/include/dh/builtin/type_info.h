/**
 * @copyright Copyright 2024-2025. Gyeongtae Kim All rights reserved.
 *
 * @file    type_info.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2024-12-23 (date of creation)
 * @updated 2025-01-15 (date of last update)
 * @version v0.1-alpha.1
 * @ingroup dasae-headers(dh)/builtin
 * @prefix  NONE
 *
 * @brief   Type information and checking utilities
 * @details Provides compile-time type analysis and validation capabilities
 */

#ifndef BUILTIN_TYPE_INFO_INCLUDED
#define BUILTIN_TYPE_INFO_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include <stdalign.h>

/*========== Macros and Definitions =========================================*/

/* Type Size and Alignment */
#define sizeOf$(T) sizeOf(T)
#define sizeOf(T_or_val)                                  \
    /**                                                   \
     * @brief Get size of a type or value at compile time \
     *                                                    \
     * @param T_or_val Type or value to measure           \
     * @return usize Size in bytes                        \
     */                                                   \
    FUNC__sizeOf(T_or_val)

#define alignOf$(T) alignOf(T)
#define alignOf(T)                                  \
    /**                                             \
     * @brief Get alignment requirement of a type   \
     *                                              \
     * @param T Type to check alignment of          \
     * @return usize Alignment requirement in bytes \
     */                                             \
    FUNC__alignOf(T)

#define alignAs(val_align)                             \
    /**                                                \
     * @brief Specify alignment requirement for a type \
     *                                                 \
     * @param val_align Alignment value in bytes       \
     * @return Alignment specifier                     \
     */                                                \
    FUNC__alignAs(val_align)

#define countOf(var_arr...)                      \
    /**                                          \
     * @brief Get number of elements in an array \
     *                                           \
     * @param var_arr Array to count elements of \
     * @return usize Number of elements          \
     */                                          \
    FUNC__countOf(var_arr)
#define safeCountOf(var_arr...) FUNC__safeCountOf(var_arr)

/* Type Information */
#define TypeOf(_Expr)                                   \
    /**                                                 \
     * @brief Get type of an expression at compile time \
     *                                                  \
     * @param _Expr Value or expression to get type of  \
     * @return Type of the expression                   \
     */                                                 \
    FUNC__TypeOf(_Expr)

#define isConstantExpr(_Expr)                              \
    /**                                                    \
     * @brief Check if expression is compile-time constant \
     *                                                     \
     * @param _Expr Expression to check                    \
     * @return bool True if constant expression            \
     */                                                    \
    FUNC__isConstantExpr(_Expr)

#define isSameType$(T_lhs, T_rhs) isSameType(T_lhs, T_rhs)
#define isSameType(T_lhs, T_rhs)               \
    /**                                        \
     * @brief Compare two types for equality   \
     *                                         \
     * @param T_lhs First type to compare      \
     * @param T_rhs Second type to compare     \
     * @return bool True if types are the same \
     */                                        \
    FUNC__isSameType(T_lhs, T_rhs)

/* Type Classification */
#define isUnsigned(T)                              \
    /**                                            \
     * @brief Check if type is an unsigned integer \
     *                                             \
     * @param T Type to check                      \
     * @return bool True if unsigned integer type  \
     */                                            \
    FUNC__isUnsigned(T)

#define isSigned(T)                             \
    /**                                         \
     * @brief Check if type is a signed integer \
     *                                          \
     * @param T Type to check                   \
     * @return bool True if signed integer type \
     */                                         \
    FUNC__isSigned(T)

#define isInt(T)                                \
    /**                                         \
     * @brief Check if type is any integer type \
     *                                          \
     * @param T Type to check                   \
     * @return bool True if integer type        \
     */                                         \
    FUNC__isInt(T)

#define isFlt(T)                                     \
    /**                                              \
     * @brief Check if type is a floating point type \
     *                                               \
     * @param T Type to check                        \
     * @return bool True if floating point type      \
     */                                              \
    FUNC__isFlt(T)

#define isBool(T)                             \
    /**                                       \
     * @brief Check if type is a boolean type \
     *                                        \
     * @param T Type to check                 \
     * @return bool True if boolean type      \
     */                                       \
    FUNC__isBool(T)

/*========== Implementation Macros =========================================*/

#define FUNC__sizeOf(T_or_val) \
    sizeof(T_or_val)

#define FUNC__alignAs(val_align) \
    alignas(val_align)

#define FUNC__alignOf(T) \
    alignof(T)

#define FUNC__countOf(var_arr...) \
    (sizeof(var_arr) / sizeof(var_arr[0]))

#define FUNC__safeCountOf(var_arr...) eval({ \
    let         _arr_ptr = &var_arr;         \
    eval_return countOf(*_arr_ptr);          \
})

#define FUNC__TypeOf(val) \
    __typeof__(val)

#define FUNC__isConstantExpr(_Expr) \
    __builtin_constant_p(_Expr)

#define FUNC__isSameType(T_lhs, T_rhs) \
    __builtin_types_compatible_p(T_lhs, T_rhs)

#define FUNC__isUnsigned(T) _Generic( \
    (T){ 0 },                         \
    u8: true,                         \
    u16: true,                        \
    u32: true,                        \
    u64: true,                        \
    default: false                    \
)

#define FUNC__isSigned(T) _Generic( \
    (T){ 0 },                       \
    i8: true,                       \
    i16: true,                      \
    i32: true,                      \
    i64: true,                      \
    default: false                  \
)

#define FUNC__isInt(T) \
    (isUnsigned(T) || isSigned(T))

#define FUNC__isFlt(T) _Generic( \
    (T){ 0 },                    \
    f32: true,                   \
    f64: true,                   \
    default: false               \
)

#define FUNC__isBool(T) _Generic( \
    (T){ 0 },                     \
    bool: true,                   \
    default: false                \
)

/*========== Example Usage (Disabled to prevent compilation) ================*/

#if /* Example Usage */ 0
#include "dh/core.h"

void example_usage(void) {
    /* Size and alignment checks */
    usize int_size  = sizeOf(i32);
    usize int_align = alignOf(i32);

    /* Array operations */
    i32   numbers[] = { 1, 2, 3, 4, 5 };
    usize arr_count = countOf(numbers);

    /* Type checking */
    i32 value                   = 42;
    TypeOf(value) another_value = 43;

    /* Type classification */
    claim_assert_static_msg(isInt(i32), "i32 should be an integer type");
    claim_assert_static_msg(isUnsigned(u32), "u32 should be unsigned");
    claim_assert_static_msg(isSigned(i32), "i32 should be signed");
    claim_assert_static_msg(isFlt(f64), "f64 should be floating point");
    claim_assert_static_msg(isBool(bool), "bool should be boolean type");

    /* Type comparison */
    claim_assert_static_msg(isSameType(TypeOf(value), i32), "Types should match");
}
#endif /* Example Usage */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* BUILTIN_TYPE_INFO_INCLUDED */

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
#define sizeOf(_T_or_val)                                 \
    /**                                                   \
     * @brief Get size of a type or value at compile time \
     *                                                    \
     * @param _T_or_val Type or value to measure          \
     * @return usize Size in bytes                        \
     */                                                   \
    FUNC__sizeOf(_T_or_val)

#define alignOf(_T)                                 \
    /**                                             \
     * @brief Get alignment requirement of a type   \
     *                                              \
     * @param _T Type to check alignment of         \
     * @return usize Alignment requirement in bytes \
     */                                             \
    FUNC__alignOf(_T)

#define alignAs(_val_align)                            \
    /**                                                \
     * @brief Specify alignment requirement for a type \
     *                                                 \
     * @param _val_align Alignment value in bytes      \
     * @return Alignment specifier                     \
     */                                                \
    FUNC__alignAs(_val_align)

#define countOf(_var_arr...)                      \
    /**                                           \
     * @brief Get number of elements in an array  \
     *                                            \
     * @param _var_arr Array to count elements of \
     * @return usize Number of elements           \
     */                                           \
    FUNC__countOf(_var_arr)

/* Type Information */
#define TypeOf(_val)                                    \
    /**                                                 \
     * @brief Get type of an expression at compile time \
     *                                                  \
     * @param _val Value or expression to get type of   \
     * @return Type of the expression                   \
     */                                                 \
    FUNC__TypeOf(_val)

#define isConstantExpr(_expr)                              \
    /**                                                    \
     * @brief Check if expression is compile-time constant \
     *                                                     \
     * @param _expr Expression to check                    \
     * @return bool True if constant expression            \
     */                                                    \
    FUNC__isConstantExpr(_expr)

#define isSameType(_T_lhs, _T_rhs)             \
    /**                                        \
     * @brief Compare two types for equality   \
     *                                         \
     * @param _T_lhs First type to compare     \
     * @param _T_rhs Second type to compare    \
     * @return bool True if types are the same \
     */                                        \
    FUNC__isSameType(_T_lhs, _T_rhs)

/* Type Classification */
#define isUnsigned(_Type)                          \
    /**                                            \
     * @brief Check if type is an unsigned integer \
     *                                             \
     * @param _Type Type to check                  \
     * @return bool True if unsigned integer type  \
     */                                            \
    FUNC__isUnsigned(_Type)

#define isSigned(_Type)                         \
    /**                                         \
     * @brief Check if type is a signed integer \
     *                                          \
     * @param _Type Type to check               \
     * @return bool True if signed integer type \
     */                                         \
    FUNC__isSigned(_Type)

#define isInt(_Type)                            \
    /**                                         \
     * @brief Check if type is any integer type \
     *                                          \
     * @param _Type Type to check               \
     * @return bool True if integer type        \
     */                                         \
    FUNC__isInt(_Type)

#define isFlt(_Type)                                 \
    /**                                              \
     * @brief Check if type is a floating point type \
     *                                               \
     * @param _Type Type to check                    \
     * @return bool True if floating point type      \
     */                                              \
    FUNC__isFlt(_Type)

#define isBool(_Type)                         \
    /**                                       \
     * @brief Check if type is a boolean type \
     *                                        \
     * @param _Type Type to check             \
     * @return bool True if boolean type      \
     */                                       \
    FUNC__isBool(_Type)

/*========== Implementation Macros =========================================*/

#define FUNC__sizeOf(T_or_val) \
    sizeof(T_or_val)

#define FUNC__alignAs(val_align) \
    alignas(val_align)

#define FUNC__alignOf(T) \
    alignof(T)

#define FUNC__countOf(var_arr...) \
    (sizeof(var_arr) / sizeof(var_arr[0]))

#define FUNC__TypeOf(val) \
    __typeof__(val)

#define FUNC__isConstantExpr(_expr) \
    __builtin_constant_p(_expr)

#define FUNC__isSameType(T_lhs, T_rhs) \
    __builtin_types_compatible_p(T_lhs, T_rhs)

#define FUNC__isUnsigned(Type) _Generic( \
    (Type){ 0 },                         \
    u8: true,                            \
    u16: true,                           \
    u32: true,                           \
    u64: true,                           \
    default: false                       \
)

#define FUNC__isSigned(Type) _Generic( \
    (Type){ 0 },                       \
    i8: true,                          \
    i16: true,                         \
    i32: true,                         \
    i64: true,                         \
    default: false                     \
)

#define FUNC__isInt(Type) \
    (isUnsigned(Type) || isSigned(Type))

#define FUNC__isFlt(Type) _Generic( \
    (Type){ 0 },                    \
    f32: true,                      \
    f64: true,                      \
    default: false                  \
)

#define FUNC__isBool(Type) _Generic( \
    (Type){ 0 },                     \
    bool: true,                      \
    default: false                   \
)

/*========== Example Usage (Disabled to prevent compilation) ================*/

#if 0 /* Example Usage */
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

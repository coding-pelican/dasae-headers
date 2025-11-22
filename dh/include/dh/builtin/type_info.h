/**
 * @copyright Copyright (c) 2024-2025 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    type_info.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2024-12-23 (date of creation)
 * @updated 2025-02-02 (date of last update)
 * @version v0.1-alpha.2
 * @ingroup dasae-headers(dh)/builtin
 * @prefix  (none)
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

/*========== Macros and Definitions =========================================*/

/* Type Size and Alignment */
#define alignAs(_align /*: u32*/...) _Alignas(1ull << (_align))
#define alignOf$(_T... /*(u32)*/) (as$(u32)((64u - 1u) - __builtin_clzll(_Alignof(_T))))
#define sizeOf$(_T... /*(usize)*/) (as$(usize)(sizeof(_T)))
#define countOf$(_T... /*(usize)*/) (sizeOf$(_T) / sizeOf$(TypeOf((*as$(_T*)(0))[0])))

/* Type Information */
#define TypeOf(_Expr...) \
    /** \
     * @brief Get type of an expression at compile time \
     * \
     * @param _Expr Value or expression to get type of \
     * @return Type of the expression \
     */ \
    __typeof__(_Expr)

#define TypeOfUnqual(_Expr...) \
    /** \
     * @brief Get unqualified type of an expression at compile time \
     * \
     * @param _Expr Value or expression to get type of \
     * @return Unqualified type of the expression \
     */ \
    __typeof_unqual(_Expr)

#define isCompTimeFoldable(_Expr...) \
    /** \
     * @brief Check if expression is compile-time constant \
     * \
     * @param _Expr Expression to check \
     * @return bool True if constant expression \
     */ \
    __builtin_constant_p(_Expr)

#define isSameType$(T_lhs, T_rhs...) \
    /** \
     * @brief Compare two types for equality \
     * \
     * @param T_lhs First type to compare \
     * @param T_rhs Second type to compare \
     * @return bool True if types are the same \
     */ \
    __builtin_types_compatible_p(T_lhs, T_rhs)

#define isSameTypeUnqual$(T_lhs, T_rhs...) \
    /** \
     * @brief Compare two types for equality \
     * \
     * @param T_lhs First type to compare \
     * @param T_rhs Second type to compare \
     * @return bool True if types are the same \
     */ \
    __builtin_types_compatible_p(T_lhs, T_rhs)

/* Type Classification */
#define isUnsigned(T) \
    /** \
     * @brief Check if type is an unsigned integer \
     * \
     * @param T Type to check \
     * @return bool True if unsigned integer type \
     */ \
    FUNC__isUnsigned(T)

#define isSigned(T) \
    /** \
     * @brief Check if type is a signed integer \
     * \
     * @param T Type to check \
     * @return bool True if signed integer type \
     */ \
    FUNC__isSigned(T)

#define isInt(T) \
    /** \
     * @brief Check if type is any integer type \
     * \
     * @param T Type to check \
     * @return bool True if integer type \
     */ \
    FUNC__isInt(T)

#define isFlt(T) \
    /** \
     * @brief Check if type is a floating point type \
     * \
     * @param T Type to check \
     * @return bool True if floating point type \
     */ \
    FUNC__isFlt(T)

#define isBool(T) \
    /** \
     * @brief Check if type is a boolean type \
     * \
     * @param T Type to check \
     * @return bool True if boolean type \
     */ \
    FUNC__isBool(T)

/*========== Macros and Implementations =====================================*/

#define FUNC__TypeOf(_Expr...)


#define FUNC__TypeOfUnqual(_Expr...)


#define FUNC__isCompTimeFoldable(_Expr...)


#define FUNC__isSameType$(T_lhs, T_rhs...)


#define FUNC__isSameTypeUnqual(T_lhs, T_rhs...)


#define FUNC__isUnsigned(T) _Generic((T){0}, u8: true, u16: true, u32: true, u64: true, default: false)

#define FUNC__isSigned(T) _Generic((T){0}, i8: true, i16: true, i32: true, i64: true, default: false)

#define FUNC__isInt(T) (isUnsigned(T) || isSigned(T))

#define FUNC__isFlt(T) _Generic((T){0}, f32: true, f64: true, default: false)

#define FUNC__isBool(T) _Generic((T){0}, bool: true, default: false)

#if defined(__cplusplus) || (defined(__STDC_VERSION__) && __STDC_VERSION__ < 202311L)
#ifndef __cplusplus
#define alignas _Alignas
#define alignof _Alignof
#endif /* !__cplusplus */
#define __alignas_is_defined 1
#define __alignof_is_defined 1
#endif /* __STDC_VERSION__ */

/*========== Example Usage (Disabled to prevent compilation) ================*/

#if EXAMPLE_USAGE
#include "dh/core.h"

void example_usage(void) {
    /* Size and alignment checks */
    usize int_size = sizeOf(i32);
    usize int_align = alignOf(i32);

    /* Array operations */
    i32 numbers[] = {1, 2, 3, 4, 5};
    usize arr_count = countOf(numbers);

    /* Type checking */
    i32 value = 42;
    TypeOf(value) another_value = 43;

    /* Type classification */
    claim_assert_static_msg(isInt(i32), "i32 should be an integer type");
    claim_assert_static_msg(isUnsigned(u32), "u32 should be unsigned");
    claim_assert_static_msg(isSigned(i32), "i32 should be signed");
    claim_assert_static_msg(isFlt(f64), "f64 should be floating point");
    claim_assert_static_msg(isBool(bool), "bool should be boolean type");

    /* Type comparison */
    claim_assert_static_msg(isSameType$(TypeOf(value), i32), "Types should match");
}
#endif /* EXAMPLE_USAGE */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* BUILTIN_TYPE_INFO_INCLUDED */

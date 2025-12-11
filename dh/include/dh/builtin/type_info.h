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
#ifndef builtin_type_info__included
#define builtin_type_info__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Macros and Declarations ========================================*/

/* Type Size and Alignment */
#define alignAs$(_align /*: mem_Log2Align*/...) ___alignAs$(_align)
#define alignOf$(_T... /*(mem_Log2Align)*/) ___alignOf$(_T)
#define sizeOf$(_T... /*(usize)*/) ___sizeOf$(_T)
#define countOf$(_T... /*(usize)*/) ___countOf$(_T)

/* Type Information */
#define TypeOf(_Expr...) \
    /** \
     * @brief Get type of an expression at compile time \
     * \
     * @param _Expr Value or expression to get type of \
     * @return Type of the expression \
     */ \
    ____TypeOf(_Expr)
#define TypeOfUnqual(_Expr...) \
    /** \
     * @brief Get unqualified type of an expression at compile time \
     * \
     * @param _Expr Value or expression to get type of \
     * @return Unqualified type of the expression \
     */ \
    ____TypeOfUnqual(_Expr)

#define RefType$(_T_Val...) \
    /** \
     * @brief Get reference type of a container type at compile time \
     * \
     * @param _T_Val The value type \
     * @return Reference type of the value \
     */ \
    ____RefType$(_T_Val)
#define DerefType$(_T_Ptr...) \
    /** \
     * @brief Get dereference type of a container type at compile time \
     * \
     * @param _T_Ptr The pointer type \
     * @return Dereference type of the container \
     */ \
    ____DerefType$(_T_Ptr)
#define DerefTypeUnqual$(_T_Ptr...) \
    /** \
     * @brief Get dereference type of a container type at compile time \
     * \
     * @param _T_Ptr The pointer type \
     * @return Dereference type of the pointer \
     */ \
    ____DerefTypeUnqual$(_T_Ptr)

#define Type_eq$(_T_Lhs, _T_Rhs...) \
    /** \
     * @brief Compare two types for equality \
     * \
     * @param _T_Lhs First type to compare \
     * @param _T_Rhs Second type to compare \
     * @return bool True if types are the same \
     */ \
    ____Type_eq$(_T_Lhs, _T_Rhs)
#define Type_eqUnqual$(T_lhs, T_rhs...) \
    /** \
     * @brief Compare two types for equality \
     * \
     * @param _T_Lhs First type to compare \
     * @param _T_Rhs Second type to compare \
     * @return bool True if types are the same \
     */ \
    ____Type_eqUnqual$(_T_Lhs, _T_Rhs)

#define isCompTimeFoldable(_Expr...) \
    /** \
     * @brief Check if expression is compile-time constant \
     * \
     * @param _Expr Expression to check \
     * @return bool True if constant expression \
     */ \
    ____isCompTimeFoldable(_Expr)

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

/*========== Macros and Definitions =========================================*/

#define ___alignAs$(_align...) _Alignas(1ull << (_align))
#define ___alignOf$(_T...) (as$(u8)((64u - 1u) - __builtin_clzll(_Alignof(_T))))
#define ___sizeOf$(_T...) (as$(usize)(sizeof(_T)))
#define ___countOf$(_T...) (sizeOf$(_T) / sizeOf$(TypeOf((*as$(_T*)(0))[0])))

#define ____TypeOf(_Expr...) __typeof__(_Expr)
/* clang >= 16.0, gcc >= 13.1 */
#define GCC_VERSION (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__)
#if defined(__clang__) && GCC_VERSION >= 16
#define ____TypeOfUnqual(_Expr...) __typeof_unqual__(_Expr)
#else
#define ____TypeOfUnqual(_Expr...) TypeOf((TypeOf(_Expr))(lit0$((TypeOf(_Expr)))))
#endif

#define ____RefType$(_T_Val...) TypeOf(_T_Val*)
#define ____DerefType$(_T_Ptr...) TypeOf(*as$(_T_Ptr)(null))
#define ____DerefTypeUnqual$(_T_Ptr...) TypeOfUnqual(*as$(_T_Ptr)(null))

#define ____Type_eq$(_T_Lhs, _T_Rhs...) __builtin_types_compatible_p(_T_Lhs, _T_Rhs)
#define ____Type_eqUnqual$(_T_Lhs, _T_Rhs...) __builtin_types_compatible_p(TypeOfUnqual(_T_Lhs), TypeOfUnqual(_T_Rhs))

#define ____isCompTimeFoldable(_Expr...) __builtin_constant_p(_Expr)

#define FUNC__isUnsigned(T) _Generic((T){ 0 }, u8: true, u16: true, u32: true, u64: true, default: false)
#define FUNC__isSigned(T) _Generic((T){ 0 }, i8: true, i16: true, i32: true, i64: true, default: false)
#define FUNC__isInt(T) (isUnsigned(T) || isSigned(T))
#define FUNC__isFlt(T) _Generic((T){ 0 }, f32: true, f64: true, default: false)
#define FUNC__isBool(T) _Generic((T){ 0 }, bool: true, default: false)

#if defined(__cplusplus) || (defined(__STDC_VERSION__) && __STDC_VERSION__ < 202311L)
#ifndef __cplusplus
#define alignas _Alignas
#define alignof _Alignof
#endif /* !__cplusplus */
#define __alignas_is_defined 1
#define __alignof_is_defined 1
#endif /* __STDC_VERSION__ */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* builtin_type_info__included */

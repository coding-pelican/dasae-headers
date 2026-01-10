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

/*========== Includes =======================================================*/

#include "comp.h"

/*========== Macros and Declarations ========================================*/

/* Type Size and Alignment */
#define alignAs$(_T...) ____alignAs$(_T)
#define alignOf$(_T... /*(mem_Log2Align)*/) ____alignOf$(_T)
#define sizeOf$(_T... /*(usize)*/) ____sizeOf$(_T)
#define countOf$(_T... /*(usize)*/) ____countOf$(_T)

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

/*========== Macros and Definitions =========================================*/

#define ____alignAs$(_T...) _Alignas(1ull << alignOf$(_T))
#if on_comptime
#define ____alignOf__expand(...) __VA_ARGS__
#define ____alignOf$(_T...) $pragma_guard_( \
    "clang diagnostic push", "clang diagnostic ignored \"-Wpointer-arith\"", "clang diagnostic pop", \
    (as$(u8)( \
        (64u - 1u) \
        - __builtin_clzll(____alignOf__expand( \
            T_switch$ pp_begin(_T)( \
                T_case$((void)(0)), \
                T_default_(_Alignof(_T)) \
            ) pp_end \
        )) \
    )) \
)
#else /* !on_comptime */
#define ____alignOf$(_T...) $pragma_guard_( \
    "clang diagnostic push", "clang diagnostic ignored \"-Wpointer-arith\"", "clang diagnostic pop", \
    (as$(u8)((64u - 1u) - __builtin_clzll(_Alignof(_T)))) \
)
#endif
#define ____sizeOf__expand(...) __VA_ARGS__
#define ____sizeOf$(_T...) $pragma_guard_( \
    "clang diagnostic push", "clang diagnostic ignored \"-Wpointer-arith\"", "clang diagnostic pop", \
    (as$(usize)(____sizeOf__expand( \
        T_switch$ pp_begin(_T)( \
            T_case$((void)(0)), \
            T_default_(sizeof(_T)) \
        ) pp_end \
    ))) \
)
#define ____countOf$(_T...) (sizeOf$(_T) / sizeOf$(TypeOf((*as$(_T*)(null))[0])))

#define ____TypeOf(_Expr...) __typeof__(_Expr)
#define GCC_VERSION (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__)
#if defined(__clang__) && __clang_major__ >= 16
/* Clang >= 16.0 supports `__typeof_unqual__` */
#define ____TypeOfUnqual(_Expr...) __typeof_unqual__(_Expr)
#elif !defined(__clang__) && GCC_VERSION >= 130100
/* GCC >= 13.1 supports `__typeof_unqual__` */
#define ____TypeOfUnqual(_Expr...) __typeof_unqual__(_Expr)
#else
/* Fallback for no support of `__typeof_unqual__` */
#define ____TypeOfUnqual(_Expr...) TypeOf((TypeOf(_Expr))(lit0$((TypeOf(_Expr)))))
#endif

#define ____RefType$(_T_Val...) TypeOf(_T_Val*)
#define ____DerefType$(_T_Ptr...) TypeOf(*as$(_T_Ptr)(null))
#define ____DerefTypeUnqual$(_T_Ptr...) TypeOfUnqual(*as$(_T_Ptr)(null))

#define ____Type_eq$(_T_Lhs, _T_Rhs...) __builtin_types_compatible_p(_T_Lhs, _T_Rhs)
#define ____Type_eqUnqual$(_T_Lhs, _T_Rhs...) __builtin_types_compatible_p(TypeOfUnqual(_T_Lhs), TypeOfUnqual(_T_Rhs))

#define ____isCompTimeFoldable(_Expr...) __builtin_constant_p(_Expr)

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

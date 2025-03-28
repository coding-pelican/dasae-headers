/**
 * @copyright Copyright 2024-2025. Gyeongtae Kim All rights reserved.
 *
 * @file    comp_attr.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2024-03-28 (date of creation)
 * @updated 2024-03-28 (date of last update)
 * @version v0.1-alpha.2
 * @ingroup dasae-headers(dh)/bti
 * @prefix  bti_comp_attr
 *
 * @brief   Compiler-specific attributes and annotations
 * @details Provides compiler-specific attributes for function and variable declarations.
 *          Part of the BTI (Built-in) module for core system configuration.
 */

#ifndef BTI_COMP_ATTR_INCLUDED
#define BTI_COMP_ATTR_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "common.h"
#include "comp_cfg.h"

/*========== Macros and Declarations =========================================*/

/* Function Attributes */

#define bti_comp_attr_inline                          \
    /**                                               \
     * @brief Marks a function for potential inlining \
     * @return Compiler-specific inline keyword       \
     */                                               \
    comp_impl__bti_comp_attr_inline
#define bti_comp_attr_force_inline                       \
    /**                                                  \
     * @brief Forces function inlining                   \
     * @return Compiler-specific forced inline attribute \
     */                                                  \
    comp_impl__bti_comp_attr_force_inline
#define bti_comp_attr_no_inline                      \
    /**                                              \
     * @brief Prevents function inlining             \
     * @return Compiler-specific no-inline attribute \
     */                                              \
    comp_impl__bti_comp_attr_no_inline
#define bti_comp_attr_align(_align)                  \
    /**                                              \
     * @brief Sets memory alignment requirement      \
     * @param _align Alignment value in bytes        \
     * @return Compiler-specific alignment attribute \
     */                                              \
    comp_impl__bti_comp_attr_align(_align)
#define bti_comp_attr_deprecated                      \
    /**                                               \
     * @brief Marks a function as deprecated          \
     * @return Compiler-specific deprecated attribute \
     */                                               \
    comp_impl__bti_comp_attr_deprecated
#define bti_comp_attr_deprecated_msg(_msg)                \
    /**                                                   \
     * @brief Marks a function as deprecated with message \
     * @param _msg Deprecation reason message             \
     * @return Compiler-specific deprecated attribute     \
     */                                                   \
    comp_impl__bti_comp_attr_deprecated_msg(_msg)
#define bti_comp_attr_no_return                      \
    /**                                              \
     * @brief Indicates function never returns       \
     * @return Compiler-specific no-return attribute \
     */                                              \
    comp_impl__bti_comp_attr_no_return
#define bti_comp_attr_must_check                      \
    /**                                               \
     * @brief Enforces checking function return value \
     * @return Compiler-specific must-check attribute \
     */                                               \
    comp_impl__bti_comp_attr_must_check

/* Calling Conventions */

#define bti_comp_attr_cdecl                      \
    /**                                          \
     * @brief C calling convention               \
     * @return Compiler-specific cdecl attribute \
     */                                          \
    comp_impl__bti_comp_attr_cdecl
#define bti_comp_attr_stdcall                      \
    /**                                            \
     * @brief Standard calling convention          \
     * @return Compiler-specific stdcall attribute \
     */                                            \
    comp_impl__bti_comp_attr_stdcall
#define bti_comp_attr_fastcall                      \
    /**                                             \
     * @brief Fast calling convention               \
     * @return Compiler-specific fastcall attribute \
     */                                             \
    comp_impl__bti_comp_attr_fastcall
#define bti_comp_attr_vectorcall                      \
    /**                                               \
     * @brief Vector calling convention               \
     * @return Compiler-specific vectorcall attribute \
     */                                               \
    comp_impl__bti_comp_attr_vectorcall

/*========== Macros and Definitions =========================================*/

/* Function Attributes - comp_Implementations */

#if bti_comp_gcc || bti_comp_clang
#define comp_impl__bti_comp_attr_inline               inline
#define comp_impl__bti_comp_attr_force_inline         __attribute__((always_inline)) inline
#define comp_impl__bti_comp_attr_no_inline            __attribute__((noinline))
#define comp_impl__bti_comp_attr_align(_align)        __attribute__((aligned(_align)))
#define comp_impl__bti_comp_attr_deprecated           __attribute__((deprecated))
#define comp_impl__bti_comp_attr_deprecated_msg(_msg) __attribute__((deprecated(_msg)))
#define comp_impl__bti_comp_attr_no_return            __attribute__((noreturn))
#define comp_impl__bti_comp_attr_must_check           __attribute__((warn_unused_result))
#elif bti_comp_msvc
#define comp_impl__bti_comp_attr_inline               __inline
#define comp_impl__bti_comp_attr_force_inline         __forceinline
#define comp_impl__bti_comp_attr_no_inline            __declspec(noinline)
#define comp_impl__bti_comp_attr_align(_align)        __declspec(align(_align))
#define comp_impl__bti_comp_attr_deprecated           __declspec(deprecated)
#define comp_impl__bti_comp_attr_deprecated_msg(_msg) __declspec(deprecated(_msg))
#define comp_impl__bti_comp_attr_no_return            __declspec(noreturn)
#define comp_impl__bti_comp_attr_must_check           _Must_inspect_result_
#else
#define comp_impl__bti_comp_attr_inline
#define comp_impl__bti_comp_attr_force_inline
#define comp_impl__bti_comp_attr_no_inline
#define comp_impl__bti_comp_attr_align(_align)
#define comp_impl__bti_comp_attr_deprecated
#define comp_impl__bti_comp_attr_deprecated_msg(_msg)
#define comp_impl__bti_comp_attr_no_return
#define comp_impl__bti_comp_attr_must_check
#endif

/* Calling Conventions - comp_Implementations */

#if !bti_comp_msvc
#define comp_impl__bti_comp_attr_cdecl
#define comp_impl__bti_comp_attr_stdcall
#define comp_impl__bti_comp_attr_fastcall
#define comp_impl__bti_comp_attr_vectorcall
#else
#define comp_impl__bti_comp_attr_cdecl      __cdecl
#define comp_impl__bti_comp_attr_stdcall    __stdcall
#define comp_impl__bti_comp_attr_fastcall   __fastcall
#define comp_impl__bti_comp_attr_vectorcall __vectorcall
#endif

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* BTI_COMP_ATTR_INCLUDED */

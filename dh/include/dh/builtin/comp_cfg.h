/**
 * @copyright Copyright (c) 2024-2025 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    comp_cfg.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2024-11-22 (date of creation)
 * @updated 2025-11-27 (date of last update)
 * @version v0.1
 * @ingroup dasae-headers(dh)/foundation
 * @prefix  comp
 *
 * @brief   Compiler detection and feature configuration
 * @details Detects compiler (Clang, GCC, MSVC) and provides compiler-specific
 *          features, extensions, and attributes.
 *          Primary support: Clang, then GCC. MSVC planned for future.
 */
#ifndef foundation_comp_cfg__included
#define foundation_comp_cfg__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "pp.h"

/*========== Macros and Declarations ========================================*/

/* --- Compiler Type --- */

#define comp_type __comp_enum__comp_type
#define comp_type_unknown __comp_enum__comp_type_unknown
#define comp_type_clang __comp_enum__comp_type_clang
#define comp_type_gcc __comp_enum__comp_type_gcc
#define comp_type_msvc __comp_enum__comp_type_msvc

#define comp_name __comp_str__comp_name
#define comp_name_unknown __comp_str__comp_name_unknown
#define comp_name_clang __comp_str__comp_name_clang
#define comp_name_gcc __comp_str__comp_name_gcc
#define comp_name_msvc __comp_str__comp_name_msvc

/* --- Compiler Version --- */

#define comp_version __comp_int__comp_version
#define comp_version_major __comp_int__comp_version_major
#define comp_version_minor __comp_int__comp_version_minor
#define comp_version_patch __comp_int__comp_version_patch

/* --- Compiler Attributes --- */

#define comp_align(_align) __comp_attr__comp_align(_align)

#define comp_inline __comp_attr__comp_inline
#define comp_inline_always __comp_attr__comp_inline_always
#define comp_inline_never __comp_attr__comp_inline_never

#define comp_keep_symbol __comp_attr__comp_keep_symbol
#define comp_maybe_unused __comp_attr__comp_maybe_unused
#define comp_must_use __comp_attr__comp_must_use

#define comp_import __comp_attr__comp_import
#define comp_export __comp_attr__comp_export

#define comp_on_load __comp_attr__comp_on_load
#define comp_on_exit __comp_attr__comp_on_exit
#define comp_on_load_priority(_priority) __comp_attr__comp_on_load_priority(_priority)
#define comp_on_exit_priority(_priority) __comp_attr__comp_on_exit_priority(_priority)

#define comp_noreturn __comp_attr__comp_noreturn

#define comp_deprecated __comp_attr__comp_deprecated
#define comp_deprecated_msg(_msg) __comp_attr__comp_deprecated_msg(_msg)
#define comp_deprecated_instead(_msg, _replacement) __comp_attr__comp_deprecated_instead(_msg, _replacement)

/*========== Macros and Definitions =========================================*/

/* --- Compiler Type --- */

/* Default: unknown compiler */
#define __comp_enum__comp_type comp_type_unknown
#define __comp_enum__comp_type_unknown 0
#define __comp_enum__comp_type_clang 1
#define __comp_enum__comp_type_gcc 2
#define __comp_enum__comp_type_msvc 3

#if defined(__clang__)
#undef __comp_enum__comp_type
#define __comp_enum__comp_type comp_type_clang

#elif defined(__GNUC__)
#undef __comp_enum__comp_type
#define __comp_enum__comp_type comp_type_gcc

#elif defined(_MSC_VER)
#undef __comp_enum__comp_type
#define __comp_enum__comp_type comp_type_msvc

#else
#warning "Unknown compiler detected. Please check your compiler settings."
#endif

/* Derive name from type */
#define __comp_str__comp_name pp_switch_( \
    (comp_type)(pp_case_((comp_type_clang)(comp_name_clang)), \
                pp_case_((comp_type_gcc)(comp_name_gcc)), \
                pp_case_((comp_type_msvc)(comp_name_msvc)), \
                pp_default_(comp_name_unknown)) \
)
#define __comp_str__comp_name_unknown "Unknown"
#define __comp_str__comp_name_clang "Clang"
#define __comp_str__comp_name_gcc "GCC"
#define __comp_str__comp_name_msvc "MSVC"

/* --- Compiler Version --- */

/* Default: 0.0.0 */
#define __comp_int__comp_version 0
#define __comp_int__comp_version_major 0
#define __comp_int__comp_version_minor 0
#define __comp_int__comp_version_patch 0

#if comp_type == comp_type_clang
#undef __comp_int__comp_version
#define __comp_int__comp_version (__clang_major__ * 10000 + __clang_minor__ * 100 + __clang_patchlevel__)
#undef __comp_int__comp_version_major
#define __comp_int__comp_version_major __clang_major__
#undef __comp_int__comp_version_minor
#define __comp_int__comp_version_minor __clang_minor__
#undef __comp_int__comp_version_patch
#define __comp_int__comp_version_patch __clang_patchlevel__

#elif comp_type == comp_type_gcc
#undef __comp_int__comp_version
#define __comp_int__comp_version (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__)
#undef __comp_int__comp_version_major
#define __comp_int__comp_version_major __GNUC__
#undef __comp_int__comp_version_minor
#define __comp_int__comp_version_minor __GNUC_MINOR__
#undef __comp_int__comp_version_patch
#define __comp_int__comp_version_patch __GNUC_PATCHLEVEL__

#elif comp_type == comp_type_msvc
#undef __comp_int__comp_version
#define __comp_int__comp_version _MSC_VER
#undef __comp_int__comp_version_major
#define __comp_int__comp_version_major (_MSC_VER / 100)
#undef __comp_int__comp_version_minor
#define __comp_int__comp_version_minor (_MSC_VER % 100)
#endif

/* --- Compiler Attributes --- */

#if comp_type == comp_type_clang || comp_type == comp_type_gcc
#define __comp_attr__comp_align(_align) __attribute__((aligned(_align)))

#define __comp_attr__comp_inline inline
#define __comp_attr__comp_inline_always __attribute__((always_inline)) inline
#define __comp_attr__comp_inline_never __attribute__((noinline))

#define __comp_attr__comp_keep_symbol __attribute__((used))
#define __comp_attr__comp_maybe_unused __attribute__((unused))
#define __comp_attr__comp_must_use __attribute__((warn_unused_result))

/* Visibility */
#define __comp_attr__comp_import
#define __comp_attr__comp_export __attribute__((visibility("default")))

#define __comp_attr__comp_on_load __attribute__((constructor))
#define __comp_attr__comp_on_exit __attribute__((destructor))
/* Note: Priority works, but syntax is validated strictly by some parsers */
#define __comp_attr__comp_on_load_priority(_priority) __attribute__((constructor(_priority)))
#define __comp_attr__comp_on_exit_priority(_priority) __attribute__((destructor(_priority)))

#define __comp_attr__comp_noreturn __attribute__((noreturn))

#define __comp_attr__comp_deprecated __attribute__((deprecated))
#define __comp_attr__comp_deprecated_msg(_msg) __attribute__((deprecated(_msg)))
#define __comp_attr__comp_deprecated_instead(_msg, _replacement) __attribute__((deprecated(_msg ": Use " #_replacement " instead")))

#elif comp_type == comp_type_msvc
#define __comp_attr__comp_align(_align) __declspec(align(_align))

#define __comp_attr__comp_inline __inline
#define __comp_attr__comp_inline_always __forceinline
#define __comp_attr__comp_inline_never __declspec(noinline)

/* FIX: MSVC does not strictly support 'used' via declspec. Often ignored or creates warning C4230.
   Usually relies on linker pragmas, but strictly speaking declspec(noinline) is often enough to keep it. */
#define __comp_attr__comp_keep_symbol
#define __comp_attr__comp_maybe_unused __pragma(warning(suppress : 4100 4101 4189))
#define __comp_attr__comp_must_use _Check_return_ /* _Must_inspect_result_ maps to this */

/* DLL Import/Export */
#define __comp_attr__comp_import __declspec(dllimport)
#define __comp_attr__comp_export __declspec(dllexport)

/* FIX: MSVC does NOT support constructor/destructor attributes.
   Implementing this requires .CRT$XCU section hacking.
   For now, define as nothing to avoid compile errors. */
#define __comp_attr__comp_on_load
#define __comp_attr__comp_on_exit
#define __comp_attr__comp_on_load_priority(_priority)
#define __comp_attr__comp_on_exit_priority(_priority)

#define __comp_attr__comp_noreturn __declspec(noreturn)

#define __comp_attr__comp_deprecated __declspec(deprecated)
#define __comp_attr__comp_deprecated_msg(_msg) __declspec(deprecated(_msg))
#define __comp_attr__comp_deprecated_instead(_msg, _replacement) __declspec(deprecated(_msg " Use " #_replacement " instead"))

#else
#define __comp_attr__comp_align(_align)

#define __comp_attr__comp_inline
#define __comp_attr__comp_inline_always
#define __comp_attr__comp_inline_never

#define __comp_attr__comp_keep_symbol
#define __comp_attr__comp_maybe_unused
#define __comp_attr__comp_must_use

#define __comp_attr__comp_import
#define __comp_attr__comp_export

#define __comp_attr__comp_on_load
#define __comp_attr__comp_on_exit
#define __comp_attr__comp_on_load_priority(_priority)
#define __comp_attr__comp_on_exit_priority(_priority)

#define __comp_attr__comp_noreturn

#define __comp_attr__comp_deprecated
#define __comp_attr__comp_deprecated_msg(_msg)
#define __comp_attr__comp_deprecated_instead(_msg, _replacement)
#endif

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* foundation_comp_cfg__included */

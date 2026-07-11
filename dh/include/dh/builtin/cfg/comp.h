/**
 * @copyright Copyright (c) 2024-2026 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    comp.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2024-11-22 (date of creation)
 * @updated 2026-06-03 (date of last update)
 * @ingroup dal-project/da/foundation/cfg
 * @prefix  comp
 *
 * @brief   Compiler detection and feature configuration
 * @details Detects compiler (Clang, GCC, MSVC) and provides compiler-specific
 *          features, extensions, and attributes.
 *          Primary support: Clang, then GCC. MSVC planned for future.
 */
#pragma once
#ifndef foundation_cfg_comp__included
#define foundation_cfg_comp__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "../pp.h"

/*========== Macros and Declarations ========================================*/

/* --- Compiler Type --- */

#define comp_type __comp_enum__comp_type
#define comp_type_unknown __comp_enum__comp_type_unknown
#define comp_type_clang __comp_enum__comp_type_clang
#define comp_type_gcc __comp_enum__comp_type_gcc
#define comp_type_msvc __comp_enum__comp_type_msvc

#define comp_is_clang __comp_bool__comp_is_clang
#define comp_is_gcc __comp_bool__comp_is_gcc
#define comp_is_msvc __comp_bool__comp_is_msvc

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

/* --- Build Facts --- */

#define comp_env_type __comp_enum__comp_env_type
#define comp_env_type_unknown __comp_enum__comp_env_type_unknown
#define comp_env_type_hosted __comp_enum__comp_env_type_hosted
#define comp_env_type_freestanding __comp_enum__comp_env_type_freestanding

#define comp_env_is_hosted __comp_bool__comp_env_is_hosted
#define comp_env_is_freestanding __comp_bool__comp_env_is_freestanding

#define comp_start_files_linked __comp_bool__comp_start_files_linked
/* Derived from start-files unless explicitly provided by `COMP_HAS_CRT` / `COMP_NO_CRT`. */
#define comp_crt_linked __comp_bool__comp_crt_linked
#define comp_default_libs_linked __comp_bool__comp_default_libs_linked
#define comp_compiler_rt_linked __comp_bool__comp_compiler_rt_linked
#define comp_libc_linked __comp_bool__comp_libc_linked
/* Derived from start-files + default-libs unless explicitly provided by `COMP_HAS_STDLIB` / `COMP_NO_STDLIB`. */
#define comp_stdlib_linked __comp_bool__comp_stdlib_linked

/* --- Compiler Attributes --- */

/* Diagnostics & Constraints */
#define comp_deprecated __comp_attr__comp_deprecated
#define comp_deprecated_msg(_$msg) __comp_attr__comp_deprecated_msg(_$msg)
#define comp_deprecated_instead(_$msg, _$replacement) __comp_attr__comp_deprecated_instead(_$msg, _$replacement)

#define comp_must_use __comp_attr__comp_must_use
#define comp_maybe_unused __comp_attr__comp_maybe_unused
#define comp_undefined __comp_attr__comp_undefined
#define comp_undefined_static __comp_attr__comp_undefined_static

/* Data Layout */
#define comp_packed __comp_attr__comp_packed
#define comp_align(_$align) __comp_attr__comp_align(_$align)

/* Visibility & Linkage */
#define comp_export __comp_attr__comp_export
#define comp_import __comp_attr__comp_import
#define comp_keep_symbol __comp_attr__comp_keep_symbol

/* Semantics & Behavior */
#define comp_pure __comp_attr__comp_pure
#define comp_view __comp_attr__comp_view
#define comp_no_return __comp_attr__comp_no_return
#define comp_must_tail __comp_attr__comp_must_tail

/* Calling conventions & ABI boundaries */
#define comp_naked __comp_attr__comp_naked
#define comp_preserve_none __comp_attr__comp_preserve_none
#define comp_preserve_all __comp_attr__comp_preserve_all
#define comp_ms_abi __comp_attr__comp_ms_abi
#define comp_sysv_abi __comp_attr__comp_sysv_abi

/* Optimization Hints */
#define comp_inline __comp_attr__comp_inline
#define comp_inline_always __comp_attr__comp_inline_always
#define comp_inline_never __comp_attr__comp_inline_never
#define comp_flatten __comp_attr__comp_flatten

#define comp_branch_hot __comp_attr__comp_branch_hot
#define comp_branch_cold __comp_attr__comp_branch_cold

/* Lifecycle */
#define comp_on_load __comp_attr__comp_on_load
#define comp_on_exit __comp_attr__comp_on_exit
#define comp_on_load_priority(_$priority) __comp_attr__comp_on_load_priority(_$priority)
#define comp_on_exit_priority(_$priority) __comp_attr__comp_on_exit_priority(_$priority)

/* Control Flow */
#define comp_fallthrough __comp_attr__comp_fallthrough
#define comp_unreachable __comp_attr__comp_unreachable

/* Branch Prediction */
#define comp_branch_predict(_$expected /*: bool*/, _$expr... /*(bool)*/) __comp_attr__comp_branch_predict(_$expected, _$expr)
#define comp_branch_likely(_$expr... /*(bool)*/) __comp_attr__comp_branch_likely(_$expr)
#define comp_branch_unlikely(_$expr... /*(bool)*/) __comp_attr__comp_branch_unlikely(_$expr)
#define comp_branch_predict_at(_$prob /*: FltType*/, _$expr... /*(bool)*/) __comp_attr__comp_branch_predict_at(_$prob, _$expr)
#define comp_branch_unpredictable(_$expr... /*(bool)*/) __comp_attr__comp_branch_unpredictable(_$expr)

/* Prefetch */
#define comp_prefetch(_$addr, _$rw, _$locality...) __comp_attr__comp_prefetch(_$addr, _$rw, _$locality)

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

#define __comp_bool__comp_is_clang pp_Tok_eql(comp_type, comp_type_clang)
#define __comp_bool__comp_is_gcc pp_Tok_eql(comp_type, comp_type_gcc)
#define __comp_bool__comp_is_msvc pp_Tok_eql(comp_type, comp_type_msvc)

/* Derive name from type */
#define __comp_str__comp_name pp_expand( \
    pp_switch_ pp_begin(comp_type)( \
        pp_case_((comp_type_clang)(comp_name_clang)), \
        pp_case_((comp_type_gcc)(comp_name_gcc)), \
        pp_case_((comp_type_msvc)(comp_name_msvc)), \
        pp_default_(comp_name_unknown) \
    ) pp_end \
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

/* --- Build Facts ---
 *
 * Inputs from build tool (`dh-c` emits these; hand-written code may also define them):
 *   `COMP_FREESTANDING`    — freestanding env (mutually exclusive with `COMP_HOSTED`)
 *   `COMP_HOSTED`          — hosted env       (mutually exclusive with `COMP_FREESTANDING`)
 *   `COMP_HAS_START_FILES` / `COMP_NO_START_FILES`
 *   `COMP_HAS_CRT` / `COMP_NO_CRT`
 *   `COMP_HAS_DEFAULT_LIBS` / `COMP_NO_DEFAULT_LIBS`
 *   `COMP_HAS_COMPILER_RT` / `COMP_NO_COMPILER_RT`
 *   `COMP_HAS_LIBC` / `COMP_NO_LIBC`
 *   `COMP_HAS_STDLIB` / `COMP_NO_STDLIB`
 *
 * Compatibility aliases:
 *   `COMP_NO_STDLIB` implies `COMP_NO_START_FILES` + `COMP_NO_DEFAULT_LIBS`
 *   only when explicit `COMP_HAS_START_FILES` / `COMP_HAS_DEFAULT_LIBS` were not provided.
 *   `COMP_NO_CRT` implies `COMP_NO_START_FILES` only when explicit `COMP_HAS_START_FILES`
 *   was not provided.
 */

#if defined(COMP_HOSTED) && defined(COMP_FREESTANDING)
#error "`COMP_HOSTED` and `COMP_FREESTANDING` cannot both be defined"
#endif

/* Expand convenience aliases before any flag evaluation */
#if defined(COMP_NO_STDLIB)
#if !defined(COMP_HAS_DEFAULT_LIBS) && !defined(COMP_NO_DEFAULT_LIBS)
#define COMP_NO_DEFAULT_LIBS
#endif
#if !defined(COMP_HAS_START_FILES) && !defined(COMP_NO_START_FILES)
#define COMP_NO_START_FILES
#endif
#endif /* defined(COMP_NO_STDLIB) */

#if defined(COMP_NO_CRT)
#if !defined(COMP_HAS_START_FILES) && !defined(COMP_NO_START_FILES)
#define COMP_NO_START_FILES
#endif
#endif /* defined(COMP_NO_CRT) */

#if defined(COMP_HAS_START_FILES) && defined(COMP_NO_START_FILES)
#error "`COMP_HAS_START_FILES` and `COMP_NO_START_FILES` cannot both be defined"
#endif
#if defined(COMP_HAS_CRT) && defined(COMP_NO_CRT)
#error "`COMP_HAS_CRT` and `COMP_NO_CRT` cannot both be defined"
#endif
#if defined(COMP_HAS_DEFAULT_LIBS) && defined(COMP_NO_DEFAULT_LIBS)
#error "`COMP_HAS_DEFAULT_LIBS` and `COMP_NO_DEFAULT_LIBS` cannot both be defined"
#endif
#if defined(COMP_HAS_COMPILER_RT) && defined(COMP_NO_COMPILER_RT)
#error "`COMP_HAS_COMPILER_RT` and `COMP_NO_COMPILER_RT` cannot both be defined"
#endif
#if defined(COMP_HAS_LIBC) && defined(COMP_NO_LIBC)
#error "`COMP_HAS_LIBC` and `COMP_NO_LIBC` cannot both be defined"
#endif
#if defined(COMP_HAS_STDLIB) && defined(COMP_NO_STDLIB)
#error "`COMP_HAS_STDLIB` and `COMP_NO_STDLIB` cannot both be defined"
#endif

/* env_type: auto-detect, then allow `COMP_FREESTANDING`/`COMP_HOSTED` to override */
#define __comp_enum__comp_env_type_unknown 0
#define __comp_enum__comp_env_type_hosted 1
#define __comp_enum__comp_env_type_freestanding 2

#if defined(__STDC_HOSTED__) && (__STDC_HOSTED__ + 0) == 0
#define __comp_enum__comp_env_type comp_env_type_freestanding
#else
#define __comp_enum__comp_env_type comp_env_type_hosted
#endif

#if defined(COMP_FREESTANDING)
#undef __comp_enum__comp_env_type
#define __comp_enum__comp_env_type comp_env_type_freestanding
#elif defined(COMP_HOSTED)
#undef __comp_enum__comp_env_type
#define __comp_enum__comp_env_type comp_env_type_hosted
#endif

/* start_files_linked: explicit fact wins; default = 1. */
#if defined(COMP_HAS_START_FILES)
#define __comp_flag__comp_start_files_linked 1
#elif defined(COMP_NO_START_FILES)
#define __comp_flag__comp_start_files_linked 0
#else
#define __comp_flag__comp_start_files_linked 1
#endif

/* crt_linked: explicit fact wins; otherwise follows start files. */
#if defined(COMP_HAS_CRT)
#define __comp_flag__comp_crt_linked 1
#elif defined(COMP_NO_CRT)
#define __comp_flag__comp_crt_linked 0
#else
#define __comp_flag__comp_crt_linked comp_start_files_linked
#endif

/* default_libs_linked: explicit fact wins; default = 1. */
#if defined(COMP_HAS_DEFAULT_LIBS)
#define __comp_flag__comp_default_libs_linked 1
#elif defined(COMP_NO_DEFAULT_LIBS)
#define __comp_flag__comp_default_libs_linked 0
#else
#define __comp_flag__comp_default_libs_linked 1
#endif

/* compiler_rt_linked: explicit fact wins; otherwise compiler default libs imply it. */
#if defined(COMP_HAS_COMPILER_RT)
#define __comp_flag__comp_compiler_rt_linked 1
#elif defined(COMP_NO_COMPILER_RT) || defined(COMP_NO_DEFAULT_LIBS)
#define __comp_flag__comp_compiler_rt_linked 0
#else
#define __comp_flag__comp_compiler_rt_linked 1
#endif

/* libc_linked: explicit fact wins; otherwise 0 when freestanding or default libs are absent. */
#if defined(COMP_HAS_LIBC)
#define __comp_flag__comp_libc_linked 1
#elif defined(COMP_NO_LIBC) || defined(COMP_NO_DEFAULT_LIBS) || (comp_env_type == comp_env_type_freestanding)
#define __comp_flag__comp_libc_linked 0
#else
#define __comp_flag__comp_libc_linked 1
#endif

/* stdlib_linked: explicit fact wins; otherwise follows start files + default libs. */
#if defined(COMP_HAS_STDLIB)
#define __comp_flag__comp_stdlib_linked 1
#elif defined(COMP_NO_STDLIB)
#define __comp_flag__comp_stdlib_linked 0
#else
#define __comp_flag__comp_stdlib_linked (comp_start_files_linked && comp_default_libs_linked)
#endif

#define __comp_bool__comp_env_is_hosted pp_Tok_eql(comp_env_type, comp_env_type_hosted)
#define __comp_bool__comp_env_is_freestanding pp_Tok_eql(comp_env_type, comp_env_type_freestanding)

#define __comp_bool__comp_start_files_linked __comp_flag__comp_start_files_linked
#define __comp_bool__comp_crt_linked __comp_flag__comp_crt_linked
#define __comp_bool__comp_default_libs_linked __comp_flag__comp_default_libs_linked
#define __comp_bool__comp_compiler_rt_linked __comp_flag__comp_compiler_rt_linked
#define __comp_bool__comp_libc_linked __comp_flag__comp_libc_linked
#define __comp_bool__comp_stdlib_linked __comp_flag__comp_stdlib_linked

/* --- Compiler Attributes --- */

#if comp_type == comp_type_clang || comp_type == comp_type_gcc
#define __comp_attr__comp_deprecated __attribute__((deprecated))
#define __comp_attr__comp_deprecated_msg(_$msg) __attribute__((deprecated(_$msg)))
#define __comp_attr__comp_deprecated_instead(_$msg, _$replacement) __attribute__((deprecated(_$msg ": Use `" #_$replacement "` instead")))

#define __comp_attr__comp_must_use __attribute__((warn_unused_result))
#define __comp_attr__comp_maybe_unused __attribute__((unused))
#define __comp_attr__comp_undefined __attribute__((uninitialized))
#define __comp_attr__comp_undefined_static __attribute__((loader_uninitialized))

#define __comp_attr__comp_packed __attribute__((packed))
#define __comp_attr__comp_align(_$align) __attribute__((aligned(_$align)))

#define __comp_attr__comp_export __attribute__((visibility("default")))
#define __comp_attr__comp_import
#define __comp_attr__comp_keep_symbol __attribute__((used))

#define __comp_attr__comp_pure __attribute__((const))
#define __comp_attr__comp_view __attribute__((pure))
#define __comp_attr__comp_no_return __attribute__((noreturn))
#define __comp_attr__comp_must_tail __attribute__((musttail))

#define __comp_attr__comp_inline inline
#define __comp_attr__comp_inline_always __attribute__((always_inline)) inline
#define __comp_attr__comp_inline_never __attribute__((noinline))
#define __comp_attr__comp_flatten __attribute__((flatten))

#define __comp_attr__comp_branch_hot __attribute__((hot))
#define __comp_attr__comp_branch_cold __attribute__((cold))

#define __comp_attr__comp_on_load __attribute__((constructor))
#define __comp_attr__comp_on_exit __attribute__((destructor))
/* Note: Priority works, but syntax is validated strictly by some parsers */
#define __comp_attr__comp_on_load_priority(_$priority) __attribute__((constructor(_$priority)))
#define __comp_attr__comp_on_exit_priority(_$priority) __attribute__((destructor(_$priority)))

#define __comp_attr__comp_fallthrough __attribute__((fallthrough))
#define __comp_attr__comp_unreachable __builtin_unreachable()

#define __comp_attr__comp_branch_predict(_$expected, _$expr...) __builtin_expect(!!(_$expr), !!(_$expected))
#define __comp_attr__comp_branch_likely(_$expr...) __builtin_expect(!!(_$expr), 1)
#define __comp_attr__comp_branch_unlikely(_$expr...) __builtin_expect(!!(_$expr), 0)
#define __comp_attr__comp_branch_predict_at(_$prob, _$expr...) __builtin_expect_with_probability(!!(_$expr), 1, _$prob)
#if comp_type == comp_type_clang
#define __comp_attr__comp_branch_unpredictable(_$expr...) __builtin_unpredictable(!!(_$expr))
#else
#define __comp_attr__comp_branch_unpredictable(_$expr...) (!!(_$expr))
#endif

#define __comp_attr__comp_prefetch(_$addr, _$rw, _$locality...) __builtin_prefetch(_$addr, _$rw, _$locality)

#define __comp_attr__comp_naked __attribute__((naked))
#define __comp_attr__comp_preserve_none __attribute__((preserve_none))
#define __comp_attr__comp_preserve_all __attribute__((preserve_all))
#define __comp_attr__comp_ms_abi __attribute__((ms_abi))
#define __comp_attr__comp_sysv_abi __attribute__((sysv_abi))

#elif comp_type == comp_type_msvc
#define __comp_attr__comp_deprecated __declspec(deprecated)
#define __comp_attr__comp_deprecated_msg(_$msg) __declspec(deprecated(_$msg))
#define __comp_attr__comp_deprecated_instead(_$msg, _$replacement) __declspec(deprecated(_$msg ": Use `" #_$replacement "` instead"))

#define __comp_attr__comp_must_use _Check_return_ /* _Must_inspect_result_ maps to this */
#define __comp_attr__comp_maybe_unused __pragma(warning(suppress : 4100 4101 4189))
#define __comp_attr__comp_undefined
#define __comp_attr__comp_undefined_static

#define __comp_attr__comp_packed /* TODO: Implement MSVC packed attribute with struct scope */
#define __comp_attr__comp_align(_$align) __declspec(align(_$align))

/* DLL Import/Export */
#define __comp_attr__comp_export __declspec(dllexport)
#define __comp_attr__comp_import __declspec(dllimport)
/* FIX: MSVC does not strictly support 'used' via declspec. Often ignored or creates warning C4230.
   Usually relies on linker pragmas, but strictly speaking declspec(noinline) is often enough to keep it. */
#define __comp_attr__comp_keep_symbol

#define __comp_attr__comp_pure __declspec(const)
#define __comp_attr__comp_view __declspec(pure)
#define __comp_attr__comp_no_return __declspec(noreturn)
#define __comp_attr__comp_must_tail __declspec(musttail)

#define __comp_attr__comp_inline __inline
#define __comp_attr__comp_inline_always __forceinline
#define __comp_attr__comp_inline_never __declspec(noinline)
#define __comp_attr__comp_flatten

#define __comp_attr__comp_branch_hot
#define __comp_attr__comp_branch_cold

/* FIX: MSVC does NOT support constructor/destructor attributes.
   Implementing this requires .CRT$XCU section hacking.
   For now, define as nothing to avoid compile errors. */
#define __comp_attr__comp_on_load
#define __comp_attr__comp_on_exit
#define __comp_attr__comp_on_load_priority(_$priority)
#define __comp_attr__comp_on_exit_priority(_$priority)

#define __comp_attr__comp_unreachable __assume(0)
#define __comp_attr__comp_fallthrough

#define __comp_attr__comp_branch_predict(_$expected, _$expr...) __builtin_expect(!!(_$expr), !!(_$expected))
#define __comp_attr__comp_branch_likely(_$expr...) __builtin_expect(!!(_$expr), 1)
#define __comp_attr__comp_branch_unlikely(_$expr...) __builtin_expect(!!(_$expr), 0)
#define __comp_attr__comp_branch_predict_at(_$prob, _$expr...) __builtin_expect_with_probability(!!(_$expr), 1, _$prob)
#define __comp_attr__comp_branch_unpredictable(_$expr...) (!!(_$expr))

#define __comp_attr__comp_prefetch(_$addr, _$rw, _$locality...) __prefetch(_$addr, _$rw, _$locality)

#define __comp_attr__comp_naked __declspec(naked)
#define __comp_attr__comp_preserve_none __preserve_none
#define __comp_attr__comp_preserve_all
#define __comp_attr__comp_ms_abi
#define __comp_attr__comp_sysv_abi

#else
#define __comp_attr__comp_deprecated
#define __comp_attr__comp_deprecated_msg(_$msg)
#define __comp_attr__comp_deprecated_instead(_$msg, _$replacement)

#define __comp_attr__comp_must_use
#define __comp_attr__comp_maybe_unused
#define __comp_attr__comp_undefined
#define __comp_attr__comp_undefined_static

#define __comp_attr__comp_packed
#define __comp_attr__comp_align(_$align)

#define __comp_attr__comp_export
#define __comp_attr__comp_import
#define __comp_attr__comp_keep_symbol

#define __comp_attr__comp_pure
#define __comp_attr__comp_view
#define __comp_attr__comp_no_return
#define __comp_attr__comp_must_tail

#define __comp_attr__comp_inline
#define __comp_attr__comp_inline_always
#define __comp_attr__comp_inline_never
#define __comp_attr__comp_flatten

#define __comp_attr__comp_branch_hot
#define __comp_attr__comp_branch_cold

#define __comp_attr__comp_on_load
#define __comp_attr__comp_on_exit
#define __comp_attr__comp_on_load_priority(_$priority)
#define __comp_attr__comp_on_exit_priority(_$priority)

#define __comp_attr__comp_fallthrough
#define __comp_attr__comp_unreachable __assume(0)

#define __comp_attr__comp_branch_predict(_$expected, _$expr...) __builtin_expect(!!(_$expr), !!(_$expected))
#define __comp_attr__comp_branch_likely(_$expr...) __builtin_expect(!!(_$expr), 1)
#define __comp_attr__comp_branch_unlikely(_$expr...) __builtin_expect(!!(_$expr), 0)
#define __comp_attr__comp_branch_predict_at(_$prob, _$expr...) __builtin_expect_with_probability(!!(_$expr), 1, _$prob)
#define __comp_attr__comp_branch_unpredictable(_$expr...) (!!(_$expr))

#define __comp_attr__comp_prefetch(_$addr, _$rw, _$locality...) __prefetch(_$addr, _$rw, _$locality)

#define __comp_attr__comp_naked
#define __comp_attr__comp_preserve_none
#define __comp_attr__comp_preserve_all
#define __comp_attr__comp_ms_abi
#define __comp_attr__comp_sysv_abi
#endif

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* foundation_cfg_comp__included */

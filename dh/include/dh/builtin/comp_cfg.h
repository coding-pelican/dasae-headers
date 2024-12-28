/**
 * @copyright Copyright 2024. Gyeongtae Kim All rights reserved.
 *
 * @file    comp_cfg.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2024-11-22 (date of creation)
 * @updated 2024-11-22 (date of last update)
 * @version v1.0.0
 * @ingroup dasae-headers(dh)/builtin
 * @prefix  BUILTIN_COMP
 *
 * @brief   Compiler detection configuration
 * @details Detects compiler type (Clang, GCC, MSVC) and provides version information.
 *          Includes compiler-specific calling conventions and optimization hints.
 */

#ifndef BUILTIN_COMP_CFG_INCLUDED
#define BUILTIN_COMP_CFG_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Compiler Type Detection ========================================*/
#if defined(__clang__)
#define BUILTIN_COMP_CLANG (1)
#define BUILTIN_COMP_GCC   (0)
#define BUILTIN_COMP_MSVC  (0)
#define BUILTIN_COMP_NAME  "Clang"
#elif defined(__GNUC__)
#define BUILTIN_COMP_CLANG (0)
#define BUILTIN_COMP_GCC   (1)
#define BUILTIN_COMP_MSVC  (0)
#define BUILTIN_COMP_NAME  "GCC"
#elif defined(_MSC_VER)
#define BUILTIN_COMP_CLANG (0)
#define BUILTIN_COMP_GCC   (0)
#define BUILTIN_COMP_MSVC  (1)
#define BUILTIN_COMP_NAME  "MSVC"
#else
#define BUILTIN_COMP_CLANG (0)
#define BUILTIN_COMP_GCC   (0)
#define BUILTIN_COMP_MSVC  (0)
#define BUILTIN_COMP_NAME  "Unknown"
#warning "Unknown compiler detected. Please check your compiler settings."
#endif

/*========== Compiler Version Information ===================================*/
#if BUILTIN_COMP_MSVC
#define BUILTIN_COMP_VERSION       _MSC_VER
#define BUILTIN_COMP_VERSION_MAJOR (_MSC_VER / 100)
#define BUILTIN_COMP_VERSION_MINOR (_MSC_VER % 100)
#elif BUILTIN_COMP_GCC
#define BUILTIN_COMP_VERSION       (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__)
#define BUILTIN_COMP_VERSION_MAJOR __GNUC__
#define BUILTIN_COMP_VERSION_MINOR __GNUC_MINOR__
#define BUILTIN_COMP_VERSION_PATCH __GNUC_PATCHLEVEL__
#elif BUILTIN_COMP_CLANG
#define BUILTIN_COMP_VERSION       (__clang_major__ * 10000 + __clang_minor__ * 100 + __clang_patchlevel__)
#define BUILTIN_COMP_VERSION_MAJOR __clang_major__
#define BUILTIN_COMP_VERSION_MINOR __clang_minor__
#define BUILTIN_COMP_VERSION_PATCH __clang_patchlevel__
#else
#define BUILTIN_COMP_VERSION       (0)
#define BUILTIN_COMP_VERSION_MAJOR (0)
#define BUILTIN_COMP_VERSION_MINOR (0)
#define BUILTIN_COMP_VERSION_PATCH (0)
#endif

/*========== Compiler-Specific Attributes ===================================*/
#if BUILTIN_COMP_MSVC
#define BUILTIN_COMP_INLINE       __inline
#define BUILTIN_COMP_FORCE_INLINE __forceinline
#define BUILTIN_COMP_NOINLINE     __declspec(noinline)
#define BUILTIN_COMP_ALIGN(x)     __declspec(align(x))
#define BUILTIN_COMP_DEPRECATED   __declspec(deprecated)
#define BUILTIN_COMP_NORETURN     __declspec(noreturn)
#define BUILTIN_COMP_MUST_CHECK   _Must_inspect_result_
#elif BUILTIN_COMP_GCC || BUILTIN_COMP_CLANG
#define BUILTIN_COMP_INLINE       inline
#define BUILTIN_COMP_FORCE_INLINE __attribute__((always_inline)) inline
#define BUILTIN_COMP_NOINLINE     __attribute__((noinline))
#define BUILTIN_COMP_ALIGN(x)     __attribute__((aligned(x)))
#define BUILTIN_COMP_DEPRECATED   __attribute__((deprecated))
#define BUILTIN_COMP_NORETURN     __attribute__((noreturn))
#define BUILTIN_COMP_MUST_CHECK   __attribute__((warn_unused_result))
#else
#define BUILTIN_COMP_INLINE
#define BUILTIN_COMP_FORCE_INLINE
#define BUILTIN_COMP_NOINLINE
#define BUILTIN_COMP_ALIGN(x)
#define BUILTIN_COMP_DEPRECATED
#define BUILTIN_COMP_NORETURN
#define BUILTIN_COMP_MUST_CHECK
#endif

/*========== Calling Conventions ============================================*/
#if BUILTIN_COMP_MSVC
#define BUILTIN_COMP_CDECL      __cdecl
#define BUILTIN_COMP_STDCALL    __stdcall
#define BUILTIN_COMP_FASTCALL   __fastcall
#define BUILTIN_COMP_VECTORCALL __vectorcall
#else
#define BUILTIN_COMP_CDECL
#define BUILTIN_COMP_STDCALL
#define BUILTIN_COMP_FASTCALL
#define BUILTIN_COMP_VECTORCALL
#endif

#if defined(__cplusplus)
} /* extern "C" */
#endif
#endif /* BUILTIN_COMP_CFG_INCLUDED */

/**
 * @copyright Copyright (c) 2024 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    null.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2024-11-02 (date of creation)
 * @updated 2024-11-02 (date of last update)
 * @version v0.1-alpha
 * @ingroup dasae-headers(dh)/core/prim
 * @prefix  (none)
 *
 * @brief   Null utilities
 * @details Provides null utilities and functions for:
 *          - Null definition
 *          - Null pointer definition
 *          - Nullable-Nonnull attributes
 *          - Nullable-Nonnull macros
 */
#ifndef CORE_PRIM_NULL_INCLUDED
#define CORE_PRIM_NULL_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "cfg.h"

/*========== Macros and Definitions =========================================*/

/* Null definition */
#ifndef NULL
#ifdef __cplusplus
#ifndef _WIN64 /* _WIN32 */
#define NULL (0)
#else /* _WIN64 */
#define NULL (0ll)
#endif
#else
#define NULL ((void*)0)
#endif
#endif /* NULL */

/* Null pointer definition */
#if defined(__cplusplus)
#if (__cplusplus >= 201103L)
/* C++11 or later - nullptr is available */
#define null nullptr
#else
/* Pre-C++11 */
#define nullptr NULL
#define null NULL
#endif
#else
/* C */
#define nullptr NULL
#define null NULL
#endif

/* Nullable-Nonnull attributes */
#define nullable comp_attr__nullable
#define nonnull comp_attr__nonnull

#define Nullable(TPtr) comp_type__Nullable(TPtr)
#define Nonnull(TPtr) comp_type__Nonnull(TPtr)

/*========== Macros Implementation ==========================================*/

#if defined(__clang__)
#define comp_attr__nullable _Nullable
#define comp_attr__nonnull _Nonnull
#elif defined(__GNUC__)
#define comp_attr__nullable __attribute__((nullable))
#define comp_attr__nonnull __attribute__((nonnull))
#else
#define comp_attr__nullable /* nullable */
#define comp_attr__nonnull  /* nonnull */
#endif

#define comp_type__Nullable(TPtr) TPtr nullable
#define comp_type__Nonnull(TPtr) TPtr nonnull

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* CORE_PRIM_NULL_INCLUDED */

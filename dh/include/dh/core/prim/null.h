/**
 * @copyright Copyright 2024. Gyeongtae Kim All rights reserved.
 *
 * @file    null.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2024-11-02 (date of creation)
 * @updated 2024-11-02 (date of last update)
 * @version v1.0.0
 * @ingroup dasae-headers(dh)/core/prim
 * @prefix  NONE
 *
 * @brief   Header of some software
 * @details Some detailed explanation
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
#define NULL ((anyptr)0)

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
#define null    nullptr
#endif
#else
/* C */
#define nullptr NULL
#define null    nullptr
#endif

/* Nullable-Nonnull attributes */
#define nullable IMPL_nullable
#define nonnull  IMPL_nonnull

#define Nullable(TPtr) IMPL_Nullable(TPtr)
#define Nonnull(TPtr)  IMPL_Nonnull(TPtr)

/*========== Macros Implementation ==========================================*/

#if defined(__clang__)
#define IMPL_nullable _Nullable
#define IMPL_nonnull  _Nonnull
#elif defined(__GNUC__)
#define IMPL_nullable __attribute__((nullable))
#define IMPL_nonnull  __attribute__((nonnull))
#else
#define IMPL_nullable
#define IMPL_nonnull
#endif

#define IMPL_Nullable(TPtr) TPtr nullable
#define IMPL_Nonnull(TPtr)  TPtr nonnull

#if defined(__cplusplus)
} /* extern "C" */
#endif /* __cplusplus */
#endif /* CORE_PRIM_NULL_INCLUDED */

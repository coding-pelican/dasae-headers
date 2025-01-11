/**
 * @copyright Copyright 2024. Gyeongtae Kim All rights reserved.
 *
 * @file    null.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2024-11-02 (date of creation)
 * @updated 2024-11-02 (date of last update)
 * @version v0.1-alpha
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
#define null    nullptr
#endif
#else
/* C */
#define nullptr NULL
#define null    nullptr
#endif

/* Nullable-Nonnull attributes */
#define nullable ATTR_nullable
#define nonnull  ATTR_nonnull

#define Nullable(TPtr) TYPE_Nullable(TPtr)
#define Nonnull(TPtr)  TYPE_Nonnull(TPtr)

/*========== Macros Implementation ==========================================*/

#if defined(__clang__)
#define ATTR_nullable _Nullable
#define ATTR_nonnull  _Nonnull
#elif defined(__GNUC__)
#define ATTR_nullable __attribute__((nullable))
#define ATTR_nonnull  __attribute__((nonnull))
#else
#define ATTR_nullable /* nullable */
#define ATTR_nonnull  /* nonnull */
#endif

#define TYPE_Nullable(TPtr) TPtr nullable
#define TYPE_Nonnull(TPtr)  TPtr nonnull

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* CORE_PRIM_NULL_INCLUDED */

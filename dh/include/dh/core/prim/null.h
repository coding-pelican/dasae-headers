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


#ifndef PRIM_NULL_INCLUDED
#define PRIM_NULL_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */


/*========== Includes =======================================================*/

#include "cfg.h"

#include "ptr.h"

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
#define nullable RETURN_nullable
#define nonnull  RETURN_nonnull

#define Nullable(TYPE) RETURN_Nullable(TYPE)
#define Nonnull(TYPE)  RETURN_Nonnull(TYPE)

/*========== Macros Implementation ==========================================*/

#if defined(__clang__)
#define RETURN_nullable _Nullable
#define RETURN_nonnull  _Nonnull
#elif defined(__GNUC__)
#define RETURN_nullable __attribute__((nullable))
#define RETURN_nonnull  __attribute__((nonnull))
#else
#define RETURN_nullable
#define RETURN_nonnull
#endif

#define RETURN_Nullable(TYPE) TYPE nullable
#define RETURN_Nonnull(TYPE)  TYPE nonnull


#if defined(__cplusplus)
} /* extern "C" */
#endif /* __cplusplus */
#endif /* PRIM_NULL_INCLUDED */

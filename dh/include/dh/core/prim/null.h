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
#define nullable IMPL_nullable
#define nonnull  IMPL_nonnull

#define Nullable(_T) IMPL_Nullable(_T)
#define Nonnull(_T)  IMPL_Nonnull(_T)

#define ensureNonNull(_var, ...) IMPL_ensureNonNull(_var, __VA_ARGS__)

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

#define IMPL_Nullable(_T) _T nullable
#define IMPL_Nonnull(_T)  _T nonnull

#define IMPL_ensureNonNull(_var, ...) ((_var) ? (_var) : (__VA_ARGS__))


#if defined(__cplusplus)
} /* extern "C" */
#endif /* __cplusplus */
#endif /* CORE_PRIM_NULL_INCLUDED */

#ifndef COMPAT_NULL_INCLUDED
#define COMPAT_NULL_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */


#include "compat_cfg_arch.h"


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
#ifdef OVERRIDE_CPP_NULL
#undef NULL
#undef nullptr
#endif
#endif /* __cplusplus */

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


#define ensure(_x, ...) ((_x) ? (_x) : __VA_ARGS__)


#if defined(COMPAT_ARCH_64BIT) && COMPAT_ARCH_64BIT
/* 64-bit */
_Static_assert(sizeof(void*) == 8, "pointer must be 8 bytes on 64-bit");
#elif defined(COMPAT_ARCH_32BIT) && COMPAT_ARCH_32BIT
/* 32-bit */
_Static_assert(sizeof(void*) == 4, "pointer must be 4 bytes on 32-bit");
#else
// #  error "Unknown architecture"
#endif


#if defined(__cplusplus)
} /* extern "C" */
#endif /* __cplusplus */
#endif /* COMPAT_NULL_INCLUDED */

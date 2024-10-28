/**
 * @copyright Copyright 2024. Gyeongtae Kim All rights reserved.
 *
 * @file    compat_bool.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2024-10-27 (date of creation)
 * @updated 2024-10-27 (date of last update)
 * @version v1.0.0
 * @ingroup dasae-headers(dh)/compat
 * @prefix  NONE
 *
 * @brief   Header of some software
 * @details Some detailed explanation
 */


#ifndef COMPAT_BOOL_INCLUDED
#define COMPAT_BOOL_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */


/* Boolean type definition */
#if defined(__cplusplus)
#ifdef OVERRIDE_CPP_BOOL
#undef bool
#undef true
#undef false
#endif
#endif /* defined(__cplusplus) */

#if !defined(__cplusplus)
/* C99 or later - use _Bool */
#if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L)
#include <stdbool.h>
#else
/* Pre-C99 - define bool manually */
#define bool  _Bool
#define true  (1)
#define false (0)
typedef unsigned char _Bool;
#endif
#endif /* !defined(__cplusplus) */


// Ensure bool is always exactly 1 byte
typedef char ensure_bool_size[(sizeof(bool) == 1) ? 1 : -1];
_Static_assert(sizeof(bool) == 1, "bool must be 1 byte");


#if defined(__cplusplus)
} /* extern "C" */
#endif /* __cplusplus */
#endif /* COMPAT_BOOL_INCLUDED */

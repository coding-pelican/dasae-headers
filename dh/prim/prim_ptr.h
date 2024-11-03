/**
 * @copyright Copyright 2024. Gyeongtae Kim All rights reserved.
 *
 * @file    prim_ptr.h
 * @author  Gyeongtae Kim(dev-dasae) <codingpelican@gmail.com>
 * @date    2024-11-02 (date of creation)
 * @updated 2024-11-02 (date of last update)
 * @version v1.0.0
 * @ingroup dasae-headers(dh)/prim
 * @prefix  NONE
 *
 * @brief   Header of some software
 * @details Some detailed explanation
 */


#ifndef PRIM_PTR_INCLUDED
#define PRIM_PTR_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */


/*========== Includes =======================================================*/

#include "../compat/compat_cfg_arch.h"

#include "../static/static_assert.h"

/*========== Macros and Definitions =========================================*/

/* Type pointer*/
#define Ptr(_T)                 RETURN_Ptr(_T)
/* Any type pointer */
#define anyptr                  RETURN_anytype
/* Convert anyptr to pointer of type _T */
#define ptrCast(_T, _ptr)       RETURN_ptrCast(_T, _ptr)
/* Access the value pointed to by a pointer */
#define ptrAccess(_ptr)         RETURN_ptrAccess(_ptr)
/* Access the value pointed to by a pointer of type _T */
#define ptrCastAccess(_T, _ptr) RETURN_ptrCastAccess(_T, _ptr)

/*========== Macros Implementation ==========================================*/

#define RETURN_Ptr(_T)                 _T*
#define RETURN_anytype                 void*
#define RETURN_ptrCast(_T, _ptr)       ((Ptr(_T))(_ptr))
#define RETURN_ptrAccess(_ptr)         (*(_ptr))
#define RETURN_ptrCastAccess(_T, _ptr) (*(Ptr(_T))(_ptr))


#if defined(COMPAT_ARCH_64BIT) && COMPAT_ARCH_64BIT
/* 64-bit */
static_assert(sizeof(anyptr) == 8, "pointer must be 8 bytes on 64-bit");
#elif defined(COMPAT_ARCH_32BIT) && COMPAT_ARCH_32BIT
/* 32-bit */
static_assert(sizeof(anyptr) == 4, "pointer must be 4 bytes on 32-bit");
#else
// #  error "Unknown architecture"
#endif


#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* PRIM_PTR_INCLUDED */

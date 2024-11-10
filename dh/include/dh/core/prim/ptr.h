/**
 * @copyright Copyright 2024. Gyeongtae Kim All rights reserved.
 *
 * @file    ptr.h
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


#ifndef PRIM_PTR_INCLUDED
#define PRIM_PTR_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */


/*========== Includes =======================================================*/

#include "cfg.h"

/*========== Macros and Definitions =========================================*/

/* Type pointer*/
#define Ptr(TYPE)                RETURN_Ptr(TYPE)
/* Any type pointer */
#define anyptr                   RETURN_anytype
/* Convert anyptr to pointer of type TYPE */
#define ptrCast(TYPE, PTR)       RETURN_ptrCast(TYPE, PTR)
/* Access the value pointed to by a pointer */
#define ptrAccess(PTR)           RETURN_ptrAccess(PTR)
/* Access the value pointed to by a pointer of type TYPE */
#define ptrCastAccess(TYPE, PTR) RETURN_ptrCastAccess(TYPE, PTR)
#define addr(VAR)                RETURN_addr(PTR)

/*========== Macros Implementation ==========================================*/

#define RETURN_Ptr(TYPE)                TYPE*
#define RETURN_anytype                  void*
#define RETURN_ptrCast(TYPE, PTR)       (Ptr(TYPE))(PTR)
#define RETURN_ptrAccess(PTR)           (*(PTR))
#define RETURN_ptrCastAccess(TYPE, PTR) (*(Ptr(TYPE))(PTR))
#define RETURN_addr(VAR)                &VAR


#if defined(ARCH_64BIT) && ARCH_64BIT
/* 64-bit */
claim_assertStatic(sizeof(anyptr) == 8, "pointer must be 8 bytes on 64-bit");
#elif defined(ARCH_32BIT) && ARCH_32BIT
/* 32-bit */
claim_assertStatic(sizeof(anyptr) == 4, "pointer must be 4 bytes on 32-bit");
#else
// #  error "Unknown architecture"
#endif


#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* PRIM_PTR_INCLUDED */

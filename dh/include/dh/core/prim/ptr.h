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


#ifndef CORE_PRIM_PTR_INCLUDED
#define CORE_PRIM_PTR_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */


/*========== Includes =======================================================*/

#include "cfg.h"

/*========== Macros and Definitions =========================================*/

// TODO: Separate Ptr to Ptr and PtrMut
/* Type pointer*/
#define Ptr(TYPE)                IMPL_Ptr(TYPE)
/* Any type pointer */
#define anyptr                   IMPL_anytype
/* Convert anyptr to pointer of type TYPE */
#define castPtr(TYPE, PTR)       IMPL_castPtr(TYPE, PTR)
/* Access the value pointed to by a pointer */
#define accessPtr(PTR)           IMPL_accessPtr(PTR)
/* Access the value pointed to by a pointer of type TYPE */
#define accessCastPtr(TYPE, PTR) IMPL_accessCastPtr(TYPE, PTR)

/* Normal type */
#define Val(TYPE) IMPL_Val(TYPE)
/* Address of a variable */
#define addr(VAR) IMPL_addr(VAR)

/*========== Macros Implementation ==========================================*/

#define IMPL_Ptr(TYPE)                TYPE*
#define IMPL_anytype                  void*
#define IMPL_castPtr(TYPE, PTR)       (TYPE*)(PTR)
#define IMPL_accessPtr(PTR)           (*(PTR))
#define IMPL_accessCastPtr(TYPE, PTR) (*((TYPE*)(PTR)))

#define IMPL_Val(TYPE)    TYPE
#define IMPL_addr(VAR...) (&(VAR))


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
#endif /* CORE_PRIM_PTR_INCLUDED */

/**
 * @copyright Copyright 2024. Gyeongtae Kim All rights reserved.
 *
 * @file    ref.h
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


#ifndef CORE_PRIM_REF_INCLUDED
#define CORE_PRIM_REF_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */


/*========== Includes =======================================================*/

#include "cfg.h"
#include "null.h"
#include "ptr.h"

/*========== Macros and Definitions =========================================*/

/* Reference definition */
#define Ref(TYPE)  IMPL_Ref(TYPE)
#define ref(VAR)   IMPL_ref(VAR)
#define deref(VAR) IMPL_deref(VAR)

/*========== Macros Implementation ==========================================*/

#if defined(__cplusplus)
/* In C++, references work normally */
#define IMPL_Ref(TYPE)  TYPE&
#define IMPL_ref(VAR)   VAR
#define IMPL_deref(VAR) VAR
#else
#define IMPL_Ref(TYPE) Nonnull(Ptr(TYPE))
#if defined(DEBUG_ENABLED) && DEBUG_ENABLED
#define IMPL_ref(VAR) Nonnull(anyptr) ref__checkNotNull((anyptr)(&(VAR)))
#else
#define IMPL_ref(VAR) ((anyptr)(&(VAR)))
#endif
#define IMPL_deref(VAR) (*(VAR))
#endif

/*========== Extern Function Prototypes =====================================*/

extern Nonnull(anyptr) ref__checkNotNull(Nonnull(anyptr) ptr);


#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* CORE_PRIM_REF_INCLUDED */

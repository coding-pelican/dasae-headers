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
#define Ref(TYPE)  RETURN_Ref(TYPE)
#define ref(VAR)   RETURN_ref(VAR)
#define deref(VAR) RETURN_deref(VAR)

/*========== Macros Implementation ==========================================*/

#if defined(__cplusplus)
/* In C++, references work normally */
#define RETURN_Ref(TYPE)  TYPE&
#define RETURN_ref(VAR)   VAR
#define RETURN_deref(VAR) VAR
#else
#define RETURN_Ref(TYPE) Nonnull(Ptr(TYPE))
#if defined(DEBUG_ENABLED) && DEBUG_ENABLED
#define RETURN_ref(VAR) Nonnull(anyptr) ref__checkNotNull((anyptr)(addr(VAR)))
#else
#define RETURN_ref(VAR) ((anyptr)(addr(VAR)))
#endif
#define RETURN_deref(VAR) (*(VAR))
#endif

/*========== Extern Function Prototypes =====================================*/

extern Nonnull(anyptr) ref__checkNotNull(Nonnull(anyptr) ptr);


#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* CORE_PRIM_REF_INCLUDED */

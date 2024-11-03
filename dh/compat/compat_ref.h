#ifndef COMPAT_REF_INCLUDED
#define COMPAT_REF_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */


/*========== Includes =======================================================*/

#include "compat_inline.h"

#include "../prim/prim_null.h"
#include "../prim/prim_ptr.h"

#include "../debug/debug_assert.h"

/*========== Macros and Definitions =========================================*/

/* Reference definition */
#define Ref(_T)     RETURN_Ref(_T)
#define ref(_var)   RETURN_ref(_var)
#define deref(_var) RETURN_deref(_var)

/*========== Macros Implementation ==========================================*/

#if defined(__cplusplus)
/* In C++, references work normally */
#define RETURN_Ref(_T)     _T&
#define RETURN_ref(_var)   _var
#define RETURN_deref(_var) _var
#else
#define RETURN_Ref(_T) Nonnull(Ptr(_T))
#if defined(DEBUG_ENABLED) && DEBUG_ENABLED
#define RETURN_ref(_var) ref__checkNotNull((anyptr)(&(_var)))
force_inline Nonnull(anyptr) ref__checkNotNull(Nonnull(anyptr) ptr) {
    debug_assertNotNullFmt(ptr, "Reference cannot be null");
    return ptr;
}
#else
#define RETURN_ref(_var) ((anyptr)(&(_var)))
#endif
#define RETURN_deref(_var) (*(_var))
#endif


#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* COMPAT_REF_INCLUDED */

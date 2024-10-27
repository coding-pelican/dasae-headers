#ifndef COMPAT_REF_INCLUDED
#define COMPAT_REF_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */


#include "../debug/debug_assert.h"


#define Ptr(_T) RETURN_Ptr(_T)
#define Ref(_T) RETURN_Ref(_T)
// #define CRef(_T) RETURN_CRef(_T)
// #define RRef(_T) RETURN_RRef(_T)

#define ref(_var)   RETURN_ref(_var)
#define deref(_var) RETURN_deref(_var)


#if defined(__cplusplus)
/* In C++, references work normally */
#  define RETURN_Ptr(_T) _T*
#  define RETURN_Ref(_T) _T&
// #  define RETURN_CRef(_T) const _T&
// #  define RETURN_RRef(_T) _T&&

#  define RETURN_ref(_var)   _var
#  define RETURN_deref(_var) _var
#else
/* In C, simulate references using pointers */
#  define RETURN_Ptr(_T) _T*
#  define RETURN_Ref(_T) _T*
// #  define RETURN_CRef(_T) const _T*
// #  define RETURN_RRef(_T) _T*

#  if defined(DEBUG_ENABLED) && DEBUG_ENABLED
#    define RETURN_ref(_var) ((void*)ref__ensureNotNull((void*)(&(_var))))
static inline void* ref__ensureNotNull(void* ptr) {
    debug_assertNotNullFmt(ptr, "Reference cannot be null");
    return ptr;
}
#  else
#    define RETURN_ref(_var) (&(_var))
#  endif
#  define RETURN_deref(_var) (*(_var))
#endif

// // Helper macros for move semantics (C++11 and later)
// #ifdef __cplusplus
// #  define MOVE(_var)          std::move(_var)
// #  define FORWARD(_T, _var) std::forward<_T>(_var)
// #else
// #  define MOVE(_var)          (_var)
// #  define FORWARD(_T, _var) (_var)
// #endif


#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* COMPAT_REF_INCLUDED */

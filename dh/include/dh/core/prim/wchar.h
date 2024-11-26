#ifndef CORE_PRIM_WCHAR_INCLUDED
#define CORE_PRIM_WCHAR_INCLUDED (1)
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

#include <wchar.h>
#if defined(_WIN32) || defined(_WIN64)
#include <corecrt.h>
#endif // defined(_WIN32) || defined(_WIN64)

typedef wchar_t wchar;

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* CORE_PRIM_WCHAR_INCLUDED */

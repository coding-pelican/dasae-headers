#ifndef OS_WINDOWS_COMMON_INCLUDED
#define OS_WINDOWS_COMMON_INCLUDED (1)

#include "dh/builtin.h"

#if bti_plat_windows
#undef auto
#undef var
#undef let
#undef warn_deprecated

#if !defined(WIN32_LEAN_AND_MEAN)
#define WIN32_LEAN_AND_MEAN
#endif /* !defined(WIN32_LEAN_AND_MEAN) */
#if !defined(NOMINMAX)
#define NOMINMAX
#endif /* !defined(NOMINMAX) */
#include <windows.h>

#define auto            SYN__auto
#define var             SYN__var
#define let             SYN__let
#define warn_deprecated ATTR__warn_deprecated
#endif /* bti_plat_windows */

#endif /* OS_WINDOWS_COMMON_INCLUDED */

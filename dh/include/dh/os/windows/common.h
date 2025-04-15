#ifndef OS_WINDOWS_COMMON_INCLUDED
#define OS_WINDOWS_COMMON_INCLUDED (1)

#include "dh/builtin.h"

#if bti_plat_windows
#undef auto
#undef var
#undef let

#if !defined(WIN32_LEAN_AND_MEAN)
#define WIN32_LEAN_AND_MEAN
#endif /* !defined(WIN32_LEAN_AND_MEAN) */
#if !defined(NOMINMAX)
#define NOMINMAX
#endif /* !defined(NOMINMAX) */
#include <windows.h>

#define auto comp_syn__auto
#define var  comp_syn__var
#define let  comp_syn__let
#endif /* bti_plat_windows */

#endif /* OS_WINDOWS_COMMON_INCLUDED */

#ifndef os_windows_common__included
#define os_windows_common__included 1

#include "dh/builtin.h"

#if plat_windows
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

#define auto __comp_syn__auto
#define var __comp_syn__var
#define let __comp_syn__let
#endif /* plat_windows */

#endif /* os_windows_common__included */

#ifndef os_windows_mem__included
#define os_windows_mem__included 1

#include "dh/builtin.h"

#if plat_type == plat_type_windows
#undef auto
#undef var
#undef let

#include <memoryapi.h>
#if comp_type == comp_type_msvc
#pragma comment(lib, "kernel32.lib")
#endif /* comp_type == comp_type_msvc */

#define auto __comp_syn__auto
#define var __comp_syn__var
#define let __comp_syn__let
#endif /* plat_type == plat_type_windows */

#endif /* os_windows_mem__included */

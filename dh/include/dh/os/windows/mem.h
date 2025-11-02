#ifndef os_windows_mem__included
#define os_windows_mem__included 1

#include "dh/builtin.h"

#if plat_windows
#undef auto
#undef var
#undef let

#include <memoryapi.h>

#define auto __comp_syn__auto
#define var  __comp_syn__var
#define let  __comp_syn__let
#endif /* plat_windows */

#endif /* os_windows_mem__included */

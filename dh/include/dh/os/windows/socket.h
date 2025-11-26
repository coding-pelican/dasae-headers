#ifndef os_windows_socket__included
#define os_windows_socket__included 1

#include "dh/builtin.h"

#if plat_windows
#undef auto
#undef var
#undef let

#include <winsock2.h>
#include <ws2tcpip.h>
#if BUILTIN_COMP_MSVC
#pragma comment(lib, "ws2_32.lib")
#endif /* BUILTIN_COMP_MSVC */

#define auto __comp_syn__auto
#define var __comp_syn__var
#define let __comp_syn__let
#endif /* plat_windows */

#endif /* os_windows_socket__included */

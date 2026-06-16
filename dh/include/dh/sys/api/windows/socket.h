#pragma once
#ifndef sys_api_windows_socket__included
#define sys_api_windows_socket__included 1

#include "dh/builtin.h"

#if plat_type == plat_type_windows
#undef auto
#undef var
#undef let

#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

#define auto __comp_syn__auto
#define var __comp_syn__var
#define let __comp_syn__let
#endif /* plat_type == plat_type_windows */

#endif /* sys_api_windows_socket__included */

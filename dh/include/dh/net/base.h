#pragma once
#ifndef net_base__included
#define net_base__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "dh/prl.h"
#include "dh/sys/posix.h"

#if plat_is_windows
#include "dh/sys/api/windows/sock.h"
#endif

/*========== Macros and Declarations ========================================*/

typedef pp_if_(plat_is_windows)(
    pp_then_(SOCKET),
    pp_else_(sys_posix_fd_t)) net_Handle;
T_use_E$(net_Handle);

typedef struct net_Sock net_Sock;
T_decl_E$(net_Sock);
typedef struct net_Stream net_Stream;
T_decl_E$(net_Stream);
typedef struct net_Svr net_Svr;
T_decl_E$(net_Svr);
typedef struct net_Clt net_Clt;
T_decl_E$(net_Clt);

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* net_base__included */

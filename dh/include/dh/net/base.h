#pragma once
#ifndef net_base__included
#define net_base__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "dh/prl.h"

/*========== Macros and Declarations ========================================*/

typedef usize net_Handle;
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

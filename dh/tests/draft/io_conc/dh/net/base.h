#ifndef net_base__included
#define net_base__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "dh/prl.h"
#include "dh/posix.h"

/*========== Macros and Declarations ========================================*/

T_alias$((net_Handle)(posix_fd_t));
T_alias$((net_Sock)(struct net_Sock));
T_decl_E$(net_Sock);

errset_((net_direct_E)(Unsupported));
T_alias$((net_Self)(struct net_Self));
T_decl_E$($set(net_direct_E)(net_Self));

T_alias$((net_Stream)(struct net_Stream));
T_decl_E$(net_Stream);
T_alias$((net_Svr)(struct net_Svr));
T_decl_E$(net_Svr);
T_alias$((net_Clt)(struct net_Clt));
T_decl_E$(net_Clt);

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* net_base__included */

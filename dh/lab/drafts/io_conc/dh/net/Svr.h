#ifndef net_Svr__included
#define net_Svr__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "Stream.h"

/*========== Macros and Declarations ========================================*/

T_alias$((net_Svr)(struct net_Svr {
    var_(socket, net_Sock);
    var_(options, net_ListenOpts);
}));
T_impl_E$(net_Svr);

$extern fn_((net_Svr_close(net_Svr* self, net_Self net))(void));
$attr($must_check)
$extern fn_((net_Svr_accept(net_Svr* self, net_Self net))(E$net_Stream));

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* net_Svr__included */

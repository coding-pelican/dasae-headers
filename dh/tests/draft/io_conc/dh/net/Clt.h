#ifndef net_Clt__included
#define net_Clt__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "Stream.h"

/*========== Macros and Declarations ========================================*/

struct net_Clt {
    var_(stream, net_Stream);
    var_(remote, net_IpAddr);
};
T_use_P$(net_Clt);
T_use_E$(net_Clt);

$attr($must_check)
$extern fn_((net_Clt_connect(net_Self net, const net_IpAddr* addr, net_ConnectOpts opts))(E$net_Clt));
$extern fn_((net_Clt_close(net_Clt* self, net_Self net))(void));
$attr($must_check)
$extern fn_((net_Clt_shutdown(net_Clt* self, net_Self net, net_ShutdownHow how))(E$void));
$extern fn_((net_Clt_stream(net_Clt* self))(net_Stream*));

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* net_Clt__included */

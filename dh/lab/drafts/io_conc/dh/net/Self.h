#ifndef net_Self__included
#define net_Self__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "Sock.h"
#include "../exec/base.h"

/*========== Macros and Declarations ========================================*/

#define net_direct_supported __bool__net_direct_supported
#if plat_is_windows || plat_based_unix
#define __bool__net_direct_supported pp_true
#else
#define __bool__net_direct_supported pp_false
#endif

T_alias$((net_Self_VTbl)(struct net_Self_VTbl));
$extern let_(net_VTbl_noop, net_Self_VTbl);
$extern let_(net_VTbl_failing, net_Self_VTbl);

struct net_Self {
    var_(ctx, P$raw);
    var_(vtbl, P_const$$(net_Self_VTbl));
};
T_impl_E$($set(net_direct_E)(net_Self));
$extern let_(net_noop, net_Self);
$extern let_(net_failing, net_Self);

$extern fn_((net_listenIp(net_Self self, const net_IpAddr* addr, net_ListenOpts opts))(E$net_Svr));
$extern fn_((net_bindIp(net_Self self, const net_IpAddr* addr, net_BindOpts opts))(E$net_Sock));
$extern fn_((net_connectIp(net_Self self, const net_IpAddr* addr, net_ConnectOpts opts))(E$net_Stream));

$attr($must_check)
$extern fn_((net_direct(void))(net_direct_E$net_Self));
$extern fn_((net_evented(exec_Coop* coop))(net_Self));

struct net_Self_VTbl {
    fn_(((*listenIpFn)(P$raw ctx, const net_IpAddr* addr, net_ListenOpts opts))(E$net_Svr));
    fn_(((*acceptFn)(P$raw ctx, net_Svr* server))(E$net_Stream));
    fn_(((*bindIpFn)(P$raw ctx, const net_IpAddr* addr, net_BindOpts opts))(E$net_Sock));
    fn_(((*connectIpFn)(P$raw ctx, const net_IpAddr* addr, net_ConnectOpts opts))(E$net_Stream));
    fn_(((*readFn)(P$raw ctx, net_Stream stream, S$u8 buf))(E$usize));
    fn_(((*writeFn)(P$raw ctx, net_Stream stream, S_const$u8 bytes))(E$usize));
    fn_(((*closeFn)(P$raw ctx, net_Sock socket))(void));
    fn_(((*shutdownFn)(P$raw ctx, net_Stream stream, net_ShutdownHow how))(E$void));
};

/*========== Macros and Definitions =========================================*/

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* net_Self__included */

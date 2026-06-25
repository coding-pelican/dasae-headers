#pragma once
#ifndef net_common__included
#define net_common__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "base.h"
#include "../time/Dur.h"

/*========== Macros and Declarations ========================================*/

errset_((net_E)(
    net_AccessDenied,
    net_AddressFamilyUnsupported,
    net_AddressInUse,
    net_AddressUnavailable,
    net_ConnectionAborted,
    net_ConnectionRefused,
    net_ConnectionResetByPeer,
    net_HostUnreachable,
    net_MessageOversize,
    net_NetworkDown,
    net_NetworkUnreachable,
    net_NotConnected,
    net_ProtocolUnsupported,
    net_SocketModeUnsupported,
    net_SystemResources,
    net_TimedOut,
    net_Unsupported,
    net_WouldBlock
));

T_alias$((net_Proto)(enum_((net_Proto $fits($packed))(
    net_Proto_tcp = 6,
    net_Proto_udp = 17,
    net_Proto_raw = 255
))));
T_alias$((net_Sock_Mode)(enum_((net_Sock_Mode $fits($packed))(
    net_Sock_Mode_stream = 0,
    net_Sock_Mode_dgram,
    net_Sock_Mode_seqpacket,
    net_Sock_Mode_raw
))));

T_alias$((net_Addr_Family)(enum_((net_Addr_Family $fits($packed))(
    net_Addr_Family_ip4 = 0,
    net_Addr_Family_ip6,
    net_Addr_Family_unix
))));
T_alias$((net_Ip4Addr)(struct net_Ip4Addr {
    var_(bytes, A$$(4, u8));
    var_(port, u16);
}));
T_alias$((net_Ip6Addr)(struct net_Ip6Addr {
    var_(bytes, A$$(16, u8));
    var_(port, u16);
    var_(flow, u32);
    var_(scope_id, u32);
}));
T_alias$((net_IpAddr)(variant_((net_IpAddr $maps(net_Addr_Family))(
    (net_Addr_Family_ip4, net_Ip4Addr),
    (net_Addr_Family_ip6, net_Ip6Addr),
    (net_Addr_Family_unix, Void)
))));
T_use_E$(net_IpAddr);
$attr($inline_always)
$static fn_((net_IpAddr_ip4Loopback(u16 port))(net_IpAddr));
$attr($inline_always)
$static fn_((net_IpAddr_ip6Loopback(u16 port))(net_IpAddr));

T_alias$((net_ConnectOpts)(struct net_ConnectOpts {
    var_(mode, net_Sock_Mode);
    var_(protocol, net_Proto);
    var_(timeout, time_Dur);
    var_(nonblocking, bool);
}));
$static let_(net_ConnectOpts_default, net_ConnectOpts) = {
    .mode = net_Sock_Mode_stream,
    .protocol = net_Proto_tcp,
    .timeout = time_Dur_zero,
    .nonblocking = false,
};

T_alias$((net_ListenOpts)(struct net_ListenOpts {
    var_(mode, net_Sock_Mode);
    var_(protocol, net_Proto);
    var_(kernel_backlog, u32);
    var_(reuse_address, bool);
    var_(nonblocking, bool);
}));
$static let_(net_ListenOpts_default, net_ListenOpts) = {
    .mode = net_Sock_Mode_stream,
    .protocol = net_Proto_tcp,
    .kernel_backlog = 16,
    .reuse_address = true,
    .nonblocking = false,
};

T_alias$((net_BindOpts)(struct net_BindOpts {
    var_(mode, net_Sock_Mode);
    var_(protocol, net_Proto);
    var_(allow_broadcast, bool);
    var_(ip6_only, bool);
    var_(nonblocking, bool);
}));
$static let_(net_BindOpts_default, net_BindOpts) = {
    .mode = net_Sock_Mode_stream,
    .protocol = net_Proto_tcp,
    .allow_broadcast = false,
    .ip6_only = false,
    .nonblocking = false,
};

T_alias$((net_ShutdownHow)(enum_((net_ShutdownHow $fits($packed))(
    net_ShutdownHow_recv = 0,
    net_ShutdownHow_send,
    net_ShutdownHow_both
))));

$extern fn_((net_listenIp(net_IpAddr addr, net_ListenOpts opts))(E$net_Svr));
$extern fn_((net_bindIp(net_IpAddr addr, net_BindOpts opts))(E$net_Sock));
$extern fn_((net_connectIp(net_IpAddr addr, net_ConnectOpts opts))(E$net_Stream));

/*========== Macros and Definitions =========================================*/

#if on_analysis_active_only || on_comptime
fn_((net_IpAddr_ip4Loopback(u16 port))(net_IpAddr)) {
    return (net_IpAddr)union_of((net_Addr_Family_ip4){
        .bytes = A_init({ 127, 0, 0, 1 }),
        .port = port,
    });
};

fn_((net_IpAddr_ip6Loopback(u16 port))(net_IpAddr)) {
    return (net_IpAddr)union_of((net_Addr_Family_ip6){
        .bytes = A_init({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 }),
        .port = port,
        .flow = 0,
        .scope_id = 0,
    });
};
#endif /* on_analysis_active_only || on_comptime */

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* net_common__included */

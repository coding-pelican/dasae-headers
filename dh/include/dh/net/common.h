#ifndef net_common__included
#define net_common__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "base.h"
#include "../time/Duration.h"

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
    net_WouldBlock
));

T_alias$((net_Prot)(enum_((net_Prot $fits($packed))(
    net_Prot_tcp = 6,
    net_Prot_udp = 17,
    net_Prot_raw = 255,
))));
T_alias$((net_Sock_Mode)(enum_((net_Sock_Mode $fits($packed))(
    net_Sock_Mode_stream = 0,
    net_Sock_Mode_dgram,
    net_Sock_Mode_seqpacket,
    net_Sock_Mode_raw,
))));
T_alias$((net_Addr_Family)(enum_((net_Addr_Family $fits($packed))(
    net_Addr_Family_ip4 = 0,
    net_Addr_Family_ip6,
    net_Addr_Family_unix,
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
T_alias$((net_IpAddr)(struct net_IpAddr {
    var_(family, net_Addr_Family);
    union {
        var_(ip4, net_Ip4Addr);
        var_(ip6, net_Ip6Addr);
    };
}));
T_use_E$(net_IpAddr);

T_alias$((net_ConnectOpts)(struct net_ConnectOpts {
    var_(mode, net_Sock_Mode);
    var_(protocol, net_Prot);
    var_(timeout, time_Duration);
    var_(nonblocking, bool);
}));
T_alias$((net_ListenOpts)(struct net_ListenOpts {
    var_(mode, net_Sock_Mode);
    var_(protocol, net_Prot);
    var_(kernel_backlog, u32);
    var_(reuse_address, bool);
    var_(nonblocking, bool);
}));
T_alias$((net_BindOpts)(struct net_BindOpts {
    var_(mode, net_Sock_Mode);
    var_(protocol, net_Prot);
    var_(allow_broadcast, bool);
    var_(ip6_only, bool);
    var_(nonblocking, bool);
}));
T_alias$((net_ShutdownHow)(enum_((net_ShutdownHow $fits($packed))(
    net_ShutdownHow_recv = 0,
    net_ShutdownHow_send,
    net_ShutdownHow_both,
))));

$static let_(net_ConnectOpts_default, net_ConnectOpts) = {
    .mode = net_Sock_Mode_stream,
    .protocol = net_Prot_tcp,
    .timeout = time_Duration_zero,
    .nonblocking = false,
};
$static let_(net_ListenOpts_default, net_ListenOpts) = {
    .mode = net_Sock_Mode_stream,
    .protocol = net_Prot_tcp,
    .kernel_backlog = 16,
    .reuse_address = true,
    .nonblocking = false,
};
$static let_(net_BindOpts_default, net_BindOpts) = {
    .mode = net_Sock_Mode_stream,
    .protocol = net_Prot_tcp,
    .allow_broadcast = false,
    .ip6_only = false,
    .nonblocking = false,
};

$attr($inline_always)
$static fn_((net_IpAddr_ip4(u8 b0, u8 b1, u8 b2, u8 b3, u16 port))(net_IpAddr)) {
    return (net_IpAddr){
        .family = net_Addr_Family_ip4,
        .ip4 = {
            .bytes = { .val = { b0, b1, b2, b3 } },
            .port = port,
        },
    };
}

$attr($inline_always)
$static fn_((net_IpAddr_ip4Loopback(u16 port))(net_IpAddr)) {
    return net_IpAddr_ip4(127, 0, 0, 1, port);
}

$attr($inline_always)
$static fn_((net_IpAddr_ip6Loopback(u16 port))(net_IpAddr)) {
    return (net_IpAddr){
        .family = net_Addr_Family_ip6,
        .ip6 = {
            .bytes = {
                .val = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 } },
            .port = port,
            .flow = 0,
            .scope_id = 0,
        },
    };
}

$extern fn_((net_listenIp(const net_IpAddr* addr, net_ListenOpts opts))(E$net_Svr));
$extern fn_((net_bindIp(const net_IpAddr* addr, net_BindOpts opts))(E$net_Sock));
$extern fn_((net_connectIp(const net_IpAddr* addr, net_ConnectOpts opts))(E$net_Stream));

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* net_common__included */

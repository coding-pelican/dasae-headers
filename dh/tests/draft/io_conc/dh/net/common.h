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
    AccessDenied,
    AddressFamilyUnsupported,
    AddressInUse,
    AddressUnavailable,
    ConnectionAborted,
    ConnectionRefused,
    ConnectionResetByPeer,
    HostUnreachable,
    MessageOversize,
    NetworkDown,
    NetworkUnreachable,
    NotConnected,
    ProtocolUnsupported,
    SocketModeUnsupported,
    SystemResources,
    Timeout,
    Unsupported,
    WouldBlock
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

T_alias$((net_ConnectOpts)(struct net_ConnectOpts {
    var_(mode, net_Sock_Mode);
    var_(protocol, net_Prot);
    var_(timeout, time_Dur);
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

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* net_common__included */

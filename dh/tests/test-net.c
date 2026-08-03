#include "dh-main.h"
#include "dh/net.h"

TEST_fn_("net/common: IPv6 loopback binds an assigned port" $guard) {
    var server = try_(net_listenIp(
        net_IpAddr_ip6Loopback(0), net_ListenOpts_default
    ));
    defer_(net_Svr_close(server));

    try_(TEST_expect(matches(server.socket.addr, net_Addr_Family_ip6)));
    try_(TEST_expect(
        union_to((server.socket.addr)(net_Addr_Family_ip6)).port != 0
    ));
} $unguarded(TEST_fn);

#include "dh-main.h"
#include "dh/net.h"

TEST_fn_("net/Clt: loopback connect accepts nonzero timeout" $guard) {
    var server = try_(net_listenIp(
        net_IpAddr_ip4Loopback(0), net_ListenOpts_default
    ));
    defer_(net_Svr_close(server));
    let port = union_to((server.socket.addr)(net_Addr_Family_ip4)).port;
    let opts = with_((net_ConnectOpts_default)(
        (.timeout)(time_Dur_fromMillis(50))
    ));
    var client = try_(net_Clt_connect(net_IpAddr_ip4Loopback(port), opts));
    defer_(net_Clt_close(client));
    var peer = try_(net_Svr_accept(server));
    defer_(net_Stream_close(peer));

    try_(TEST_expect(matches(client.remote, net_Addr_Family_ip4)));
    try_(TEST_expect(
        union_to((client.remote)(net_Addr_Family_ip4)).port == port
    ));
} $unguarded(TEST_fn);

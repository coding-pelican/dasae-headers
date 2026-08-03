#include "dh-main.h"
#include "dh/net/Sock.h"

TEST_fn_("net/Sock: bound loopback socket retains assigned address and flags" $guard) {
    let opts = with_((net_BindOpts_default)(
        (.nonblocking)(true)
    ));
    let socket = try_(net_bindIp(net_IpAddr_ip4Loopback(0), opts));
    defer_(net_Sock_close(socket));

    try_(TEST_expect(socket.flags.nonblocking));
    try_(TEST_expect(matches(socket.addr, net_Addr_Family_ip4)));
    try_(TEST_expect(
        union_to((socket.addr)(net_Addr_Family_ip4)).port != 0
    ));
} $unguarded(TEST_fn);

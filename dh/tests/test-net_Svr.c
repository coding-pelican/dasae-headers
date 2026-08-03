#include "dh-main.h"
#include "dh/net.h"
#include "dh/mem/common.h"

TEST_fn_("net/Svr: nonblocking accept reports WouldBlock without peer" $guard) {
    let opts = with_((net_ListenOpts_default)(
        (.nonblocking)(true)
    ));
    var server = try_(net_listenIp(net_IpAddr_ip4Loopback(0), opts));
    defer_(net_Svr_close(server));

    let would_block = eval_(bool $scope)(catch_((net_Svr_accept(server))(err, {
        try_(TEST_expect(mem_eqlBytes(
            E_strfy(&err), u8_l("net_WouldBlock")
        )));
        $break_(true);
    }))) eval_(else)({
        $break_(false);
    }) $unscoped(eval);
    try_(TEST_expect(would_block));
} $unguarded(TEST_fn);

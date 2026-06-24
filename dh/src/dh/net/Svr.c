#include "dh/net/Svr.h"
#include "private/share.h"

#if plat_is_windows
$static fn_((net_Svr__windows_accept(net_Svr self))(E$net_Stream) $scope) {
    var_(remote, net__windows_SockAddr) = {
        .storage = cleared(),
        .len = sizeOf$(SOCKADDR_STORAGE),
    };
    let accepted = accept(as$(SOCKET)(self.socket.handle), ptrCast$((SOCKADDR*)(&remote.storage)), &remote.len);
    if (accepted == INVALID_SOCKET) return_err(net__windows_mapError(WSAGetLastError()));
    let remote_ip = catch_((net__windows_ipFromSockAddr(remote))(err, {
        closesocket(accepted);
        return_err(err);
    }));
    if (self.socket.flags.nonblocking) {
        catch_((net__windows_setNonblocking(as$(net_Handle)(accepted), true))(err, {
            closesocket(accepted);
            return_err(err);
        }));
    }
    return_ok({
        .socket = net_Sock_promote(as$(net_Handle)(accepted), remote_ip, self.socket.flags),
    });
} $unscoped(fn);
#endif /* plat_is_windows */

$attr($maybe_unused)
$static fn_((net_Svr__unsupported_accept(net_Svr self))(E$net_Stream) $scope) {
    let_ignore = self;
    return_err(E_cause$net_Unsupported());
} $unscoped(fn);

$static let net_Svr__accept = pp_if_(plat_is_windows)(
    pp_then_(net_Svr__windows_accept),
    pp_else_(net_Svr__unsupported_accept));

fn_((net_Svr_close(net_Svr self))(void)) {
    net_Sock_close(self.socket);
};

fn_((net_Svr_accept(net_Svr self))(E$net_Stream)) {
    return net_Svr__accept(self);
};

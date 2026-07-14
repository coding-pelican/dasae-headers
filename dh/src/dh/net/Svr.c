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

#if plat_is_linux
$static fn_((net_Svr__linux_accept(net_Svr self))(E$net_Stream) $scope) {
    var_(remote, net__linux_SockAddr) = {
        .storage = cleared(),
        .len = sizeOf$(sys_call_linux_sockaddr_storage),
    };
    let accepted = sys_call_linux_accept4(
        (as$(sys_call_linux_fd_t)(self.socket.handle)),
        (ptrCast$((sys_call_linux_sockaddr*)(&remote.storage))),
        &remote.len,
        self.socket.flags.nonblocking ? sys_call_linux_SOCK_NONBLOCK : 0
    );
    if (sys_call_linux_syscall_isErr(accepted)) return_err(net__linux_mapError(sys_call_linux_syscall_err(accepted)));
    let remote_ip = catch_((net__linux_ipFromSockAddr(remote))(err, {
        let_ignore = sys_call_linux_close(as$(sys_call_linux_fd_t)(accepted));
        return_err(err);
    }));
    return_ok({
        .socket = net_Sock_promote(as$(net_Handle)(accepted), remote_ip, self.socket.flags),
    });
} $unscoped(fn);
#endif /* plat_is_linux */

$attr($maybe_unused)
$static fn_((net_Svr__unsupported_accept(net_Svr self))(E$net_Stream) $scope) {
    let_ignore = self;
    return_err(E_cause$net_Unsupported());
} $unscoped(fn);

$static let net_Svr__accept = pp_if_(plat_is_windows)(
    pp_then_(net_Svr__windows_accept),
    pp_else_(pp_if_(plat_is_linux)(
        pp_then_(net_Svr__linux_accept),
        pp_else_(net_Svr__unsupported_accept)
    )));

fn_((net_Svr_close(net_Svr self))(void)) {
    net_Sock_close(self.socket);
};

fn_((net_Svr_accept(net_Svr self))(E$net_Stream)) {
    return net_Svr__accept(self);
};

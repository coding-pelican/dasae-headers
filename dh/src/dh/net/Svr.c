#include "dh/net/Svr.h"
#include "private/share.h"

fn_((net_Svr_close(net_Svr* self))(void)) {
    claim_assert_nonnull(self);
    net_Sock_close(self->socket);
    asg_l((self)(cleared()));
};

fn_((net_Svr_accept(net_Svr* self))(E$net_Stream) $scope) {
    claim_assert_nonnull(self);
#if plat_is_windows
    SOCKADDR_STORAGE remote = cleared();
    var_(remote_len, i32) = sizeOf$(SOCKADDR_STORAGE);
    let accepted = accept(self->socket.handle, ptrCast$((SOCKADDR*)(&remote)), &remote_len);
    if (accepted == INVALID_SOCKET) return_err(net__mapWinErr(WSAGetLastError()));
    let remote_ip = catch_((net__sockaddrToIp(ptrCast$((const SOCKADDR*)(&remote)), remote_len))(err, {
        closesocket(accepted);
        return_err(err);
    }));
    if (self->socket.flags.nonblocking) {
        catch_((net__setNonblocking(accepted, true))(err, {
            closesocket(accepted);
            return_err(err);
        }));
    }
    return_ok({
        .socket = net_Sock_promote(accepted, remote_ip, self->socket.flags),
    });
#else
    let_ignore = self;
    return_err(E_cause$net_SystemResources());
#endif
} $unscoped(fn);

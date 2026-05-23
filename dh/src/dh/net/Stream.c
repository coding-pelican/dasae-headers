#include "dh/net/Stream.h"
#include "private/share.h"

#if plat_is_windows

$static fn_((net__shutdownHow(net_ShutdownHow how))(i32)) {
    switch (how) {
    case net_ShutdownHow_recv: return SD_RECEIVE;
    case net_ShutdownHow_send: return SD_SEND;
    case net_ShutdownHow_both: $fallthrough;
    default_() return SD_BOTH $end(default);
    }
}

#endif

fn_((net_Sock_close(net_Sock self))(void)) {
#if plat_is_windows
    if (self.handle != INVALID_SOCKET) {
        let_ignore = closesocket(self.handle);
    }
#else
    let_ignore = self;
#endif
}

fn_((net_Stream_close(net_Stream self))(void)) {
    net_Sock_close(self.socket);
}

fn_((net_Stream_shutdown(net_Stream self, net_ShutdownHow how))(E$void) $scope) {
#if plat_is_windows
    if (shutdown(self.socket.handle, net__shutdownHow(how)) == SOCKET_ERROR) {
        return_err(net__mapWinErr(WSAGetLastError()));
    }
    return_ok({});
#else
    let_ignore = self;
    let_ignore = how;
    return_err(E_cause$SystemResourcesNet());
#endif
} $unscoped(fn);

fn_((net_Stream_read(net_Stream self, S$u8 buf))(E$usize) $scope) {
#if plat_is_windows
    let read = recv(self.socket.handle, ptrCast$((char*)(buf.ptr)), as$(i32)(buf.len), 0);
    if (read == SOCKET_ERROR) return_err(net__mapWinErr(WSAGetLastError()));
    if (read == 0) return_ok(0);
    return_ok(as$(usize)(read));
#else
    let_ignore = self;
    let_ignore = buf;
    return_err(E_cause$SystemResourcesNet());
#endif
} $unscoped(fn);

fn_((net_Stream_write(net_Stream self, S_const$u8 bytes))(E$usize) $scope) {
#if plat_is_windows
    let wrote = send(self.socket.handle, ptrCast$((const char*)(bytes.ptr)), as$(i32)(bytes.len), 0);
    if (wrote == SOCKET_ERROR) return_err(net__mapWinErr(WSAGetLastError()));
    return_ok(as$(usize)(wrote));
#else
    let_ignore = self;
    let_ignore = bytes;
    return_err(E_cause$SystemResourcesNet());
#endif
} $unscoped(fn);

$static fn_((net_Stream_IO__read(P$raw ctx, S$u8 buf))(E$usize)) {
    let self = ptrCast$((net_Stream_IO*)(ensureNonnull(ctx)));
    return net_Stream_read(self->stream, buf);
}

$static fn_((net_Stream_IO__write(P$raw ctx, S_const$u8 bytes))(E$usize)) {
    let self = ptrCast$((net_Stream_IO*)(ensureNonnull(ctx)));
    return net_Stream_write(self->stream, bytes);
}

fn_((net_Stream_io(net_Stream stream))(net_Stream_IO)) {
    return (net_Stream_IO){
        .stream = stream,
    };
}

fn_((net_Stream_IO_reader(net_Stream_IO* self))(io_Reader)) {
    claim_assert_nonnull(self);
    return (io_Reader){
        .ctx = self,
        .read = net_Stream_IO__read,
    };
}

fn_((net_Stream_IO_writer(net_Stream_IO* self))(io_Writer)) {
    claim_assert_nonnull(self);
    return (io_Writer){
        .ctx = self,
        .write = net_Stream_IO__write,
    };
}

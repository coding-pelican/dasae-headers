#include "dh/net/Stream.h"
#include "private/share.h"

#if plat_is_windows
$static fn_((net_Stream__windows_shutdownHow(net_ShutdownHow how))(i32)) {
    switch (how) {
    case net_ShutdownHow_recv: return SD_RECEIVE;
    case net_ShutdownHow_send: return SD_SEND;
    case net_ShutdownHow_both: $fallthrough;
    default_() return SD_BOTH $end(default);
    }
};

$static fn_((net_Sock__windows_close(net_Sock self))(void)) {
    if (self.handle != INVALID_SOCKET) {
        let_ignore = closesocket(as$(SOCKET)(self.handle));
    }
};

$static fn_((net_Stream__windows_shutdown(net_Stream self, net_ShutdownHow how))(E$void) $scope) {
    if (shutdown(as$(SOCKET)(self.socket.handle), net_Stream__windows_shutdownHow(how)) == SOCKET_ERROR) {
        return_err(net__windows_mapError(WSAGetLastError()));
    }
    return_ok({});
} $unscoped(fn);

$static fn_((net_Stream__windows_read(net_Stream self, S$u8 buf))(E$usize) $scope) {
    let read = recv(as$(SOCKET)(self.socket.handle), ptrCast$((char*)(buf.ptr)), as$(i32)(buf.len), 0);
    if (read == SOCKET_ERROR) return_err(net__windows_mapError(WSAGetLastError()));
    if (read == 0) return_ok(0);
    return_ok(as$(usize)(read));
} $unscoped(fn);

$static fn_((net_Stream__windows_write(net_Stream self, S_const$u8 bytes))(E$usize) $scope) {
    let wrote = send(as$(SOCKET)(self.socket.handle), ptrCast$((const char*)(bytes.ptr)), as$(i32)(bytes.len), 0);
    if (wrote == SOCKET_ERROR) return_err(net__windows_mapError(WSAGetLastError()));
    return_ok(as$(usize)(wrote));
} $unscoped(fn);
#endif /* plat_is_windows */

#if plat_is_linux
$static fn_((net_Stream__linux_shutdownHow(net_ShutdownHow how))(sys_call_linux_shutdown_how_t)) {
    switch (how) {
    case net_ShutdownHow_recv: return sys_call_linux_SHUT_RD;
    case net_ShutdownHow_send: return sys_call_linux_SHUT_WR;
    case net_ShutdownHow_both: $fallthrough;
    default_() return sys_call_linux_SHUT_RDWR $end(default);
    }
};

$static fn_((net_Sock__linux_close(net_Sock self))(void)) {
    let_ignore = sys_call_linux_close(as$(sys_call_linux_fd_t)(self.handle));
};

$static fn_((net_Stream__linux_shutdown(net_Stream self, net_ShutdownHow how))(E$void) $scope) {
    let rc = sys_call_linux_shutdown(as$(sys_call_linux_fd_t)(self.socket.handle), net_Stream__linux_shutdownHow(how));
    if (sys_call_linux_syscall_isErr(rc)) return_err(net__linux_mapError(sys_call_linux_syscall_err(rc)));
    return_ok({});
} $unscoped(fn);

$static fn_((net_Stream__linux_read(net_Stream self, S$u8 buf))(E$usize) $scope) {
    let read = sys_call_linux_recvfrom(as$(sys_call_linux_fd_t)(self.socket.handle), buf.ptr, buf.len, 0, null, null);
    if (sys_call_linux_syscall_isErr(read)) return_err(net__linux_mapError(sys_call_linux_syscall_err(read)));
    return_ok(as$(usize)(read));
} $unscoped(fn);

$static fn_((net_Stream__linux_write(net_Stream self, S_const$u8 bytes))(E$usize) $scope) {
    let wrote = sys_call_linux_sendto(as$(sys_call_linux_fd_t)(self.socket.handle), bytes.ptr, bytes.len, 0, null, 0);
    if (sys_call_linux_syscall_isErr(wrote)) return_err(net__linux_mapError(sys_call_linux_syscall_err(wrote)));
    return_ok(as$(usize)(wrote));
} $unscoped(fn);
#endif /* plat_is_linux */

$attr($maybe_unused)
$static fn_((net_Sock__unsupported_close(net_Sock self))(void)) {
    let_ignore = self;
};

$attr($maybe_unused)
$static fn_((net_Stream__unsupported_shutdown(net_Stream self, net_ShutdownHow how))(E$void) $scope) {
    let_ignore = self;
    let_ignore = how;
    return_err(E_cause$net_Unsupported());
} $unscoped(fn);

$attr($maybe_unused)
$static fn_((net_Stream__unsupported_read(net_Stream self, S$u8 buf))(E$usize) $scope) {
    let_ignore = self;
    let_ignore = buf;
    return_err(E_cause$net_Unsupported());
} $unscoped(fn);

$attr($maybe_unused)
$static fn_((net_Stream__unsupported_write(net_Stream self, S_const$u8 bytes))(E$usize) $scope) {
    let_ignore = self;
    let_ignore = bytes;
    return_err(E_cause$net_Unsupported());
} $unscoped(fn);

$static let net_Sock__close = pp_if_(plat_is_windows)(
    pp_then_(net_Sock__windows_close),
    pp_else_(pp_if_(plat_is_linux)(
        pp_then_(net_Sock__linux_close),
        pp_else_(net_Sock__unsupported_close)
    )));
$static let net_Stream__shutdown = pp_if_(plat_is_windows)(
    pp_then_(net_Stream__windows_shutdown),
    pp_else_(pp_if_(plat_is_linux)(
        pp_then_(net_Stream__linux_shutdown),
        pp_else_(net_Stream__unsupported_shutdown)
    )));
$static let net_Stream__read = pp_if_(plat_is_windows)(
    pp_then_(net_Stream__windows_read),
    pp_else_(pp_if_(plat_is_linux)(
        pp_then_(net_Stream__linux_read),
        pp_else_(net_Stream__unsupported_read)
    )));
$static let net_Stream__write = pp_if_(plat_is_windows)(
    pp_then_(net_Stream__windows_write),
    pp_else_(pp_if_(plat_is_linux)(
        pp_then_(net_Stream__linux_write),
        pp_else_(net_Stream__unsupported_write)
    )));

fn_((net_Sock_close(net_Sock self))(void)) {
    net_Sock__close(self);
};

fn_((net_Stream_close(net_Stream self))(void)) {
    net_Sock_close(self.socket);
};

fn_((net_Stream_shutdown(net_Stream self, net_ShutdownHow how))(E$void)) {
    return net_Stream__shutdown(self, how);
};

fn_((net_Stream_read(net_Stream self, S$u8 buf))(E$usize)) {
    return net_Stream__read(self, buf);
};

fn_((net_Stream_write(net_Stream self, S_const$u8 bytes))(E$usize)) {
    return net_Stream__write(self, bytes);
};

$static fn_((net_Stream_IO__read(
    P$raw ctx,
    S$u8 buf
))(io_ReadE$usize) $scope) {
    let self = ptrCast$((net_Stream_IO*)(ensureNonnull(ctx)));
    let read = catch_((net_Stream_read(self->stream, buf))($ignore, {
        return_err(E_cause$io_ReadFailed());
    }));
    return_ok(read);
} $unscoped(fn);

$static fn_((net_Stream_IO__write(
    P$raw ctx,
    S_const$u8 bytes
))(io_WriteE$usize) $scope) {
    let self = ptrCast$((net_Stream_IO*)(ensureNonnull(ctx)));
    let written = catch_((net_Stream_write(self->stream, bytes))($ignore, {
        return_err(E_cause$io_WriteFailed());
    }));
    return_ok(written);
} $unscoped(fn);

fn_((net_Stream_io(net_Stream stream))(net_Stream_IO)) {
    return (net_Stream_IO){
        .stream = stream,
    };
};

fn_((net_Stream_IO_reader(net_Stream_IO* self))(io_Reader)) {
    claim_assert_nonnull(self);
    return (io_Reader){
        .ctx = self,
        .readFn = net_Stream_IO__read,
    };
};

fn_((net_Stream_IO_writer(net_Stream_IO* self))(io_Writer)) {
    claim_assert_nonnull(self);
    return (io_Writer){
        .ctx = self,
        .writeFn = net_Stream_IO__write,
    };
};

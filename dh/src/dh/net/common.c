#include "dh/mem/common.h"
#include "dh/net.h"
#include "private/share.h"

#if plat_is_windows
$static INIT_ONCE net__windows_wsa_once = INIT_ONCE_STATIC_INIT;
$static var_(net__windows_wsa_status, DWORD) = ERROR_SUCCESS;

static BOOL CALLBACK net__windows_wsaInit(PINIT_ONCE init_once, PVOID param, PVOID* ctx) {
    let_ignore = init_once;
    let_ignore = param;
    let_ignore = ctx;

    WSADATA data = cleared();
    let rc = WSAStartup(MAKEWORD(2, 2), &data);
    net__windows_wsa_status = rc == 0 ? ERROR_SUCCESS : as$(DWORD)(rc);
    return TRUE;
};

$static fn_((net__windows_ensureStarted(void))(E$void) $scope) {
    if (!InitOnceExecuteOnce(&net__windows_wsa_once, net__windows_wsaInit, null, null)) {
        return_err(E_cause$net_SysResrcs());
    }
    if (net__windows_wsa_status != ERROR_SUCCESS) return_err(E_cause$net_SysResrcs());
    return_ok({});
} $unscoped(fn);

$static fn_((net__windows_sockType(net_Sock_Mode mode))(E$i32) $scope) {
    switch (mode) {
    case_((net_Sock_Mode_stream)) return_ok(SOCK_STREAM) $end(case);
    case_((net_Sock_Mode_dgram)) return_ok(SOCK_DGRAM) $end(case);
    case_((net_Sock_Mode_raw)) return_ok(SOCK_RAW) $end(case);
    case net_Sock_Mode_seqpacket: $fallthrough;
    default_() return_err(E_cause$net_SocketModeUnsupported()) $end(default);
    }
} $unscoped(fn);

$static fn_((net__windows_protocol(net_Protoc protocol))(E$i32) $scope) {
    switch (protocol) {
    case_((net_Protoc_tcp)) return_ok(IPPROTO_TCP) $end(case);
    case_((net_Protoc_udp)) return_ok(IPPROTO_UDP) $end(case);
    case_((net_Protoc_raw)) return_ok(IPPROTO_RAW) $end(case);
    default_() return_err(E_cause$net_ProtocolUnsupported()) $end(default);
    }
} $unscoped(fn);

$static fn_((net__windows_family(net_IpAddr addr))(E$i32) $scope) {
    match_(addr){
    case_((net_Addr_Family_ip4)) return_ok(AF_INET) $end(case);
    case_((net_Addr_Family_ip6)) return_ok(AF_INET6) $end(case);
    case net_Addr_Family_unix: $fallthrough;
    default_() return_err(E_cause$net_AddressFamilyUnsupported()) $end(default);
    }$end(match);
} $unscoped(fn);

$static fn_((net__windows_sockaddrFromIp(net_IpAddr addr))(E$net__windows_SockAddr) $scope) {
    var_(out, net__windows_SockAddr) = {
        .storage = cleared(),
        .len = 0,
    };
    match_(addr){
    patt_((net_Addr_Family_ip4)(ip)) {
        SOCKADDR_IN* ip4 = ptrCast$((SOCKADDR_IN*)(&out.storage));
        *ip4 = (SOCKADDR_IN){ 0 };
        ip4->sin_family = AF_INET;
        ip4->sin_port = htons(ip.port);
        mem_copyBytes(mem_asBytesMut(u_anyP(&ip4->sin_addr)), A_ref$((S_const$u8)(ip.bytes)));
        out.len = sizeOf$(SOCKADDR_IN);
        return_ok(out);
    } $end(patt);
    patt_((net_Addr_Family_ip6)(ip)) {
        SOCKADDR_IN6* ip6 = ptrCast$((SOCKADDR_IN6*)(&out.storage));
        *ip6 = (SOCKADDR_IN6){ 0 };
        ip6->sin6_family = AF_INET6;
        ip6->sin6_port = htons(ip.port);
        ip6->sin6_flowinfo = htonl(ip.flow);
        ip6->sin6_scope_id = ip.scope_id;
        mem_copyBytes(mem_asBytesMut(u_anyP(&ip6->sin6_addr)), A_ref$((S_const$u8)(ip.bytes)));
        out.len = sizeOf$(SOCKADDR_IN6);
        return_ok(out);
    } $end(patt);
    case net_Addr_Family_unix: $fallthrough;
    default_() return_err(E_cause$net_AddressFamilyUnsupported()) $end(default);
    }$end(match);
} $unscoped(fn);

$static fn_((net__windows_localIp(SOCKET socket))(E$net_IpAddr) $scope) {
    var_(addr, net__windows_SockAddr) = {
        .storage = cleared(),
        .len = sizeOf$(SOCKADDR_STORAGE),
    };
    if (getsockname(socket, ptrCast$((SOCKADDR*)(&addr.storage)), &addr.len) == SOCKET_ERROR) {
        return_err(net__windows_mapError(WSAGetLastError()));
    }
    return_(net__windows_ipFromSockAddr(addr));
} $unscoped(fn);

$static fn_((net__windows_newSocket(net_IpAddr addr, net_Sock_Mode mode, net_Protoc protocol))(E$net_Handle) $scope) {
    let family = try_(net__windows_family(addr));
    let sock_type = try_(net__windows_sockType(mode));
    let sock_protocol = try_(net__windows_protocol(protocol));
    let socket = WSASocketA(family, sock_type, sock_protocol, null, 0, 0);
    if (socket == INVALID_SOCKET) return_err(net__windows_mapError(WSAGetLastError()));
    return_ok(as$(net_Handle)(socket));
} $unscoped(fn);

$static fn_((net__windows_setIp6Only(net_Handle socket, bool ip6_only))(E$void) $scope) {
    var flag = ip6_only ? 1 : 0;
    if (setsockopt(socket, IPPROTO_IPV6, IPV6_V6ONLY, ptrCast$((const char*)(&flag)), sizeOf$(i32)) == SOCKET_ERROR) {
        return_err(net__windows_mapError(WSAGetLastError()));
    }
    return_ok({});
} $unscoped(fn);

$static fn_((net__windows_timeval(time_Dur timeout))(struct timeval)) {
    let secs = timeout.secs > as$(u64)(i32_limit_max) ? i32_limit_max : as$(i32)(timeout.secs);
    return (struct timeval){
        .tv_sec = as$(long)(secs),
        .tv_usec = as$(long)(timeout.nanos / time_nanos_per_micro),
    };
};

$static fn_((net__windows_waitWritable(net_Handle socket, time_Dur timeout))(E$void) $scope) {
    var write_fds = (fd_set){ 0 };
    FD_ZERO(&write_fds);
    FD_SET(socket, &write_fds);

    var timeout_tv = net__windows_timeval(timeout);
    let selected = select(0, null, &write_fds, null, &timeout_tv);
    if (selected == SOCKET_ERROR) return_err(net__windows_mapError(WSAGetLastError()));
    if (selected == 0) return_err(E_cause$net_TimedOut());
    return_(net__windows_finishConnect(socket));
} $unscoped(fn);

$static fn_((net__windows_bindIp(net_IpAddr addr, net_BindOpts opts))(E$net_Sock) $scope) {
    try_(net__windows_ensureStarted());
    if (opts.ip6_only && !matches(addr, net_Addr_Family_ip6)) return_err(E_cause$net_AddressFamilyUnsupported());

    let socket = try_(net__windows_newSocket(addr, opts.mode, opts.protocol));
    if (matches(addr, net_Addr_Family_ip6)) {
        catch_((net__windows_setIp6Only(socket, opts.ip6_only))(err, {
            closesocket(as$(SOCKET)(socket));
            return_err(err);
        }));
    }
    if (opts.allow_broadcast) {
        var_(flag, BOOL) = TRUE;
        if (setsockopt(socket, SOL_SOCKET, SO_BROADCAST, as$(const char*)(&flag), sizeOf$(BOOL)) == SOCKET_ERROR) {
            let err = WSAGetLastError();
            closesocket(as$(SOCKET)(socket));
            return_err(net__windows_mapError(err));
        }
    }

    let sockaddr = try_(net__windows_sockaddrFromIp(addr));
    if (bind(as$(SOCKET)(socket), ptrCast$((const SOCKADDR*)(&sockaddr.storage)), sockaddr.len) == SOCKET_ERROR) {
        let err = WSAGetLastError();
        closesocket(as$(SOCKET)(socket));
        return_err(net__windows_mapError(err));
    }
    if (opts.nonblocking) {
        catch_((net__windows_setNonblocking(socket, true))(err, {
            closesocket(as$(SOCKET)(socket));
            return_err(err);
        }));
    }

    let local = catch_((net__windows_localIp(as$(SOCKET)(socket)))(err, {
        closesocket(as$(SOCKET)(socket));
        return_err(err);
    }));
    return_ok(net_Sock_promote(socket, local, (net_Sock_Flags){ .nonblocking = opts.nonblocking }));
} $unscoped(fn);

$static fn_((net__windows_listenIp(net_IpAddr addr, net_ListenOpts opts))(E$net_Svr) $scope) {
    try_(net__windows_ensureStarted());
    if (opts.mode != net_Sock_Mode_stream) return_err(E_cause$net_SocketModeUnsupported());
    if (opts.protocol != net_Protoc_tcp) return_err(E_cause$net_ProtocolUnsupported());

    let socket = try_(net__windows_newSocket(addr, opts.mode, opts.protocol));
    if (matches(addr, net_Addr_Family_ip6)) {
        catch_((net__windows_setIp6Only(socket, false))(err, {
            closesocket(as$(SOCKET)(socket));
            return_err(err);
        }));
    }
    if (opts.reuse_address) {
        var_(flag, BOOL) = TRUE;
        if (setsockopt(socket, SOL_SOCKET, SO_REUSEADDR, as$(const char*)(&flag), sizeOf$(BOOL)) == SOCKET_ERROR) {
            let err = WSAGetLastError();
            closesocket(as$(SOCKET)(socket));
            return_err(net__windows_mapError(err));
        }
    }

    let sockaddr = try_(net__windows_sockaddrFromIp(addr));
    if (bind(as$(SOCKET)(socket), ptrCast$((const SOCKADDR*)(&sockaddr.storage)), sockaddr.len) == SOCKET_ERROR) {
        let err = WSAGetLastError();
        closesocket(as$(SOCKET)(socket));
        return_err(net__windows_mapError(err));
    }

    let local = catch_((net__windows_localIp(as$(SOCKET)(socket)))(err, {
        closesocket(as$(SOCKET)(socket));
        return_err(err);
    }));

    if (listen(as$(SOCKET)(socket), as$(i32)(opts.kernel_backlog)) == SOCKET_ERROR) {
        let err = WSAGetLastError();
        closesocket(as$(SOCKET)(socket));
        return_err(net__windows_mapError(err));
    }
    if (opts.nonblocking) {
        catch_((net__windows_setNonblocking(socket, true))(err, {
            closesocket(as$(SOCKET)(socket));
            return_err(err);
        }));
    }

    return_ok({
        .socket = net_Sock_promote(socket, local, (net_Sock_Flags){ .nonblocking = opts.nonblocking }),
        .options = opts,
    });
} $unscoped(fn);

$static fn_((net__windows_connectIp(net_IpAddr addr, net_ConnectOpts opts))(E$net_Stream) $scope) {
    try_(net__windows_ensureStarted());
    if (opts.mode != net_Sock_Mode_stream) return_err(E_cause$net_SocketModeUnsupported());
    if (opts.protocol != net_Protoc_tcp) return_err(E_cause$net_ProtocolUnsupported());

    let socket = try_(net__windows_newSocket(addr, opts.mode, opts.protocol));
    let use_async_connect = opts.nonblocking || !time_Dur_isZero(opts.timeout);
    if (use_async_connect) {
        catch_((net__windows_setNonblocking(socket, true))(err, {
            closesocket(as$(SOCKET)(socket));
            return_err(err);
        }));
    }

    let sockaddr = try_(net__windows_sockaddrFromIp(addr));

    let rc = connect(as$(SOCKET)(socket), ptrCast$((const SOCKADDR*)(&sockaddr.storage)), sockaddr.len);
    if (rc == SOCKET_ERROR) {
        let err = WSAGetLastError();
        let in_progress = err == WSAEWOULDBLOCK || err == WSAEINPROGRESS || err == WSAEALREADY;
        if (!in_progress) {
            closesocket(as$(SOCKET)(socket));
            return_err(net__windows_mapError(err));
        }
        if (time_Dur_isZero(opts.timeout)) {
            if (!opts.nonblocking) {
                closesocket(as$(SOCKET)(socket));
                return_err(E_cause$net_WouldBlock());
            }
        } else {
            catch_((net__windows_waitWritable(socket, opts.timeout))(wait_err, {
                closesocket(as$(SOCKET)(socket));
                return_err(wait_err);
            }));
        }
    }

    if (!opts.nonblocking && use_async_connect) {
        catch_((net__windows_setNonblocking(socket, false))(err, {
            closesocket(as$(SOCKET)(socket));
            return_err(err);
        }));
    }

    return_ok({
        .socket = net_Sock_promote(socket, addr, (net_Sock_Flags){ .nonblocking = opts.nonblocking }),
    });
} $unscoped(fn);
#endif /* plat_is_windows */

#if plat_is_linux
$static fn_((net__linux_sockType(net_Sock_Mode mode))(E$sys_call_linux_sock_type_t) $scope) {
    switch (mode) {
    case_((net_Sock_Mode_stream)) return_ok(sys_call_linux_SOCK_STREAM) $end(case);
    case_((net_Sock_Mode_dgram)) return_ok(sys_call_linux_SOCK_DGRAM) $end(case);
    case_((net_Sock_Mode_raw)) return_ok(sys_call_linux_SOCK_RAW) $end(case);
    case net_Sock_Mode_seqpacket: $fallthrough;
    default_() return_err(E_cause$net_SocketModeUnsupported()) $end(default);
    }
} $unscoped(fn);

$static fn_((net__linux_protocol(net_Protoc protocol))(E$sys_call_linux_sock_protocol_t) $scope) {
    switch (protocol) {
    case_((net_Protoc_tcp)) return_ok(sys_call_linux_IPPROTO_TCP) $end(case);
    case_((net_Protoc_udp)) return_ok(sys_call_linux_IPPROTO_UDP) $end(case);
    case_((net_Protoc_raw)) return_ok(sys_call_linux_IPPROTO_RAW) $end(case);
    default_() return_err(E_cause$net_ProtocolUnsupported()) $end(default);
    }
} $unscoped(fn);

$static fn_((net__linux_family(net_IpAddr addr))(E$sys_call_linux_sock_family_t) $scope) {
    match_(addr){
    case_((net_Addr_Family_ip4)) return_ok(sys_call_linux_AF_INET) $end(case);
    case_((net_Addr_Family_ip6)) return_ok(sys_call_linux_AF_INET6) $end(case);
    case net_Addr_Family_unix: $fallthrough;
    default_() return_err(E_cause$net_AddressFamilyUnsupported()) $end(default);
    }$end(match);
} $unscoped(fn);

$static fn_((net__linux_sockaddrFromIp(net_IpAddr addr))(E$net__linux_SockAddr) $scope) {
    var_(out, net__linux_SockAddr) = {
        .storage = cleared(),
        .len = 0,
    };
    match_(addr){
    patt_((net_Addr_Family_ip4)(ip)) {
        let ip4 = ptrCast$((sys_call_linux_sockaddr_in*)(&out.storage));
        *ip4 = (sys_call_linux_sockaddr_in){ 0 };
        ip4->sin_family = sys_call_linux_AF_INET;
        ip4->sin_port = net__linux_toNetwork16(ip.port);
        raw_memcpy(ip4->sin_addr.s_addr.val, ip.bytes.val, 4);
        out.len = sizeOf$(sys_call_linux_sockaddr_in);
        return_ok(out);
    } $end(patt);
    patt_((net_Addr_Family_ip6)(ip)) {
        let ip6 = ptrCast$((sys_call_linux_sockaddr_in6*)(&out.storage));
        *ip6 = (sys_call_linux_sockaddr_in6){ 0 };
        ip6->sin6_family = sys_call_linux_AF_INET6;
        ip6->sin6_port = net__linux_toNetwork16(ip.port);
        ip6->sin6_flowinfo = net__linux_toNetwork32(ip.flow);
        raw_memcpy(ip6->sin6_addr.s6_addr.val, ip.bytes.val, 16);
        ip6->sin6_scope_id = ip.scope_id;
        out.len = sizeOf$(sys_call_linux_sockaddr_in6);
        return_ok(out);
    } $end(patt);
    case net_Addr_Family_unix: $fallthrough;
    default_() return_err(E_cause$net_AddressFamilyUnsupported()) $end(default);
    }$end(match);
} $unscoped(fn);

$static fn_((net__linux_localIp(net_Handle socket))(E$net_IpAddr) $scope) {
    var_(addr, net__linux_SockAddr) = {
        .storage = cleared(),
        .len = sizeOf$(sys_call_linux_sockaddr_storage),
    };
    let rc = sys_call_linux_getsockname(
        (as$(sys_call_linux_fd_t)(socket)),
        (ptrCast$((sys_call_linux_sockaddr*)(&addr.storage))),
        &addr.len
    );
    if (sys_call_linux_syscall_isErr(rc)) return_err(net__linux_mapError(sys_call_linux_syscall_err(rc)));
    return_(net__linux_ipFromSockAddr(addr));
} $unscoped(fn);

$static fn_((net__linux_newSocket(net_IpAddr addr, net_Sock_Mode mode, net_Protoc protocol))(E$net_Handle) $scope) {
    let family = try_(net__linux_family(addr));
    let sock_type = try_(net__linux_sockType(mode));
    let sock_protocol = try_(net__linux_protocol(protocol));
    let socket = sys_call_linux_socket(family, sock_type, sock_protocol);
    if (sys_call_linux_syscall_isErr(socket)) return_err(net__linux_mapError(sys_call_linux_syscall_err(socket)));
    return_ok(as$(net_Handle)(socket));
} $unscoped(fn);

$static fn_((net__linux_setIp6Only(net_Handle socket, bool ip6_only))(E$void) $scope) {
    var_(flag, i32) = ip6_only ? 1 : 0;
    let rc = sys_call_linux_setsockopt(
        (as$(sys_call_linux_fd_t)(socket)),
        sys_call_linux_IPPROTO_IPV6,
        sys_call_linux_IPV6_V6ONLY,
        &flag, sizeOf$(i32)
    );
    if (sys_call_linux_syscall_isErr(rc)) return_err(net__linux_mapError(sys_call_linux_syscall_err(rc)));
    return_ok({});
} $unscoped(fn);

$static fn_((net__linux_bindIp(net_IpAddr addr, net_BindOpts opts))(E$net_Sock) $scope) {
    if (opts.ip6_only && !matches(addr, net_Addr_Family_ip6)) return_err(E_cause$net_AddressFamilyUnsupported());

    let socket = try_(net__linux_newSocket(addr, opts.mode, opts.protocol));
    if (matches(addr, net_Addr_Family_ip6)) {
        catch_((net__linux_setIp6Only(socket, opts.ip6_only))(err, {
            let_ignore = sys_call_linux_close(as$(sys_call_linux_fd_t)(socket));
            return_err(err);
        }));
    }
    if (opts.allow_broadcast) {
        var_(flag, i32) = 1;
        let rc = sys_call_linux_setsockopt(
            (as$(sys_call_linux_fd_t)(socket)),
            sys_call_linux_SOL_SOCKET,
            sys_call_linux_SO_BROADCAST,
            &flag, sizeOf$(i32)
        );
        if (sys_call_linux_syscall_isErr(rc)) {
            let err = sys_call_linux_syscall_err(rc);
            let_ignore = sys_call_linux_close(as$(sys_call_linux_fd_t)(socket));
            return_err(net__linux_mapError(err));
        }
    }

    let sockaddr = try_(net__linux_sockaddrFromIp(addr));
    let bound = sys_call_linux_bind(
        (as$(sys_call_linux_fd_t)(socket)),
        (ptrCast$((const sys_call_linux_sockaddr*)(&sockaddr.storage))),
        sockaddr.len
    );
    if (sys_call_linux_syscall_isErr(bound)) {
        let err = sys_call_linux_syscall_err(bound);
        let_ignore = sys_call_linux_close(as$(sys_call_linux_fd_t)(socket));
        return_err(net__linux_mapError(err));
    }
    if (opts.nonblocking) {
        catch_((net__linux_setNonblocking(socket, true))(err, {
            let_ignore = sys_call_linux_close(as$(sys_call_linux_fd_t)(socket));
            return_err(err);
        }));
    }
    let local = catch_((net__linux_localIp(socket))(err, {
        let_ignore = sys_call_linux_close(as$(sys_call_linux_fd_t)(socket));
        return_err(err);
    }));
    return_ok(net_Sock_promote(socket, local, (net_Sock_Flags){ .nonblocking = opts.nonblocking }));
} $unscoped(fn);

$static fn_((net__linux_listenIp(net_IpAddr addr, net_ListenOpts opts))(E$net_Svr) $scope) {
    if (opts.mode != net_Sock_Mode_stream) return_err(E_cause$net_SocketModeUnsupported());
    if (opts.protocol != net_Protoc_tcp) return_err(E_cause$net_ProtocolUnsupported());

    let socket = try_(net__linux_newSocket(addr, opts.mode, opts.protocol));
    if (matches(addr, net_Addr_Family_ip6)) {
        catch_((net__linux_setIp6Only(socket, false))(err, {
            let_ignore = sys_call_linux_close(as$(sys_call_linux_fd_t)(socket));
            return_err(err);
        }));
    }
    if (opts.reuse_address) {
        var_(flag, i32) = 1;
        let rc = sys_call_linux_setsockopt(
            (as$(sys_call_linux_fd_t)(socket)),
            sys_call_linux_SOL_SOCKET,
            sys_call_linux_SO_REUSEADDR,
            &flag, sizeOf$(i32)
        );
        if (sys_call_linux_syscall_isErr(rc)) {
            let err = sys_call_linux_syscall_err(rc);
            let_ignore = sys_call_linux_close(as$(sys_call_linux_fd_t)(socket));
            return_err(net__linux_mapError(err));
        }
    }

    let sockaddr = try_(net__linux_sockaddrFromIp(addr));
    let bound = sys_call_linux_bind(
        (as$(sys_call_linux_fd_t)(socket)),
        (ptrCast$((const sys_call_linux_sockaddr*)(&sockaddr.storage))),
        sockaddr.len
    );
    if (sys_call_linux_syscall_isErr(bound)) {
        let err = sys_call_linux_syscall_err(bound);
        let_ignore = sys_call_linux_close(as$(sys_call_linux_fd_t)(socket));
        return_err(net__linux_mapError(err));
    }
    let local = catch_((net__linux_localIp(socket))(err, {
        let_ignore = sys_call_linux_close(as$(sys_call_linux_fd_t)(socket));
        return_err(err);
    }));
    let listening = sys_call_linux_listen(
        (as$(sys_call_linux_fd_t)(socket)),
        (as$(sys_call_linux_word)(opts.kernel_backlog))
    );
    if (sys_call_linux_syscall_isErr(listening)) {
        let err = sys_call_linux_syscall_err(listening);
        let_ignore = sys_call_linux_close(as$(sys_call_linux_fd_t)(socket));
        return_err(net__linux_mapError(err));
    }
    if (opts.nonblocking) {
        catch_((net__linux_setNonblocking(socket, true))(err, {
            let_ignore = sys_call_linux_close(as$(sys_call_linux_fd_t)(socket));
            return_err(err);
        }));
    }
    return_ok({
        .socket = net_Sock_promote(socket, local, (net_Sock_Flags){ .nonblocking = opts.nonblocking }),
        .options = opts,
    });
} $unscoped(fn);

$static fn_((net__linux_connectIp(net_IpAddr addr, net_ConnectOpts opts))(E$net_Stream) $scope) {
    if (opts.mode != net_Sock_Mode_stream) return_err(E_cause$net_SocketModeUnsupported());
    if (opts.protocol != net_Protoc_tcp) return_err(E_cause$net_ProtocolUnsupported());

    let socket = try_(net__linux_newSocket(addr, opts.mode, opts.protocol));
    if (opts.nonblocking) {
        catch_((net__linux_setNonblocking(socket, true))(err, {
            let_ignore = sys_call_linux_close(as$(sys_call_linux_fd_t)(socket));
            return_err(err);
        }));
    }

    let sockaddr = try_(net__linux_sockaddrFromIp(addr));
    let rc = sys_call_linux_connect(
        (as$(sys_call_linux_fd_t)(socket)),
        (ptrCast$((const sys_call_linux_sockaddr*)(&sockaddr.storage))),
        sockaddr.len
    );
    if (sys_call_linux_syscall_isErr(rc)) {
        let err = sys_call_linux_syscall_err(rc);
        if (err != sys_call_linux_EINPROGRESS && err != sys_call_linux_EAGAIN) {
            let_ignore = sys_call_linux_close(as$(sys_call_linux_fd_t)(socket));
            return_err(net__linux_mapError(err));
        }
        if (!opts.nonblocking) {
            let_ignore = sys_call_linux_close(as$(sys_call_linux_fd_t)(socket));
            return_err(E_cause$net_WouldBlock());
        }
    }
    return_ok({
        .socket = net_Sock_promote(socket, addr, (net_Sock_Flags){ .nonblocking = opts.nonblocking }),
    });
} $unscoped(fn);
#endif /* plat_is_linux */

$attr($maybe_unused)
$static fn_((net__unsupported_bindIp(net_IpAddr addr, net_BindOpts opts))(E$net_Sock) $scope) {
    let_ignore = addr;
    let_ignore = opts;
    return_err(E_cause$net_Unsupported());
} $unscoped(fn);

$attr($maybe_unused)
$static fn_((net__unsupported_listenIp(net_IpAddr addr, net_ListenOpts opts))(E$net_Svr) $scope) {
    let_ignore = addr;
    let_ignore = opts;
    return_err(E_cause$net_Unsupported());
} $unscoped(fn);

$attr($maybe_unused)
$static fn_((net__unsupported_connectIp(net_IpAddr addr, net_ConnectOpts opts))(E$net_Stream) $scope) {
    let_ignore = addr;
    let_ignore = opts;
    return_err(E_cause$net_Unsupported());
} $unscoped(fn);

$static let net__bindIp = pp_if_(plat_is_windows)(
    pp_then_(net__windows_bindIp),
    pp_else_(pp_if_(plat_is_linux)(
        pp_then_(net__linux_bindIp),
        pp_else_(net__unsupported_bindIp)
    )));
$static let net__listenIp = pp_if_(plat_is_windows)(
    pp_then_(net__windows_listenIp),
    pp_else_(pp_if_(plat_is_linux)(
        pp_then_(net__linux_listenIp),
        pp_else_(net__unsupported_listenIp)
    )));
$static let net__connectIp = pp_if_(plat_is_windows)(
    pp_then_(net__windows_connectIp),
    pp_else_(pp_if_(plat_is_linux)(
        pp_then_(net__linux_connectIp),
        pp_else_(net__unsupported_connectIp)
    )));

fn_((net_bindIp(net_IpAddr addr, net_BindOpts opts))(E$net_Sock)) {
    return net__bindIp(addr, opts);
};

fn_((net_listenIp(net_IpAddr addr, net_ListenOpts opts))(E$net_Svr)) {
    return net__listenIp(addr, opts);
};

fn_((net_connectIp(net_IpAddr addr, net_ConnectOpts opts))(E$net_Stream)) {
    return net__connectIp(addr, opts);
};

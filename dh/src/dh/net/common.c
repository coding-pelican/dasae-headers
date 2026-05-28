#include "dh/net.h"
#include "private/share.h"

#if plat_is_windows

$static INIT_ONCE net__wsa_once = INIT_ONCE_STATIC_INIT;
$static var_(net__wsa_status, DWORD) = ERROR_SUCCESS;

static BOOL CALLBACK net__wsaInit(PINIT_ONCE init_once, PVOID param, PVOID* ctx) {
    let_ignore = init_once;
    let_ignore = param;
    let_ignore = ctx;

    WSADATA data = cleared();
    let rc = WSAStartup(MAKEWORD(2, 2), &data);
    net__wsa_status = rc == 0 ? ERROR_SUCCESS : as$(DWORD)(rc);
    return TRUE;
}

$static fn_((net__ensureStarted(void))(E$void) $scope) {
    if (!InitOnceExecuteOnce(&net__wsa_once, net__wsaInit, null, null)) {
        return_err(E_cause$net_SystemResources());
    }
    if (net__wsa_status != ERROR_SUCCESS) return_err(E_cause$net_SystemResources());
    return_ok({});
} $unscoped(fn);

$static fn_((net__sockType(net_Sock_Mode mode))(E$i32) $scope) {
    switch (mode) {
    case net_Sock_Mode_stream: return_ok(SOCK_STREAM);
    case net_Sock_Mode_dgram: return_ok(SOCK_DGRAM);
    case net_Sock_Mode_raw: return_ok(SOCK_RAW);
    case net_Sock_Mode_seqpacket: $fallthrough;
    default_() return_err(E_cause$net_SocketModeUnsupported()) $end(default);
    }
} $unscoped(fn);

$static fn_((net__protocol(net_Prot protocol))(E$i32) $scope) {
    switch (protocol) {
    case net_Prot_tcp: return_ok(IPPROTO_TCP);
    case net_Prot_udp: return_ok(IPPROTO_UDP);
    case net_Prot_raw: return_ok(IPPROTO_RAW);
    default_() return_err(E_cause$net_ProtocolUnsupported()) $end(default);
    }
} $unscoped(fn);

$static fn_((net__family(const net_IpAddr* addr))(E$i32) $scope) {
    claim_assert_nonnull(addr);
    switch (addr->family) {
    case net_Addr_Family_ip4: return_ok(AF_INET);
    case net_Addr_Family_ip6: return_ok(AF_INET6);
    case net_Addr_Family_unix: $fallthrough;
    default_() return_err(E_cause$net_AddressFamilyUnsupported()) $end(default);
    }
} $unscoped(fn);

$static fn_((net__sockaddrFromIp(const net_IpAddr* addr, SOCKADDR_STORAGE* out, i32* out_len))(E$void) $scope) {
    claim_assert_nonnull(addr);
    claim_assert_nonnull(out);
    claim_assert_nonnull(out_len);

    switch (addr->family) {
    case net_Addr_Family_ip4: {
        SOCKADDR_IN* ip4 = ptrCast$((SOCKADDR_IN*)(out));
        *ip4 = (SOCKADDR_IN){ 0 };
        ip4->sin_family = AF_INET;
        ip4->sin_port = htons(addr->ip4.port);
        raw_memcpy(&ip4->sin_addr, addr->ip4.bytes.val, 4);
        *out_len = sizeOf$(SOCKADDR_IN);
        return_ok({});
    }
    case net_Addr_Family_ip6: {
        SOCKADDR_IN6* ip6 = ptrCast$((SOCKADDR_IN6*)(out));
        *ip6 = (SOCKADDR_IN6){ 0 };
        ip6->sin6_family = AF_INET6;
        ip6->sin6_port = htons(addr->ip6.port);
        ip6->sin6_flowinfo = htonl(addr->ip6.flow);
        ip6->sin6_scope_id = addr->ip6.scope_id;
        raw_memcpy(&ip6->sin6_addr, addr->ip6.bytes.val, 16);
        *out_len = sizeOf$(SOCKADDR_IN6);
        return_ok({});
    }
    case net_Addr_Family_unix: $fallthrough;
    default_() return_err(E_cause$net_AddressFamilyUnsupported()) $end(default);
    }
} $unscoped(fn);

$static fn_((net__localIp(SOCKET socket))(E$net_IpAddr) $scope) {
    SOCKADDR_STORAGE addr = cleared();
    var_(addr_len, i32) = sizeOf$(SOCKADDR_STORAGE);
    if (getsockname(socket, ptrCast$((SOCKADDR*)(&addr)), &addr_len) == SOCKET_ERROR) {
        return_err(net__mapWinErr(WSAGetLastError()));
    }
    return_(net__sockaddrToIp(ptrCast$((const SOCKADDR*)(&addr)), addr_len));
} $unscoped(fn);

$static fn_((net__newSocket(const net_IpAddr* addr, net_Sock_Mode mode, net_Prot protocol))(E$net_Handle) $scope) {
    claim_assert_nonnull(addr);
    let family = try_(net__family(addr));
    let sock_type = try_(net__sockType(mode));
    let sock_protocol = try_(net__protocol(protocol));
    let socket = WSASocketA(family, sock_type, sock_protocol, null, 0, 0);
    if (socket == INVALID_SOCKET) return_err(net__mapWinErr(WSAGetLastError()));
    return_ok(socket);
} $unscoped(fn);

$static fn_((net__setIp6Only(net_Handle socket, bool ip6_only))(E$void) $scope) {
    var flag = ip6_only ? 1 : 0;
    if (setsockopt(socket, IPPROTO_IPV6, IPV6_V6ONLY, ptrCast$((const char*)(&flag)), sizeOf$(i32)) == SOCKET_ERROR) {
        return_err(net__mapWinErr(WSAGetLastError()));
    }
    return_ok({});
} $unscoped(fn);

$static fn_((net__timeoutToTimeval(time_Duration timeout))(struct timeval)) {
    let secs = timeout.secs > as$(u64)(i32_limit_max) ? i32_limit_max : as$(i32)(timeout.secs);
    return (struct timeval){
        .tv_sec = as$(long)(secs),
        .tv_usec = as$(long)(timeout.nanos / time_nanos_per_micro),
    };
}

$static fn_((net__waitWritable(net_Handle socket, time_Duration timeout))(E$void) $scope) {
    var write_fds = (fd_set){ 0 };
    FD_ZERO(&write_fds);
    FD_SET(socket, &write_fds);

    var timeout_tv = net__timeoutToTimeval(timeout);
    let selected = select(0, null, &write_fds, null, &timeout_tv);
    if (selected == SOCKET_ERROR) return_err(net__mapWinErr(WSAGetLastError()));
    if (selected == 0) return_err(E_cause$net_TimedOut());
    return_(net__finishConnect(socket));
} $unscoped(fn);

fn_((net_bindIp(const net_IpAddr* addr, net_BindOpts opts))(E$net_Sock) $scope) {
    try_(net__ensureStarted());
    claim_assert_nonnull(addr);
    if (opts.ip6_only && addr->family != net_Addr_Family_ip6) return_err(E_cause$net_AddressFamilyUnsupported());

    let socket = try_(net__newSocket(addr, opts.mode, opts.protocol));
    if (addr->family == net_Addr_Family_ip6) {
        catch_((net__setIp6Only(socket, opts.ip6_only))(err, {
            closesocket(socket);
            return_err(err);
        }));
    }
    if (opts.allow_broadcast) {
        var_(flag, BOOL) = TRUE;
        if (setsockopt(socket, SOL_SOCKET, SO_BROADCAST, as$(const char*)(&flag), sizeOf$(BOOL)) == SOCKET_ERROR) {
            let err = WSAGetLastError();
            closesocket(socket);
            return_err(net__mapWinErr(err));
        }
    }

    SOCKADDR_STORAGE sockaddr = cleared();
    var_(sockaddr_len, i32) = 0;
    try_(net__sockaddrFromIp(addr, &sockaddr, &sockaddr_len));
    if (bind(socket, ptrCast$((const SOCKADDR*)(&sockaddr)), sockaddr_len) == SOCKET_ERROR) {
        let err = WSAGetLastError();
        closesocket(socket);
        return_err(net__mapWinErr(err));
    }
    if (opts.nonblocking) {
        catch_((net__setNonblocking(socket, true))(err, {
            closesocket(socket);
            return_err(err);
        }));
    }

    let local = catch_((net__localIp(socket))(err, {
        closesocket(socket);
        return_err(err);
    }));
    return_ok(net_Sock_promote(socket, local, (net_Sock_Flags){ .nonblocking = opts.nonblocking }));
} $unscoped(fn);

fn_((net_listenIp(const net_IpAddr* addr, net_ListenOpts opts))(E$net_Svr) $scope) {
    claim_assert_nonnull(addr);
    try_(net__ensureStarted());
    if (opts.mode != net_Sock_Mode_stream) return_err(E_cause$net_SocketModeUnsupported());
    if (opts.protocol != net_Prot_tcp) return_err(E_cause$net_ProtocolUnsupported());

    let socket = try_(net__newSocket(addr, opts.mode, opts.protocol));
    if (addr->family == net_Addr_Family_ip6) {
        catch_((net__setIp6Only(socket, false))(err, {
            closesocket(socket);
            return_err(err);
        }));
    }
    if (opts.reuse_address) {
        var_(flag, BOOL) = TRUE;
        if (setsockopt(socket, SOL_SOCKET, SO_REUSEADDR, as$(const char*)(&flag), sizeOf$(BOOL)) == SOCKET_ERROR) {
            let err = WSAGetLastError();
            closesocket(socket);
            return_err(net__mapWinErr(err));
        }
    }

    SOCKADDR_STORAGE sockaddr = cleared();
    var_(sockaddr_len, i32) = 0;
    try_(net__sockaddrFromIp(addr, &sockaddr, &sockaddr_len));
    if (bind(socket, ptrCast$((const SOCKADDR*)(&sockaddr)), sockaddr_len) == SOCKET_ERROR) {
        let err = WSAGetLastError();
        closesocket(socket);
        return_err(net__mapWinErr(err));
    }

    let local = catch_((net__localIp(socket))(err, {
        closesocket(socket);
        return_err(err);
    }));

    if (listen(socket, as$(i32)(opts.kernel_backlog)) == SOCKET_ERROR) {
        let err = WSAGetLastError();
        closesocket(socket);
        return_err(net__mapWinErr(err));
    }
    if (opts.nonblocking) {
        catch_((net__setNonblocking(socket, true))(err, {
            closesocket(socket);
            return_err(err);
        }));
    }

    return_ok((net_Svr){
        .socket = net_Sock_promote(socket, local, (net_Sock_Flags){ .nonblocking = opts.nonblocking }),
        .options = opts,
    });
} $unscoped(fn);

fn_((net_connectIp(const net_IpAddr* addr, net_ConnectOpts opts))(E$net_Stream) $scope) {
    claim_assert_nonnull(addr);
    try_(net__ensureStarted());
    if (opts.mode != net_Sock_Mode_stream) return_err(E_cause$net_SocketModeUnsupported());
    if (opts.protocol != net_Prot_tcp) return_err(E_cause$net_ProtocolUnsupported());

    let socket = try_(net__newSocket(addr, opts.mode, opts.protocol));
    let use_async_connect = opts.nonblocking || !time_Duration_isZero(opts.timeout);
    if (use_async_connect) {
        catch_((net__setNonblocking(socket, true))(err, {
            closesocket(socket);
            return_err(err);
        }));
    }

    SOCKADDR_STORAGE sockaddr = cleared();
    var_(sockaddr_len, i32) = 0;
    try_(net__sockaddrFromIp(addr, &sockaddr, &sockaddr_len));

    let rc = connect(socket, ptrCast$((const SOCKADDR*)(&sockaddr)), sockaddr_len);
    if (rc == SOCKET_ERROR) {
        let err = WSAGetLastError();
        let in_progress = err == WSAEWOULDBLOCK || err == WSAEINPROGRESS || err == WSAEALREADY;
        if (!in_progress) {
            closesocket(socket);
            return_err(net__mapWinErr(err));
        }
        if (time_Duration_isZero(opts.timeout)) {
            if (!opts.nonblocking) {
                closesocket(socket);
                return_err(E_cause$net_WouldBlock());
            }
        } else {
            catch_((net__waitWritable(socket, opts.timeout))(wait_err, {
                closesocket(socket);
                return_err(wait_err);
            }));
        }
    }

    if (!opts.nonblocking && use_async_connect) {
        catch_((net__setNonblocking(socket, false))(err, {
            closesocket(socket);
            return_err(err);
        }));
    }

    return_ok((net_Stream){
        .socket = net_Sock_promote(socket, *addr, (net_Sock_Flags){ .nonblocking = opts.nonblocking }),
    });
} $unscoped(fn);

#else

fn_((net_bindIp(const net_IpAddr* addr, net_BindOpts opts))(E$net_Sock) $scope) {
    let_ignore = addr;
    let_ignore = opts;
    return_err(E_cause$net_SystemResources());
} $unscoped(fn);

fn_((net_listenIp(const net_IpAddr* addr, net_ListenOpts opts))(E$net_Svr) $scope) {
    let_ignore = addr;
    let_ignore = opts;
    return_err(E_cause$net_SystemResources());
} $unscoped(fn);

fn_((net_connectIp(const net_IpAddr* addr, net_ConnectOpts opts))(E$net_Stream) $scope) {
    let_ignore = addr;
    let_ignore = opts;
    return_err(E_cause$net_SystemResources());
} $unscoped(fn);

#endif

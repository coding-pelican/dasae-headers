#pragma once
#ifndef dh_net_private_share__included
#define dh_net_private_share__included 1

#include "dh/net.h"

#if plat_is_windows
#include "dh/sys/api/windows/sock.h"

T_alias$((net__windows_SockAddr)(struct net__windows_SockAddr {
    var_(storage, SOCKADDR_STORAGE);
    var_(len, i32);
}));
T_use_E$(net__windows_SockAddr);

$attr($maybe_unused)
$static fn_((net__windows_mapError(i32 err))(net_E)) {
    switch (err) {
    case WSAEACCES: return E_cause$net_AccessDenied();
    case WSAEADDRINUSE: return E_cause$net_AddressInUse();
    case WSAEADDRNOTAVAIL: return E_cause$net_AddressUnavailable();
    case WSAECONNABORTED: return E_cause$net_ConnectionAborted();
    case WSAECONNREFUSED: return E_cause$net_ConnectionRefused();
    case WSAECONNRESET: return E_cause$net_ConnectionResetByPeer();
    case WSAEHOSTUNREACH: return E_cause$net_HostUnreachable();
    case WSAEMSGSIZE: return E_cause$net_MessageOversize();
    case WSAENETDOWN: return E_cause$net_NetworkDown();
    case WSAENETUNREACH: return E_cause$net_NetworkUnreachable();
    case WSAENOTCONN: return E_cause$net_NotConnected();
    case WSAETIMEDOUT: return E_cause$net_TimedOut();
    case WSAEWOULDBLOCK: $fallthrough;
    case WSAEINPROGRESS: $fallthrough;
    case WSAEALREADY: return E_cause$net_WouldBlock();
    default_() return E_cause$net_SystemResources() $end(default);
    }
};

$attr($maybe_unused)
$static fn_((net__windows_ipFromSockAddr(net__windows_SockAddr addr))(E$net_IpAddr) $scope) {
    let raw = ptrCast$((const SOCKADDR*)(&addr.storage));
    let_ignore = addr.len;

    switch (raw->sa_family) {
    case AF_INET: {
        var_(ip4, SOCKADDR_IN) = { 0 };
        raw_memcpy(&ip4, raw, sizeOf$(SOCKADDR_IN));
        let raw = ptrCast$((const u8*)(&ip4.sin_addr));
        return_ok(net_IpAddr_ip4(raw[0], raw[1], raw[2], raw[3], ntohs(ip4.sin_port)));
    }
    case AF_INET6: {
        var_(ip6, SOCKADDR_IN6) = { 0 };
        raw_memcpy(&ip6, raw, sizeOf$(SOCKADDR_IN6));
        var result = (net_IpAddr)union_of((net_Addr_Family_ip6){
            .bytes = { 0 },
            .port = ntohs(ip6.sin6_port),
            .flow = ntohl(ip6.sin6_flowinfo),
            .scope_id = ip6.sin6_scope_id,
        });
        raw_memcpy(union_as((&result)(net_Addr_Family_ip6))->bytes.val, &ip6.sin6_addr, 16);
        return_ok(result);
    }
    default_() return_err(E_cause$net_AddressFamilyUnsupported()) $end(default);
    }
} $unscoped(fn);

$attr($maybe_unused)
$static fn_((net__windows_setNonblocking(net_Handle socket, bool nonblocking))(E$void) $scope) {
    var mode = nonblocking ? 1ul : 0ul;
    if (ioctlsocket(as$(SOCKET)(socket), as$(long)(FIONBIO), &mode) != 0) {
        return_err(net__windows_mapError(WSAGetLastError()));
    }
    return_ok({});
} $unscoped(fn);

$attr($maybe_unused)
$static fn_((net__windows_finishConnect(net_Handle socket))(E$void) $scope) {
    var_(so_error, i32) = 0;
    var_(opt_len, i32) = sizeOf$(i32);
    if (getsockopt(as$(SOCKET)(socket), SOL_SOCKET, SO_ERROR, ptrCast$((char*)(&so_error)), &opt_len) == SOCKET_ERROR) {
        return_err(net__windows_mapError(WSAGetLastError()));
    }
    if (so_error != 0) return_err(net__windows_mapError(so_error));
    return_ok({});
} $unscoped(fn);
#endif /* plat_is_windows */

#endif /* dh_net_private_share__included */

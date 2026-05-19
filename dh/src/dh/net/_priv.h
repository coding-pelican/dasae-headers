#ifndef dh_src_dh_net_priv__included
#define dh_src_dh_net_priv__included 1

#include "dh/net.h"

#if plat_is_windows

$attr($maybe_unused)
$static fn_((net__mapWinErr(i32 err))(net_E)) {
    switch (err) {
    case WSAEACCES: return E_cause$AccessDeniedNet();
    case WSAEADDRINUSE: return E_cause$AddressInUseNet();
    case WSAEADDRNOTAVAIL: return E_cause$AddressUnavailableNet();
    case WSAECONNABORTED: return E_cause$ConnectionAbortedNet();
    case WSAECONNREFUSED: return E_cause$ConnectionRefusedNet();
    case WSAECONNRESET: return E_cause$ConnectionResetByPeerNet();
    case WSAEHOSTUNREACH: return E_cause$HostUnreachableNet();
    case WSAEMSGSIZE: return E_cause$MessageOversizeNet();
    case WSAENETDOWN: return E_cause$NetworkDownNet();
    case WSAENETUNREACH: return E_cause$NetworkUnreachableNet();
    case WSAENOTCONN: return E_cause$NotConnectedNet();
    case WSAETIMEDOUT: return E_cause$TimedOutNet();
    case WSAEWOULDBLOCK: $fallthrough;
    case WSAEINPROGRESS: $fallthrough;
    case WSAEALREADY: return E_cause$WouldBlockNet();
    default_() return E_cause$SystemResourcesNet() $end(default);
    }
}

$attr($maybe_unused)
$static fn_((net__sockaddrToIp(const SOCKADDR* addr, i32 addr_len))(E$net_IpAddr) $scope) {
    claim_assert_nonnull(addr);
    let_ignore = addr_len;

    switch (addr->sa_family) {
    case AF_INET: {
        var_(ip4, SOCKADDR_IN) = {0};
        raw_memcpy(&ip4, addr, sizeOf$(SOCKADDR_IN));
        let raw = ptrCast$((const u8*)(&ip4.sin_addr));
        return_ok(net_IpAddr_ip4(raw[0], raw[1], raw[2], raw[3], ntohs(ip4.sin_port)));
    }
    case AF_INET6: {
        var_(ip6, SOCKADDR_IN6) = {0};
        raw_memcpy(&ip6, addr, sizeOf$(SOCKADDR_IN6));
        var result = (net_IpAddr){
            .family = net_Addr_Family_ip6,
            .ip6 = {
                .bytes = {0},
                .port = ntohs(ip6.sin6_port),
                .flow = ntohl(ip6.sin6_flowinfo),
                .scope_id = ip6.sin6_scope_id,
            },
        };
        raw_memcpy(result.ip6.bytes.val, &ip6.sin6_addr, 16);
        return_ok(result);
    }
    default_() return_err(E_cause$AddressFamilyUnsupportedNet()) $end(default);
    }
} $unscoped(fn);

$attr($maybe_unused)
$static fn_((net__setNonblocking(net_Handle socket, bool nonblocking))(E$void) $scope) {
    var mode = nonblocking ? 1ul : 0ul;
    if (ioctlsocket(socket, as$(long)(FIONBIO), &mode) != 0) {
        return_err(net__mapWinErr(WSAGetLastError()));
    }
    return_ok({});
} $unscoped(fn);

$attr($maybe_unused)
$static fn_((net__finishConnect(net_Handle socket))(E$void) $scope) {
    var_(so_error, i32) = 0;
    var_(opt_len, i32) = sizeOf$(i32);
    if (getsockopt(socket, SOL_SOCKET, SO_ERROR, ptrCast$((char*)(&so_error)), &opt_len) == SOCKET_ERROR) {
        return_err(net__mapWinErr(WSAGetLastError()));
    }
    if (so_error != 0) return_err(net__mapWinErr(so_error));
    return_ok({});
} $unscoped(fn);

#endif

#endif

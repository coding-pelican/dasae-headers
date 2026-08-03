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
    default_() return E_cause$net_SysResrcs() $end(default);
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
        return_ok((net_IpAddr)union_of((net_Addr_Family_ip4){
            .bytes = A_init({ raw[0], raw[1], raw[2], raw[3] }),
            .port = ntohs(ip4.sin_port),
        }));
    }
    case AF_INET6: {
        var_(ip6, SOCKADDR_IN6) = { 0 };
        raw_memcpy(&ip6, raw, sizeOf$(SOCKADDR_IN6));
        var result = (net_IpAddr)union_of((net_Addr_Family_ip6){
            .bytes = A_zero(),
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

#if plat_is_linux
#include "dh/sys/call/linux.h"

T_use_E$(sys_call_linux_sock_type_t);
T_use_E$(sys_call_linux_sock_protocol_t);
T_use_E$(sys_call_linux_sock_family_t);

T_alias$((net__linux_SockAddr)(struct net__linux_SockAddr {
    var_(storage, sys_call_linux_sockaddr_storage);
    var_(len, sys_call_linux_socklen_t);
}));
T_use_E$(net__linux_SockAddr);

$attr($maybe_unused)
$static fn_((net__linux_toNetwork16(u16 value))(u16)) {
    return pp_if_(arch_byte_order_is_little_endian)(
        pp_then_(raw_swapBytes16(value)),
        pp_else_(value));
};

$attr($maybe_unused)
$static fn_((net__linux_toNetwork32(u32 value))(u32)) {
    return pp_if_(arch_byte_order_is_little_endian)(
        pp_then_(raw_swapBytes32(value)),
        pp_else_(value));
};

$attr($maybe_unused)
$static fn_((net__linux_mapError(sys_call_linux_word err))(net_E)) {
    switch (err) {
    case 13: return E_cause$net_AccessDenied();
    case 98: return E_cause$net_AddressInUse();
    case 99: return E_cause$net_AddressUnavailable();
    case 103: return E_cause$net_ConnectionAborted();
    case 111: return E_cause$net_ConnectionRefused();
    case 104: return E_cause$net_ConnectionResetByPeer();
    case 113: return E_cause$net_HostUnreachable();
    case 90: return E_cause$net_MessageOversize();
    case 100: return E_cause$net_NetworkDown();
    case 101: return E_cause$net_NetworkUnreachable();
    case 107: return E_cause$net_NotConnected();
    case 110: return E_cause$net_TimedOut();
    case sys_call_linux_EAGAIN: $fallthrough;
    case sys_call_linux_EINPROGRESS: return E_cause$net_WouldBlock();
    default_() return E_cause$net_SysResrcs() $end(default);
    }
};

$attr($maybe_unused)
$static fn_((net__linux_ipFromSockAddr(net__linux_SockAddr addr))(E$net_IpAddr) $scope) {
    let raw = ptrCast$((const sys_call_linux_sockaddr*)(&addr.storage));
    let_ignore = addr.len;
    switch (raw->sa_family) {
    case sys_call_linux_AF_INET: {
        var_(ip4, sys_call_linux_sockaddr_in) = cleared();
        raw_memcpy(&ip4, raw, sizeOf$(sys_call_linux_sockaddr_in));
        var result = (net_IpAddr)union_of((net_Addr_Family_ip4){
            .bytes = A_zero(),
            .port = net__linux_toNetwork16(ip4.sin_port),
        });
        raw_memcpy(union_as((&result)(net_Addr_Family_ip4))->bytes.val, ip4.sin_addr.s_addr.val, 4);
        return_ok(result);
    }
    case sys_call_linux_AF_INET6: {
        var_(ip6, sys_call_linux_sockaddr_in6) = cleared();
        raw_memcpy(&ip6, raw, sizeOf$(sys_call_linux_sockaddr_in6));
        var result = (net_IpAddr)union_of((net_Addr_Family_ip6){
            .bytes = A_zero(),
            .port = net__linux_toNetwork16(ip6.sin6_port),
            .flow = net__linux_toNetwork32(ip6.sin6_flowinfo),
            .scope_id = ip6.sin6_scope_id,
        });
        raw_memcpy(union_as((&result)(net_Addr_Family_ip6))->bytes.val, ip6.sin6_addr.s6_addr.val, 16);
        return_ok(result);
    }
    default_() return_err(E_cause$net_AddressFamilyUnsupported()) $end(default);
    }
} $unscoped(fn);

$attr($maybe_unused)
$static fn_((net__linux_setNonblocking(net_Handle socket, bool nonblocking))(E$void) $scope) {
    let flags = sys_call_linux_fcntl(as$(sys_call_linux_fd_t)(socket), sys_call_linux_F_GETFL, 0);
    if (sys_call_linux_syscall_isErr(flags)) return_err(net__linux_mapError(sys_call_linux_syscall_err(flags)));
    let new_flags = nonblocking ? (flags | sys_call_linux_O_NONBLOCK) : (flags & ~sys_call_linux_O_NONBLOCK);
    let rc = sys_call_linux_fcntl(as$(sys_call_linux_fd_t)(socket), sys_call_linux_F_SETFL, new_flags);
    if (sys_call_linux_syscall_isErr(rc)) return_err(net__linux_mapError(sys_call_linux_syscall_err(rc)));
    return_ok({});
} $unscoped(fn);

$attr($maybe_unused)
$static fn_((net__linux_finishConnect(net_Handle socket))(E$void) $scope) {
    var_(so_error, i32) = 0;
    var_(opt_len, sys_call_linux_socklen_t) = sizeOf$(i32);
    let rc = sys_call_linux_getsockopt(
        (as$(sys_call_linux_fd_t)(socket)),
        sys_call_linux_SOL_SOCKET,
        sys_call_linux_SO_ERROR,
        &so_error, &opt_len
    );
    if (sys_call_linux_syscall_isErr(rc)) return_err(net__linux_mapError(sys_call_linux_syscall_err(rc)));
    if (so_error != 0) return_err(net__linux_mapError(so_error));
    return_ok({});
} $unscoped(fn);
#endif /* plat_is_linux */

#endif /* dh_net_private_share__included */

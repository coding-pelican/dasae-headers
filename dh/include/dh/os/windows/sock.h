/**
 * @copyright Copyright (c) 2025 Gyeongtae Kim
 * @license   MIT License - see LICENSE file for details
 *
 * @file    sock.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2025-11-26 (date of creation)
 * @updated 2025-12-31 (date of last update)
 * @ingroup dasae-headers(dh)/os/windows
 * @prefix  (none)
 *
 * @brief   Windows Sockets 2 (Winsock) APIs
 * @details Provides the Windows Sockets 2 interface for network programming.
 *          Supports protocol-independent communication, standard socket operations,
 *          and name resolution.
 *
 * API Overview:
 * - Setup: WSAStartup, WSACleanup
 * - Socket Creation: socket, closesocket
 * - Server (TCP): bind, listen, accept
 * - Client (TCP): connect
 * - Data Transfer (TCP): send, recv
 * - Data Transfer (UDP): sendto, recvfrom
 * - Options: setsockopt, getsockopt
 * - Error Handling: WSAGetLastError
 * - Name Resolution: getaddrinfo, freeaddrinfo, getnameinfo
 * - Address Conversion: inet_ntop, inet_pton, inet_addr [Deprecated], inet_ntoa [Deprecated]
 */
#ifndef os_windows_sock__included
#define os_windows_sock__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */
#include "base.h"
#include "pp-def/guard.h"
#if plat_is_windows

/// ==================================================================================
/// Windows Sockets 2 (Winsock) API
/// ==================================================================================
/// Provides a protocol-independent transport interface for network programming.
/// Note: Must call WSAStartup() before using any Winsock functions, and WSACleanup() when done.
///
/// Key APIs:
///
/// Initialization:
/// - int WSAStartup(WORD wVersionRequested, LPWSADATA lpWSAData);
///   Initializes the Winsock library.
///   @param wVersionRequested Version requested (use \b MAKEWORD(2,2) for version 2.2)
///   @param lpWSAData Receives WSADATA structure with library information
///   @return 0 on success, error code on failure
///   @note Must be called before using any Winsock functions.
///
/// - int WSACleanup(void);
///   Terminates use of the Winsock library.
///   @return 0 on success, \b SOCKET_ERROR on failure
///   @note Must be called when done with Winsock.
///
/// Socket Creation:
/// - SOCKET socket(int af, int type, int protocol);
///   Creates a socket.
///   @param af Address family: \b AF_INET (IPv4), \b AF_INET6 (IPv6), \b AF_UNSPEC (any)
///   @param type Socket type: \b SOCK_STREAM (TCP), \b SOCK_DGRAM (UDP), \b SOCK_RAW
///   @param protocol Protocol: \b IPPROTO_TCP, \b IPPROTO_UDP, 0 (auto-select)
///   @return Socket descriptor on success, \b INVALID_SOCKET on failure
///
/// - int closesocket(SOCKET s);
///   Closes a socket.
///   @param s Socket descriptor
///   @return 0 on success, \b SOCKET_ERROR on failure
///
/// Server Operations (TCP):
/// - int bind(SOCKET s, const struct sockaddr* name, int namelen);
///   Associates a local address with a socket.
///   @param s Socket descriptor
///   @param name Pointer to sockaddr structure (use sockaddr_in for IPv4)
///   @param namelen Length of sockaddr structure
///   @return 0 on success, \b SOCKET_ERROR on failure
///
/// - int listen(SOCKET s, int backlog);
///   Places a socket in a state where it listens for incoming connections.
///   @param s Socket descriptor (must be bound)
///   @param backlog Maximum length of pending connections queue
///   @return 0 on success, \b SOCKET_ERROR on failure
///
/// - SOCKET accept(SOCKET s, struct sockaddr* addr, int* addrlen);
///   Accepts a connection on a socket.
///   @param s Listening socket
///   @param addr Receives address of connecting entity (can be \b NULL)
///   @param addrlen Input: Size of addr, Output: Actual size
///   @return New socket for connection on success, \b INVALID_SOCKET on failure
///
/// Client Operations (TCP):
/// - int connect(SOCKET s, const struct sockaddr* name, int namelen);
///   Establishes a connection to a specified socket.
///   @param s Socket descriptor
///   @param name Pointer to sockaddr structure with destination address
///   @param namelen Length of sockaddr structure
///   @return 0 on success, \b SOCKET_ERROR on failure
///
/// Data Transmission (TCP):
/// - int send(SOCKET s, const char* buf, int len, int flags);
///   Sends data on a connected socket.
///   @param s Connected socket
///   @param buf Buffer containing data to send
///   @param len Length of data in bytes
///   @param flags Send options: 0, \b MSG_DONTROUTE, \b MSG_OOB
///   @return Number of bytes sent, \b SOCKET_ERROR on failure
///
/// - int recv(SOCKET s, char* buf, int len, int flags);
///   Receives data from a connected socket.
///   @param s Connected socket
///   @param buf Buffer to receive data
///   @param len Maximum length of buffer
///   @param flags Receive options: 0, \b MSG_PEEK, \b MSG_OOB, \b MSG_WAITALL
///   @return Number of bytes received, 0 if connection closed, \b SOCKET_ERROR on failure
///
/// Data Transmission (UDP):
/// - int sendto(SOCKET s, const char* buf, int len, int flags, const struct sockaddr* to, int tolen);
///   Sends data to a specific destination.
///   @param s Socket descriptor
///   @param buf Buffer containing data
///   @param len Length of data
///   @param flags Send options
///   @param to Destination address (sockaddr_in for IPv4)
///   @param tolen Length of destination address structure
///   @return Number of bytes sent, \b SOCKET_ERROR on failure
///
/// - int recvfrom(SOCKET s, char* buf, int len, int flags, struct sockaddr* from, int* fromlen);
///   Receives data and stores the source address.
///   @param s Socket descriptor
///   @param buf Buffer to receive data
///   @param len Maximum length of buffer
///   @param flags Receive options
///   @param from Receives source address (can be \b NULL)
///   @param fromlen Input: Size of from, Output: Actual size
///   @return Number of bytes received, \b SOCKET_ERROR on failure
///
/// Socket Options:
/// - int setsockopt(SOCKET s, int level, int optname, const char* optval, int optlen);
///   Sets a socket option.
///   @param s Socket descriptor
///   @param level Option level (\b SOL_SOCKET for socket-level options)
///   @param optname Option name: \b SO_REUSEADDR, \b SO_KEEPALIVE, \b SO_RCVBUF, \b SO_SNDBUF
///   @param optval Pointer to option value
///   @param optlen Size of option value
///   @return 0 on success, \b SOCKET_ERROR on failure
///
/// - int getsockopt(SOCKET s, int level, int optname, char* optval, int* optlen);
///   Retrieves a socket option.
///   @param s Socket descriptor
///   @param level Option level
///   @param optname Option name
///   @param optval Buffer to receive option value
///   @param optlen Input: Size of buffer, Output: Actual size
///   @return 0 on success, \b SOCKET_ERROR on failure
///
/// Error Handling:
/// - int WSAGetLastError(void);
///   Retrieves the last error code for the calling thread.
///   @return Error code (Winsock-specific, different from \b GetLastError)
///
/// Address Structures:
///   struct sockaddr_in {
///       short sin_family;        // AF_INET
///       unsigned short sin_port; // Port number (use htons() for host-to-network)
///       struct in_addr sin_addr; // IP address
///       char sin_zero[8];        // Padding
///   };
///
/// Documentation:
/// https://learn.microsoft.com/en-us/windows/win32/api/winsock2/
#include <winsock2.h>
/// ==================================================================================
/// Winsock Extension API
/// ==================================================================================
/// Provides advanced definitions and functions for IP address handling (IPv4/IPv6).
///
/// Key APIs:
///
/// Name Resolution:
/// - int getaddrinfo(const char* nodename, const char* servname, const struct addrinfo* hints, struct addrinfo** res);
///   Provides protocol-independent translation from host name to address.
///   @param nodename Host name or IP address string (\b NULL for local host)
///   @param servname Service name or port number string
///   @param hints Hints structure (can be \b NULL): \code{.c}
///     struct addrinfo {
///         int ai_flags;           // AI_PASSIVE, AI_CANONNAME, etc.
///         int ai_family;          // AF_INET, AF_INET6, AF_UNSPEC
///         int ai_socktype;        // SOCK_STREAM, SOCK_DGRAM
///         int ai_protocol;        // IPPROTO_TCP, IPPROTO_UDP
///         size_t ai_addrlen;      // Length of ai_addr
///         char* ai_canonname;     // Canonical name
///         struct sockaddr* ai_addr; // Socket address
///         struct addrinfo* ai_next; // Next structure
///     }; \endcode
///   @param res Receives linked list of addrinfo structures
///   @return 0 on success, error code on failure
///
/// - void freeaddrinfo(struct addrinfo* ai);
///   Frees the memory allocated by \b getaddrinfo().
///   @param ai Pointer to addrinfo structure
///
/// - int getnameinfo(const struct sockaddr* sa, socklen_t salen, char* host, size_t hostlen, char* serv, size_t servlen, int flags);
///   Provides protocol-independent name resolution from address to host name.
///   @param sa Socket address structure
///   @param salen Length of socket address
///   @param host Buffer for host name (can be \b NULL)
///   @param hostlen Size of host buffer
///   @param serv Buffer for service name (can be \b NULL)
///   @param servlen Size of serv buffer
///   @param flags Flags: \b NI_NUMERICHOST, \b NI_NUMERICSERV, \b NI_NAMEREQD, etc.
///   @return 0 on success, error code on failure
///
/// Address Conversion:
/// - const char* inet_ntop(int af, const void* src, char* dst, socklen_t size);
///   Converts a network address structure to a string.
///   @param af Address family (\b AF_INET or \b AF_INET6)
///   @param src Pointer to in_addr (IPv4) or in6_addr (IPv6)
///   @param dst Buffer to receive string
///   @param size Size of buffer (use \b INET_ADDRSTRLEN for IPv4, \b INET6_ADDRSTRLEN for IPv6)
///   @return Pointer to dst on success, \b NULL on failure
///
/// - int inet_pton(int af, const char* src, void* dst);
///   Converts an address string to a network address structure.
///   @param af Address family (\b AF_INET or \b AF_INET6)
///   @param src IP address string (e.g., "192.168.1.1")
///   @param dst Buffer to receive address structure
///   @return 1 on success, 0 on invalid format, -1 on error
///
/// Legacy Address Conversion (IPv4 only) [Deprecated]:
/// @warning These functions are deprecated. Use \b inet_pton and \b inet_ntop instead for IPv4/IPv6 support.
///
/// - unsigned long inet_addr(const char* cp);  [Deprecated]
///   Converts IPv4 address string to network byte order.
///   @param cp IP address string (e.g., "192.168.1.1")
///   @return Address on success, \b INADDR_NONE on failure
///   @deprecated Use \b inet_pton instead.
///
/// - char* inet_ntoa(struct in_addr in);  [Deprecated]
///   Converts IPv4 address to string.
///   @param in IPv4 address structure
///   @return Pointer to static buffer (not thread-safe)
///   @deprecated Use \b inet_ntop instead. This function is not thread-safe.
///
/// Documentation:
/// https://learn.microsoft.com/en-us/windows/win32/api/ws2tcpip/
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

#endif /* plat_is_windows */
#include "pp-def/unguarded.h"
#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* os_windows_sock__included */

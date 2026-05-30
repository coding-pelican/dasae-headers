#ifndef net_Sock__included
#define net_Sock__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "common.h"

/*========== Macros and Declarations ========================================*/

T_alias$((net_Sock_Handle)(net_Handle));
T_alias$((net_Sock_Flags)(struct net_Sock_Flags {
    var_(nonblocking, bool);
}));
$static let_(net_Sock_Flags_default, net_Sock_Flags) = {
    .nonblocking = false,
};
struct net_Sock {
    var_(handle, net_Sock_Handle);
    var_(addr, net_IpAddr);
    var_(flags, net_Sock_Flags);
};
T_impl_E$(net_Sock);
$attr($inline_always)
$static fn_((net_Sock_Handle_promote(net_Sock_Handle handle, net_IpAddr addr, net_Sock_Flags flags))(net_Sock));
$attr($inline_always)
$static fn_((net_Sock_handle(net_Sock self))(net_Sock_Handle));

/*========== External Definitions ===========================================*/

fn_((net_Sock_Handle_promote(net_Sock_Handle handle, net_IpAddr addr, net_Sock_Flags flags))(net_Sock)) {
    return (net_Sock){
        .handle = handle,
        .addr = addr,
        .flags = flags,
    };
};

fn_((net_Sock_handle(net_Sock self))(net_Sock_Handle)) {
    return self.handle;
};

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* net_Sock__included */

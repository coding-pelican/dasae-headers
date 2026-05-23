#include "dh/net/Clt.h"

fn_((net_Clt_connect(const net_IpAddr* addr, net_ConnectOpts opts))(E$net_Clt) $scope) {
    claim_assert_nonnull(addr);
    let stream = try_(net_connectIp(addr, opts));
    return_ok((net_Clt){
        .stream = stream,
        .remote = *addr,
    });
} $unscoped(fn);

fn_((net_Clt_close(net_Clt* self))(void)) {
    claim_assert_nonnull(self);
    net_Stream_close(self->stream);
    asg_l((self)(cleared()));
};

fn_((net_Clt_shutdown(net_Clt* self, net_ShutdownHow how))(E$void)) {
    claim_assert_nonnull(self);
    return net_Stream_shutdown(self->stream, how);
};

fn_((net_Clt_stream(net_Clt* self))(net_Stream*)) {
    claim_assert_nonnull(self);
    return &self->stream;
};

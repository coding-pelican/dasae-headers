#include "Svr.h"

fn_((net_Svr_close(net_Svr* self, net_Self net))(void)) {
    claim_assert_nonnull(self);
    claim_assert_nonnull(net.vtbl);
    net.vtbl->closeFn(net.ctx, self->socket);
    asg_l((self)(cleared()));
}

fn_((net_Svr_accept(net_Svr* self, net_Self net))(E$net_Stream)) {
    claim_assert_nonnull(self);
    claim_assert_nonnull(net.vtbl);
    return net.vtbl->acceptFn(net.ctx, self);
}

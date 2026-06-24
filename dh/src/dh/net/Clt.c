#include "dh/net/Clt.h"

fn_((net_Clt_connect(net_IpAddr addr, net_ConnectOpts opts))(E$net_Clt) $scope) {
    let stream = try_(net_connectIp(addr, opts));
    return_ok((net_Clt){
        .stream = stream,
        .remote = addr,
    });
} $unscoped(fn);

fn_((net_Clt_close(net_Clt self))(void)) {
    net_Stream_close(self.stream);
};

fn_((net_Clt_shutdown(net_Clt self, net_ShutdownHow how))(E$void)) {
    return net_Stream_shutdown(self.stream, how);
};

fn_((net_Clt_stream(net_Clt self))(net_Stream)) {
    return self.stream;
};

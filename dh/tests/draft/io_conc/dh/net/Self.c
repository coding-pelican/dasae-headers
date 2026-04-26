#include "Self.h"
#include "Stream.h"
#include "Svr.h"
#include "../exec/Coop.h"

/*========== Internal Definitions ===========================================*/

$static fn_((net_VTbl_failingListenIp(P$raw ctx, const net_IpAddr* addr, net_ListenOpts opts))(E$net_Svr) $scope) {
    let_ignore = ctx;
    let_ignore = addr;
    let_ignore = opts;
    return_err(net_E_Unsupported());
} $unscoped(fn);

$static fn_((net_VTbl_failingAccept(P$raw ctx, net_Svr* server))(E$net_Stream) $scope) {
    let_ignore = ctx;
    let_ignore = server;
    return_err(net_E_Unsupported());
} $unscoped(fn);

$static fn_((net_VTbl_failingBindIp(P$raw ctx, const net_IpAddr* addr, net_BindOpts opts))(E$net_Sock) $scope) {
    let_ignore = ctx;
    let_ignore = addr;
    let_ignore = opts;
    return_err(net_E_Unsupported());
} $unscoped(fn);

$static fn_((net_VTbl_failingConnectIp(P$raw ctx, const net_IpAddr* addr, net_ConnectOpts opts))(E$net_Stream) $scope) {
    let_ignore = ctx;
    let_ignore = addr;
    let_ignore = opts;
    return_err(net_E_Unsupported());
} $unscoped(fn);

$static fn_((net_VTbl_failingRead(P$raw ctx, net_Stream stream, S$u8 buf))(E$usize) $scope) {
    let_ignore = ctx;
    let_ignore = stream;
    let_ignore = buf;
    return_err(net_E_Unsupported());
} $unscoped(fn);

$static fn_((net_VTbl_failingWrite(P$raw ctx, net_Stream stream, S_const$u8 bytes))(E$usize) $scope) {
    let_ignore = ctx;
    let_ignore = stream;
    let_ignore = bytes;
    return_err(net_E_Unsupported());
} $unscoped(fn);

$static fn_((net_VTbl_noClose(P$raw ctx, net_Sock socket))(void)) {
    let_ignore = ctx;
    let_ignore = socket;
}

$static fn_((net_VTbl_failingShutdown(P$raw ctx, net_Stream stream, net_ShutdownHow how))(E$void) $scope) {
    let_ignore = ctx;
    let_ignore = stream;
    let_ignore = how;
    return_err(net_E_Unsupported());
} $unscoped(fn);

/*========== External Definitions ===========================================*/

let_(net_VTbl_failing, net_Self_VTbl) = {
    .listenIpFn = net_VTbl_failingListenIp,
    .acceptFn = net_VTbl_failingAccept,
    .bindIpFn = net_VTbl_failingBindIp,
    .connectIpFn = net_VTbl_failingConnectIp,
    .readFn = net_VTbl_failingRead,
    .writeFn = net_VTbl_failingWrite,
    .closeFn = net_VTbl_noClose,
    .shutdownFn = net_VTbl_failingShutdown,
};

$static var_(net_failing__ctx, Void) = cleared();
let_(net_failing, net_Self) = {
    .ctx = &net_failing__ctx,
    .vtbl = &net_VTbl_failing,
};

fn_((net_listenIp(net_Self self, const net_IpAddr* addr, net_ListenOpts opts))(E$net_Svr)) {
    claim_assert_nonnull(self.ctx);
    claim_assert_nonnull(self.vtbl);
    return self.vtbl->listenIpFn(self.ctx, addr, opts);
}

fn_((net_bindIp(net_Self self, const net_IpAddr* addr, net_BindOpts opts))(E$net_Sock)) {
    claim_assert_nonnull(self.ctx);
    claim_assert_nonnull(self.vtbl);
    return self.vtbl->bindIpFn(self.ctx, addr, opts);
}

fn_((net_connectIp(net_Self self, const net_IpAddr* addr, net_ConnectOpts opts))(E$net_Stream)) {
    claim_assert_nonnull(self.ctx);
    claim_assert_nonnull(self.vtbl);
    return self.vtbl->connectIpFn(self.ctx, addr, opts);
}

fn_((net_direct(void))(net_direct_E$net_Self) $scope) {
    return_err(net_direct_E_Unsupported());
} $unscoped(fn);

fn_((net_evented(exec_Coop* coop))(net_Self)) {
    claim_assert_nonnull(coop);
    let_ignore = exec_Coop_evented(coop);
    return net_failing;
}

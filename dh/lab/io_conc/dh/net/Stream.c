#include "Stream.h"

$static fn_((net_Stream_IO__read(P$raw ctx, S$u8 buf))(E$usize)) {
    let self = ptrCast$((net_Stream_IO*)(ensureNonnull(ctx)));
    return self->net.vtbl->readFn(self->net.ctx, self->stream, buf);
}

$static fn_((net_Stream_IO__write(P$raw ctx, S_const$u8 bytes))(E$usize)) {
    let self = ptrCast$((net_Stream_IO*)(ensureNonnull(ctx)));
    return self->net.vtbl->writeFn(self->net.ctx, self->stream, bytes);
}

fn_((net_Stream_close(net_Stream self, net_Self net))(void)) {
    claim_assert_nonnull(net.vtbl);
    net.vtbl->closeFn(net.ctx, self.socket);
}

fn_((net_Stream_shutdown(net_Stream self, net_Self net, net_ShutdownHow how))(E$void)) {
    claim_assert_nonnull(net.vtbl);
    return net.vtbl->shutdownFn(net.ctx, self, how);
}

fn_((net_Stream_io(net_Stream stream, net_Self net))(net_Stream_IO)) {
    claim_assert_nonnull(net.vtbl);
    return (net_Stream_IO){
        .net = net,
        .stream = stream,
    };
}

fn_((net_Stream_IO_reader(net_Stream_IO* self))(io_Reader)) {
    claim_assert_nonnull(self);
    return (io_Reader){
        .ctx = self,
        .read = net_Stream_IO__read,
    };
}

fn_((net_Stream_IO_writer(net_Stream_IO* self))(io_Writer)) {
    claim_assert_nonnull(self);
    return (io_Writer){
        .ctx = self,
        .write = net_Stream_IO__write,
    };
}

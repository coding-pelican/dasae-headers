#include "dh/io/Fixed.h"
#include "dh/io/common.h"
#include "dh/mem/common.h"

/* --- Fixed Stream ---*/

fn_((io_Fixed_reading(S_const$u8 buf))(io_Fixed)) {
    return (io_Fixed){
        .buf = buf,
        .pos = 0,
    };
};

fn_((io_Fixed_writing(S$u8 buf))(io_FixedMut)) {
    return (io_FixedMut){
        .buf = buf,
        .pos = 0,
    };
};

fn_((io_Fixed_written(io_FixedMut self))(S$u8)) {
    return S_prefix((self.buf)(self.pos));
};

fn_((io_Fixed_reset(io_Fixed* self))(void)) {
    self->pos = 0;
};

/* --- Fixed Stream Reader ---*/

fn_((io_Fixed_Reader_init(io_Fixed stream))(io_Fixed_Reader)) {
    return (io_Fixed_Reader){
        .stream = stream,
    };
};

$static fn_((io_Fixed_Reader__read(P$raw ctx, S$u8 output))(E$usize) $scope) {
    let self = ptrCast$((io_Fixed_Reader*)(ctx));
    let available = self->stream.buf.len - self->stream.pos;
    if (available == 0) { return_ok(0); }
    let to_read = prim_min(available, output.len);
    prim_memcpy(output.ptr, self->stream.buf.ptr + self->stream.pos, to_read);
    self->stream.pos += to_read;
    return_ok(to_read);
} $unscoped_(fn);

fn_((io_Fixed_reader(io_Fixed_Reader* self))(io_Reader)) {
    return (io_Reader){
        .ctx = ptrCast$((P$raw)(self)),
        .read = io_Fixed_Reader__read,
    };
};

/* --- Fixed Stream Writer ---*/

fn_((io_Fixed_Writer_init(io_FixedMut stream))(io_Fixed_Writer)) {
    return (io_Fixed_Writer){
        .stream = stream,
    };
};

$static fn_((io_Fixed_Writer__write(P$raw ctx, S_const$u8 bytes))(E$usize) $scope) {
    let self = ptrCast$((io_Fixed_Writer*)(ctx));
    if (bytes.len == 0) { return_ok(0); }
    if (self->stream.pos >= self->stream.buf.len) { return_err(io_Err_BufferTooSmall()); }
    let available = self->stream.buf.len - self->stream.pos;
    let to_write = prim_min(available, bytes.len);
    prim_memcpy(self->stream.buf.ptr + self->stream.pos, bytes.ptr, to_write);
    self->stream.pos += to_write;
    if (to_write == 0) { return_err(io_Err_BufferTooSmall()); }
    return_ok(to_write);
} $unscoped_(fn);

fn_((io_Fixed_writer(io_Fixed_Writer* self))(io_Writer)) {
    return (io_Writer){
        .ctx = ptrCast$((P$raw)(self)),
        .write = io_Fixed_Writer__write,
    };
};

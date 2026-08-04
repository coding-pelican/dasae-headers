#include "dh/io/Count.h"

/*========== Internal Declarations ==========================================*/

$attr($must_check)
$static fn_((io_Count_Reader__read(P$raw ctx, S$u8 out_buf))(io_ReadE$usize));
$attr($must_check)
$static fn_((io_Count_Writer__write(P$raw ctx, S_const$u8 bytes))(io_WriteE$usize));

/*========== External Definitions ===========================================*/

fn_((io_Count_Reader_from(io_Reader inner))(io_Count_Reader)) {
    return (io_Count_Reader){
        .inner = io_Reader_ensureValid(inner),
        .count = 0,
    };
};
fn_((io_Count_Reader_count(io_Count_Reader self))(u64)) {
    return self.count;
};
fn_((io_Count_reader(io_Count_Reader* self))(io_Reader)) {
    return (io_Reader){
        .ctx = ensureNonnull(self),
        .readFn = io_Count_Reader__read,
    };
};

fn_((io_Count_Writer_from(io_Writer inner))(io_Count_Writer)) {
    return (io_Count_Writer){
        .inner = io_Writer_ensureValid(inner),
        .count = 0,
    };
};
fn_((io_Count_Writer_count(io_Count_Writer self))(u64)) {
    return self.count;
};
fn_((io_Count_writer(io_Count_Writer* self))(io_Writer)) {
    return (io_Writer){
        .ctx = ensureNonnull(self),
        .writeFn = io_Count_Writer__write,
    };
};

/*========== Internal Definitions ===========================================*/

fn_((io_Count_Reader__read(P$raw ctx, S$u8 out_buf))(io_ReadE$usize) $scope) {
    let self = ptrAlignCast$((io_Count_Reader*)(ensureNonnull(ctx)));
    let bytes_read = try_(io_Reader_read(self->inner, out_buf));
    self->count = unwrap_(u64_addChkd(self->count, as$(u64)(bytes_read)));
    return_ok(bytes_read);
} $unscoped(fn);
fn_((io_Count_Writer__write(P$raw ctx, S_const$u8 bytes))(io_WriteE$usize) $scope) {
    let self = ptrAlignCast$((io_Count_Writer*)(ensureNonnull(ctx)));
    let written = try_(io_Writer_write(self->inner, bytes));
    self->count = unwrap_(u64_addChkd(self->count, as$(u64)(written)));
    return_ok(written);
} $unscoped(fn);

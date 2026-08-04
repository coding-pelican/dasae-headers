#include "dh/io/Limited.h"

/*========== Internal Declarations ==========================================*/

$attr($must_check)
$static fn_((io_Limited_Reader__read(P$raw ctx, S$u8 out_buf))(io_ReadE$usize));

/*========== External Definitions ===========================================*/

fn_((io_Limited_Reader_from(io_Reader inner, usize limit))(io_Limited_Reader)) {
    return (io_Limited_Reader){
        .inner = io_Reader_ensureValid(inner),
        .remaining = limit,
    };
};
fn_((io_Limited_Reader_remaining(io_Limited_Reader self))(usize)) {
    return self.remaining;
};
fn_((io_Limited_reader(io_Limited_Reader* self))(io_Reader)) {
    return (io_Reader){
        .ctx = ensureNonnull(self),
        .readFn = io_Limited_Reader__read,
    };
};

/*========== Internal Definitions ===========================================*/

fn_((io_Limited_Reader__read(P$raw ctx, S$u8 out_buf))(io_ReadE$usize) $scope) {
    let self = ptrAlignCast$((io_Limited_Reader*)(ensureNonnull(ctx)));
    if (self->remaining == 0) return_ok(0);
    let limited = S_prefix((out_buf)(pri_min(out_buf.len, self->remaining)));
    let bytes_read = try_(io_Reader_read(self->inner, limited));
    self->remaining -= bytes_read;
    return_ok(bytes_read);
} $unscoped(fn);

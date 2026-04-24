#include "Self.h"
#include "../../../../../include/dh/io/common.h"
#include "../../../../../include/dh/io/Writer.h"
#include "../../../../../include/dh/fs/File.h"

/*========== Internal Definitions ===========================================*/

$static fn_((io_direct__writer(io_Stream stream))(io_Writer)) {
    return fs_File_writer(stream == io_Stream_err ? io_getStdErr() : io_getStdOut());
};

$static fn_((io_direct__nl(P$raw ctx, io_Stream stream))(void)) {
    let_ignore = ctx;
    let_ignore = catch_((io_Writer_nl(io_direct__writer(stream)))($ignore, $do_nothing));
};

$static fn_((io_direct__printVaArgs(P$raw ctx, io_Stream stream, S_const$u8 fmt, va_list va_args))(void)) {
    let_ignore = ctx;
    let_ignore = catch_((io_Writer_printVaArgs(io_direct__writer(stream), fmt, va_args))($ignore, $do_nothing));
};

/*========== External Definitions ===========================================*/

fn_((io_direct(void))(io_Self)) {
    $static var_(ctx, Void) $like_ref = cleared();
    $static let_(vtbl, io_Self_VTbl) $like_ref = { {
        .nlFn = io_direct__nl,
        .printVaArgsFn = io_direct__printVaArgs,
    } };
    return (io_Self){
        .ctx = &ctx,
        .vtbl = vtbl,
    };
};

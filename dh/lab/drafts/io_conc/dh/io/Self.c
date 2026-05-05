#include "Self.h"
#include "common.h"
#include "Writer.h"
#include "../fs/File.h"
#include "../fs/Self.h"
#include "../exec/Coop.h"

/*========== Internal Declarations ==========================================*/

$static fn_((io_direct__nl(P$raw ctx, io_Stream stream))(void));
$static fn_((io_direct__printVaArgs(P$raw ctx, io_Stream stream, S_const$u8 fmt, va_list va_args))(void));

/*========== External Definitions ===========================================*/

let_(io_VTbl_noop, io_Self_VTbl) = {
    .nlFn = io_VTbl_noNL,
    .printVaArgsFn = io_VTbl_noPrintVaArgs,
};

let_(io_VTbl_failing, io_Self_VTbl) = {
    .nlFn = io_VTbl_unreachableNL,
    .printVaArgsFn = io_VTbl_unreachablePrintVaArgs,
};

$static var_(io_noop__ctx, Void) = cleared();
let_(io_noop, io_Self) = {
    .ctx = &io_noop__ctx,
    .vtbl = &io_VTbl_noop,
};

$static var_(io_failing__ctx, Void) = cleared();
let_(io_failing, io_Self) = {
    .ctx = &io_failing__ctx,
    .vtbl = &io_VTbl_failing,
};

T_alias$((io_direct__Ctx)(struct io_direct__Ctx {
    var_(fs, fs_Self);
    var_(out, fs_File);
    var_(err, fs_File);
}));
fn_((io_direct(void))(io_direct_E$io_Self) $scope) {
    $static var_(ctx, io_direct__Ctx) = cleared();
    $static let_(vtbl, io_Self_VTbl) $like_ref = { {
        .nlFn = io_direct__nl,
        .printVaArgsFn = io_direct__printVaArgs,
    } };
    $static var_(is_initialized, bool) = false;
    if (!is_initialized) {
        ctx.fs = try_(fs_direct());
        ctx.out = io_getStdOut();
        ctx.err = io_getStdErr();
        is_initialized = true;
    }
    return_ok(io_Self_ensureValid((io_Self){
        .ctx = &ctx,
        .vtbl = vtbl,
    }));
} $unscoped(fn);

fn_((io_evented(exec_Coop* coop))(io_Self)) {
    claim_assert_nonnull(coop);
    let_ignore = exec_Coop_evented(coop);
    let direct = catch_((io_direct())(
        $ignore, return io_failing
    ));
    return direct;
};

fn_((io_VTbl_noNL(P$raw ctx, io_Stream stream))(void)) {
    let_ignore = ctx;
    let_ignore = stream;
};

fn_((io_VTbl_unreachableNL(P$raw ctx, io_Stream stream))(void)) {
    let_ignore = ctx;
    let_ignore = stream;
    claim_unreachable_msg("");
};

fn_((io_VTbl_noPrintVaArgs(P$raw ctx, io_Stream stream, S_const$u8 fmt, va_list va_args))(void)) {
    let_ignore = ctx;
    let_ignore = stream;
    let_ignore = fmt;
    let_ignore = va_args;
};

fn_((io_VTbl_unreachablePrintVaArgs(P$raw ctx, io_Stream stream, S_const$u8 fmt, va_list va_args))(void)) {
    let_ignore = ctx;
    let_ignore = stream;
    let_ignore = fmt;
    let_ignore = va_args;
    claim_unreachable_msg("");
};

/*========== Internal Definitions ===========================================*/

$attr($inline_always)
$static fn_((io_direct__file(io_direct__Ctx* self, io_Stream stream))(fs_File)) {
    return stream == io_Stream_err ? self->err : self->out;
};

$attr($inline_always)
$static fn_((io_direct__fileIO(io_direct__Ctx* self, io_Stream stream))(fs_File_IO)) {
    return fs_File_io(io_direct__file(self, stream), self->fs);
};

$static fn_((io_direct__nl(P$raw ctx, io_Stream stream))(void)) {
    let self = ptrCast$((io_direct__Ctx*)(ensureNonnull(ctx)));
    var io = io_direct__fileIO(self, stream);
    catch_((io_Writer_nl(fs_File_IO_writer(&io)))(
        $ignore, $do_nothing
    ));
};

$static fn_((io_direct__printVaArgs(P$raw ctx, io_Stream stream, S_const$u8 fmt, va_list va_args))(void)) {
    let self = ptrCast$((io_direct__Ctx*)(ensureNonnull(ctx)));
    var io = io_direct__fileIO(self, stream);
    catch_((io_Writer_printVaArgs(fs_File_IO_writer(&io), fmt, va_args))(
        $ignore, $do_nothing
    ));
};

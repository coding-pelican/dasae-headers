#include "dh/log.h"

$static fn_((log__write(io_Writer writer, log_Level level, S_const$u8 scope, S_const$u8 fmt, va_list args))(io_PrintE$void) $scope) {
    try_(io_Writer_writeBytes(writer, log_Level_asText(level)));
    if (scope.len != 0) {
        try_(io_Writer_writeByte(writer, u8_c('(')));
        try_(io_Writer_writeBytes(writer, scope));
        try_(io_Writer_writeByte(writer, u8_c(')')));
    }
    try_(io_Writer_writeBytes(writer, u8_l(": ")));
    try_(io_Writer_printVaArgs(writer, fmt, args));
    try_(io_Writer_nl(writer));
    return_ok({});
} $unscoped(fn);



$static var_(log__ctx_noop, Void) $undefined_static;
$static let_(log__vtbl_noop, log_Self_VTbl) = {
    .enabledFn = log_Self_VTbl_noEnabled,
    .stampFn = log_Self_VTbl_noStamp,
};
let_(log_Self_noop, log_Self) = {
    .ctx = &log__ctx_noop,
    .vtbl = &log__vtbl_noop,
};

$static var_(log__ctx_failing, Void) $undefined_static;
$static let_(log__vtbl_failing, log_Self_VTbl) = {
    .enabledFn = log_Self_VTbl_unreachableEnabled,
    .stampFn = log_Self_VTbl_unreachableStamp,
};
let_(log_Self_failing, log_Self) = {
    .ctx = &log__ctx_failing,
    .vtbl = &log__vtbl_failing,
};

fn_((log_Level_asText(log_Level self))(S_const$u8)) {
    switch (self) {
    case log_Level_err: return u8_l("error");
    case log_Level_warn: return u8_l("warning");
    case log_Level_info: return u8_l("info");
    case log_Level_debug: return u8_l("debug");
    }
    claim_unreachable;
};

fn_((log_enabled(log_Self self, log_Level level, S_const$u8 scope))(bool)) {
    self = log_ensureValid(self);
    return self.vtbl->enabledFn(self.ctx, level, scope);
};

fn_((log_stampVaArgs(
    log_Self self,
    log_Level level,
    S_const$u8 scope,
    S_const$u8 fmt,
    va_list args
))(void)) {
    self = log_ensureValid(self);
    if (!self.vtbl->enabledFn(self.ctx, level, scope)) return;
    self.vtbl->stampFn(self.ctx, level, scope, fmt, args);
};

fn_((log_stamp(
    log_Self self,
    log_Level level,
    S_const$u8 scope,
    S_const$u8 fmt,
    ...
))(void)) {
    self = log_ensureValid(self);
    if (!self.vtbl->enabledFn(self.ctx, level, scope)) return;
    using_(var_(args, va_list) $undefined) using_fini_(va_start(args, fmt), va_end(args)) {
        self.vtbl->stampFn(self.ctx, level, scope, fmt, args);
    };
};

fn_((log_scopedErr(log_Self self, S_const$u8 scope, S_const$u8 fmt, ...))(void)) {
    using_(var_(args, va_list) $undefined) using_fini_(va_start(args, fmt), va_end(args)) {
        log_stampVaArgs(self, log_Level_err, scope, fmt, args);
    };
};
fn_((log_scopedWarn(log_Self self, S_const$u8 scope, S_const$u8 fmt, ...))(void)) {
    using_(var_(args, va_list) $undefined) using_fini_(va_start(args, fmt), va_end(args)) {
        log_stampVaArgs(self, log_Level_warn, scope, fmt, args);
    };
};
fn_((log_scopedInfo(log_Self self, S_const$u8 scope, S_const$u8 fmt, ...))(void)) {
    using_(var_(args, va_list) $undefined) using_fini_(va_start(args, fmt), va_end(args)) {
        log_stampVaArgs(self, log_Level_info, scope, fmt, args);
    };
};
fn_((log_scopedDebug(log_Self self, S_const$u8 scope, S_const$u8 fmt, ...))(void)) {
    using_(var_(args, va_list) $undefined) using_fini_(va_start(args, fmt), va_end(args)) {
        log_stampVaArgs(self, log_Level_debug, scope, fmt, args);
    };
};
fn_((log_err(log_Self self, S_const$u8 fmt, ...))(void)) {
    using_(var_(args, va_list) $undefined) using_fini_(va_start(args, fmt), va_end(args)) {
        log_stampVaArgs(self, log_Level_err, log_scope_default, fmt, args);
    };
};
fn_((log_warn(log_Self self, S_const$u8 fmt, ...))(void)) {
    using_(var_(args, va_list) $undefined) using_fini_(va_start(args, fmt), va_end(args)) {
        log_stampVaArgs(self, log_Level_warn, log_scope_default, fmt, args);
    };
};
fn_((log_info(log_Self self, S_const$u8 fmt, ...))(void)) {
    using_(var_(args, va_list) $undefined) using_fini_(va_start(args, fmt), va_end(args)) {
        log_stampVaArgs(self, log_Level_info, log_scope_default, fmt, args);
    };
};
fn_((log_debug(log_Self self, S_const$u8 fmt, ...))(void)) {
    using_(var_(args, va_list) $undefined) using_fini_(va_start(args, fmt), va_end(args)) {
        log_stampVaArgs(self, log_Level_debug, log_scope_default, fmt, args);
    };
};

$static fn_((log_Writer__ctx(P$raw ctx))(log_Writer*)) {
    return ptrCast$((log_Writer*)(ensureNonnull(ctx)));
};
$static fn_((log_Writer__enabled(
    P$raw ctx,
    log_Level level,
    S_const$u8 scope
))(bool)) {
    let_ignore = scope;
    return level <= log_Writer__ctx(ctx)->max_level;
};
$static fn_((log_Writer__log(
    P$raw ctx,
    log_Level level,
    S_const$u8 scope,
    S_const$u8 fmt,
    va_list args
))(void)) {
    let self = log_Writer__ctx(ctx);
    let_ignore = catch_((log__write(self->output, level, scope, fmt, args))(
        $ignore,
        $do_nothing
    ));
};
$static let_(log_Writer__vtbl, log_Self_VTbl) = {
    .enabledFn = log_Writer__enabled,
    .stampFn = log_Writer__log,
};

fn_((log_Writer_init(io_Writer output, log_Level max_level))(log_Writer)) {
    return (log_Writer){
        .output = io_Writer_ensureValid(output),
        .max_level = max_level,
    };
};
fn_((log_Writer_self(log_Writer* self))(log_Self)) {
    claim_assert_nonnull(self);
    return log_ensureValid((log_Self){
        .ctx = self,
        .vtbl = &log_Writer__vtbl,
    });
};

$static fn_((log_Default__ctx(P$raw ctx))(log_Default*)) {
    return ptrCast$((log_Default*)(ensureNonnull(ctx)));
};
$static fn_((log_Default__enabled(
    P$raw ctx,
    log_Level level,
    S_const$u8 scope
))(bool)) {
    let_ignore = scope;
    return level <= log_Default__ctx(ctx)->max_level;
};
$static fn_((log_Default__log(
    P$raw ctx,
    log_Level level,
    S_const$u8 scope,
    S_const$u8 fmt,
    va_list args
))(void) $guard) {
    let self = log_Default__ctx(ctx);
    io_lockStdErr(self->io);
    defer_(io_unlockStdErr(self->io));
    let writer = fs_File_IO_writer(&self->err_io);
    let_ignore = catch_((log__write(writer, level, scope, fmt, args))(
        $ignore,
        $do_nothing
    ));
} $unguarded(fn);
$static let_(log_Default__vtbl, log_Self_VTbl) = {
    .enabledFn = log_Default__enabled,
    .stampFn = log_Default__log,
};

fn_((log_Default_init(
    io_Self io,
    proc_std_Self std,
    log_Level max_level
))(log_Default)) {
    return (log_Default){
        .io = io_ensureValid(io),
        .err_io = fs_File_io(proc_std_err(proc_std_ensureValid(std))),
        .max_level = max_level,
    };
};
fn_((log_Default_self(log_Default* self))(log_Self)) {
    claim_assert_nonnull(self);
    return log_ensureValid((log_Self){
        .ctx = self,
        .vtbl = &log_Default__vtbl,
    });
};

fn_((log_Self_VTbl_noEnabled(P$raw ctx, log_Level level, S_const$u8 scope))(bool)) {
    claim_assert_nonnull(ctx);
    let_ignore = level;
    let_ignore = scope;
    return false;
};
fn_((log_Self_VTbl_unreachableEnabled(P$raw ctx, log_Level level, S_const$u8 scope))(bool)) {
    claim_assert_nonnull(ctx);
    let_ignore = level;
    let_ignore = scope;
    claim_unreachable;
};
fn_((log_Self_VTbl_noStamp(P$raw ctx, log_Level level, S_const$u8 scope, S_const$u8 fmt, va_list args))(void)) {
    claim_assert_nonnull(ctx);
    let_ignore = level;
    let_ignore = scope;
    let_ignore = fmt;
    let_ignore = args;
};
fn_((log_Self_VTbl_unreachableStamp(P$raw ctx, log_Level level, S_const$u8 scope, S_const$u8 fmt, va_list args))(void)) {
    claim_assert_nonnull(ctx);
    let_ignore = level;
    let_ignore = scope;
    let_ignore = fmt;
    let_ignore = args;
    claim_unreachable;
};

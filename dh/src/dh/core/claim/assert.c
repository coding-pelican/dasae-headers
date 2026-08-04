#include "dh/core/claim/assert.h"
#if claim_fail_printing_enabled
#include "dh/mem/common.h"
#include "dh/io/stream.h"

/*========== Extern Function Implementations ================================*/

$static let_(claim_assert__fmt_default, S_const$u8) = u8_l("Assertion failed: {:z}, in function {:z}, at file {:z}, line {:u}");
$static let_(claim_assert__fmt_msg, S_const$u8) = u8_l("Assertion failed: {:z}");
$static let_(claim_assert__fmt_expr, S_const$u8) = u8_l("`{:z}`");
$static let_(claim_assert__fmt_loc, S_const$u8) = u8_l("    in function {:z}, at file {:z}, line {:u}");

fn_((claim_assert_failLog(const char* expr, const char* func, const char* file, u32 line))(void) $guard) {
    let std = catch_((io_std_direct())($ignore, io_std_noop));
    var locked = io_std_lockErr(std);
    defer_(io_Locked_Writer_unlock(&locked));
    io_stream_eprintln(claim_assert__fmt_default, expr, func, file, line);
} $unguarded(fn);

fn_((claim_assert_failLogMsg(const char* expr, const char* func, const char* file, u32 line, const char* msg))(void) $guard) {
    let std = catch_((io_std_direct())($ignore, io_std_noop));
    var locked = io_std_lockErr(std);
    defer_(io_Locked_Writer_unlock(&locked));
    io_stream_eprint(claim_assert__fmt_msg, msg);
    io_stream_eprint(u8_l(" "));
    io_stream_eprintln(claim_assert__fmt_expr, expr);
    io_stream_eprintln(claim_assert__fmt_loc, func, file, line);
} $unguarded(fn);

fn_((claim_assert_failLogFmt(const char* expr, const char* func, const char* file, u32 line, const char* fmt, ...))(void) $guard) {
    let std = catch_((io_std_direct())($ignore, io_std_noop));
    var locked = io_std_lockErr(std);
    defer_(io_Locked_Writer_unlock(&locked));
    using_(var_(args, va_list) $undefined) using_fini_(va_start(args, fmt), va_end(args)) {
        io_stream_eprintVaArgs(mem_spanZ0Bytes(as$(const u8*)(fmt)), args);
    }
    io_stream_eprint(u8_l(" "));
    io_stream_eprintln(claim_assert__fmt_expr, expr);
    io_stream_eprintln(claim_assert__fmt_loc, func, file, line);
} $unguarded(fn);
#endif /* claim_fail_printing_enabled */

#include "dh/core/claim/assert.h"
#if claim_fail_logging_comp_enabled
#include "dh/mem/common.h"
#include "dh/io/stream.h"

/*========== Extern Function Implementations ================================*/

$static let_(claim_assert__fmt_default, S_const$u8) = u8_l("Assertion failed: {:z}, in function {:z}, at file {:z}, line {:d}");
$static let_(claim_assert__fmt_msg, S_const$u8) = u8_l("Assertion failed: {:z}");
$static let_(claim_assert__fmt_expr, S_const$u8) = u8_l("`{:z}`");
$static let_(claim_assert__fmt_loc, S_const$u8) = u8_l("    in function {:z}, at file {:z}, line {:d}");

fn_((claim_assert_failLog(const char* expr, const char* func, const char* file, i32 line))(void)) {
    io_stream_eprintln(claim_assert__fmt_default, expr, func, file, line);
};

fn_((claim_assert_failLogMsg(const char* expr, const char* func, const char* file, i32 line, const char* msg))(void)) {
    io_stream_eprint(claim_assert__fmt_msg, msg);
    io_stream_eprint(u8_l(" "));
    io_stream_eprintln(claim_assert__fmt_expr, expr);
    io_stream_eprintln(claim_assert__fmt_loc, func, file, line);
};

fn_((claim_assert_failLogFmt(const char* expr, const char* func, const char* file, i32 line, const char* fmt, ...))(void)) {
    with_(va_list args = null) with_fini_(va_start(args, fmt), va_end(args)) {
        io_stream_eprintVaArgs(mem_spanZ0$u8(as$(const u8*)(fmt)), args);
    }
    io_stream_eprint(u8_l(" "));
    io_stream_eprintln(claim_assert__fmt_expr, expr);
    io_stream_eprintln(claim_assert__fmt_loc, func, file, line);
};
#endif /* claim_fail_logging_comp_enabled */

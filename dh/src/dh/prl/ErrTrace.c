#include "dh/prl/ErrTrace.h"
#if ETrace_comp_enabled
#include "dh/io/common.h"
#include "dh/fs/File.h"

#if on_comptime
$static fn_((ETrace__instance(void))(ETrace*)) {
    $static $thrd_local var_(ETrace_s_instance, ETrace) = { .frames = {}, .len = 0 };
    return &ETrace_s_instance;
};

fn_((ETrace_reset_debug(void))(void)) {
    let trace = ETrace__instance();
    trace->len = 0;
};

fn_((ETrace_captureFrame_debug(SrcLoc src_loc, P$raw ret_addr))(void)) {
    let trace = ETrace__instance();
    if (trace->len < ETrace_max_frames) {
        asg_l((A_at((trace->frames)[trace->len++]))({
            .src_loc = src_loc,
            .ret_addr = ret_addr,
        }));
    }
};

fn_((ETrace_print_debug(void))(void)) {
    let trace = ETrace__instance();
    if (trace->len <= 0) { return; }

    let out = fs_File_writer(io_getStdErr());
    // catch_((io_Writer_print(out, u8_l("Error trace:\n")))($ignore, claim_unreachable));
    let frames = slice$A(trace->frames, $r(0, trace->len));
    for_(($s(frames))(frame)) {
        catch_((io_Writer_print(
            out, u8_l("    at {:z} ({:z}:{:u}:{:u})\n"),
            frame->src_loc.fn_name,
            frame->src_loc.file_name, frame->src_loc.line, frame->src_loc.column
        ))($ignore, claim_unreachable));
    } $end(for);
};
#endif /* on_comptime */
#endif /* ETrace_comp_enabled */

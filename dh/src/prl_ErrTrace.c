#include "dh/prl/ErrTrace.h"
#if ErrTrace_comp_enabled
#include "dh/io/common.h"
#include "dh/fs/File.h"

#if on_comptime
$static fn_((ErrTrace__instance(void))(ErrTrace*)) {
    $static $Thrd_local var_(ErrTrace_s_instance, ErrTrace) = { .frames = {}, .len = 0 };
    return &ErrTrace_s_instance;
}

fn_((ErrTrace_reset_debug(void))(void)) {
    let trace = ErrTrace__instance();
    trace->len = 0;
}

fn_((ErrTrace_captureFrame_debug(SrcLoc src_loc, P$raw ret_addr))(void)) {
    let trace = ErrTrace__instance();
    if (trace->len < ErrTrace_max_frames) {
        asg_lit((A_at((trace->frames)[trace->len++]))({
            .src_loc = src_loc,
            .ret_addr = ret_addr,
        }));
    }
}

fn_((ErrTrace_print_debug(void))(void)) {
    let trace = ErrTrace__instance();
    if (trace->len <= 0) { return; }

    let out = fs_File_writer(io_getStdErr());
    // catch_((io_Writer_print(out, u8_l("Error trace:\n")))($ignore, claim_unreachable));
    let frames = slice$A(trace->frames, $r(0, trace->len));
    for_(($s(frames))(frame) {
        catch_((io_Writer_print(
            out, u8_l("    at {:z} ({:z}:{:u}:{:u})\n"),
            frame->src_loc.fn_name,
            frame->src_loc.file_name, frame->src_loc.line, frame->src_loc.column
        ))($ignore, claim_unreachable));
    });
}
#endif /* on_comptime */
#endif /* ErrTrace_comp_enabled */

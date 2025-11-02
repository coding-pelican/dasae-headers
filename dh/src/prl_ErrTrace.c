#include "dh/prl/ErrTrace.h"
#include "dh/io/common.h"
#include "dh/fs/File.h"
#include "dh/io/Writer.h"

$static fn_((ErrTrace_instance(void))(ErrTrace*)) {
    $static ErrTrace s_trace = { .frames = {}, .len = 0 };
    return &s_trace;
}

fn_((ErrTrace_reset_debug(void))(void)) {
    let trace = ErrTrace_instance();
    trace->len = 0;
}

fn_((ErrTrace_captureFrame_debug(SrcLoc src_loc, P$raw ret_addr))(void)) {
    let trace = ErrTrace_instance();
    if (trace->len < ErrTrace_max_frames) {
        *at$A(trace->frames, trace->len++) = (ErrTrace_Frame){ .src_loc = src_loc, .ret_addr = ret_addr };
    }
}

fn_((ErrTrace_print_debug(void))(void)) {
    let trace = ErrTrace_instance();
    if (trace->len <= 0) { return; }

    let out = fs_File_writer(io_getStdErr());
    // catch_((io_Writer_print(out, u8_l("Error trace:\n")))($ignore, claim_unreachable));
    let frames = slice$A(trace->frames, $r(0, trace->len));
    for_(($s(frames))(frame) {
        catch_((io_Writer_print(
            out, u8_l("    at {:z} ({:z}:{:d}:{:d})\n"),
            frame->src_loc.fn_name,
            frame->src_loc.file_name, frame->src_loc.line, frame->src_loc.column
        ))($ignore, claim_unreachable));
    });
}

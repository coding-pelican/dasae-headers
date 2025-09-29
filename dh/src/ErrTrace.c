#include "dh/ErrTrace.h"
#include "dh/Str.h"
#include "dh/io/common.h"
#include "dh/fs/File.h"
#include "dh/io/Writer.h"

static fn_(instance(void), ErrTrace*) {
    static ErrTrace trace = { .frames = {}, .len = 0 };
    return &trace;
}

fn_(ErrTrace_reset_debug(void), void) {
    let trace  = instance();
    trace->len = 0;
}

fn_(ErrTrace_captureFrame_debug(SrcLoc src_loc, anyptr ret_addr), void) {
    let trace = instance();
    if (trace->len < ErrTrace_max_frames) {
        trace->frames[trace->len++] = (ErrTrace_Frame){ .src_loc = src_loc, .ret_addr = ret_addr };
    }
}

fn_(ErrTrace_print_debug(void), void) {
    let trace = instance();
    if (trace->len <= 0) { return; }

    let out = fs_File_writer(io_getStdErr());
    // catch_((io_Writer_print(out, u8_l("Error trace:\n")))($ignore_capture, claim_unreachable));
    for (usize i = 0; i < trace->len; ++i) {
        let frame = &trace->frames[i];
        catch_((io_Writer_print(
            out, u8_l("    at {:z} ({:z}:{:d}:{:d})\n"),
            frame->src_loc.fn_name,
            frame->src_loc.file_name, frame->src_loc.line, frame->src_loc.column
        ))($ignore_capture, claim_unreachable));
    }
}

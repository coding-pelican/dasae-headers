#include "dh/ErrTrace.h"

static ErrTrace* instance(void) {
    static ErrTrace trace = { .frames = {}, .len = 0 };
    return &trace;
}

void ErrTrace_reset_debug(void) {
    let trace  = instance();
    trace->len = 0;
}

void ErrTrace_captureFrame_debug(SrcLoc src_loc, anyptr ret_addr) {
    let trace = instance();
    if (trace->len < ErrTrace_max_frames) {
        trace->frames[trace->len++] = (ErrTrace_Frame){ .src_loc = src_loc, .ret_addr = ret_addr };
    }
}

void ErrTrace_print_debug(void) {
    let trace = instance();

    if (trace->len <= 0) { return; }

    ignore fprintf(stderr, "Error trace:\n");
    for (usize i = 0; i < trace->len; ++i) {
        let frame = &trace->frames[i];
        // clang-format off
        ignore fprintf(stderr, "  at %s (%s:%d:%d)\n",
            frame->src_loc.fn_name,
            frame->src_loc.file_name, frame->src_loc.line, frame->src_loc.column
        );
        // clang-format on
    }
}

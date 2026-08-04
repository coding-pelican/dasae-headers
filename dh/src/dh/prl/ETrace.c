#include "dh/prl/ETrace.h"
#if ETrace_enabled
#include "dh/mem/common.h"
#include "dh/io/stream.h"

$static fn_((ETrace__instance(void))(ETrace*)) {
    $attr(pp_if_(pp_not(plat_is_darwin)(pp_then_($thrd_local))))
    $static var_(instance, ETrace) = {
        .rest_frames = cleared(),
        .last_frame = none(),
        .depth = 0,
        .is_enabled = true,
    };
    return &instance;
};

fn_((ETrace_reset_debug(void))(void)) {
    let trace = ETrace__instance();
    mem_set0Bytes(mem_asBytesMut(u_anyP(&trace->rest_frames)));
    asg_l((&trace->last_frame)(none()));
    trace->depth = 0;
};

fn_((ETrace_isEnabled_debug(void))(bool)) {
    return ETrace__instance()->is_enabled;
};

fn_((ETrace_enable_debug(void))(void)) {
    ETrace__instance()->is_enabled = true;
};

fn_((ETrace_disable_debug(void))(void)) {
    ETrace__instance()->is_enabled = false;
};

fn_((ETrace_depth_debug(void))(usize)) {
    return ETrace__instance()->depth;
};

fn_((ETrace_captureFrame_debug(SrcLoc src_loc, P$raw ret_addr))(void)) {
    let trace = ETrace__instance();
    if (!trace->is_enabled) return;
    let_(frame, ETrace_Frame) = {
        .src_loc = src_loc,
        .ret_addr = ret_addr,
    };
    if (trace->depth < A_len(trace->rest_frames)) {
        asg_l((A_at((trace->rest_frames)[trace->depth]))(frame));
    } else {
        asg_l((&trace->last_frame)(some(frame)));
    }
    trace->depth++;
};

fn_((ETrace_print_debug(void))(void) $guard) {
    let trace = ETrace__instance();
    if (!trace->is_enabled) return;
    if (trace->depth == 0) return;
    let std = catch_((io_std_direct())($ignore, io_std_noop));
    var locked = io_std_lockErr(std);
    defer_(io_Locked_Writer_unlock(&locked));
    let writer = io_Locked_writer(&locked);
    {
        let rest_frames = A_prefix((trace->rest_frames)(pri_min(A_len(trace->rest_frames), trace->depth)));
        for_(($s(rest_frames))(frame)) catch_((io_Writer_println(
            writer, u8_l("    at {:z} ({:z}:{:u}:{:u})"),
            frame->src_loc.fn_name,
            frame->src_loc.file_name,
            frame->src_loc.line,
            frame->src_loc.column
        ))($ignore, $do_nothing)) $end(for);
        untie_((let last_frame, let captured_frames)local_({
            let last = orelse_((trace->last_frame)(return));
            let captured = rest_frames.len + 1;
            local_return_($tup((last), (captured)));
        }));
        let omitted_frames = trace->depth - captured_frames;
        if (omitted_frames > 0) catch_((io_Writer_println(
            writer, u8_l("    ... {:uz} omitted frames ..."), omitted_frames
        ))($ignore, $do_nothing));
        catch_((io_Writer_println(
            writer, u8_l("    at {:z} ({:z}:{:u}:{:u})"),
            last_frame.src_loc.fn_name,
            last_frame.src_loc.file_name,
            last_frame.src_loc.line,
            last_frame.src_loc.column
        ))($ignore, $do_nothing));
    }
} $unguarded(fn);
#endif /* ETrace_enabled */

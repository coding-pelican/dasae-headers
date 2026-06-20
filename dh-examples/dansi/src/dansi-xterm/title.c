#include "dansi-xterm/title.h"
#include "dansi-core/Seq.h"
#include <dh/io/Fixed.h>

$static fn_((dansi_xterm_title__set(u16 cmd, S_const$u8 title, S$u8 buf))(E$S$u8)) {
    return dansi_osc_make(cmd, title, buf);
};

$static fn_((dansi_xterm_title__setWrite(u16 cmd, S_const$u8 title, io_Writer out))(E$void)) {
    return dansi_osc_write(cmd, title, out);
};

$static fn_((dansi_xterm_title__parseReport(dansi_osc_Frame frame, u8 marker, dansi_xterm_title_Target target))(dansi_xterm_title_E$dansi_xterm_title_Report) $scope) {
    if (frame.payload.len == 0 || *S_at((frame.payload)[dansi_xterm_title_payload_marker_index]) != marker) {
        return_err(E_cause$dansi_xterm_title_InvalidResponse());
    }
    return_ok({
        .target = target,
        .text = S_suffix((frame.payload)(dansi_xterm_title_payload_text_start)),
    });
} $unscoped(fn);

fn_((dansi_xterm_title_setIcon(S_const$u8 title, S$u8 buf))(E$S$u8)) {
    return dansi_xterm_title__set(dansi_xterm_title_cmd_icon_u16, title, buf);
};

fn_((dansi_xterm_title_setIconWrite(S_const$u8 title, io_Writer out))(E$void)) {
    return dansi_xterm_title__setWrite(dansi_xterm_title_cmd_icon_u16, title, out);
};

fn_((dansi_xterm_title_setWindow(S_const$u8 title, S$u8 buf))(E$S$u8)) {
    return dansi_xterm_title__set(dansi_xterm_title_cmd_window_u16, title, buf);
};

fn_((dansi_xterm_title_setWindowWrite(S_const$u8 title, io_Writer out))(E$void)) {
    return dansi_xterm_title__setWrite(dansi_xterm_title_cmd_window_u16, title, out);
};

fn_((dansi_xterm_title_setBoth(S_const$u8 title, S$u8 buf))(E$S$u8)) {
    return dansi_xterm_title__set(dansi_xterm_title_cmd_both_u16, title, buf);
};

fn_((dansi_xterm_title_setBothWrite(S_const$u8 title, io_Writer out))(E$void)) {
    return dansi_xterm_title__setWrite(dansi_xterm_title_cmd_both_u16, title, out);
};

fn_((dansi_xterm_title_push(dansi_xterm_title_Target target, dansi_xterm_title_StackBuf* buf))(S$u8)) {
    var writing = io_Fixed_Writer_init(io_Fixed_writing(A_ref$((S$u8)(*buf))));
    catch_((dansi_xterm_title_pushWrite(target, io_Fixed_writer(&writing)))($ignore, claim_unreachable));
    return io_Fixed_written(writing.stream);
};

fn_((dansi_xterm_title_pushWrite(dansi_xterm_title_Target target, io_Writer out))(E$void)) {
    return io_Writer_print(
        out,
        u8_l(dansi_csi_make_static(
            "{:uh}" dansi_csi_param_sep "{:uhh}", "", dansi_xterm_title_report_final
        )),
        dansi_xterm_title_stack_push_op_u16, as$(u8)(target)
    );
};

fn_((dansi_xterm_title_pop(dansi_xterm_title_Target target, dansi_xterm_title_StackBuf* buf))(S$u8)) {
    var writing = io_Fixed_Writer_init(io_Fixed_writing(A_ref$((S$u8)(*buf))));
    catch_((dansi_xterm_title_popWrite(target, io_Fixed_writer(&writing)))($ignore, claim_unreachable));
    return io_Fixed_written(writing.stream);
};

fn_((dansi_xterm_title_popWrite(dansi_xterm_title_Target target, io_Writer out))(E$void)) {
    return io_Writer_print(
        out,
        u8_l(dansi_csi_make_static(
            "{:uh}" dansi_csi_param_sep "{:uhh}", "", dansi_xterm_title_report_final
        )),
        dansi_xterm_title_stack_pop_op_u16, as$(u8)(target)
    );
};

fn_((dansi_xterm_title_requestIconLabel(void))(S_const$u8)) {
    return u8_l(dansi_xterm_title_requestIconLabel_static());
};

fn_((dansi_xterm_title_requestIconLabelWrite(io_Writer out))(E$void)) {
    return io_Writer_writeBytes(out, dansi_xterm_title_requestIconLabel());
};

fn_((dansi_xterm_title_receiveIconLabelReport(io_Reader in, S$u8 buf))(E$S$u8)) {
    return dansi_Seq_receiveOSC(in, buf);
};

fn_((dansi_xterm_title_parseIconLabelReport(
    dansi_osc_Frame frame
))(dansi_xterm_title_E$dansi_xterm_title_Report)) {
    return dansi_xterm_title__parseReport(
        frame, dansi_xterm_title_icon_label_marker_byte, dansi_xterm_title_Target_icon
    );
};

fn_((dansi_xterm_title_fetchIconLabel(
    io_Writer out, io_Reader in, S$u8 buf
))(dansi_xterm_title_E$dansi_xterm_title_Report) $scope) {
    try_(dansi_xterm_title_requestIconLabelWrite(out));
    let bytes = try_(dansi_xterm_title_receiveIconLabelReport(in, buf));
    let frame = catch_((dansi_osc_parse(bytes.as_const))(
        $ignore, return_err(E_cause$dansi_xterm_title_InvalidResponse())
    ));
    return dansi_xterm_title_parseIconLabelReport(frame);
} $unscoped(fn);

fn_((dansi_xterm_title_requestWindowTitle(void))(S_const$u8)) {
    return u8_l(dansi_xterm_title_requestWindowTitle_static());
};

fn_((dansi_xterm_title_requestWindowTitleWrite(io_Writer out))(E$void)) {
    return io_Writer_writeBytes(out, dansi_xterm_title_requestWindowTitle());
};

fn_((dansi_xterm_title_receiveWindowTitleReport(io_Reader in, S$u8 buf))(E$S$u8)) {
    return dansi_Seq_receiveOSC(in, buf);
};

fn_((dansi_xterm_title_parseWindowTitleReport(
    dansi_osc_Frame frame
))(dansi_xterm_title_E$dansi_xterm_title_Report)) {
    return dansi_xterm_title__parseReport(
        frame, dansi_xterm_title_window_title_marker_byte, dansi_xterm_title_Target_window
    );
};

fn_((dansi_xterm_title_fetchWindowTitle(
    io_Writer out, io_Reader in, S$u8 buf
))(dansi_xterm_title_E$dansi_xterm_title_Report) $scope) {
    try_(dansi_xterm_title_requestWindowTitleWrite(out));
    let bytes = try_(dansi_xterm_title_receiveWindowTitleReport(in, buf));
    let frame = catch_((dansi_osc_parse(bytes.as_const))(
        $ignore, return_err(E_cause$dansi_xterm_title_InvalidResponse())
    ));
    return dansi_xterm_title_parseWindowTitleReport(frame);
} $unscoped(fn);

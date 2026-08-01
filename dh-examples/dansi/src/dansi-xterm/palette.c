#include "dansi-xterm/palette.h"
#include "dansi-core/Seq.h"
#include <dh/fmt/common.h>
#include <dh/io/Fixed.h>
#include <dh/mem/common.h>

$static fn_((dansi_xterm_palette__hex16(S_const$u8 text))(O$u16)) {
    if (text.len == 0) return none$((O$u16));
    return catch_none$((O$u16)(fmt_parse$u16(text, dansi_xterm_palette_rgb_component_radix)));
};

$static fn_((dansi_xterm_palette__stackAt(u8 index, u8 final, dansi_xterm_palette_StackBuf* buf))(S$u8)) {
    var writing = io_Fixed_Writer_from(io_Fixed_writing(A_ref$((S$u8)(*buf))));
    catch_((io_Writer_print(
        io_Fixed_writer(&writing),
        u8_l(dansi_csi_make_static("{:uhh}", dansi_xterm_palette_stack_intermediate, "{:c}")),
        index, final
    ))(
        $ignore, claim_unreachable
    ));
    return io_Fixed_written(writing.stream);
};

$static fn_((dansi_xterm_palette__stackAtWrite(u8 index, u8 final, io_Writer out))(io_PrintE$void)) {
    return io_Writer_print(
        out,
        u8_l(dansi_csi_make_static("{:uhh}", dansi_xterm_palette_stack_intermediate, "{:c}")),
        index, final
    );
};

fn_((dansi_xterm_palette_push(void))(S_const$u8)) {
    return u8_l(dansi_xterm_palette_push_static());
};

fn_((dansi_xterm_palette_pushWrite(io_Writer out))(io_WriteE$void)) {
    return io_Writer_writeBytes(out, dansi_xterm_palette_push());
};

fn_((dansi_xterm_palette_pushAt(u8 index, dansi_xterm_palette_StackBuf* buf))(S$u8)) {
    return dansi_xterm_palette__stackAt(index, dansi_xterm_palette_stack_push_final_byte, buf);
};

fn_((dansi_xterm_palette_pushAtWrite(u8 index, io_Writer out))(io_PrintE$void)) {
    return dansi_xterm_palette__stackAtWrite(index, dansi_xterm_palette_stack_push_final_byte, out);
};

fn_((dansi_xterm_palette_pop(void))(S_const$u8)) {
    return u8_l(dansi_xterm_palette_pop_static());
};

fn_((dansi_xterm_palette_popWrite(io_Writer out))(io_WriteE$void)) {
    return io_Writer_writeBytes(out, dansi_xterm_palette_pop());
};

fn_((dansi_xterm_palette_popAt(u8 index, dansi_xterm_palette_StackBuf* buf))(S$u8)) {
    return dansi_xterm_palette__stackAt(index, dansi_xterm_palette_stack_pop_final_byte, buf);
};

fn_((dansi_xterm_palette_popAtWrite(u8 index, io_Writer out))(io_PrintE$void)) {
    return dansi_xterm_palette__stackAtWrite(index, dansi_xterm_palette_stack_pop_final_byte, out);
};

fn_((dansi_xterm_palette_reportStack(void))(S_const$u8)) {
    return u8_l(dansi_xterm_palette_reportStack_static());
};

fn_((dansi_xterm_palette_reportStackWrite(io_Writer out))(io_WriteE$void)) {
    return io_Writer_writeBytes(out, dansi_xterm_palette_reportStack());
};

fn_((dansi_xterm_palette_reportStackAt(u8 index, dansi_xterm_palette_StackBuf* buf))(S$u8)) {
    return dansi_xterm_palette__stackAt(index, dansi_xterm_palette_stack_report_final_byte, buf);
};

fn_((dansi_xterm_palette_reportStackAtWrite(u8 index, io_Writer out))(io_PrintE$void)) {
    return dansi_xterm_palette__stackAtWrite(index, dansi_xterm_palette_stack_report_final_byte, out);
};

fn_((dansi_xterm_palette_setColor16(
    u8 index, dansi_xterm_color_RGB16 rgb, dansi_xterm_palette_SetColorBuf* buf
))(S$u8)) {
    var writing = io_Fixed_Writer_from(io_Fixed_writing(A_ref$((S$u8)(*buf))));
    catch_((dansi_xterm_palette_setColor16Write(index, rgb, io_Fixed_writer(&writing)))($ignore, claim_unreachable));
    return io_Fixed_written(writing.stream);
};

fn_((dansi_xterm_palette_setColor16Write(
    u8 index, dansi_xterm_color_RGB16 rgb, io_Writer out
))(io_PrintE$void) $scope) {
    var payload = (A$$(24, u8)){};
    var writing = io_Fixed_Writer_from(io_Fixed_writing(A_ref$((S$u8)(payload))));
    try_(io_Writer_print(
        io_Fixed_writer(&writing),
        u8_l(
            "{:uhh}" dansi_xterm_palette_payload_sep dansi_xterm_palette_rgb_prefix
            "{:04xh}" dansi_xterm_palette_rgb_sep "{:04xh}" dansi_xterm_palette_rgb_sep "{:04xh}"
        ),
        index, rgb.r, rgb.g, rgb.b
    ));
    return dansi_osc_write(dansi_xterm_palette_color_cmd_u16, io_Fixed_written(writing.stream).as_const, out);
} $unscoped(fn);

fn_((dansi_xterm_palette_setColor(
    u8 index, dansi_xterm_color_RGB8 rgb, dansi_xterm_palette_SetColorBuf* buf
))(S$u8)) {
    return dansi_xterm_palette_setColor16(index, dansi_xterm_color_RGB8_toRGB16(rgb), buf);
};

fn_((dansi_xterm_palette_setColorWrite(
    u8 index, dansi_xterm_color_RGB8 rgb, io_Writer out
))(io_PrintE$void)) {
    return dansi_xterm_palette_setColor16Write(index, dansi_xterm_color_RGB8_toRGB16(rgb), out);
};

fn_((dansi_xterm_palette_queryColor(u8 index, dansi_xterm_palette_QueryColorBuf* buf))(S$u8)) {
    var writing = io_Fixed_Writer_from(io_Fixed_writing(A_ref$((S$u8)(*buf))));
    catch_((dansi_xterm_palette_queryColorWrite(index, io_Fixed_writer(&writing)))($ignore, claim_unreachable));
    return io_Fixed_written(writing.stream);
};

fn_((dansi_xterm_palette_queryColorWrite(u8 index, io_Writer out))(io_PrintE$void) $scope) {
    var payload = (A$$(6, u8)){};
    var writing = io_Fixed_Writer_from(io_Fixed_writing(A_ref$((S$u8)(payload))));
    try_(io_Writer_print(
        io_Fixed_writer(&writing),
        u8_l("{:uhh}" dansi_xterm_palette_payload_sep dansi_xterm_palette_query),
        index
    ));
    return dansi_osc_write(dansi_xterm_palette_color_cmd_u16, io_Fixed_written(writing.stream).as_const, out);
} $unscoped(fn);

fn_((dansi_xterm_palette_receiveColorReport(io_Reader in, S$u8 buf))(E$S$u8)) {
    return dansi_Seq_receiveOSC(in, buf);
};

fn_((dansi_xterm_palette_parseColorReport(
    dansi_osc_Frame frame
))(dansi_xterm_palette_E$dansi_xterm_palette_ColorReport) $scope) {
    let split = orelse_((dansi_osc_Frame_splitCmd(frame))(
        return_err(E_cause$dansi_xterm_palette_InvalidResponse())
    ));
    let cmd = orelse_((dansi_osc_CmdSplit_cmdAsU16(split))(
        return_err(E_cause$dansi_xterm_palette_InvalidResponse())
    ));
    if (cmd != dansi_xterm_palette_color_cmd_u16) return_err(E_cause$dansi_xterm_palette_InvalidResponse());

    let payload = split.payload;
    let semi = orelse_((mem_findFirstUnitBytes(payload, dansi_xterm_palette_payload_sep_byte))(
        return_err(E_cause$dansi_xterm_palette_InvalidResponse())
    ));
    let index = orelse_((catch_none$((O$u8)(fmt_parse$u8(
        S_prefix((payload)(semi)), dansi_xterm_palette_index_radix
    ))))(
        return_err(E_cause$dansi_xterm_palette_InvalidResponse())
    ));
    let rgb = S_suffix((payload)(semi + 1));
    if (rgb.len < dansi_xterm_palette_rgb_prefix_len + 1
        || !mem_eqlBytes(S_prefix((rgb)(dansi_xterm_palette_rgb_prefix_len)), u8_l(dansi_xterm_palette_rgb_prefix))) {
        return_err(E_cause$dansi_xterm_palette_InvalidResponse());
    }
    let parts = S_suffix((rgb)(dansi_xterm_palette_rgb_prefix_len));
    let first = orelse_((mem_findFirstUnitBytes(parts, dansi_xterm_palette_rgb_sep_byte))(
        return_err(E_cause$dansi_xterm_palette_InvalidResponse())
    ));
    let rest = S_suffix((parts)(first + 1));
    let second = orelse_((mem_findFirstUnitBytes(rest, dansi_xterm_palette_rgb_sep_byte))(
        return_err(E_cause$dansi_xterm_palette_InvalidResponse())
    ));
    let r = orelse_((dansi_xterm_palette__hex16(S_prefix((parts)(first))))(
        return_err(E_cause$dansi_xterm_palette_InvalidResponse())
    ));
    let g = orelse_((dansi_xterm_palette__hex16(S_prefix((rest)(second))))(
        return_err(E_cause$dansi_xterm_palette_InvalidResponse())
    ));
    let b = orelse_((dansi_xterm_palette__hex16(S_suffix((rest)(second + 1))))(
        return_err(E_cause$dansi_xterm_palette_InvalidResponse())
    ));
    return_ok({
        .index = index,
        .rgb = { .r = r, .g = g, .b = b },
    });
} $unscoped(fn);

fn_((dansi_xterm_palette_fetchColor(
    u8 index, io_Writer out, io_Reader in, S$u8 buf
))(dansi_xterm_palette_E$dansi_xterm_palette_ColorReport) $scope) {
    try_(dansi_xterm_palette_queryColorWrite(index, out));
    let report = try_(dansi_xterm_palette_receiveColorReport(in, buf));
    let frame = catch_((dansi_osc_parse(report.as_const))(
        $ignore, return_err(E_cause$dansi_xterm_palette_InvalidResponse())
    ));
    return dansi_xterm_palette_parseColorReport(frame);
} $unscoped(fn);

fn_((dansi_xterm_palette_resetColor(u8 index, dansi_xterm_palette_ResetColorBuf* buf))(S$u8)) {
    var writing = io_Fixed_Writer_from(io_Fixed_writing(A_ref$((S$u8)(*buf))));
    catch_((dansi_xterm_palette_resetColorWrite(index, io_Fixed_writer(&writing)))($ignore, claim_unreachable));
    return io_Fixed_written(writing.stream);
};

fn_((dansi_xterm_palette_resetColorWrite(u8 index, io_Writer out))(io_PrintE$void)) {
    return io_Writer_print(out, u8_l(dansi_xterm_palette_resetColor_static("{:uhh}")), index);
};

fn_((dansi_xterm_palette_resetColors(S_const$u8 indices, S$u8 buf))(E$S$u8)) {
    return dansi_osc_make(dansi_xterm_palette_reset_color_cmd_u16, indices, buf);
};

fn_((dansi_xterm_palette_resetColorsWrite(S_const$u8 indices, io_Writer out))(io_PrintE$void)) {
    return dansi_osc_write(dansi_xterm_palette_reset_color_cmd_u16, indices, out);
};

fn_((dansi_xterm_palette_resetAllColors(void))(S_const$u8)) {
    return u8_l(dansi_xterm_palette_resetAllColors_static());
};

fn_((dansi_xterm_palette_resetAllColorsWrite(io_Writer out))(io_WriteE$void)) {
    return io_Writer_writeBytes(out, dansi_xterm_palette_resetAllColors());
};

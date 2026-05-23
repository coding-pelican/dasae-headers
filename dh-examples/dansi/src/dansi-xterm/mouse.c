#include "dansi-xterm/mouse.h"
#include <dh/fmt/common.h>
#include <dh/io/Fixed.h>
#include <dh/mem/common.h>

T_use$((u8)(
    mem_Delim,
    mem_TokzIter,
    mem_tokzAny,
    mem_TokzIter_next
));

fn_((dansi_mouse_enable(dansi_mouse_Mode mode, dansi_mouse_ModeBuf* buf))(S$u8)) {
    var writing = io_Fixed_Writer_init(io_Fixed_writing(A_ref$((S$u8)(*buf))));
    catch_((dansi_mouse_enableWrite(mode, io_Fixed_writer(&writing)))($ignore, claim_unreachable));
    return io_Fixed_written(writing.stream);
};

fn_((dansi_mouse_enableWrite(dansi_mouse_Mode mode, io_Writer writer))(E$void)) {
    return dansi_mode_enablePrivateWrite(as$(u16)(mode), writer);
};

fn_((dansi_mouse_disable(dansi_mouse_Mode mode, dansi_mouse_ModeBuf* buf))(S$u8)) {
    var writing = io_Fixed_Writer_init(io_Fixed_writing(A_ref$((S$u8)(*buf))));
    catch_((dansi_mouse_disableWrite(mode, io_Fixed_writer(&writing)))($ignore, claim_unreachable));
    return io_Fixed_written(writing.stream);
};

fn_((dansi_mouse_disableWrite(dansi_mouse_Mode mode, io_Writer writer))(E$void)) {
    return dansi_mode_disablePrivateWrite(as$(u16)(mode), writer);
};

fn_((dansi_mouse_set(dansi_mouse_Mode mode, bool enabled, dansi_mouse_ModeBuf* buf))(S$u8)) {
    var writing = io_Fixed_Writer_init(io_Fixed_writing(A_ref$((S$u8)(*buf))));
    catch_((dansi_mouse_setWrite(mode, enabled, io_Fixed_writer(&writing)))($ignore, claim_unreachable));
    return io_Fixed_written(writing.stream);
};

fn_((dansi_mouse_setWrite(dansi_mouse_Mode mode, bool enabled, io_Writer writer))(E$void)) {
    return dansi_mode_setPrivateWrite(as$(u16)(mode), enabled, writer);
};

fn_((dansi_mouse_enableAny(void))(S_const$u8)) {
    return u8_l(dansi_mouse_enableAny_static());
};

fn_((dansi_mouse_enableAnyWrite(io_Writer writer))(E$void)) {
    return io_Writer_writeBytes(writer, dansi_mouse_enableAny());
};

fn_((dansi_mouse_disableAny(void))(S_const$u8)) {
    return u8_l(dansi_mouse_disableAny_static());
};

fn_((dansi_mouse_disableAnyWrite(io_Writer writer))(E$void)) {
    return io_Writer_writeBytes(writer, dansi_mouse_disableAny());
};

fn_((dansi_mouse_enableSGR(void))(S_const$u8)) {
    return u8_l(dansi_mouse_enableSGR_static());
};

fn_((dansi_mouse_enableSGRWrite(io_Writer writer))(E$void)) {
    return io_Writer_writeBytes(writer, dansi_mouse_enableSGR());
};

fn_((dansi_mouse_disableSGR(void))(S_const$u8)) {
    return u8_l(dansi_mouse_disableSGR_static());
};

fn_((dansi_mouse_disableSGRWrite(io_Writer writer))(E$void)) {
    return io_Writer_writeBytes(writer, dansi_mouse_disableSGR());
};

$static fn_((dansi_mouse__button(u16 code))(dansi_mouse_Button)) {
    switch (code & 0x03) {
    case 0: return dansi_mouse_Button_left;
    case 1: return dansi_mouse_Button_middle;
    case 2: return dansi_mouse_Button_right;
    default: return dansi_mouse_Button_none;
    }
};

fn_((dansi_mouse_parseSGR(dansi_Seq raw))(dansi_mouse_E$dansi_mouse_Event) $scope) {
    if (raw.kind != dansi_Seq_Kind_csi || raw.bytes.len < 6) {
        return_err(E_cause$dansi_mouse_NotSGRSequence());
    }
    let payload = S_slice((raw.bytes)$r(2, raw.bytes.len));
    if (*S_at((payload)[0]) != '<') {
        return_err(E_cause$dansi_mouse_NotSGRSequence());
    }
    let final = *S_at((payload)[payload.len - 1]);
    if (final != 'M' && final != 'm') {
        return_err(E_cause$dansi_mouse_NotSGRSequence());
    }

    var it = mem_tokzAny$u8(S_slice((payload)$r(1, payload.len - 1)), u8_l(";"));
    let code_str = orelse_((mem_TokzIter_next$u8(&it))(return_err(E_cause$dansi_mouse_InvalidSGRSequence())));
    let x_str = orelse_((mem_TokzIter_next$u8(&it))(return_err(E_cause$dansi_mouse_InvalidSGRSequence())));
    let y_str = orelse_((mem_TokzIter_next$u8(&it))(return_err(E_cause$dansi_mouse_InvalidSGRSequence())));
    let code = catch_((fmt_parse$u16(code_str, 10))($ignore, return_err(E_cause$dansi_mouse_InvalidSGRSequence())));
    let x = catch_((fmt_parse$u16(x_str, 10))($ignore, return_err(E_cause$dansi_mouse_InvalidSGRSequence())));
    let y = catch_((fmt_parse$u16(y_str, 10))($ignore, return_err(E_cause$dansi_mouse_InvalidSGRSequence())));

    let wheel = expr_(dansi_mouse_Wheel $scope)(if ((code & 64) != 0) {
        $break_((code & 1) != 0 ? dansi_mouse_Wheel_down : dansi_mouse_Wheel_up);
    } else {
        $break_(dansi_mouse_Wheel_none);
    }) $unscoped(expr);
    let_(button, dansi_mouse_Button) = wheel != dansi_mouse_Wheel_none ? dansi_mouse_Button_none : dansi_mouse__button(code);
    let action = expr_(dansi_mouse_Action $scope)(if (wheel != dansi_mouse_Wheel_none) {
        $break_(dansi_mouse_Action_press);
    } else if (final == 'm') {
        $break_(dansi_mouse_Action_release);
    } else if ((code & 32) != 0) {
        $break_(button == dansi_mouse_Button_none ? dansi_mouse_Action_motion : dansi_mouse_Action_drag);
    } else {
        $break_(dansi_mouse_Action_press);
    }) $unscoped(expr);

    return_ok({
        .x = x,
        .y = y,
        .button = button,
        .action = action,
        .wheel = wheel,
        .mods = {
            .shift = (code & 4) != 0,
            .alt = (code & 8) != 0,
            .ctrl = (code & 16) != 0,
        },
    });
} $unscoped(fn);

#include "dansi-core/Event.h"
#include <dh/fmt/common.h>
#include <dh/io/common.h>
#include <dh/mem/common.h>
#include <dh/utf8.h>

/*========== Internal Declarations ==========================================*/

$static fn_((dansi_Event__special(dansi_key_Code code, dansi_key_Mods mods))(dansi_Event));
$static fn_((dansi_Event__text(u32 codepoint, dansi_key_Mods mods))(dansi_Event));
$static fn_((dansi_Event__mouse(dansi_Event_Mouse mouse))(dansi_Event));
$static fn_((dansi_Event__fromCodepoint(u32 codepoint, dansi_key_Mods mods))(dansi_Event));
$static fn_((dansi_Event__keyCodeFromCSIFinal(u8 ch))(O$dansi_key_Code));
$static fn_((dansi_Event__keyCodeFromTilde(u8 code))(O$dansi_key_Code));
$static fn_((dansi_Event__parseFocus(dansi_Seq seq))(dansi_Event_E$dansi_Event));
$static fn_((dansi_Event__parseMouse(dansi_Seq seq))(dansi_Event_E$dansi_Event));
$static fn_((dansi_Event__parseRaw(dansi_Seq seq))(dansi_Event_E$dansi_Event));
$static fn_((dansi_Event__parseEsc(dansi_Seq seq))(dansi_Event_E$dansi_Event));
$static fn_((dansi_Event__parseCSI(dansi_Seq seq))(dansi_Event_E$dansi_Event));
$static fn_((dansi_Event__parseCSI_u(S_const$u8 payload))(dansi_Event_E$dansi_Event));
$static fn_((dansi_Event__parseCSI_tilde(S_const$u8 payload))(dansi_Event_E$dansi_Event));
$static fn_((dansi_Event__parseCSI_modified(S_const$u8 payload))(dansi_Event_E$dansi_Event));
$static fn_((dansi_Event__parseSS3(dansi_Seq seq))(dansi_Event_E$dansi_Event));
$static fn_((dansi_Event__mouseBtn(u16 code))(O$dansi_mouse_Btn));
$static fn_((dansi_Event__mouseWheel(u16 code))(dansi_mouse_Wheel));

/*========== External Definitions ===========================================*/

fn_((dansi_Event_parse(dansi_Seq seq))(dansi_Event_E$dansi_Event) $scope) {
    if (seq.kind == dansi_Seq_Kind_csi) {
        if (seq.bytes.len == 3) {
            let ch = *S_at((seq.bytes)[2]);
            if (ch == 'I' || ch == 'O') return dansi_Event__parseFocus(seq);
        }
        if (seq.bytes.len >= 6) {
            let payload = S_slice((seq.bytes)$r(2, seq.bytes.len));
            if (*S_at((payload)[0]) == '<') return dansi_Event__parseMouse(seq);
        }
        return dansi_Event__parseCSI(seq);
    }
    $suppress_(switch_enum)(switch (seq.kind)) {
    case_((dansi_Seq_Kind_raw)) return dansi_Event__parseRaw(seq) $end(case);
    case_((dansi_Seq_Kind_esc)) return dansi_Event__parseEsc(seq) $end(case);
    case_((dansi_Seq_Kind_ss3)) return dansi_Event__parseSS3(seq) $end(case);
    default_() return_err(E_cause$dansi_Event_UnknownSeq()) $end(default);
    }
} $unscoped(fn);

fn_((dansi_Event_tryParse(dansi_Seq seq))(O$dansi_Event) $guard) {
    ETrace_disable();
    defer_(ETrace_enable());
    return_some(catch_((dansi_Event_parse(seq))($ignore, return_none())));
} $unguarded(fn);

/*========== Internal Definitions ===========================================*/

$static fn_((dansi_Event__special(dansi_key_Code code, dansi_key_Mods mods))(dansi_Event)) {
    return (dansi_Event)union_of((dansi_Event_special){
        .code = code,
        .mods = mods,
    });
};

$static fn_((dansi_Event__text(u32 codepoint, dansi_key_Mods mods))(dansi_Event)) {
    return (dansi_Event)union_of((dansi_Event_text){
        .codepoint = codepoint,
        .mods = mods,
    });
};

$static fn_((dansi_Event__mouse(dansi_Event_Mouse mouse))(dansi_Event)) {
    return (dansi_Event)union_of((dansi_Event_mouse)(mouse));
};

$static fn_((dansi_Event__fromCodepoint(u32 codepoint, dansi_key_Mods mods))(dansi_Event)) {
    switch (codepoint) {
    case_((0x0D)) return dansi_Event__special(dansi_key_Code_enter, mods) $end(case);
    case_((0x09)) return dansi_Event__special(dansi_key_Code_tab, mods) $end(case);
    case_((0x08)) return dansi_Event__special(dansi_key_Code_backspace, mods) $end(case);
    case_((0x7F)) return dansi_Event__special(dansi_key_Code_backspace, mods) $end(case);
    case_((0x1B)) return dansi_Event__special(dansi_key_Code_esc, mods) $end(case);
    default_() $do_nothing $end(default);
    }
    return dansi_Event__text(codepoint, mods);
};

$static fn_((dansi_Event__mouseBtn(u16 code))(O$dansi_mouse_Btn) $guard) {
    if ((code & 128) != 0) {
        switch (code & 0x03) {
        case 0: return_some(dansi_mouse_Btn_backward);
        case 1: return_some(dansi_mouse_Btn_forward);
        case 2: return_some(dansi_mouse_Btn_aux1);
        default: return_some(dansi_mouse_Btn_aux2);
        }
    }
    switch (code & 0x03) {
    case 0: return_some(dansi_mouse_Btn_left);
    case 1: return_some(dansi_mouse_Btn_middle);
    case 2: return_some(dansi_mouse_Btn_right);
    default: return_none();
    }
} $unguarded(fn);

$static fn_((dansi_Event__mouseWheel(u16 code))(dansi_mouse_Wheel)) {
    switch (code & 0x03) {
    case 0: return dansi_mouse_Wheel_up;
    case 1: return dansi_mouse_Wheel_down;
    case 2: return dansi_mouse_Wheel_right;
    default: return dansi_mouse_Wheel_left;
    }
};

$static fn_((dansi_Event__parseFocus(dansi_Seq seq))(dansi_Event_E$dansi_Event) $scope) {
    if (seq.bytes.len != 3) return_err(E_cause$dansi_Event_UnknownSeq());
    let ch = *S_at((seq.bytes)[2]);
    if (ch == 'I') return_ok(union_of((dansi_Event_focus)(dansi_Event_Focus_in)));
    if (ch == 'O') return_ok(union_of((dansi_Event_focus)(dansi_Event_Focus_out)));
    return_err(E_cause$dansi_Event_UnknownSeq());
} $unscoped(fn);

$static fn_((dansi_Event__parseMouse(dansi_Seq seq))(dansi_Event_E$dansi_Event) $scope) {
    if (seq.bytes.len < 6) return_err(E_cause$dansi_Event_UnknownSeq());
    let payload = S_slice((seq.bytes)$r(2, seq.bytes.len));
    if (*S_at((payload)[0]) != '<') return_err(E_cause$dansi_Event_UnknownSeq());
    let final = *S_at((payload)[payload.len - 1]);
    if (final != 'M' && final != 'm') return_err(E_cause$dansi_Event_UnknownSeq());

    var it = mem_tokzAnyBytes(S_slice((payload)$r(1, payload.len - 1)), u8_l(";"));
    let code_str = orelse_((mem_TokzIter_nextBytes(&it))(return_err(E_cause$dansi_Event_InvalidSeq())));
    let x_str = orelse_((mem_TokzIter_nextBytes(&it))(return_err(E_cause$dansi_Event_InvalidSeq())));
    let y_str = orelse_((mem_TokzIter_nextBytes(&it))(return_err(E_cause$dansi_Event_InvalidSeq())));
    let code = catch_((fmt_parse$u16(code_str, 10))($ignore, return_err(E_cause$dansi_Event_InvalidSeq())));
    let x = catch_((fmt_parse$u16(x_str, 10))($ignore, return_err(E_cause$dansi_Event_InvalidSeq())));
    let y = catch_((fmt_parse$u16(y_str, 10))($ignore, return_err(E_cause$dansi_Event_InvalidSeq())));

    let mods = (dansi_key_Mods){
        .shift = (code & 4) != 0,
        .alt = (code & 8) != 0,
        .ctrl = (code & 16) != 0,
    };

    if ((code & 64) != 0 && (code & 128) == 0) {
        let mouse = (dansi_Event_Mouse)union_of((dansi_Event_Mouse_wheel)((dansi_Event_MouseWheelReport){
            .x = x,
            .y = y,
            .wheel = dansi_Event__mouseWheel(code),
            .mods = mods,
        }));
        return_ok(dansi_Event__mouse(mouse));
    }

    if (final == 'm') {
        let btn = dansi_Event__mouseBtn(code);
        if_none((btn)) return_err(E_cause$dansi_Event_InvalidSeq());
        let report = (dansi_Event_MouseBtnReport){
            .x = x,
            .y = y,
            .btn = unwrap_(btn),
            .mods = mods,
        };
        let mouse = (dansi_Event_Mouse)union_of((dansi_Event_Mouse_release)(report));
        return_ok(dansi_Event__mouse(mouse));
    }

    let btn = dansi_Event__mouseBtn(code);
    if ((code & 32) != 0) {
        if_none((btn)) {
            let mouse = (dansi_Event_Mouse)union_of((dansi_Event_Mouse_motion)((dansi_Event_MouseMotionReport){
                .x = x,
                .y = y,
                .mods = mods,
            }));
            return_ok(dansi_Event__mouse(mouse));
        }
        let report = (dansi_Event_MouseBtnReport){
            .x = x,
            .y = y,
            .btn = unwrap_(btn),
            .mods = mods,
        };
        let mouse = (dansi_Event_Mouse)union_of((dansi_Event_Mouse_drag)(report));
        return_ok(dansi_Event__mouse(mouse));
    }
    if_none((btn)) return_err(E_cause$dansi_Event_InvalidSeq());
    let report = (dansi_Event_MouseBtnReport){
        .x = x,
        .y = y,
        .btn = unwrap_(btn),
        .mods = mods,
    };
    let mouse = (dansi_Event_Mouse)union_of((dansi_Event_Mouse_press)(report));
    return_ok(dansi_Event__mouse(mouse));
} $unscoped(fn);

$static fn_((dansi_Event__keyCodeFromCSIFinal(u8 ch))(O$dansi_key_Code) $guard) {
    switch (ch) {
    case_(('A')) return_some(dansi_key_Code_up) $end(case);
    case_(('B')) return_some(dansi_key_Code_down) $end(case);
    case_(('C')) return_some(dansi_key_Code_right) $end(case);
    case_(('D')) return_some(dansi_key_Code_left) $end(case);
    case_(('H')) return_some(dansi_key_Code_home) $end(case);
    case_(('F')) return_some(dansi_key_Code_end) $end(case);
    case_(('P')) return_some(dansi_key_Code_f1) $end(case);
    case_(('Q')) return_some(dansi_key_Code_f2) $end(case);
    case_(('R')) return_some(dansi_key_Code_f3) $end(case);
    case_(('S')) return_some(dansi_key_Code_f4) $end(case);
    default_() return_none() $end(default);
    }
} $unguarded(fn);

$static fn_((dansi_Event__keyCodeFromTilde(u8 code))(O$dansi_key_Code) $guard) {
    switch (code) {
    case_((1)) return_some(dansi_key_Code_home) $end(case);
    case_((2)) return_some(dansi_key_Code_insert) $end(case);
    case_((3)) return_some(dansi_key_Code_delete) $end(case);
    case_((4)) return_some(dansi_key_Code_end) $end(case);
    case_((5)) return_some(dansi_key_Code_page_up) $end(case);
    case_((6)) return_some(dansi_key_Code_page_down) $end(case);
    case_((11)) return_some(dansi_key_Code_f1) $end(case);
    case_((12)) return_some(dansi_key_Code_f2) $end(case);
    case_((13)) return_some(dansi_key_Code_f3) $end(case);
    case_((14)) return_some(dansi_key_Code_f4) $end(case);
    case_((15)) return_some(dansi_key_Code_f5) $end(case);
    case_((17)) return_some(dansi_key_Code_f6) $end(case);
    case_((18)) return_some(dansi_key_Code_f7) $end(case);
    case_((19)) return_some(dansi_key_Code_f8) $end(case);
    case_((20)) return_some(dansi_key_Code_f9) $end(case);
    case_((21)) return_some(dansi_key_Code_f10) $end(case);
    case_((23)) return_some(dansi_key_Code_f11) $end(case);
    case_((24)) return_some(dansi_key_Code_f12) $end(case);
    case_((25)) return_some(dansi_key_Code_fn(13)) $end(case);
    case_((26)) return_some(dansi_key_Code_fn(14)) $end(case);
    case_((28)) return_some(dansi_key_Code_fn(15)) $end(case);
    case_((29)) return_some(dansi_key_Code_fn(16)) $end(case);
    case_((31)) return_some(dansi_key_Code_fn(17)) $end(case);
    case_((32)) return_some(dansi_key_Code_fn(18)) $end(case);
    case_((33)) return_some(dansi_key_Code_fn(19)) $end(case);
    case_((34)) return_some(dansi_key_Code_fn(20)) $end(case);
    default_() return_none() $end(default);
    }
} $unguarded(fn);

$static fn_((dansi_Event__parseRaw(dansi_Seq seq))(dansi_Event_E$dansi_Event) $scope) {
    let first = *S_at((seq.bytes)[0]);
    switch (first) {
    cases_((0x08, 0x7F)(return_ok(dansi_Event__special(dansi_key_Code_backspace, dansi_key_modsNone())))) $end(cases);
    cases_((0x09)(return_ok(dansi_Event__special(dansi_key_Code_tab, dansi_key_modsNone())))) $end(cases);
    cases_((0x0A, 0x0D)(return_ok(dansi_Event__special(dansi_key_Code_enter, dansi_key_modsNone())))) $end(cases);
    case_((0x1B)) return_ok(dansi_Event__special(dansi_key_Code_esc, dansi_key_modsNone())) $end(case);
    default_() $do_nothing $end(default);
    }
    if ((0x01 <= first && first <= 0x0C) || (0x0E <= first && first <= 0x1A)) {
        return_ok(dansi_Event__text(
            as$(u32)(first - 0x01 + 'a'), (dansi_key_Mods){ .ctrl = 1 }));
    }
    let codepoint = catch_((utf8_decode(seq.bytes))($ignore, return_err(E_cause$dansi_Event_InvalidSeq())));
    return_ok(dansi_Event__fromCodepoint(codepoint, dansi_key_modsNone()));
} $unscoped(fn);

$static fn_((dansi_Event__parseEsc(dansi_Seq seq))(dansi_Event_E$dansi_Event) $scope) {
    if (seq.bytes.len == 1) return_ok(dansi_Event__special(dansi_key_Code_esc, dansi_key_modsNone()));
    if (seq.bytes.len == 2) {
        let nested = dansi_Seq_raw(S_suffix((seq.bytes)(1)));
        let parsed = try_(dansi_Event__parseRaw(nested));
        match_(parsed) {
        pattern_((dansi_Event_text)(text)) {
            let modified = local_({
                var text_mut = text;
                text_mut.mods.alt = true;
                local_return_(text_mut);
            });
            return_ok(union_of((dansi_Event_text)(modified)));
        } $end(pattern);
        pattern_((dansi_Event_special)(special)) {
            let modified = local_({
                var special_mut = special;
                special_mut.mods.alt = true;
                local_return_(special_mut);
            });
            return_ok(union_of((dansi_Event_special)(modified)));
        } $end(pattern);
        case dansi_Event_mouse: $fallthrough;
        case dansi_Event_focus: $fallthrough;
        default_() return_err(E_cause$dansi_Event_UnknownSeq()) $end(default);
        } $end(match);
    }
    return_err(E_cause$dansi_Event_InvalidSeq());
} $unscoped(fn);

$static fn_((dansi_Event__parseCSI_u(S_const$u8 payload))(dansi_Event_E$dansi_Event) $scope) {
    if (payload.len < 2 || *S_at((payload)[payload.len - 1]) != 'u') {
        return_err(E_cause$dansi_Event_InvalidSeq());
    }
    let body = S_slice((payload)$r(0, payload.len - 1));
    var it = mem_tokzAnyBytes(body, u8_l(";"));
    let code_str = orelse_((mem_TokzIter_nextBytes(&it))(return_err(E_cause$dansi_Event_InvalidSeq())));
    let mod_str = mem_TokzIter_nextBytes(&it);
    let codepoint = catch_((fmt_parse$u32(code_str, 10))($ignore, return_err(E_cause$dansi_Event_InvalidSeq())));
    var_(mods, dansi_key_Mods) = dansi_key_modsNone();
    if_some((mod_str)(str)) {
        let param = catch_((fmt_parse$u8(str, 10))($ignore, return_err(E_cause$dansi_Event_InvalidSeq())));
        mods = dansi_key_modsFromParam(param);
    }
    return_ok(dansi_Event__fromCodepoint(codepoint, mods));
} $unscoped(fn);

$static fn_((dansi_Event__parseCSI_tilde(S_const$u8 payload))(dansi_Event_E$dansi_Event) $scope) {
    if (payload.len < 2 || *S_at((payload)[payload.len - 1]) != '~') {
        return_err(E_cause$dansi_Event_InvalidSeq());
    }
    let body = S_slice((payload)$r(0, payload.len - 1));
    var it = mem_tokzAnyBytes(body, u8_l(";"));
    let code_str = orelse_((mem_TokzIter_nextBytes(&it))(return_err(E_cause$dansi_Event_InvalidSeq())));
    let mod_str = mem_TokzIter_nextBytes(&it);
    let code = catch_((fmt_parse$u8(code_str, 10))($ignore, return_err(E_cause$dansi_Event_InvalidSeq())));
    var_(mods, dansi_key_Mods) = dansi_key_modsNone();
    if_some((mod_str)(str)) {
        let param = catch_((fmt_parse$u8(str, 10))($ignore, return_err(E_cause$dansi_Event_InvalidSeq())));
        mods = dansi_key_modsFromParam(param);
    }
    let key_code = orelse_((dansi_Event__keyCodeFromTilde(code))(return_err(E_cause$dansi_Event_UnknownSeq())));
    return_ok(dansi_Event__special(key_code, mods));
} $unscoped(fn);

$static fn_((dansi_Event__parseCSI_modified(S_const$u8 payload))(dansi_Event_E$dansi_Event) $scope) {
    if (payload.len < 3 || *S_at((payload)[0]) != '1' || *S_at((payload)[1]) != ';') {
        return_err(E_cause$dansi_Event_InvalidSeq());
    }
    let final = *S_at((payload)[payload.len - 1]);
    let mod_str = S_slice((payload)$r(2, payload.len - 1));
    let param = catch_((fmt_parse$u8(mod_str, 10))($ignore, return_err(E_cause$dansi_Event_InvalidSeq())));
    let mods = dansi_key_modsFromParam(param);
    let key_code = orelse_((dansi_Event__keyCodeFromCSIFinal(final))(return_err(E_cause$dansi_Event_UnknownSeq())));
    return_ok(dansi_Event__special(key_code, mods));
} $unscoped(fn);

$static fn_((dansi_Event__parseCSI(dansi_Seq seq))(dansi_Event_E$dansi_Event) $scope) {
    if (seq.bytes.len < 3) return_err(E_cause$dansi_Event_InvalidSeq());
    let payload = S_slice((seq.bytes)$r(2, seq.bytes.len));

    if (payload.len >= 2 && *S_at((payload)[payload.len - 1]) == 'u') {
        return dansi_Event__parseCSI_u(payload);
    }
    if (payload.len >= 2 && *S_at((payload)[payload.len - 1]) == '~') {
        return dansi_Event__parseCSI_tilde(payload);
    }
    if (payload.len >= 3 && *S_at((payload)[0]) == '1' && *S_at((payload)[1]) == ';') {
        return dansi_Event__parseCSI_modified(payload);
    }
    if (payload.len == 1) {
        let key_code = orelse_((dansi_Event__keyCodeFromCSIFinal(*S_at((payload)[0])))(
            return_err(E_cause$dansi_Event_UnknownSeq())
        ));
        return_ok(dansi_Event__special(key_code, dansi_key_modsNone()));
    }
    return_err(E_cause$dansi_Event_UnknownSeq());
} $unscoped(fn);

$static fn_((dansi_Event__parseSS3(dansi_Seq seq))(dansi_Event_E$dansi_Event) $scope) {
    if (seq.bytes.len != 3) return_err(E_cause$dansi_Event_InvalidSeq());
    let key_code = orelse_((dansi_Event__keyCodeFromCSIFinal(*S_at((seq.bytes)[2])))(
        return_err(E_cause$dansi_Event_UnknownSeq())
    ));
    return_ok(dansi_Event__special(key_code, dansi_key_modsNone()));
} $unscoped(fn);

#include "dansi-dec/key.h"
#include <dh/io/Fixed.h>

$static fn_((dansi_dec_key__named(dansi_dec_key_Named key))(dansi_dec_key_Event)) {
    return union_of$((dansi_dec_key_Event)(dansi_dec_key_Event_named)(key));
};

$static fn_((dansi_dec_key__keypad(dansi_dec_key_Keypad key))(dansi_dec_key_Event)) {
    return union_of$((dansi_dec_key_Event)(dansi_dec_key_Event_keypad)(key));
};

$static fn_((dansi_dec_key__namedFromCursorFinal(u8 final))(O$dansi_dec_key_Named) $scope) {
    switch (final) {
    case_((u8_c('A'))) return_some(dansi_dec_key_Named_up) $end(case);
    case_((u8_c('B'))) return_some(dansi_dec_key_Named_down) $end(case);
    case_((u8_c('C'))) return_some(dansi_dec_key_Named_right) $end(case);
    case_((u8_c('D'))) return_some(dansi_dec_key_Named_left) $end(case);
    case_((u8_c('H'))) return_some(dansi_dec_key_Named_home) $end(case);
    case_((u8_c('F'))) return_some(dansi_dec_key_Named_end) $end(case);
    default_() return_none() $end(default);
    }
} $unscoped(fn);

$static fn_((dansi_dec_key__namedFromPFOrCursorFinal(u8 final))(O$dansi_dec_key_Named) $scope) {
    switch (final) {
    case_((u8_c('P'))) return_some(dansi_dec_key_Named_pf1) $end(case);
    case_((u8_c('Q'))) return_some(dansi_dec_key_Named_pf2) $end(case);
    case_((u8_c('R'))) return_some(dansi_dec_key_Named_pf3) $end(case);
    case_((u8_c('S'))) return_some(dansi_dec_key_Named_pf4) $end(case);
    default_() return dansi_dec_key__namedFromCursorFinal(final) $end(default);
    }
} $unscoped(fn);

$static fn_((dansi_dec_key__keypadFromFinal(u8 final))(O$dansi_dec_key_Keypad) $scope) {
    switch (final) {
    case_((u8_c('p'))) return_some(dansi_dec_key_Keypad_0) $end(case);
    case_((u8_c('q'))) return_some(dansi_dec_key_Keypad_1) $end(case);
    case_((u8_c('r'))) return_some(dansi_dec_key_Keypad_2) $end(case);
    case_((u8_c('s'))) return_some(dansi_dec_key_Keypad_3) $end(case);
    case_((u8_c('t'))) return_some(dansi_dec_key_Keypad_4) $end(case);
    case_((u8_c('u'))) return_some(dansi_dec_key_Keypad_5) $end(case);
    case_((u8_c('v'))) return_some(dansi_dec_key_Keypad_6) $end(case);
    case_((u8_c('w'))) return_some(dansi_dec_key_Keypad_7) $end(case);
    case_((u8_c('x'))) return_some(dansi_dec_key_Keypad_8) $end(case);
    case_((u8_c('y'))) return_some(dansi_dec_key_Keypad_9) $end(case);
    case_((u8_c('n'))) return_some(dansi_dec_key_Keypad_decimal) $end(case);
    case_((u8_c('m'))) return_some(dansi_dec_key_Keypad_minus) $end(case);
    case_((u8_c('l'))) return_some(dansi_dec_key_Keypad_comma) $end(case);
    case_((u8_c('M'))) return_some(dansi_dec_key_Keypad_enter) $end(case);
    default_() return_none() $end(default);
    }
} $unscoped(fn);

$static fn_((dansi_dec_key__namedFromTildeCode(u16 code))(O$dansi_dec_key_Named) $scope) {
    switch (code) {
    case_((1)) return_some(dansi_dec_key_Named_find) $end(case);
    case_((2)) return_some(dansi_dec_key_Named_insert) $end(case);
    case_((3)) return_some(dansi_dec_key_Named_remove) $end(case);
    case_((4)) return_some(dansi_dec_key_Named_select) $end(case);
    case_((5)) return_some(dansi_dec_key_Named_prev) $end(case);
    case_((6)) return_some(dansi_dec_key_Named_next) $end(case);
    case_((17)) return_some(dansi_dec_key_Named_f6) $end(case);
    case_((18)) return_some(dansi_dec_key_Named_f7) $end(case);
    case_((19)) return_some(dansi_dec_key_Named_f8) $end(case);
    case_((20)) return_some(dansi_dec_key_Named_f9) $end(case);
    case_((21)) return_some(dansi_dec_key_Named_f10) $end(case);
    case_((23)) return_some(dansi_dec_key_Named_f11) $end(case);
    case_((24)) return_some(dansi_dec_key_Named_f12) $end(case);
    case_((25)) return_some(dansi_dec_key_Named_f13) $end(case);
    case_((26)) return_some(dansi_dec_key_Named_f14) $end(case);
    case_((28)) return_some(dansi_dec_key_Named_f15) $end(case);
    case_((29)) return_some(dansi_dec_key_Named_f16) $end(case);
    case_((31)) return_some(dansi_dec_key_Named_f17) $end(case);
    case_((32)) return_some(dansi_dec_key_Named_f18) $end(case);
    case_((33)) return_some(dansi_dec_key_Named_f19) $end(case);
    case_((34)) return_some(dansi_dec_key_Named_f20) $end(case);
    case_((35)) return_some(dansi_dec_key_Named_f21) $end(case);
    case_((36)) return_some(dansi_dec_key_Named_f22) $end(case);
    case_((37)) return_some(dansi_dec_key_Named_f23) $end(case);
    case_((38)) return_some(dansi_dec_key_Named_f24) $end(case);
    default_() return_none() $end(default);
    }
} $unscoped(fn);

fn_((dansi_dec_key_setCursor(bool enabled, dansi_dec_key_SetCursorBuf* buf))(S$u8)) {
    return dansi_dec_mode_set(dansi_dec_mode_Code_cursor_keys, enabled, buf);
};

fn_((dansi_dec_key_setCursorWrite(bool enabled, io_Writer out))(E$void)) {
    return dansi_dec_mode_setWrite(dansi_dec_mode_Code_cursor_keys, enabled, out);
};

fn_((dansi_dec_key_enableCursor(dansi_dec_key_EnableCursorBuf* buf))(S$u8)) {
    return dansi_dec_mode_enable(dansi_dec_mode_Code_cursor_keys, buf);
};

fn_((dansi_dec_key_enableCursorWrite(io_Writer out))(E$void)) {
    return dansi_dec_mode_enableWrite(dansi_dec_mode_Code_cursor_keys, out);
};

fn_((dansi_dec_key_disableCursor(dansi_dec_key_DisableCursorBuf* buf))(S$u8)) {
    return dansi_dec_mode_disable(dansi_dec_mode_Code_cursor_keys, buf);
};

fn_((dansi_dec_key_disableCursorWrite(io_Writer out))(E$void)) {
    return dansi_dec_mode_disableWrite(dansi_dec_mode_Code_cursor_keys, out);
};

fn_((dansi_dec_key_setKeypad(bool enabled, dansi_dec_key_SetKeypadBuf* buf))(S$u8)) {
    var writing = io_Fixed_Writer_init(io_Fixed_writing(A_ref$((S$u8)(*buf))));
    catch_((dansi_dec_key_setKeypadWrite(enabled, io_Fixed_writer(&writing)))($ignore, claim_unreachable));
    return io_Fixed_written(writing.stream);
};

fn_((dansi_dec_key_setKeypadWrite(bool enabled, io_Writer out))(E$void)) {
    return enabled ? dansi_dec_key_enableKeypadWrite(out)
                   : dansi_dec_key_disableKeypadWrite(out);
};

fn_((dansi_dec_key_enableKeypad(void))(S_const$u8)) {
    return u8_l(dansi_dec_key_enableKeypad_static());
};

fn_((dansi_dec_key_enableKeypadWrite(io_Writer out))(E$void)) {
    return io_Writer_writeBytes(out, dansi_dec_key_enableKeypad());
};

fn_((dansi_dec_key_disableKeypad(void))(S_const$u8)) {
    return u8_l(dansi_dec_key_disableKeypad_static());
};

fn_((dansi_dec_key_disableKeypadWrite(io_Writer out))(E$void)) {
    return io_Writer_writeBytes(out, dansi_dec_key_disableKeypad());
};

fn_((dansi_dec_key_parseReport(dansi_Seq seq))(O$dansi_dec_key_Report) $scope) {
    switch (seq.kind) {
    case_((dansi_Seq_Kind_esc)) {
        let frame = catch_((dansi_esc_parse(seq.bytes))($ignore, return_none()));
        return_some(union_of((dansi_dec_key_Report_esc){
            .final = frame.final,
            .bytes = seq.bytes,
        }));
    } $end(case);
    case_((dansi_Seq_Kind_ss3)) {
        if (seq.bytes.len < 2) return_none();
        return_some(union_of((dansi_dec_key_Report_ss3){
            .final = *S_at((seq.bytes)[seq.bytes.len - 1]),
            .bytes = seq.bytes,
        }));
    } $end(case);
    case_((dansi_Seq_Kind_csi)) {
        let frame = catch_((dansi_csi_parse(seq.bytes))($ignore, return_none()));
        if (frame.final == u8_c('~')) {
            return_some(union_of((dansi_dec_key_Report_csi_tilde){ .frame = frame }));
        }
        return_some(union_of((dansi_dec_key_Report_csi){ .frame = frame }));
    } $end(case);
    default_((
        dansi_Seq_Kind_text,
        dansi_Seq_Kind_c0,
        dansi_Seq_Kind_c1,
        dansi_Seq_Kind_ss2,
        dansi_Seq_Kind_dcs,
        dansi_Seq_Kind_sos,
        dansi_Seq_Kind_osc,
        dansi_Seq_Kind_pm,
        dansi_Seq_Kind_apc
    )) return_none() $end(default);
    }
} $unscoped(fn);

fn_((dansi_dec_key_interpretReport(dansi_dec_key_Report report))(dansi_dec_key_E$dansi_dec_key_Event) $scope) {
    match_(report) {
    pattern_((dansi_dec_key_Report_esc)(esc)) {
        let named = orelse_((dansi_dec_key__namedFromCursorFinal(esc.final))(return_err(E_cause$dansi_dec_key_InvalidReport())));
        return_ok(dansi_dec_key__named(named));
    } $end(pattern);
    pattern_((dansi_dec_key_Report_ss3)(ss3)) {
        if_some((dansi_dec_key__namedFromPFOrCursorFinal(ss3.final))(named)) {
            return_ok(dansi_dec_key__named(named));
        }
        if_some((dansi_dec_key__keypadFromFinal(ss3.final))(keypad)) {
            return_ok(dansi_dec_key__keypad(keypad));
        }
        return_err(E_cause$dansi_dec_key_InvalidReport());
    } $end(pattern);
    pattern_((dansi_dec_key_Report_csi)(csi)) {
        let named = orelse_((dansi_dec_key__namedFromCursorFinal(csi.frame.final))(return_err(E_cause$dansi_dec_key_InvalidReport())));
        return_ok(dansi_dec_key__named(named));
    } $end(pattern);
    pattern_((dansi_dec_key_Report_csi_tilde)(tilde)) {
        let code = orelse_((dansi_csi_Frame_paramAtAsU16(tilde.frame, 0))(return_err(E_cause$dansi_dec_key_InvalidReport())));
        let named = orelse_((dansi_dec_key__namedFromTildeCode(code))(return_err(E_cause$dansi_dec_key_InvalidReport())));
        return_ok(dansi_dec_key__named(named));
    } $end(pattern);
    pattern_((dansi_dec_key_Report_vt52)(vt52)) {
        let named = orelse_((dansi_dec_key__namedFromCursorFinal(vt52.final))(return_err(E_cause$dansi_dec_key_InvalidReport())));
        return_ok(dansi_dec_key__named(named));
    } $end(pattern);
    } $end(match);
    return_err(E_cause$dansi_dec_key_InvalidReport());
} $unscoped(fn);

fn_((dansi_dec_key_parseEvent(dansi_Seq seq))(O$dansi_dec_key_Event) $scope) {
    let report = orelse_((dansi_dec_key_parseReport(seq))(return_none()));
    return catch_none$((O$dansi_dec_key_Event)(dansi_dec_key_interpretReport(report)));
} $unscoped(fn);

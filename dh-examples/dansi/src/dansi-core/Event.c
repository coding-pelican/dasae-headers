#include "dansi-core/Event.h"
#include <dh/io/common.h>
#include <dh/mem/common.h>
#include <dh/utf8.h>

$static fn_((dansi_Event__key(dansi_Event_KeyCode code))(dansi_Event)) {
    let key = (dansi_Event_Key){
        .code = code,
        .codepoint = 0,
        .mods = cleared(),
    };
    return (dansi_Event)union_of((dansi_Event_key)(key));
};

$static fn_((dansi_Event__parseText(dansi_Seq seq))(dansi_Event_E$dansi_Event) $scope) {
    let first = *S_at((seq.bytes)[0]);
    switch (first) {
    case 0x08:
    case 0x7F: return_ok(dansi_Event__key(dansi_Event_KeyCode_backspace));
    case 0x09: return_ok(dansi_Event__key(dansi_Event_KeyCode_tab));
    case 0x0A:
    case 0x0D: return_ok(dansi_Event__key(dansi_Event_KeyCode_enter));
    case 0x1B: return_ok(dansi_Event__key(dansi_Event_KeyCode_esc));
    default: break;
    }
    if ((0x01 <= first && first <= 0x0C) || (0x0E <= first && first <= 0x1A)) {
        let key = (dansi_Event_Key){
            .code = dansi_Event_KeyCode_char,
            .codepoint = as$(u32)(first - 0x01 + 'a'),
            .mods = { .ctrl = 1 },
        };
        return_ok(union_of((dansi_Event_key)(key)));
    }
    let codepoint = catch_((utf8_decode(seq.bytes))($ignore, return_err(E_cause$dansi_Event_InvalidSequence())));
    let key = (dansi_Event_Key){
        .code = dansi_Event_KeyCode_char,
        .codepoint = codepoint,
        .mods = cleared(),
    };
    return_ok(union_of((dansi_Event_key)(key)));
} $unscoped(fn);

$static fn_((dansi_Event__parseSS3(dansi_Seq seq))(dansi_Event_E$dansi_Event) $scope) {
    if (seq.bytes.len != 3) { return_err(E_cause$dansi_Event_InvalidSequence()); }
    switch (*S_at((seq.bytes)[2])) {
    case 'P': return_ok(dansi_Event__key(dansi_Event_KeyCode_f1));
    case 'Q': return_ok(dansi_Event__key(dansi_Event_KeyCode_f2));
    case 'R': return_ok(dansi_Event__key(dansi_Event_KeyCode_f3));
    case 'S': return_ok(dansi_Event__key(dansi_Event_KeyCode_f4));
    case 'H': return_ok(dansi_Event__key(dansi_Event_KeyCode_home));
    case 'F': return_ok(dansi_Event__key(dansi_Event_KeyCode_end));
    default: return_err(E_cause$dansi_Event_UnknownSequence());
    }
} $unscoped(fn);

$static fn_((dansi_Event__parseCSI(dansi_Seq seq))(dansi_Event_E$dansi_Event) $scope) {
    if (seq.bytes.len < 3) { return_err(E_cause$dansi_Event_InvalidSequence()); }
    let payload = S_slice((seq.bytes)$r(2, seq.bytes.len));
    if (payload.len == 1) {
        switch (*S_at((payload)[0])) {
        case 'A': return_ok(dansi_Event__key(dansi_Event_KeyCode_up));
        case 'B': return_ok(dansi_Event__key(dansi_Event_KeyCode_down));
        case 'C': return_ok(dansi_Event__key(dansi_Event_KeyCode_right));
        case 'D': return_ok(dansi_Event__key(dansi_Event_KeyCode_left));
        case 'H': return_ok(dansi_Event__key(dansi_Event_KeyCode_home));
        case 'F': return_ok(dansi_Event__key(dansi_Event_KeyCode_end));
        default: break;
        }
    }
    if (mem_eqlBytes(payload, u8_l("1~"))) { return_ok(dansi_Event__key(dansi_Event_KeyCode_home)); }
    if (mem_eqlBytes(payload, u8_l("2~"))) { return_ok(dansi_Event__key(dansi_Event_KeyCode_insert)); }
    if (mem_eqlBytes(payload, u8_l("3~"))) { return_ok(dansi_Event__key(dansi_Event_KeyCode_delete)); }
    if (mem_eqlBytes(payload, u8_l("4~"))) { return_ok(dansi_Event__key(dansi_Event_KeyCode_end)); }
    if (mem_eqlBytes(payload, u8_l("5~"))) { return_ok(dansi_Event__key(dansi_Event_KeyCode_page_up)); }
    if (mem_eqlBytes(payload, u8_l("6~"))) { return_ok(dansi_Event__key(dansi_Event_KeyCode_page_down)); }
    if (mem_eqlBytes(payload, u8_l("11~"))) { return_ok(dansi_Event__key(dansi_Event_KeyCode_f1)); }
    if (mem_eqlBytes(payload, u8_l("12~"))) { return_ok(dansi_Event__key(dansi_Event_KeyCode_f2)); }
    if (mem_eqlBytes(payload, u8_l("13~"))) { return_ok(dansi_Event__key(dansi_Event_KeyCode_f3)); }
    if (mem_eqlBytes(payload, u8_l("14~"))) { return_ok(dansi_Event__key(dansi_Event_KeyCode_f4)); }
    if (mem_eqlBytes(payload, u8_l("15~"))) { return_ok(dansi_Event__key(dansi_Event_KeyCode_f5)); }
    if (mem_eqlBytes(payload, u8_l("17~"))) { return_ok(dansi_Event__key(dansi_Event_KeyCode_f6)); }
    if (mem_eqlBytes(payload, u8_l("18~"))) { return_ok(dansi_Event__key(dansi_Event_KeyCode_f7)); }
    if (mem_eqlBytes(payload, u8_l("19~"))) { return_ok(dansi_Event__key(dansi_Event_KeyCode_f8)); }
    if (mem_eqlBytes(payload, u8_l("20~"))) { return_ok(dansi_Event__key(dansi_Event_KeyCode_f9)); }
    if (mem_eqlBytes(payload, u8_l("21~"))) { return_ok(dansi_Event__key(dansi_Event_KeyCode_f10)); }
    if (mem_eqlBytes(payload, u8_l("23~"))) { return_ok(dansi_Event__key(dansi_Event_KeyCode_f11)); }
    if (mem_eqlBytes(payload, u8_l("24~"))) { return_ok(dansi_Event__key(dansi_Event_KeyCode_f12)); }
    return_err(E_cause$dansi_Event_UnknownSequence());
} $unscoped(fn);

fn_((dansi_Event_parse(dansi_Seq seq))(dansi_Event_E$dansi_Event) $scope) {
    switch (seq.kind) {
    case dansi_Seq_Kind_text: return dansi_Event__parseText(seq);
    case dansi_Seq_Kind_esc:
        if (seq.bytes.len == 1) { return_ok(dansi_Event__key(dansi_Event_KeyCode_esc)); }
        if (seq.bytes.len == 2) {
            let nested = (dansi_Seq){ .kind = dansi_Seq_Kind_text, .bytes = S_suffix((seq.bytes)(1)) };
            let parsed = try_(dansi_Event__parseText(nested));
            match_(parsed) {
            pattern_((dansi_Event_key)(key)) {
                var_(key_mut, dansi_Event_Key) = key;
                key_mut.mods.alt = 1;
                return_ok(union_of((dansi_Event_key)(key_mut)));
            } $end(pattern);
            default_() return_err(E_cause$dansi_Event_UnknownSequence()) $end(default);
            } $end(match);
        }
        return_err(E_cause$dansi_Event_InvalidSequence());
    case dansi_Seq_Kind_csi: return dansi_Event__parseCSI(seq);
    case dansi_Seq_Kind_ss3: return dansi_Event__parseSS3(seq);
    default: return_err(E_cause$dansi_Event_UnknownSequence());
    }
} $unscoped(fn);

fn_((dansi_Event_tryParse(dansi_Seq seq))(O$dansi_Event) $scope) {
    return_some(catch_((dansi_Event_parse(seq))($ignore, return_none())));
} $unscoped(fn);

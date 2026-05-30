#include "dansi-core/Event.h"
#include <dh/io/common.h>
#include <dh/mem/common.h>
#include <dh/utf8.h>

/*========== Internal Declarations ==========================================*/

$static fn_((dansi_Event__pureKey(dansi_Event_KeyCode code))(dansi_Event));
$static fn_((dansi_Event__parseRaw(dansi_Seq seq))(dansi_Event_E$dansi_Event));
$static fn_((dansi_Event__parseEsc(dansi_Seq seq))(dansi_Event_E$dansi_Event));
$static fn_((dansi_Event__parseCSI(dansi_Seq seq))(dansi_Event_E$dansi_Event));
$static fn_((dansi_Event__parseSS3(dansi_Seq seq))(dansi_Event_E$dansi_Event));

/*========== External Definitions ===========================================*/

fn_((dansi_Event_parse(dansi_Seq seq))(dansi_Event_E$dansi_Event) $scope) {
    switch ($suppress_(switch_enum)(seq.kind)) {
    case_((dansi_Seq_Kind_raw)) return dansi_Event__parseRaw(seq) $end(case);
    case_((dansi_Seq_Kind_esc)) return dansi_Event__parseEsc(seq) $end(case);
    case_((dansi_Seq_Kind_csi)) return dansi_Event__parseCSI(seq) $end(case);
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

$static fn_((dansi_Event__pureKey(dansi_Event_KeyCode code))(dansi_Event)) {
    return (dansi_Event)union_of((dansi_Event_key){
        .code = code,
        .codepoint = 0,
        .mods = cleared(),
    });
};

$static fn_((dansi_Event__parseRaw(dansi_Seq seq))(dansi_Event_E$dansi_Event) $scope) {
    let first = *S_at((seq.bytes)[0]);
    switch (first) {
    cases_((0x08, 0x7F)(return_ok(dansi_Event__pureKey(dansi_Event_KeyCode_backspace)))) $end(cases);
    cases_((0x09)(return_ok(dansi_Event__pureKey(dansi_Event_KeyCode_tab)))) $end(cases);
    cases_((0x0A, 0x0D)(return_ok(dansi_Event__pureKey(dansi_Event_KeyCode_enter)))) $end(cases);
    case_((0x1B)) return_ok(dansi_Event__pureKey(dansi_Event_KeyCode_esc)) $end(case);
    default_() $do_nothing $end(default);
    }
    if ((0x01 <= first && first <= 0x0C) || (0x0E <= first && first <= 0x1A)) return_ok(union_of((dansi_Event_key){
        .code = dansi_Event_KeyCode_char,
        .codepoint = as$(u32)(first - 0x01 + 'a'),
        .mods = { .ctrl = 1 },
    }));
    let codepoint = catch_((utf8_decode(seq.bytes))($ignore, return_err(E_cause$dansi_Event_InvalidSeq())));
    return_ok(union_of((dansi_Event_key){
        .code = dansi_Event_KeyCode_char,
        .codepoint = codepoint,
        .mods = cleared(),
    }));
} $unscoped(fn);

$static fn_((dansi_Event__parseEsc(dansi_Seq seq))(dansi_Event_E$dansi_Event) $scope) {
    if (seq.bytes.len == 1) return_ok(dansi_Event__pureKey(dansi_Event_KeyCode_esc));
    if (seq.bytes.len == 2) {
        let nested = dansi_Seq_raw(S_suffix((seq.bytes)(1)));
        let parsed = try_(dansi_Event__parseRaw(nested));
        match_(parsed) {
        pattern_((dansi_Event_key)(key)) {
            let modified_key = local_({
                var key_mut = key;
                key_mut.mods.alt = true;
                local_return_(key_mut);
            });
            return_ok(union_of((dansi_Event_key)(modified_key)));
        } $end(pattern);
        default_() return_err(E_cause$dansi_Event_UnknownSeq()) $end(default);
        } $end(match);
    }
    return_err(E_cause$dansi_Event_InvalidSeq());
} $unscoped(fn);

$static fn_((dansi_Event__parseCSI(dansi_Seq seq))(dansi_Event_E$dansi_Event) $scope) {
    if (seq.bytes.len < 3) return_err(E_cause$dansi_Event_InvalidSeq());
    let payload = S_slice((seq.bytes)$r(2, seq.bytes.len));
    if (payload.len == 1) {
        switch (*S_at((payload)[0])) {
        case_(('A')) return_ok(dansi_Event__pureKey(dansi_Event_KeyCode_up)) $end(case);
        case_(('B')) return_ok(dansi_Event__pureKey(dansi_Event_KeyCode_down)) $end(case);
        case_(('C')) return_ok(dansi_Event__pureKey(dansi_Event_KeyCode_right)) $end(case);
        case_(('D')) return_ok(dansi_Event__pureKey(dansi_Event_KeyCode_left)) $end(case);
        case_(('H')) return_ok(dansi_Event__pureKey(dansi_Event_KeyCode_home)) $end(case);
        case_(('F')) return_ok(dansi_Event__pureKey(dansi_Event_KeyCode_end)) $end(case);
        default_() $do_nothing $end(default);
        }
    }
    if (mem_eqlBytes(payload, u8_l("1~"))) return_ok(dansi_Event__pureKey(dansi_Event_KeyCode_home));
    if (mem_eqlBytes(payload, u8_l("2~"))) return_ok(dansi_Event__pureKey(dansi_Event_KeyCode_insert));
    if (mem_eqlBytes(payload, u8_l("3~"))) return_ok(dansi_Event__pureKey(dansi_Event_KeyCode_delete));
    if (mem_eqlBytes(payload, u8_l("4~"))) return_ok(dansi_Event__pureKey(dansi_Event_KeyCode_end));
    if (mem_eqlBytes(payload, u8_l("5~"))) return_ok(dansi_Event__pureKey(dansi_Event_KeyCode_page_up));
    if (mem_eqlBytes(payload, u8_l("6~"))) return_ok(dansi_Event__pureKey(dansi_Event_KeyCode_page_down));
    if (mem_eqlBytes(payload, u8_l("11~"))) return_ok(dansi_Event__pureKey(dansi_Event_KeyCode_f1));
    if (mem_eqlBytes(payload, u8_l("12~"))) return_ok(dansi_Event__pureKey(dansi_Event_KeyCode_f2));
    if (mem_eqlBytes(payload, u8_l("13~"))) return_ok(dansi_Event__pureKey(dansi_Event_KeyCode_f3));
    if (mem_eqlBytes(payload, u8_l("14~"))) return_ok(dansi_Event__pureKey(dansi_Event_KeyCode_f4));
    if (mem_eqlBytes(payload, u8_l("15~"))) return_ok(dansi_Event__pureKey(dansi_Event_KeyCode_f5));
    if (mem_eqlBytes(payload, u8_l("17~"))) return_ok(dansi_Event__pureKey(dansi_Event_KeyCode_f6));
    if (mem_eqlBytes(payload, u8_l("18~"))) return_ok(dansi_Event__pureKey(dansi_Event_KeyCode_f7));
    if (mem_eqlBytes(payload, u8_l("19~"))) return_ok(dansi_Event__pureKey(dansi_Event_KeyCode_f8));
    if (mem_eqlBytes(payload, u8_l("20~"))) return_ok(dansi_Event__pureKey(dansi_Event_KeyCode_f9));
    if (mem_eqlBytes(payload, u8_l("21~"))) return_ok(dansi_Event__pureKey(dansi_Event_KeyCode_f10));
    if (mem_eqlBytes(payload, u8_l("23~"))) return_ok(dansi_Event__pureKey(dansi_Event_KeyCode_f11));
    if (mem_eqlBytes(payload, u8_l("24~"))) return_ok(dansi_Event__pureKey(dansi_Event_KeyCode_f12));
    return_err(E_cause$dansi_Event_UnknownSeq());
} $unscoped(fn);

$static fn_((dansi_Event__parseSS3(dansi_Seq seq))(dansi_Event_E$dansi_Event) $scope) {
    if (seq.bytes.len != 3) return_err(E_cause$dansi_Event_InvalidSeq());
    switch (*S_at((seq.bytes)[2])) {
    case_(('P')) return_ok(dansi_Event__pureKey(dansi_Event_KeyCode_f1)) $end(case);
    case_(('Q')) return_ok(dansi_Event__pureKey(dansi_Event_KeyCode_f2)) $end(case);
    case_(('R')) return_ok(dansi_Event__pureKey(dansi_Event_KeyCode_f3)) $end(case);
    case_(('S')) return_ok(dansi_Event__pureKey(dansi_Event_KeyCode_f4)) $end(case);
    case_(('H')) return_ok(dansi_Event__pureKey(dansi_Event_KeyCode_home)) $end(case);
    case_(('F')) return_ok(dansi_Event__pureKey(dansi_Event_KeyCode_end)) $end(case);
    default_() return_err(E_cause$dansi_Event_UnknownSeq()) $end(default);
    }
} $unscoped(fn);

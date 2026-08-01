#include "dansi-xterm/key.h"
#include "dansi-core/esc.h"
#include <dh/io/Fixed.h>

$static fn_((dansi_xterm_key__special(dansi_xterm_key_Named key, dansi_xterm_key_Mods mods))(dansi_xterm_key_Event)) {
    var event = union_of$((dansi_xterm_key_Event)(dansi_xterm_key_Event_special)((dansi_xterm_key_Special){
        .key = key,
        .mods = mods,
    }));
    return event;
};

$static fn_((dansi_xterm_key__text(u32 codepoint, dansi_xterm_key_Mods mods))(dansi_xterm_key_Event)) {
    var event = union_of$((dansi_xterm_key_Event)(dansi_xterm_key_Event_text)((dansi_xterm_key_Text){
        .codepoint = codepoint,
        .mods = mods,
    }));
    return event;
};

$static fn_((dansi_xterm_key__namedFromCursorFinal(u8 final))(O$dansi_xterm_key_Named) $scope) {
    switch (final) {
    case_((dansi_xterm_key_final_up_byte)) return_some(dansi_xterm_key_Named_up) $end(case);
    case_((dansi_xterm_key_final_down_byte)) return_some(dansi_xterm_key_Named_down) $end(case);
    case_((dansi_xterm_key_final_right_byte)) return_some(dansi_xterm_key_Named_right) $end(case);
    case_((dansi_xterm_key_final_left_byte)) return_some(dansi_xterm_key_Named_left) $end(case);
    case_((dansi_xterm_key_final_home_byte)) return_some(dansi_xterm_key_Named_home) $end(case);
    case_((dansi_xterm_key_final_end_byte)) return_some(dansi_xterm_key_Named_end) $end(case);
    case_((dansi_xterm_key_final_begin_byte)) return_some(dansi_xterm_key_Named_begin) $end(case);
    default_() return_none() $end(default);
    }
} $unscoped(fn);

$static fn_((dansi_xterm_key__namedFromSS3Final(u8 final))(O$dansi_xterm_key_Named) $scope) {
    switch (final) {
    case_((dansi_xterm_key_final_f1_byte)) return_some(dansi_xterm_key_Named_f1) $end(case);
    case_((dansi_xterm_key_final_f2_byte)) return_some(dansi_xterm_key_Named_f2) $end(case);
    case_((dansi_xterm_key_final_f3_byte)) return_some(dansi_xterm_key_Named_f3) $end(case);
    case_((dansi_xterm_key_final_f4_byte)) return_some(dansi_xterm_key_Named_f4) $end(case);
    default_() return dansi_xterm_key__namedFromCursorFinal(final) $end(default);
    }
} $unscoped(fn);

$static fn_((dansi_xterm_key__namedFromTildeCode(u16 code))(O$dansi_xterm_key_Named) $scope) {
    switch (code) {
    case_((dansi_xterm_key_TildeCode_home)) return_some(dansi_xterm_key_Named_home) $end(case);
    case_((dansi_xterm_key_TildeCode_insert)) return_some(dansi_xterm_key_Named_insert) $end(case);
    case_((dansi_xterm_key_TildeCode_delete)) return_some(dansi_xterm_key_Named_delete) $end(case);
    case_((dansi_xterm_key_TildeCode_end)) return_some(dansi_xterm_key_Named_end) $end(case);
    case_((dansi_xterm_key_TildeCode_page_up)) return_some(dansi_xterm_key_Named_page_up) $end(case);
    case_((dansi_xterm_key_TildeCode_page_down)) return_some(dansi_xterm_key_Named_page_down) $end(case);
    case_((dansi_xterm_key_TildeCode_f5)) return_some(dansi_xterm_key_Named_f5) $end(case);
    case_((dansi_xterm_key_TildeCode_f6)) return_some(dansi_xterm_key_Named_f6) $end(case);
    case_((dansi_xterm_key_TildeCode_f7)) return_some(dansi_xterm_key_Named_f7) $end(case);
    case_((dansi_xterm_key_TildeCode_f8)) return_some(dansi_xterm_key_Named_f8) $end(case);
    case_((dansi_xterm_key_TildeCode_f9)) return_some(dansi_xterm_key_Named_f9) $end(case);
    case_((dansi_xterm_key_TildeCode_f10)) return_some(dansi_xterm_key_Named_f10) $end(case);
    case_((dansi_xterm_key_TildeCode_f11)) return_some(dansi_xterm_key_Named_f11) $end(case);
    case_((dansi_xterm_key_TildeCode_f12)) return_some(dansi_xterm_key_Named_f12) $end(case);
    case_((dansi_xterm_key_TildeCode_f13)) return_some(dansi_xterm_key_Named_f13) $end(case);
    case_((dansi_xterm_key_TildeCode_f14)) return_some(dansi_xterm_key_Named_f14) $end(case);
    case_((dansi_xterm_key_TildeCode_f15)) return_some(dansi_xterm_key_Named_f15) $end(case);
    case_((dansi_xterm_key_TildeCode_f16)) return_some(dansi_xterm_key_Named_f16) $end(case);
    case_((dansi_xterm_key_TildeCode_f17)) return_some(dansi_xterm_key_Named_f17) $end(case);
    case_((dansi_xterm_key_TildeCode_f18)) return_some(dansi_xterm_key_Named_f18) $end(case);
    case_((dansi_xterm_key_TildeCode_f19)) return_some(dansi_xterm_key_Named_f19) $end(case);
    case_((dansi_xterm_key_TildeCode_f20)) return_some(dansi_xterm_key_Named_f20) $end(case);
    case_((dansi_xterm_key_TildeCode_f21)) return_some(dansi_xterm_key_Named_f21) $end(case);
    case_((dansi_xterm_key_TildeCode_f22)) return_some(dansi_xterm_key_Named_f22) $end(case);
    case_((dansi_xterm_key_TildeCode_f23)) return_some(dansi_xterm_key_Named_f23) $end(case);
    case_((dansi_xterm_key_TildeCode_f24)) return_some(dansi_xterm_key_Named_f24) $end(case);
    default_() return_none() $end(default);
    }
} $unscoped(fn);

$static fn_((dansi_xterm_key__namedFromCodepoint(u32 codepoint))(O$dansi_xterm_key_Named) $scope) {
    switch (codepoint) {
    case_((dansi_xterm_key_codepoint_tab)) return_some(dansi_xterm_key_Named_tab) $end(case);
    case_((dansi_xterm_key_codepoint_enter)) return_some(dansi_xterm_key_Named_enter) $end(case);
    case_((dansi_xterm_key_codepoint_escape)) return_some(dansi_xterm_key_Named_escape) $end(case);
    case_((dansi_xterm_key_codepoint_backspace)) return_some(dansi_xterm_key_Named_backspace) $end(case);
    default_() return_none() $end(default);
    }
} $unscoped(fn);

$static fn_((dansi_xterm_key__isModifiedCSI(dansi_csi_Frame frame))(bool)) {
    if (isNone(dansi_csi_Frame_paramAtAsU16(frame, dansi_xterm_key_param_modifier))) return false;
    if (frame.final == dansi_xterm_key_final_tilde_byte) {
        return isSome(dansi_csi_Frame_paramAtAsU16(frame, dansi_xterm_key_param_key));
    }
    return isSome(dansi_xterm_key__namedFromCursorFinal(frame.final));
};

fn_((dansi_xterm_key_setModifyRaw(
    dansi_xterm_key_Resrc resource, u8 value, dansi_xterm_key_SetModifyRawBuf* buf
))(S$u8)) {
    var writing = io_Fixed_Writer_from(io_Fixed_writing(A_ref$((S$u8)(*buf))));
    catch_((dansi_xterm_key_setModifyRawWrite(resource, value, io_Fixed_writer(&writing)))($ignore, claim_unreachable));
    return io_Fixed_written(writing.stream);
};

fn_((dansi_xterm_key_setModifyRawWrite(dansi_xterm_key_Resrc resource, u8 value, io_Writer out))(io_PrintE$void)) {
    return io_Writer_print(
        out,
        u8_l(dansi_csi_make_static(
            dansi_xterm_key_modify_marker "{:uhh}" dansi_csi_param_sep "{:uhh}", "", dansi_xterm_key_modify_final
        )),
        as$(u8)(resource), value
    );
};

fn_((dansi_xterm_key_setModifyLevel(
    dansi_xterm_key_Resrc resource, dansi_xterm_key_ModifyLevel level, dansi_xterm_key_SetModifyLevelBuf* buf
))(S$u8)) {
    return dansi_xterm_key_setModifyRaw(resource, as$(u8)(level), buf);
};

fn_((dansi_xterm_key_setModifyLevelWrite(
    dansi_xterm_key_Resrc resource, dansi_xterm_key_ModifyLevel level, io_Writer out
))(io_PrintE$void)) {
    return dansi_xterm_key_setModifyRawWrite(resource, as$(u8)(level), out);
};

fn_((dansi_xterm_key_resetModify(dansi_xterm_key_Resrc resource, dansi_xterm_key_ResetModifyBuf* buf))(S$u8)) {
    var writing = io_Fixed_Writer_from(io_Fixed_writing(A_ref$((S$u8)(*buf))));
    catch_((dansi_xterm_key_resetModifyWrite(resource, io_Fixed_writer(&writing)))($ignore, claim_unreachable));
    return io_Fixed_written(writing.stream);
};

fn_((dansi_xterm_key_resetModifyWrite(dansi_xterm_key_Resrc resource, io_Writer out))(io_PrintE$void)) {
    return io_Writer_print(
        out,
        u8_l(dansi_csi_make_static(
            dansi_xterm_key_modify_marker "{:uhh}", "", dansi_xterm_key_modify_final
        )),
        as$(u8)(resource)
    );
};

fn_((dansi_xterm_key_disableModify(dansi_xterm_key_Resrc resource, dansi_xterm_key_DisableModifyBuf* buf))(S$u8)) {
    var writing = io_Fixed_Writer_from(io_Fixed_writing(A_ref$((S$u8)(*buf))));
    catch_((dansi_xterm_key_disableModifyWrite(resource, io_Fixed_writer(&writing)))($ignore, claim_unreachable));
    return io_Fixed_written(writing.stream);
};

fn_((dansi_xterm_key_disableModifyWrite(dansi_xterm_key_Resrc resource, io_Writer out))(io_PrintE$void)) {
    return io_Writer_print(
        out,
        u8_l(dansi_csi_make_static(
            dansi_xterm_key_modify_marker "{:uhh}", "", dansi_xterm_key_disable_modify_final
        )),
        as$(u8)(resource)
    );
};

fn_((dansi_xterm_key_queryModify(dansi_xterm_key_Resrc resource, dansi_xterm_key_QueryModifyBuf* buf))(S$u8)) {
    var writing = io_Fixed_Writer_from(io_Fixed_writing(A_ref$((S$u8)(*buf))));
    catch_((dansi_xterm_key_queryModifyWrite(resource, io_Fixed_writer(&writing)))($ignore, claim_unreachable));
    return io_Fixed_written(writing.stream);
};

fn_((dansi_xterm_key_queryModifyWrite(dansi_xterm_key_Resrc resource, io_Writer out))(io_PrintE$void)) {
    return io_Writer_print(
        out,
        u8_l(dansi_csi_make_static(
            dansi_xterm_key_query_marker "{:uhh}", "", dansi_xterm_key_modify_final
        )),
        as$(u8)(resource)
    );
};

fn_((dansi_xterm_key_setFormat(
    dansi_xterm_key_Resrc resource, dansi_xterm_key_Format format, dansi_xterm_key_SetFormatBuf* buf
))(S$u8)) {
    var writing = io_Fixed_Writer_from(io_Fixed_writing(A_ref$((S$u8)(*buf))));
    catch_((dansi_xterm_key_setFormatWrite(resource, format, io_Fixed_writer(&writing)))($ignore, claim_unreachable));
    return io_Fixed_written(writing.stream);
};

fn_((dansi_xterm_key_setFormatWrite(dansi_xterm_key_Resrc resource, dansi_xterm_key_Format format, io_Writer out))(io_PrintE$void)) {
    return io_Writer_print(
        out,
        u8_l(dansi_csi_make_static(
            dansi_xterm_key_modify_marker "{:uhh}" dansi_csi_param_sep "{:uhh}", "", dansi_xterm_key_format_final
        )),
        as$(u8)(resource), as$(u8)(format)
    );
};

fn_((dansi_xterm_key_resetFormat(dansi_xterm_key_Resrc resource, dansi_xterm_key_ResetFormatBuf* buf))(S$u8)) {
    var writing = io_Fixed_Writer_from(io_Fixed_writing(A_ref$((S$u8)(*buf))));
    catch_((dansi_xterm_key_resetFormatWrite(resource, io_Fixed_writer(&writing)))($ignore, claim_unreachable));
    return io_Fixed_written(writing.stream);
};

fn_((dansi_xterm_key_resetFormatWrite(dansi_xterm_key_Resrc resource, io_Writer out))(io_PrintE$void)) {
    return io_Writer_print(
        out,
        u8_l(dansi_csi_make_static(
            dansi_xterm_key_modify_marker "{:uhh}", "", dansi_xterm_key_format_final
        )),
        as$(u8)(resource)
    );
};

fn_((dansi_xterm_key_queryFormat(dansi_xterm_key_Resrc resource, dansi_xterm_key_QueryFormatBuf* buf))(S$u8)) {
    var writing = io_Fixed_Writer_from(io_Fixed_writing(A_ref$((S$u8)(*buf))));
    catch_((dansi_xterm_key_queryFormatWrite(resource, io_Fixed_writer(&writing)))($ignore, claim_unreachable));
    return io_Fixed_written(writing.stream);
};

fn_((dansi_xterm_key_queryFormatWrite(dansi_xterm_key_Resrc resource, io_Writer out))(io_PrintE$void)) {
    return io_Writer_print(
        out,
        u8_l(dansi_csi_make_static(
            dansi_xterm_key_query_marker "{:uhh}", "", dansi_xterm_key_query_format_final
        )),
        as$(u8)(resource)
    );
};

fn_((dansi_xterm_key_enableEnhanced(dansi_xterm_key_EnableEnhancedBuf* buf))(S$u8)) {
    var writing = io_Fixed_Writer_from(io_Fixed_writing(A_ref$((S$u8)(*buf))));
    catch_((dansi_xterm_key_enableEnhancedWrite(io_Fixed_writer(&writing)))($ignore, claim_unreachable));
    return io_Fixed_written(writing.stream);
};

fn_((dansi_xterm_key_enableEnhancedWrite(io_Writer out))(io_PrintE$void) $scope) {
    try_(dansi_xterm_key_setModifyLevelWrite(
        dansi_xterm_key_Resrc_other_keys,
        dansi_xterm_key_ModifyLevel_other_keys,
        out
    ));
    return dansi_xterm_key_setFormatWrite(
        dansi_xterm_key_Resrc_other_keys,
        dansi_xterm_key_Format_csi_u,
        out
    );
} $unscoped(fn);

fn_((dansi_xterm_key_disableEnhanced(dansi_xterm_key_DisableEnhancedBuf* buf))(S$u8)) {
    var writing = io_Fixed_Writer_from(io_Fixed_writing(A_ref$((S$u8)(*buf))));
    catch_((dansi_xterm_key_disableEnhancedWrite(io_Fixed_writer(&writing)))($ignore, claim_unreachable));
    return io_Fixed_written(writing.stream);
};

fn_((dansi_xterm_key_disableEnhancedWrite(io_Writer out))(io_PrintE$void) $scope) {
    try_(dansi_xterm_key_resetFormatWrite(dansi_xterm_key_Resrc_other_keys, out));
    return dansi_xterm_key_resetModifyWrite(dansi_xterm_key_Resrc_other_keys, out);
} $unscoped(fn);

fn_((dansi_xterm_key_parseReport(dansi_Seq seq))(O$dansi_xterm_key_Report) $scope) {
    switch (seq.kind) {
    case_((dansi_Seq_Kind_esc)) {
        let frame = catch_((dansi_esc_parse(seq.bytes))($ignore, return_none()));
        return_some(union_of((dansi_xterm_key_Report_legacy_esc){
            .final = frame.final,
            .bytes = seq.bytes,
        }));
    } $end(case);
    case_((dansi_Seq_Kind_ss3)) {
        if (seq.bytes.len < 2) return_none();
        return_some(union_of((dansi_xterm_key_Report_legacy_ss3){
            .final = *S_at((seq.bytes)[seq.bytes.len - 1]),
            .bytes = seq.bytes,
        }));
    } $end(case);
    case_((dansi_Seq_Kind_csi)) {
        let frame = catch_((dansi_csi_parse(seq.bytes))($ignore, return_none()));
        if (frame.final == dansi_xterm_key_final_csi_u_byte) {
            let codepoint = orelse_((dansi_csi_Frame_paramAtAsU16(frame, dansi_xterm_key_param_key))(return_none()));
            return_some(union_of((dansi_xterm_key_Report_csi_u){
                .codepoint = codepoint,
                .modifier_param = dansi_csi_Frame_paramAtAsU16(frame, dansi_xterm_key_param_modifier),
                .frame = frame,
            }));
        }
        if (frame.final == dansi_xterm_key_final_tilde_byte) {
            let first = orelse_((dansi_csi_Frame_paramAtAsU16(frame, dansi_xterm_key_param_key))(return_none()));
            if (first == dansi_xterm_key_modify_other_prefix_u16) {
                let modifier_param = orelse_((dansi_csi_Frame_paramAtAsU16(frame, dansi_xterm_key_param_modifier))(
                    return_none()
                ));
                let codepoint = orelse_((dansi_csi_Frame_paramAtAsU16(frame, dansi_xterm_key_param_codepoint))(
                    return_none()
                ));
                return_some(union_of((dansi_xterm_key_Report_modify_other){
                    .codepoint = codepoint,
                    .modifier_param = modifier_param,
                    .frame = frame,
                }));
            }
        }
        if (dansi_xterm_key__isModifiedCSI(frame)) {
            return_some(union_of((dansi_xterm_key_Report_modified_csi){ .frame = frame }));
        }
        return_some(union_of((dansi_xterm_key_Report_legacy_csi){ .frame = frame }));
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

fn_((dansi_xterm_key_interpretReport(
    dansi_xterm_key_Report report
))(dansi_xterm_key_E$dansi_xterm_key_Event) $scope) {
    match_(report) {
    patt_((dansi_xterm_key_Report_legacy_esc)(esc)) {
        let named = orelse_((dansi_xterm_key__namedFromCursorFinal(esc.final))(
            return_err(E_cause$dansi_xterm_key_InvalidReport())
        ));
        return_ok(dansi_xterm_key__special(named, dansi_xterm_key_modsNone()));
    } $end(patt);
    patt_((dansi_xterm_key_Report_legacy_ss3)(ss3)) {
        let named = orelse_((dansi_xterm_key__namedFromSS3Final(ss3.final))(
            return_err(E_cause$dansi_xterm_key_InvalidReport())
        ));
        return_ok(dansi_xterm_key__special(named, dansi_xterm_key_modsNone()));
    } $end(patt);
    patt_((dansi_xterm_key_Report_legacy_csi)(csi)) {
        if (csi.frame.final == dansi_xterm_key_final_tilde_byte) {
            let code = orelse_((dansi_csi_Frame_paramAtAsU16(csi.frame, dansi_xterm_key_param_key))(
                return_err(E_cause$dansi_xterm_key_InvalidReport())
            ));
            let named = orelse_((dansi_xterm_key__namedFromTildeCode(code))(
                return_err(E_cause$dansi_xterm_key_InvalidReport())
            ));
            return_ok(dansi_xterm_key__special(named, dansi_xterm_key_modsNone()));
        }
        let named = orelse_((dansi_xterm_key__namedFromCursorFinal(csi.frame.final))(
            return_err(E_cause$dansi_xterm_key_InvalidReport())
        ));
        return_ok(dansi_xterm_key__special(named, dansi_xterm_key_modsNone()));
    } $end(patt);
    patt_((dansi_xterm_key_Report_modified_csi)(modified)) {
        let modifier_param = orelse_((dansi_csi_Frame_paramAtAsU16(modified.frame, dansi_xterm_key_param_modifier))(
            return_err(E_cause$dansi_xterm_key_InvalidReport())
        ));
        let mods = dansi_xterm_key_modsFromParam(modifier_param);
        if (modified.frame.final == dansi_xterm_key_final_tilde_byte) {
            let code = orelse_((dansi_csi_Frame_paramAtAsU16(modified.frame, dansi_xterm_key_param_key))(
                return_err(E_cause$dansi_xterm_key_InvalidReport())
            ));
            let named = orelse_((dansi_xterm_key__namedFromTildeCode(code))(
                return_err(E_cause$dansi_xterm_key_InvalidReport())
            ));
            return_ok(dansi_xterm_key__special(named, mods));
        }
        let named = orelse_((dansi_xterm_key__namedFromCursorFinal(modified.frame.final))(
            return_err(E_cause$dansi_xterm_key_InvalidReport())
        ));
        return_ok(dansi_xterm_key__special(named, mods));
    } $end(patt);
    patt_((dansi_xterm_key_Report_modify_other)(other)) {
        return_ok(union_of((dansi_xterm_key_Event_modify_other){
            .codepoint = other.codepoint,
            .mods = dansi_xterm_key_modsFromParam(other.modifier_param),
        }));
    } $end(patt);
    patt_((dansi_xterm_key_Report_csi_u)(csi_u)) {
        let mods = isSome(csi_u.modifier_param)
                     ? dansi_xterm_key_modsFromParam(unwrap_(csi_u.modifier_param))
                     : dansi_xterm_key_modsNone();
        if_some((dansi_xterm_key__namedFromCodepoint(csi_u.codepoint))(named)) {
            return_ok(dansi_xterm_key__special(named, mods));
        }
        return_ok(dansi_xterm_key__text(csi_u.codepoint, mods));
    } $end(patt);
    } $end(match);
    return_err(E_cause$dansi_xterm_key_InvalidReport());
} $unscoped(fn);

fn_((dansi_xterm_key_parseEvent(dansi_Seq seq))(O$dansi_xterm_key_Event) $scope) {
    let report = orelse_((dansi_xterm_key_parseReport(seq))(return_none()));
    return catch_none$((O$dansi_xterm_key_Event)(dansi_xterm_key_interpretReport(report)));
} $unscoped(fn);

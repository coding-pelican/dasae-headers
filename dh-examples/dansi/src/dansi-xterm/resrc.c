#include "dansi-xterm/resrc.h"
#include <dh/io/Fixed.h>

$static fn_((dansi_xterm_resrc__parseResponse(
    dansi_dcs_Frame frame, u8 final
))(dansi_xterm_resrc_E$dansi_xterm_resrc_Response) $scope) {
    if (frame.final != final || frame.intermediates.len != 1 || *S_at((frame.intermediates)[0]) != dansi_xterm_resrc_dcs_intermediate_byte) {
        return_err(E_cause$dansi_xterm_resrc_InvalidResponse());
    }
    if (frame.params.len == 0) return_err(E_cause$dansi_xterm_resrc_InvalidResponse());
    let flag = *S_at((frame.params)[dansi_xterm_resrc_response_param_flag]);
    if (flag != dansi_xterm_resrc_response_invalid_byte && flag != dansi_xterm_resrc_response_valid_byte) {
        return_err(E_cause$dansi_xterm_resrc_InvalidResponse());
    }
    return_ok({
        .valid = flag == dansi_xterm_resrc_response_valid_byte,
        .payload = frame.payload,
    });
} $unscoped(fn);

fn_((dansi_xterm_resrc_requestXResRaw(S_const$u8 names_hex, S$u8 buf))(E$S$u8)) {
    return dansi_dcs_make(u8_l(""), u8_l(dansi_xterm_resrc_dcs_intermediate), dansi_xterm_resrc_request_xres_final_byte, names_hex, buf);
};

fn_((dansi_xterm_resrc_requestXResRawWrite(S_const$u8 names_hex, io_Writer out))(E$void)) {
    return dansi_dcs_write(u8_l(""), u8_l(dansi_xterm_resrc_dcs_intermediate), dansi_xterm_resrc_request_xres_final_byte, names_hex, out);
};

fn_((dansi_xterm_resrc_parseXResResponse(
    dansi_dcs_Frame frame
))(dansi_xterm_resrc_E$dansi_xterm_resrc_Response)) {
    return dansi_xterm_resrc__parseResponse(frame, dansi_xterm_resrc_response_xres_final_byte);
};

fn_((dansi_xterm_resrc_setTermcapRaw(S_const$u8 name_hex, S$u8 buf))(E$S$u8)) {
    return dansi_dcs_make(u8_l(""), u8_l(dansi_xterm_resrc_dcs_intermediate), dansi_xterm_resrc_set_termcap_final_byte, name_hex, buf);
};

fn_((dansi_xterm_resrc_setTermcapRawWrite(S_const$u8 name_hex, io_Writer out))(E$void)) {
    return dansi_dcs_write(u8_l(""), u8_l(dansi_xterm_resrc_dcs_intermediate), dansi_xterm_resrc_set_termcap_final_byte, name_hex, out);
};

fn_((dansi_xterm_resrc_requestTermcapRaw(S_const$u8 names_hex, S$u8 buf))(E$S$u8)) {
    return dansi_dcs_make(u8_l(""), u8_l(dansi_xterm_resrc_dcs_intermediate), dansi_xterm_resrc_request_termcap_final_byte, names_hex, buf);
};

fn_((dansi_xterm_resrc_requestTermcapRawWrite(S_const$u8 names_hex, io_Writer out))(E$void)) {
    return dansi_dcs_write(u8_l(""), u8_l(dansi_xterm_resrc_dcs_intermediate), dansi_xterm_resrc_request_termcap_final_byte, names_hex, out);
};

fn_((dansi_xterm_resrc_parseTermcapResponse(
    dansi_dcs_Frame frame
))(dansi_xterm_resrc_E$dansi_xterm_resrc_Response)) {
    return dansi_xterm_resrc__parseResponse(frame, dansi_xterm_resrc_response_termcap_final_byte);
};

fn_((dansi_xterm_resrc_queryAllowed(void))(S_const$u8)) {
    return u8_l(dansi_xterm_resrc_queryAllowed_static());
};

fn_((dansi_xterm_resrc_queryAllowedWrite(io_Writer out))(E$void)) {
    return io_Writer_writeBytes(out, dansi_xterm_resrc_queryAllowed());
};

fn_((dansi_xterm_resrc_queryAllowableRaw(S_const$u8 feature, S$u8 buf))(E$S$u8)) {
    return dansi_osc_make(dansi_xterm_resrc_allowed_cmd_u16, feature, buf);
};

fn_((dansi_xterm_resrc_queryAllowableRawWrite(S_const$u8 feature, io_Writer out))(E$void)) {
    return dansi_osc_write(dansi_xterm_resrc_allowed_cmd_u16, feature, out);
};

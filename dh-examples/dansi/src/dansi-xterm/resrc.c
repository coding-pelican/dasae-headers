#include "dansi-xterm/resrc.h"
#include <dh/io/Fixed.h>

$static fn_((dansi_xterm_resrc__parseResponse(
    dansi_dcs_Frame frame, u8 final
))(dansi_xterm_resrc_E$dansi_xterm_resrc_Response) $scope) {
    if (frame.final != final || frame.intermediates.len != 1 || *S_at((frame.intermediates)[0]) != u8_c('+')) {
        return_err(E_cause$dansi_xterm_resrc_InvalidResponse());
    }
    if (frame.params.len == 0) return_err(E_cause$dansi_xterm_resrc_InvalidResponse());
    let flag = *S_at((frame.params)[0]);
    if (flag != u8_c('0') && flag != u8_c('1')) {
        return_err(E_cause$dansi_xterm_resrc_InvalidResponse());
    }
    return_ok({
        .valid = flag == u8_c('1'),
        .payload = frame.payload,
    });
} $unscoped(fn);

fn_((dansi_xterm_resrc_requestXResRaw(S_const$u8 names_hex, S$u8 buf))(E$S$u8)) {
    return dansi_dcs_make(u8_l(""), u8_l("+"), u8_c('Q'), names_hex, buf);
};

fn_((dansi_xterm_resrc_requestXResRawWrite(S_const$u8 names_hex, io_Writer out))(E$void)) {
    return dansi_dcs_write(u8_l(""), u8_l("+"), u8_c('Q'), names_hex, out);
};

fn_((dansi_xterm_resrc_parseXResResponse(
    dansi_dcs_Frame frame
))(dansi_xterm_resrc_E$dansi_xterm_resrc_Response)) {
    return dansi_xterm_resrc__parseResponse(frame, u8_c('R'));
};

fn_((dansi_xterm_resrc_setTermcapRaw(S_const$u8 name_hex, S$u8 buf))(E$S$u8)) {
    return dansi_dcs_make(u8_l(""), u8_l("+"), u8_c('p'), name_hex, buf);
};

fn_((dansi_xterm_resrc_setTermcapRawWrite(S_const$u8 name_hex, io_Writer out))(E$void)) {
    return dansi_dcs_write(u8_l(""), u8_l("+"), u8_c('p'), name_hex, out);
};

fn_((dansi_xterm_resrc_requestTermcapRaw(S_const$u8 names_hex, S$u8 buf))(E$S$u8)) {
    return dansi_dcs_make(u8_l(""), u8_l("+"), u8_c('q'), names_hex, buf);
};

fn_((dansi_xterm_resrc_requestTermcapRawWrite(S_const$u8 names_hex, io_Writer out))(E$void)) {
    return dansi_dcs_write(u8_l(""), u8_l("+"), u8_c('q'), names_hex, out);
};

fn_((dansi_xterm_resrc_parseTermcapResponse(
    dansi_dcs_Frame frame
))(dansi_xterm_resrc_E$dansi_xterm_resrc_Response)) {
    return dansi_xterm_resrc__parseResponse(frame, u8_c('r'));
};

fn_((dansi_xterm_resrc_queryAllowed(void))(S_const$u8)) {
    return u8_l(dansi_xterm_resrc_queryAllowed_static());
};

fn_((dansi_xterm_resrc_queryAllowedWrite(io_Writer out))(E$void)) {
    return io_Writer_writeBytes(out, dansi_xterm_resrc_queryAllowed());
};

fn_((dansi_xterm_resrc_queryAllowableRaw(S_const$u8 feature, S$u8 buf))(E$S$u8)) {
    return dansi_osc_make(60, feature, buf);
};

fn_((dansi_xterm_resrc_queryAllowableRawWrite(S_const$u8 feature, io_Writer out))(E$void)) {
    return dansi_osc_write(60, feature, out);
};

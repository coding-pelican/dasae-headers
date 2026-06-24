#include "dansi-link/osc8.h"
#include <dh/io/Fixed.h>
#include <dh/mem/common.h>

$static fn_((dansi_link_osc8__idFromRawParams(S_const$u8 raw))(O$S_const$u8) $scope) {
    var_(cursor, usize) = 0;
    while (cursor <= raw.len) {
        let rest = S_suffix((raw)(cursor));
        var_(segment, S_const$u8) = rest;
        if_some((mem_findFirstUnitBytes(rest, dansi_link_osc8_params_sep_byte))(delim)) {
            segment = S_prefix((rest)(delim));
            cursor += delim + 1;
        } else {
            cursor = raw.len + 1;
        }
        if (segment.len >= dansi_link_osc8_param_id_key_len
            && mem_eqlBytes(S_prefix((segment)(dansi_link_osc8_param_id_key_len)), u8_l(dansi_link_osc8_param_id_key))) {
            return_some(S_suffix((segment)(dansi_link_osc8_param_id_key_len)));
        }
    }
    return_none();
} $unscoped(fn);

$static fn_((dansi_link_osc8__writeParams(dansi_link_osc8_Params params, io_Writer out))(E$void) $scope) {
    match_(params) {
    patt_((dansi_link_osc8_Params_none)($ignore)) {
        return_ok({});
    } $end(patt);
    patt_((dansi_link_osc8_Params_id)(id)) {
        try_(io_Writer_writeBytes(out, u8_l(dansi_link_osc8_param_id_key)));
        return io_Writer_writeBytes(out, id);
    } $end(patt);
    patt_((dansi_link_osc8_Params_raw)(raw)) {
        return io_Writer_writeBytes(out, raw);
    } $end(patt);
    }
    $end_match;
    return_ok({});
} $unscoped(fn);

fn_((dansi_link_osc8_open(S_const$u8 uri, dansi_link_osc8_Params params, S$u8 buf))(E$S$u8) $scope) {
    var writing = io_Fixed_Writer_init(io_Fixed_writing(buf));
    try_(dansi_link_osc8_openWrite(uri, params, io_Fixed_writer(&writing)));
    return_ok(io_Fixed_written(writing.stream));
} $unscoped(fn);

fn_((dansi_link_osc8_openWrite(S_const$u8 uri, dansi_link_osc8_Params params, io_Writer out))(E$void) $scope) {
    try_(io_Writer_writeBytes(out, u8_l(dansi_osc_7bit_prefix dansi_link_osc8_cmd dansi_osc_cmd_sep)));
    try_(dansi_link_osc8__writeParams(params, out));
    try_(io_Writer_writeByte(out, dansi_link_osc8_payload_sep_byte));
    try_(io_Writer_writeBytes(out, uri));
    return dansi_Seq_EOS_write(dansi_Seq_EOS_st_7bit, out);
} $unscoped(fn);

fn_((dansi_link_osc8_openPlain(S_const$u8 uri, S$u8 buf))(E$S$u8)) {
    return dansi_link_osc8_open(uri, union_of$((dansi_link_osc8_Params)(dansi_link_osc8_Params_none){}), buf);
};

fn_((dansi_link_osc8_openPlainWrite(S_const$u8 uri, io_Writer out))(E$void)) {
    return dansi_link_osc8_openWrite(uri, union_of$((dansi_link_osc8_Params)(dansi_link_osc8_Params_none){}), out);
};

fn_((dansi_link_osc8_openWithId(S_const$u8 uri, S_const$u8 id, S$u8 buf))(E$S$u8)) {
    return dansi_link_osc8_open(uri, union_of$((dansi_link_osc8_Params)(dansi_link_osc8_Params_id)(id)), buf);
};

fn_((dansi_link_osc8_openWithIdWrite(S_const$u8 uri, S_const$u8 id, io_Writer out))(E$void)) {
    return dansi_link_osc8_openWrite(uri, union_of$((dansi_link_osc8_Params)(dansi_link_osc8_Params_id)(id)), out);
};

fn_((dansi_link_osc8_openRaw(S_const$u8 uri, S_const$u8 params, S$u8 buf))(E$S$u8)) {
    return dansi_link_osc8_open(uri, union_of$((dansi_link_osc8_Params)(dansi_link_osc8_Params_raw)(params)), buf);
};

fn_((dansi_link_osc8_openRawWrite(S_const$u8 uri, S_const$u8 params, io_Writer out))(E$void)) {
    return dansi_link_osc8_openWrite(uri, union_of$((dansi_link_osc8_Params)(dansi_link_osc8_Params_raw)(params)), out);
};

fn_((dansi_link_osc8_close(S$u8 buf))(E$S$u8) $scope) {
    var writing = io_Fixed_Writer_init(io_Fixed_writing(buf));
    try_(dansi_link_osc8_closeWrite(io_Fixed_writer(&writing)));
    return_ok(io_Fixed_written(writing.stream));
} $unscoped(fn);

fn_((dansi_link_osc8_closeWrite(io_Writer out))(E$void) $scope) {
    try_(io_Writer_writeBytes(out, u8_l(dansi_osc_7bit_prefix dansi_link_osc8_cmd dansi_osc_cmd_sep dansi_osc_cmd_sep)));
    return dansi_Seq_EOS_write(dansi_Seq_EOS_st_7bit, out);
} $unscoped(fn);

fn_((dansi_link_osc8_parse(dansi_osc_Frame frame))(dansi_link_osc8_E$dansi_link_osc8_Frame) $scope) {
    let split = orelse_((dansi_osc_Frame_splitCmd(frame))(return_err(E_cause$dansi_link_osc8_Invalid())));
    let cmd = orelse_((dansi_osc_CmdSplit_cmdAsU16(split))(return_err(E_cause$dansi_link_osc8_Invalid())));
    if (cmd != dansi_link_osc8_cmd_u16) return_err(E_cause$dansi_link_osc8_Invalid());

    let delim = orelse_((mem_findFirstUnitBytes(split.payload, dansi_link_osc8_payload_sep_byte))(
        return_err(E_cause$dansi_link_osc8_Invalid())
    ));
    let params = S_prefix((split.payload)(delim));
    let uri = S_suffix((split.payload)(delim + 1));
    if (uri.len == 0) {
        return_ok(union_of((dansi_link_osc8_Frame_close){}));
    }
    return_ok(union_of((dansi_link_osc8_Frame_open){
        .params = {
            .raw = params,
            .id = dansi_link_osc8__idFromRawParams(params),
        },
        .uri = uri,
    }));
} $unscoped(fn);

#include "dansi-core/csi.h"
#include <dh/fmt/common.h>
#include <dh/io/Fixed.h>
#include <dh/mem/common.h>

$static fn_((dansi_csi__paramsWithoutPrivate(dansi_csi_Frame self))(S_const$u8)) {
    let marker = dansi_csi_Frame_privateMarker(self);
    if (isSome(marker)) return S_suffix((self.params)1);
    return self.params;
};

$static fn_((dansi_csi__parseU16(S_const$u8 text))(O$u16)) {
    if (text.len == 0) return none$((O$u16));
    return catch_none$((O$u16)(fmt_parse$u16(text, dansi_csi_param_radix)));
};

fn_((dansi_csi_parse(S_const$u8 bytes))(dansi_csi_E$dansi_csi_Frame) $scope) {
    if (bytes.len < 2) return_err(E_cause$dansi_csi_Invalid());
    var_(prefix_len, usize) = 0;
    if (*S_at((bytes)[0]) == dansi_Seq_esc_byte && bytes.len >= 3 && *S_at((bytes)[1]) == dansi_csi_7bit_intro_byte) {
        prefix_len = 2;
    } else if (*S_at((bytes)[0]) == dansi_csi_8bit_intro_byte) {
        prefix_len = 1;
    } else {
        return_err(E_cause$dansi_csi_Invalid());
    }

    let final = *S_at((bytes)[bytes.len - 1]);
    if (final < dansi_csi_final_min_byte || final > dansi_csi_final_max_byte) return_err(E_cause$dansi_csi_Invalid());

    var_(idx, usize) = prefix_len;
    while (idx + 1 < bytes.len) {
        let byte = *S_at((bytes)[idx]);
        if (byte < dansi_csi_intermediate_min_byte || byte > dansi_csi_private_marker_max_byte) {
            return_err(E_cause$dansi_csi_Invalid());
        }
        idx += 1;
    }

    let body = S_prefix((S_suffix((bytes)(prefix_len)))(bytes.len - prefix_len - 1));
    var_(intermediates_at, usize) = body.len;
    idx = 0;
    while (idx < body.len) {
        let byte = *S_at((body)[idx]);
        if (dansi_csi_intermediate_min_byte <= byte && byte <= dansi_csi_intermediate_max_byte) {
            intermediates_at = idx;
            break;
        }
        idx += 1;
    }

    return_ok({
        .params = S_prefix((body)(intermediates_at)),
        .intermediates = S_suffix((body)(intermediates_at)),
        .final = final,
        .bytes = bytes,
    });
} $unscoped(fn);

fn_((dansi_csi_make(S_const$u8 params, S_const$u8 intermediates, u8 final, S$u8 buf))(E$S$u8) $scope) {
    var writing = io_Fixed_Writer_from(io_Fixed_writing(buf));
    try_(dansi_csi_write(params, intermediates, final, io_Fixed_writer(&writing)));
    return_ok(io_Fixed_written(writing.stream));
} $unscoped(fn);

fn_((dansi_csi_write(S_const$u8 params, S_const$u8 intermediates, u8 final, io_Writer out))(io_PrintE$void) $scope) {
    try_(io_Writer_writeBytes(out, u8_l(dansi_csi_7bit_prefix)));
    try_(io_Writer_writeBytes(out, params));
    try_(io_Writer_writeBytes(out, intermediates));
    return_ok(try_(io_Writer_writeByte(out, final)));
} $unscoped(fn);

fn_((dansi_csi_Frame_isPrivate(dansi_csi_Frame self, u8 marker))(bool)) {
    let private_marker = dansi_csi_Frame_privateMarker(self);
    return isSome(private_marker) && unwrap_(private_marker) == marker;
};

fn_((dansi_csi_Frame_privateMarker(dansi_csi_Frame self))(O$u8)) {
    if (self.params.len == 0) return none$((O$u8));
    let marker = *S_at((self.params)[0]);
    if (dansi_csi_private_marker_min_byte <= marker && marker <= dansi_csi_private_marker_max_byte) {
        return some$((O$u8)(marker));
    }
    return none$((O$u8));
};

fn_((dansi_csi_Frame_paramAt(dansi_csi_Frame self, usize index))(O$dansi_csi_Param) $scope) {
    var iter = dansi_csi_Frame_paramIter(self);
    var_(i, usize) = 0;
    while_some((dansi_csi_ParamIter_next(&iter)), param) {
        if (i == index) return_some(param);
        i += 1;
    }
    return_none();
} $unscoped(fn);

fn_((dansi_csi_Frame_paramAtAsU16(dansi_csi_Frame self, usize index))(O$u16)) {
    if_some((dansi_csi_Frame_paramAt(self, index))(param)) {
        return dansi_csi_Param_asU16(param);
    }
    return none$((O$u16));
};

fn_((dansi_csi_Frame_paramIter(dansi_csi_Frame self))(dansi_csi_ParamIter)) {
    return (dansi_csi_ParamIter){ .params = dansi_csi__paramsWithoutPrivate(self), .cursor = 0 };
};

fn_((dansi_csi_Param_subparamIter(dansi_csi_Param self))(dansi_csi_SubparamIter)) {
    return (dansi_csi_SubparamIter){ .param = self.raw, .cursor = 0 };
};

fn_((dansi_csi_Param_asU16(dansi_csi_Param self))(O$u16)) {
    return dansi_csi__parseU16(self.raw);
};

fn_((dansi_csi_ParamIter_next(dansi_csi_ParamIter* self))(O$dansi_csi_Param) $scope) {
    if (self->cursor > self->params.len) return_none();
    let rest = S_suffix((self->params)(self->cursor));
    if_some((mem_findFirstUnitBytes(rest, dansi_csi_param_sep_byte))(delim)) {
        let raw = S_prefix((rest)(delim));
        self->cursor += delim + 1;
        return_some({ .raw = raw });
    }
    self->cursor = self->params.len + 1;
    return_some({ .raw = rest });
} $unscoped(fn);

fn_((dansi_csi_SubparamIter_next(dansi_csi_SubparamIter* self))(O$S_const$u8) $scope) {
    if (self->cursor > self->param.len) return_none();
    let rest = S_suffix((self->param)(self->cursor));
    if_some((mem_findFirstUnitBytes(rest, dansi_csi_subparam_sep_byte))(delim)) {
        let raw = S_prefix((rest)(delim));
        self->cursor += delim + 1;
        return_some(raw);
    }
    self->cursor = self->param.len + 1;
    return_some(rest);
} $unscoped(fn);

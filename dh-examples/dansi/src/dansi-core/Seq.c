#include "dansi-core/Seq.h"
#include "dansi-core/c1.h"
#include "dansi-core/esc.h"
#include "dansi-core/csi.h"
#include "dansi-core/dcs.h"
#include "dansi-core/osc.h"
#include "dansi-core/pm.h"
#include "dansi-core/apc.h"
#include "dansi-core/sos.h"
#include <dh/utf8.h>

$static fn_((dansi_Seq__readable(io_Buf_Reader* reader))(S_const$u8));
$attr($must_check)
$static fn_((dansi_Seq__ensure(io_Buf_Reader* reader, usize len))(E$void));
$attr($must_check)
$static fn_((dansi_Seq__complete(io_Buf_Reader* reader, dansi_Seq_Kind kind, usize len))(dansi_Seq_E$dansi_Seq));
$attr($must_check)
$static fn_((dansi_Seq__extractText(io_Buf_Reader* reader))(dansi_Seq_E$dansi_Seq));
$attr($must_check)
$static fn_((dansi_Seq__extractEsc(io_Buf_Reader* reader))(dansi_Seq_E$dansi_Seq));
$attr($must_check)
$static fn_((dansi_Seq__extractCsi(io_Buf_Reader* reader, usize prefix_len))(dansi_Seq_E$dansi_Seq));
$attr($must_check)
$static fn_((dansi_Seq__extractCtrlString(io_Buf_Reader* reader, dansi_Seq_Kind kind, usize prefix_len))(dansi_Seq_E$dansi_Seq));

$static fn_((dansi_Seq__receivePrefixedOrC1(io_Reader in, S$u8 buf, S_const$u8 prefix, u8 c1))(E$S$u8));
$attr($must_check)
$static fn_((dansi_Seq__receiveCsi7Or8(io_Reader in, S$u8 buf, S_const$u8 prefix, u8 c1))(E$S$u8));
$attr($must_check)
$static fn_((dansi_Seq__receiveCtrlString7Or8(io_Reader in, S$u8 buf, S_const$u8 prefix, u8 c1))(E$S$u8));

fn_((dansi_Seq_EOS_write(dansi_Seq_EOS self, io_Writer out))(E$void) $scope) {
    switch (self) {
    case_((dansi_Seq_EOS_none)) return_ok({}) $end(case);
    case_((dansi_Seq_EOS_bel)) return io_Writer_writeByte(out, dansi_Seq_bel_byte) $end(case);
    case_((dansi_Seq_EOS_st_7bit)) return io_Writer_writeBytes(out, u8_l(dansi_Seq_st_7bit)) $end(case);
    case_((dansi_Seq_EOS_st_8bit)) return io_Writer_writeByte(out, dansi_Seq_st_8bit_byte) $end(case);
    default_() return_err(E_cause$Unexpected()) $end(default);
    }
} $unscoped(fn);

fn_((dansi_Seq_CtrlTer_find(S_const$u8 bytes, usize start))(O$dansi_Seq_CtrlTer) $scope) {
    var_(idx, usize) = start;
    while (idx < bytes.len) {
        let byte = *S_at((bytes)[idx]);
        if (byte == dansi_Seq_bel_byte) return_some({ .payload_end = idx, .eos = dansi_Seq_EOS_bel });
        if (byte == dansi_Seq_st_8bit_byte) return_some({ .payload_end = idx, .eos = dansi_Seq_EOS_st_8bit });
        if (byte == dansi_Seq_esc_byte && idx + 1 < bytes.len && *S_at((bytes)[idx + 1]) == dansi_Seq_st_7bit_final_byte) {
            return_some({ .payload_end = idx, .eos = dansi_Seq_EOS_st_7bit });
        }
        idx += 1;
    }
    return_none();
} $unscoped(fn);

fn_((dansi_Seq_extract(io_Buf_Reader* reader))(dansi_Seq_E$dansi_Seq) $scope) {
    claim_assert_nonnull(reader);
    catch_((dansi_Seq__ensure(reader, 1))($ignore, return_err(E_cause$dansi_Seq_Incomplete())));
    let bytes = dansi_Seq__readable(reader);
    let first = *S_at((bytes)[0]);

    if (first <= dansi_Seq_c0_max_byte) {
        if (first == dansi_Seq_esc_byte) return dansi_Seq__extractEsc(reader);
        return dansi_Seq__complete(reader, dansi_Seq_Kind_c0, 1);
    }

    switch (first) {
    case_((dansi_c1_Code_ss2)) {
        catch_((dansi_Seq__ensure(reader, 2))($ignore, return_err(E_cause$dansi_Seq_Incomplete())));
        return dansi_Seq__complete(reader, dansi_Seq_Kind_ss2, 2);
    } $end(case);
    case_((dansi_c1_Code_ss3)) {
        catch_((dansi_Seq__ensure(reader, 2))($ignore, return_err(E_cause$dansi_Seq_Incomplete())));
        return dansi_Seq__complete(reader, dansi_Seq_Kind_ss3, 2);
    } $end(case);
    case_((dansi_c1_Code_dcs)) return dansi_Seq__extractCtrlString(reader, dansi_Seq_Kind_dcs, 1) $end(case);
    case_((dansi_c1_Code_sos)) return dansi_Seq__extractCtrlString(reader, dansi_Seq_Kind_sos, 1) $end(case);
    case_((dansi_c1_Code_csi)) return dansi_Seq__extractCsi(reader, 1) $end(case);
    case_((dansi_c1_Code_osc)) return dansi_Seq__extractCtrlString(reader, dansi_Seq_Kind_osc, 1) $end(case);
    case_((dansi_c1_Code_pm)) return dansi_Seq__extractCtrlString(reader, dansi_Seq_Kind_pm, 1) $end(case);
    case_((dansi_c1_Code_apc)) return dansi_Seq__extractCtrlString(reader, dansi_Seq_Kind_apc, 1) $end(case);
    default_() {
        if (dansi_Seq_c1_min_byte <= first && first <= dansi_Seq_c1_max_byte) {
            return dansi_Seq__complete(reader, dansi_Seq_Kind_c1, 1);
        }
        return dansi_Seq__extractText(reader);
    } $end(default);
    }
} $unscoped(fn);

fn_((dansi_Seq_receive(io_Reader in, S$u8 buf))(dansi_Seq_E$dansi_Seq)) {
    claim_assert_nonnullS(buf);
    var reader = io_Buf_Reader_init(in, buf);
    return dansi_Seq_extract(&reader);
};

fn_((dansi_Seq_receiveC0(io_Reader in, S$u8 buf))(E$S$u8) $scope) {
    claim_assert_nonnullS(buf);
    if (buf.len < 1) return_err(E_cause$TooSmallBuffer());
    let byte = try_(io_Reader_readByte(in));
    if (byte > dansi_Seq_c0_max_byte) return_err(E_cause$Unexpected());
    *S_at((buf)[0]) = byte;
    return_ok(S_prefix((buf)(1)));
} $unscoped(fn);

fn_((dansi_Seq_receiveESC(io_Reader in, S$u8 buf))(E$S$u8) $scope) {
    claim_assert_nonnullS(buf);
    if (buf.len < 2) return_err(E_cause$TooSmallBuffer());
    *S_at((buf)[0]) = try_(io_Reader_readByte(in));
    if (*S_at((buf)[0]) != dansi_Seq_esc_byte) return_err(E_cause$Unexpected());
    var_(written, usize) = 1;
    while (written < buf.len) {
        let byte = try_(io_Reader_readByte(in));
        *S_at((buf)[written]) = byte;
        written += 1;
        if (dansi_esc_final_min_byte <= byte && byte <= dansi_esc_final_max_byte) return_ok(S_prefix((buf)(written)));
    }
    return_err(E_cause$TooSmallBuffer());
} $unscoped(fn);

fn_((dansi_Seq_receiveC1(io_Reader in, S$u8 buf))(E$S$u8) $scope) {
    claim_assert_nonnullS(buf);
    if (buf.len < 1) return_err(E_cause$TooSmallBuffer());
    let byte = try_(io_Reader_readByte(in));
    if (byte < dansi_Seq_c1_min_byte || byte > dansi_Seq_c1_max_byte) return_err(E_cause$Unexpected());
    *S_at((buf)[0]) = byte;
    return_ok(S_prefix((buf)(1)));
} $unscoped(fn);

fn_((dansi_Seq_receiveSS2(io_Reader in, S$u8 buf))(E$S$u8)) {
    return dansi_Seq__receivePrefixedOrC1(in, buf, u8_l(dansi_Seq_ss2_7bit_prefix), dansi_c1_Code_ss2);
};

fn_((dansi_Seq_receiveSS3(io_Reader in, S$u8 buf))(E$S$u8)) {
    return dansi_Seq__receivePrefixedOrC1(in, buf, u8_l(dansi_Seq_ss3_7bit_prefix), dansi_c1_Code_ss3);
};

fn_((dansi_Seq_receiveDCS(io_Reader in, S$u8 buf))(E$S$u8)) {
    return dansi_Seq__receiveCtrlString7Or8(in, buf, u8_l(dansi_dcs_7bit_prefix), dansi_dcs_8bit_intro_byte);
};

fn_((dansi_Seq_receiveSOS(io_Reader in, S$u8 buf))(E$S$u8)) {
    return dansi_Seq__receiveCtrlString7Or8(in, buf, u8_l(dansi_sos_7bit_prefix), dansi_sos_8bit_intro_byte);
};

fn_((dansi_Seq_receiveCSI(io_Reader in, S$u8 buf))(E$S$u8)) {
    return dansi_Seq__receiveCsi7Or8(in, buf, u8_l(dansi_csi_7bit_prefix), dansi_csi_8bit_intro_byte);
};

fn_((dansi_Seq_receiveOSC(io_Reader in, S$u8 buf))(E$S$u8)) {
    return dansi_Seq__receiveCtrlString7Or8(in, buf, u8_l(dansi_osc_7bit_prefix), dansi_osc_8bit_intro_byte);
};

fn_((dansi_Seq_receivePM(io_Reader in, S$u8 buf))(E$S$u8)) {
    return dansi_Seq__receiveCtrlString7Or8(in, buf, u8_l(dansi_pm_7bit_prefix), dansi_pm_8bit_intro_byte);
};

fn_((dansi_Seq_receiveAPC(io_Reader in, S$u8 buf))(E$S$u8)) {
    return dansi_Seq__receiveCtrlString7Or8(in, buf, u8_l(dansi_apc_7bit_prefix), dansi_apc_8bit_intro_byte);
};

fn_((dansi_Seq__readable(io_Buf_Reader* reader))(S_const$u8)) {
    return S_slice((reader->buf)$r(reader->start, reader->end)).as_const;
};

fn_((dansi_Seq__ensure(io_Buf_Reader* reader, usize len))(E$void) $scope) {
    while (reader->end - reader->start < len) {
        let old_end = reader->end;
        catch_((io_Buf_Reader_fill(reader))($ignore, return_err(E_cause$UnexpectedEOF())));
        if (reader->end == old_end) return_err(E_cause$UnexpectedEOF());
    }
    return_ok({});
} $unscoped(fn);

fn_((dansi_Seq__complete(io_Buf_Reader* reader, dansi_Seq_Kind kind, usize len))(dansi_Seq_E$dansi_Seq) $scope) {
    let bytes = S_prefix((dansi_Seq__readable(reader))(len));
    catch_((io_Buf_Reader_skip(reader, len))($ignore, return_err(E_cause$dansi_Seq_Incomplete())));
    return_ok(dansi_Seq_from(kind, bytes));
} $unscoped(fn);

fn_((dansi_Seq__extractText(io_Buf_Reader* reader))(dansi_Seq_E$dansi_Seq) $scope) {
    let first = *S_at((dansi_Seq__readable(reader))[0]);
    let len = catch_((utf8_byteSeqLen(first))($ignore, 1));
    catch_((dansi_Seq__ensure(reader, as$(usize)(len)))($ignore, return_err(E_cause$dansi_Seq_Incomplete())));
    return dansi_Seq__complete(reader, dansi_Seq_Kind_text, as$(usize)(len));
} $unscoped(fn);

fn_((dansi_Seq__extractEsc(io_Buf_Reader* reader))(dansi_Seq_E$dansi_Seq) $scope) {
    catch_((dansi_Seq__ensure(reader, 2))($ignore, return_err(E_cause$dansi_Seq_Incomplete())));
    let second = *S_at((dansi_Seq__readable(reader))[1]);
    switch (second) {
    case_((dansi_Seq_ss2_7bit_intro_byte)) {
        catch_((dansi_Seq__ensure(reader, 3))($ignore, return_err(E_cause$dansi_Seq_Incomplete())));
        return dansi_Seq__complete(reader, dansi_Seq_Kind_ss2, 3);
    } $end(case);
    case_((dansi_Seq_ss3_7bit_intro_byte)) {
        catch_((dansi_Seq__ensure(reader, 3))($ignore, return_err(E_cause$dansi_Seq_Incomplete())));
        return dansi_Seq__complete(reader, dansi_Seq_Kind_ss3, 3);
    } $end(case);
    case_((dansi_dcs_7bit_intro_byte)) return dansi_Seq__extractCtrlString(reader, dansi_Seq_Kind_dcs, 2) $end(case);
    case_((dansi_sos_7bit_intro_byte)) return dansi_Seq__extractCtrlString(reader, dansi_Seq_Kind_sos, 2) $end(case);
    case_((dansi_csi_7bit_intro_byte)) return dansi_Seq__extractCsi(reader, 2) $end(case);
    case_((dansi_osc_7bit_intro_byte)) return dansi_Seq__extractCtrlString(reader, dansi_Seq_Kind_osc, 2) $end(case);
    case_((dansi_pm_7bit_intro_byte)) return dansi_Seq__extractCtrlString(reader, dansi_Seq_Kind_pm, 2) $end(case);
    case_((dansi_apc_7bit_intro_byte)) return dansi_Seq__extractCtrlString(reader, dansi_Seq_Kind_apc, 2) $end(case);
    default_() return dansi_Seq__complete(reader, dansi_Seq_Kind_esc, 2) $end(default);
    }
} $unscoped(fn);

fn_((dansi_Seq__extractCsi(io_Buf_Reader* reader, usize prefix_len))(dansi_Seq_E$dansi_Seq) $scope) {
    var_(idx, usize) = prefix_len;
    while (idx < reader->buf.len) {
        catch_((dansi_Seq__ensure(reader, idx + 1))($ignore, return_err(E_cause$dansi_Seq_Incomplete())));
        let byte = *S_at((dansi_Seq__readable(reader))[idx]);
        if (dansi_csi_final_min_byte <= byte && byte <= dansi_csi_final_max_byte) {
            return dansi_Seq__complete(reader, dansi_Seq_Kind_csi, idx + 1);
        }
        idx += 1;
    }
    return_err(E_cause$dansi_Seq_TooLong());
} $unscoped(fn);

fn_((dansi_Seq__extractCtrlString(io_Buf_Reader* reader, dansi_Seq_Kind kind, usize prefix_len))(dansi_Seq_E$dansi_Seq) $scope) {
    var_(idx, usize) = prefix_len;
    while (idx < reader->buf.len) {
        catch_((dansi_Seq__ensure(reader, idx + 1))($ignore, return_err(E_cause$dansi_Seq_Incomplete())));
        let bytes = dansi_Seq__readable(reader);
        let byte = *S_at((bytes)[idx]);
        if (byte == dansi_Seq_bel_byte || byte == dansi_Seq_st_8bit_byte) return dansi_Seq__complete(reader, kind, idx + 1);
        if (byte == dansi_Seq_esc_byte) {
            catch_((dansi_Seq__ensure(reader, idx + 2))($ignore, return_err(E_cause$dansi_Seq_Incomplete())));
            if (*S_at((dansi_Seq__readable(reader))[idx + 1]) == dansi_Seq_st_7bit_final_byte) {
                return dansi_Seq__complete(reader, kind, idx + 2);
            }
        }
        idx += 1;
    }
    return_err(E_cause$dansi_Seq_TooLong());
} $unscoped(fn);

fn_((dansi_Seq__receivePrefixedOrC1(io_Reader in, S$u8 buf, S_const$u8 prefix, u8 c1))(E$S$u8) $scope) {
    if (buf.len < 2) return_err(E_cause$TooSmallBuffer());
    let first = try_(io_Reader_readByte(in));
    if (first == c1) {
        *S_at((buf)[0]) = first;
        *S_at((buf)[1]) = try_(io_Reader_readByte(in));
        return_ok(S_prefix((buf)(2)));
    }
    if (buf.len < prefix.len + 1) return_err(E_cause$TooSmallBuffer());
    if (first != *S_at((prefix)[0])) return_err(E_cause$Unexpected());
    *S_at((buf)[0]) = first;
    var_(idx, usize) = 1;
    while (idx < prefix.len) {
        let byte = try_(io_Reader_readByte(in));
        if (byte != *S_at((prefix)[idx])) return_err(E_cause$Unexpected());
        *S_at((buf)[idx]) = byte;
        idx += 1;
    }
    *S_at((buf)[idx]) = try_(io_Reader_readByte(in));
    return_ok(S_prefix((buf)(idx + 1)));
} $unscoped(fn);

fn_((dansi_Seq__receiveCsi7Or8(io_Reader in, S$u8 buf, S_const$u8 prefix, u8 c1))(E$S$u8) $scope) {
    if (buf.len < 2) return_err(E_cause$TooSmallBuffer());
    let first = try_(io_Reader_readByte(in));
    var_(written, usize) = 0;
    if (first == c1) {
        *S_at((buf)[0]) = first;
        written = 1;
    } else {
        if (buf.len < prefix.len + 1) return_err(E_cause$TooSmallBuffer());
        if (first != *S_at((prefix)[0])) return_err(E_cause$Unexpected());
        *S_at((buf)[0]) = first;
        written = 1;
        while (written < prefix.len) {
            let byte = try_(io_Reader_readByte(in));
            if (byte != *S_at((prefix)[written])) return_err(E_cause$Unexpected());
            *S_at((buf)[written]) = byte;
            written += 1;
        }
    }
    while (written < buf.len) {
        let byte = try_(io_Reader_readByte(in));
        *S_at((buf)[written]) = byte;
        written += 1;
        if (dansi_csi_final_min_byte <= byte && byte <= dansi_csi_final_max_byte) return_ok(S_prefix((buf)(written)));
    }
    return_err(E_cause$TooSmallBuffer());
} $unscoped(fn);

fn_((dansi_Seq__receiveCtrlString7Or8(io_Reader in, S$u8 buf, S_const$u8 prefix, u8 c1))(E$S$u8) $scope) {
    if (buf.len < 2) return_err(E_cause$TooSmallBuffer());
    let first = try_(io_Reader_readByte(in));
    var_(written, usize) = 0;
    if (first == c1) {
        *S_at((buf)[0]) = first;
        written = 1;
    } else {
        if (buf.len < prefix.len + 1) return_err(E_cause$TooSmallBuffer());
        if (first != *S_at((prefix)[0])) return_err(E_cause$Unexpected());
        *S_at((buf)[0]) = first;
        written = 1;
        while (written < prefix.len) {
            let byte = try_(io_Reader_readByte(in));
            if (byte != *S_at((prefix)[written])) return_err(E_cause$Unexpected());
            *S_at((buf)[written]) = byte;
            written += 1;
        }
    }
    while (written < buf.len) {
        let byte = try_(io_Reader_readByte(in));
        *S_at((buf)[written]) = byte;
        written += 1;
        if (byte == dansi_Seq_bel_byte || byte == dansi_Seq_st_8bit_byte) return_ok(S_prefix((buf)(written)));
        if (byte == dansi_Seq_esc_byte) {
            if (written >= buf.len) return_err(E_cause$TooSmallBuffer());
            let next = try_(io_Reader_readByte(in));
            *S_at((buf)[written]) = next;
            written += 1;
            if (next == dansi_Seq_st_7bit_final_byte) return_ok(S_prefix((buf)(written)));
        }
    }
    return_err(E_cause$TooSmallBuffer());
} $unscoped(fn);

#include "dansi-core/Seq.h"
#include <dh/io/common.h>
#include <dh/utf8.h>

$static fn_((dansi_Seq__readable(io_Buf_Reader* reader))(S_const$u8)) {
    return S_slice((reader->buf)$r(reader->start, reader->end)).as_const;
};

$static fn_((dansi_Seq__ensure(io_Buf_Reader* reader, usize len))(E$void) $scope) {
    while (reader->end - reader->start < len) {
        let old_end = reader->end;
        catch_((io_Buf_Reader_fill(reader))($ignore, return_err(E_cause$IOUnexpectedEof())));
        if (reader->end == old_end) {
            return_err(E_cause$IOUnexpectedEof());
        }
    }
    return_ok({});
} $unscoped(fn);

$static fn_((dansi_Seq__complete(io_Buf_Reader* reader, dansi_Seq_Kind kind, usize len))(dansi_Seq_E$dansi_Seq) $scope) {
    let bytes = S_prefix((dansi_Seq__readable(reader))(len));
    catch_((io_Buf_Reader_skip(reader, len))($ignore, return_err(E_cause$dansi_Seq_Incomplete())));
    return_ok(dansi_Seq_from(kind, bytes));
} $unscoped(fn);

$static fn_((dansi_Seq__extractText(io_Buf_Reader* reader))(dansi_Seq_E$dansi_Seq) $scope) {
    let first = *S_at((dansi_Seq__readable(reader))[0]);
    let len = catch_((utf8_byteSeqLen(first))($ignore, 1));
    catch_((dansi_Seq__ensure(reader, len))($ignore, return_err(E_cause$dansi_Seq_Incomplete())));
    return dansi_Seq__complete(reader, dansi_Seq_Kind_text, len);
} $unscoped(fn);

$static fn_((dansi_Seq__extractString(io_Buf_Reader* reader, dansi_Seq_Kind kind))(dansi_Seq_E$dansi_Seq) $scope) {
    var_(idx, usize) = 2;
    while (idx < reader->buf.len) {
        catch_((dansi_Seq__ensure(reader, idx + 1))($ignore, return_err(E_cause$dansi_Seq_Incomplete())));
        let bytes = dansi_Seq__readable(reader);
        let ch = *S_at((bytes)[idx]);
        if (ch == 0x07) {
            return dansi_Seq__complete(reader, kind, idx + 1);
        }
        if (ch == 0x1B) {
            catch_((dansi_Seq__ensure(reader, idx + 2))($ignore, return_err(E_cause$dansi_Seq_Incomplete())));
            let refreshed = dansi_Seq__readable(reader);
            if (*S_at((refreshed)[idx + 1]) == '\\') {
                return dansi_Seq__complete(reader, kind, idx + 2);
            }
        }
        idx += 1;
    }
    return_err(E_cause$dansi_Seq_TooLong());
} $unscoped(fn);

$static fn_((dansi_Seq__extractCSI(io_Buf_Reader* reader))(dansi_Seq_E$dansi_Seq) $scope) {
    var_(idx, usize) = 2;
    while (idx < reader->buf.len) {
        catch_((dansi_Seq__ensure(reader, idx + 1))($ignore, return_err(E_cause$dansi_Seq_Incomplete())));
        let ch = *S_at((dansi_Seq__readable(reader))[idx]);
        if (0x40 <= ch && ch <= 0x7E) {
            return dansi_Seq__complete(reader, dansi_Seq_Kind_csi, idx + 1);
        }
        idx += 1;
    }
    return_err(E_cause$dansi_Seq_TooLong());
} $unscoped(fn);

fn_((dansi_Seq_extract(io_Buf_Reader* reader))(dansi_Seq_E$dansi_Seq) $scope) {
    claim_assert_nonnull(reader);
    catch_((dansi_Seq__ensure(reader, 1))($ignore, return_err(E_cause$dansi_Seq_Incomplete())));
    let bytes = dansi_Seq__readable(reader);
    if (*S_at((bytes)[0]) != 0x1B) {
        return dansi_Seq__extractText(reader);
    }

    catch_((dansi_Seq__ensure(reader, 2))($ignore, return_err(E_cause$dansi_Seq_Incomplete())));
    let esc_bytes = dansi_Seq__readable(reader);
    switch (*S_at((esc_bytes)[1])) {
    case '[': return dansi_Seq__extractCSI(reader);
    case 'O':
        catch_((dansi_Seq__ensure(reader, 3))($ignore, return_err(E_cause$dansi_Seq_Incomplete())));
        return dansi_Seq__complete(reader, dansi_Seq_Kind_ss3, 3);
    case ']': return dansi_Seq__extractString(reader, dansi_Seq_Kind_osc);
    case 'P': return dansi_Seq__extractString(reader, dansi_Seq_Kind_dcs);
    default: return dansi_Seq__complete(reader, dansi_Seq_Kind_esc, 2);
    }
} $unscoped(fn);

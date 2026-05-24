#include "dansi-core/screen.h"
#include "dansi-core/cursor.h"
#include "dansi-core/Seq.h"
#include <dh/fmt/common.h>
#include <dh/mem/common.h>

/*========== External Definitions ===========================================*/

T_use$((u8)(
    mem_Delim,
    mem_TokzIter,
    mem_tokzAny,
    mem_TokzIter_next
));

$static fn_((dansi_screen__receiveReport(io_Reader in, S$u8 buf))(E$S$u8) $scope) {
    return dansi_Seq_receiveCSI(in, buf);
} $unscoped(fn);

$static fn_((dansi_screen__parsePixelSizeReport(
    S_const$u8 report,
    S_const$u8 expected_kind
))(dansi_screen_E$dansi_screen_PixelSize) $scope) {
    if (!mem_startsWithBytes(report, u8_l(dansi_utils_csi))) {
        return_err(E_cause$dansi_screen_InvalidResponse());
    }
    if (report.len <= u8_l(dansi_utils_csi).len || *S_at((report)[report.len - 1]) != 't') {
        return_err(E_cause$dansi_screen_InvalidResponse());
    }

    var it = mem_tokzAny$u8(
        S_suffix((report)(u8_l(dansi_utils_csi).len)),
        u8_l(dansi_utils_sep dansi_utils_screen_response_size)
    );
    let kind_str = orelse_((mem_TokzIter_next$u8(&it))(return_err(E_cause$dansi_screen_InvalidResponse())));
    let height_str = orelse_((mem_TokzIter_next$u8(&it))(return_err(E_cause$dansi_screen_InvalidResponse())));
    let width_str = orelse_((mem_TokzIter_next$u8(&it))(return_err(E_cause$dansi_screen_InvalidResponse())));
    if (!mem_eqlBytes(kind_str, expected_kind)) {
        return_err(E_cause$dansi_screen_InvalidResponse());
    }

    let height = catch_((fmt_parse$u16(height_str, 10))($ignore, return_err(E_cause$dansi_screen_InvalidResponse())));
    let width = catch_((fmt_parse$u16(width_str, 10))($ignore, return_err(E_cause$dansi_screen_InvalidResponse())));
    return_ok({ .width = width, .height = height });
} $unscoped(fn);

fn_((dansi_screen_enterAlternate(void))(S_const$u8)) {
    return u8_l(dansi_screen_enterAlternate_static());
};

fn_((dansi_screen_enterAlternateWrite(io_Writer writer))(E$void)) {
    return io_Writer_writeBytes(writer, dansi_screen_enterAlternate());
};

fn_((dansi_screen_exitAlternate(void))(S_const$u8)) {
    return u8_l(dansi_screen_exitAlternate_static());
};

fn_((dansi_screen_exitAlternateWrite(io_Writer writer))(E$void)) {
    return io_Writer_writeBytes(writer, dansi_screen_exitAlternate());
};

fn_((dansi_screen_clearFromCursor(void))(S_const$u8)) {
    return u8_l(dansi_screen_clearFromCursor_static());
};

fn_((dansi_screen_clearFromCursorWrite(io_Writer writer))(E$void)) {
    return io_Writer_writeBytes(writer, dansi_screen_clearFromCursor());
};

fn_((dansi_screen_clearToCursor(void))(S_const$u8)) {
    return u8_l(dansi_screen_clearToCursor_static());
};

fn_((dansi_screen_clearToCursorWrite(io_Writer writer))(E$void)) {
    return io_Writer_writeBytes(writer, dansi_screen_clearToCursor());
};

fn_((dansi_screen_clear(void))(S_const$u8)) {
    return u8_l(dansi_screen_clear_static());
};

fn_((dansi_screen_clearWrite(io_Writer writer))(E$void)) {
    return io_Writer_writeBytes(writer, dansi_screen_clear());
};

fn_((dansi_screen_requestCellSizePixels(void))(S_const$u8)) {
    return u8_l(dansi_screen_requestCellSizePixels_static());
};

fn_((dansi_screen_requestCellSizePixelsWrite(io_Writer writer))(E$void)) {
    return io_Writer_writeBytes(writer, dansi_screen_requestCellSizePixels());
};

fn_((dansi_screen_receiveCellSizePixelsReport(io_Reader in, S$u8 buf))(E$S$u8)) {
    return dansi_screen__receiveReport(in, buf);
};

fn_((dansi_screen_parseCellSizePixelsReport(S_const$u8 report))(dansi_screen_E$dansi_screen_PixelSize)) {
    return dansi_screen__parsePixelSizeReport(report, u8_l(dansi_utils_screen_response_cell_size_pixels));
};

fn_((dansi_screen_fetchCellSizePixels(io_Writer out, io_Reader in, S$u8 buf))(dansi_screen_E$dansi_screen_PixelSize) $scope) {
    try_(dansi_screen_requestCellSizePixelsWrite(out));
    let report = try_(dansi_screen_receiveCellSizePixelsReport(in, buf));
    return dansi_screen_parseCellSizePixelsReport(report.as_const);
} $unscoped(fn);

fn_((dansi_screen_requestTextAreaSizeChars(void))(S_const$u8)) {
    return u8_l(dansi_screen_requestTextAreaSizeChars_static());
};

fn_((dansi_screen_requestTextAreaSizeCharsWrite(io_Writer writer))(E$void)) {
    return io_Writer_writeBytes(writer, dansi_screen_requestTextAreaSizeChars());
};

fn_((dansi_screen_receiveTextAreaSizeCharsReport(io_Reader in, S$u8 buf))(E$S$u8)) {
    return dansi_screen__receiveReport(in, buf);
};

fn_((dansi_screen_parseTextAreaSizeCharsReport(S_const$u8 report))(dansi_screen_E$dansi_screen_Size) $scope) {
    let size = try_(dansi_screen__parsePixelSizeReport(report, u8_l(dansi_utils_screen_response_text_area_size_chars)));
    return_ok({
        .cols = size.width,
        .rows = size.height,
    });
} $unscoped(fn);

fn_((dansi_screen_fetchTextAreaSizeChars(io_Writer out, io_Reader in, S$u8 buf))(dansi_screen_E$dansi_screen_Size) $scope) {
    try_(dansi_screen_requestTextAreaSizeCharsWrite(out));
    let report = try_(dansi_screen_receiveTextAreaSizeCharsReport(in, buf));
    return dansi_screen_parseTextAreaSizeCharsReport(report.as_const);
} $unscoped(fn);

fn_((dansi_screen_requestTextAreaSizePixels(void))(S_const$u8)) {
    return u8_l(dansi_screen_requestTextAreaSizePixels_static());
};

fn_((dansi_screen_requestTextAreaSizePixelsWrite(io_Writer writer))(E$void)) {
    return io_Writer_writeBytes(writer, dansi_screen_requestTextAreaSizePixels());
};

fn_((dansi_screen_receiveTextAreaSizePixelsReport(io_Reader in, S$u8 buf))(E$S$u8)) {
    return dansi_screen__receiveReport(in, buf);
};

fn_((dansi_screen_parseTextAreaSizePixelsReport(S_const$u8 report))(dansi_screen_E$dansi_screen_PixelSize)) {
    return dansi_screen__parsePixelSizeReport(report, u8_l(dansi_utils_screen_response_text_area_size_pixels));
};

fn_((dansi_screen_fetchTextAreaSizePixels(io_Writer out, io_Reader in, S$u8 buf))(dansi_screen_E$dansi_screen_PixelSize) $scope) {
    try_(dansi_screen_requestTextAreaSizePixelsWrite(out));
    let report = try_(dansi_screen_receiveTextAreaSizePixelsReport(in, buf));
    return dansi_screen_parseTextAreaSizePixelsReport(report.as_const);
} $unscoped(fn);

fn_((dansi_screen_fetchSizeByCursorPos(io_Writer out, io_Reader in, S$u8 buf))(dansi_screen_E$dansi_screen_Size) $guard) {
    try_(dansi_cursor_storePosWrite(out));
    defer_(catch_((dansi_cursor_restorePosWrite(out))($ignore, $do_nothing)));
    try_(dansi_cursor_moveToWrite(u16_limit_max, u16_limit_max, out));
    let pos = try_(dansi_cursor_fetchPos(out, in, buf));
    return_ok({
        .cols = pos.col,
        .rows = pos.row,
    });
} $unguarded(fn);

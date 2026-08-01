#include "dansi-xterm/screen.h"
#include "dansi-core/Seq.h"

$static fn_((dansi_xterm_screen__parsePixelSize(S_const$u8 report, u16 response_code))(dansi_xterm_screen_E$dansi_xterm_screen_PixelSize) $scope) {
    let frame = catch_((dansi_csi_parse(report))(
        $ignore, return_err(E_cause$dansi_xterm_screen_InvalidResponse())
    ));
    if (frame.final != dansi_xterm_screen_report_final_byte) return_err(E_cause$dansi_xterm_screen_InvalidResponse());
    let code = orelse_((dansi_csi_Frame_paramAtAsU16(frame, dansi_xterm_screen_report_param_code))(
        return_err(E_cause$dansi_xterm_screen_InvalidResponse())
    ));
    if (code != response_code) return_err(E_cause$dansi_xterm_screen_InvalidResponse());
    let height = orelse_((dansi_csi_Frame_paramAtAsU16(frame, dansi_xterm_screen_report_param_height))(
        return_err(E_cause$dansi_xterm_screen_InvalidResponse())
    ));
    let width = orelse_((dansi_csi_Frame_paramAtAsU16(frame, dansi_xterm_screen_report_param_width))(
        return_err(E_cause$dansi_xterm_screen_InvalidResponse())
    ));
    return_ok({ .width = width, .height = height });
} $unscoped(fn);

$static fn_((dansi_xterm_screen__parseCellCount(S_const$u8 report, u16 response_code))(dansi_xterm_screen_E$dansi_xterm_screen_CellCount) $scope) {
    let frame = catch_((dansi_csi_parse(report))(
        $ignore, return_err(E_cause$dansi_xterm_screen_InvalidResponse())
    ));
    if (frame.final != dansi_xterm_screen_report_final_byte) return_err(E_cause$dansi_xterm_screen_InvalidResponse());
    let code = orelse_((dansi_csi_Frame_paramAtAsU16(frame, dansi_xterm_screen_report_param_code))(
        return_err(E_cause$dansi_xterm_screen_InvalidResponse())
    ));
    if (code != response_code) return_err(E_cause$dansi_xterm_screen_InvalidResponse());
    let rows = orelse_((dansi_csi_Frame_paramAtAsU16(frame, dansi_xterm_screen_report_param_height))(
        return_err(E_cause$dansi_xterm_screen_InvalidResponse())
    ));
    let cols = orelse_((dansi_csi_Frame_paramAtAsU16(frame, dansi_xterm_screen_report_param_width))(
        return_err(E_cause$dansi_xterm_screen_InvalidResponse())
    ));
    return_ok({ .cols = cols, .rows = rows });
} $unscoped(fn);

fn_((dansi_xterm_screen_enterAlternate(void))(S_const$u8)) {
    return u8_l(dansi_xterm_screen_enterAlternate_static());
};

fn_((dansi_xterm_screen_enterAlternateWrite(io_Writer out))(io_WriteE$void)) {
    return io_Writer_writeBytes(out, dansi_xterm_screen_enterAlternate());
};

fn_((dansi_xterm_screen_exitAlternate(void))(S_const$u8)) {
    return u8_l(dansi_xterm_screen_exitAlternate_static());
};

fn_((dansi_xterm_screen_exitAlternateWrite(io_Writer out))(io_WriteE$void)) {
    return io_Writer_writeBytes(out, dansi_xterm_screen_exitAlternate());
};

fn_((dansi_xterm_screen_enterAlternateSaveCursor(void))(S_const$u8)) {
    return u8_l(dansi_xterm_screen_enterAlternateSaveCursor_static());
};

fn_((dansi_xterm_screen_enterAlternateSaveCursorWrite(io_Writer out))(io_WriteE$void)) {
    return io_Writer_writeBytes(out, dansi_xterm_screen_enterAlternateSaveCursor());
};

fn_((dansi_xterm_screen_exitAlternateSaveCursor(void))(S_const$u8)) {
    return u8_l(dansi_xterm_screen_exitAlternateSaveCursor_static());
};

fn_((dansi_xterm_screen_exitAlternateSaveCursorWrite(io_Writer out))(io_WriteE$void)) {
    return io_Writer_writeBytes(out, dansi_xterm_screen_exitAlternateSaveCursor());
};

fn_((dansi_xterm_screen_requestCellPixels(void))(S_const$u8)) {
    return u8_l(dansi_xterm_screen_requestCellPixels_static());
};

fn_((dansi_xterm_screen_requestCellPixelsWrite(io_Writer out))(io_WriteE$void)) {
    return io_Writer_writeBytes(out, dansi_xterm_screen_requestCellPixels());
};

fn_((dansi_xterm_screen_receiveCellPixelsReport(io_Reader in, S$u8 buf))(E$S$u8)) {
    return dansi_Seq_receiveCSI(in, buf);
};

fn_((dansi_xterm_screen_parseCellPixelsReport(S_const$u8 report))(dansi_xterm_screen_E$dansi_xterm_screen_PixelSize)) {
    return dansi_xterm_screen__parsePixelSize(report, dansi_xterm_screen_response_cell_pixels);
};

fn_((dansi_xterm_screen_fetchCellPixels(
    io_Writer out, io_Reader in, S$u8 buf
))(dansi_xterm_screen_E$dansi_xterm_screen_PixelSize) $scope) {
    try_(dansi_xterm_screen_requestCellPixelsWrite(out));
    let report = try_(dansi_xterm_screen_receiveCellPixelsReport(in, buf));
    return dansi_xterm_screen_parseCellPixelsReport(report.as_const);
} $unscoped(fn);

fn_((dansi_xterm_screen_requestTextAreaCells(void))(S_const$u8)) {
    return u8_l(dansi_xterm_screen_requestTextAreaCells_static());
};

fn_((dansi_xterm_screen_requestTextAreaCellsWrite(io_Writer out))(io_WriteE$void)) {
    return io_Writer_writeBytes(out, dansi_xterm_screen_requestTextAreaCells());
};

fn_((dansi_xterm_screen_receiveTextAreaCellsReport(io_Reader in, S$u8 buf))(E$S$u8)) {
    return dansi_Seq_receiveCSI(in, buf);
};

fn_((dansi_xterm_screen_parseTextAreaCellsReport(S_const$u8 report))(dansi_xterm_screen_E$dansi_xterm_screen_CellCount)) {
    return dansi_xterm_screen__parseCellCount(report, dansi_xterm_screen_response_text_area_cells);
};

fn_((dansi_xterm_screen_fetchTextAreaCells(
    io_Writer out, io_Reader in, S$u8 buf
))(dansi_xterm_screen_E$dansi_xterm_screen_CellCount) $scope) {
    try_(dansi_xterm_screen_requestTextAreaCellsWrite(out));
    let report = try_(dansi_xterm_screen_receiveTextAreaCellsReport(in, buf));
    return dansi_xterm_screen_parseTextAreaCellsReport(report.as_const);
} $unscoped(fn);

fn_((dansi_xterm_screen_requestScreenCells(void))(S_const$u8)) {
    return u8_l(dansi_xterm_screen_requestScreenCells_static());
};

fn_((dansi_xterm_screen_requestScreenCellsWrite(io_Writer out))(io_WriteE$void)) {
    return io_Writer_writeBytes(out, dansi_xterm_screen_requestScreenCells());
};

fn_((dansi_xterm_screen_receiveScreenCellsReport(io_Reader in, S$u8 buf))(E$S$u8)) {
    return dansi_Seq_receiveCSI(in, buf);
};

fn_((dansi_xterm_screen_parseScreenCellsReport(S_const$u8 report))(dansi_xterm_screen_E$dansi_xterm_screen_CellCount)) {
    return dansi_xterm_screen__parseCellCount(report, dansi_xterm_screen_response_screen_cells);
};

fn_((dansi_xterm_screen_fetchScreenCells(
    io_Writer out, io_Reader in, S$u8 buf
))(dansi_xterm_screen_E$dansi_xterm_screen_CellCount) $scope) {
    try_(dansi_xterm_screen_requestScreenCellsWrite(out));
    let report = try_(dansi_xterm_screen_receiveScreenCellsReport(in, buf));
    return dansi_xterm_screen_parseScreenCellsReport(report.as_const);
} $unscoped(fn);

fn_((dansi_xterm_screen_requestTextAreaPixels(void))(S_const$u8)) {
    return u8_l(dansi_xterm_screen_requestTextAreaPixels_static());
};

fn_((dansi_xterm_screen_requestTextAreaPixelsWrite(io_Writer out))(io_WriteE$void)) {
    return io_Writer_writeBytes(out, dansi_xterm_screen_requestTextAreaPixels());
};

fn_((dansi_xterm_screen_receiveTextAreaPixelsReport(io_Reader in, S$u8 buf))(E$S$u8)) {
    return dansi_Seq_receiveCSI(in, buf);
};

fn_((dansi_xterm_screen_parseTextAreaPixelsReport(S_const$u8 report))(dansi_xterm_screen_E$dansi_xterm_screen_PixelSize)) {
    return dansi_xterm_screen__parsePixelSize(report, dansi_xterm_screen_response_text_area_pixels);
};

fn_((dansi_xterm_screen_fetchTextAreaPixels(
    io_Writer out, io_Reader in, S$u8 buf
))(dansi_xterm_screen_E$dansi_xterm_screen_PixelSize) $scope) {
    try_(dansi_xterm_screen_requestTextAreaPixelsWrite(out));
    let report = try_(dansi_xterm_screen_receiveTextAreaPixelsReport(in, buf));
    return dansi_xterm_screen_parseTextAreaPixelsReport(report.as_const);
} $unscoped(fn);

fn_((dansi_xterm_screen_requestScreenPixels(void))(S_const$u8)) {
    return u8_l(dansi_xterm_screen_requestScreenPixels_static());
};

fn_((dansi_xterm_screen_requestScreenPixelsWrite(io_Writer out))(io_WriteE$void)) {
    return io_Writer_writeBytes(out, dansi_xterm_screen_requestScreenPixels());
};

fn_((dansi_xterm_screen_receiveScreenPixelsReport(io_Reader in, S$u8 buf))(E$S$u8)) {
    return dansi_Seq_receiveCSI(in, buf);
};

fn_((dansi_xterm_screen_parseScreenPixelsReport(S_const$u8 report))(dansi_xterm_screen_E$dansi_xterm_screen_PixelSize)) {
    return dansi_xterm_screen__parsePixelSize(report, dansi_xterm_screen_response_screen_pixels);
};

fn_((dansi_xterm_screen_fetchScreenPixels(
    io_Writer out, io_Reader in, S$u8 buf
))(dansi_xterm_screen_E$dansi_xterm_screen_PixelSize) $scope) {
    try_(dansi_xterm_screen_requestScreenPixelsWrite(out));
    let report = try_(dansi_xterm_screen_receiveScreenPixelsReport(in, buf));
    return dansi_xterm_screen_parseScreenPixelsReport(report.as_const);
} $unscoped(fn);

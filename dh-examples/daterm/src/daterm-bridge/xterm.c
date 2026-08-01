#include "daterm-bridge/xterm.h"

/*========== Internal Declarations ==========================================*/

$attr($must_check)
$static fn_((daterm_xterm__cellPixelsRequestWrite(P$raw ctx, io_Writer out))(E$void));
$attr($must_check)
$static fn_((daterm_xterm__cellPixelsMatch(P$raw ctx, dansi_Seq seq, P$raw out))(E$daterm_TxnMatch));

/*========== External Definitions ===========================================*/

fn_((daterm_xterm_enableMouse(
    daterm_ANSI* ansi, daterm_xterm_MouseCfg cfg
))(E$void) $guard) {
    claim_assert_nonnull(ansi);
#if plat_is_windows
    if (ansi->input_mode != daterm_ANSI_InputMode_vt) {
        return_err(E_cause$daterm_ANSI_Unsupported());
    }
#endif
    if (cfg.encoding != dansi_xterm_mouse_Encoding_sgr
        && cfg.encoding != dansi_xterm_mouse_Encoding_sgr_pixels) {
        return_err(E_cause$daterm_ANSI_Unsupported());
    }
    if (ansi->is_tracking_mouse) try_(daterm_xterm_disableMouse(ansi));
    let out = io_TTY_writer(&ansi->tty);
    try_(dansi_xterm_mouse_enableReportModeWrite(cfg.report_mode, out));
    errdefer_($ignore, catch_((dansi_xterm_mouse_disableReportModeWrite(cfg.report_mode, out))($ignore, $do_nothing)));
    try_(dansi_xterm_mouse_enableEncodingWrite(cfg.encoding, out));
    ansi->is_tracking_mouse = true;
    ansi->mouse_report_mode_ = as$(u16)(cfg.report_mode);
    ansi->mouse_pos_kind = cfg.encoding == dansi_xterm_mouse_Encoding_sgr_pixels
                             ? daterm_mouse_PosKind_pixel
                             : daterm_mouse_PosKind_cell;
    return_ok({});
} $unguarded(fn);

fn_((daterm_xterm_disableMouse(daterm_ANSI* ansi))(E$void) $scope) {
    claim_assert_nonnull(ansi);
    if (!ansi->is_tracking_mouse) return_ok({});
    let out = io_TTY_writer(&ansi->tty);
    let_(encoding, dansi_xterm_mouse_Encoding) = ansi->mouse_pos_kind == daterm_mouse_PosKind_pixel
                                                   ? dansi_xterm_mouse_Encoding_sgr_pixels
                                                   : dansi_xterm_mouse_Encoding_sgr;
    try_(dansi_xterm_mouse_disableEncodingWrite(encoding, out));
    try_(dansi_xterm_mouse_disableReportModeWrite(as$(dansi_xterm_mouse_ReportMode)(ansi->mouse_report_mode_), out));
    ansi->is_tracking_mouse = false;
    ansi->mouse_report_mode_ = 0;
    ansi->mouse_pos_kind = daterm_mouse_PosKind_cell;
    return_ok({});
} $unscoped(fn);

fn_((daterm_xterm_setFocusTracking(daterm_ANSI* ansi, bool enabled))(E$void) $scope) {
    claim_assert_nonnull(ansi);
#if plat_is_windows
    if (ansi->input_mode != daterm_ANSI_InputMode_vt) {
        return_err(E_cause$daterm_ANSI_Unsupported());
    }
#endif
    if (ansi->is_tracking_focus == enabled) return_ok({});
    try_(dansi_xterm_focus_setTrackingWrite(enabled, io_TTY_writer(&ansi->tty)));
    ansi->is_tracking_focus = enabled;
    return_ok({});
} $unscoped(fn);

fn_((daterm_xterm_enableFocusTracking(daterm_ANSI* ansi))(E$void)) {
    return daterm_xterm_setFocusTracking(ansi, true);
};

fn_((daterm_xterm_disableFocusTracking(daterm_ANSI* ansi))(E$void)) {
    return daterm_xterm_setFocusTracking(ansi, false);
};

fn_((daterm_xterm_setEnhancedKeyboard(daterm_ANSI* ansi, bool enabled))(E$void) $scope) {
    claim_assert_nonnull(ansi);
#if plat_is_windows
    if (ansi->input_mode != daterm_ANSI_InputMode_vt) {
        return_err(E_cause$daterm_ANSI_Unsupported());
    }
#endif
    if (ansi->is_enhanced_keyboard == enabled) return_ok({});
    let out = io_TTY_writer(&ansi->tty);
    if (enabled) {
        try_(dansi_xterm_key_enableEnhancedWrite(out));
    } else {
        try_(dansi_xterm_key_disableEnhancedWrite(out));
    }
    ansi->is_enhanced_keyboard = enabled;
    return_ok({});
} $unscoped(fn);

fn_((daterm_xterm_enableEnhancedKeyboard(daterm_ANSI* ansi))(E$void)) {
    return daterm_xterm_setEnhancedKeyboard(ansi, true);
};

fn_((daterm_xterm_disableEnhancedKeyboard(daterm_ANSI* ansi))(E$void)) {
    return daterm_xterm_setEnhancedKeyboard(ansi, false);
};

fn_((daterm_xterm_fetchCellPixels(
    daterm_Term term,
    time_Dur timeout,
    daterm_PixelSize* out
))(daterm_Txn_E$Void)) {
    claim_assert_nonnull(out);
    return daterm_Term_runTxn(
        term,
        (daterm_Txn){
            .timeout = timeout,
            .ctx = ptrCast$((P$raw)(out)),
            .out = out,
            .requestWriteFn = daterm_xterm__cellPixelsRequestWrite,
            .matchFn = daterm_xterm__cellPixelsMatch,
        }
    );
};

/*========== Internal Definitions ===========================================*/

fn_((daterm_xterm__cellPixelsRequestWrite(P$raw ctx, io_Writer out))(E$void) $scope) {
    let_ignore = ctx;
    try_(dansi_xterm_screen_requestCellPixelsWrite(out));
    return_ok({});
} $unscoped(fn);

fn_((daterm_xterm__cellPixelsMatch(P$raw ctx, dansi_Seq seq, P$raw out))(E$daterm_TxnMatch) $scope) {
    let_ignore = ctx;
    claim_assert_nonnull(out);
    if (seq.kind != dansi_Seq_Kind_csi) return_ok(daterm_TxnMatch_no);
    let report = catch_((dansi_xterm_screen_parseCellPixelsReport(seq.bytes))(
        $ignore, return_ok(daterm_TxnMatch_no)
    ));
    let result = ptrAlignCast$((daterm_PixelSize*)(out));
    *result = (daterm_PixelSize){
        .width = report.width,
        .height = report.height,
    };
    return_ok(daterm_TxnMatch_done);
} $unscoped(fn);

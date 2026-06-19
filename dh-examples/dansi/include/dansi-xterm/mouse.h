/**
 * @copyright Copyright (c) 2026 Gyeongtae Kim. All rights reserved.
 * @license   Proprietary and confidential. Unauthorized use, reproduction,
 *            or distribution is strictly prohibited.
 *
 * @file    mouse.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2026-06-19 (date of creation)
 * @updated 2026-06-19 (date of last update)
 * @ingroup dasae-headers-workspace(dh-workspace)/dansi
 * @prefix  dansi_xterm_mouse
 */
#pragma once
#ifndef dansi_xterm_mouse__included
#define dansi_xterm_mouse__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "mode.h"

/*========== Macros and Declarations ========================================*/

typedef enum_((dansi_xterm_mouse_Btn $fits($packed))(
    dansi_xterm_mouse_Btn_left,
    dansi_xterm_mouse_Btn_middle,
    dansi_xterm_mouse_Btn_right,
    dansi_xterm_mouse_Btn_backward,
    dansi_xterm_mouse_Btn_forward,
    dansi_xterm_mouse_Btn_aux1,
    dansi_xterm_mouse_Btn_aux2
)) dansi_xterm_mouse_Btn;
claim_assert_static(eqlType$(dansi_xterm_mouse_Btn, u8));
T_use_prl$(dansi_xterm_mouse_Btn);

typedef union dansi_xterm_mouse_Btns {
    struct {
        u8 left      : 1;
        u8 middle    : 1;
        u8 right     : 1;
        u8 backward  : 1;
        u8 forward   : 1;
        u8 aux1      : 1;
        u8 aux2      : 1;
        u8 reserved_ : 1;
    };
    u8 packed;
} dansi_xterm_mouse_Btns;
T_use_prl$(dansi_xterm_mouse_Btns);

typedef enum_((dansi_xterm_mouse_Wheel $fits($packed))(
    dansi_xterm_mouse_Wheel_up,
    dansi_xterm_mouse_Wheel_down,
    dansi_xterm_mouse_Wheel_left,
    dansi_xterm_mouse_Wheel_right
)) dansi_xterm_mouse_Wheel;
claim_assert_static(eqlType$(dansi_xterm_mouse_Wheel, u8));
T_use_prl$(dansi_xterm_mouse_Wheel);

typedef struct dansi_xterm_mouse_Pos {
    var_(x, u16);
    var_(y, u16);
} dansi_xterm_mouse_Pos;
T_use_prl$(dansi_xterm_mouse_Pos);

typedef union dansi_xterm_mouse_Mods {
    struct {
        u8 shift     : 1;
        u8 alt       : 1;
        u8 ctrl      : 1;
        u8 meta      : 1;
        u8 reserved_ : 4;
    };
    u8 packed;
} dansi_xterm_mouse_Mods;
T_use_prl$(dansi_xterm_mouse_Mods);

$attr($inline_always)
$static fn_((dansi_xterm_mouse_modsNone(void))(dansi_xterm_mouse_Mods)) {
    return (dansi_xterm_mouse_Mods){};
};

$attr($inline_always)
$static fn_((dansi_xterm_mouse_modsFromCb(u16 cb))(dansi_xterm_mouse_Mods)) {
    let_(meta, bool) = (cb & 8) != 0;
    return (dansi_xterm_mouse_Mods){
        .shift = (cb & 4) != 0,
        .alt = meta,
        .ctrl = (cb & 16) != 0,
        .meta = meta,
    };
};

$attr($inline_always)
$static fn_((dansi_xterm_mouse_modsToCb(dansi_xterm_mouse_Mods mods))(u16)) {
    return (mods.shift ? 4 : 0)
         | ((mods.alt || mods.meta) ? 8 : 0)
         | (mods.ctrl ? 16 : 0);
};

typedef enum_((dansi_xterm_mouse_ReportMode $fits($packed))(
    dansi_xterm_mouse_ReportMode_x10 = dansi_xterm_mode_Code_mouse_x10,
    dansi_xterm_mouse_ReportMode_normal = dansi_xterm_mode_Code_mouse_normal,
    dansi_xterm_mouse_ReportMode_hilite = dansi_xterm_mode_Code_mouse_hilite,
    dansi_xterm_mouse_ReportMode_button_event = dansi_xterm_mode_Code_mouse_button_event,
    dansi_xterm_mouse_ReportMode_any_event = dansi_xterm_mode_Code_mouse_any_event
)) dansi_xterm_mouse_ReportMode;
claim_assert_static(eqlType$(dansi_xterm_mouse_ReportMode, u16));
T_use_prl$(dansi_xterm_mouse_ReportMode);

#define dansi_xterm_mouse_ReportMode_staticParse(_mode_tok) \
    ____dansi_xterm_mouse_ReportMode_staticParse(_mode_tok)

typedef enum_((dansi_xterm_mouse_Encoding $fits($packed))(
    dansi_xterm_mouse_Encoding_default = 0,
    dansi_xterm_mouse_Encoding_utf8 = dansi_xterm_mode_Code_mouse_utf8,
    dansi_xterm_mouse_Encoding_sgr = dansi_xterm_mode_Code_mouse_sgr,
    dansi_xterm_mouse_Encoding_urxvt = dansi_xterm_mode_Code_mouse_urxvt,
    dansi_xterm_mouse_Encoding_sgr_pixels = dansi_xterm_mode_Code_mouse_sgr_pixels
)) dansi_xterm_mouse_Encoding;
claim_assert_static(eqlType$(dansi_xterm_mouse_Encoding, u16));
T_use_prl$(dansi_xterm_mouse_Encoding);

#define dansi_xterm_mouse_Encoding_staticParse(_encoding_tok) \
    ____dansi_xterm_mouse_Encoding_staticParse(_encoding_tok)

typedef struct dansi_xterm_mouse_SGRReport {
    var_(cb, u16);
    var_(x, u16);
    var_(y, u16);
    var_(final, u8);
} dansi_xterm_mouse_SGRReport;
T_use_prl$(dansi_xterm_mouse_SGRReport);

typedef struct dansi_xterm_mouse_PressEvent {
    var_(btn, dansi_xterm_mouse_Btn);
    var_(pos, dansi_xterm_mouse_Pos);
    var_(mods, dansi_xterm_mouse_Mods);
} dansi_xterm_mouse_PressEvent;
T_use_prl$(dansi_xterm_mouse_PressEvent);

typedef struct dansi_xterm_mouse_ReleaseEvent {
    var_(pos, dansi_xterm_mouse_Pos);
    var_(mods, dansi_xterm_mouse_Mods);
} dansi_xterm_mouse_ReleaseEvent;
T_use_prl$(dansi_xterm_mouse_ReleaseEvent);

typedef struct dansi_xterm_mouse_MotionEvent {
    var_(btn, O$dansi_xterm_mouse_Btn);
    var_(pos, dansi_xterm_mouse_Pos);
    var_(mods, dansi_xterm_mouse_Mods);
} dansi_xterm_mouse_MotionEvent;
T_use_prl$(dansi_xterm_mouse_MotionEvent);

typedef struct dansi_xterm_mouse_WheelEvent {
    var_(wheel, dansi_xterm_mouse_Wheel);
    var_(pos, dansi_xterm_mouse_Pos);
    var_(mods, dansi_xterm_mouse_Mods);
} dansi_xterm_mouse_WheelEvent;
T_use_prl$(dansi_xterm_mouse_WheelEvent);

typedef variant_((dansi_xterm_mouse_Event $fits($packed))(
    (dansi_xterm_mouse_Event_press, dansi_xterm_mouse_PressEvent),
    (dansi_xterm_mouse_Event_release, dansi_xterm_mouse_ReleaseEvent),
    (dansi_xterm_mouse_Event_motion, dansi_xterm_mouse_MotionEvent),
    (dansi_xterm_mouse_Event_wheel, dansi_xterm_mouse_WheelEvent)
)) dansi_xterm_mouse_Event;
claim_assert_static(eqlType$(FieldType$(dansi_xterm_mouse_Event, tag), u8));
T_use_prl$(dansi_xterm_mouse_Event);

errset_((dansi_xterm_mouse_E)(dansi_xterm_mouse_InvalidReport));
T_use_E$($set(dansi_xterm_mouse_E)(dansi_xterm_mouse_Event));

#define dansi_xterm_mouse_setReportMode_static(_mode_tok, _pp_enabled) \
    ____dansi_xterm_mouse_setReportMode_static(_mode_tok, _pp_enabled)
typedef dansi_xterm_mode_SetRawBuf dansi_xterm_mouse_SetReportModeBuf;
$attr($must_check)
$extern fn_((dansi_xterm_mouse_setReportMode(
    dansi_xterm_mouse_ReportMode mode, bool enabled, dansi_xterm_mouse_SetReportModeBuf* buf
))(S$u8));
$attr($must_check)
$extern fn_((dansi_xterm_mouse_setReportModeWrite(
    dansi_xterm_mouse_ReportMode mode, bool enabled, io_Writer out
))(E$void));

#define dansi_xterm_mouse_enableReportMode_static(_mode_tok) \
    ____dansi_xterm_mouse_enableReportMode_static(_mode_tok)
typedef dansi_xterm_mode_EnableRawBuf dansi_xterm_mouse_EnableReportModeBuf;
$attr($must_check)
$extern fn_((dansi_xterm_mouse_enableReportMode(
    dansi_xterm_mouse_ReportMode mode, dansi_xterm_mouse_EnableReportModeBuf* buf
))(S$u8));
$attr($must_check)
$extern fn_((dansi_xterm_mouse_enableReportModeWrite(
    dansi_xterm_mouse_ReportMode mode, io_Writer out
))(E$void));

#define dansi_xterm_mouse_disableReportMode_static(_mode_tok) \
    ____dansi_xterm_mouse_disableReportMode_static(_mode_tok)
typedef dansi_xterm_mode_DisableRawBuf dansi_xterm_mouse_DisableReportModeBuf;
$attr($must_check)
$extern fn_((dansi_xterm_mouse_disableReportMode(
    dansi_xterm_mouse_ReportMode mode, dansi_xterm_mouse_DisableReportModeBuf* buf
))(S$u8));
$attr($must_check)
$extern fn_((dansi_xterm_mouse_disableReportModeWrite(
    dansi_xterm_mouse_ReportMode mode, io_Writer out
))(E$void));

#define dansi_xterm_mouse_setEncoding_static(_encoding_tok, _pp_enabled) \
    ____dansi_xterm_mouse_setEncoding_static(_encoding_tok, _pp_enabled)
typedef dansi_xterm_mode_SetRawBuf dansi_xterm_mouse_SetEncodingBuf;
$attr($must_check)
$extern fn_((dansi_xterm_mouse_setEncoding(
    dansi_xterm_mouse_Encoding encoding, bool enabled, dansi_xterm_mouse_SetEncodingBuf* buf
))(S$u8));
$attr($must_check)
$extern fn_((dansi_xterm_mouse_setEncodingWrite(
    dansi_xterm_mouse_Encoding encoding, bool enabled, io_Writer out
))(E$void));

#define dansi_xterm_mouse_enableEncoding_static(_encoding_tok) \
    ____dansi_xterm_mouse_enableEncoding_static(_encoding_tok)
typedef dansi_xterm_mode_EnableRawBuf dansi_xterm_mouse_EnableEncodingBuf;
$attr($must_check)
$extern fn_((dansi_xterm_mouse_enableEncoding(
    dansi_xterm_mouse_Encoding encoding, dansi_xterm_mouse_EnableEncodingBuf* buf
))(S$u8));
$attr($must_check)
$extern fn_((dansi_xterm_mouse_enableEncodingWrite(
    dansi_xterm_mouse_Encoding encoding, io_Writer out
))(E$void));

#define dansi_xterm_mouse_disableEncoding_static(_encoding_tok) \
    ____dansi_xterm_mouse_disableEncoding_static(_encoding_tok)
typedef dansi_xterm_mode_DisableRawBuf dansi_xterm_mouse_DisableEncodingBuf;
$attr($must_check)
$extern fn_((dansi_xterm_mouse_disableEncoding(
    dansi_xterm_mouse_Encoding encoding, dansi_xterm_mouse_DisableEncodingBuf* buf
))(S$u8));
$attr($must_check)
$extern fn_((dansi_xterm_mouse_disableEncodingWrite(
    dansi_xterm_mouse_Encoding encoding, io_Writer out
))(E$void));

#define dansi_xterm_mouse_enableAny_static() \
    ____dansi_xterm_mouse_enableAny_static()
typedef dansi_xterm_mouse_EnableReportModeBuf dansi_xterm_mouse_EnableAnyBuf;
$attr($must_check)
$extern fn_((dansi_xterm_mouse_enableAny(dansi_xterm_mouse_EnableAnyBuf* buf))(S$u8));
$attr($must_check)
$extern fn_((dansi_xterm_mouse_enableAnyWrite(io_Writer out))(E$void));

#define dansi_xterm_mouse_disableAny_static() \
    ____dansi_xterm_mouse_disableAny_static()
typedef dansi_xterm_mouse_DisableReportModeBuf dansi_xterm_mouse_DisableAnyBuf;
$attr($must_check)
$extern fn_((dansi_xterm_mouse_disableAny(dansi_xterm_mouse_DisableAnyBuf* buf))(S$u8));
$attr($must_check)
$extern fn_((dansi_xterm_mouse_disableAnyWrite(io_Writer out))(E$void));

#define dansi_xterm_mouse_enableSGR_static(_mode_tok) \
    ____dansi_xterm_mouse_enableSGR_static(_mode_tok)
typedef A$$(18, u8) dansi_xterm_mouse_EnableSGRBuf;
$attr($must_check)
$extern fn_((dansi_xterm_mouse_enableSGR(
    dansi_xterm_mouse_ReportMode mode, dansi_xterm_mouse_EnableSGRBuf* buf
))(S$u8));
$attr($must_check)
$extern fn_((dansi_xterm_mouse_enableSGRWrite(
    dansi_xterm_mouse_ReportMode mode, io_Writer out
))(E$void));

#define dansi_xterm_mouse_disableSGR_static(_mode_tok) \
    ____dansi_xterm_mouse_disableSGR_static(_mode_tok)
typedef A$$(18, u8) dansi_xterm_mouse_DisableSGRBuf;
$attr($must_check)
$extern fn_((dansi_xterm_mouse_disableSGR(
    dansi_xterm_mouse_ReportMode mode, dansi_xterm_mouse_DisableSGRBuf* buf
))(S$u8));
$attr($must_check)
$extern fn_((dansi_xterm_mouse_disableSGRWrite(
    dansi_xterm_mouse_ReportMode mode, io_Writer out
))(E$void));
$extern fn_((dansi_xterm_mouse_parseSGRReport(S_const$u8 report))(O$dansi_xterm_mouse_SGRReport));
$attr($must_check)
$extern fn_((dansi_xterm_mouse_interpretSGR(
    dansi_xterm_mouse_SGRReport report
))(dansi_xterm_mouse_E$dansi_xterm_mouse_Event));
$extern fn_((dansi_xterm_mouse_parseSGR(S_const$u8 report))(O$dansi_xterm_mouse_Event));

$attr($inline_always)
$static fn_((dansi_xterm_mouse_btnsNone(void))(dansi_xterm_mouse_Btns)) {
    return (dansi_xterm_mouse_Btns){};
};

/*========== Macros and Definitions =========================================*/

#define ____dansi_xterm_mouse_enableReportMode_static(_mode_tok) \
    dansi_xterm_mode_enableRaw_static(dansi_xterm_mouse_ReportMode_staticParse(_mode_tok))
#define ____dansi_xterm_mouse_disableReportMode_static(_mode_tok) \
    dansi_xterm_mode_disableRaw_static(dansi_xterm_mouse_ReportMode_staticParse(_mode_tok))
#define ____dansi_xterm_mouse_setReportMode_static(_mode_tok, _pp_enabled) \
    dansi_xterm_mode_setRaw_static(dansi_xterm_mouse_ReportMode_staticParse(_mode_tok), _pp_enabled)
#define ____dansi_xterm_mouse_enableEncoding_static(_encoding_tok) \
    dansi_xterm_mode_enableRaw_static(dansi_xterm_mouse_Encoding_staticParse(_encoding_tok))
#define ____dansi_xterm_mouse_disableEncoding_static(_encoding_tok) \
    dansi_xterm_mode_disableRaw_static(dansi_xterm_mouse_Encoding_staticParse(_encoding_tok))
#define ____dansi_xterm_mouse_setEncoding_static(_encoding_tok, _pp_enabled) \
    dansi_xterm_mode_setRaw_static(dansi_xterm_mouse_Encoding_staticParse(_encoding_tok), _pp_enabled)
#define ____dansi_xterm_mouse_enableAny_static() \
    dansi_xterm_mouse_enableReportMode_static(dansi_xterm_mouse_ReportMode_any_event)
#define ____dansi_xterm_mouse_disableAny_static() \
    dansi_xterm_mouse_disableReportMode_static(dansi_xterm_mouse_ReportMode_any_event)
#define ____dansi_xterm_mouse_enableSGR_static(_mode_tok) \
    dansi_xterm_mouse_enableReportMode_static(_mode_tok) \
    dansi_xterm_mouse_enableEncoding_static(dansi_xterm_mouse_Encoding_sgr)
#define ____dansi_xterm_mouse_disableSGR_static(_mode_tok) \
    dansi_xterm_mouse_disableEncoding_static(dansi_xterm_mouse_Encoding_sgr) \
    dansi_xterm_mouse_disableReportMode_static(_mode_tok)
#define ____dansi_xterm_mouse_ReportMode_staticParse(_mode_tok) \
    pp_join($, ____dansi_xterm_mouse_ReportMode_str, _mode_tok)
#define ____dansi_xterm_mouse_ReportMode_str$dansi_xterm_mouse_ReportMode_x10 "9"
#define ____dansi_xterm_mouse_ReportMode_str$dansi_xterm_mouse_ReportMode_normal "1000"
#define ____dansi_xterm_mouse_ReportMode_str$dansi_xterm_mouse_ReportMode_hilite "1001"
#define ____dansi_xterm_mouse_ReportMode_str$dansi_xterm_mouse_ReportMode_button_event "1002"
#define ____dansi_xterm_mouse_ReportMode_str$dansi_xterm_mouse_ReportMode_any_event "1003"
#define ____dansi_xterm_mouse_Encoding_staticParse(_encoding_tok) \
    pp_join($, ____dansi_xterm_mouse_Encoding_str, _encoding_tok)
#define ____dansi_xterm_mouse_Encoding_str$dansi_xterm_mouse_Encoding_default "0"
#define ____dansi_xterm_mouse_Encoding_str$dansi_xterm_mouse_Encoding_utf8 "1005"
#define ____dansi_xterm_mouse_Encoding_str$dansi_xterm_mouse_Encoding_sgr "1006"
#define ____dansi_xterm_mouse_Encoding_str$dansi_xterm_mouse_Encoding_urxvt "1015"
#define ____dansi_xterm_mouse_Encoding_str$dansi_xterm_mouse_Encoding_sgr_pixels "1016"

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* dansi_xterm_mouse__included */

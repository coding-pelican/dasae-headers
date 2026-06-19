/**
 * @copyright Copyright (c) 2026 Gyeongtae Kim. All rights reserved.
 * @license   Proprietary and confidential. Unauthorized use, reproduction,
 *            or distribution is strictly prohibited.
 *
 * @file    key.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2026-06-19 (date of creation)
 * @updated 2026-06-19 (date of last update)
 * @ingroup dasae-headers-workspace(dh-workspace)/dansi
 * @prefix  dansi_dec_key
 */
#pragma once
#ifndef dansi_dec_key__included
#define dansi_dec_key__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "dansi-core/esc.h"
#include "dansi-core/Seq.h"
#include "dansi-core/csi.h"
#include "mode.h"

/*========== Macros and Declarations ========================================*/

typedef enum_((dansi_dec_key_Named $fits($packed))(
    dansi_dec_key_Named_up,
    dansi_dec_key_Named_down,
    dansi_dec_key_Named_right,
    dansi_dec_key_Named_left,
    dansi_dec_key_Named_home,
    dansi_dec_key_Named_end,
    dansi_dec_key_Named_find,
    dansi_dec_key_Named_insert,
    dansi_dec_key_Named_remove,
    dansi_dec_key_Named_select,
    dansi_dec_key_Named_prev,
    dansi_dec_key_Named_next,
    dansi_dec_key_Named_pf1,
    dansi_dec_key_Named_pf2,
    dansi_dec_key_Named_pf3,
    dansi_dec_key_Named_pf4,
    dansi_dec_key_Named_f6,
    dansi_dec_key_Named_f7,
    dansi_dec_key_Named_f8,
    dansi_dec_key_Named_f9,
    dansi_dec_key_Named_f10,
    dansi_dec_key_Named_f11,
    dansi_dec_key_Named_f12,
    dansi_dec_key_Named_f13,
    dansi_dec_key_Named_f14,
    dansi_dec_key_Named_f15,
    dansi_dec_key_Named_f16,
    dansi_dec_key_Named_f17,
    dansi_dec_key_Named_f18,
    dansi_dec_key_Named_f19,
    dansi_dec_key_Named_f20,
    dansi_dec_key_Named_f21,
    dansi_dec_key_Named_f22,
    dansi_dec_key_Named_f23,
    dansi_dec_key_Named_f24
)) dansi_dec_key_Named;
claim_assert_static(eqlType$(dansi_dec_key_Named, u8));
T_use_prl$(dansi_dec_key_Named);

typedef enum_((dansi_dec_key_Keypad $fits($packed))(
    dansi_dec_key_Keypad_0,
    dansi_dec_key_Keypad_1,
    dansi_dec_key_Keypad_2,
    dansi_dec_key_Keypad_3,
    dansi_dec_key_Keypad_4,
    dansi_dec_key_Keypad_5,
    dansi_dec_key_Keypad_6,
    dansi_dec_key_Keypad_7,
    dansi_dec_key_Keypad_8,
    dansi_dec_key_Keypad_9,
    dansi_dec_key_Keypad_decimal,
    dansi_dec_key_Keypad_minus,
    dansi_dec_key_Keypad_comma,
    dansi_dec_key_Keypad_enter
)) dansi_dec_key_Keypad;
claim_assert_static(eqlType$(dansi_dec_key_Keypad, u8));
T_use_prl$(dansi_dec_key_Keypad);

typedef struct dansi_dec_key_ESCReport {
    var_(final, u8);
    var_(bytes, S_const$u8);
} dansi_dec_key_ESCReport;
T_use_prl$(dansi_dec_key_ESCReport);

typedef struct dansi_dec_key_SS3Report {
    var_(final, u8);
    var_(bytes, S_const$u8);
} dansi_dec_key_SS3Report;
T_use_prl$(dansi_dec_key_SS3Report);

typedef struct dansi_dec_key_CSIReport {
    var_(frame, dansi_csi_Frame);
} dansi_dec_key_CSIReport;
T_use_prl$(dansi_dec_key_CSIReport);

typedef struct dansi_dec_key_CSITildeReport {
    var_(frame, dansi_csi_Frame);
} dansi_dec_key_CSITildeReport;
T_use_prl$(dansi_dec_key_CSITildeReport);

typedef struct dansi_dec_key_VT52Report {
    var_(final, u8);
    var_(bytes, S_const$u8);
} dansi_dec_key_VT52Report;
T_use_prl$(dansi_dec_key_VT52Report);

typedef variant_((dansi_dec_key_Report $fits($packed))(
    (dansi_dec_key_Report_esc, dansi_dec_key_ESCReport),
    (dansi_dec_key_Report_ss3, dansi_dec_key_SS3Report),
    (dansi_dec_key_Report_csi, dansi_dec_key_CSIReport),
    (dansi_dec_key_Report_csi_tilde, dansi_dec_key_CSITildeReport),
    (dansi_dec_key_Report_vt52, dansi_dec_key_VT52Report)
)) dansi_dec_key_Report;
claim_assert_static(eqlType$(FieldType$(dansi_dec_key_Report, tag), u8));
T_use_prl$(dansi_dec_key_Report);

typedef variant_((dansi_dec_key_Event $fits($packed))(
    (dansi_dec_key_Event_named, dansi_dec_key_Named),
    (dansi_dec_key_Event_keypad, dansi_dec_key_Keypad)
)) dansi_dec_key_Event;
claim_assert_static(eqlType$(FieldType$(dansi_dec_key_Event, tag), u8));
T_use_prl$(dansi_dec_key_Event);

errset_((dansi_dec_key_E)(dansi_dec_key_InvalidReport));
T_use_E$($set(dansi_dec_key_E)(dansi_dec_key_Event));

#define dansi_dec_key_setCursor_static(_pp_enabled) \
    ____dansi_dec_key_setCursor_static(_pp_enabled)
typedef dansi_dec_mode_SetBuf dansi_dec_key_SetCursorBuf;
$attr($must_check)
$extern fn_((dansi_dec_key_setCursor(bool enabled, dansi_dec_key_SetCursorBuf* buf))(S$u8));
$attr($must_check)
$extern fn_((dansi_dec_key_setCursorWrite(bool enabled, io_Writer out))(E$void));

#define dansi_dec_key_enableCursor_static() \
    ____dansi_dec_key_enableCursor_static()
typedef dansi_dec_mode_EnableBuf dansi_dec_key_EnableCursorBuf;
$attr($must_check)
$extern fn_((dansi_dec_key_enableCursor(dansi_dec_key_EnableCursorBuf* buf))(S$u8));
$attr($must_check)
$extern fn_((dansi_dec_key_enableCursorWrite(io_Writer out))(E$void));

#define dansi_dec_key_disableCursor_static() \
    ____dansi_dec_key_disableCursor_static()
typedef dansi_dec_mode_DisableBuf dansi_dec_key_DisableCursorBuf;
$attr($must_check)
$extern fn_((dansi_dec_key_disableCursor(dansi_dec_key_DisableCursorBuf* buf))(S$u8));
$attr($must_check)
$extern fn_((dansi_dec_key_disableCursorWrite(io_Writer out))(E$void));

#define dansi_dec_key_setKeypad_static(_pp_enabled) \
    ____dansi_dec_key_setKeypad_static(_pp_enabled)
typedef A$$(2, u8) dansi_dec_key_SetKeypadBuf;
$attr($must_check)
$extern fn_((dansi_dec_key_setKeypad(bool enabled, dansi_dec_key_SetKeypadBuf* buf))(S$u8));
$attr($must_check)
$extern fn_((dansi_dec_key_setKeypadWrite(bool enabled, io_Writer out))(E$void));

#define dansi_dec_key_enableKeypad_static() \
    ____dansi_dec_key_enableKeypad_static()
$extern fn_((dansi_dec_key_enableKeypad(void))(S_const$u8));
$attr($must_check)
$extern fn_((dansi_dec_key_enableKeypadWrite(io_Writer out))(E$void));

#define dansi_dec_key_disableKeypad_static() \
    ____dansi_dec_key_disableKeypad_static()
$extern fn_((dansi_dec_key_disableKeypad(void))(S_const$u8));
$attr($must_check)
$extern fn_((dansi_dec_key_disableKeypadWrite(io_Writer out))(E$void));

$extern fn_((dansi_dec_key_parseReport(dansi_Seq seq))(O$dansi_dec_key_Report));
$attr($must_check)
$extern fn_((dansi_dec_key_interpretReport(
    dansi_dec_key_Report report
))(dansi_dec_key_E$dansi_dec_key_Event));
$extern fn_((dansi_dec_key_parseEvent(dansi_Seq seq))(O$dansi_dec_key_Event));

/*========== Macros and Definitions =========================================*/

#define ____dansi_dec_key_setCursor_static(_pp_enabled) \
    dansi_dec_mode_set_static(dansi_dec_mode_Code_cursor_keys, _pp_enabled)
#define ____dansi_dec_key_enableCursor_static() \
    dansi_dec_mode_enable_static(dansi_dec_mode_Code_cursor_keys)
#define ____dansi_dec_key_disableCursor_static() \
    dansi_dec_mode_disable_static(dansi_dec_mode_Code_cursor_keys)
#define ____dansi_dec_key_setKeypad_static(_pp_enabled) \
    pp_if_(_pp_enabled)( \
        pp_then_(dansi_dec_key_enableKeypad_static()), \
        pp_else_(dansi_dec_key_disableKeypad_static()) \
    )
#define ____dansi_dec_key_enableKeypad_static() \
    dansi_esc_make_static("", "=")
#define ____dansi_dec_key_disableKeypad_static() \
    dansi_esc_make_static("", ">")

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* dansi_dec_key__included */

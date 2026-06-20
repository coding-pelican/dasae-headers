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

#define dansi_dec_key_final_up __str__dansi_dec_key_final_up
#define dansi_dec_key_final_up_byte __uint__dansi_dec_key_final_up_byte
#define dansi_dec_key_final_down __str__dansi_dec_key_final_down
#define dansi_dec_key_final_down_byte __uint__dansi_dec_key_final_down_byte
#define dansi_dec_key_final_right __str__dansi_dec_key_final_right
#define dansi_dec_key_final_right_byte __uint__dansi_dec_key_final_right_byte
#define dansi_dec_key_final_left __str__dansi_dec_key_final_left
#define dansi_dec_key_final_left_byte __uint__dansi_dec_key_final_left_byte
#define dansi_dec_key_final_home __str__dansi_dec_key_final_home
#define dansi_dec_key_final_home_byte __uint__dansi_dec_key_final_home_byte
#define dansi_dec_key_final_end __str__dansi_dec_key_final_end
#define dansi_dec_key_final_end_byte __uint__dansi_dec_key_final_end_byte
#define dansi_dec_key_final_pf1 __str__dansi_dec_key_final_pf1
#define dansi_dec_key_final_pf1_byte __uint__dansi_dec_key_final_pf1_byte
#define dansi_dec_key_final_pf2 __str__dansi_dec_key_final_pf2
#define dansi_dec_key_final_pf2_byte __uint__dansi_dec_key_final_pf2_byte
#define dansi_dec_key_final_pf3 __str__dansi_dec_key_final_pf3
#define dansi_dec_key_final_pf3_byte __uint__dansi_dec_key_final_pf3_byte
#define dansi_dec_key_final_pf4 __str__dansi_dec_key_final_pf4
#define dansi_dec_key_final_pf4_byte __uint__dansi_dec_key_final_pf4_byte
#define dansi_dec_key_final_keypad_0 __str__dansi_dec_key_final_keypad_0
#define dansi_dec_key_final_keypad_0_byte __uint__dansi_dec_key_final_keypad_0_byte
#define dansi_dec_key_final_keypad_1 __str__dansi_dec_key_final_keypad_1
#define dansi_dec_key_final_keypad_1_byte __uint__dansi_dec_key_final_keypad_1_byte
#define dansi_dec_key_final_keypad_2 __str__dansi_dec_key_final_keypad_2
#define dansi_dec_key_final_keypad_2_byte __uint__dansi_dec_key_final_keypad_2_byte
#define dansi_dec_key_final_keypad_3 __str__dansi_dec_key_final_keypad_3
#define dansi_dec_key_final_keypad_3_byte __uint__dansi_dec_key_final_keypad_3_byte
#define dansi_dec_key_final_keypad_4 __str__dansi_dec_key_final_keypad_4
#define dansi_dec_key_final_keypad_4_byte __uint__dansi_dec_key_final_keypad_4_byte
#define dansi_dec_key_final_keypad_5 __str__dansi_dec_key_final_keypad_5
#define dansi_dec_key_final_keypad_5_byte __uint__dansi_dec_key_final_keypad_5_byte
#define dansi_dec_key_final_keypad_6 __str__dansi_dec_key_final_keypad_6
#define dansi_dec_key_final_keypad_6_byte __uint__dansi_dec_key_final_keypad_6_byte
#define dansi_dec_key_final_keypad_7 __str__dansi_dec_key_final_keypad_7
#define dansi_dec_key_final_keypad_7_byte __uint__dansi_dec_key_final_keypad_7_byte
#define dansi_dec_key_final_keypad_8 __str__dansi_dec_key_final_keypad_8
#define dansi_dec_key_final_keypad_8_byte __uint__dansi_dec_key_final_keypad_8_byte
#define dansi_dec_key_final_keypad_9 __str__dansi_dec_key_final_keypad_9
#define dansi_dec_key_final_keypad_9_byte __uint__dansi_dec_key_final_keypad_9_byte
#define dansi_dec_key_final_keypad_decimal __str__dansi_dec_key_final_keypad_decimal
#define dansi_dec_key_final_keypad_decimal_byte __uint__dansi_dec_key_final_keypad_decimal_byte
#define dansi_dec_key_final_keypad_minus __str__dansi_dec_key_final_keypad_minus
#define dansi_dec_key_final_keypad_minus_byte __uint__dansi_dec_key_final_keypad_minus_byte
#define dansi_dec_key_final_keypad_comma __str__dansi_dec_key_final_keypad_comma
#define dansi_dec_key_final_keypad_comma_byte __uint__dansi_dec_key_final_keypad_comma_byte
#define dansi_dec_key_final_keypad_enter __str__dansi_dec_key_final_keypad_enter
#define dansi_dec_key_final_keypad_enter_byte __uint__dansi_dec_key_final_keypad_enter_byte
#define dansi_dec_key_final_tilde __str__dansi_dec_key_final_tilde
#define dansi_dec_key_final_tilde_byte __uint__dansi_dec_key_final_tilde_byte
#define dansi_dec_key_tilde_param_code __uint__dansi_dec_key_tilde_param_code
#define dansi_dec_key_keypad_enable_final __str__dansi_dec_key_keypad_enable_final
#define dansi_dec_key_keypad_enable_final_byte __uint__dansi_dec_key_keypad_enable_final_byte
#define dansi_dec_key_keypad_disable_final __str__dansi_dec_key_keypad_disable_final
#define dansi_dec_key_keypad_disable_final_byte __uint__dansi_dec_key_keypad_disable_final_byte

typedef enum_((dansi_dec_key_TildeCode $fits($packed))(
    dansi_dec_key_TildeCode_find = 1,
    dansi_dec_key_TildeCode_insert = 2,
    dansi_dec_key_TildeCode_remove = 3,
    dansi_dec_key_TildeCode_select = 4,
    dansi_dec_key_TildeCode_prev = 5,
    dansi_dec_key_TildeCode_next = 6,
    dansi_dec_key_TildeCode_f6 = 17,
    dansi_dec_key_TildeCode_f7 = 18,
    dansi_dec_key_TildeCode_f8 = 19,
    dansi_dec_key_TildeCode_f9 = 20,
    dansi_dec_key_TildeCode_f10 = 21,
    dansi_dec_key_TildeCode_f11 = 23,
    dansi_dec_key_TildeCode_f12 = 24,
    dansi_dec_key_TildeCode_f13 = 25,
    dansi_dec_key_TildeCode_f14 = 26,
    dansi_dec_key_TildeCode_f15 = 28,
    dansi_dec_key_TildeCode_f16 = 29,
    dansi_dec_key_TildeCode_f17 = 31,
    dansi_dec_key_TildeCode_f18 = 32,
    dansi_dec_key_TildeCode_f19 = 33,
    dansi_dec_key_TildeCode_f20 = 34,
    dansi_dec_key_TildeCode_f21 = 35,
    dansi_dec_key_TildeCode_f22 = 36,
    dansi_dec_key_TildeCode_f23 = 37,
    dansi_dec_key_TildeCode_f24 = 38
)) dansi_dec_key_TildeCode;
claim_assert_static(eqlType$(dansi_dec_key_TildeCode, u8));
T_use_prl$(dansi_dec_key_TildeCode);

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

#define __str__dansi_dec_key_final_up "A"
#define __uint__dansi_dec_key_final_up_byte u8_c('A')
#define __str__dansi_dec_key_final_down "B"
#define __uint__dansi_dec_key_final_down_byte u8_c('B')
#define __str__dansi_dec_key_final_right "C"
#define __uint__dansi_dec_key_final_right_byte u8_c('C')
#define __str__dansi_dec_key_final_left "D"
#define __uint__dansi_dec_key_final_left_byte u8_c('D')
#define __str__dansi_dec_key_final_home "H"
#define __uint__dansi_dec_key_final_home_byte u8_c('H')
#define __str__dansi_dec_key_final_end "F"
#define __uint__dansi_dec_key_final_end_byte u8_c('F')
#define __str__dansi_dec_key_final_pf1 "P"
#define __uint__dansi_dec_key_final_pf1_byte u8_c('P')
#define __str__dansi_dec_key_final_pf2 "Q"
#define __uint__dansi_dec_key_final_pf2_byte u8_c('Q')
#define __str__dansi_dec_key_final_pf3 "R"
#define __uint__dansi_dec_key_final_pf3_byte u8_c('R')
#define __str__dansi_dec_key_final_pf4 "S"
#define __uint__dansi_dec_key_final_pf4_byte u8_c('S')
#define __str__dansi_dec_key_final_keypad_0 "p"
#define __uint__dansi_dec_key_final_keypad_0_byte u8_c('p')
#define __str__dansi_dec_key_final_keypad_1 "q"
#define __uint__dansi_dec_key_final_keypad_1_byte u8_c('q')
#define __str__dansi_dec_key_final_keypad_2 "r"
#define __uint__dansi_dec_key_final_keypad_2_byte u8_c('r')
#define __str__dansi_dec_key_final_keypad_3 "s"
#define __uint__dansi_dec_key_final_keypad_3_byte u8_c('s')
#define __str__dansi_dec_key_final_keypad_4 "t"
#define __uint__dansi_dec_key_final_keypad_4_byte u8_c('t')
#define __str__dansi_dec_key_final_keypad_5 "u"
#define __uint__dansi_dec_key_final_keypad_5_byte u8_c('u')
#define __str__dansi_dec_key_final_keypad_6 "v"
#define __uint__dansi_dec_key_final_keypad_6_byte u8_c('v')
#define __str__dansi_dec_key_final_keypad_7 "w"
#define __uint__dansi_dec_key_final_keypad_7_byte u8_c('w')
#define __str__dansi_dec_key_final_keypad_8 "x"
#define __uint__dansi_dec_key_final_keypad_8_byte u8_c('x')
#define __str__dansi_dec_key_final_keypad_9 "y"
#define __uint__dansi_dec_key_final_keypad_9_byte u8_c('y')
#define __str__dansi_dec_key_final_keypad_decimal "n"
#define __uint__dansi_dec_key_final_keypad_decimal_byte u8_c('n')
#define __str__dansi_dec_key_final_keypad_minus "m"
#define __uint__dansi_dec_key_final_keypad_minus_byte u8_c('m')
#define __str__dansi_dec_key_final_keypad_comma "l"
#define __uint__dansi_dec_key_final_keypad_comma_byte u8_c('l')
#define __str__dansi_dec_key_final_keypad_enter "M"
#define __uint__dansi_dec_key_final_keypad_enter_byte u8_c('M')
#define __str__dansi_dec_key_final_tilde "~"
#define __uint__dansi_dec_key_final_tilde_byte u8_c('~')
#define __str__dansi_dec_key_keypad_enable_final "="
#define __uint__dansi_dec_key_keypad_enable_final_byte u8_c('=')
#define __str__dansi_dec_key_keypad_disable_final ">"
#define __uint__dansi_dec_key_keypad_disable_final_byte u8_c('>')
#define __uint__dansi_dec_key_tilde_param_code 0
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
    dansi_esc_make_static("", dansi_dec_key_keypad_enable_final)
#define ____dansi_dec_key_disableKeypad_static() \
    dansi_esc_make_static("", dansi_dec_key_keypad_disable_final)

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* dansi_dec_key__included */

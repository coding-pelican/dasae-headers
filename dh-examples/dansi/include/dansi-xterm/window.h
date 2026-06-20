/**
 * @copyright Copyright (c) 2026 Gyeongtae Kim. All rights reserved.
 * @license   Proprietary and confidential. Unauthorized use, reproduction,
 *            or distribution is strictly prohibited.
 *
 * @file    window.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2026-06-19 (date of creation)
 * @updated 2026-06-19 (date of last update)
 * @ingroup dasae-headers-workspace(dh-workspace)/dansi
 * @prefix  dansi_xterm_window
 */
#pragma once
#ifndef dansi_xterm_window__included
#define dansi_xterm_window__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "dansi-core/csi.h"

/*========== Macros and Declarations ========================================*/

typedef enum_((dansi_xterm_window_Op $fits($packed))(
    dansi_xterm_window_Op_deiconify = 1,
    dansi_xterm_window_Op_iconify = 2,
    dansi_xterm_window_Op_move = 3,
    dansi_xterm_window_Op_resize_pixels = 4,
    dansi_xterm_window_Op_raise = 5,
    dansi_xterm_window_Op_lower = 6,
    dansi_xterm_window_Op_refresh = 7,
    dansi_xterm_window_Op_resize_cells = 8,
    dansi_xterm_window_Op_maximize = 9,
    dansi_xterm_window_Op_fullscreen = 10,
    dansi_xterm_window_Op_report_state = 11,
    dansi_xterm_window_Op_report_pos = 13,
    dansi_xterm_window_Op_report_text_area_pixels = 14,
    dansi_xterm_window_Op_report_screen_pixels = 15,
    dansi_xterm_window_Op_report_cell_pixels = 16,
    dansi_xterm_window_Op_report_text_area_cells = 18,
    dansi_xterm_window_Op_report_screen_cells = 19,
    dansi_xterm_window_Op_report_icon_label = 20,
    dansi_xterm_window_Op_report_title = 21
)) dansi_xterm_window_Op;
claim_assert_static(eqlType$(dansi_xterm_window_Op, u8));
T_use_prl$(dansi_xterm_window_Op);

#define dansi_xterm_window_Op_staticParse(_op_tok) \
    ____dansi_xterm_window_Op_staticParse(_op_tok)

#define dansi_xterm_window_report_final __str__dansi_xterm_window_report_final
#define dansi_xterm_window_report_final_byte __uint__dansi_xterm_window_report_final_byte
#define dansi_xterm_window_report_param_code __uint__dansi_xterm_window_report_param_code
#define dansi_xterm_window_report_param_x __uint__dansi_xterm_window_report_param_x
#define dansi_xterm_window_report_param_y __uint__dansi_xterm_window_report_param_y

typedef enum_((dansi_xterm_window_Maximize $fits($packed))(
    dansi_xterm_window_Maximize_restore = 0,
    dansi_xterm_window_Maximize_full = 1,
    dansi_xterm_window_Maximize_vertical = 2,
    dansi_xterm_window_Maximize_horizontal = 3
)) dansi_xterm_window_Maximize;
claim_assert_static(eqlType$(dansi_xterm_window_Maximize, u8));
T_use_prl$(dansi_xterm_window_Maximize);

#define dansi_xterm_window_Maximize_staticParse(_mode_tok) \
    ____dansi_xterm_window_Maximize_staticParse(_mode_tok)

typedef enum_((dansi_xterm_window_Fullscreen $fits($packed))(
    dansi_xterm_window_Fullscreen_restore = 0,
    dansi_xterm_window_Fullscreen_set = 1,
    dansi_xterm_window_Fullscreen_toggle = 2
)) dansi_xterm_window_Fullscreen;
claim_assert_static(eqlType$(dansi_xterm_window_Fullscreen, u8));
T_use_prl$(dansi_xterm_window_Fullscreen);

#define dansi_xterm_window_Fullscreen_staticParse(_mode_tok) \
    ____dansi_xterm_window_Fullscreen_staticParse(_mode_tok)

typedef enum_((dansi_xterm_window_State $fits($packed))(
    dansi_xterm_window_State_non_iconified = 1,
    dansi_xterm_window_State_iconified = 2
)) dansi_xterm_window_State;
claim_assert_static(eqlType$(dansi_xterm_window_State, u8));
T_use_prl$(dansi_xterm_window_State);

typedef struct dansi_xterm_window_Pos {
    var_(x, u16);
    var_(y, u16);
} dansi_xterm_window_Pos;
T_use_prl$(dansi_xterm_window_Pos);

errset_((dansi_xterm_window_E)(dansi_xterm_window_InvalidResponse));
T_use_E$($set(dansi_xterm_window_E)(dansi_xterm_window_State));
T_use_E$($set(dansi_xterm_window_E)(dansi_xterm_window_Pos));

#define dansi_xterm_window_op_static(_op_tok) \
    ____dansi_xterm_window_op_static(_op_tok)
typedef A$$(3 + uint_log10Ceil_static(u8_limit_max), u8) dansi_xterm_window_OpBuf;
$attr($must_check)
$extern fn_((dansi_xterm_window_op(dansi_xterm_window_Op op, dansi_xterm_window_OpBuf* buf))(S$u8));
$attr($must_check)
$extern fn_((dansi_xterm_window_opWrite(dansi_xterm_window_Op op, io_Writer out))(E$void));

#define dansi_xterm_window_move_static(_x_tok, _y_tok) \
    ____dansi_xterm_window_move_static(_x_tok, _y_tok)
typedef A$$(6 + uint_log10Ceil_static(u16_limit_max) * 2, u8) dansi_xterm_window_MoveBuf;
$attr($must_check)
$extern fn_((dansi_xterm_window_move(u16 x, u16 y, dansi_xterm_window_MoveBuf* buf))(S$u8));
$attr($must_check)
$extern fn_((dansi_xterm_window_moveWrite(u16 x, u16 y, io_Writer out))(E$void));

#define dansi_xterm_window_resizePixels_static(_height_tok, _width_tok) \
    ____dansi_xterm_window_resizePixels_static(_height_tok, _width_tok)
typedef A$$(6 + uint_log10Ceil_static(u16_limit_max) * 2, u8) dansi_xterm_window_ResizePixelsBuf;
$attr($must_check)
$extern fn_((
    dansi_xterm_window_resizePixels(u16 height, u16 width, dansi_xterm_window_ResizePixelsBuf* buf)
)(S$u8));
$attr($must_check)
$extern fn_((dansi_xterm_window_resizePixelsWrite(u16 height, u16 width, io_Writer out))(E$void));

#define dansi_xterm_window_resizeCells_static(_height_tok, _width_tok) \
    ____dansi_xterm_window_resizeCells_static(_height_tok, _width_tok)
typedef A$$(6 + uint_log10Ceil_static(u16_limit_max) * 2, u8) dansi_xterm_window_ResizeCellsBuf;
$attr($must_check)
$extern fn_((
    dansi_xterm_window_resizeCells(u16 height, u16 width, dansi_xterm_window_ResizeCellsBuf* buf)
)(S$u8));
$attr($must_check)
$extern fn_((dansi_xterm_window_resizeCellsWrite(u16 height, u16 width, io_Writer out))(E$void));

#define dansi_xterm_window_maximize_static(_mode_tok) \
    ____dansi_xterm_window_maximize_static(_mode_tok)
typedef A$$(6 + uint_log10Ceil_static(u8_limit_max), u8) dansi_xterm_window_MaximizeBuf;
$attr($must_check)
$extern fn_((dansi_xterm_window_maximize(
    dansi_xterm_window_Maximize mode, dansi_xterm_window_MaximizeBuf* buf
))(S$u8));
$attr($must_check)
$extern fn_((dansi_xterm_window_maximizeWrite(dansi_xterm_window_Maximize mode, io_Writer out))(E$void));

#define dansi_xterm_window_fullscreen_static(_mode_tok) \
    ____dansi_xterm_window_fullscreen_static(_mode_tok)
typedef A$$(7 + uint_log10Ceil_static(u8_limit_max), u8) dansi_xterm_window_FullscreenBuf;
$attr($must_check)
$extern fn_((dansi_xterm_window_fullscreen(
    dansi_xterm_window_Fullscreen mode, dansi_xterm_window_FullscreenBuf* buf
))(S$u8));
$attr($must_check)
$extern fn_((dansi_xterm_window_fullscreenWrite(dansi_xterm_window_Fullscreen mode, io_Writer out))(E$void));

#define dansi_xterm_window_requestState_static() \
    ____dansi_xterm_window_requestState_static()
$extern fn_((dansi_xterm_window_requestState(void))(S_const$u8));
$attr($must_check)
$extern fn_((dansi_xterm_window_requestStateWrite(io_Writer out))(E$void));
$attr($must_check)
$extern fn_((dansi_xterm_window_receiveStateReport(io_Reader in, S$u8 buf))(E$S$u8));
$attr($must_check)
$extern fn_((dansi_xterm_window_parseStateReport(
    S_const$u8 report
))(dansi_xterm_window_E$dansi_xterm_window_State));
$attr($must_check)
$extern fn_((dansi_xterm_window_fetchState(
    io_Writer out, io_Reader in, S$u8 buf
))(dansi_xterm_window_E$dansi_xterm_window_State));

#define dansi_xterm_window_requestPos_static() \
    ____dansi_xterm_window_requestPos_static()
$extern fn_((dansi_xterm_window_requestPos(void))(S_const$u8));
$attr($must_check)
$extern fn_((dansi_xterm_window_requestPosWrite(io_Writer out))(E$void));
$attr($must_check)
$extern fn_((dansi_xterm_window_receivePosReport(io_Reader in, S$u8 buf))(E$S$u8));
$attr($must_check)
$extern fn_((dansi_xterm_window_parsePosReport(
    S_const$u8 report
))(dansi_xterm_window_E$dansi_xterm_window_Pos));
$attr($must_check)
$extern fn_((dansi_xterm_window_fetchPos(
    io_Writer out, io_Reader in, S$u8 buf
))(dansi_xterm_window_E$dansi_xterm_window_Pos));

/*========== Macros and Definitions =========================================*/

#define __str__dansi_xterm_window_report_final "t"
#define __uint__dansi_xterm_window_report_final_byte u8_c('t')
#define __uint__dansi_xterm_window_report_param_code 0
#define __uint__dansi_xterm_window_report_param_x 1
#define __uint__dansi_xterm_window_report_param_y 2
#define ____dansi_xterm_window_op_static(_op_tok) \
    dansi_csi_make1_static(dansi_xterm_window_Op_staticParse(_op_tok), dansi_xterm_window_report_final)
#define ____dansi_xterm_window_move_static(_x_tok, _y_tok) \
    dansi_csi_make_static(dansi_xterm_window_Op_staticParse(dansi_xterm_window_Op_move) dansi_csi_param_sep _x_tok dansi_csi_param_sep _y_tok, "", dansi_xterm_window_report_final)
#define ____dansi_xterm_window_resizePixels_static(_height_tok, _width_tok) \
    dansi_csi_make_static(dansi_xterm_window_Op_staticParse(dansi_xterm_window_Op_resize_pixels) dansi_csi_param_sep _height_tok dansi_csi_param_sep _width_tok, "", dansi_xterm_window_report_final)
#define ____dansi_xterm_window_resizeCells_static(_height_tok, _width_tok) \
    dansi_csi_make_static(dansi_xterm_window_Op_staticParse(dansi_xterm_window_Op_resize_cells) dansi_csi_param_sep _height_tok dansi_csi_param_sep _width_tok, "", dansi_xterm_window_report_final)
#define ____dansi_xterm_window_maximize_static(_mode_tok) \
    dansi_csi_make_static(dansi_xterm_window_Op_staticParse(dansi_xterm_window_Op_maximize) dansi_csi_param_sep dansi_xterm_window_Maximize_staticParse(_mode_tok), "", dansi_xterm_window_report_final)
#define ____dansi_xterm_window_fullscreen_static(_mode_tok) \
    dansi_csi_make_static(dansi_xterm_window_Op_staticParse(dansi_xterm_window_Op_fullscreen) dansi_csi_param_sep dansi_xterm_window_Fullscreen_staticParse(_mode_tok), "", dansi_xterm_window_report_final)
#define ____dansi_xterm_window_requestState_static() \
    dansi_xterm_window_op_static(dansi_xterm_window_Op_report_state)
#define ____dansi_xterm_window_requestPos_static() \
    dansi_xterm_window_op_static(dansi_xterm_window_Op_report_pos)
#define ____dansi_xterm_window_Op_staticParse(_op_tok) \
    pp_join($, ____dansi_xterm_window_Op_str, _op_tok)
#define ____dansi_xterm_window_Op_str$dansi_xterm_window_Op_deiconify "1"
#define ____dansi_xterm_window_Op_str$dansi_xterm_window_Op_iconify "2"
#define ____dansi_xterm_window_Op_str$dansi_xterm_window_Op_move "3"
#define ____dansi_xterm_window_Op_str$dansi_xterm_window_Op_resize_pixels "4"
#define ____dansi_xterm_window_Op_str$dansi_xterm_window_Op_raise "5"
#define ____dansi_xterm_window_Op_str$dansi_xterm_window_Op_lower "6"
#define ____dansi_xterm_window_Op_str$dansi_xterm_window_Op_refresh "7"
#define ____dansi_xterm_window_Op_str$dansi_xterm_window_Op_resize_cells "8"
#define ____dansi_xterm_window_Op_str$dansi_xterm_window_Op_maximize "9"
#define ____dansi_xterm_window_Op_str$dansi_xterm_window_Op_fullscreen "10"
#define ____dansi_xterm_window_Op_str$dansi_xterm_window_Op_report_state "11"
#define ____dansi_xterm_window_Op_str$dansi_xterm_window_Op_report_pos "13"
#define ____dansi_xterm_window_Op_str$dansi_xterm_window_Op_report_text_area_pixels "14"
#define ____dansi_xterm_window_Op_str$dansi_xterm_window_Op_report_screen_pixels "15"
#define ____dansi_xterm_window_Op_str$dansi_xterm_window_Op_report_cell_pixels "16"
#define ____dansi_xterm_window_Op_str$dansi_xterm_window_Op_report_text_area_cells "18"
#define ____dansi_xterm_window_Op_str$dansi_xterm_window_Op_report_screen_cells "19"
#define ____dansi_xterm_window_Op_str$dansi_xterm_window_Op_report_icon_label "20"
#define ____dansi_xterm_window_Op_str$dansi_xterm_window_Op_report_title "21"
#define ____dansi_xterm_window_Maximize_staticParse(_mode_tok) \
    pp_join($, ____dansi_xterm_window_Maximize_str, _mode_tok)
#define ____dansi_xterm_window_Maximize_str$dansi_xterm_window_Maximize_restore "0"
#define ____dansi_xterm_window_Maximize_str$dansi_xterm_window_Maximize_full "1"
#define ____dansi_xterm_window_Maximize_str$dansi_xterm_window_Maximize_vertical "2"
#define ____dansi_xterm_window_Maximize_str$dansi_xterm_window_Maximize_horizontal "3"
#define ____dansi_xterm_window_Fullscreen_staticParse(_mode_tok) \
    pp_join($, ____dansi_xterm_window_Fullscreen_str, _mode_tok)
#define ____dansi_xterm_window_Fullscreen_str$dansi_xterm_window_Fullscreen_restore "0"
#define ____dansi_xterm_window_Fullscreen_str$dansi_xterm_window_Fullscreen_set "1"
#define ____dansi_xterm_window_Fullscreen_str$dansi_xterm_window_Fullscreen_toggle "2"

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* dansi_xterm_window__included */

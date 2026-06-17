/**
 * @copyright Copyright (c) 2026 Gyeongtae Kim. All rights reserved.
 * @license   Proprietary and confidential. Unauthorized use, reproduction,
 *            or distribution is strictly prohibited.
 *
 * @file    window.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2026-06-17 (date of creation)
 * @updated 2026-06-17 (date of last update)
 * @ingroup dasae-headers-workspace(dh-workspace)/dansi
 * @prefix  dansi_window
 *
 * @brief   xterm window manipulation controls (XTWINOPS).
 */
#pragma once
#ifndef dansi_window__included
#define dansi_window__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include <dh/io/common.h>

/*========== Macros and Declarations ========================================*/

typedef enum_((dansi_window_Op $fits($packed))(
    dansi_window_Op_deiconify = 1,
    dansi_window_Op_iconify = 2,
    dansi_window_Op_raise = 5,
    dansi_window_Op_lower = 6,
    dansi_window_Op_refresh = 7,
    dansi_window_Op_report_state = 11,
    dansi_window_Op_report_pos = 13,
    dansi_window_Op_report_text_area_pixels = 14,
    dansi_window_Op_report_screen_pixels = 15,
    dansi_window_Op_report_cell_pixels = 16,
    dansi_window_Op_report_text_area_chars = 18,
    dansi_window_Op_report_screen_chars = 19,
    dansi_window_Op_report_icon_label = 20,
    dansi_window_Op_report_title = 21
)) dansi_window_Op;

typedef enum_((dansi_window_Maximize $fits($packed))(
    dansi_window_Maximize_restore = 0,
    dansi_window_Maximize_full = 1,
    dansi_window_Maximize_vertical = 2,
    dansi_window_Maximize_horizontal = 3
)) dansi_window_Maximize;

typedef enum_((dansi_window_Fullscreen $fits($packed))(
    dansi_window_Fullscreen_restore = 0,
    dansi_window_Fullscreen_set = 1,
    dansi_window_Fullscreen_toggle = 2
)) dansi_window_Fullscreen;

typedef A$$(32, u8) dansi_window_Buf;

#define dansi_window_op_static(_op_tok) \
    ____dansi_window_op_static(_op_tok)
$extern fn_((dansi_window_op(dansi_window_Op op, dansi_window_Buf* buf))(S$u8));
$attr($must_check)
$extern fn_((dansi_window_opWrite(dansi_window_Op op, io_Writer writer))(E$void));

#define dansi_window_move_static(_x_tok, _y_tok) \
    ____dansi_window_move_static(_x_tok, _y_tok)
$extern fn_((dansi_window_move(u16 x, u16 y, dansi_window_Buf* buf))(S$u8));
$attr($must_check)
$extern fn_((dansi_window_moveWrite(u16 x, u16 y, io_Writer writer))(E$void));

#define dansi_window_resizePixels_static(_height_tok, _width_tok) \
    ____dansi_window_resizePixels_static(_height_tok, _width_tok)
$extern fn_((dansi_window_resizePixels(u16 height, u16 width, dansi_window_Buf* buf))(S$u8));
$attr($must_check)
$extern fn_((dansi_window_resizePixelsWrite(u16 height, u16 width, io_Writer writer))(E$void));

#define dansi_window_resizeChars_static(_height_tok, _width_tok) \
    ____dansi_window_resizeChars_static(_height_tok, _width_tok)
$extern fn_((dansi_window_resizeChars(u16 height, u16 width, dansi_window_Buf* buf))(S$u8));
$attr($must_check)
$extern fn_((dansi_window_resizeCharsWrite(u16 height, u16 width, io_Writer writer))(E$void));

#define dansi_window_maximize_static(_mode_tok) \
    ____dansi_window_maximize_static(_mode_tok)
$extern fn_((dansi_window_maximize(dansi_window_Maximize mode, dansi_window_Buf* buf))(S$u8));
$attr($must_check)
$extern fn_((dansi_window_maximizeWrite(dansi_window_Maximize mode, io_Writer writer))(E$void));

#define dansi_window_fullscreen_static(_mode_tok) \
    ____dansi_window_fullscreen_static(_mode_tok)
$extern fn_((dansi_window_fullscreen(dansi_window_Fullscreen mode, dansi_window_Buf* buf))(S$u8));
$attr($must_check)
$extern fn_((dansi_window_fullscreenWrite(dansi_window_Fullscreen mode, io_Writer writer))(E$void));

/*========== Macros and Definitions =========================================*/

#include "utils.h"

#define ____dansi_window_op_static(_op_tok) \
    dansi_utils_csi _op_tok dansi_utils_window_op
#define ____dansi_window_move_static(_x_tok, _y_tok) \
    dansi_utils_csi "3" dansi_utils_sep _x_tok dansi_utils_sep _y_tok dansi_utils_window_op
#define ____dansi_window_resizePixels_static(_height_tok, _width_tok) \
    dansi_utils_csi "4" dansi_utils_sep _height_tok dansi_utils_sep _width_tok dansi_utils_window_op
#define ____dansi_window_resizeChars_static(_height_tok, _width_tok) \
    dansi_utils_csi "8" dansi_utils_sep _height_tok dansi_utils_sep _width_tok dansi_utils_window_op
#define ____dansi_window_maximize_static(_mode_tok) \
    dansi_utils_csi "9" dansi_utils_sep _mode_tok dansi_utils_window_op
#define ____dansi_window_fullscreen_static(_mode_tok) \
    dansi_utils_csi "10" dansi_utils_sep _mode_tok dansi_utils_window_op

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* dansi_window__included */

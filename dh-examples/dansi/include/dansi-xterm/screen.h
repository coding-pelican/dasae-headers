/**
 * @copyright Copyright (c) 2026 Gyeongtae Kim. All rights reserved.
 * @license   Proprietary and confidential. Unauthorized use, reproduction,
 *            or distribution is strictly prohibited.
 *
 * @file    screen.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2026-06-19 (date of creation)
 * @updated 2026-06-19 (date of last update)
 * @ingroup dasae-headers-workspace(dh-workspace)/dansi
 * @prefix  dansi_xterm_screen
 */
#pragma once
#ifndef dansi_xterm_screen__included
#define dansi_xterm_screen__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "dansi-core/csi.h"
#include "mode.h"

/*========== Macros and Declarations ========================================*/

#define dansi_xterm_screen_report_final __str__dansi_xterm_screen_report_final
#define dansi_xterm_screen_report_final_byte __uint__dansi_xterm_screen_report_final_byte
#define dansi_xterm_screen_request_cell_pixels __str__dansi_xterm_screen_request_cell_pixels
#define dansi_xterm_screen_request_cell_pixels_u16 __uint__dansi_xterm_screen_request_cell_pixels_u16
#define dansi_xterm_screen_request_text_area_cells __str__dansi_xterm_screen_request_text_area_cells
#define dansi_xterm_screen_request_text_area_cells_u16 __uint__dansi_xterm_screen_request_text_area_cells_u16
#define dansi_xterm_screen_request_screen_cells __str__dansi_xterm_screen_request_screen_cells
#define dansi_xterm_screen_request_screen_cells_u16 __uint__dansi_xterm_screen_request_screen_cells_u16
#define dansi_xterm_screen_request_text_area_pixels __str__dansi_xterm_screen_request_text_area_pixels
#define dansi_xterm_screen_request_text_area_pixels_u16 __uint__dansi_xterm_screen_request_text_area_pixels_u16
#define dansi_xterm_screen_request_screen_pixels __str__dansi_xterm_screen_request_screen_pixels
#define dansi_xterm_screen_request_screen_pixels_u16 __uint__dansi_xterm_screen_request_screen_pixels_u16
#define dansi_xterm_screen_response_text_area_pixels __uint__dansi_xterm_screen_response_text_area_pixels
#define dansi_xterm_screen_response_screen_pixels __uint__dansi_xterm_screen_response_screen_pixels
#define dansi_xterm_screen_response_cell_pixels __uint__dansi_xterm_screen_response_cell_pixels
#define dansi_xterm_screen_response_text_area_cells __uint__dansi_xterm_screen_response_text_area_cells
#define dansi_xterm_screen_response_screen_cells __uint__dansi_xterm_screen_response_screen_cells
#define dansi_xterm_screen_report_param_code __uint__dansi_xterm_screen_report_param_code
#define dansi_xterm_screen_report_param_height __uint__dansi_xterm_screen_report_param_height
#define dansi_xterm_screen_report_param_width __uint__dansi_xterm_screen_report_param_width

typedef struct dansi_xterm_screen_CellCount {
    var_(cols, u16);
    var_(rows, u16);
} dansi_xterm_screen_CellCount;
T_use_prl$(dansi_xterm_screen_CellCount);

typedef struct dansi_xterm_screen_PixelSize {
    var_(width, u16);
    var_(height, u16);
} dansi_xterm_screen_PixelSize;
T_use_prl$(dansi_xterm_screen_PixelSize);

errset_((dansi_xterm_screen_E)(dansi_xterm_screen_InvalidResponse));
T_use_E$($set(dansi_xterm_screen_E)(dansi_xterm_screen_CellCount));
T_use_E$($set(dansi_xterm_screen_E)(dansi_xterm_screen_PixelSize));

#define dansi_xterm_screen_enterAlternate_static() \
    ____dansi_xterm_screen_enterAlternate_static()
$extern fn_((dansi_xterm_screen_enterAlternate(void))(S_const$u8));
$attr($must_check)
$extern fn_((dansi_xterm_screen_enterAlternateWrite(io_Writer out))(io_WriteE$void));

#define dansi_xterm_screen_exitAlternate_static() \
    ____dansi_xterm_screen_exitAlternate_static()
$extern fn_((dansi_xterm_screen_exitAlternate(void))(S_const$u8));
$attr($must_check)
$extern fn_((dansi_xterm_screen_exitAlternateWrite(io_Writer out))(io_WriteE$void));

#define dansi_xterm_screen_enterAlternateSaveCursor_static() \
    ____dansi_xterm_screen_enterAlternateSaveCursor_static()
$extern fn_((dansi_xterm_screen_enterAlternateSaveCursor(void))(S_const$u8));
$attr($must_check)
$extern fn_((dansi_xterm_screen_enterAlternateSaveCursorWrite(io_Writer out))(io_WriteE$void));

#define dansi_xterm_screen_exitAlternateSaveCursor_static() \
    ____dansi_xterm_screen_exitAlternateSaveCursor_static()
$extern fn_((dansi_xterm_screen_exitAlternateSaveCursor(void))(S_const$u8));
$attr($must_check)
$extern fn_((dansi_xterm_screen_exitAlternateSaveCursorWrite(io_Writer out))(io_WriteE$void));

#define dansi_xterm_screen_requestCellPixels_static() \
    ____dansi_xterm_screen_requestCellPixels_static()
$extern fn_((dansi_xterm_screen_requestCellPixels(void))(S_const$u8));
$attr($must_check)
$extern fn_((dansi_xterm_screen_requestCellPixelsWrite(io_Writer out))(io_WriteE$void));
$attr($must_check)
$extern fn_((dansi_xterm_screen_receiveCellPixelsReport(io_Reader in, S$u8 buf))(E$S$u8));
$attr($must_check)
$extern fn_((dansi_xterm_screen_parseCellPixelsReport(
    S_const$u8 report
))(dansi_xterm_screen_E$dansi_xterm_screen_PixelSize));
$attr($must_check)
$extern fn_((dansi_xterm_screen_fetchCellPixels(
    io_Writer out, io_Reader in, S$u8 buf
))(dansi_xterm_screen_E$dansi_xterm_screen_PixelSize));

#define dansi_xterm_screen_requestTextAreaCells_static() \
    ____dansi_xterm_screen_requestTextAreaCells_static()
$extern fn_((dansi_xterm_screen_requestTextAreaCells(void))(S_const$u8));
$attr($must_check)
$extern fn_((dansi_xterm_screen_requestTextAreaCellsWrite(io_Writer out))(io_WriteE$void));
$attr($must_check)
$extern fn_((dansi_xterm_screen_receiveTextAreaCellsReport(io_Reader in, S$u8 buf))(E$S$u8));
$attr($must_check)
$extern fn_((dansi_xterm_screen_parseTextAreaCellsReport(
    S_const$u8 report
))(dansi_xterm_screen_E$dansi_xterm_screen_CellCount));
$attr($must_check)
$extern fn_((dansi_xterm_screen_fetchTextAreaCells(
    io_Writer out, io_Reader in, S$u8 buf
))(dansi_xterm_screen_E$dansi_xterm_screen_CellCount));

#define dansi_xterm_screen_requestScreenCells_static() \
    ____dansi_xterm_screen_requestScreenCells_static()
$extern fn_((dansi_xterm_screen_requestScreenCells(void))(S_const$u8));
$attr($must_check)
$extern fn_((dansi_xterm_screen_requestScreenCellsWrite(io_Writer out))(io_WriteE$void));
$attr($must_check)
$extern fn_((dansi_xterm_screen_receiveScreenCellsReport(io_Reader in, S$u8 buf))(E$S$u8));
$attr($must_check)
$extern fn_((dansi_xterm_screen_parseScreenCellsReport(
    S_const$u8 report
))(dansi_xterm_screen_E$dansi_xterm_screen_CellCount));
$attr($must_check)
$extern fn_((dansi_xterm_screen_fetchScreenCells(
    io_Writer out, io_Reader in, S$u8 buf
))(dansi_xterm_screen_E$dansi_xterm_screen_CellCount));

#define dansi_xterm_screen_requestTextAreaPixels_static() \
    ____dansi_xterm_screen_requestTextAreaPixels_static()
$extern fn_((dansi_xterm_screen_requestTextAreaPixels(void))(S_const$u8));
$attr($must_check)
$extern fn_((dansi_xterm_screen_requestTextAreaPixelsWrite(io_Writer out))(io_WriteE$void));
$attr($must_check)
$extern fn_((dansi_xterm_screen_receiveTextAreaPixelsReport(io_Reader in, S$u8 buf))(E$S$u8));
$attr($must_check)
$extern fn_((dansi_xterm_screen_parseTextAreaPixelsReport(
    S_const$u8 report
))(dansi_xterm_screen_E$dansi_xterm_screen_PixelSize));
$attr($must_check)
$extern fn_((dansi_xterm_screen_fetchTextAreaPixels(
    io_Writer out, io_Reader in, S$u8 buf
))(dansi_xterm_screen_E$dansi_xterm_screen_PixelSize));

#define dansi_xterm_screen_requestScreenPixels_static() \
    ____dansi_xterm_screen_requestScreenPixels_static()
$extern fn_((dansi_xterm_screen_requestScreenPixels(void))(S_const$u8));
$attr($must_check)
$extern fn_((dansi_xterm_screen_requestScreenPixelsWrite(io_Writer out))(io_WriteE$void));
$attr($must_check)
$extern fn_((dansi_xterm_screen_receiveScreenPixelsReport(io_Reader in, S$u8 buf))(E$S$u8));
$attr($must_check)
$extern fn_((dansi_xterm_screen_parseScreenPixelsReport(
    S_const$u8 report
))(dansi_xterm_screen_E$dansi_xterm_screen_PixelSize));
$attr($must_check)
$extern fn_((dansi_xterm_screen_fetchScreenPixels(
    io_Writer out, io_Reader in, S$u8 buf
))(dansi_xterm_screen_E$dansi_xterm_screen_PixelSize));

/*========== Macros and Definitions =========================================*/

#define __str__dansi_xterm_screen_report_final "t"
#define __uint__dansi_xterm_screen_report_final_byte u8_c('t')
#define __str__dansi_xterm_screen_request_cell_pixels "16"
#define __uint__dansi_xterm_screen_request_cell_pixels_u16 16
#define __str__dansi_xterm_screen_request_text_area_cells "18"
#define __uint__dansi_xterm_screen_request_text_area_cells_u16 18
#define __str__dansi_xterm_screen_request_screen_cells "19"
#define __uint__dansi_xterm_screen_request_screen_cells_u16 19
#define __str__dansi_xterm_screen_request_text_area_pixels "14"
#define __uint__dansi_xterm_screen_request_text_area_pixels_u16 14
#define __str__dansi_xterm_screen_request_screen_pixels "15"
#define __uint__dansi_xterm_screen_request_screen_pixels_u16 15
#define __uint__dansi_xterm_screen_response_text_area_pixels 4
#define __uint__dansi_xterm_screen_response_screen_pixels 5
#define __uint__dansi_xterm_screen_response_cell_pixels 6
#define __uint__dansi_xterm_screen_response_text_area_cells 8
#define __uint__dansi_xterm_screen_response_screen_cells 9
#define __uint__dansi_xterm_screen_report_param_code 0
#define __uint__dansi_xterm_screen_report_param_height 1
#define __uint__dansi_xterm_screen_report_param_width 2
#define ____dansi_xterm_screen_enterAlternate_static() \
    dansi_xterm_mode_enable_static(dansi_xterm_mode_Code_alt_screen)
#define ____dansi_xterm_screen_exitAlternate_static() \
    dansi_xterm_mode_disable_static(dansi_xterm_mode_Code_alt_screen)
#define ____dansi_xterm_screen_enterAlternateSaveCursor_static() \
    dansi_xterm_mode_enable_static(dansi_xterm_mode_Code_alt_screen_save_cursor)
#define ____dansi_xterm_screen_exitAlternateSaveCursor_static() \
    dansi_xterm_mode_disable_static(dansi_xterm_mode_Code_alt_screen_save_cursor)
#define ____dansi_xterm_screen_requestCellPixels_static() \
    dansi_csi_make1_static(dansi_xterm_screen_request_cell_pixels, dansi_xterm_screen_report_final)
#define ____dansi_xterm_screen_requestTextAreaCells_static() \
    dansi_csi_make1_static(dansi_xterm_screen_request_text_area_cells, dansi_xterm_screen_report_final)
#define ____dansi_xterm_screen_requestScreenCells_static() \
    dansi_csi_make1_static(dansi_xterm_screen_request_screen_cells, dansi_xterm_screen_report_final)
#define ____dansi_xterm_screen_requestTextAreaPixels_static() \
    dansi_csi_make1_static(dansi_xterm_screen_request_text_area_pixels, dansi_xterm_screen_report_final)
#define ____dansi_xterm_screen_requestScreenPixels_static() \
    dansi_csi_make1_static(dansi_xterm_screen_request_screen_pixels, dansi_xterm_screen_report_final)

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* dansi_xterm_screen__included */

/**
 * @copyright Copyright (c) 2026 Gyeongtae Kim. All rights reserved.
 * @license   Proprietary and confidential. Unauthorized use, reproduction,
 *            or distribution is strictly prohibited.
 *
 * @file    palette.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2026-06-19 (date of creation)
 * @updated 2026-06-19 (date of last update)
 * @ingroup dasae-headers-workspace(dh-workspace)/dansi
 * @prefix  dansi_xterm_palette
 */
#pragma once
#ifndef dansi_xterm_palette__included
#define dansi_xterm_palette__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "dansi-core/csi.h"
#include "dansi-core/osc.h"
#include "Palette4bit.h"
#include "Palette8bit.h"
#include "color.h"

/*========== Macros and Declarations ========================================*/

#define dansi_xterm_palette_stack_intermediate __str__dansi_xterm_palette_stack_intermediate
#define dansi_xterm_palette_stack_intermediate_byte __uint__dansi_xterm_palette_stack_intermediate_byte
#define dansi_xterm_palette_stack_push_final __str__dansi_xterm_palette_stack_push_final
#define dansi_xterm_palette_stack_push_final_byte __uint__dansi_xterm_palette_stack_push_final_byte
#define dansi_xterm_palette_stack_pop_final __str__dansi_xterm_palette_stack_pop_final
#define dansi_xterm_palette_stack_pop_final_byte __uint__dansi_xterm_palette_stack_pop_final_byte
#define dansi_xterm_palette_stack_report_final __str__dansi_xterm_palette_stack_report_final
#define dansi_xterm_palette_stack_report_final_byte __uint__dansi_xterm_palette_stack_report_final_byte
#define dansi_xterm_palette_color_cmd __str__dansi_xterm_palette_color_cmd
#define dansi_xterm_palette_color_cmd_u16 __uint__dansi_xterm_palette_color_cmd_u16
#define dansi_xterm_palette_reset_color_cmd __str__dansi_xterm_palette_reset_color_cmd
#define dansi_xterm_palette_reset_color_cmd_u16 __uint__dansi_xterm_palette_reset_color_cmd_u16
#define dansi_xterm_palette_payload_sep __str__dansi_xterm_palette_payload_sep
#define dansi_xterm_palette_payload_sep_byte __uint__dansi_xterm_palette_payload_sep_byte
#define dansi_xterm_palette_rgb_prefix __str__dansi_xterm_palette_rgb_prefix
#define dansi_xterm_palette_rgb_prefix_len __uint__dansi_xterm_palette_rgb_prefix_len
#define dansi_xterm_palette_rgb_sep __str__dansi_xterm_palette_rgb_sep
#define dansi_xterm_palette_rgb_sep_byte __uint__dansi_xterm_palette_rgb_sep_byte
#define dansi_xterm_palette_query __str__dansi_xterm_palette_query
#define dansi_xterm_palette_query_byte __uint__dansi_xterm_palette_query_byte
#define dansi_xterm_palette_index_radix __uint__dansi_xterm_palette_index_radix
#define dansi_xterm_palette_rgb_component_radix __uint__dansi_xterm_palette_rgb_component_radix

claim_assert_static(u8_(dansi_xterm_Palette4bit_black) == u8_(dansi_xterm_Palette8bit_black));
claim_assert_static(u8_(dansi_xterm_Palette4bit_maroon) == u8_(dansi_xterm_Palette8bit_maroon));
claim_assert_static(u8_(dansi_xterm_Palette4bit_green) == u8_(dansi_xterm_Palette8bit_green));
claim_assert_static(u8_(dansi_xterm_Palette4bit_olive) == u8_(dansi_xterm_Palette8bit_olive));
claim_assert_static(u8_(dansi_xterm_Palette4bit_navy) == u8_(dansi_xterm_Palette8bit_navy));
claim_assert_static(u8_(dansi_xterm_Palette4bit_purple) == u8_(dansi_xterm_Palette8bit_purple));
claim_assert_static(u8_(dansi_xterm_Palette4bit_teal) == u8_(dansi_xterm_Palette8bit_teal));
claim_assert_static(u8_(dansi_xterm_Palette4bit_silver) == u8_(dansi_xterm_Palette8bit_silver));
claim_assert_static(u8_(dansi_xterm_Palette4bit_grey) == u8_(dansi_xterm_Palette8bit_grey));
claim_assert_static(u8_(dansi_xterm_Palette4bit_red) == u8_(dansi_xterm_Palette8bit_red));
claim_assert_static(u8_(dansi_xterm_Palette4bit_lime) == u8_(dansi_xterm_Palette8bit_lime));
claim_assert_static(u8_(dansi_xterm_Palette4bit_yellow) == u8_(dansi_xterm_Palette8bit_yellow));
claim_assert_static(u8_(dansi_xterm_Palette4bit_blue) == u8_(dansi_xterm_Palette8bit_blue));
claim_assert_static(u8_(dansi_xterm_Palette4bit_fuchsia) == u8_(dansi_xterm_Palette8bit_fuchsia));
claim_assert_static(u8_(dansi_xterm_Palette4bit_aqua) == u8_(dansi_xterm_Palette8bit_aqua));
claim_assert_static(u8_(dansi_xterm_Palette4bit_white) == u8_(dansi_xterm_Palette8bit_white));

typedef struct dansi_xterm_palette_ColorReport {
    var_(index, u8);
    var_(rgb, dansi_xterm_color_RGB16);
} dansi_xterm_palette_ColorReport;
T_use_prl$(dansi_xterm_palette_ColorReport);

errset_((dansi_xterm_palette_E)(dansi_xterm_palette_InvalidResponse));
T_use_E$($set(dansi_xterm_palette_E)(dansi_xterm_palette_ColorReport));

#define dansi_xterm_palette_push_static() \
    ____dansi_xterm_palette_push_static()
$extern fn_((dansi_xterm_palette_push(void))(S_const$u8));
$attr($must_check)
$extern fn_((dansi_xterm_palette_pushWrite(io_Writer out))(io_WriteE$void));

#define dansi_xterm_palette_pushAt_static(_index_tok) \
    ____dansi_xterm_palette_pushAt_static(_index_tok)
typedef A$$(4 + uint_log10Ceil_static(u8_limit_max), u8) dansi_xterm_palette_StackBuf;
$attr($must_check)
$extern fn_((dansi_xterm_palette_pushAt(u8 index, dansi_xterm_palette_StackBuf* buf))(S$u8));
$attr($must_check)
$extern fn_((dansi_xterm_palette_pushAtWrite(u8 index, io_Writer out))(io_PrintE$void));

#define dansi_xterm_palette_pop_static() \
    ____dansi_xterm_palette_pop_static()
$extern fn_((dansi_xterm_palette_pop(void))(S_const$u8));
$attr($must_check)
$extern fn_((dansi_xterm_palette_popWrite(io_Writer out))(io_WriteE$void));

#define dansi_xterm_palette_popAt_static(_index_tok) \
    ____dansi_xterm_palette_popAt_static(_index_tok)
$attr($must_check)
$extern fn_((dansi_xterm_palette_popAt(u8 index, dansi_xterm_palette_StackBuf* buf))(S$u8));
$attr($must_check)
$extern fn_((dansi_xterm_palette_popAtWrite(u8 index, io_Writer out))(io_PrintE$void));

#define dansi_xterm_palette_reportStack_static() \
    ____dansi_xterm_palette_reportStack_static()
$extern fn_((dansi_xterm_palette_reportStack(void))(S_const$u8));
$attr($must_check)
$extern fn_((dansi_xterm_palette_reportStackWrite(io_Writer out))(io_WriteE$void));

#define dansi_xterm_palette_reportStackAt_static(_index_tok) \
    ____dansi_xterm_palette_reportStackAt_static(_index_tok)
$attr($must_check)
$extern fn_((dansi_xterm_palette_reportStackAt(u8 index, dansi_xterm_palette_StackBuf* buf))(S$u8));
$attr($must_check)
$extern fn_((dansi_xterm_palette_reportStackAtWrite(u8 index, io_Writer out))(io_PrintE$void));

#define dansi_xterm_palette_setColor16_static(_index_tok, _r_tok, _g_tok, _b_tok) \
    ____dansi_xterm_palette_setColor16_static(_index_tok, _r_tok, _g_tok, _b_tok)
typedef A$$(25 + uint_log10Ceil_static(u8_limit_max), u8) dansi_xterm_palette_SetColorBuf;
$attr($must_check)
$extern fn_((dansi_xterm_palette_setColor16(
    u8 index, dansi_xterm_color_RGB16 rgb, dansi_xterm_palette_SetColorBuf* buf
))(S$u8));
$attr($must_check)
$extern fn_((dansi_xterm_palette_setColor16Write(
    u8 index, dansi_xterm_color_RGB16 rgb, io_Writer out
))(io_PrintE$void));
$attr($must_check)
$extern fn_((dansi_xterm_palette_setColor(
    u8 index, dansi_xterm_color_RGB8 rgb, dansi_xterm_palette_SetColorBuf* buf
))(S$u8));
$attr($must_check)
$extern fn_((dansi_xterm_palette_setColorWrite(
    u8 index, dansi_xterm_color_RGB8 rgb, io_Writer out
))(io_PrintE$void));

#define dansi_xterm_palette_queryColor_static(_index_tok) \
    ____dansi_xterm_palette_queryColor_static(_index_tok)
typedef A$$(8 + uint_log10Ceil_static(u8_limit_max), u8) dansi_xterm_palette_QueryColorBuf;
$attr($must_check)
$extern fn_((dansi_xterm_palette_queryColor(u8 index, dansi_xterm_palette_QueryColorBuf* buf))(S$u8));
$attr($must_check)
$extern fn_((dansi_xterm_palette_queryColorWrite(u8 index, io_Writer out))(io_PrintE$void));
$attr($must_check)
$extern fn_((dansi_xterm_palette_receiveColorReport(io_Reader in, S$u8 buf))(E$S$u8));
$attr($must_check)
$extern fn_((dansi_xterm_palette_parseColorReport(
    dansi_osc_Frame frame
))(dansi_xterm_palette_E$dansi_xterm_palette_ColorReport));
$attr($must_check)
$extern fn_((dansi_xterm_palette_fetchColor(
    u8 index, io_Writer out, io_Reader in, S$u8 buf
))(dansi_xterm_palette_E$dansi_xterm_palette_ColorReport));

#define dansi_xterm_palette_resetColor_static(_index_tok) \
    ____dansi_xterm_palette_resetColor_static(_index_tok)
typedef A$$(8 + uint_log10Ceil_static(u8_limit_max), u8) dansi_xterm_palette_ResetColorBuf;
$attr($must_check)
$extern fn_((dansi_xterm_palette_resetColor(u8 index, dansi_xterm_palette_ResetColorBuf* buf))(S$u8));
$attr($must_check)
$extern fn_((dansi_xterm_palette_resetColorWrite(u8 index, io_Writer out))(io_PrintE$void));

#define dansi_xterm_palette_resetColors_static(_indices_tok) \
    ____dansi_xterm_palette_resetColors_static(_indices_tok)
$attr($must_check)
$extern fn_((dansi_xterm_palette_resetColors(S_const$u8 indices, S$u8 buf))(E$S$u8));
$attr($must_check)
$extern fn_((dansi_xterm_palette_resetColorsWrite(S_const$u8 indices, io_Writer out))(io_PrintE$void));

#define dansi_xterm_palette_resetAllColors_static() \
    ____dansi_xterm_palette_resetAllColors_static()
$extern fn_((dansi_xterm_palette_resetAllColors(void))(S_const$u8));
$attr($must_check)
$extern fn_((dansi_xterm_palette_resetAllColorsWrite(io_Writer out))(io_WriteE$void));

/*========== Macros and Definitions =========================================*/

#define __str__dansi_xterm_palette_stack_intermediate "#"
#define __uint__dansi_xterm_palette_stack_intermediate_byte u8_c('#')
#define __str__dansi_xterm_palette_stack_push_final "P"
#define __uint__dansi_xterm_palette_stack_push_final_byte u8_c('P')
#define __str__dansi_xterm_palette_stack_pop_final "Q"
#define __uint__dansi_xterm_palette_stack_pop_final_byte u8_c('Q')
#define __str__dansi_xterm_palette_stack_report_final "R"
#define __uint__dansi_xterm_palette_stack_report_final_byte u8_c('R')
#define __str__dansi_xterm_palette_color_cmd "4"
#define __uint__dansi_xterm_palette_color_cmd_u16 4
#define __str__dansi_xterm_palette_reset_color_cmd "104"
#define __uint__dansi_xterm_palette_reset_color_cmd_u16 104
#define __str__dansi_xterm_palette_payload_sep ";"
#define __uint__dansi_xterm_palette_payload_sep_byte u8_c(';')
#define __str__dansi_xterm_palette_rgb_prefix "rgb:"
#define __uint__dansi_xterm_palette_rgb_prefix_len (sizeof(dansi_xterm_palette_rgb_prefix) - 1)
#define __str__dansi_xterm_palette_rgb_sep "/"
#define __uint__dansi_xterm_palette_rgb_sep_byte u8_c('/')
#define __str__dansi_xterm_palette_query "?"
#define __uint__dansi_xterm_palette_query_byte u8_c('?')
#define __uint__dansi_xterm_palette_index_radix 10
#define __uint__dansi_xterm_palette_rgb_component_radix 16
#define ____dansi_xterm_palette_push_static() \
    dansi_csi_make_static("", dansi_xterm_palette_stack_intermediate, dansi_xterm_palette_stack_push_final)
#define ____dansi_xterm_palette_pushAt_static(_index_tok) \
    dansi_csi_make_static(_index_tok, dansi_xterm_palette_stack_intermediate, dansi_xterm_palette_stack_push_final)
#define ____dansi_xterm_palette_pop_static() \
    dansi_csi_make_static("", dansi_xterm_palette_stack_intermediate, dansi_xterm_palette_stack_pop_final)
#define ____dansi_xterm_palette_popAt_static(_index_tok) \
    dansi_csi_make_static(_index_tok, dansi_xterm_palette_stack_intermediate, dansi_xterm_palette_stack_pop_final)
#define ____dansi_xterm_palette_reportStack_static() \
    dansi_csi_make_static("", dansi_xterm_palette_stack_intermediate, dansi_xterm_palette_stack_report_final)
#define ____dansi_xterm_palette_reportStackAt_static(_index_tok) \
    dansi_csi_make_static(_index_tok, dansi_xterm_palette_stack_intermediate, dansi_xterm_palette_stack_report_final)
#define ____dansi_xterm_palette_setColor16_static(_index_tok, _r_tok, _g_tok, _b_tok) \
    dansi_osc_make_static(dansi_xterm_palette_color_cmd, _index_tok dansi_xterm_palette_payload_sep dansi_xterm_palette_rgb_prefix _r_tok dansi_xterm_palette_rgb_sep _g_tok dansi_xterm_palette_rgb_sep _b_tok)
#define ____dansi_xterm_palette_queryColor_static(_index_tok) \
    dansi_osc_make_static(dansi_xterm_palette_color_cmd, _index_tok dansi_xterm_palette_payload_sep dansi_xterm_palette_query)
#define ____dansi_xterm_palette_resetColor_static(_index_tok) \
    dansi_osc_make_static(dansi_xterm_palette_reset_color_cmd, _index_tok)
#define ____dansi_xterm_palette_resetColors_static(_indices_tok) \
    dansi_osc_make_static(dansi_xterm_palette_reset_color_cmd, _indices_tok)
#define ____dansi_xterm_palette_resetAllColors_static() \
    dansi_osc_makeRaw_static(dansi_xterm_palette_reset_color_cmd)

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* dansi_xterm_palette__included */

/**
 * @copyright Copyright (c) 2026 Gyeongtae Kim. All rights reserved.
 * @license   Proprietary and confidential. Unauthorized use, reproduction,
 *            or distribution is strictly prohibited.
 *
 * @file    scroll.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2026-06-18 (date of creation)
 * @updated 2026-06-18 (date of last update)
 * @ingroup dasae-headers-workspace(dh-workspace)/dansi
 * @prefix  dansi_scroll
 */
#pragma once
#ifndef dansi_scroll__included
#define dansi_scroll__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "csi.h"

/*========== Macros and Declarations ========================================*/

#define dansi_scroll_up_final __str__dansi_scroll_up_final
#define dansi_scroll_up_final_byte __uint__dansi_scroll_up_final_byte
#define dansi_scroll_down_final __str__dansi_scroll_down_final
#define dansi_scroll_down_final_byte __uint__dansi_scroll_down_final_byte

#define dansi_scroll_up_static(_lines_tok) \
    ____dansi_scroll_up_static(_lines_tok)
typedef A$$(3 + uint_log10Ceil_static(u16_limit_max), u8) dansi_scroll_UpBuf;
$attr($must_check)
$extern fn_((dansi_scroll_up(u16 lines, dansi_scroll_UpBuf* buf))(S$u8));
$attr($must_check)
$extern fn_((dansi_scroll_upWrite(u16 lines, io_Writer out))(io_PrintE$void));

#define dansi_scroll_down_static(_lines_tok) \
    ____dansi_scroll_down_static(_lines_tok)
typedef A$$(3 + uint_log10Ceil_static(u16_limit_max), u8) dansi_scroll_DownBuf;
$attr($must_check)
$extern fn_((dansi_scroll_down(u16 lines, dansi_scroll_DownBuf* buf))(S$u8));
$attr($must_check)
$extern fn_((dansi_scroll_downWrite(u16 lines, io_Writer out))(io_PrintE$void));

/*========== Macros and Definitions =========================================*/

#define __str__dansi_scroll_up_final "S"
#define __uint__dansi_scroll_up_final_byte u8_c('S')
#define __str__dansi_scroll_down_final "T"
#define __uint__dansi_scroll_down_final_byte u8_c('T')
#define ____dansi_scroll_up_static(_lines_tok) \
    dansi_csi_make1_static(_lines_tok, dansi_scroll_up_final)
#define ____dansi_scroll_down_static(_lines_tok) \
    dansi_csi_make1_static(_lines_tok, dansi_scroll_down_final)

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* dansi_scroll__included */

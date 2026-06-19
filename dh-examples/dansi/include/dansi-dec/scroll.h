/**
 * @copyright Copyright (c) 2026 Gyeongtae Kim. All rights reserved.
 * @license   Proprietary and confidential. Unauthorized use, reproduction,
 *            or distribution is strictly prohibited.
 *
 * @file    scroll.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2026-06-19 (date of creation)
 * @updated 2026-06-19 (date of last update)
 * @ingroup dasae-headers-workspace(dh-workspace)/dansi
 * @prefix  dansi_dec_scroll
 */
#pragma once
#ifndef dansi_dec_scroll__included
#define dansi_dec_scroll__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "dansi-core/csi.h"

/*========== Macros and Declarations ========================================*/

#define dansi_dec_scroll_setMargins_static(_top_tok, _bottom_tok) \
    ____dansi_dec_scroll_setMargins_static(_top_tok, _bottom_tok)
typedef A$$(4 + uint_log10Ceil_static(u16_limit_max) * 2, u8) dansi_dec_scroll_SetMarginsBuf;
$attr($must_check)
$extern fn_((dansi_dec_scroll_setMargins(
    u16 top, u16 bottom, dansi_dec_scroll_SetMarginsBuf* buf
))(S$u8));
$attr($must_check)
$extern fn_((dansi_dec_scroll_setMarginsWrite(u16 top, u16 bottom, io_Writer out))(E$void));

#define dansi_dec_scroll_resetMargins_static() \
    ____dansi_dec_scroll_resetMargins_static()
$attr($must_check)
$extern fn_((dansi_dec_scroll_resetMargins(void))(S_const$u8));
$attr($must_check)
$extern fn_((dansi_dec_scroll_resetMarginsWrite(io_Writer out))(E$void));

/*========== Macros and Definitions =========================================*/

#define ____dansi_dec_scroll_setMargins_static(_top_tok, _bottom_tok) \
    dansi_csi_make2_static(_top_tok, _bottom_tok, "r")
#define ____dansi_dec_scroll_resetMargins_static() \
    dansi_csi_make0_static("r")

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* dansi_dec_scroll__included */

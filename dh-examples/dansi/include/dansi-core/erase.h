/**
 * @copyright Copyright (c) 2026 Gyeongtae Kim. All rights reserved.
 * @license   Proprietary and confidential. Unauthorized use, reproduction,
 *            or distribution is strictly prohibited.
 *
 * @file    erase.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2026-06-18 (date of creation)
 * @updated 2026-06-18 (date of last update)
 * @ingroup dasae-headers-workspace(dh-workspace)/dansi
 * @prefix  dansi_erase
 */
#pragma once
#ifndef dansi_erase__included
#define dansi_erase__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "csi.h"

/*========== Macros and Declarations ========================================*/

typedef enum_((dansi_erase_Area $fits($packed))(
    dansi_erase_Area_to_end = 0,
    dansi_erase_Area_to_start = 1,
    dansi_erase_Area_all = 2
)) dansi_erase_Area;
claim_assert_static(eqlType$(dansi_erase_Area, u8));
T_use_prl$(dansi_erase_Area);

#define dansi_erase_Area_staticParse(_area_tok) \
    ____dansi_erase_Area_staticParse(_area_tok)

#define dansi_erase_inDisplay_static(_area_tok) \
    ____dansi_erase_inDisplay_static(_area_tok)
typedef A$$(3 + uint_log10Ceil_static(u8_limit_max), u8) dansi_erase_InDisplayBuf;
$attr($must_check)
$extern fn_((dansi_erase_inDisplay(dansi_erase_Area area, dansi_erase_InDisplayBuf* buf))(S$u8));
$attr($must_check)
$extern fn_((dansi_erase_inDisplayWrite(dansi_erase_Area area, io_Writer out))(E$void));

#define dansi_erase_inLine_static(_area_tok) \
    ____dansi_erase_inLine_static(_area_tok)
typedef A$$(3 + uint_log10Ceil_static(u8_limit_max), u8) dansi_erase_InLineBuf;
$attr($must_check)
$extern fn_((dansi_erase_inLine(dansi_erase_Area area, dansi_erase_InLineBuf* buf))(S$u8));
$attr($must_check)
$extern fn_((dansi_erase_inLineWrite(dansi_erase_Area area, io_Writer out))(E$void));

#define dansi_erase_chars_static(_count_tok) \
    ____dansi_erase_chars_static(_count_tok)
typedef A$$(3 + uint_log10Ceil_static(u16_limit_max), u8) dansi_erase_CharsBuf;
$attr($must_check)
$extern fn_((dansi_erase_chars(u16 count, dansi_erase_CharsBuf* buf))(S$u8));
$attr($must_check)
$extern fn_((dansi_erase_charsWrite(u16 count, io_Writer out))(E$void));

/*========== Macros and Definitions =========================================*/

#define ____dansi_erase_inDisplay_static(_area_tok) \
    dansi_csi_make1_static(dansi_erase_Area_staticParse(_area_tok), "J")
#define ____dansi_erase_inLine_static(_area_tok) \
    dansi_csi_make1_static(dansi_erase_Area_staticParse(_area_tok), "K")
#define ____dansi_erase_chars_static(_count_tok) \
    dansi_csi_make1_static(_count_tok, "X")
#define ____dansi_erase_Area_staticParse(_area_tok) \
    pp_join(____dansi_erase_Area_str$, _area_tok)
#define ____dansi_erase_Area_str$dansi_erase_Area_to_end "0"
#define ____dansi_erase_Area_str$dansi_erase_Area_to_start "1"
#define ____dansi_erase_Area_str$dansi_erase_Area_all "2"

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* dansi_erase__included */

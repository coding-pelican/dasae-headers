/**
 * @copyright Copyright (c) 2026 Gyeongtae Kim. All rights reserved.
 * @license   Proprietary and confidential. Unauthorized use, reproduction,
 *            or distribution is strictly prohibited.
 *
 * @file    graphics.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2026-06-17 (date of creation)
 * @updated 2026-06-17 (date of last update)
 * @ingroup dasae-headers-workspace(dh-workspace)/dansi
 * @prefix  dansi_graphics
 *
 * @brief   xterm graphics attribute controls (XTSMGRAPHICS).
 */
#pragma once
#ifndef dansi_graphics__included
#define dansi_graphics__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include <dh/io/common.h>

/*========== Macros and Declarations ========================================*/

typedef enum_((dansi_graphics_Item $fits($packed))(
    dansi_graphics_Item_color_registers = 1,
    dansi_graphics_Item_sixel_geometry = 2,
    dansi_graphics_Item_regis_geometry = 3
)) dansi_graphics_Item;

typedef A$$(32, u8) dansi_graphics_Buf;

#define dansi_graphics_read_static(_item_tok) \
    ____dansi_graphics_read_static(_item_tok)
$extern fn_((dansi_graphics_read(dansi_graphics_Item item, dansi_graphics_Buf* buf))(S$u8));
$attr($must_check)
$extern fn_((dansi_graphics_readWrite(dansi_graphics_Item item, io_Writer writer))(E$void));

#define dansi_graphics_reset_static(_item_tok) \
    ____dansi_graphics_reset_static(_item_tok)
$extern fn_((dansi_graphics_reset(dansi_graphics_Item item, dansi_graphics_Buf* buf))(S$u8));
$attr($must_check)
$extern fn_((dansi_graphics_resetWrite(dansi_graphics_Item item, io_Writer writer))(E$void));

#define dansi_graphics_readMax_static(_item_tok) \
    ____dansi_graphics_readMax_static(_item_tok)
$extern fn_((dansi_graphics_readMax(dansi_graphics_Item item, dansi_graphics_Buf* buf))(S$u8));
$attr($must_check)
$extern fn_((dansi_graphics_readMaxWrite(dansi_graphics_Item item, io_Writer writer))(E$void));

#define dansi_graphics_setColorRegisters_static(_count_tok) \
    ____dansi_graphics_setColorRegisters_static(_count_tok)
$extern fn_((dansi_graphics_setColorRegisters(u16 count, dansi_graphics_Buf* buf))(S$u8));
$attr($must_check)
$extern fn_((dansi_graphics_setColorRegistersWrite(u16 count, io_Writer writer))(E$void));

#define dansi_graphics_setGeometry_static(_item_tok, _width_tok, _height_tok) \
    ____dansi_graphics_setGeometry_static(_item_tok, _width_tok, _height_tok)
$extern fn_((dansi_graphics_setGeometry(dansi_graphics_Item item, u16 width, u16 height, dansi_graphics_Buf* buf))(S$u8));
$attr($must_check)
$extern fn_((dansi_graphics_setGeometryWrite(dansi_graphics_Item item, u16 width, u16 height, io_Writer writer))(E$void));

/*========== Macros and Definitions =========================================*/

#include "utils.h"

#define ____dansi_graphics_read_static(_item_tok) \
    dansi_utils_csi "?" _item_tok ";1;0" dansi_utils_graphics_attrs
#define ____dansi_graphics_reset_static(_item_tok) \
    dansi_utils_csi "?" _item_tok ";2;0" dansi_utils_graphics_attrs
#define ____dansi_graphics_readMax_static(_item_tok) \
    dansi_utils_csi "?" _item_tok ";4;0" dansi_utils_graphics_attrs
#define ____dansi_graphics_setColorRegisters_static(_count_tok) \
    dansi_utils_csi "?1;3;" _count_tok dansi_utils_graphics_attrs
#define ____dansi_graphics_setGeometry_static(_item_tok, _width_tok, _height_tok) \
    dansi_utils_csi "?" _item_tok ";3;" _width_tok dansi_utils_sep _height_tok dansi_utils_graphics_attrs

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* dansi_graphics__included */

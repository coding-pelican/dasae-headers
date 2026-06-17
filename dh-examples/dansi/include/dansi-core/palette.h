/**
 * @copyright Copyright (c) 2026 Gyeongtae Kim. All rights reserved.
 * @license   Proprietary and confidential. Unauthorized use, reproduction,
 *            or distribution is strictly prohibited.
 *
 * @file    palette.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2026-06-17 (date of creation)
 * @updated 2026-06-17 (date of last update)
 * @ingroup dasae-headers-workspace(dh-workspace)/dansi
 * @prefix  dansi_palette
 *
 * @brief   xterm palette stack controls.
 */
#pragma once
#ifndef dansi_palette__included
#define dansi_palette__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include <dh/io/common.h>

/*========== Macros and Declarations ========================================*/

typedef A$$(16, u8) dansi_palette_StackBuf;

#define dansi_palette_push_static() \
    ____dansi_palette_push_static()
#define dansi_palette_pushAt_static(_index_tok) \
    ____dansi_palette_pushAt_static(_index_tok)
$extern fn_((dansi_palette_push(u8 index, dansi_palette_StackBuf* buf))(S$u8));
$attr($must_check)
$extern fn_((dansi_palette_pushWrite(u8 index, io_Writer writer))(E$void));

#define dansi_palette_pop_static() \
    ____dansi_palette_pop_static()
#define dansi_palette_popAt_static(_index_tok) \
    ____dansi_palette_popAt_static(_index_tok)
$extern fn_((dansi_palette_pop(u8 index, dansi_palette_StackBuf* buf))(S$u8));
$attr($must_check)
$extern fn_((dansi_palette_popWrite(u8 index, io_Writer writer))(E$void));

#define dansi_palette_reportStack_static() \
    ____dansi_palette_reportStack_static()
#define dansi_palette_reportStackAt_static(_index_tok) \
    ____dansi_palette_reportStackAt_static(_index_tok)
$extern fn_((dansi_palette_reportStack(u8 index, dansi_palette_StackBuf* buf))(S$u8));
$attr($must_check)
$extern fn_((dansi_palette_reportStackWrite(u8 index, io_Writer writer))(E$void));

/*========== Macros and Definitions =========================================*/

#include "utils.h"

#define ____dansi_palette_push_static() \
    dansi_utils_csi dansi_utils_palette_stack_push
#define ____dansi_palette_pushAt_static(_index_tok) \
    dansi_utils_csi _index_tok dansi_utils_palette_stack_push
#define ____dansi_palette_pop_static() \
    dansi_utils_csi dansi_utils_palette_stack_pop
#define ____dansi_palette_popAt_static(_index_tok) \
    dansi_utils_csi _index_tok dansi_utils_palette_stack_pop
#define ____dansi_palette_reportStack_static() \
    dansi_utils_csi dansi_utils_palette_stack_report
#define ____dansi_palette_reportStackAt_static(_index_tok) \
    dansi_utils_csi _index_tok dansi_utils_palette_stack_report

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* dansi_palette__included */

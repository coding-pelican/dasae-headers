/**
 * @copyright Copyright (c) 2026 Gyeongtae Kim. All rights reserved.
 * @license   Proprietary and confidential. Unauthorized use, reproduction,
 *            or distribution is strictly prohibited.
 *
 * @file    scroll.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2026-05-23 (date of creation)
 * @updated 2026-05-23 (date of last update)
 * @ingroup dasae-headers-workspace(dh-workspace)/dansi
 * @prefix  dansi_scroll
 */
#ifndef dansi_scroll__included
#define dansi_scroll__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

#include <dh/io/common.h>

typedef A$$(4 + uint_log10Ceil_static(u16_limit_max) * 2, u8) dansi_scroll_RegionBuf;

#define dansi_scroll_setRegion_static(_top_tok, _bottom_tok) \
    ____dansi_scroll_setRegion_static(_top_tok, _bottom_tok)
$extern fn_((dansi_scroll_setRegion(u16 top, u16 bottom, dansi_scroll_RegionBuf* buf))(S$u8));
$attr($must_check)
$extern fn_((dansi_scroll_setRegionWrite(u16 top, u16 bottom, io_Writer writer))(E$void));

#define dansi_scroll_resetRegion_static() \
    ____dansi_scroll_resetRegion_static()
$extern fn_((dansi_scroll_resetRegion(void))(S_const$u8));
$attr($must_check)
$extern fn_((dansi_scroll_resetRegionWrite(io_Writer writer))(E$void));

/*========== Macros and Definitions =========================================*/

#include "dansi-core/utils.h"

#define ____dansi_scroll_setRegion_static(_top_tok, _bottom_tok) \
    dansi_utils_formatCSI_u16x2_static(dansi_utils_scroll_set_region, _top_tok, _bottom_tok)
#define ____dansi_scroll_resetRegion_static() \
    dansi_utils_formatCSI_static(dansi_utils_scroll_reset_region)

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* dansi_scroll__included */

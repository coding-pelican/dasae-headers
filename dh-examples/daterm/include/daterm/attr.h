/**
 * @copyright Copyright (c) 2026 Gyeongtae Kim. All rights reserved.
 * @license   Proprietary and confidential. Unauthorized use, reproduction,
 *            or distribution is strictly prohibited.
 *
 * @file    attr.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2026-01-08 (date of creation)
 * @updated 2026-01-18 (date of last update)
 * @ingroup dasae-headers-workspace(dh-workspace)/daterm
 * @prefix  daterm_attr
 *
 * @brief   Terminal attribute reset
 * @details Provides functions to reset all colors and styles.
 */
#ifndef daterm_attr__included
#define daterm_attr__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "color.h"
#include "style.h"

/*========== Macros and Declarations ========================================*/

#define daterm_attr_reset_static() \
    ____daterm_attr_reset_static()
/// Get ANSI sequence to reset all colors and styles
/// Returns constant string
$extern fn_((daterm_attr_reset(void))(S_const$u8));
/// Write ANSI sequence to reset all colors and styles
$attr($must_check)
$extern fn_((daterm_attr_resetWrite(io_Writer writer))(E$void));

/*========== Macros and Definitions =========================================*/

#include "daterm/utils.h"

#define ____daterm_attr_reset_static() \
    daterm_utils_formatCSI_static(daterm_utils_attr_reset)

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* daterm_attr__included */

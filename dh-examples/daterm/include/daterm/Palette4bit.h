/**
 * @copyright Copyright (c) 2026 Gyeongtae Kim. All rights reserved.
 * @license   Proprietary and confidential. Unauthorized use, reproduction,
 *            or distribution is strictly prohibited.
 *
 * @file    Palette4bit.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2026-01-08 (date of creation)
 * @updated 2026-01-18 (date of last update)
 * @ingroup dasae-headers-workspace(dh-workspace)/daterm
 * @prefix  daterm_Palette4bit
 *
 * @brief   16-color palette for terminal
 * @details This enum defines the 16-color palette for terminal.
 *
 * @see Reference: https://www.ditig.com/256-colors-cheat-sheet
 */
#ifndef daterm_Palette4bit__included
#define daterm_Palette4bit__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include <dh/prl.h>

/*========== Macros and Declarations ========================================*/

typedef enum_((daterm_Palette4bit $fits($packed))(
    daterm_Palette4bit_black = 0,
    daterm_Palette4bit_maroon = 1,
    daterm_Palette4bit_green = 2,
    daterm_Palette4bit_olive = 3,
    daterm_Palette4bit_navy = 4,
    daterm_Palette4bit_purple = 5,
    daterm_Palette4bit_teal = 6,
    daterm_Palette4bit_silver = 7,
    daterm_Palette4bit_grey = 8,
    daterm_Palette4bit_red = 9,
    daterm_Palette4bit_lime = 10,
    daterm_Palette4bit_yellow = 11,
    daterm_Palette4bit_blue = 12,
    daterm_Palette4bit_fuchsia = 13,
    daterm_Palette4bit_aqua = 14,
    daterm_Palette4bit_white = 15
)) daterm_Palette4bit;
claim_assert_static(eqlType$(daterm_Palette4bit, u8));

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* daterm_Palette4bit__included */

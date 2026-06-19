/**
 * @copyright Copyright (c) 2026 Gyeongtae Kim. All rights reserved.
 * @license   Proprietary and confidential. Unauthorized use, reproduction,
 *            or distribution is strictly prohibited.
 *
 * @file    Palette4bit.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2026-06-19 (date of creation)
 * @updated 2026-06-19 (date of last update)
 * @ingroup dasae-headers-workspace(dh-workspace)/dansi
 * @prefix  dansi_xterm_Palette4bit
 */
#pragma once
#ifndef dansi_xterm_Palette4bit__included
#define dansi_xterm_Palette4bit__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include <dh/prl.h>

/*========== Macros and Declarations ========================================*/

typedef enum_((dansi_xterm_Palette4bit $fits($packed))(
    dansi_xterm_Palette4bit_black = 0,
    dansi_xterm_Palette4bit_maroon = 1,
    dansi_xterm_Palette4bit_green = 2,
    dansi_xterm_Palette4bit_olive = 3,
    dansi_xterm_Palette4bit_navy = 4,
    dansi_xterm_Palette4bit_purple = 5,
    dansi_xterm_Palette4bit_teal = 6,
    dansi_xterm_Palette4bit_silver = 7,
    dansi_xterm_Palette4bit_grey = 8,
    dansi_xterm_Palette4bit_red = 9,
    dansi_xterm_Palette4bit_lime = 10,
    dansi_xterm_Palette4bit_yellow = 11,
    dansi_xterm_Palette4bit_blue = 12,
    dansi_xterm_Palette4bit_fuchsia = 13,
    dansi_xterm_Palette4bit_aqua = 14,
    dansi_xterm_Palette4bit_white = 15
)) dansi_xterm_Palette4bit;
claim_assert_static(eqlType$(dansi_xterm_Palette4bit, u8));
T_use_prl$(dansi_xterm_Palette4bit);

#define dansi_xterm_Palette4bit_staticParse(_color_tok) \
    ____dansi_xterm_Palette4bit_staticParse(_color_tok)

/*========== Macros and Definitions =========================================*/

#define ____dansi_xterm_Palette4bit_staticParse(_color_tok) \
    pp_Tok_if_(pp_Tok_hasCmp(_color_tok))( \
        pp_Tok_then_(#_color_tok), \
        pp_Tok_else_(pp_join($, ____dansi_xterm_Palette4bit_str, _color_tok)) \
    )
#define ____dansi_xterm_Palette4bit_str$dansi_xterm_Palette4bit_black "0"
#define ____dansi_xterm_Palette4bit_str$dansi_xterm_Palette4bit_maroon "1"
#define ____dansi_xterm_Palette4bit_str$dansi_xterm_Palette4bit_green "2"
#define ____dansi_xterm_Palette4bit_str$dansi_xterm_Palette4bit_olive "3"
#define ____dansi_xterm_Palette4bit_str$dansi_xterm_Palette4bit_navy "4"
#define ____dansi_xterm_Palette4bit_str$dansi_xterm_Palette4bit_purple "5"
#define ____dansi_xterm_Palette4bit_str$dansi_xterm_Palette4bit_teal "6"
#define ____dansi_xterm_Palette4bit_str$dansi_xterm_Palette4bit_silver "7"
#define ____dansi_xterm_Palette4bit_str$dansi_xterm_Palette4bit_grey "8"
#define ____dansi_xterm_Palette4bit_str$dansi_xterm_Palette4bit_red "9"
#define ____dansi_xterm_Palette4bit_str$dansi_xterm_Palette4bit_lime "10"
#define ____dansi_xterm_Palette4bit_str$dansi_xterm_Palette4bit_yellow "11"
#define ____dansi_xterm_Palette4bit_str$dansi_xterm_Palette4bit_blue "12"
#define ____dansi_xterm_Palette4bit_str$dansi_xterm_Palette4bit_fuchsia "13"
#define ____dansi_xterm_Palette4bit_str$dansi_xterm_Palette4bit_aqua "14"
#define ____dansi_xterm_Palette4bit_str$dansi_xterm_Palette4bit_white "15"

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* dansi_xterm_Palette4bit__included */

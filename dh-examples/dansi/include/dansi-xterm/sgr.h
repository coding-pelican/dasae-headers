/**
 * @copyright Copyright (c) 2026 Gyeongtae Kim. All rights reserved.
 * @license   Proprietary and confidential. Unauthorized use, reproduction,
 *            or distribution is strictly prohibited.
 *
 * @file    sgr.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2026-06-19 (date of creation)
 * @updated 2026-06-19 (date of last update)
 * @ingroup dasae-headers-workspace(dh-workspace)/dansi
 * @prefix  dansi_xterm_sgr
 */
#pragma once
#ifndef dansi_xterm_sgr__included
#define dansi_xterm_sgr__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "dansi-core/sgr.h"

/*========== Macros and Declarations ========================================*/

#define dansi_xterm_sgr_stack_intermediate __str__dansi_xterm_sgr_stack_intermediate
#define dansi_xterm_sgr_stack_intermediate_byte __uint__dansi_xterm_sgr_stack_intermediate_byte
#define dansi_xterm_sgr_push_final __str__dansi_xterm_sgr_push_final
#define dansi_xterm_sgr_push_final_byte __uint__dansi_xterm_sgr_push_final_byte
#define dansi_xterm_sgr_pop_final __str__dansi_xterm_sgr_pop_final
#define dansi_xterm_sgr_pop_final_byte __uint__dansi_xterm_sgr_pop_final_byte

typedef enum_((dansi_xterm_sgr_Code $fits($packed))(
    dansi_xterm_sgr_Code_fg_bright_black = 90,
    dansi_xterm_sgr_Code_fg_bright_red = 91,
    dansi_xterm_sgr_Code_fg_bright_green = 92,
    dansi_xterm_sgr_Code_fg_bright_yellow = 93,
    dansi_xterm_sgr_Code_fg_bright_blue = 94,
    dansi_xterm_sgr_Code_fg_bright_magenta = 95,
    dansi_xterm_sgr_Code_fg_bright_cyan = 96,
    dansi_xterm_sgr_Code_fg_bright_white = 97,
    dansi_xterm_sgr_Code_bg_bright_black = 100,
    dansi_xterm_sgr_Code_bg_bright_red = 101,
    dansi_xterm_sgr_Code_bg_bright_green = 102,
    dansi_xterm_sgr_Code_bg_bright_yellow = 103,
    dansi_xterm_sgr_Code_bg_bright_blue = 104,
    dansi_xterm_sgr_Code_bg_bright_magenta = 105,
    dansi_xterm_sgr_Code_bg_bright_cyan = 106,
    dansi_xterm_sgr_Code_bg_bright_white = 107
)) dansi_xterm_sgr_Code;
claim_assert_static(eqlType$(dansi_xterm_sgr_Code, u8));
T_use_prl$(dansi_xterm_sgr_Code);

#define dansi_xterm_sgr_Code_staticParse(_code_tok) \
    ____dansi_xterm_sgr_Code_staticParse(_code_tok)

#define dansi_xterm_sgr_push_static() \
    ____dansi_xterm_sgr_push_static()
typedef A$$(5, u8) dansi_xterm_sgr_PushBuf;
$attr($must_check)
$extern fn_((dansi_xterm_sgr_push(dansi_xterm_sgr_PushBuf* buf))(S$u8));
$attr($must_check)
$extern fn_((dansi_xterm_sgr_pushWrite(io_Writer out))(E$void));

#define dansi_xterm_sgr_pop_static() \
    ____dansi_xterm_sgr_pop_static()
typedef A$$(5, u8) dansi_xterm_sgr_PopBuf;
$attr($must_check)
$extern fn_((dansi_xterm_sgr_pop(dansi_xterm_sgr_PopBuf* buf))(S$u8));
$attr($must_check)
$extern fn_((dansi_xterm_sgr_popWrite(io_Writer out))(E$void));

/*========== Macros and Definitions =========================================*/

#define __str__dansi_xterm_sgr_stack_intermediate "#"
#define __uint__dansi_xterm_sgr_stack_intermediate_byte u8_c('#')
#define __str__dansi_xterm_sgr_push_final "p"
#define __uint__dansi_xterm_sgr_push_final_byte u8_c('p')
#define __str__dansi_xterm_sgr_pop_final "q"
#define __uint__dansi_xterm_sgr_pop_final_byte u8_c('q')
#define ____dansi_xterm_sgr_push_static() \
    dansi_csi_make_static("", dansi_xterm_sgr_stack_intermediate, dansi_xterm_sgr_push_final)
#define ____dansi_xterm_sgr_pop_static() \
    dansi_csi_make_static("", dansi_xterm_sgr_stack_intermediate, dansi_xterm_sgr_pop_final)
#define ____dansi_xterm_sgr_Code_staticParse(_code_tok) \
    pp_join($, ____dansi_xterm_sgr_Code_str, _code_tok)
#define ____dansi_xterm_sgr_Code_str$dansi_xterm_sgr_Code_fg_bright_black "90"
#define ____dansi_xterm_sgr_Code_str$dansi_xterm_sgr_Code_fg_bright_red "91"
#define ____dansi_xterm_sgr_Code_str$dansi_xterm_sgr_Code_fg_bright_green "92"
#define ____dansi_xterm_sgr_Code_str$dansi_xterm_sgr_Code_fg_bright_yellow "93"
#define ____dansi_xterm_sgr_Code_str$dansi_xterm_sgr_Code_fg_bright_blue "94"
#define ____dansi_xterm_sgr_Code_str$dansi_xterm_sgr_Code_fg_bright_magenta "95"
#define ____dansi_xterm_sgr_Code_str$dansi_xterm_sgr_Code_fg_bright_cyan "96"
#define ____dansi_xterm_sgr_Code_str$dansi_xterm_sgr_Code_fg_bright_white "97"
#define ____dansi_xterm_sgr_Code_str$dansi_xterm_sgr_Code_bg_bright_black "100"
#define ____dansi_xterm_sgr_Code_str$dansi_xterm_sgr_Code_bg_bright_red "101"
#define ____dansi_xterm_sgr_Code_str$dansi_xterm_sgr_Code_bg_bright_green "102"
#define ____dansi_xterm_sgr_Code_str$dansi_xterm_sgr_Code_bg_bright_yellow "103"
#define ____dansi_xterm_sgr_Code_str$dansi_xterm_sgr_Code_bg_bright_blue "104"
#define ____dansi_xterm_sgr_Code_str$dansi_xterm_sgr_Code_bg_bright_magenta "105"
#define ____dansi_xterm_sgr_Code_str$dansi_xterm_sgr_Code_bg_bright_cyan "106"
#define ____dansi_xterm_sgr_Code_str$dansi_xterm_sgr_Code_bg_bright_white "107"

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* dansi_xterm_sgr__included */

/**
 * @copyright Copyright (c) 2026 Gyeongtae Kim. All rights reserved.
 * @license   Proprietary and confidential. Unauthorized use, reproduction,
 *            or distribution is strictly prohibited.
 *
 * @file    paste.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2026-06-19 (date of creation)
 * @updated 2026-06-19 (date of last update)
 * @ingroup dasae-headers-workspace(dh-workspace)/dansi
 * @prefix  dansi_xterm_paste
 */
#pragma once
#ifndef dansi_xterm_paste__included
#define dansi_xterm_paste__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "mode.h"

/*========== Macros and Declarations ========================================*/

#define dansi_xterm_paste_boundary_final __str__dansi_xterm_paste_boundary_final
#define dansi_xterm_paste_boundary_final_byte __uint__dansi_xterm_paste_boundary_final_byte
#define dansi_xterm_paste_boundary_code_param __uint__dansi_xterm_paste_boundary_code_param

typedef enum_((dansi_xterm_paste_Event $fits($packed))(
    dansi_xterm_paste_Event_begin = 200,
    dansi_xterm_paste_Event_end = 201
)) dansi_xterm_paste_Event;
claim_assert_static(eqlType$(dansi_xterm_paste_Event, u8));
T_use_prl$(dansi_xterm_paste_Event);

#define dansi_xterm_paste_setBracketed_static(_pp_enabled) \
    ____dansi_xterm_paste_setBracketed_static(_pp_enabled)
typedef dansi_xterm_mode_SetBuf dansi_xterm_paste_SetBracketedBuf;
$attr($must_check)
$extern fn_((dansi_xterm_paste_setBracketed(bool enabled, dansi_xterm_paste_SetBracketedBuf* buf))(S$u8));
$attr($must_check)
$extern fn_((dansi_xterm_paste_setBracketedWrite(bool enabled, io_Writer out))(io_PrintE$void));

#define dansi_xterm_paste_enableBracketed_static() \
    ____dansi_xterm_paste_enableBracketed_static()
typedef dansi_xterm_mode_EnableBuf dansi_xterm_paste_EnableBracketedBuf;
$attr($must_check)
$extern fn_((dansi_xterm_paste_enableBracketed(dansi_xterm_paste_EnableBracketedBuf* buf))(S$u8));
$attr($must_check)
$extern fn_((dansi_xterm_paste_enableBracketedWrite(io_Writer out))(io_PrintE$void));

#define dansi_xterm_paste_disableBracketed_static() \
    ____dansi_xterm_paste_disableBracketed_static()
typedef dansi_xterm_mode_DisableBuf dansi_xterm_paste_DisableBracketedBuf;
$attr($must_check)
$extern fn_((dansi_xterm_paste_disableBracketed(dansi_xterm_paste_DisableBracketedBuf* buf))(S$u8));
$attr($must_check)
$extern fn_((dansi_xterm_paste_disableBracketedWrite(io_Writer out))(io_PrintE$void));
$extern fn_((dansi_xterm_paste_parseBoundary(S_const$u8 report))(O$dansi_xterm_paste_Event));

/*========== Macros and Definitions =========================================*/

#define __str__dansi_xterm_paste_boundary_final "~"
#define __uint__dansi_xterm_paste_boundary_final_byte u8_c('~')
#define __uint__dansi_xterm_paste_boundary_code_param 0
#define ____dansi_xterm_paste_enableBracketed_static() \
    dansi_xterm_mode_enable_static(dansi_xterm_mode_Code_bracketed_paste)
#define ____dansi_xterm_paste_disableBracketed_static() \
    dansi_xterm_mode_disable_static(dansi_xterm_mode_Code_bracketed_paste)
#define ____dansi_xterm_paste_setBracketed_static(_pp_enabled) \
    dansi_xterm_mode_set_static(dansi_xterm_mode_Code_bracketed_paste, _pp_enabled)

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* dansi_xterm_paste__included */

/**
 * @copyright Copyright (c) 2026 Gyeongtae Kim. All rights reserved.
 * @license   Proprietary and confidential. Unauthorized use, reproduction,
 *            or distribution is strictly prohibited.
 *
 * @file    mode.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2026-06-18 (date of creation)
 * @updated 2026-06-18 (date of last update)
 * @ingroup dasae-headers-workspace(dh-workspace)/dansi
 * @prefix  dansi_dec_mode
 */
#pragma once
#ifndef dansi_dec_mode__included
#define dansi_dec_mode__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "dansi-core/csi.h"

/*========== Macros and Declarations ========================================*/

typedef enum_((dansi_dec_mode_Code $fits($packed))(
    dansi_dec_mode_Code_cursor_keys = 1,
    dansi_dec_mode_Code_ansi = 2,
    dansi_dec_mode_Code_132_columns = 3,
    dansi_dec_mode_Code_smooth_scroll = 4,
    dansi_dec_mode_Code_reverse_video = 5,
    dansi_dec_mode_Code_origin = 6,
    dansi_dec_mode_Code_auto_wrap = 7,
    dansi_dec_mode_Code_auto_repeat = 8,
    dansi_dec_mode_Code_interlace = 9,
    dansi_dec_mode_Code_print_form_feed = 18,
    dansi_dec_mode_Code_print_extent = 19,
    dansi_dec_mode_Code_text_cursor = 25,
    dansi_dec_mode_Code_numeric_keypad = 66,
    dansi_dec_mode_Code_left_right_margin = 69
)) dansi_dec_mode_Code;
claim_assert_static(eqlType$(dansi_dec_mode_Code, u8));
T_use_prl$(dansi_dec_mode_Code);

#define dansi_dec_mode_Code_staticParse(_mode_tok) \
    ____dansi_dec_mode_Code_staticParse(_mode_tok)

typedef enum_((dansi_dec_mode_Status $fits($packed))(
    dansi_dec_mode_Status_not_recognized = 0,
    dansi_dec_mode_Status_set = 1,
    dansi_dec_mode_Status_reset = 2,
    dansi_dec_mode_Status_permanently_set = 3,
    dansi_dec_mode_Status_permanently_reset = 4
)) dansi_dec_mode_Status;
claim_assert_static(eqlType$(dansi_dec_mode_Status, u8));
T_use_prl$(dansi_dec_mode_Status);

typedef struct dansi_dec_mode_Report {
    var_(mode, u16);
    var_(status, dansi_dec_mode_Status);
} dansi_dec_mode_Report;
T_use_prl$(dansi_dec_mode_Report);

errset_((dansi_dec_mode_E)(dansi_dec_mode_InvalidResponse));
T_use_E$($set(dansi_dec_mode_E)(dansi_dec_mode_Report));

#define dansi_dec_mode_set_static(_mode_tok, _pp_enabled) \
    ____dansi_dec_mode_set_static(_mode_tok, _pp_enabled)
typedef A$$(4 + uint_log10Ceil_static(u16_limit_max), u8) dansi_dec_mode_SetBuf;
$attr($must_check)
$extern fn_((dansi_dec_mode_set(dansi_dec_mode_Code mode, bool enabled, dansi_dec_mode_SetBuf* buf))(S$u8));
$attr($must_check)
$extern fn_((dansi_dec_mode_setWrite(dansi_dec_mode_Code mode, bool enabled, io_Writer out))(E$void));

#define dansi_dec_mode_enable_static(_mode_tok) \
    ____dansi_dec_mode_enable_static(_mode_tok)
typedef dansi_dec_mode_SetBuf dansi_dec_mode_EnableBuf;
$attr($must_check)
$extern fn_((dansi_dec_mode_enable(dansi_dec_mode_Code mode, dansi_dec_mode_EnableBuf* buf))(S$u8));
$attr($must_check)
$extern fn_((dansi_dec_mode_enableWrite(dansi_dec_mode_Code mode, io_Writer out))(E$void));

#define dansi_dec_mode_disable_static(_mode_tok) \
    ____dansi_dec_mode_disable_static(_mode_tok)
typedef dansi_dec_mode_SetBuf dansi_dec_mode_DisableBuf;
$attr($must_check)
$extern fn_((dansi_dec_mode_disable(dansi_dec_mode_Code mode, dansi_dec_mode_DisableBuf* buf))(S$u8));
$attr($must_check)
$extern fn_((dansi_dec_mode_disableWrite(dansi_dec_mode_Code mode, io_Writer out))(E$void));

#define dansi_dec_mode_save_static(_mode_tok) \
    ____dansi_dec_mode_save_static(_mode_tok)
typedef dansi_dec_mode_SetBuf dansi_dec_mode_SaveBuf;
$attr($must_check)
$extern fn_((dansi_dec_mode_save(dansi_dec_mode_Code mode, dansi_dec_mode_SaveBuf* buf))(S$u8));
$attr($must_check)
$extern fn_((dansi_dec_mode_saveWrite(dansi_dec_mode_Code mode, io_Writer out))(E$void));

#define dansi_dec_mode_restore_static(_mode_tok) \
    ____dansi_dec_mode_restore_static(_mode_tok)
typedef dansi_dec_mode_SetBuf dansi_dec_mode_RestoreBuf;
$attr($must_check)
$extern fn_((dansi_dec_mode_restore(dansi_dec_mode_Code mode, dansi_dec_mode_RestoreBuf* buf))(S$u8));
$attr($must_check)
$extern fn_((dansi_dec_mode_restoreWrite(dansi_dec_mode_Code mode, io_Writer out))(E$void));

#define dansi_dec_mode_request_static(_mode_tok) \
    ____dansi_dec_mode_request_static(_mode_tok)
typedef A$$(5 + uint_log10Ceil_static(u16_limit_max), u8) dansi_dec_mode_RequestBuf;
$attr($must_check)
$extern fn_((dansi_dec_mode_request(dansi_dec_mode_Code mode, dansi_dec_mode_RequestBuf* buf))(S$u8));
$attr($must_check)
$extern fn_((dansi_dec_mode_requestWrite(dansi_dec_mode_Code mode, io_Writer out))(E$void));
$attr($must_check)
$extern fn_((dansi_dec_mode_receiveReport(io_Reader in, S$u8 buf))(E$S$u8));
$attr($must_check)
$extern fn_((dansi_dec_mode_parseReport(S_const$u8 report))(dansi_dec_mode_E$dansi_dec_mode_Report));
$attr($must_check)
$extern fn_((dansi_dec_mode_fetch(
    io_Writer out, io_Reader in, dansi_dec_mode_Code mode, S$u8 buf
))(dansi_dec_mode_E$dansi_dec_mode_Report));

/*========== Macros and Definitions =========================================*/

#define ____dansi_dec_mode_enable_static(_mode_tok) \
    dansi_csi_makePrivate1_static(dansi_dec_mode_Code_staticParse(_mode_tok), "h")
#define ____dansi_dec_mode_disable_static(_mode_tok) \
    dansi_csi_makePrivate1_static(dansi_dec_mode_Code_staticParse(_mode_tok), "l")
#define ____dansi_dec_mode_set_static(_mode_tok, _pp_enabled) \
    pp_if_(_pp_enabled)( \
        pp_then_(dansi_dec_mode_enable_static(_mode_tok)), \
        pp_else_(dansi_dec_mode_disable_static(_mode_tok)) \
    )
#define ____dansi_dec_mode_save_static(_mode_tok) \
    dansi_csi_makePrivate1_static(dansi_dec_mode_Code_staticParse(_mode_tok), "s")
#define ____dansi_dec_mode_restore_static(_mode_tok) \
    dansi_csi_makePrivate1_static(dansi_dec_mode_Code_staticParse(_mode_tok), "r")
#define ____dansi_dec_mode_request_static(_mode_tok) \
    dansi_csi_make_static("?" dansi_dec_mode_Code_staticParse(_mode_tok), "$", "p")
#define ____dansi_dec_mode_Code_staticParse(_mode_tok) \
    pp_join($, ____dansi_dec_mode_Code_str, _mode_tok)
#define ____dansi_dec_mode_Code_str$dansi_dec_mode_Code_cursor_keys "1"
#define ____dansi_dec_mode_Code_str$dansi_dec_mode_Code_ansi "2"
#define ____dansi_dec_mode_Code_str$dansi_dec_mode_Code_132_columns "3"
#define ____dansi_dec_mode_Code_str$dansi_dec_mode_Code_smooth_scroll "4"
#define ____dansi_dec_mode_Code_str$dansi_dec_mode_Code_reverse_video "5"
#define ____dansi_dec_mode_Code_str$dansi_dec_mode_Code_origin "6"
#define ____dansi_dec_mode_Code_str$dansi_dec_mode_Code_auto_wrap "7"
#define ____dansi_dec_mode_Code_str$dansi_dec_mode_Code_auto_repeat "8"
#define ____dansi_dec_mode_Code_str$dansi_dec_mode_Code_interlace "9"
#define ____dansi_dec_mode_Code_str$dansi_dec_mode_Code_print_form_feed "18"
#define ____dansi_dec_mode_Code_str$dansi_dec_mode_Code_print_extent "19"
#define ____dansi_dec_mode_Code_str$dansi_dec_mode_Code_text_cursor "25"
#define ____dansi_dec_mode_Code_str$dansi_dec_mode_Code_numeric_keypad "66"
#define ____dansi_dec_mode_Code_str$dansi_dec_mode_Code_left_right_margin "69"

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* dansi_dec_mode__included */

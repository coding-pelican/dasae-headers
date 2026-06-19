/**
 * @copyright Copyright (c) 2026 Gyeongtae Kim. All rights reserved.
 * @license   Proprietary and confidential. Unauthorized use, reproduction,
 *            or distribution is strictly prohibited.
 *
 * @file    resrc.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2026-06-19 (date of creation)
 * @updated 2026-06-19 (date of last update)
 * @ingroup dasae-headers-workspace(dh-workspace)/dansi
 * @prefix  dansi_xterm_resrc
 */
#pragma once
#ifndef dansi_xterm_resrc__included
#define dansi_xterm_resrc__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "dansi-core/dcs.h"
#include "dansi-core/osc.h"

/*========== Macros and Declarations ========================================*/

typedef enum_((dansi_xterm_resrc_Allowed $fits($packed))(
    dansi_xterm_resrc_Allowed_color_ops,
    dansi_xterm_resrc_Allowed_font_ops,
    dansi_xterm_resrc_Allowed_mouse_ops,
    dansi_xterm_resrc_Allowed_paste_controls,
    dansi_xterm_resrc_Allowed_tcap_ops,
    dansi_xterm_resrc_Allowed_title_ops,
    dansi_xterm_resrc_Allowed_window_ops
)) dansi_xterm_resrc_Allowed;
claim_assert_static(eqlType$(dansi_xterm_resrc_Allowed, u8));
T_use_prl$(dansi_xterm_resrc_Allowed);

typedef struct dansi_xterm_resrc_Response {
    var_(valid, bool);
    var_(payload, S_const$u8);
} dansi_xterm_resrc_Response;
T_use_prl$(dansi_xterm_resrc_Response);

errset_((dansi_xterm_resrc_E)(dansi_xterm_resrc_InvalidResponse));
T_use_E$($set(dansi_xterm_resrc_E)(dansi_xterm_resrc_Response));

#define dansi_xterm_resrc_requestXResRaw_static(_names_hex_tok) \
    ____dansi_xterm_resrc_requestXResRaw_static(_names_hex_tok)
$attr($must_check)
$extern fn_((dansi_xterm_resrc_requestXResRaw(S_const$u8 names_hex, S$u8 buf))(E$S$u8));
$attr($must_check)
$extern fn_((dansi_xterm_resrc_requestXResRawWrite(S_const$u8 names_hex, io_Writer out))(E$void));
$attr($must_check)
$extern fn_((dansi_xterm_resrc_parseXResResponse(
    dansi_dcs_Frame frame
))(dansi_xterm_resrc_E$dansi_xterm_resrc_Response));

#define dansi_xterm_resrc_setTermcapRaw_static(_name_hex_tok) \
    ____dansi_xterm_resrc_setTermcapRaw_static(_name_hex_tok)
$attr($must_check)
$extern fn_((dansi_xterm_resrc_setTermcapRaw(S_const$u8 name_hex, S$u8 buf))(E$S$u8));
$attr($must_check)
$extern fn_((dansi_xterm_resrc_setTermcapRawWrite(S_const$u8 name_hex, io_Writer out))(E$void));

#define dansi_xterm_resrc_requestTermcapRaw_static(_names_hex_tok) \
    ____dansi_xterm_resrc_requestTermcapRaw_static(_names_hex_tok)
$attr($must_check)
$extern fn_((dansi_xterm_resrc_requestTermcapRaw(S_const$u8 names_hex, S$u8 buf))(E$S$u8));
$attr($must_check)
$extern fn_((dansi_xterm_resrc_requestTermcapRawWrite(S_const$u8 names_hex, io_Writer out))(E$void));
$attr($must_check)
$extern fn_((dansi_xterm_resrc_parseTermcapResponse(
    dansi_dcs_Frame frame
))(dansi_xterm_resrc_E$dansi_xterm_resrc_Response));

#define dansi_xterm_resrc_queryAllowed_static() \
    ____dansi_xterm_resrc_queryAllowed_static()
$extern fn_((dansi_xterm_resrc_queryAllowed(void))(S_const$u8));
$attr($must_check)
$extern fn_((dansi_xterm_resrc_queryAllowedWrite(io_Writer out))(E$void));

#define dansi_xterm_resrc_queryAllowableRaw_static(_feature_tok) \
    ____dansi_xterm_resrc_queryAllowableRaw_static(_feature_tok)
$attr($must_check)
$extern fn_((dansi_xterm_resrc_queryAllowableRaw(S_const$u8 feature, S$u8 buf))(E$S$u8));
$attr($must_check)
$extern fn_((dansi_xterm_resrc_queryAllowableRawWrite(S_const$u8 feature, io_Writer out))(E$void));

/*========== Macros and Definitions =========================================*/

#define ____dansi_xterm_resrc_requestXResRaw_static(_names_hex_tok) \
    dansi_dcs_make_static("", "+", "Q", _names_hex_tok)
#define ____dansi_xterm_resrc_setTermcapRaw_static(_name_hex_tok) \
    dansi_dcs_make_static("", "+", "p", _name_hex_tok)
#define ____dansi_xterm_resrc_requestTermcapRaw_static(_names_hex_tok) \
    dansi_dcs_make_static("", "+", "q", _names_hex_tok)
#define ____dansi_xterm_resrc_queryAllowed_static() \
    dansi_osc_makeRaw_static("60;?")
#define ____dansi_xterm_resrc_queryAllowableRaw_static(_feature_tok) \
    dansi_osc_make_static("60", _feature_tok)

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* dansi_xterm_resrc__included */

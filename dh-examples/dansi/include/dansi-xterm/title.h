/**
 * @copyright Copyright (c) 2026 Gyeongtae Kim. All rights reserved.
 * @license   Proprietary and confidential. Unauthorized use, reproduction,
 *            or distribution is strictly prohibited.
 *
 * @file    title.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2026-06-19 (date of creation)
 * @updated 2026-06-19 (date of last update)
 * @ingroup dasae-headers-workspace(dh-workspace)/dansi
 * @prefix  dansi_xterm_title
 */
#pragma once
#ifndef dansi_xterm_title__included
#define dansi_xterm_title__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "dansi-core/csi.h"
#include "dansi-core/osc.h"

/*========== Macros and Declarations ========================================*/

typedef enum_((dansi_xterm_title_Target $fits($packed))(
    dansi_xterm_title_Target_icon = 1,
    dansi_xterm_title_Target_window = 2,
    dansi_xterm_title_Target_both = 3
)) dansi_xterm_title_Target;
claim_assert_static(eqlType$(dansi_xterm_title_Target, u8));
T_use_prl$(dansi_xterm_title_Target);

typedef struct dansi_xterm_title_Report {
    var_(target, dansi_xterm_title_Target);
    var_(text, S_const$u8);
} dansi_xterm_title_Report;
T_use_prl$(dansi_xterm_title_Report);

errset_((dansi_xterm_title_E)(dansi_xterm_title_InvalidResponse));
T_use_E$($set(dansi_xterm_title_E)(dansi_xterm_title_Report));

#define dansi_xterm_title_Target_staticParse(_target_tok) \
    ____dansi_xterm_title_Target_staticParse(_target_tok)

#define dansi_xterm_title_setIcon_static(_title_tok) \
    ____dansi_xterm_title_setIcon_static(_title_tok)
$attr($must_check)
$extern fn_((dansi_xterm_title_setIcon(S_const$u8 title, S$u8 buf))(E$S$u8));
$attr($must_check)
$extern fn_((dansi_xterm_title_setIconWrite(S_const$u8 title, io_Writer out))(E$void));

#define dansi_xterm_title_setWindow_static(_title_tok) \
    ____dansi_xterm_title_setWindow_static(_title_tok)
$attr($must_check)
$extern fn_((dansi_xterm_title_setWindow(S_const$u8 title, S$u8 buf))(E$S$u8));
$attr($must_check)
$extern fn_((dansi_xterm_title_setWindowWrite(S_const$u8 title, io_Writer out))(E$void));

#define dansi_xterm_title_setBoth_static(_title_tok) \
    ____dansi_xterm_title_setBoth_static(_title_tok)
$attr($must_check)
$extern fn_((dansi_xterm_title_setBoth(S_const$u8 title, S$u8 buf))(E$S$u8));
$attr($must_check)
$extern fn_((dansi_xterm_title_setBothWrite(S_const$u8 title, io_Writer out))(E$void));

#define dansi_xterm_title_push_static(_target_tok) \
    ____dansi_xterm_title_push_static(_target_tok)
typedef A$$(7 + uint_log10Ceil_static(u8_limit_max), u8) dansi_xterm_title_StackBuf;
$attr($must_check)
$extern fn_((dansi_xterm_title_push(dansi_xterm_title_Target target, dansi_xterm_title_StackBuf* buf))(S$u8));
$attr($must_check)
$extern fn_((dansi_xterm_title_pushWrite(dansi_xterm_title_Target target, io_Writer out))(E$void));

#define dansi_xterm_title_pop_static(_target_tok) \
    ____dansi_xterm_title_pop_static(_target_tok)
$attr($must_check)
$extern fn_((dansi_xterm_title_pop(dansi_xterm_title_Target target, dansi_xterm_title_StackBuf* buf))(S$u8));
$attr($must_check)
$extern fn_((dansi_xterm_title_popWrite(dansi_xterm_title_Target target, io_Writer out))(E$void));

#define dansi_xterm_title_requestIconLabel_static() \
    ____dansi_xterm_title_requestIconLabel_static()
$extern fn_((dansi_xterm_title_requestIconLabel(void))(S_const$u8));
$attr($must_check)
$extern fn_((dansi_xterm_title_requestIconLabelWrite(io_Writer out))(E$void));
$attr($must_check)
$extern fn_((dansi_xterm_title_receiveIconLabelReport(io_Reader in, S$u8 buf))(E$S$u8));
$attr($must_check)
$extern fn_((dansi_xterm_title_parseIconLabelReport(
    dansi_osc_Frame frame
))(dansi_xterm_title_E$dansi_xterm_title_Report));
$attr($must_check)
$extern fn_((dansi_xterm_title_fetchIconLabel(
    io_Writer out, io_Reader in, S$u8 buf
))(dansi_xterm_title_E$dansi_xterm_title_Report));

#define dansi_xterm_title_requestWindowTitle_static() \
    ____dansi_xterm_title_requestWindowTitle_static()
$extern fn_((dansi_xterm_title_requestWindowTitle(void))(S_const$u8));
$attr($must_check)
$extern fn_((dansi_xterm_title_requestWindowTitleWrite(io_Writer out))(E$void));
$attr($must_check)
$extern fn_((dansi_xterm_title_receiveWindowTitleReport(io_Reader in, S$u8 buf))(E$S$u8));
$attr($must_check)
$extern fn_((dansi_xterm_title_parseWindowTitleReport(
    dansi_osc_Frame frame
))(dansi_xterm_title_E$dansi_xterm_title_Report));
$attr($must_check)
$extern fn_((dansi_xterm_title_fetchWindowTitle(
    io_Writer out, io_Reader in, S$u8 buf
))(dansi_xterm_title_E$dansi_xterm_title_Report));

/*========== Macros and Definitions =========================================*/

#define ____dansi_xterm_title_setIcon_static(_title_tok) \
    dansi_osc_make_static("1", _title_tok)
#define ____dansi_xterm_title_setWindow_static(_title_tok) \
    dansi_osc_make_static("2", _title_tok)
#define ____dansi_xterm_title_setBoth_static(_title_tok) \
    dansi_osc_make_static("0", _title_tok)
#define ____dansi_xterm_title_push_static(_target_tok) \
    dansi_csi_make2_static("22", dansi_xterm_title_Target_staticParse(_target_tok), "t")
#define ____dansi_xterm_title_pop_static(_target_tok) \
    dansi_csi_make2_static("23", dansi_xterm_title_Target_staticParse(_target_tok), "t")
#define ____dansi_xterm_title_requestIconLabel_static() \
    dansi_csi_make1_static("20", "t")
#define ____dansi_xterm_title_requestWindowTitle_static() \
    dansi_csi_make1_static("21", "t")
#define ____dansi_xterm_title_Target_staticParse(_target_tok) \
    pp_join(____dansi_xterm_title_Target_str$, _target_tok)
#define ____dansi_xterm_title_Target_str$dansi_xterm_title_Target_icon "1"
#define ____dansi_xterm_title_Target_str$dansi_xterm_title_Target_window "2"
#define ____dansi_xterm_title_Target_str$dansi_xterm_title_Target_both "3"

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* dansi_xterm_title__included */

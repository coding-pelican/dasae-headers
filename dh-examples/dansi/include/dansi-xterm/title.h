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

#define dansi_xterm_title_cmd_both __str__dansi_xterm_title_cmd_both
#define dansi_xterm_title_cmd_both_u16 __uint__dansi_xterm_title_cmd_both_u16
#define dansi_xterm_title_cmd_icon __str__dansi_xterm_title_cmd_icon
#define dansi_xterm_title_cmd_icon_u16 __uint__dansi_xterm_title_cmd_icon_u16
#define dansi_xterm_title_cmd_window __str__dansi_xterm_title_cmd_window
#define dansi_xterm_title_cmd_window_u16 __uint__dansi_xterm_title_cmd_window_u16
#define dansi_xterm_title_stack_push_op __str__dansi_xterm_title_stack_push_op
#define dansi_xterm_title_stack_push_op_u16 __uint__dansi_xterm_title_stack_push_op_u16
#define dansi_xterm_title_stack_pop_op __str__dansi_xterm_title_stack_pop_op
#define dansi_xterm_title_stack_pop_op_u16 __uint__dansi_xterm_title_stack_pop_op_u16
#define dansi_xterm_title_request_icon_label_op __str__dansi_xterm_title_request_icon_label_op
#define dansi_xterm_title_request_icon_label_op_u16 __uint__dansi_xterm_title_request_icon_label_op_u16
#define dansi_xterm_title_request_window_title_op __str__dansi_xterm_title_request_window_title_op
#define dansi_xterm_title_request_window_title_op_u16 __uint__dansi_xterm_title_request_window_title_op_u16
#define dansi_xterm_title_report_final __str__dansi_xterm_title_report_final
#define dansi_xterm_title_report_final_byte __uint__dansi_xterm_title_report_final_byte
#define dansi_xterm_title_icon_label_marker __str__dansi_xterm_title_icon_label_marker
#define dansi_xterm_title_icon_label_marker_byte __uint__dansi_xterm_title_icon_label_marker_byte
#define dansi_xterm_title_window_title_marker __str__dansi_xterm_title_window_title_marker
#define dansi_xterm_title_window_title_marker_byte __uint__dansi_xterm_title_window_title_marker_byte
#define dansi_xterm_title_payload_marker_index __uint__dansi_xterm_title_payload_marker_index
#define dansi_xterm_title_payload_text_start __uint__dansi_xterm_title_payload_text_start

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
$extern fn_((dansi_xterm_title_setIconWrite(S_const$u8 title, io_Writer out))(io_PrintE$void));

#define dansi_xterm_title_setWindow_static(_title_tok) \
    ____dansi_xterm_title_setWindow_static(_title_tok)
$attr($must_check)
$extern fn_((dansi_xterm_title_setWindow(S_const$u8 title, S$u8 buf))(E$S$u8));
$attr($must_check)
$extern fn_((dansi_xterm_title_setWindowWrite(S_const$u8 title, io_Writer out))(io_PrintE$void));

#define dansi_xterm_title_setBoth_static(_title_tok) \
    ____dansi_xterm_title_setBoth_static(_title_tok)
$attr($must_check)
$extern fn_((dansi_xterm_title_setBoth(S_const$u8 title, S$u8 buf))(E$S$u8));
$attr($must_check)
$extern fn_((dansi_xterm_title_setBothWrite(S_const$u8 title, io_Writer out))(io_PrintE$void));

#define dansi_xterm_title_push_static(_target_tok) \
    ____dansi_xterm_title_push_static(_target_tok)
typedef A$$(7 + uint_log10Ceil_static(u8_limit_max), u8) dansi_xterm_title_StackBuf;
$attr($must_check)
$extern fn_((dansi_xterm_title_push(dansi_xterm_title_Target target, dansi_xterm_title_StackBuf* buf))(S$u8));
$attr($must_check)
$extern fn_((dansi_xterm_title_pushWrite(dansi_xterm_title_Target target, io_Writer out))(io_PrintE$void));

#define dansi_xterm_title_pop_static(_target_tok) \
    ____dansi_xterm_title_pop_static(_target_tok)
$attr($must_check)
$extern fn_((dansi_xterm_title_pop(dansi_xterm_title_Target target, dansi_xterm_title_StackBuf* buf))(S$u8));
$attr($must_check)
$extern fn_((dansi_xterm_title_popWrite(dansi_xterm_title_Target target, io_Writer out))(io_PrintE$void));

#define dansi_xterm_title_requestIconLabel_static() \
    ____dansi_xterm_title_requestIconLabel_static()
$extern fn_((dansi_xterm_title_requestIconLabel(void))(S_const$u8));
$attr($must_check)
$extern fn_((dansi_xterm_title_requestIconLabelWrite(io_Writer out))(io_WriteE$void));
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
$extern fn_((dansi_xterm_title_requestWindowTitleWrite(io_Writer out))(io_WriteE$void));
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

#define __str__dansi_xterm_title_cmd_both "0"
#define __uint__dansi_xterm_title_cmd_both_u16 0
#define __str__dansi_xterm_title_cmd_icon "1"
#define __uint__dansi_xterm_title_cmd_icon_u16 1
#define __str__dansi_xterm_title_cmd_window "2"
#define __uint__dansi_xterm_title_cmd_window_u16 2
#define __str__dansi_xterm_title_stack_push_op "22"
#define __uint__dansi_xterm_title_stack_push_op_u16 22
#define __str__dansi_xterm_title_stack_pop_op "23"
#define __uint__dansi_xterm_title_stack_pop_op_u16 23
#define __str__dansi_xterm_title_request_icon_label_op "20"
#define __uint__dansi_xterm_title_request_icon_label_op_u16 20
#define __str__dansi_xterm_title_request_window_title_op "21"
#define __uint__dansi_xterm_title_request_window_title_op_u16 21
#define __str__dansi_xterm_title_report_final "t"
#define __uint__dansi_xterm_title_report_final_byte u8_c('t')
#define __str__dansi_xterm_title_icon_label_marker "L"
#define __uint__dansi_xterm_title_icon_label_marker_byte u8_c('L')
#define __str__dansi_xterm_title_window_title_marker "l"
#define __uint__dansi_xterm_title_window_title_marker_byte u8_c('l')
#define __uint__dansi_xterm_title_payload_marker_index 0
#define __uint__dansi_xterm_title_payload_text_start 1
#define ____dansi_xterm_title_setIcon_static(_title_tok) \
    dansi_osc_make_static(dansi_xterm_title_cmd_icon, _title_tok)
#define ____dansi_xterm_title_setWindow_static(_title_tok) \
    dansi_osc_make_static(dansi_xterm_title_cmd_window, _title_tok)
#define ____dansi_xterm_title_setBoth_static(_title_tok) \
    dansi_osc_make_static(dansi_xterm_title_cmd_both, _title_tok)
#define ____dansi_xterm_title_push_static(_target_tok) \
    dansi_csi_make2_static(dansi_xterm_title_stack_push_op, dansi_xterm_title_Target_staticParse(_target_tok), dansi_xterm_title_report_final)
#define ____dansi_xterm_title_pop_static(_target_tok) \
    dansi_csi_make2_static(dansi_xterm_title_stack_pop_op, dansi_xterm_title_Target_staticParse(_target_tok), dansi_xterm_title_report_final)
#define ____dansi_xterm_title_requestIconLabel_static() \
    dansi_csi_make1_static(dansi_xterm_title_request_icon_label_op, dansi_xterm_title_report_final)
#define ____dansi_xterm_title_requestWindowTitle_static() \
    dansi_csi_make1_static(dansi_xterm_title_request_window_title_op, dansi_xterm_title_report_final)
#define ____dansi_xterm_title_Target_staticParse(_target_tok) \
    pp_join($, ____dansi_xterm_title_Target_str, _target_tok)
#define ____dansi_xterm_title_Target_str$dansi_xterm_title_Target_icon "1"
#define ____dansi_xterm_title_Target_str$dansi_xterm_title_Target_window "2"
#define ____dansi_xterm_title_Target_str$dansi_xterm_title_Target_both "3"

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* dansi_xterm_title__included */

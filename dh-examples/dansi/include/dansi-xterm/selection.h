/**
 * @copyright Copyright (c) 2026 Gyeongtae Kim. All rights reserved.
 * @license   Proprietary and confidential. Unauthorized use, reproduction,
 *            or distribution is strictly prohibited.
 *
 * @file    selection.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2026-06-19 (date of creation)
 * @updated 2026-06-19 (date of last update)
 * @ingroup dasae-headers-workspace(dh-workspace)/dansi
 * @prefix  dansi_xterm_selection
 */
#pragma once
#ifndef dansi_xterm_selection__included
#define dansi_xterm_selection__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "dansi-core/osc.h"

/*========== Macros and Declarations ========================================*/

#define dansi_xterm_selection_cmd __str__dansi_xterm_selection_cmd
#define dansi_xterm_selection_cmd_u16 __uint__dansi_xterm_selection_cmd_u16
#define dansi_xterm_selection_sep __str__dansi_xterm_selection_sep
#define dansi_xterm_selection_sep_byte __uint__dansi_xterm_selection_sep_byte
#define dansi_xterm_selection_query_marker __str__dansi_xterm_selection_query_marker
#define dansi_xterm_selection_query_marker_byte __uint__dansi_xterm_selection_query_marker_byte
#define dansi_xterm_selection_target_count __uint__dansi_xterm_selection_target_count
#define dansi_xterm_selection_target_clipboard __str__dansi_xterm_selection_target_clipboard
#define dansi_xterm_selection_target_clipboard_byte __uint__dansi_xterm_selection_target_clipboard_byte
#define dansi_xterm_selection_target_primary __str__dansi_xterm_selection_target_primary
#define dansi_xterm_selection_target_primary_byte __uint__dansi_xterm_selection_target_primary_byte
#define dansi_xterm_selection_target_secondary __str__dansi_xterm_selection_target_secondary
#define dansi_xterm_selection_target_secondary_byte __uint__dansi_xterm_selection_target_secondary_byte
#define dansi_xterm_selection_target_select __str__dansi_xterm_selection_target_select
#define dansi_xterm_selection_target_select_byte __uint__dansi_xterm_selection_target_select_byte
#define dansi_xterm_selection_target_cut0 __str__dansi_xterm_selection_target_cut0
#define dansi_xterm_selection_target_cut0_byte __uint__dansi_xterm_selection_target_cut0_byte
#define dansi_xterm_selection_target_cut1 __str__dansi_xterm_selection_target_cut1
#define dansi_xterm_selection_target_cut1_byte __uint__dansi_xterm_selection_target_cut1_byte
#define dansi_xterm_selection_target_cut2 __str__dansi_xterm_selection_target_cut2
#define dansi_xterm_selection_target_cut2_byte __uint__dansi_xterm_selection_target_cut2_byte
#define dansi_xterm_selection_target_cut3 __str__dansi_xterm_selection_target_cut3
#define dansi_xterm_selection_target_cut3_byte __uint__dansi_xterm_selection_target_cut3_byte
#define dansi_xterm_selection_target_cut4 __str__dansi_xterm_selection_target_cut4
#define dansi_xterm_selection_target_cut4_byte __uint__dansi_xterm_selection_target_cut4_byte
#define dansi_xterm_selection_target_cut5 __str__dansi_xterm_selection_target_cut5
#define dansi_xterm_selection_target_cut5_byte __uint__dansi_xterm_selection_target_cut5_byte
#define dansi_xterm_selection_target_cut6 __str__dansi_xterm_selection_target_cut6
#define dansi_xterm_selection_target_cut6_byte __uint__dansi_xterm_selection_target_cut6_byte
#define dansi_xterm_selection_target_cut7 __str__dansi_xterm_selection_target_cut7
#define dansi_xterm_selection_target_cut7_byte __uint__dansi_xterm_selection_target_cut7_byte

typedef union dansi_xterm_selection_Targets {
    struct {
        u16 clipboard : 1;
        u16 primary   : 1;
        u16 secondary : 1;
        u16 select    : 1;
        u16 cut0      : 1;
        u16 cut1      : 1;
        u16 cut2      : 1;
        u16 cut3      : 1;
        u16 cut4      : 1;
        u16 cut5      : 1;
        u16 cut6      : 1;
        u16 cut7      : 1;
        u16 reserved_ : 4;
    };
    u16 packed;
} dansi_xterm_selection_Targets;
T_use_prl$(dansi_xterm_selection_Targets);

$attr($must_check)
$extern fn_((dansi_xterm_selection_targetsToRaw(
    dansi_xterm_selection_Targets targets, S$u8 buf
))(E$S$u8));
$attr($must_check)
$extern fn_((dansi_xterm_selection_set(
    dansi_xterm_selection_Targets targets, S_const$u8 data_base64, S$u8 buf
))(E$S$u8));
$attr($must_check)
$extern fn_((dansi_xterm_selection_setWrite(
    dansi_xterm_selection_Targets targets, S_const$u8 data_base64, io_Writer out
))(io_PrintE$void));
$attr($must_check)
$extern fn_((dansi_xterm_selection_query(
    dansi_xterm_selection_Targets targets, S$u8 buf
))(E$S$u8));
$attr($must_check)
$extern fn_((dansi_xterm_selection_queryWrite(
    dansi_xterm_selection_Targets targets, io_Writer out
))(io_PrintE$void));
$attr($must_check)
$extern fn_((dansi_xterm_selection_clear(
    dansi_xterm_selection_Targets targets, S$u8 buf
))(E$S$u8));
$attr($must_check)
$extern fn_((dansi_xterm_selection_clearWrite(
    dansi_xterm_selection_Targets targets, io_Writer out
))(io_PrintE$void));

#define dansi_xterm_selection_setRaw_static(_targets_tok, _data_base64_tok) \
    ____dansi_xterm_selection_setRaw_static(_targets_tok, _data_base64_tok)
$attr($must_check)
$extern fn_((dansi_xterm_selection_setRaw(S_const$u8 targets, S_const$u8 data_base64, S$u8 buf))(E$S$u8));
$attr($must_check)
$extern fn_((dansi_xterm_selection_setRawWrite(S_const$u8 targets, S_const$u8 data_base64, io_Writer out))(io_PrintE$void));

#define dansi_xterm_selection_queryRaw_static(_targets_tok) \
    ____dansi_xterm_selection_queryRaw_static(_targets_tok)
$attr($must_check)
$extern fn_((dansi_xterm_selection_queryRaw(S_const$u8 targets, S$u8 buf))(E$S$u8));
$attr($must_check)
$extern fn_((dansi_xterm_selection_queryRawWrite(S_const$u8 targets, io_Writer out))(io_PrintE$void));

#define dansi_xterm_selection_clearRaw_static(_targets_tok) \
    ____dansi_xterm_selection_clearRaw_static(_targets_tok)
$attr($must_check)
$extern fn_((dansi_xterm_selection_clearRaw(S_const$u8 targets, S$u8 buf))(E$S$u8));
$attr($must_check)
$extern fn_((dansi_xterm_selection_clearRawWrite(S_const$u8 targets, io_Writer out))(io_PrintE$void));

/*========== Macros and Definitions =========================================*/

#define __str__dansi_xterm_selection_cmd "52"
#define __uint__dansi_xterm_selection_cmd_u16 52
#define __str__dansi_xterm_selection_sep ";"
#define __uint__dansi_xterm_selection_sep_byte u8_c(';')
#define __str__dansi_xterm_selection_query_marker "?"
#define __uint__dansi_xterm_selection_query_marker_byte u8_c('?')
#define __uint__dansi_xterm_selection_target_count 12
#define __str__dansi_xterm_selection_target_clipboard "c"
#define __uint__dansi_xterm_selection_target_clipboard_byte u8_c('c')
#define __str__dansi_xterm_selection_target_primary "p"
#define __uint__dansi_xterm_selection_target_primary_byte u8_c('p')
#define __str__dansi_xterm_selection_target_secondary "q"
#define __uint__dansi_xterm_selection_target_secondary_byte u8_c('q')
#define __str__dansi_xterm_selection_target_select "s"
#define __uint__dansi_xterm_selection_target_select_byte u8_c('s')
#define __str__dansi_xterm_selection_target_cut0 "0"
#define __uint__dansi_xterm_selection_target_cut0_byte u8_c('0')
#define __str__dansi_xterm_selection_target_cut1 "1"
#define __uint__dansi_xterm_selection_target_cut1_byte u8_c('1')
#define __str__dansi_xterm_selection_target_cut2 "2"
#define __uint__dansi_xterm_selection_target_cut2_byte u8_c('2')
#define __str__dansi_xterm_selection_target_cut3 "3"
#define __uint__dansi_xterm_selection_target_cut3_byte u8_c('3')
#define __str__dansi_xterm_selection_target_cut4 "4"
#define __uint__dansi_xterm_selection_target_cut4_byte u8_c('4')
#define __str__dansi_xterm_selection_target_cut5 "5"
#define __uint__dansi_xterm_selection_target_cut5_byte u8_c('5')
#define __str__dansi_xterm_selection_target_cut6 "6"
#define __uint__dansi_xterm_selection_target_cut6_byte u8_c('6')
#define __str__dansi_xterm_selection_target_cut7 "7"
#define __uint__dansi_xterm_selection_target_cut7_byte u8_c('7')
#define ____dansi_xterm_selection_setRaw_static(_targets_tok, _data_base64_tok) \
    dansi_osc_make_static(dansi_xterm_selection_cmd, _targets_tok dansi_xterm_selection_sep _data_base64_tok)
#define ____dansi_xterm_selection_queryRaw_static(_targets_tok) \
    dansi_osc_make_static(dansi_xterm_selection_cmd, _targets_tok dansi_xterm_selection_sep dansi_xterm_selection_query_marker)
#define ____dansi_xterm_selection_clearRaw_static(_targets_tok) \
    dansi_osc_make_static(dansi_xterm_selection_cmd, _targets_tok dansi_xterm_selection_sep)

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* dansi_xterm_selection__included */

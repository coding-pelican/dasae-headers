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
))(E$void));
$attr($must_check)
$extern fn_((dansi_xterm_selection_query(
    dansi_xterm_selection_Targets targets, S$u8 buf
))(E$S$u8));
$attr($must_check)
$extern fn_((dansi_xterm_selection_queryWrite(
    dansi_xterm_selection_Targets targets, io_Writer out
))(E$void));
$attr($must_check)
$extern fn_((dansi_xterm_selection_clear(
    dansi_xterm_selection_Targets targets, S$u8 buf
))(E$S$u8));
$attr($must_check)
$extern fn_((dansi_xterm_selection_clearWrite(
    dansi_xterm_selection_Targets targets, io_Writer out
))(E$void));

#define dansi_xterm_selection_setRaw_static(_targets_tok, _data_base64_tok) \
    ____dansi_xterm_selection_setRaw_static(_targets_tok, _data_base64_tok)
$attr($must_check)
$extern fn_((dansi_xterm_selection_setRaw(S_const$u8 targets, S_const$u8 data_base64, S$u8 buf))(E$S$u8));
$attr($must_check)
$extern fn_((dansi_xterm_selection_setRawWrite(S_const$u8 targets, S_const$u8 data_base64, io_Writer out))(E$void));

#define dansi_xterm_selection_queryRaw_static(_targets_tok) \
    ____dansi_xterm_selection_queryRaw_static(_targets_tok)
$attr($must_check)
$extern fn_((dansi_xterm_selection_queryRaw(S_const$u8 targets, S$u8 buf))(E$S$u8));
$attr($must_check)
$extern fn_((dansi_xterm_selection_queryRawWrite(S_const$u8 targets, io_Writer out))(E$void));

#define dansi_xterm_selection_clearRaw_static(_targets_tok) \
    ____dansi_xterm_selection_clearRaw_static(_targets_tok)
$attr($must_check)
$extern fn_((dansi_xterm_selection_clearRaw(S_const$u8 targets, S$u8 buf))(E$S$u8));
$attr($must_check)
$extern fn_((dansi_xterm_selection_clearRawWrite(S_const$u8 targets, io_Writer out))(E$void));

/*========== Macros and Definitions =========================================*/

#define ____dansi_xterm_selection_setRaw_static(_targets_tok, _data_base64_tok) \
    dansi_osc_make_static("52", _targets_tok ";" _data_base64_tok)
#define ____dansi_xterm_selection_queryRaw_static(_targets_tok) \
    dansi_osc_make_static("52", _targets_tok ";?")
#define ____dansi_xterm_selection_clearRaw_static(_targets_tok) \
    dansi_osc_make_static("52", _targets_tok ";")

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* dansi_xterm_selection__included */

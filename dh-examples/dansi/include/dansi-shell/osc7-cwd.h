/**
 * @copyright Copyright (c) 2026 Gyeongtae Kim. All rights reserved.
 * @license   Proprietary and confidential. Unauthorized use, reproduction,
 *            or distribution is strictly prohibited.
 *
 * @file    osc7-cwd.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2026-06-18 (date of creation)
 * @updated 2026-06-18 (date of last update)
 * @ingroup dasae-headers-workspace(dh-workspace)/dansi
 * @prefix  dansi_shell_osc7
 */
#pragma once
#ifndef dansi_shell_osc7__included
#define dansi_shell_osc7__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "../dansi-core/osc.h"

/*========== Macros and Declarations ========================================*/

typedef struct dansi_shell_osc7_Cwd {
    var_(file_uri, S_const$u8);
} dansi_shell_osc7_Cwd;
T_use_prl$(dansi_shell_osc7_Cwd);

errset_((dansi_shell_osc7_E)(dansi_shell_osc7_Invalid));
T_use_E$($set(dansi_shell_osc7_E)(dansi_shell_osc7_Cwd));

$attr($must_check)
$extern fn_((dansi_shell_osc7_set(dansi_shell_osc7_Cwd cwd, S$u8 buf))(E$S$u8));
$attr($must_check)
$extern fn_((dansi_shell_osc7_setWrite(dansi_shell_osc7_Cwd cwd, io_Writer out))(E$void));
$attr($must_check)
$extern fn_((dansi_shell_osc7_setRaw(S_const$u8 file_uri, S$u8 buf))(E$S$u8));
$attr($must_check)
$extern fn_((dansi_shell_osc7_setRawWrite(S_const$u8 file_uri, io_Writer out))(E$void));
$attr($must_check)
$extern fn_((dansi_shell_osc7_parse(dansi_osc_Frame frame))(dansi_shell_osc7_E$dansi_shell_osc7_Cwd));

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* dansi_shell_osc7__included */

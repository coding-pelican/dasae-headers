/**
 * @copyright Copyright (c) 2026 Gyeongtae Kim. All rights reserved.
 * @license   Proprietary and confidential. Unauthorized use, reproduction,
 *            or distribution is strictly prohibited.
 *
 * @file    osc633-vscode.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2026-06-18 (date of creation)
 * @updated 2026-06-18 (date of last update)
 * @ingroup dasae-headers-workspace(dh-workspace)/dansi
 * @prefix  dansi_shell_osc633
 */
#pragma once
#ifndef dansi_shell_osc633__included
#define dansi_shell_osc633__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "../dansi-core/osc.h"

/*========== Macros and Declarations ========================================*/

typedef enum_((dansi_shell_osc633_Mark $fits($packed))(
    dansi_shell_osc633_Mark_prompt_start = 'A',
    dansi_shell_osc633_Mark_prompt_end = 'B',
    dansi_shell_osc633_Mark_command_start = 'C'
)) dansi_shell_osc633_Mark;
claim_assert_static(eqlType$(dansi_shell_osc633_Mark, u8));
T_use_prl$(dansi_shell_osc633_Mark);

typedef struct dansi_shell_osc633_CmdEnd {
    var_(exit_code, O$i32);
} dansi_shell_osc633_CmdEnd;
T_use_prl$(dansi_shell_osc633_CmdEnd);

typedef struct dansi_shell_osc633_CmdLine {
    var_(command_line, S_const$u8);
} dansi_shell_osc633_CmdLine;
T_use_prl$(dansi_shell_osc633_CmdLine);

typedef struct dansi_shell_osc633_Prop {
    var_(key, S_const$u8);
    var_(value, S_const$u8);
} dansi_shell_osc633_Prop;
T_use_prl$(dansi_shell_osc633_Prop);

typedef variant_((dansi_shell_osc633_Frame $fits($packed))(
    (dansi_shell_osc633_Frame_prompt_start, Void),
    (dansi_shell_osc633_Frame_prompt_end, Void),
    (dansi_shell_osc633_Frame_command_start, Void),
    (dansi_shell_osc633_Frame_command_end, dansi_shell_osc633_CmdEnd),
    (dansi_shell_osc633_Frame_command_line, dansi_shell_osc633_CmdLine),
    (dansi_shell_osc633_Frame_property, dansi_shell_osc633_Prop)
)) dansi_shell_osc633_Frame;
claim_assert_static(eqlType$(FieldType$(dansi_shell_osc633_Frame, tag), u8));
T_use_prl$(dansi_shell_osc633_Frame);

errset_((dansi_shell_osc633_E)(dansi_shell_osc633_Invalid));
T_use_E$($set(dansi_shell_osc633_E)(dansi_shell_osc633_Frame));

$attr($must_check)
$extern fn_((dansi_shell_osc633_mark(dansi_shell_osc633_Mark mark, S$u8 buf))(E$S$u8));
$attr($must_check)
$extern fn_((dansi_shell_osc633_markWrite(dansi_shell_osc633_Mark mark, io_Writer out))(E$void));
$attr($must_check)
$extern fn_((dansi_shell_osc633_commandEnd(O$i32 exit_code, S$u8 buf))(E$S$u8));
$attr($must_check)
$extern fn_((dansi_shell_osc633_commandEndWrite(O$i32 exit_code, io_Writer out))(E$void));
$attr($must_check)
$extern fn_((dansi_shell_osc633_commandLine(S_const$u8 command_line, S$u8 buf))(E$S$u8));
$attr($must_check)
$extern fn_((dansi_shell_osc633_commandLineWrite(S_const$u8 command_line, io_Writer out))(E$void));
$attr($must_check)
$extern fn_((dansi_shell_osc633_commandLineRaw(S_const$u8 command_line, S$u8 buf))(E$S$u8));
$attr($must_check)
$extern fn_((dansi_shell_osc633_commandLineRawWrite(S_const$u8 command_line, io_Writer out))(E$void));
$attr($must_check)
$extern fn_((dansi_shell_osc633_property(dansi_shell_osc633_Prop property, S$u8 buf))(E$S$u8));
$attr($must_check)
$extern fn_((dansi_shell_osc633_propertyWrite(dansi_shell_osc633_Prop property, io_Writer out))(E$void));
$attr($must_check)
$extern fn_((dansi_shell_osc633_propertyRaw(S_const$u8 property, S$u8 buf))(E$S$u8));
$attr($must_check)
$extern fn_((dansi_shell_osc633_propertyRawWrite(S_const$u8 property, io_Writer out))(E$void));
$attr($must_check)
$extern fn_((dansi_shell_osc633_parse(dansi_osc_Frame frame))(dansi_shell_osc633_E$dansi_shell_osc633_Frame));

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* dansi_shell_osc633__included */

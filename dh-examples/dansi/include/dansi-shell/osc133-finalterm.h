/**
 * @copyright Copyright (c) 2026 Gyeongtae Kim. All rights reserved.
 * @license   Proprietary and confidential. Unauthorized use, reproduction,
 *            or distribution is strictly prohibited.
 *
 * @file    osc133-finalterm.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2026-06-18 (date of creation)
 * @updated 2026-06-18 (date of last update)
 * @ingroup dasae-headers-workspace(dh-workspace)/dansi
 * @prefix  dansi_shell_osc133
 */
#pragma once
#ifndef dansi_shell_osc133__included
#define dansi_shell_osc133__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "../dansi-core/osc.h"

/*========== Macros and Declarations ========================================*/

typedef enum_((dansi_shell_osc133_Mark $fits($packed))(
    dansi_shell_osc133_Mark_prompt_start = 'A',
    dansi_shell_osc133_Mark_prompt_end = 'B',
    dansi_shell_osc133_Mark_command_start = 'C',
    dansi_shell_osc133_Mark_command_end = 'D'
)) dansi_shell_osc133_Mark;
claim_assert_static(eqlType$(dansi_shell_osc133_Mark, u8));
T_use_prl$(dansi_shell_osc133_Mark);

typedef struct dansi_shell_osc133_CmdEnd {
    var_(exit_code, O$i32);
} dansi_shell_osc133_CmdEnd;
T_use_prl$(dansi_shell_osc133_CmdEnd);

typedef variant_((dansi_shell_osc133_Frame $fits($packed))(
    (dansi_shell_osc133_Frame_prompt_start, Void),
    (dansi_shell_osc133_Frame_prompt_end, Void),
    (dansi_shell_osc133_Frame_command_start, Void),
    (dansi_shell_osc133_Frame_command_end, dansi_shell_osc133_CmdEnd)
)) dansi_shell_osc133_Frame;
claim_assert_static(eqlType$(FieldType$(dansi_shell_osc133_Frame, tag), u8));
T_use_prl$(dansi_shell_osc133_Frame);

errset_((dansi_shell_osc133_E)(dansi_shell_osc133_Invalid));
T_use_E$($set(dansi_shell_osc133_E)(dansi_shell_osc133_Frame));

$attr($must_check)
$extern fn_((dansi_shell_osc133_mark(dansi_shell_osc133_Mark mark, S$u8 buf))(E$S$u8));
$attr($must_check)
$extern fn_((dansi_shell_osc133_markWrite(dansi_shell_osc133_Mark mark, io_Writer out))(E$void));
$attr($must_check)
$extern fn_((dansi_shell_osc133_commandEnd(O$i32 exit_code, S$u8 buf))(E$S$u8));
$attr($must_check)
$extern fn_((dansi_shell_osc133_commandEndWrite(O$i32 exit_code, io_Writer out))(E$void));
$attr($must_check)
$extern fn_((dansi_shell_osc133_parse(dansi_osc_Frame frame))(dansi_shell_osc133_E$dansi_shell_osc133_Frame));

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* dansi_shell_osc133__included */

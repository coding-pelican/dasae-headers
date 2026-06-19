/**
 * @copyright Copyright (c) 2026 Gyeongtae Kim. All rights reserved.
 * @license   Proprietary and confidential. Unauthorized use, reproduction,
 *            or distribution is strictly prohibited.
 *
 * @file    pm.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2026-06-18 (date of creation)
 * @updated 2026-06-18 (date of last update)
 * @ingroup dasae-headers-workspace(dh-workspace)/dansi
 * @prefix  dansi_pm
 */
#pragma once
#ifndef dansi_pm__included
#define dansi_pm__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "Seq.h"

/*========== Macros and Declarations ========================================*/

errset_((dansi_pm_E)(dansi_pm_Invalid));

typedef struct dansi_pm_Frame dansi_pm_Frame;
T_decl_E$($set(dansi_pm_E)(dansi_pm_Frame));
$attr($must_check)
$extern fn_((dansi_pm_parse(S_const$u8 bytes))(dansi_pm_E$dansi_pm_Frame));
#define dansi_pm_make_static(_payload_tok) \
    ____dansi_pm_make_static(_payload_tok)
$attr($must_check)
$extern fn_((dansi_pm_make(S_const$u8 payload, S$u8 buf))(E$S$u8));
$attr($must_check)
$extern fn_((dansi_pm_write(S_const$u8 payload, io_Writer out))(E$void));
$attr($must_check)
$extern fn_((dansi_pm_makeWithEOS(S_const$u8 payload, dansi_Seq_EOS eos, S$u8 buf))(E$S$u8));
$attr($must_check)
$extern fn_((dansi_pm_writeWithEOS(S_const$u8 payload, dansi_Seq_EOS eos, io_Writer out))(E$void));

struct dansi_pm_Frame {
    var_(payload, S_const$u8);
    var_(terminator, dansi_Seq_EOS);
    var_(bytes, S_const$u8);
};
T_use_prl$(dansi_pm_Frame);
T_impl_E$($set(dansi_pm_E)(dansi_pm_Frame));

/*========== Macros and Definitions =========================================*/

#define ____dansi_pm_make_static(_payload_tok) \
    "\x1b^" _payload_tok "\x1b\\"

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* dansi_pm__included */

/**
 * @copyright Copyright (c) 2026 Gyeongtae Kim. All rights reserved.
 * @license   Proprietary and confidential. Unauthorized use, reproduction,
 *            or distribution is strictly prohibited.
 *
 * @file    apc.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2026-06-18 (date of creation)
 * @updated 2026-06-18 (date of last update)
 * @ingroup dasae-headers-workspace(dh-workspace)/dansi
 * @prefix  dansi_apc
 */
#pragma once
#ifndef dansi_apc__included
#define dansi_apc__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "Seq.h"

/*========== Macros and Declarations ========================================*/

errset_((dansi_apc_E)(dansi_apc_Invalid));

typedef struct dansi_apc_Frame dansi_apc_Frame;
T_decl_E$($set(dansi_apc_E)(dansi_apc_Frame));
$attr($must_check)
$extern fn_((dansi_apc_parse(S_const$u8 bytes))(dansi_apc_E$dansi_apc_Frame));
#define dansi_apc_make_static(_payload_tok) \
    ____dansi_apc_make_static(_payload_tok)
$attr($must_check)
$extern fn_((dansi_apc_make(S_const$u8 payload, S$u8 buf))(E$S$u8));
$attr($must_check)
$extern fn_((dansi_apc_write(S_const$u8 payload, io_Writer out))(E$void));
$attr($must_check)
$extern fn_((dansi_apc_makeWithEOS(S_const$u8 payload, dansi_Seq_EOS eos, S$u8 buf))(E$S$u8));
$attr($must_check)
$extern fn_((dansi_apc_writeWithEOS(S_const$u8 payload, dansi_Seq_EOS eos, io_Writer out))(E$void));

struct dansi_apc_Frame {
    var_(payload, S_const$u8);
    var_(terminator, dansi_Seq_EOS);
    var_(bytes, S_const$u8);
};
T_use_prl$(dansi_apc_Frame);
T_impl_E$($set(dansi_apc_E)(dansi_apc_Frame));

/*========== Macros and Definitions =========================================*/

#define ____dansi_apc_make_static(_payload_tok) \
    "\x1b_" _payload_tok "\x1b\\"

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* dansi_apc__included */

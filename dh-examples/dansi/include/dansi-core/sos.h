/**
 * @copyright Copyright (c) 2026 Gyeongtae Kim. All rights reserved.
 * @license   Proprietary and confidential. Unauthorized use, reproduction,
 *            or distribution is strictly prohibited.
 *
 * @file    sos.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2026-06-18 (date of creation)
 * @updated 2026-06-18 (date of last update)
 * @ingroup dasae-headers-workspace(dh-workspace)/dansi
 * @prefix  dansi_sos
 */
#pragma once
#ifndef dansi_sos__included
#define dansi_sos__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "Seq.h"

/*========== Macros and Declarations ========================================*/

errset_((dansi_sos_E)(dansi_sos_Invalid));

typedef struct dansi_sos_Frame dansi_sos_Frame;
T_decl_E$($set(dansi_sos_E)(dansi_sos_Frame));
$attr($must_check)
$extern fn_((dansi_sos_parse(S_const$u8 bytes))(dansi_sos_E$dansi_sos_Frame));
#define dansi_sos_make_static(_payload_tok) \
    ____dansi_sos_make_static(_payload_tok)
$attr($must_check)
$extern fn_((dansi_sos_make(S_const$u8 payload, S$u8 buf))(E$S$u8));
$attr($must_check)
$extern fn_((dansi_sos_write(S_const$u8 payload, io_Writer out))(E$void));
$attr($must_check)
$extern fn_((dansi_sos_makeWithEOS(S_const$u8 payload, dansi_Seq_EOS eos, S$u8 buf))(E$S$u8));
$attr($must_check)
$extern fn_((dansi_sos_writeWithEOS(S_const$u8 payload, dansi_Seq_EOS eos, io_Writer out))(E$void));

struct dansi_sos_Frame {
    var_(payload, S_const$u8);
    var_(terminator, dansi_Seq_EOS);
    var_(bytes, S_const$u8);
};
T_use_prl$(dansi_sos_Frame);
T_impl_E$($set(dansi_sos_E)(dansi_sos_Frame));

/*========== Macros and Definitions =========================================*/

#define ____dansi_sos_make_static(_payload_tok) \
    "\x1bX" _payload_tok "\x1b\\"

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* dansi_sos__included */

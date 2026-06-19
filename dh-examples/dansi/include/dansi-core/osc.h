/**
 * @copyright Copyright (c) 2026 Gyeongtae Kim. All rights reserved.
 * @license   Proprietary and confidential. Unauthorized use, reproduction,
 *            or distribution is strictly prohibited.
 *
 * @file    osc.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2026-06-18 (date of creation)
 * @updated 2026-06-18 (date of last update)
 * @ingroup dasae-headers-workspace(dh-workspace)/dansi
 * @prefix  dansi_osc
 */
#pragma once
#ifndef dansi_osc__included
#define dansi_osc__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "Seq.h"

/*========== Macros and Declarations ========================================*/

errset_((dansi_osc_E)(dansi_osc_Invalid));

typedef struct dansi_osc_Frame dansi_osc_Frame;
T_decl_E$($set(dansi_osc_E)(dansi_osc_Frame));

$attr($must_check)
$extern fn_((dansi_osc_parse(S_const$u8 bytes))(dansi_osc_E$dansi_osc_Frame));

#define dansi_osc_makeRaw_static(_payload_tok) \
    ____dansi_osc_makeRaw_static(_payload_tok)
#define dansi_osc_makeRawBEL_static(_payload_tok) \
    ____dansi_osc_makeRawBEL_static(_payload_tok)
$attr($must_check)
$extern fn_((dansi_osc_makeRaw(S_const$u8 payload, S$u8 buf))(E$S$u8));
$attr($must_check)
$extern fn_((dansi_osc_writeRaw(S_const$u8 payload, io_Writer out))(E$void));
$attr($must_check)
$extern fn_((dansi_osc_makeRawWithEOS(S_const$u8 payload, dansi_Seq_EOS eos, S$u8 buf))(E$S$u8));
$attr($must_check)
$extern fn_((dansi_osc_writeRawWithEOS(S_const$u8 payload, dansi_Seq_EOS eos, io_Writer out))(E$void));

#define dansi_osc_make_static(_cmd_tok, _payload_tok) \
    ____dansi_osc_make_static(_cmd_tok, _payload_tok)
#define dansi_osc_makeBEL_static(_cmd_tok, _payload_tok) \
    ____dansi_osc_makeBEL_static(_cmd_tok, _payload_tok)
$attr($must_check)
$extern fn_((dansi_osc_make(u16 cmd, S_const$u8 payload, S$u8 buf))(E$S$u8));
$attr($must_check)
$extern fn_((dansi_osc_write(u16 cmd, S_const$u8 payload, io_Writer out))(E$void));
$attr($must_check)
$extern fn_((dansi_osc_makeWithEOS(u16 cmd, S_const$u8 payload, dansi_Seq_EOS eos, S$u8 buf))(E$S$u8));
$attr($must_check)
$extern fn_((dansi_osc_writeWithEOS(u16 cmd, S_const$u8 payload, dansi_Seq_EOS eos, io_Writer out))(E$void));

struct dansi_osc_Frame {
    var_(payload, S_const$u8);
    var_(terminator, dansi_Seq_EOS);
    var_(bytes, S_const$u8);
};
T_use_prl$(dansi_osc_Frame);
T_impl_E$($set(dansi_osc_E)(dansi_osc_Frame));

typedef struct dansi_osc_CmdSplit dansi_osc_CmdSplit;
T_decl_prl$(dansi_osc_CmdSplit);
$extern fn_((dansi_osc_Frame_splitCmd(dansi_osc_Frame self))(O$dansi_osc_CmdSplit));

struct dansi_osc_CmdSplit {
    var_(cmd, S_const$u8);
    var_(payload, S_const$u8);
};
T_impl_prl$(dansi_osc_CmdSplit);
$extern fn_((dansi_osc_CmdSplit_cmdAsU16(dansi_osc_CmdSplit self))(O$u16));

/*========== Macros and Definitions =========================================*/

#define ____dansi_osc_makeRaw_static(_payload_tok) \
    "\x1b]" _payload_tok "\x1b\\"
#define ____dansi_osc_makeRawBEL_static(_payload_tok) \
    "\x1b]" _payload_tok "\x07"
#define ____dansi_osc_make_static(_cmd_tok, _payload_tok) \
    dansi_osc_makeRaw_static(_cmd_tok ";" _payload_tok)
#define ____dansi_osc_makeBEL_static(_cmd_tok, _payload_tok) \
    dansi_osc_makeRawBEL_static(_cmd_tok ";" _payload_tok)

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* dansi_osc__included */

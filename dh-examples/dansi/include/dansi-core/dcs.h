/**
 * @copyright Copyright (c) 2026 Gyeongtae Kim. All rights reserved.
 * @license   Proprietary and confidential. Unauthorized use, reproduction,
 *            or distribution is strictly prohibited.
 *
 * @file    dcs.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2026-06-18 (date of creation)
 * @updated 2026-06-18 (date of last update)
 * @ingroup dasae-headers-workspace(dh-workspace)/dansi
 * @prefix  dansi_dcs
 */
#pragma once
#ifndef dansi_dcs__included
#define dansi_dcs__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "Seq.h"

/*========== Macros and Declarations ========================================*/

#define dansi_dcs_7bit_prefix __str__dansi_dcs_7bit_prefix
#define dansi_dcs_7bit_intro __str__dansi_dcs_7bit_intro
#define dansi_dcs_7bit_intro_byte __uint__dansi_dcs_7bit_intro_byte
#define dansi_dcs_8bit_intro __str__dansi_dcs_8bit_intro
#define dansi_dcs_8bit_intro_byte __uint__dansi_dcs_8bit_intro_byte

errset_((dansi_dcs_E)(dansi_dcs_Invalid));

typedef struct dansi_dcs_Frame dansi_dcs_Frame;
T_decl_E$($set(dansi_dcs_E)(dansi_dcs_Frame));

$attr($must_check)
$extern fn_((dansi_dcs_parse(S_const$u8 bytes))(dansi_dcs_E$dansi_dcs_Frame));

#define dansi_dcs_makeRaw_static(_payload_tok) \
    ____dansi_dcs_makeRaw_static(_payload_tok)
$attr($must_check)
$extern fn_((dansi_dcs_makeRaw(S_const$u8 payload, S$u8 buf))(E$S$u8));
$attr($must_check)
$extern fn_((dansi_dcs_writeRaw(S_const$u8 payload, io_Writer out))(io_PrintE$void));
$attr($must_check)
$extern fn_((dansi_dcs_makeRawWithEOS(S_const$u8 payload, dansi_Seq_EOS eos, S$u8 buf))(E$S$u8));
$attr($must_check)
$extern fn_((dansi_dcs_writeRawWithEOS(S_const$u8 payload, dansi_Seq_EOS eos, io_Writer out))(io_PrintE$void));

#define dansi_dcs_make_static(_params_tok, _intermediates_tok, _final_tok, _payload_tok) \
    ____dansi_dcs_make_static(_params_tok, _intermediates_tok, _final_tok, _payload_tok)
$attr($must_check)
$extern fn_((dansi_dcs_make(
    S_const$u8 params, S_const$u8 intermediates, u8 final,
    S_const$u8 payload, S$u8 buf
))(E$S$u8));
$attr($must_check)
$extern fn_((dansi_dcs_write(
    S_const$u8 params, S_const$u8 intermediates, u8 final,
    S_const$u8 payload, io_Writer out
))(io_PrintE$void));
$attr($must_check)
$extern fn_((dansi_dcs_makeWithEOS(
    S_const$u8 params, S_const$u8 intermediates, u8 final,
    S_const$u8 payload, dansi_Seq_EOS eos, S$u8 buf
))(E$S$u8));
$attr($must_check)
$extern fn_((dansi_dcs_writeWithEOS(
    S_const$u8 params, S_const$u8 intermediates, u8 final,
    S_const$u8 payload, dansi_Seq_EOS eos, io_Writer out
))(io_PrintE$void));

struct dansi_dcs_Frame {
    var_(params, S_const$u8);
    var_(intermediates, S_const$u8);
    var_(final, u8);
    var_(payload, S_const$u8);
    var_(terminator, dansi_Seq_EOS);
    var_(bytes, S_const$u8);
};
T_use_prl$(dansi_dcs_Frame);
T_impl_E$($set(dansi_dcs_E)(dansi_dcs_Frame));

/*========== Macros and Definitions =========================================*/

#define __str__dansi_dcs_7bit_prefix dansi_Seq_esc dansi_dcs_7bit_intro
#define __str__dansi_dcs_7bit_intro "P"
#define __uint__dansi_dcs_7bit_intro_byte u8_c('P')
#define __str__dansi_dcs_8bit_intro "\x90"
#define __uint__dansi_dcs_8bit_intro_byte 0x90
#define ____dansi_dcs_makeRaw_static(_payload_tok) \
    dansi_dcs_7bit_prefix _payload_tok dansi_Seq_st_7bit
#define ____dansi_dcs_make_static(_params_tok, _intermediates_tok, _final_tok, _payload_tok) \
    dansi_dcs_7bit_prefix _params_tok _intermediates_tok _final_tok _payload_tok dansi_Seq_st_7bit

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* dansi_dcs__included */

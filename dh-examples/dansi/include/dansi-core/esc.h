/**
 * @copyright Copyright (c) 2026 Gyeongtae Kim. All rights reserved.
 * @license   Proprietary and confidential. Unauthorized use, reproduction,
 *            or distribution is strictly prohibited.
 *
 * @file    esc.h
 * @author  Gyeongtae Kim (dev-dasae) <codingpelican@gmail.com>
 * @date    2026-06-18 (date of creation)
 * @updated 2026-06-18 (date of last update)
 * @ingroup dasae-headers-workspace(dh-workspace)/dansi
 * @prefix  dansi_esc
 */
#pragma once
#ifndef dansi_esc__included
#define dansi_esc__included 1
#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

/*========== Includes =======================================================*/

#include "Seq.h"

/*========== Macros and Declarations ========================================*/

#define dansi_esc_7bit_prefix __str__dansi_esc_7bit_prefix
#define dansi_esc_intermediate_min_byte __uint__dansi_esc_intermediate_min_byte
#define dansi_esc_intermediate_max_byte __uint__dansi_esc_intermediate_max_byte
#define dansi_esc_final_min_byte __uint__dansi_esc_final_min_byte
#define dansi_esc_final_max_byte __uint__dansi_esc_final_max_byte

errset_((dansi_esc_E)(dansi_esc_Invalid));

typedef struct dansi_esc_Frame dansi_esc_Frame;
T_decl_E$($set(dansi_esc_E)(dansi_esc_Frame));

$attr($must_check)
$extern fn_((dansi_esc_parse(S_const$u8 bytes))(dansi_esc_E$dansi_esc_Frame));
#define dansi_esc_make_static(_intermediates_tok, _final_tok) \
    ____dansi_esc_make_static(_intermediates_tok, _final_tok)
$attr($must_check)
$extern fn_((dansi_esc_make(S_const$u8 intermediates, u8 final, S$u8 buf))(E$S$u8));
$attr($must_check)
$extern fn_((dansi_esc_write(S_const$u8 intermediates, u8 final, io_Writer out))(io_PrintE$void));

struct dansi_esc_Frame {
    var_(intermediates, S_const$u8);
    var_(final, u8);
    var_(bytes, S_const$u8);
};
T_use_prl$(dansi_esc_Frame);
T_impl_E$($set(dansi_esc_E)(dansi_esc_Frame));

/*========== Macros and Definitions =========================================*/

#define __str__dansi_esc_7bit_prefix dansi_Seq_esc
#define __uint__dansi_esc_intermediate_min_byte 0x20
#define __uint__dansi_esc_intermediate_max_byte 0x2f
#define __uint__dansi_esc_final_min_byte 0x30
#define __uint__dansi_esc_final_max_byte 0x7e
#define ____dansi_esc_make_static(_intermediates_tok, _final_tok) \
    dansi_esc_7bit_prefix _intermediates_tok _final_tok

#if defined(__cplusplus)
} /* extern "C" */
#endif /* defined(__cplusplus) */
#endif /* dansi_esc__included */
